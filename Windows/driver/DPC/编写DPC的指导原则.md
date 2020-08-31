# DPC过程设计指导原则

+ Dpc过程必须和驱动的其他过程同步它们对物理设备以及驱动维护的共享状态共享资源的访问     
如果一个DPC过程要和ISR访问数据，那么必须使用KeSynchronizeExection来调用SyncCritSection来访问。      
如果DPC与不是ISR的过程共享数据，必须使用一个驱动提供的SPINLOCK来保护数据

+ DPc过程执行在Dispatch_LEVEL上，这限制了它可以调用的支持过程。例如它们不可以访问或者申请分页内存，不可以等待内核对象。不过它们可以用KeAcquireSpinLockAtDpc和KeRelaseSpinLockForkDpcLevel,这些比KeAcurireSpinLock和KeReleaseSpinlock快些        
尽管DPC过程无法进行阻塞的调用，它可以将一个工作对象排入运行在PASSIVE_LEVEL的工作线程中。工作未向可以在线程中进行等待。DpcForIsr通常使用IoQueeuWorkItem来将Work对象排入工作线程.而custom通常调用exQueueWorkItem过程

+ Dpc过程通常要负责启动设备的下一个IO操作       
使用DirectIO的最下层物理设备驱动，可以使用SynchCritSection过程来编程设备传输更多的数据以在调用IoStartnextPacket前满足当前irp

+ DPC过程应当运行尽可能短的事件，并且应当将尽可能多的处理交给工作线程       
当一个DPC运行在一个处理器上时，所有的线程都被被阻止在该处理器上运行。其他的排队的DPC也被阻塞，知道当前DPC完成。为了避免降低系统的反应效率，通常DPC每次被调用都不会运行操作100毫秒。如果一个任务需要更多的事件，并且必须在DISPATCH_LEVEL上执行。该DPC应当在100ms前结束并排队一个CustomDPC完成后续操作。      
一个DPC过程应当只执行必须运行在DISPATCH_LEVEL上的操作。，然后剩余的中断相关的工作交给在PASSIVE_LEVEL上运行的工作线程。      
调用KeStallExecution来延迟执行的DPC过程，不能延迟超过100MS。        
使用WDK中的性能分析工具来检查dpc的执行事件。

+ If the driver uses DMA and its AdapterControl routine returns KeepObject or DeallocateObjectKeepRegisters (thereby retaining the system DMA controller channel or bus-master adapter for additional transfer operations), the DpcForIsr or CustomDpc routine is responsible for releasing the adapter object or map registers with FreeAdapterChannel or FreeMapRegisters before it completes the current IRP and returns control.

+ If a lowest-level physical device driver sets up a controller object to synchronize I/O operations through the controller to attached devices, its DpcForIsr or CustomDpc routine is responsible for releasing the controller object using IoFreeController before it completes the current IRP and returns control.

+ Dpc过程通常负责为设备处理请求过程中产生的错误记录Log，如果必要和可能重试当前请求，并设置I/O状态块，并调用IoCompleteReuqets完成请求
+ 如果一个驱动支持overlapped I/O,必须准寻处理overlapped I/O的规则 https://docs.microsoft.com/zh-cn/windows-hardware/drivers/kernel/handling-overlapped-i-o-operations

+ 任何驱动的DPC过程通常只为驱动必须支持的一小部份I/O控制码完成I/O处理。In particular, the DPC routine completes operations for device control requests with the following characteristics:
    + Requests that change the state of the physical device
    + Requests that require the return of inherently volatile information about the physical device