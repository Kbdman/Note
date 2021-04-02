## DRIVER_ADD_DEVICE 回调函数 (wdm.h)
AddDevice过程负责为Pnp管理器美剧到的设备创建功能设备对象或者或者过滤设备对象


```
DRIVER_ADD_DEVICE DriverAddDevice;

NTSTATUS DriverAddDevice(
  _DRIVER_OBJECT *DriverObject,
  _DEVICE_OBJECT *PhysicalDeviceObject
)
{...}
```
### 参数
+ DriverObject      
调用者提供的驱动对象指针，指针驱动的驱动对象

+ PhysicalDeviceObject  
调用者提供的由下层驱动创建的代表着PDO的设备对象指针


### 返回值
成功返回STATUS_SUCCESS，否则必须返回Ntstatus.h中定义的一个错误码

### 备注
所有支持PnP的内核驱动都必须提供一个AddDevice过程，并在DriverEntry中将其地址存入DriverObject->DriverExtension->AddDevice.

AddDevice的主要责任时调用IoCreateDevice创建一个设备对象，然后调用IoAttachDeviceToDeviceStack将它附加到设备栈上。

如何实现AddDevice见[Writing an AddDevice Routine](https://docs.microsoft.com/en-us/windows-hardware/drivers/kernel/writing-an-adddevice-routine)

AddDevice 过程运行在PASSIVE_LEVEL上。