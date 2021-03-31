# Tips
## 抓取系统快照
CreateToolhelp32Snapshot 可以快照，可以通过参数来设置抓快照的类型
+ TH32CS_INHERIT(0x80000000)        
 声明快照句柄是可继承的。
+ TH32CS_SNAPALL        
在快照中包含系统中所有的进程和线程。
+ TH32CS_SNAPHEAPLIST(0x00000001)       
在快照中包含在th32ProcessID中指定的进程的所有的堆。
+ TH32CS_SNAPMODULE(0x00000008)     
在快照中包含在th32ProcessID中指定的进程的所有的模块。
+ TH32CS_SNAPPROCESS(0x00000002)        
在快照中包含系统中所有的进程。
+ TH32CS_SNAPTHREAD(0x00000004)         
在快照中包含系统中所有的线程。
+ TH32CS_SNAPALL        
(TH32CS_SNAPHEAPLIST | TH32CS_SNAPPROCESS | TH32CS_SNAPTHREAD | TH32CS_SNAPMODULE)

### 遍历快照信息
+ 遍历进程 Process32Next,Process32First
+ 遍历Module Module32Next,Module32First
+ 遍历线程 Thread32Next,Thread32First


## 进程权限调整
```
//请求一个可用于修改当前进程特权的Token
OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken);
//设置新的特权
tp.PrivilegeCount = 1;
tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &tp.Privileges[0].Luid);
//通过token修改特权
AdjustTokenPrivileges(hToken, FALSE, &tp, 0, NULL, NULL) ;

```

## 通过Process Monitor获取由依赖库不满足导致的loadlibrary失败原因的方法
LoadLibrary时，进程会打开被加载的动态库，再依次打开被加载的动态库依赖的动态库，如果存在找不到依赖，则LoadLibrary失败。
通过Process Monitor跟踪进程LoadLibrary返回前最后的几个打开动态库文件失败的记录，就可以得知无法满足的依赖


