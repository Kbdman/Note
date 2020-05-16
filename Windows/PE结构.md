# PE结构
## 一些概念
### VA与RVA
VA是基于模块被加载到IMAGEBASE时的虚拟地址
RVA是相对于加载到内存基地址的偏移
VA在模块被加载到内存中时，需要被重定位

### 重定位
可执行文件中一些通过VA引用变量的代码，在模块被加载到内存中时，需要修改指针，来保证正确的修改
重定位表中保存着重定位时需要修改的数据的地址
重定位时，加载器遍历重定位表的每一项，将其引用的位置的数据减去原IMAGEBASE,然后加上现在模块的基地址
## 结构
### IMAGE_DOS_HEADER
重要成员:
+ e_magic       
魔术，字符形式位MZ，0x4D5A
+ e_lfanew  
IMAGE_NT_HEADER的文件偏移
### IMAGE_NT_HEADERS
NT_HEADER 主要分为SIGNATURE，FILE_HEADER和OPTIONAL_HEADER
####  SIGNATURE
0x4550 pE
#### IMAGE_FILE_HEADER
+ Machine;
平台类型
+ NumberOfSections;
分节数量
+ TimeDateStamp;
创建时间戳?
+ PointerToSymbolTable;
COFF文件符号表的文件偏移
+ NumberOfSymbols;
符号表数量
+ SizeOfOptionalHeader;
可选头大小
+ Characteristics;
可执行文件的属性
#### IMAGE_OPTIONAL_HEADER
+ Magic;        
魔数，区分32位还是64位
+ MajorLinkerVersion;
+ MinorLinkerVersion;
链接器版本？
+ SizeOfCode;
代码的总长度
+ SizeOfInitializedData;        
已初始化的数据长度
+ SizeOfUninitializedData;      
未初始化数据的长度
+ AddressOfEntryPoint;
PE VA
+ BaseOfCode;
代码段RVA
+ ImageBase;
建议加载的基地址VA,所有使用的VA都是基于这个VA的基础上
+ SectionAlignment;     
Section在加载到内存中时的对齐方案
+ FileAlignment;        
Section在文件中的对齐方案
+ MajorOperatingSystemVersio
+ MinorOperatingSystemVersio        
需要的最小操作版本号
+ MajorImageVersion;
+ MinorImageVersion;
+ MajorSubsystemVersion;
+ MinorSubsystemVersion;        
子系统版本号
+ Win32VersionValue;
+ SizeOfImage;      
占用虚拟地址的大小
+ SizeOfHeaders;
头大小？
+ CheckSum;     
文件校验和
+ Subsystem;        
子系统 控制台 GUI等
+ DllCharacteristics;
+ SizeOfStackReserve;
+ SizeOfStackCommit;
初始栈信息
+ SizeOfHeapReserve;
+ SizeOfHeapCommit;
初始堆信息
+ LoaderFlags;
+ NumberOfRvaAndSizes;      
数据目录的个数
+ IMAGE_DATA_DIRECTORY DataDirectory[IMAGE_NUMBEROF_DIRECTORY_ENTRIES];     
数据目录
### Section表
分节表
指定了每个分节的文件偏移，文件大小，RVA，加载到内存中的大小
### 数据目录
```
// Directory Entries

#define IMAGE_DIRECTORY_ENTRY_EXPORT          0   // 导出表
#define IMAGE_DIRECTORY_ENTRY_IMPORT          1   // 导入表
#define IMAGE_DIRECTORY_ENTRY_RESOURCE        2   // Resource Directory 资源表
#define IMAGE_DIRECTORY_ENTRY_EXCEPTION       3   // Exception Directory 异常表
#define IMAGE_DIRECTORY_ENTRY_SECURITY        4   // Security Directory 文件表
#define IMAGE_DIRECTORY_ENTRY_BASERELOC       5   // Base Relocation Table 重定向表
#define IMAGE_DIRECTORY_ENTRY_DEBUG           6   // Debug Directory
//      IMAGE_DIRECTORY_ENTRY_COPYRIGHT       7   // (X86 usage)
#define IMAGE_DIRECTORY_ENTRY_ARCHITECTURE    7   // Architecture Specific Data
#define IMAGE_DIRECTORY_ENTRY_GLOBALPTR       8   // RVA of GP
#define IMAGE_DIRECTORY_ENTRY_TLS             9   // TLS Directory  TLS表
#define IMAGE_DIRECTORY_ENTRY_LOAD_CONFIG    10   // Load Configuration Directory
#define IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT   11   // Bound Import Directory in headers 绑定导入表
#define IMAGE_DIRECTORY_ENTRY_IAT            12   // Import Address Table 导入地址表
#define IMAGE_DIRECTORY_ENTRY_DELAY_IMPORT   13   // Delay Load Import Descriptors 延迟导入表
#define IMAGE_DIRECTORY_ENTRY_COM_DESCRIPTOR 14   // COM Runtime descriptor

```
#### 导入表
导入表是一组IMAGE_IMPORT_DESCRIPTOR，一个IMAGE_IMPORT_DESCRIPTER代表着一个动态库的导入信息
```
typedef struct _IMAGE_IMPORT_DESCRIPTOR {
    union {
        DWORD   Characteristics;            // 0 表示为这是导入表的结束符
        DWORD   OriginalFirstThunk;         // 非0时为，原始的RVA RVA to original unbound IAT (PIMAGE_THUNK_DATA)
    } DUMMYUNIONNAME;
    DWORD   TimeDateStamp;                  // 0 if not bound,
                                            // -1 if bound, and real date\time stamp
                                            //     in IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT (new BIND)
                                            // O.W. date/time stamp of DLL bound to (Old BIND)
 
    DWORD   ForwarderChain;                 // -1 if no forwarders
    DWORD   Name;
    DWORD   FirstThunk;                     // RVA to IAT (if bound this IAT has actual addresses)
} IMAGE_IMPORT_DESCRIPTOR;
```
OriginalFirstThunk和FirstThunk，分别指向INT和IAT，INT和IAT都是IMAGE_THUNK_DATA的数组
```
typedef struct _IMAGE_THUNK_DATA32 {
    union {
        DWORD ForwarderString;      // PBYTE 
        DWORD Function;             // 函数地址
        DWORD Ordinal;              // 函数序号
        DWORD AddressOfData;        // 函数名字的RVA
    } u1;
} IMAGE_THUNK_DATA32;
```
在模块在加载到内存前，无论到时INT,还是IAT里都时保存着函数的名称的RVA，在模块被加载到内存后，依赖被加载后，IAT里的数据会被改成导入函数的地址。
>是序号导入还是名称导入，看Ordinal的最高位是否为1，为1则为序号

