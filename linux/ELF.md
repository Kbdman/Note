# ELF
## ELF Header
elf头处在文件的头部，描述文件的组织形式
成员:
```
#define EI_NIDENT        16
typedef struct {
    unsigned char  e_ident[EI_NIDENT]; //平台无关的信息，包括魔数等
    Elf32_Half     e_type;      //文件类型，可执行文件，共享库文件，可重定位文件，CoreDump文件
    Elf32_Half     e_machine;   //ISA
    Elf32_Word     e_version;
    Elf32_Addr     e_entry;     //入口点
    Elf32_Off      e_phoff;     //Program Header表的文件偏移
    Elf32_Off      e_shoff;     //Section Header表的文件偏移
    Elf32_Word     e_flags;
    Elf32_Half     e_ehsize;    //ELF头的大小
    Elf32_Half     e_phentsize; //Program Header表项的大小
    Elf32_Half     e_phnum;     //Program Header表项的数量
    Elf32_Half     e_shentsize; //Section Header表项的大小
    Elf32_Half     e_shnum;     //Section Header表项的数量
    Elf32_Half     e_shstrndx;  //Section表名称所在字符串表所在Section的索引
} Elf32_Ehdr; 
```
## Program Header Table
描述如何创建一个进程。每个可执行文件都需要有ProgramHeaderTable
```
    typedef struct {
        Elf32_Word    p_type;   //类型
        Elf32_Off     p_offset; //Segment的文件偏移
        Elf32_Addr    p_vaddr;  //要加在到的虚拟内存
        Elf32_Addr    p_paddr;
        Elf32_Word    p_filesz;  //文件中的大小
        Elf32_Word    p_memsz;   //内存中的大小
        Elf32_Word    p_flags;   //
        Elf32_Word    p_align;   //对齐
    } Elf32_Phdr;
```
### 类型
+ PT_NULL       
不使用的
+ PT_LOAD  
通过文p_filesz和p_memsz指定一个可以加载的段。文件中对应的数据将会被映射到内存中，如果内存大小大于文件大小，多出来的控件补0。文件大小不可以超过内存大小。在一个文件中可加载段可以有多个，以虚拟地址递增的顺序排列在ProgramHeader表中
+ PT_DYNAMIC  
动态链接信息
+ PT_INTERP     
程序解释器的位置，以'\0'结尾的字符串
+ PT_NOTE       
辅助信息的位置和大小 
+ PT_SHLIB      
预留的，未指定
+ PT_PHDR   
如果存在，指定了ProgramHeader表在文件和内存中的位置和大小。这个项在文件中不会出现一次以上。此外，只有程序头表需要是内存映像的一部分时，才有该项。如果有一个PT_PHDR项，它必须在一个可加载段之前。
## Section Header Table
描述文件分节的信息
```
typedef struct {
    Elf32_Word   sh_name;  //名字索引
    Elf32_Word   sh_type;  //Section的类型
    Elf32_Word   sh_flags; //属性，读写执行
    Elf32_Addr   sh_addr;  //加载到内存后的基地址
    Elf32_Off    sh_offset;//Section的文件偏移
    Elf32_Word   sh_size;  //大小
    Elf32_Word   sh_link;
    Elf32_Word   sh_info;
    Elf32_Word   sh_addralign;//内存对齐
    Elf32_Word   sh_entsize; //有些Section的内容是固定大小的Entry，这里保存了这些entry的大小
} Elf32_Shdr;
```
## 类型
+ SHT_SYMTAB and SHT_DYNSYM    
符号表，当前一个obj文件只有一个这种类型的Section，未来可能放开这个限制。SHT_SYMTAB通常为链接提供符号，它也可以用来动态链接。一个保存着动态连接符号的最小集信息的OBJ文件，可能也包含一个SHT_DYNSYM节。 
+ SHT_STRTAB        
字符串表
+ SHT_HASH    
参与动态连接的每个对象都必须包含一个符号hash表。当前一个obj文件只包含一个HASH表。  
+ SHT_DYNAMIC       
动态链接信息

### 字符串表
sh_type为SHT_STRTAB的Section为字符串表。    
字符串表的是一组以'\0'结尾的字符串依次连接而成。    
字符串通过其在字符串表中的地址偏移来引用
常见字符串表：
+ .shstrtab         
保存着Section名称的字符串表
+ .strtab   
保存字符串，保存着和符号表项关联名称。
### 符号表
在符号表的SectionHeader中，link表示符号表中字符串所在的字符串表所在Section的索引
sh_type为SHT_SYMTAB的Section是符号表
符号表项
```
typedef struct {
    Elf32_Word    st_name; //符号名称字符串索引
    Elf32_Addr    st_value;
    Elf32_Word    st_size;//符号相关数据的大小
    unsigned char st_info; //符号的类型如变量/函数/文件，符号的绑定类型如GLOBAL/LOCAL
    unsigned char st_other;
    Elf32_Half    st_shndx;
} Elf32_Sym;
```