# Which Type of DPC Should You Use?

基于驱动的设计，可以采用以下的几种方案：
+ 一个单独的DpcForIsr完成所有的中断驱动I/O
+ 一个或者多个CustomDpc过程
+ 一个DpcForIsr和指定操作类型的CustomDpc过程

无论驱动使用哪种方案，都基于下层设备以及它支持的I/O请求。

大部分的最底层设备驱动都只用一个DpcForIsr来完成每一个IRP的I/o操作。单独使用DpcForISr来完成在一次处理一个操作的设备上每一个中断驱动的i/o操作是相对容易的。这样的驱动的ISR应该为每个中断驱动的i/o操作调用IoRequestDpc.

只有很少的最底层驱动由于它们的设备需要多个DPC来完成一组中断驱动的I/O操作而使用CustomDPc，

要使用一个单独的DpcForISr来为一个可以同时完成多个操作的设备来完成overlapped的中断驱动I/O操作，需要小心的设计，这相对较难。对此，ISR可以通过调用KeInsertQueueDpc来将一组驱动提供的操作相关的CustomDpc排队

例如，考虑到编写一个串口驱动的设计问题。这时一个全双工设备的驱动，串口驱动不能依赖于一个IRp被排入StartIO过程时的顺序和设备在多任务多处理器系统中中断的顺序一一对应。此外，串口驱动必须处理超时请求和用户生成的取消前一个请求的异步请求。

然后一个串口驱动可能要维护内部的读、写、清空、等待操作的队列。它也可能在内部队列中维护着引用计数或者其他追踪机制，例如一组flag。它的ISR可以调用KeInsertQueueDpc来将与驱动提供的DPC对象之关联的DPC过程排入队列中。

Consequently, a serial driver might maintain internal queues for the read, write, purge, and wait operations that user-mode COM port applications can request. It also could maintain reference counts or use some other tracking mechanism, such as a set of flags, for the IRPs in its internal queues. Its ISR would call KeInsertQueueDpc with any of a number of driver-allocated and initialized DPC objects, each associated with a driver-supplied CustomDpc routine.