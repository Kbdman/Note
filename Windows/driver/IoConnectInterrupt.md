# IoConnectInterrupt

IoConnectInterrupt过程注册一个设备驱动的中断服务过程ISR，这样当设备的中断出现在任意cpu上时ISR都会被调用

## 参数
+ PKINTERRUPT       *InterruptObject        
驱动提供的用于存储中断对象的指针的空间，在后面调用KeSynchronizeExection时，这个指针必须被传入
+ PKSERVICE_ROUTINE ServiceRoutine      
ISR过程的入口点
+ PVOID             ServiceContext      
驱动决定的在调用ISR时传递给ISR的上下文指针，上下文的空间保持在物理内存中，比如设备拓展，驱动创建的控制器兑现古的控制器拓展，或者非分页池中。关于提供ISR上下文的更多信息见 https://docs.microsoft.com/zh-cn/windows-hardware/drivers/kernel/providing-isr-context-information
+ PKSPIN_LOCK       SpinLock        
一个驱动提供存储的已初始化的自旋锁指针，用于同步对一些其他驱动过程也会访问的数据的访问。当ISR处理不止一个向量或者驱动有多个ISR时，这个自旋锁时必须的。此外，驱动不需要为中断自旋锁申请空间，这个指针可以为空
+ ULONG             Vector      
指定了通过中断资源CM_PARTIAL_RESOURCE_DESCRIPTOR的成员u.Interrupt.Vector传递的中断向量
+ KIRQL             Irql        
指定了通过中断资源CM_PARTIAL_RESOURCE_DESCRIPTOR的成员u.Interrupt.Level传递的DIRQL值
+ KIRQL             SynchronizeIrql     
指定了ISR运行的DIRQL.如果这个ISR处理多个中断向量，或者驱动有多个ISR,这个值必须为通过u.Interrrupt.Level传递的IRQL的值中的最高值，否则Irql和SynchronizeIrql值一样
+ KINTERRUPT_MODE   InterruptMode       
LevelSensitive or Latched.
+ BOOLEAN           ShareVector     
Specifies whether the interrupt vector is sharable.
+ KAFFINITY         ProcessorEnableMask     
指定了一个代表了这个中断在当前平台上可以在那些处理器上出现的KAFFINITY值。可搁置通过中断资源的u.Interrupt.Affinity传递
+ BOOLEAN           FloatingSave
指定在设备中断时是否保存浮点栈,在基于X86和安腾的平台上，这必须为FALSE。关于保存浮点和MMX的更多信息见 https://docs.microsoft.com/windows-hardware/drivers/kernel/using-floating-point-or-mmx-in-a-wdm-driver
);


## 返回值
返回一个NTSTATUS

## 备注
新驱动应当使用IoConnectInterruptEx,它更容易使用。支持message-signaled 中断的设备驱动必须使用IoConnectInterruptEx

Pnp驱动应当在设备初始化时调用IoConnectInterrupt,在驱动完成PNP的 IRP_MN_START_DEVICE之前


When a driver receives an IRP_MN_START_DEVICE request, the driver receives raw and translated hardware resources in the Parameters.StartDevice.AllocatedResources and Parameters.StartDevice.AllocatedResourcesTranslated members of the IRP's IO_STACK_LOCATION structure, respectively. To connect its interrupt, the driver uses the resources at AllocatedResourcesTranslated.List.PartialResourceList.PartialDescriptors[]. The driver must scan the array of partial descriptors for resources of type CmResourceTypeInterrupt.

当一个驱动为自旋锁提供资源时，必须在将他传递给IoConnectInterrupt前调用KeInitializeSpinLock来进行初始化
If the driver supplies the storage for the SpinLock, it must call KeInitializeSpinLock before passing its interrupt spin lock to IoConnectInterrupt.

在从一个成功的IoConnectInterrupt返回时，如果在中断启用状态或者ShareVector设置为TRUE时，调用者的ISR都可能会被调用。驱动在IoConnectInterrupt返回前都不能允许中断