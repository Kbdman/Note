# Introduction to DPC Objects
由于ISR必须执行的尽可能地快，驱动必须将处理中断的部份操作放到ISR返回后。因此，系统提供了延迟过程调用的支持，使得ISR可以排队一个DPC在之后一个低一些的IRQL上执行.

每个DPC都必须与一个系统定义的DPC对象关联。系统为每一个设备对象提供一个DPC对象。系统在驱动注册DpcForISR时初始化DPC对象。如果驱动需要更多的DPC，可以自行创建其他的DPC对象。这些对象被称为CustomDPC过程

DPC对象的内容不应当被驱动直接引用。DPC对象的数据结构是没有纳入文档的。驱动对于系统为每个设备提供的DPC是无法访问的。其他的DPC驱动自行申请存储空间。，但是这些DPC的内容也应当制备系统过程所引用。
 
DPC对象和DPC可以和计时器一同使用，更多的信息见 https://docs.microsoft.com/zh-cn/windows-hardware/drivers/kernel/timer-objects-and-dpcs
