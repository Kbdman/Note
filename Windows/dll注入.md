# DLL 注入
## 通过CreateRemoteThread注入
1. OpenProcess打开目标进程
2. VirtualAllocEx,在目标进程中申请空间
3. writeProcessMemory 将要注入的动态库名写入目标进程控件
4. 通过GetProcAddress 获取LoadLibrary的地址
5. 调用createRemoteThread(loadlibrary,名称地址)

>由于是在目标进程中创建线程，线程的起始地址和参数地址都需要是在目标进程的地址空间中

## 通过NtCreateThread来注入

注入过程与CreateRemoteThread类似
不过NtCreateThread的地址需要去ntdll.dll中取

## 通过QueueUserAPC
1. 遇上先相似，打开目标进程，申请空间，写入动态库名称
2. 通过GetProcAddress获取LoadLibrary的地址
3. 创建线程快照，找到目标进程的一个线程
4. OpenThread打开线程获得句柄
5. 通过线程句柄，调用QueueUserApc，LoadLibrary的地址和动态库名的地址作为参数，实现在apc时完成注入
### APC
异步过程调用
APC是与线程相关的，执行在线程的上下文中， 当一个线程被唤醒时，会检查自己有没有Apc，有Apc执行APC，处理完APC再执行自身的代码


## 通过shellcode注入DLL
1. 写好一个shellcode,shellcode里调用loadlibrary加载目标动态库，然后跳转回原地址     
在编写shellcode时动态库名称地址，loadlibrary地址以及跳转回的源地址都不确定，可以先随便用个地址占位，后期在确定地址后，在动态的修改
2. OpenProcess目标进程，virtualAllocEx在目标进程中申请两块空间，一块用于存放动态库名字，一块用于村妇shellcode
3. 将动态库地址写入目标进程
4. 通过快照找到一个目标进程的线程，OpenThread打开目标线程,suspendThread暂停目标线程.
5. 通过GetProcAddress获取LoadLibrary的地址
6. GetThreadContext获取目标线程的上下文，如ip
7. 将IP写入shellcode的返回地址位置，动态库名称地址，和loadlibrary地址也写入对应的位置
8. shellcode写进目标进程
9. 修改获取到的上下文的ip,使用setThreadContext设置目标线程的ctx,ResumeThread
10. 过一段时间将用作shellcode和动态库名称的内存释放掉

## SetWindowsHookEx
```
HHOOK SetWindowsHookExA(
  int       idHook,
  HOOKPROC  lpfn,
  HINSTANCE hmod,
  DWORD     dwThreadId
);
```
将模块hmod的方法lpfn加入到指定线程的idhook的hook链中，dwThreadId为0时为所有线程。hook成功hmod会被加载到每个hook的进程空间中

### 反射式注入
反射式注入要求被注入的目标进程的动态库有个自加载的使用位置无关代码的导出函数，这里称为反射加载函数
反射加载函数

#### 过程
1. 打开目标进程，将要注入的dll直接写入目标进程
2. 通过dll的导出表找到导出表反射加载函数在目标进程中的内存
3. 使用createremotethread调用反射加载函数,由反射函数完成注入   

反射函数的过程
1. 获取文件头的位置，可以通过caller()获得ip网上遍历寻找'MZ',或者由createRemoteThread时传入
2. 由于反射函数钥匙位置无关代码，要使用的相关库函数都要通过PEB->ldr中遍历动态库获取库函数的地址
3. 根据sizeofimage申请展开用的空间
4. 拷贝文件头
5. 根据节表展开节
6. 遍历更新导入表
7. 更新重定位数据
8. 调用oep 
