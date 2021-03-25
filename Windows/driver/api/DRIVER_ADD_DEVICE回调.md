## DRIVER_ADD_DEVICE �ص����� (wdm.h)
AddDevice���̸���ΪPnp���������絽���豸���������豸������߻��߹����豸����


```
DRIVER_ADD_DEVICE DriverAddDevice;

NTSTATUS DriverAddDevice(
  _DRIVER_OBJECT *DriverObject,
  _DEVICE_OBJECT *PhysicalDeviceObject
)
{...}
```
### ����
+ DriverObject      
�������ṩ����������ָ�룬ָ����������������

+ PhysicalDeviceObject  
�������ṩ�����²����������Ĵ�����PDO���豸����ָ��


### ����ֵ
�ɹ�����STATUS_SUCCESS��������뷵��Ntstatus.h�ж����һ��������

### ��ע
����֧��PnP���ں������������ṩһ��AddDevice���̣�����DriverEntry�н����ַ����DriverObject->DriverExtension->AddDevice.

AddDevice����Ҫ����ʱ����IoCreateDevice����һ���豸����Ȼ�����IoAttachDeviceToDeviceStack�������ӵ��豸ջ�ϡ�

���ʵ��AddDevice��[Writing an AddDevice Routine](https://docs.microsoft.com/en-us/windows-hardware/drivers/kernel/writing-an-adddevice-routine)

AddDevice ����������PASSIVE_LEVEL�ϡ