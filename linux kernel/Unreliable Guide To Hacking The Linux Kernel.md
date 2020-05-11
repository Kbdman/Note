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



可以通过in_softirq()来判断是不是在一个softirq或者tasklet中
```
Warning
Beware that this will return a false positive if a botton half lock is held.
```
基本规则
+ 没有内存保护      
无论你是在用户上下文还是中断上下文中破坏了内存，整个机器都会crash.
+ 没有浮点或者MMX       
FPU上下文不会被保存;即使是在用户上下文中，FPU的状态都有可能和当前进程不对应.如果你确实需要使用FPU，你需要显示的去保存和重建FPU的状态(并避免上下文切换)。这通常是个坏注意。有限试用整型算法
+ 固定的栈限制      
基于内核配置的设定，在大部分32位架构上，内核栈大概就是3K-6K大小，在大部分64位架构上，内核栈大概是14k.而且栈控件是与中断共享的，所以没有办法使用全部的栈，因此要避免深层递归和大型局部变量.
+ Linux内核是可移植的     
代码应当可以移植，大小端无关。应当经可能小的使用特定CPI相关的代码，比如内联汇编应当尽可能小而简洁以方便移植。通常这些代码应该被限制在内核树的架构相关部分



## ioctls: Not writing a new system call

系统调用通常看起来是这样
```
asmlinkage long sys_mycall(int arg)
{
        return 0;
}
```
在大部分情况下都不要创建新的系统调用，而是创建一个字设备，然后为他实现一个合适的ioctl.这比系统调用更加灵活，不需要修改每个架构的include/asm/unistd.h和arch/kernel/entry.S，而且也更可能被Linus接受。

如果你的所有过程都是读写某些参数，考虑实现一个sysfs()接口作为替代

在ioctl中，是处于一个进程的用户上下文中。当有错误出现时，你应该返回一个错误码，如果没有出错返回0。错误码，见include/uapi/asm-generic/errno-base.h, include/uapi/asm-generic/errno.h and include/linux/errno.h.

在休眠后，应当检查是否有信号出现：Unix/Linux处理信号的方式是以 -ERESTARTSYS 的方式暂时退出系统调用系统代码回到用户上下文中处理信号，如果用户没有禁止,重启系统调用。因此应当准备处理重启过程，例如你处在处理一些数据结构的过程中
```

if (signal_pending(current))
        return -ERESTARTSYS;
```
如果你要做更长的运算：首先考虑在用户空间中实现。如果你真的要在内核里进程大量的运算，你应当经常检查你是否应该放弃CPU：
```
cond_resched(); /* Will sleep */
```

A short note on interface design: the UNIX system call motto is “Provide mechanism not policy”.
Recipes for Deadlock

You cannot call any routines which may sleep, unless:

    You are in user context.
    You do not own any spinlocks.
    You have interrupts enabled (actually, Andi Kleen says that the scheduling code will enable them for you, but that’s probably not what you wanted).

注意一些函数会隐式的进行睡眠：通常这些是用户控件访问函数，而不带GFP_ATOMIC的内存申请函数

应当以CONFIG_DEBUG_ATOMIC_SLEEP编译内核，这会在你打破这些规则的时候提示你。

## 常用过程

### printk()      
include/linux/printk.h
printk将内核信息输出到控制台，dmesg,和syslog服务上。这对调试和错误报告很有用，并且可以在中断上下文中使用。不过用的时候需要注意，被printk信息占满屏幕的机器是无法使用的。它使用一个与ANSI C printf几乎兼容的格式化字符串，还有个参数来设定信息的优先级
```
printk(KERN_INFO "i = %u\n", i);
```
KERN_的值见kern_levels.h，它们代表这syslog中的level.
特例：输出IP地址
```

__be32 ipaddress;
printk(KERN_INFO "my ip: %pI4\n", &ipaddress);
```
printk内部使用了一个1k的buffer,并且不检查溢出。调用时要确保不会超出buffer




### copy_to_user() / copy_from_user() / get_user() / put_user()

include/linux/uaccess.h / asm/uaccess.h

[SLEEPS]
put_user和get_user用于从用户控件读取或者向用户空间写入一个值。永远不要直接通过地址来直接访问用户控件的数据。而是是用这些方法。它们都返回-EFAULT或者0

copy_to_user和copy_from_user更通用，用于在用户空间和系统空间之间互相拷贝数据。它们返回没有拷贝成功的数据量，0代表拷贝成功

The functions may sleep implicitly. This should never be called outside user context (it makes no sense), with interrupts disabled, or a spinlock held.
### kmalloc()/kfree()
include/linux/slab.h

[MAY SLEEP: SEE BELOW]

