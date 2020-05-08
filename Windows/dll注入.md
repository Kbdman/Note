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