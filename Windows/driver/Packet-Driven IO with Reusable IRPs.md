# Packet-Driven I/O with Reusable IRPs
I/O管理器,PnP管理器，电源管理器使用IRP与内核模式驱动交互,并允许驱动间交互

I/O管理器处理I/O的步骤
+ 接受I/O请求，通常是来自与用户模式程序
+ 创建代表I/O请求的IRP
+ 将IRP发送给适当的驱动
+ 追踪IRP直到他们被完成
+ 将I/O操作的结果返回给原始的请求者

一个IRP可能被发往多个驱动，例如，一个打开文件的请求，首先时到文件系统驱动，然后通过中间层镜像驱动，最终到达磁盘驱动，也可能到一个pnp总线驱动，这个称为驱动栈。

因此每个IRP都有一个固定部份，对于每个每个控制着设备的驱动还有一个驱动相关的I/O Stack location。
+ 在固定部份，I/O管理器维护者原始请求相关的信息，如调用者线程id和参数，文件打开的设备对象的地址之类的。这个固定的部份还包含一个I/O状态块,驱动会在其中设置I/O请求的处理状态。
+ 在最上层驱动的I/O stack localtion，I/O管理器，PNP管理器或者电源管理器设置了驱动相关的参数，比如请求的功能码，以及上下文，驱动根据这些信息来决定应该做什么。每个驱动在将IRP传递给下层驱动时都会设置好给下层驱动的I/O stack location 

驱动在处理IRP时，可以通过irp来访问其I/O stack Location.从而在驱动执行的每个阶段从用IRP.(?As each driver processes an IRP, it can access its I/O stack location in the IRP, thereby reusing the IRP at each stage of the driver's operations. )此外，高层驱动可以创建或者重用IRP来讲请求发给下层驱动。


IRPs的更多信息,见 Handling IRPs.(https://docs.microsoft.com/en-us/windows-hardware/drivers/kernel/handling-irps)