用来动态的申请指针对其的内存，就像用户空间里的malloc和free,不过kmalloc需要一个额外的flag参数
比较重要的Flag参数有：
+ GFP_KERNEL        
可能会休眠并换到swap,只能在用户上下文中使用。是最可靠的申请内存的方式
+ GFP_ATOMIC        
不会休眠，比GFP_KERNL相对不可靠些，但是可以在中断上下文中调用。需要处理好OOM的状况
+ GFP_DMA       
申请小于16MB的ISA DMA。如果不知道它是什么，就不需要用它。非常不可靠

如果看到一个休眠的函数被用从无效上下文中调用的警告消息，那可能是你从中断上下文中没有用GFP_ATOMIC调用kmalloc

如果是要申请大于或等于PAGE_SIZE的空间，考虑使用__get_free_pages().

如果要申请超过一页的空间，可以使用vmalloc，它会从kernel map中获取一个虚拟内存，他在物理上可能不是连续的，但是MMU会让他在地址空间里连续。运行中的内存分片状况使得获取物理上连续的大内存很难。获取物理上连续的大内存的的最好方法是通过alloc_bootmen在启动早期就获取


Before inventing your own cache of often-used objects consider using a slab cache in include/linux/slab.h
### current()
include/asm/current.h
在用户上下文时可以获得当前 task结构的指针。在中断上下文中不会返回NULL

### mdelay()/udelay()

include/asm/delay.h / include/linux/delay.h

小暂停

### cpu_to_be32()/be32_to_cpu()/cpu_to_le32()/le32_to_cpu()

include/asm/byteorder.h

字节序转换函数，变种有cpu_to_be32p 转换指针，cpu_to_be32s转换指针引用的值

### local_irq_save()/local_irq_restore()

include/linux/irqflags.h
在当前CPU禁用或者启用硬件中断，它们是可重入的。如果知道中断被以前哦嗯啦，可以简单的调用local_irq_disable，local_irq_enable


### local_bh_disable()/local_bh_enable()

include/linux/bottom_half.h

在当前CPU禁用启用软中断.可重入.如果之前中断就被禁用了，在disable,enable后，中断还是被禁用的，它们可以组织softirq和tasklets在当前cpu上运行

### smp_processor_id()

Defined in include/linux/smp.h

get_cpu() disables preemption (so you won’t suddenly get moved to another CPU) and returns the current processor number, between 0 and NR_CPUS. Note that the CPU numbers are not necessarily continuous. You return it again with put_cpu() when you are done.

If you know you cannot be preempted by another task (ie. you are in interrupt context, or have preemption disabled) you can use smp_processor_id().
__init/__exit/__initdata

Defined in include/linux/init.h

After boot, the kernel frees up a special section; functions marked with __init and data structures marked with __initdata are dropped after boot is complete: similarly modules discard this memory after initialization. __exit is used to declare a function which is only required on exit: the function will be dropped if this file is not compiled as a module. See the header file for use. Note that it makes no sense for a function marked with __init to be exported to modules with EXPORT_SYMBOL() or EXPORT_SYMBOL_GPL()- this will break.

### __initcall()/module_init()

Defined in include/linux/init.h / include/linux/module.h

Many parts of the kernel are well served as a module (dynamically-loadable parts of the kernel). Using the module_init() and module_exit() macros it is easy to write code without #ifdefs which can operate both as a module or built into the kernel.

module_init()宏定义了module被加载时被调用的函数，可以返回负数来让模块加载失败。在允许中断的用户上下文中被调用，可以它可以休眠

The module_init() macro defines which function is to be called at module insertion time (if the file is compiled as a module), or at boot time: if the file is not compiled as a module the module_init() macro becomes equivalent to __initcall(), which through linker magic ensures that the function is called on boot.

The function can return a negative error number to cause module loading to fail (unfortunately, this has no effect if the module is compiled into the kernel). This function is called in user context with interrupts enabled, so it can sleep.
### module_exit()

Defined in include/linux/module.h
用来只用模块卸载时被调用函数，可以休眠但是不能失败。只有模块的引用计数到0时才会被调用，必须在返回前清理所有资源
如果不指定卸载函数，则该模块无法被卸载，除了rmmod -f


### try_module_get()/module_put()

Defined in include/linux/module.h
这些会控制引用计数。在调用模块代码前，应当使用try_module_get，使用完调用module_put

Most registerable structures have an owner field, such as in the struct file_operations structure. Set this field to the macro THIS_MODULE.
### Wait Queues include/linux/wait.h

[SLEEPS]

A wait queue is used to wait for someone to wake you up when a certain condition is true. They must be used carefully to ensure there is no race condition. You declare a wait_queue_head_t, and then processes which want to wait for that condition declare a wait_queue_entry_t referring to themselves, and place that in the queue.
Declaring

You declare a wait_queue_head_t using the DECLARE_WAIT_QUEUE_HEAD() macro, or using the init_waitqueue_head() routine in your initialization code.
Queuing

