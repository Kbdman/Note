# I/O Completion Ports

## 概述
I/O完成端口，提供了一种在与之关联的一组文件的i/o操作完成后唤醒不超过特定数量的线程进行的处理
处理线程通过GetQueuedCompletionStatus来与I/O端口关联，并休眠，直到需要时候被唤醒。

I/O完成端口提供了一种在多处理器系统上处理多个异步I/O请求的线程模型。当一个进程创建了一个I/O完成端口时，系统请求创建了一个关联的队列对象来服务请求。相较于收到I/O请求再创建线程处理，通过I/O完成端口和一个预先分配的线程池，处理器可以更加快速而有效的同时处理多个异步I/O请求。

CreateIoCompletionPort函数创建了一个I/O完成端口，并将一个或者多个文件与这个端口相关联，当由与这些文件有关的I/O完成时，会在端口的队列中创建一个I/O完成包

The CreateIoCompletionPort function creates an I/O completion port and associates one or more file handles with that port. When an asynchronous I/O operation on one of these file handles completes, an I/O completion packet is queued in first-in-first-out (FIFO) order to the associated I/O completion port. One powerful use for this mechanism is to combine the synchronization point for multiple file handles into a single object, although there are also other useful applications. Please note that while the packets are queued in FIFO order they may be dequeued in a different order.

Note

The term file handle as used here refers to a system abstraction representing an overlapped I/O endpoint, not only a file on disk. For example, it can be a network endpoint, TCP socket, named pipe, or mail slot. Any system object that supports overlapped I/O can be used. For a list of related I/O functions, see the end of this topic.

 

When a file handle is associated with a completion port, the status block passed in will not be updated until the packet is removed from the completion port. The only exception is if the original operation returns synchronously with an error. A thread (either one created by the main thread or the main thread itself) uses the GetQueuedCompletionStatus function to wait for a completion packet to be queued to the I/O completion port, rather than waiting directly for the asynchronous I/O to complete. Threads that block their execution on an I/O completion port are released in last-in-first-out (LIFO) order, and the next completion packet is pulled from the I/O completion port's FIFO queue for that thread. This means that, when a completion packet is released to a thread, the system releases the last (most recent) thread associated with that port, passing it the completion information for the oldest I/O completion.

任意数量的线程可以调用GetQueuedCompletionStatus，调用了这个函数的线程都会与完成端口关联，直到线程退出，或者与其他I/O端口关联，或者关闭了I/O完成端口。也就是说，在同一个时间，一个线程只能关联到一个I/O完成端口。


当一个完成包被放入完成端口的队列，系统首先检查，这个端口有多少个线程再运行，当少于特定值的时候，创建一个线程来处理完成的packet.当一个线程完成了它的处理后，它通常调用GetQueuedCompletionStatus来获取下一个packet来处理，如果没有则等待。

线程可以使用PostQueuedCompletionStatus来将一个packet放入端口的队列中。完成端口可以通过这个来接受别的线程的信息。PostQueuedCompletionStatus允许一个应用讲一些与io没有关联的packet放入队列中已完成特定的目的。这可以用来通知工作线程一些外部事件。

完成端口的句柄和与之关联的文件的句柄都引用了，完成端口。完成端口只有再引用清0后才会被释放，因此要释放完成端口要先讲相关文件句柄都关闭，在关闭完成端口句柄

>I/O完成端口与创建它的进程关联，无法在进程间共享，但是可以在同个进程的多个进程中共享


 
## 线程和一致性

The most important property of an I/O completion port to consider carefully is the concurrency value. The concurrency value of a completion port is specified when it is created with CreateIoCompletionPort via the NumberOfConcurrentThreads parameter. This value limits the number of runnable threads associated with the completion port. When the total number of runnable threads associated with the completion port reaches the concurrency value, the system blocks the execution of any subsequent threads associated with that completion port until the number of runnable threads drops below the concurrency value.

The most efficient scenario occurs when there are completion packets waiting in the queue, but no waits can be satisfied because the port has reached its concurrency limit. Consider what happens with a concurrency value of one and multiple threads waiting in the GetQueuedCompletionStatus function call. In this case, if the queue always has completion packets waiting, when the running thread calls GetQueuedCompletionStatus, it will not block execution because, as mentioned earlier, the thread queue is LIFO. Instead, this thread will immediately pick up the next queued completion packet. No thread context switches will occur, because the running thread is continually picking up completion packets and the other threads are unable to run.

Note

In the previous example, the extra threads appear to be useless and never run, but that assumes that the running thread never gets put in a wait state by some other mechanism, terminates, or otherwise closes its associated I/O completion port. Consider all such thread execution ramifications when designing the application.

 

The best overall maximum value to pick for the concurrency value is the number of CPUs on the computer. If your transaction required a lengthy computation, a larger concurrency value will allow more threads to run. Each completion packet may take longer to finish, but more completion packets will be processed at the same time. You can experiment with the concurrency value in conjunction with profiling tools to achieve the best effect for your application.

The system also allows a thread waiting in GetQueuedCompletionStatus to process a completion packet if another running thread associated with the same I/O completion port enters a wait state for other reasons, for example the SuspendThread function. When the thread in the wait state begins running again, there may be a brief period when the number of active threads exceeds the concurrency value. However, the system quickly reduces this number by not allowing any new active threads until the number of active threads falls below the concurrency value. This is one reason to have your application create more threads in its thread pool than the concurrency value. Thread pool management is beyond the scope of this topic, but a good rule of thumb is to have a minimum of twice as many threads in the thread pool as there are processors on the system. For additional information about thread pooling, see Thread Pools.
Supported I/O Functions

The following functions can be used to start I/O operations that complete by using I/O completion ports. You must pass the function an instance of the OVERLAPPED structure and a file handle previously associated with an I/O completion port (by a call to CreateIoCompletionPort) to enable the I/O completion port mechanism:

    ConnectNamedPipe
    DeviceIoControl
    LockFileEx
    ReadDirectoryChangesW
    ReadFile
    TransactNamedPipe
    WaitCommEvent
    WriteFile
    WSASendMsg
    WSASendTo
    WSASend
    WSARecvFrom
    WSARecvMsg
    WSARecv

Related topics

About Processes and Threads

BindIoCompletionCallback

CreateIoCompletionPort

GetQueuedCompletionStatus

GetQueuedCompletionStatusEx

PostQueuedCompletionStatus
