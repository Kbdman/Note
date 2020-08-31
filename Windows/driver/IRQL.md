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

+ 在IRQL2或以上，等待任意内核对象都会导致系统崩溃，因为等待需要让调度器在当前CPU上唤起另外一个线程，但是调度器无法被唤醒，导致bugcheck;
+ 无法处理页错误，因为处理页错误需要进行上下切换，但是无法进行上下文切换。所以运行在IRQL 2或者以上的代码不能方位非分页池的数据


```
Managing Hardware Priorities
05/08/2018
4 minutes to read
   
The IRQL at which a driver routine executes determines which kernel-mode driver support routines it can call. For example, some driver support routines require that the caller be running at IRQL = DISPATCH_LEVEL. Others cannot be called safely if the caller is running at any IRQL higher than PASSIVE_LEVEL.

Following is a list of IRQLs at which the most commonly implemented standard driver routines are called. The IRQLs are listed from lowest to highest priority.

PASSIVE_LEVEL
Interrupts Masked Off — None.

Driver Routines Called at PASSIVE_LEVEL — DriverEntry, AddDevice, Reinitialize, Unload routines, most dispatch routines, driver-created threads, worker-thread callbacks.

APC_LEVEL
Interrupts Masked Off — APC_LEVEL interrupts are masked off.

Driver Routines Called at APC_LEVEL — Some dispatch routines (see Dispatch Routines and IRQLs).

DISPATCH_LEVEL
Interrupts Masked Off — DISPATCH_LEVEL and APC_LEVEL interrupts are masked off. Device, clock, and power failure interrupts can occur.

Driver Routines Called at DISPATCH_LEVEL — StartIo, AdapterControl, AdapterListControl, ControllerControl, IoTimer, Cancel (while holding the cancel spin lock), DpcForIsr, CustomTimerDpc, CustomDpc routines.

DIRQL
Interrupts Masked Off — All interrupts at IRQL<= DIRQL of driver's interrupt object. Device interrupts with a higher DIRQL value can occur, along with clock and power failure interrupts.

Driver Routines Called at DIRQL — InterruptService, SynchCritSection routines.

The only difference between APC_LEVEL and PASSIVE_LEVEL is that a process executing at APC_LEVEL cannot get APC interrupts. But both IRQLs imply a thread context and both imply that the code can be paged out.

Lowest-level drivers process IRPs while running at one of three IRQLs:

PASSIVE_LEVEL, with no interrupts masked off on the processor, in the driver's Dispatch routine(s)

DriverEntry, AddDevice, Reinitialize, and Unload routines also are run at PASSIVE_LEVEL, as are any driver-created system threads.

DISPATCH_LEVEL, with DISPATCH_LEVEL and APC_LEVEL interrupts masked off on the processor, in the StartIo routine

AdapterControl, AdapterListControl, ControllerControl, IoTimer, Cancel (while it holds the cancel spin lock), and CustomTimerDpc routines also are run at DISPATCH_LEVEL, as are DpcForIsr and CustomDpc routines.

Device IRQL (DIRQL), with all interrupts at less than or equal to the SynchronizeIrql of the driver's interrupt object(s) masked off on the processor, in the ISR and SynchCritSection routines

Most higher-level drivers process IRPs while running at either of two IRQLs:

PASSIVE_LEVEL, with no interrupts masked off on the processor, in the driver's dispatch routines

DriverEntry, Reinitialize, AddDevice, and Unload routines also are run at PASSIVE_LEVEL, as are any driver-created system threads or worker-thread callback routines or file system drivers.

DISPATCH_LEVEL, with DISPATCH_LEVEL and APC_LEVEL interrupts masked off on the processor, in the driver's IoCompletion routine(s)

IoTimer, Cancel, and CustomTimerDpc routines also are run at DISPATCH_LEVEL.

In some circumstances, intermediate and lowest-level drivers of mass-storage devices are called at IRQL APC_LEVEL. In particular, this can occur at a page fault for which a file system driver sends an IRP_MJ_READ request to lower drivers.

Most standard driver routines are run at an IRQL that allows them simply to call the appropriate support routines. For example, a device driver must call AllocateAdapterChannel while running at IRQL DISPATCH_LEVEL. Since most device drivers call these routines from a StartIo routine, usually they are running at DISPATCH_LEVEL already.

Note that a device driver that has no StartIo routine because it sets up and manages its own queues of IRPs is not necessarily running at DISPATCH_LEVEL IRQL when it should call AllocateAdapterChannel. Such a driver must nest its call to AllocateAdapterChannel between calls to KeRaiseIrql and KeLowerIrql so that it runs at the required IRQL when it calls AllocateAdapterChannel and restores the original IRQL when the calling routine regains control.

When calling driver support routines, be aware of the following.

Calling KeRaiseIrql with an input NewIrql value that is less than the current IRQL causes a fatal error. Calling KeLowerIrql except to restore the original IRQL (that is, after a call to KeRaiseIrql) also causes a fatal error.

While running at IRQL >= DISPATCH_LEVEL, calling KeWaitForSingleObject or KeWaitForMultipleObjects for kernel-defined dispatcher objects to wait for a nonzero interval causes a fatal error.

The only driver routines that can safely wait for events, semaphores, mutexes, or timers to be set to the signaled state are those that run in a nonarbitrary thread context at IRQL PASSIVE_LEVEL, such as driver-created threads, the DriverEntry and Reinitialize routines, or dispatch routines for inherently synchronous I/O operations (such as most device I/O control requests).

Even while running at IRQL PASSIVE_LEVEL, pageable driver code must not call KeSetEvent, KeReleaseSemaphore, or KeReleaseMutex with the input Wait parameter set to TRUE. Such a call can cause a fatal page fault.

Any routine that is running at greater than IRQL APC_LEVEL can neither allocate memory from paged pool nor access memory in paged pool safely. If a routine running at IRQL greater than APC_LEVEL causes a page fault, it is a fatal error.

A driver must be running at IRQL DISPATCH_LEVEL when it calls KeAcquireSpinLockAtDpcLevel and KeReleaseSpinLockFromDpcLevel.

A driver can be running at IRQL <= DISPATCH_LEVEL when it calls KeAcquireSpinLock but it must release that spin lock by calling KeReleaseSpinLock. In other words, it is a programming error to release a spin lock acquired with KeAcquireSpinLock by calling KeReleaseSpinLockFromDpcLevel.

A driver must not call KeAcquireSpinLockAtDpcLevel, KeReleaseSpinLockFromDpcLevel, KeAcquireSpinLock, or KeReleaseSpinLock while running at IRQL > DISPATCH_LEVEL.

Calling a support routine that uses a spin lock, such as an ExInterlockedXxx routine, raises IRQL on the current processor either to DISPATCH_LEVEL or to DIRQL if the caller is not already running at a raised IRQL.

Driver code that runs at IRQL > PASSIVE_LEVEL should execute as quickly as possible. The higher the IRQL at which a routine runs, the more important it is for good overall performance to tune that routine to execute as quickly as possible. For example, any driver that calls KeRaiseIrql should make the reciprocal call to KeLowerIrql as soon as it can.

For more information about determining priorities, see the Scheduling, Thread Context, and IRQL white paper.
```