# IoGetAttachedDeviceReference function (ntifs.h)
获取设备所在设备栈上最上层设备对象，并增加该设备对象的引用计数


```
PDEVICE_OBJECT IoGetAttachedDeviceReference(
  PDEVICE_OBJECT DeviceObject
);

```
## 参数
+ DeviceObject       被附加的设备对象指针

## 返回值
返回指定设备所在的设备栈中最顶层的设备对象，并增加其引用计数

## 备注
如果没有设备对象附加在设备上，返回的指针和参数一样。

设备驱动的编写者必须保证当他们完成他们所需的操作后，调用ObDereferenceObject,来减少设备对象的引用计数，否则会妨碍系统删除设备对象