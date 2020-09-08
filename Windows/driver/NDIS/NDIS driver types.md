# NDIS driver types
网络驱动接口规范(NDIS)库将网络影箭从网络驱动中抽象出来。NDIS也指定了分层网络驱动中见的标准接口，从上层驱动中抽象出了管理硬件的下层驱动，例如网络传输层。NDIS同样为了了网络驱动的状态信息和参数，包括函数指针，句柄，以及链接时的参数块，以及其他系统值

NDIS支持一下的主要集中网络驱动      
+ [Miniport drivers](https://docs.microsoft.com/zh-cn/windows-hardware/drivers/network/ndis-miniport-drivers2)
+ [Protocol drivers](https://docs.microsoft.com/zh-cn/windows-hardware/drivers/network/ndis-protocol-drivers2)
+ [Filter drivers](https://docs.microsoft.com/zh-cn/windows-hardware/drivers/network/ndis-filter-drivers)
+ [Intermediate drivers](https://docs.microsoft.com/zh-cn/windows-hardware/drivers/network/ndis-intermediate-drivers)
