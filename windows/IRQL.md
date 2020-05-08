## IRQL
+ PASSIVE_LEVEL—execute thread
+ APC_LEVEL—execute special kernel APC; page fault
+ DISPATCH_LEVEL—dispatch (execute DPC)
+ WAKE_LEVEL—debugger execution
+ CLOCK2_LEVEL—interval-timer execution
+ REQUEST_LEVEL—interprocessor request
+ POWER_LEVEL—power failure notification
+ HIGH_LEVEL—machine checks or bus errors


## PASSIVE_LEVEL
一般线程执行的IRQL,可以被内核调度器调度


## DISPATCH_LEVEL
调度器运行的IRQL

如果一个线程将当前CPU的IRQL提升到2，那么它将不会被另外的线程抢占，调度器将无法被唤醒，除非IRQL再掉到2以下。

+ 在IRQL2或以上，等待任意内核对象都会导致系统崩溃，因为等待后需要让调度器在当前CPU上唤起另外一个线程，但是调度器无法被唤醒，导致bugcheck;
+ 无法处理页错误，因为处理页错误需要进行上下切换，但是无法进行上下文切换。所以运行在IRQL 2或者以上的代码不能方位非分页池的数据