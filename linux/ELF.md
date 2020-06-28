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
一个ProgramHeader表项可能对应这一个或者多个Section
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
    Elf32_Addr    st_value;//在可重定位文件中，保存这符号定义在所在节中的偏移，在可执行文件或者共享库文件中保存着VA
    Elf32_Word    st_size;//符号相关数据的大小
    unsigned char st_info; //符号的类型如变量/函数/文件，符号的绑定类型如GLOBAL/LOCAL
    unsigned char st_other;
    Elf32_Half    st_shndx;//符号定义在的Section索引，如果是外部符号，则为SHN_UNDEF
} Elf32_Sym;
```

## 动态链接
### 解释器
需要动态链接的程序都有一个PT_INTERP的ProgramHeader.执行时，系统从PT_INTERP中取出解释器的位置，以解释器构造进程。系统通常回将一个可执行文件的文件描述符交给解释器进程，或者将可执行文件映射进内存
### 动态链接器
在构建动态链接的程序时，链接器在程序头表中新增一个PT_INTERP项，以告知系统以动态链接器作为程序的解释器
可执行文件和动态链接器合作创建程序的进程，步骤:
+ 将可执行文件的内存段加入进程空间
+ 将共享库的内存段加入进程控件
+ 为可执行文件和共享库的内存映像进行重定位
+ 关闭用于读取可执行文件的文件描述符
+ 将控制权交还给程序

#### 动态段
一个需要动态链接的程序，会有一个PT_DYNAMIC ProgramHeader,它引用一个.dynamic section。存放这动态链接的信息
```
typedef struct {
    Elf32_Sword   d_tag;
    union {
        Elf32_Word    d_val;
        Elf32_Addr    d_ptr;
        } d_un;
        } Elf32_Dyn;
    extern Elf32_Dyn _DYNAMIC[];
```
d_tag指明了项的类型，根据类型的不同，d_un可能取d_val或者d_ptr

重要的dtag类型：
|类型|说明|
|---|---|
|DT_NULL|数组的结尾|
|DT_NEEDED|所需要的库的名称在字符串表中的索引，字符串表在DT_STRTAB项中指定,可以有多个，有顺序要求|
|DT_PLTRELSZ|plt相关重定位项的大小|
|DT_PLTGOT|plt或者got的地址|
|DT_HASH|符号hash表的地址|
|DT_STRTAB|动态链接相关字符串表地址|
|DT_STRSZ|字符串表大小|
|DT_SYMTAB|符号表地址|
|DT_SYMENT|符号表项大小|
|DT_RELA/DT_REL|重定位表地址|
|DT_RELASZ/DT_RELSZ|重定位表大小|
|DT_RELAENT/DT_RELENT|重定位表项大小|
|DT_INIT|init函数地址|
|DT_FINI|fini函数地址|
|DT_SONAME|共享库的名称|
|DT_RPATH|库目录搜索路径字符串偏移|
|DT_SYMBOLIC||
|DT_PLTREL|plt引用的重定位表项|
|DT_DEBUG|DEBUG相关|
|DT_TEXTREL||
|DT_JMPREL||
|DT_BIND_NOW|在将控制权交还给程序前完成所有重定位|

#### 动态库加载
动态链接器在加载可执行程序时，根据DT_NEEDED项指明的依赖，将依赖的动态库，以及依赖的依赖加载到内存中。
在解析符号时，动态链接器，首先检查当前可执行文件的符号表，如果没有则遍历所有直接依赖项的符号表，再没有则遍历依赖项的依赖项的符号表

库文件的搜索
如果文件名带有/则认为是路径
若文件名中不带有/:
1. 依次搜索DT_RPATH路径
2. 搜索LD_LIBRARY_PATH
3. /usr/lib

在完成依赖的动态库加载和重定位后，会依次执行每个文件的init代码，先调用被依赖的，再调用自身的。
在程序退出时会以init调用的反序调用fini代码
init和fini代码分别在.init和.fini Section中
