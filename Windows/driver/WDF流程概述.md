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
    + 通过WDF_OBJECT_ATTRIBUTES_INIT_CONTEXT_TYPE 设置上下文空间，推测创建设备时会按照后面指定的类型的大小来创建设备的上下文空间
    + 设置EvtCleanupCallback回调
+ 调用WdfDeviceCreate创建Wdf设备对象
+ 为设备创建IoQueue
    + 调用WDF_IO_QUEUE_CONFIG_INIT_DEFAULT_QUEUE，初始化一个WDF_IO_QUEUE_CONFIG作为队列的配置，设置队列的分发类型，可以串行，并行，或者手动。设置请求处理函数
    + 使用WDF_OBJECT_ATTRIBUTES_INIT_CONTEXT_TYPE设置队列对象的上下文空间的配置
    + 使用设备对象，队列配置，队列属性作为参数调用WdfIoQueueCreate为设备根绝属性创建设备的IO处理队列