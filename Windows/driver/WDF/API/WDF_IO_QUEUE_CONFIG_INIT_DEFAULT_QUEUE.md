# WDF_IO_QUEUE_CONFIG_INIT_DEFAULT_QUEUE
WDF_IO_QUEUE_CONFIG_INIT_DEFAULT_QUEUE函数畜视话一个驱动的WDF_IO_QUEUE_CONFIG结构

```
void WDF_IO_QUEUE_CONFIG_INIT_DEFAULT_QUEUE(
  PWDF_IO_QUEUE_CONFIG       Config,
  WDF_IO_QUEUE_DISPATCH_TYPE DispatchType
);
```
## 参数
+ Config        
一个指向驱动的WDF_IO_QUEUE_CONFIG结构的指针
+ DispatchType
一个WDF_IO_QUEUE_DISPATCH_TYPE类型的枚举值，用户表示这个队列对请求的分发类型

## 返回值
无

## 备注
在创建一个电源托管的I/O队列作为设备的默认队列时，驱动当调用WDF_IO_QUEUE_CONFIG_INIT_DEFAULT_QUEUE、该函数清空WDF_IO_QUEUE_CONFIG结构并设置他的Size成员。同时将PowerManaged成员设置成WdfUseDefauly，设置DefaultQueue成员为TRUE，并将指定的分发类型设置到DispatchType成员上。

从KMDF 1.9开始，如果DispatchType被设置成WdfIoQueueDispatchParallel,WDF_IO_QUEUE_CONFIG_INIT_DEFAULY_QUEUE将NumberOfPresentedRequest设置成-1,这表示，框架可以将不受限制的数量的I/O Reuquest传递给驱动。

## WDF_IO_QUEUE_DISPATCH_TYPE 
| 类型|描述|
| --- | --- |
|WdfIoQueueDispatchInvalid|保留值，内部使用|
|WdfIoQueueDispatchSequential|I/O队列中的请求将依次传递给驱动的请求处理函数。框架在驱动调用WdfRequestComplete完成当前请求前，不会将下一个请求传递给驱动|
|WdfQueueDispatchParallel|框架在收到I/O请求后尽快转交给驱动|
|WdfIoQueueDispatchManual|框架将请求放入队列中，但不会将他们传递给驱动。驱动别绪通过WdfIoQueueRetrieveNextRequest来从队列中获取请求|
|WdfIoQueueDispatchMax|仅供内部使用|