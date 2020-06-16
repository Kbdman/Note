# WdfIoQueueCreate 
WdfIoQueueCreate方法为指定的设备创建并设置一个I/O队列
```
NTSTATUS WdfIoQueueCreate(
  WDFDEVICE              Device,
  PWDF_IO_QUEUE_CONFIG   Config,
  PWDF_OBJECT_ATTRIBUTES QueueAttributes,
  WDFQUEUE               *Queue
);
```
## 参数
+ Device        
要与队列相关联的设备句柄

+ Config        
一个调用者申请的WDF_IO_QUEUE_CONFIG结构体指针

+ QueueAttributes       
一个描述新建对象的属性的用户申请的WDF_OBJECT_ATTRIBUTES结构体指针。这个参数是可选的，可以是WDF_NO_OBJECT_ATTRIBUTES.

+ Queue     
用于接受框架队列对象句柄的内存位置的指针


## 返回值
如果成功返回STATUS_SUCCESS.否则将返回以下的一个值
|Return code|Description|
| --- | --- |
|STATUS_INVALID_PARAMETER|输入参数无效|
|STATUS_INFO_LENGTH_MISMATCH|WDF_IO_QUEUE_CONFIG的Size成员不正确|
|STATUS_POWER_STATE_INVALID|框架在进行一个电源管理操作|
|STATUS_INSUFFICIENT_RESOURCES|可用的内存太少|
|STATUS_WDF_NO_CALLBACK|WDF_IO_QUEUE_CONFIG中没有指定任何请求处理函数，并且分发方法不是WdfIoQueueDispatchManual|
|STATUS_UNSUCCESSFUL|驱动尝试为一个已有默认队列的设备创建一个默认队列，或者一个内部错误|

这个方法也可能返回其他的NTSTATUS值 
当驱动指定了一个非法的对象句柄，系统会进入bug check


## 备注
每次调用WdfIoQueueCreate方法将为设备创建一个I/O队列。驱动可以为每个设备创建对各I/O队列

Config和QueueAttributes参数指定了队列的配置和对象属性

在默认情况下，传入的框架设备对象成为了新创建的框架队列对象的父对象。如果驱动在WDF_OBJECT_ATTRIBUTS结构的ParentObject中指定了父对象，父对象可以是框架设备对象或者任意父对象链指向一个框架设备对象的对象。框架会在删除父对象时删除队列对象

如果驱动为队列对象提供了EvtCleanupCallback或者EvtDestroyCallback方法，框架会在PASSIVE_LEVEL调用他们.

更多信息见Creating I/O Queues.

If your driver provides EvtCleanupCallback or EvtDestroyCallback callback functions for the framework queue object, the framework calls these callback functions at IRQL = PASSIVE_LEVEL.

For more information about WdfIoQueueCreate, see Creating I/O Queues.