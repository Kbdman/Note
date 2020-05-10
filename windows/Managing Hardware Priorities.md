# Managing Hardware Priorities

驱动过程执行的IRQL决定了它可以调用哪些驱动支持过程.例如，有些驱动支持过程要求调用者必须运行在DISPATCH_LEVEL.而其他一些过程，在调用者运行在比PASSIVE_LEVEL更高的IRQL时无法被安全的调用
   
下面时通常实现的标准驱动过程被调用的IRQL,从低优先级到高优先级

+ PASSIVE_LEVEL  
    不屏蔽任何中断  
    执行在PASSIVE_LEVEL的过程 DriverEntry,AddDevice,Reinstialize,Unload，大部分Dispatch过程，驱动创建的线程，工作线程回调

+ APC_LEVEL     
    屏蔽APC_LEVEL的中断
    执行在APC_LEVEL的过程，一些Dispatch过程，见 https://docs.microsoft.com/en-us/windows-hardware/drivers/kernel/dispatch-routines-and-irqls

+ DISPATCH_LEVEL        
    屏蔽DISPATCH_LEVEL和APC_LEVEL的中断。设备，时钟和电源错误中断可以出现   
    执行在DISPATCH_LEVEL的驱动过程:StartIo,AdapterControl, AdapterListControl, ControllerControl, IoTimer, Cancel (持有Cancel自旋锁), DpcForIsr, CustomTimerDpc, CustomDpc .

+ DIRQL
所有小于DIRQL的中断都被屏蔽，更高的IRQL的中断可以出现，包括时钟和电源错误中断 
执行在DIRQL的驱动过程，ISR, SynchCritSection routines.

APC_LEVEL和PASSIVE_LEVEL的唯一区别时，执行在APC_LEVEL的代码不会被APC中断中断。这两个IRQL下都有一个线程上下文，并且代码都可以被分页到磁盘。

最底层驱动在下面三个IRQL之一上处理IRP：
+ PASSIVE_LEVEL,在驱动的Dispatch过程中. 
DriverEntry, AddDevice, Reinitialize, 以及 Unload 也在PASSIVE_LEVEL上执行，任何驱动创建的系统线程也一样

+ DISPATCH_LEVEL,在StartIo过程中

+ DIRQL ISR和 SynchCritSection 

大部分更高层的驱动在下面二种IRQL中处理IRP：
+ PASSIVE_LEVEL  dispatch过程
+ DISPATCH_LEVEL IoCompletion过程

在一些状况下，大型存储设备的中间层或者最下层设备的驱动是在APC_LEVEL被调用的。这种状况下，文件驱动在下下层驱动发送IRP_MJ_READ请求时可能会出现页错误。

大部分在某个IRQL上运行的标准驱动过程允许他们简单的调用适当的支持过程。例如一个运行在DISPATCH_LEVEL的驱动过程一定可以调用AllocateAdapterChannel.由于大部分设备驱动都是在StartIO过程中调用这些过程，通常他们已经运行在DISPATCH_LEVEL了


注意，一个自己管理自己的IRP队列所以没有StartIO的设备驱动在要调用AllocateAdapterChannel时时不一定运行在DISPATCH_LEVEL.这样的驱动，必须把它对AllocateAdatoerChannel的调用放在KeRaiseIrql和KeLowerLrql中间，这样他在调用AllocateAdatoerChannel这样的过程时可以提升到所需的IRQL，在调用结束后再回到原先的IRQL。

在调用驱动支持过程时，需要注意：
+ 使用比当前IRQL低的值作为参数调用KeRaiseIrql，会导致致命错误，除了恢复原有IRQL外调用KeLowerIrql也会导致致命错误
+ 在IRQL>= DISPATCH_LEVEL上调用 KeWaitForSingleObject 或 KeWaitForMultipleObjects等待对象会导致致命错误
+ 驱动过程中只有那些运行在PASSIVE_LEVEL上的nonarbitrary线程上下文中的代码可以安全的等待事件，信号量，互斥锁或者计时器.例如驱动创建的线程，DriverEntry，Reinitialize routines, 以及一些固有的同步I/O操作的Dispatch过程 例如大部分的I/O control 请求).
+ 即使运行在PASSIVE_LEVEL上，可分页的驱动代码不能以Wait参数设为True来调用KeSetEvent,KeReleaseSemaphore或者KeRelaseMutex。这样的调用会导致致命的页错误
+ 任何执行在比APC_LEVEL高的IRQL上的过程既不能从分页池中申请内存，也不能访问分页池中的内存，如果一个在APC_LEVEL以上运行的代码出现页错误，这是知名错误。
+ 驱动在调用KeAcquireSpinLockAtDpcLevel和KEReleaseSpinLockFromDpcLevel时必须运行在DISPATCH_LEVEL上      
驱动在调用KeAquireSpinLock时可以运行在低于或等于DISPATCH_LEVEL上，但是它必须通过KeReleaseSpinLock来释放.或者说，使用KeReleaseSpinLockFromDpcLevel来释放KeAquireSpinLock获得阿SPINLOCK是个编程错误.    
在IRQL>DISPATCH_LEVEL时，驱动不能调用KeAquireSpinLockAtDpcLevel，KeReleaseSpinLockFromDpcLevel,KeAquireSpinLock,KeReleaseSpinLock
+ 调用使用SPINLOCK的支持过程，例如ExInterlockedExx过程，如果之前的IRQL与目标的SPINLOCK不在一个IRQL，会把当前处理器的IRQL提升到DISPATCH_LEVEL或者DIRQL。

+在IRQL>PASSIVE_LEVEL上运行的驱动代码应该尽可能快的执行。执行的IRQL越高，过程执行的速度对总体性能的影响越大.调用KeRaiseIrql的驱动应该在它可以的情况下尽可能快的调用KeLowerIrql.
更多见Scheduling, Thread Context, and IRQL白皮书(https://go.microsoft.com/fwlink/p/?linkid=59757);