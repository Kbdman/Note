# Unreliable Guide To Hacking The Linux Kernel
 
在任意时刻，系统中的CPU可以是以下几种状况之一：
+ 与任何进程没有关系，服务于一个硬件中断
+ 与任何进程没有关系，服务于一个软中断
+ 运行在内核空间中，与某个进程有关
+ 运行在一个进程的用户空间中
  
下面两个可以互相抢占，而上面的有严格的优先级关系，只有上面的可以抢占下面的

## 用户上下文
User context is when you are coming in from a system call or other trap: like userspace。运行在用户上下文时，可能会被更重要的任务或者终端抢占。在用户上下文可以用schedule()进行sleep
> 在模块load unload，以及操作块设备层时，你总是在用户上下文中

在用户上下文时，current指针(指向当前执行的任务)是有效的。in_interrupt()返回false.
> 注意：如果禁用了抢占或者软中断，in_interrupt()会返回一个false positive（返回正值，但逻辑是错误的）。

## 硬件中断
内核运行着中断处理过程。内核保证中断处理过程是不会重入的，如果在中断处理的过程中，有相同的中断出现，他将会被排队或者丢弃。由于在中断处理过程中中断被禁用了，因此中断处理过程必须要快。通常是记录下中断的信息，生成一个执行用的软件中断，然后就退出
在硬件中断中in_irq()返回true

## 软件中断
当系统调用将用回到用户空间，或者硬件中断处理过程退出，所有被标记为pending的软件中断都会被执行。

很多真正处理中断的工作都是在软中断处理中完成。

linux.interrupt.h中列出了不同的softirqs。一个非常重要的softirq就是计时器softirq.可以通过注册让他在指定长的时间内调用指定的函数
SoftIrqs通常难以处理，因为同一个softirq可能会同事在做个CPU上执行。因此，tasklet用的更多。

### tasklet
tasklet是可动态注册的，并且保证任一tasklet在任何事件都只会在一个CPU上运行，而不同

==================================================================
https://www.kernel.org/doc/html/latest/kernel-hacking/hacking.html