导入表名称项
```
typedef struct _IMAGE_IMPORT_BY_NAME {
    WORD    Hint; //序号
    CHAR   Name[1];//用于保存名字的边长数组
} IMAGE_IMPORT_BY_NAME
```
#### 导出表
```
typedef struct _IMAGE_EXPORT_DIRECTORY {
    DWORD   Characteristics;
    DWORD   TimeDateStamp;
    WORD    MajorVersion;
    WORD    MinorVersion;
    DWORD   Name; //模块名
    DWORD   Base;
    DWORD   NumberOfFunctions;
    DWORD   NumberOfNames;
    DWORD   AddressOfFunctions;     // RVA from base of image
    DWORD   AddressOfNames;         // RVA from base of image
    DWORD   AddressOfNameOrdinals;  // RVA from base of image
} IMAGE_EXPORT_DIRECTORY, *PIMAGE_EXPORT_DIRECTORY;
```
重要成员
+ Base      
导出函数的序号的基数
+ NumberOfFunctions     
导出的函数的个数
+ NumberOfNames     
按名字导出的函数的个数      
+ AddressOfFunctions        
导出函数的RVA表
+ AddressOfNames        
函数名字的RVA
+ AddressOfNameOrdinals     
名字对应的序号的数组

##### AddressOfFunctions，AddressOfNames以及AddressOfNameOrdinals的对应关系
1. AddressOfFunctions中保存着所有导出函数的地址,函数地址在表中的序号即为导出的序号(要加上Base)
2. AddressOfNames保存着所有通过名字导出的函数的名字的RVA数组
3. AddressOfNameOrdinals指向的数组中保存着名字到函数数组的映射关系，即名字数组中同序号的名字对应的函数在函数地址数组中的序号

#### 重定位
```
typedef struct _IMAGE_BASE_RELOCATION {
    DWORD   VirtualAddress;
    DWORD   SizeOfBlock;
//  WORD    TypeOffset[1];
} IMAGE_BASE_RELOCATION;
```
重定位表由一组IMAGE_BASE_RELOCATION组成,每个IMAGE_BASE_RELOCATION项，保存这一段数据中需要做重定位的信息，通常是1页，4k
+ VirtualAddress    
表示这个重定位信息所关注的数据块的RVA
+ SizeOfBlock       
当前这个IMAGE_BASE_RELOCATION的size,字节大小
+ TypeOffset        
type and offset ,高四位为类型,低四位为要重定位的数据在页内的偏移
IMAGE_BASE_RELOCATION数组以一个VirtualAddress为0的对象结束


