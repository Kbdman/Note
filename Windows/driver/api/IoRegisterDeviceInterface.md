# IoRegisterDeviceInterface

IoRegisterDeviceInterface过程注册一个设备接口类(在这个接口类没有被注册的情况下)，然后创建一个这个接口类的实例。之后驱动可以启用这个接口，使之可以被应用或者其他组件使用


```
NTSTATUS IoRegisterDeviceInterface(
  PDEVICE_OBJECT  PhysicalDeviceObject,
  const GUID      *InterfaceClassGuid,
  PUNICODE_STRING ReferenceString,
  PUNICODE_STRING SymbolicLinkName
);
```
## 参数
+ PhysicalDeviceObject    
指向设备PDO的指针

+ InterfaceClassGuid    
指明要注册的接口类的GUID


+ ReferenceString   
可选参数，一个UNICODE_STRING指针。字符串中不能不好任何路径分隔符("/" or "\").功能驱动的这个参数通常是NULL，过滤驱动的这个参数必须是NULL。   
ReferenceString只有少数总线驱动使用，例如swenum,一个会按需创建设备接口实例作为软件设备的占位符。当一个接口实例被打开，I/O管理器将实例的ReferenceString传递给驱动。这个字符串会成为一个接口实例的名称的一部分 (as an appended path component)。驱动可以使用ReferenceString来区分一个设备同一个类型的两个接口实例   
在Windows 98/Me，ReferenceString的长度不可以超过MAX_PATH.而在windows 2000以及更新版本的Windows上，ReferenceString没有长度限制。
+ SymbolicLinkName    
输出参数，一个指向调用者申请内存的Unicode string结构的指针。如果成功，他会初始化这个Unicode string,并申请字符串缓冲区来保存指定的设备接口类实例的符号链接的地址   
调用者必须将SymbolicLinkName作为一个不透明的数据来处理并且不能拆解它(?)。
调用者负责在不需要SymbolicLinkName时调用RtlFreeUnicodeString释放它



## 返回值
成功返回STATUS_SUCCESS。
### 可能的错误返回

|RETURN VALUE|Return code	Description|
|---|---|
|STATUS_INVALID_DEVICE_REQUEST|无效的参数，可能是PhysicalDeviceObject不是一个PDO指针，或者ReferenceString中有非法字符|    

## 备注
驱动可以为一个设备调用这个过程多次来创建多个类型实例。功能驱动或者过滤驱动通常再AddDevice过程中注册设备接口。   

如果一个设备接口之前没有被注册，I/O管理器会为他创建一个注册表项。驱动可以调用IoOpenDeviceInterfaceRegistryKey来访问注册表项。

驱动再注册设备接口类实例后，可以通过调用IoSetDeviceInterfaceState来启用或者禁用接口

注册的接口会一直存在即使系统重启。如果指定的接口已经被注册了，I/O管理器将他的名字通过SymbolicLinkName返回并返回STATUS_OBJECT_NAME_EXISTS.

大部分驱动使用一个NULL的ReferenceString。如果驱动要使用非NULL的ReferenceString，需要一些额外的工作，例如管理它的命名空间以及安全。过滤驱动必须使用一个NULL的ReferenceString以避免和设备栈中的其他驱动冲突

这个过程的调用者并不需要再它不需要的时候移除设备接口的注册。如果必要的话，设备接口注册可以在用户模式移除

IoRegisterDeviceInterface的调用者必须运行在一个PASSIVE_LEVEL的系统线程上下文中
