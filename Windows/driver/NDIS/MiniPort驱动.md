# Miniport drivers
NDIS的miniport驱动有两个基本的功能：
+ 管理网卡，包括通过网卡收发数据
+ 和上层驱动交互，例如过滤驱动，中间层驱动，以及协议驱动

miniport驱动通过NDIS库来和网卡以及上层驱动交互。NDIS库导出一个包括了miniport驱动所需的所有系统功能的完整的函数集(NdisM*Xxxx*/Ndis*Xxxx*)。Miniport驱动则必须导出一组入口点(MiniportXxx)给NDIS或者上层驱动调用来访问miniport驱动



>NDIS驱动栈的更多信息以及描述四种NDIS驱动的关系的图，见[NDIS Driver Stack](https://docs.microsoft.com/zh-cn/windows-hardware/drivers/network/ndis-driver-stack)

下面的收发操作描述了miniport驱动和NDIS以及其上层驱动的交互过程：        
+   当一个传输层驱动要发送一个数据包，它调用一个NDIS导出的Ndis函数。之后NDIS将这个数据包通过调用miniport驱动导出的MiniportXXX函数来将包传递给miniport驱动。之后miniport驱动通过调用NdisXxxx结构来将包交给网卡发送。

+   当网卡收到一个发送给自己的包时，它发出一个会被NDIS或者miniport驱动处理硬件中断。NDIS通过调用miniport导出的适当接口来通知网卡的miniport驱动。miniport驱动设置从网卡来的数据传输，然后通过调用NdisXxxx函数来通知上层驱动

   

## 无连接的MiniPort驱动和面向连接的MiniPort驱动
NDIS为无连接的环境和面向连接的环境支持miniport驱动

无连接的Miniport驱动控制无连接网络的网卡，例如以太网。无连接的miniport驱动可以再分序列化和反序列化驱动：



> 所有 NDIS 6.0 及以后的驱动都是反序列化的.
+ 反序列化驱动将他们的MiniportXXX的操作序列化，在内部将所有的要要发送的数据包排序。由于驱动的critical section很小，这会获得更好的全双工性能。
+ 序列化驱动依赖NDIS来将对他们的MiniportXxx接口的调用序列化和管理发送队列

面向连接的miniport驱动为面向连接的网络控制网卡，例如ISDN。面向连接的驱动总是反序列化的。它总是序列化驱动，它们总是序将它们自己的MiniportXxx功能序列化，并在内杯将所有的发送包排队。

NDIS miniport驱动可以有一个非NDIS的下层边缘

An NDIS miniport driver can have a non-NDIS lower edge (see the following figure).

![具有非 ndis 下边缘的 ndis 微型端口驱动程序](https://docs.microsoft.com/zh-cn/windows-hardware/drivers/network/images/nonndslo.png)

通过它的非NDIS下层边界，miniport驱动使用总线的类接口，例如USB来控制总线上的设备。Miniport驱动通过向总线或者连接到总线的远程设备发送I/O请求来与设备交互。在它的上层边界。miniport导出标准的NDIS miniport驱动接口，这使得miniport驱动可以与上层的NDIS驱动交互
