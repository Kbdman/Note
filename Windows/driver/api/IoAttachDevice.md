# IoAttachDevice
The IoAttachDevice routine attaches the caller's device object to a named target device object, so that I/O requests bound for the target device are routed first to the caller.
IoAttachDevice例程将调用者指定的设备对象附加到一个命名的设备对象上，这样发往目标设备的I/O请求会被首先发往调用者
```
NTSTATUS IoAttachDevice(
  PDEVICE_OBJECT  SourceDevice,
  PUNICODE_STRING TargetDevice,
  PDEVICE_OBJECT  *AttachedDevice
);
```
## 参数
+ SourceDevice      
调用者创建的设备的指针

+ TargetDevice  
一个指向存有要附加到的设备的名称的缓冲区的指针

+ AttachedDevice
一个指向用户申请的存储空间的指针。如果附加成功，在返回时会保存着目标设备的指针

## 返回值
IoAttachDevice返回一个NTSTATUS值:

## 备注
IoAttachDevice 将驱动分层组织，这样同样一个IRP可以被发给这个链中的每一个驱动        
中间层驱动在初始化过程中调用这个例程。它允许驱动将其自己的设备附加到另一个设备上，这样任何发往目标设备的请求都会被先发往中间层驱动

调用者只会被放在已有分层驱动链的顶层。IoAttachDevice查找在目标设备之上的最高层的设备，然后附加在其上。因此，如果一个必须是顶层驱动的驱动，并且已经被附加在目标驱动上是，不能调用这个例程(Therefore, this routine must not be called if a driver that must be higher-level has already layered itself over the target device.)


注意，对于文件系统驱动和存储栈中的驱动，IoAttachDevice通过FILE_READ_ATTRIBUTES打开目标设备，然后调用IoGetRelatedDeiceObject.这不会导致文件系统被挂载。因此一个成功的IoAttachDevice调用会返回存储驱动的设备对象，而不是文件系统驱动的设备对象。

这个例程将SourceDevice的AlignmentRequirement设置成其下层设备对象的AlignmentRequirement，并将StackSize值设置成下层对象的StackSize+1.

>Warning  AttachedDevice必须指向一个全局内存位置，比如驱动的设备拓展。IoAttachDevice为目标设备打开文件对象，更新AttachedDevice,执行附加。然后关闭文件对象。因此，SourceDevice会在IoAttachDevice返回钱收到文件对象的IRP_MJ_CLEANUP和IRP_MJ_CLOSE请求。驱动必须将这些请求转发给目标设备，并且AttachedDevice必须是在DispatchCleanup和DispatchClose例程中可以访问的内存位置
