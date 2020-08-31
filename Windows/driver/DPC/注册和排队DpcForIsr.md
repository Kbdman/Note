# 注册和排队DpcForIsr
驱动通过在创建设备对象后调用IoInitializeDpcRequest来为设备对象注册DpcForIsr.驱动可以在它的AddDevice过程或者处理IRP_MN_START_DEVICE的DispatchPnp中注册DpcForIsr.

驱动的ISR在它返回前调用IoRequestDpc来讲一个DpcForIsr过程排入队列。



![](https://docs.microsoft.com/zh-cn/windows-hardware/drivers/kernel/images/3dpcisr.png)

如上图所示,调用IoInitializeDpcRequest讲一个Dpc对象与驱动提供的DpcForIsr过程相关联。I/O管理器为Dpc对象申请内存，然后根据驱动的行为调用KeInitializeDpc。

当ISR被调用来在DIRQL上处理设备中时，为了更好的系统总体性能以及驱动总体性能，它应当尽可能快的讲控制权交还给系统。通常ISR仅仅时清楚中断，为DpcForIsr完成IO操作手机所需的信息，调用IoRequestDpc，返回

当ISR调用IoRequestDpc时，他需要传递一个设备对象的指针，一个Device->CurrentIrp的指针，以及一个驱动设定的上下文。I/O管理器根据驱动的行为调用KeInsertQueueDpc来将DPC对象排入队列。当一个处理器的IRQL掉下DISPATCH_LEVEL后，内核从队列中取出DPC对象，然后在那个处理器上以DISPATCH_LEVEL运行DpcForIsr.

DpcForIsr过程负责进行完成当前IRP所需的操作。在开始，过程收到一个DPC对象的指针，以及设备对象和IRP以及上下文区域的指针。上下文区域必须被锁定在物理内存中，通常在设备拓展中。此外它也可以在驱动申请的非分页池中。或者驱动使用的控制器的控制器拓展中。

由于ISR和DpcForIsr可能会在不同处理器上同时运行，应当追寻一下规则：
ISR必须只在它即将返回前才调用IoReuqestDpc.否则DPC会在ISR设置完上下文区域前就执行。      
如果Dpc或其他驱动过程共享一个上下文，它们必须通过KeSynchronizeExecution来访问共享的数据区域。       
如果一个驱动使用设备拓展来维护设备i/o操作的上下文，DpcForIsr不应该为输入设备对象调用IoStartNextPacket或者从一个输入设备对象的队列中取出一个IRP，直到它马上要调用IoCompeletRequest否则驱动的StartIO(过车队列管理过程)可能开始另一个i/o操作，以至于共享的上下文区域在DpcForIsr可以完成I/O前被覆盖。       
即是在一个单处理器的机器上，ISR也可能因为中断而在DpcForIsr执行中或者执行前被调用。如果出现这种状况，DpcForIsr只运行一次。也就是说，if a driver overlaps I/O operations for its target device objects，ISR对IoRequestDpc和DpcForISr过程的调用没有一对一的关系


Even on a uniprocessor machine, the ISR could be called again if the device interrupts while or before the DpcForIsr routine is run. If this occurs, the DpcForIsr routine is run only once. In other words, there is no one-to-one correspondence between an ISR's calls to IoRequestDpc and instantiations of the DpcForIsr routine if a driver overlaps I/O operations for its target device objects.