## I/O Request packet(IRP)
发往驱动的请求大部分是被打包在IRP中

系统组件或者设备驱动通过IoCallDriver将irp传递给驱动

IoCallDriver参数
+ DeviceObject指针
+ irp指针
将irp发送给与指定设备关联的驱动程序

当一个IRP被一个设备栈处理时，通常是将IRP发送给设备栈顶的设备。处理IRP的设备驱动可以选择完成IRP或者将IRP交给下层对象，通过IoCallDriver

IRP包含了驱动处理时所需的所有信息。



一个IRP的处理可能会关联到pnp设备树中的多个设备节点，如操作U盘的请求，可能需要USB Mass stroage device,usb root hub, usb host controller等。这样多个设备栈都参与到了irp的处理过程中。无论涉及到多少设备栈，涉及到这个I/O处理的驱动的总序列成为这个irp的驱动栈