Placing yourself in the waitqueue is fairly complex, because you must put yourself in the queue before checking the condition. There is a macro to do this: wait_event_interruptible() (include/linux/wait.h) The first argument is the wait queue head, and the second is an expression which is evaluated; the macro returns 0 when this expression is true, or -ERESTARTSYS if a signal is received. The wait_event() version ignores signals.
Waking Up Queued Tasks

Call wake_up() (include/linux/wait.h), which will wake up every process in the queue. The exception is if one has TASK_EXCLUSIVE set, in which case the remainder of the queue will not be woken. There are other variants of this basic function available in the same header.
## Atomic Operations

Certain operations are guaranteed atomic on all platforms. The first class of operations work on atomic_t (include/asm/atomic.h); this contains a signed integer (at least 32 bits long), and you must use these functions to manipulate or read atomic_t variables. atomic_read() and atomic_set() get and set the counter, atomic_add(), atomic_sub(), atomic_inc(), atomic_dec(), and atomic_dec_and_test() (returns true if it was decremented to zero).

Yes. It returns true (i.e. != 0) if the atomic variable is zero.

Note that these functions are slower than normal arithmetic, and so should not be used unnecessarily.

The second class of atomic operations is atomic bit operations on an unsigned long, defined in include/linux/bitops.h. These operations generally take a pointer to the bit pattern, and a bit number: 0 is the least significant bit. set_bit(), clear_bit() and change_bit() set, clear, and flip the given bit. test_and_set_bit(), test_and_clear_bit() and test_and_change_bit() do the same thing, except return true if the bit was previously set; these are particularly useful for atomically setting flags.

It is possible to call these operations with bit indices greater than BITS_PER_LONG. The resulting behavior is strange on big-endian platforms though so it is a good idea not to do this.
## Symbols

Within the kernel proper, the normal linking rules apply (ie. unless a symbol is declared to be file scope with the static keyword, it can be used anywhere in the kernel). However, for modules, a special exported symbol table is kept which limits the entry points to the kernel proper. Modules can also export symbols.
EXPORT_SYMBOL()

Defined in include/linux/export.h

This is the classic method of exporting a symbol: dynamically loaded modules will be able to use the symbol as normal.
EXPORT_SYMBOL_GPL()

Defined in include/linux/export.h

Similar to EXPORT_SYMBOL() except that the symbols exported by EXPORT_SYMBOL_GPL() can only be seen by modules with a MODULE_LICENSE() that specifies a GPL compatible license. It implies that the function is considered an internal implementation issue, and not really an interface. Some maintainers and developers may however require EXPORT_SYMBOL_GPL() when adding any new APIs or functionality.
EXPORT_SYMBOL_NS()

Defined in include/linux/export.h

This is the variant of EXPORT_SYMBOL() that allows specifying a symbol namespace. Symbol Namespaces are documented in Symbol Namespaces
EXPORT_SYMBOL_NS_GPL()

Defined in include/linux/export.h

This is the variant of EXPORT_SYMBOL_GPL() that allows specifying a symbol namespace. Symbol Namespaces are documented in Symbol Namespaces
## Routines and Conventions
Double-linked lists include/linux/list.h

There used to be three sets of linked-list routines in the kernel headers, but this one is the winner. If you don’t have some particular pressing need for a single list, it’s a good choice.

In particular, list_for_each_entry() is useful.
Return Conventions

For code called in user context, it’s very common to defy C convention, and return 0 for success, and a negative error number (eg. -EFAULT) for failure. This can be unintuitive at first, but it’s fairly widespread in the kernel.

Using ERR_PTR() (include/linux/err.h) to encode a negative error number into a pointer, and IS_ERR() and PTR_ERR() to get it back out again: avoids a separate pointer parameter for the error number. Icky, but in a good way.
Breaking Compilation

Linus and the other developers sometimes change function or structure names in development kernels; this is not done just to keep everyone on their toes: it reflects a fundamental change (eg. can no longer be called with interrupts on, or does extra checks, or doesn’t do checks which were caught before). Usually this is accompanied by a fairly complete note to the linux-kernel mailing list; search the archive. Simply doing a global replace on the file usually makes things worse.
Initializing structure members

The preferred method of initializing structures is to use designated initialisers, as defined by ISO C99, eg:

static struct block_device_operations opt_fops = {
        .open               = opt_open,
        .release            = opt_release,
        .ioctl              = opt_ioctl,
        .check_media_change = opt_media_change,
};

This makes it easy to grep for, and makes it clear which structure fields are set. You should do this because it looks cool.
GNU Extensions

