# KSERVICE_ROUTINE callback function

InterruptService过程快速的处理设备中断，并在必要的情况下，安排之后对收到的数据的处理

```
KSERVICE_ROUTINE KserviceRoutine;

BOOLEAN KserviceRoutine(
  _KINTERRUPT *Interrupt,
  PVOID ServiceContext
)
{...}
```
## 参数
+ Interrupt     
调用者提供的中断的KINTERRUPT结构体指针
+ ServiceContext        
调用者提供的上下文信息的指针，在IoConnectInterrupt或者IoConnectInterruptEx时指定

## 返回值
如果过程判断这个中断不是来之这个驱动的设备，则必须返回FALSE.否则必须处理中断并返回TRUE.

## 备注
为了为指定的中断和处理器关联关系注册ISR,驱动必须调用IoConnetInterrupt或者IoConnectInterruptEx.

驱动的ISR以IoConnectInterrupt指定的IRQL执行在中断上下文中.(有着更高DIRQL的设备可以中断当前ISR)。

在系统调用ISR前，系统会获取中断的SpinLock(IoConnectInterrupt参数中指定的SpinLOCk),所以这个ISR不会同时再另外一个处理器上执行。再ISR返回后，系统释放这个SPINLOCK.

ISR必须首先通过测试上下文信息来判断这个中断是否来自于当前驱动的设备。如果中断不是来自当前设备，ISR必须立刻返回FALSE，这样I/O管理器可以调用其他为了相同的中断向量和CPU注册了ISR的驱动。

实现ISR的更多信息见 https://docs.microsoft.com/windows-hardware/drivers/kernel/interrupt-service-routines


## 示例

要定义一个ISR，必须首先提供一个指明了你定义的回调函数的函数声明。Windows为驱动提供了一组回调函数的类型。使用回调函数类型声明函数可以帮助驱动的代码分析，Static Driver Verifier，以及其他的校验工具防线错误。这是为Windows写驱动的一个要求。

定义一个叫MyInerruptService的InterruptService的回调函数的方法如下   
```    
// 声明
KSERVICE_ROUTINE MyInterruptService;
//定义函数
_Use_decl_annotations_
BOOLEAN
  MyInterruptService(
    struct _KINTERRUPT  *Interrupt,
    PVOID  ServiceContext
    )
  {
      // Function body
  }
```
KSERVICE_ROUTINE函数的类型定义再Wdm.h中。为了在执行代码分析工具时获得更准确的错误信息，确保在函数定义前使用_Use_decl_annotations_. _Use_decl_annotations_ 确保KSERVICE_ROUTINE在头文件中注释会被应用到程序中。      
函数声明的更多要求，见 https://docs.microsoft.com/windows-hardware/drivers/devtest/declaring-functions-using-function-role-types-for-wdm-drivers。
_Use_decl_annotation_的更多信息见 https://go.microsoft.com/fwlink/p/?linkid=286697。
