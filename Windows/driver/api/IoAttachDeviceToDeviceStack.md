# IoAttachDeviceToDeviceStack
IoAttachDeviceToDeviceStack例程将调用者的设备对象附加到设备对象栈的最顶层，并将之前的最顶层设备对象指针返回

```
PDEVICE_OBJECT IoAttachDeviceToDeviceStack(
  PDEVICE_OBJECT SourceDevice,
  PDEVICE_OBJECT TargetDevice
);
```
## 参数
+ SourceDevice      
调用者创建的设备对象指针

+ TargetDevice      
要附加到的设备栈中的设备对象指针，例如之前调用IoGetDeviceObjectPointer获得指针。
Pointer to another driver's device object, such as a pointer returned by a preceding call to IoGetDeviceObjectPointer.

## 返回值
IoAttachDeviceToDeviceStack 返回SourceDevice附加到的设备对象的指针。如果有其他驱动附加在目标设备上，返回的指针将不会是目标设备的指针。
如果没有附加成功，例如目标设备被卸载了，IoAttachDeviceToDeviceStack返回NULL.

## 备注
IoAttachDeviceToDeviceStack将驱动分层组织，这样同样一个IRP可以被发给这个链中的每一个驱动        
中间层驱动在初始化过程中调用这个例程。它允许驱动将其自己的设备附加到另一个设备上，这样任何发往目标设备的请求都会被先发往中间层驱动      
这个例程将SourceDevice的AlignmentRequirement设置成其下层设备对象的AlignmentRequirement，并将StackSize值设置成下层对象的StackSize+1.
驱动的编写者应当注意必须在任意必须在顶层的驱动附加之前调用这个例程。IoAttachDeviceToDeviceStack将SourceDevice附加到当前设备栈中最上层的设备对象上，并没有办法判断驱动是否是以正确的顺序分层的

通过调用IoGetDeviceObjectPointer来获取目标设备对象的指针的驱动在脱离附加前应当调用ObDereferenceObject来释放它对下层设备对象的引用

