# 编写DPC过程
DpcForIsr和CustomDpc的主要责任时保证完成当前啊IRP，以及启动下一个I/O操作        
DpcForIsr或者CustomDpc的其他工作时基于驱动的设计以及设备的特性。例如Dpc页可以做如下的事情：
+ 重试一个超时或者失败的操作
+ 调用IoAllocateErrorLogEntry和IoWriteErrorLogEntry来报告一个设备i/o错误。
+ 如果驱动使用buffered I/O或者irp指定了一个设备控制操作。再完成IRP前，将数据传输到Irp->AssoctiatedIrp.SystemBuffer.
+ 如果驱动使用Direct I/O,并且将大量传输分成小块，保存每个小块操作的状态，计算下次的传输范围，并使用SynchCritSection过程来编程设备以完成下次传输。       
Buffered_I/O也可能会把大操作分成小操作。
+ If the driver uses packet-based DMA, call FlushAdapterBuffers after each device transfer operation, and call FreeAdapterChannel or FreeMapRegisters when a sequence of partial transfers is done and the full transfer request is satisfied.
+ If a requested transfer is only partly satisfied by a single DMA operation, the DpcForIsr or CustomDpc routine is usually responsible for setting up one or more DMA operations until the IRP's specified number of bytes have been fully transferred.        
For more information about using DMA, see Adapter Objects and DMA.      
+ If the driver uses programmed I/O (PIO), call KeFlushIoBuffers at the end of each transfer operation if the current IRP requests a read.      
If a requested transfer is only partly satisfied by a single PIO operation, the DpcForIsr or CustomDpc routine is usually responsible for setting up one or more transfer operations until the IRP's specified number of bytes have been fully transferred.     
For more information about using PIO, see Using Direct I/O.

+ If a non-WDM driver has a ControllerControl routine, call IoFreeController when a requested operation is complete.

注意DpcForIsr或者customerDpc过程通常要处理设备I/O的大部分操作来曼珠IRP。这些过程要负责将IRP排队到设备对应驱动的分发过程中

考虑遵循下列设计规则：
+ Dpc过程应在安全的情况下应当尽可能快的调用IoStartNextPacket.安全的状况指，不会造成资源冲突或者和StartIo过程产生竞争状态

+ 如果一个驱动管理它自己的IRP队列，DPC应当在可以安全的取出IRP进行处理时尽快通知驱动。

DpcForIsr或者CustomDpc过程必须调用IoStartNextPacket或者通知驱动何时进行下一个请求的处理。基于驱动以及其设备。这可能是在Dpc要完成当前IRP之前，or it can occur immediately before this routine completes the current IRP and returns control.
