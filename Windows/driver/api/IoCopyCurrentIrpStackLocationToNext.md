# IoCopyCurrentIrpStackLocationToNext
IoCopyCurrentIrpStackLocationToNext例程将IRP栈参数从当前I/O栈位置拷贝到下层驱动的栈位置

```
void IoCopyCurrentIrpStackLocationToNext(
  PIRP Irp
);
```
## 参数
+ Irp   
Irp指针

## 返回值
无

## 备注
驱动调用IoCopyCurrentIrpStackLocationToNext将IRP参数从它的栈位置拷贝到下层驱动的栈位置中。

在调用这个例程之后，驱动通常都在将IRP传递给下层驱动前调用IoSetCompletionRoutine来设置一个I/O完成例程。要将IRP传递给下层驱动，但是不设置I/O完成例程的驱动应当调用IoSkipCurrentIrpStackLocation而不是IoCopyCurrentIrpStackLocationToNext

如果你的驱动调用IoSkipCurrentIrpStackLocation，注意不要修改IO_STACK_LOCATION结构的值以免再无意间影响到下层驱动或者驱动相关的系统行为。特别是，驱动不应当修改IO_STACK_LOCATION结构的Parameters成员，也不应当调用IoMarkIrpPending，

If your driver calls IoSkipCurrentIrpStackLocation, be careful not to modify the IO_STACK_LOCATION structure in a way that could unintentionally affect the lower driver or the 