GNU Extensions are explicitly allowed in the Linux kernel. Note that some of the more complex ones are not very well supported, due to lack of general use, but the following are considered standard (see the GCC info page section “C Extensions” for more details - Yes, really the info page, the man page is only a short summary of the stuff in info).

    Inline functions
    Statement expressions (ie. the ({ and }) constructs).
    Declaring attributes of a function / variable / type (__attribute__)
    typeof
    Zero length arrays
    Macro varargs
    Arithmetic on void pointers
    Non-Constant initializers
    Assembler Instructions (not outside arch/ and include/asm/)
    Function names as strings (__func__).
    __builtin_constant_p()

Be wary when using long long in the kernel, the code gcc generates for it is horrible and worse: division and multiplication does not work on i386 because the GCC runtime functions for it are missing from the kernel environment.
C++

Using C++ in the kernel is usually a bad idea, because the kernel does not provide the necessary runtime environment and the include files are not tested for it. It is still possible, but not recommended. If you really want to do this, forget about exceptions at least.
#if

It is generally considered cleaner to use macros in header files (or at the top of .c files) to abstract away functions rather than using `#if’ pre-processor statements throughout the source code.
Putting Your Stuff in the Kernel

In order to get your stuff into shape for official inclusion, or even to make a neat patch, there’s administrative work to be done:

    Figure out whose pond you’ve been pissing in. Look at the top of the source files, inside the MAINTAINERS file, and last of all in the CREDITS file. You should coordinate with this person to make sure you’re not duplicating effort, or trying something that’s already been rejected.

    Make sure you put your name and EMail address at the top of any files you create or mangle significantly. This is the first place people will look when they find a bug, or when they want to make a change.

    Usually you want a configuration option for your kernel hack. Edit Kconfig in the appropriate directory. The Config language is simple to use by cut and paste, and there’s complete documentation in Documentation/kbuild/kconfig-language.rst.

    In your description of the option, make sure you address both the expert user and the user who knows nothing about your feature. Mention incompatibilities and issues here. Definitely end your description with “if in doubt, say N” (or, occasionally, `Y’); this is for people who have no idea what you are talking about.

    Edit the Makefile: the CONFIG variables are exported here so you can usually just add a “obj-$(CONFIG_xxx) += xxx.o” line. The syntax is documented in Documentation/kbuild/makefiles.rst.

    Put yourself in CREDITS if you’ve done something noteworthy, usually beyond a single file (your name should be at the top of the source files anyway). MAINTAINERS means you want to be consulted when changes are made to a subsystem, and hear about bugs; it implies a more-than-passing commitment to some part of the code.

    Finally, don’t forget to read Documentation/process/submitting-patches.rst and possibly Documentation/process/submitting-drivers.rst.

Kernel Cantrips

Some favorites from browsing the source. Feel free to add to this list.

arch/x86/include/asm/delay.h:

#define ndelay(n) (__builtin_constant_p(n) ? \
        ((n) > 20000 ? __bad_ndelay() : __const_udelay((n) * 5ul)) : \
        __ndelay(n))

include/linux/fs.h:

/*
 * Kernel pointers have redundant information, so we can use a
 * scheme where we can return either an error code or a dentry
 * pointer with the same return value.
 *
 * This should be a per-architecture thing, to allow different
 * error and pointer decisions.
 */
 #define ERR_PTR(err)    ((void *)((long)(err)))
 #define PTR_ERR(ptr)    ((long)(ptr))
 #define IS_ERR(ptr)     ((unsigned long)(ptr) > (unsigned long)(-1000))

arch/x86/include/asm/uaccess_32.h::

#define copy_to_user(to,from,n)                         \
        (__builtin_constant_p(n) ?                      \
         __constant_copy_to_user((to),(from),(n)) :     \
         __generic_copy_to_user((to),(from),(n)))

arch/sparc/kernel/head.S::

/*
 * Sun people can't spell worth damn. "compatability" indeed.
 * At least we *know* we can't spell, and use a spell-checker.
 */

/* Uh, actually Linus it is I who cannot spell. Too much murky
 * Sparc assembly will do this to ya.
 */
C_LABEL(cputypvar):
        .asciz "compatibility"

/* Tested on SS-5, SS-10. Probably someone at Sun applied a spell-checker. */
        .align 4
C_LABEL(cputypvar_sun4m):
        .asciz "compatible"

arch/sparc/lib/checksum.S::

/* Sun, you just can't beat me, you just can't.  Stop trying,
 * give up.  I'm serious, I am going to kick the living shit
 * out of you, game over, lights out.
 */

Thanks

Thanks to Andi Kleen for the idea, answering my questions, fixing my mistakes, filling content, etc. Philipp Rumpf for more spelling and clarity fixes, and some excellent non-obvious points. Werner Almesberger for giving me a great summary of disable_irq(), and Jes Sorensen and Andrea Arcangeli added caveats. Michael Elizabeth Chastain for checking and adding to the Configure section. Telsa Gwynne for teaching me DocBook.

==================================================================
https://www.kernel.org/doc/html/latest/kernel-hacking/hacking.html