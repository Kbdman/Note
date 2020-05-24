# SynchCritSection
Critcal Section 是需要对硬件资源或者驱动数据进行互斥访问的代码。因此这样的代码必须不能被要访问的同样资源的代码所打断，并且这个资源在同一时间只能被一个处理器所访问

CriticalSection应当为限制为ISR和SynchCritSection以及获取自旋锁。在一个SynchCritSection返回后，系统释放对应的自旋锁并降低处理器的IRQL。

将处理器的IRQL提升到DIRQL会组织当前处理器被低IRQL的中断。获取SPINLOCK防止其他处理器执行与该SPINLOCK关联的critical section代码

设备驱动的StartIo,DpcForIsr,或者CustomDpc过程通常需要频繁的访问一些相同的硬件资源(设备寄存器，或者一些其他总线相关的内存)或ISR要访问一些驱动维护的数据。基于驱动的设备或者设计，dispatch函数AdapterControl, ControllerControl或者计时器过程也可能要访问一些硬件资源或者驱动维护的数据

要调用一个非ISR得critical section,驱动必须使用keSynchronizeExection方法。这个方法接受有一个SynchCritSection过程，一个驱动定义得上下文信息，以及一个中断对象指针作为参数。系统通过中断对象指针来判断应当提升到得IRQL,以及要锁定得SPINLOCK

像ISR一样，一个SynchCritSection必须执行的尽可能的快，在返回前设置必要的设备寄存器或者更新上下文数据。       
由于KeSynchronizeExection保持着一个中断SPINLOCK,在SyncCritSection执行的时候对应的ISR是无法执行的。      
在接受到一个IRP时，设备应当尽可能在IRQL_PASSIVE_LEVEL的dispatch方法中,或者在DISPATCH_LEVEL的StartIO过程或者DPC中