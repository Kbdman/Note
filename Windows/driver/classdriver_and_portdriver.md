## class driver
类驱动，为一类设备服务，在接口标准化的设备中，可以用来为多个厂商的设备提供服务
## miniclass driver
厂商实现的驱动，不直接处理IRP，由class driver调用，为classdriver服务
## Port Driver
通常是微软实现的，对某一类I/o端口的操作
## MiniPort Driver
小端口驱动，提供功能供PortDriver调用