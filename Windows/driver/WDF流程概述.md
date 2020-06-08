# WDF 流程概述
## 入口
DriverEntry入口，主要工作:
+ 创建一个WDF_DRIVER_CONFIG
+ 使用WDF_DRIVER_CONFIG_INIT设置EvtDriverDeviceAdd回调，相当于WDM中的AddDevice
+ 调用WdfDriverCreate创建Wdf驱动对象

### 设备创建
+ 通过WdfDeviceInitxxx函数初始化设备初始化信息DeviceInit        
    + WdfDeviceInitAssignName 设置设备名称
    + WdfDeviceSetDeviceType 设置设备类型
    + WdfDeviceSetIoType 设置Io类型
    + WdfDeviceSetExclusive 设置设备是否互斥
+ 初始化一个WDF_OBJECT_ATTRIBUTES作为设备的属性     
    + 通过WDF_OBJECT_ATTRIBUTES_INIT_CONTEXT_TYPE 设置设备拓展，推测创建设备时会按照后面指定的类型的大小来创建设备拓展
    + 设置EvtCleanupCallback回调
+ 调用WdfDeviceCreate创建Wdf设备对象