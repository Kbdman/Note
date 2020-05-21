# KeSynchronizeExecution

KeSynchronizeExecution过程用于通过特定过程和连接到指定中断对象的一个或者多个ISR的执行

```
BOOLEAN KeSynchronizeExecution(
  PKINTERRUPT            Interrupt,
  PKSYNCHRONIZE_ROUTINE  SynchronizeRoutine,
  __drv_aliasesMem PVOID SynchronizeContext
);
```
## 参数
+ Interrupt     
一组中断对象的指针.调用者通过IoConnectInterrupt或者IoConnectInterruptEx过程来获得中断对象的指针

+ SynchronizeRoutine    
一个调用者提供的SyncCritSection过程，这个过程的执行会和指定中断对象的ISR同步

+ SynchronizeContext
调用者提供的在SyncCritSection过程被调用时传递给它的上下文指针
A pointer to a caller-supplied context value to be passed to the SynchCritSection routine when it is called.

## 返回值
如果操作成功，KeSynchronizeExecution返回true,否则返回FALSE.

## 备注
当KeSynchronizeExecution被调用时，会发生以下事情：
IRQL会提升到注册ISR时IoConnectInterrupt或者IoConnectInterruptEx指定的IRQL上     
对同步上下文的访问会通过获取中断向量关联的SPINLOCK的方式来和ISR同步.（对于运行在PASSIVE_LEVEL的ISR，是通过系统事件对象）

指定的SyncCritSection过程被调用，同步上下文值作为它的参数。

如果ISR在DISPATCH_LEVEL及以上运行时，SyncCritSection过程会运行在同样的DIRQL上，因此它必须运行事件尽可能短以避免影响其他高优先级的任务。

KeSynchronizeExecution必须运行在不高于DIRQL的IRQL上,即小于等于注册ISR时指定的IRQL

从Windows 8开始，驱动可以调用KeSynchronizeExecution来与运行PASSIVE_LEVEL上的ISR进行同步。早期版本的Windows上，KeSynchronizeExecution只能和运行在DISPATCH_LEVEL以及以上的ISR进行同步。       
PASSIVE_LEVEL ISR的更多信息见 https://docs.microsoft.com/windows-hardware/drivers/kernel/using-passive-level-interrupt-handling-routines

