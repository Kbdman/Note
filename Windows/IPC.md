# IPC
## NamedPipe 命名管道
+ CreateNamedPipeA      
创建命名管道，可以是单项的或者双向的
+ ConnectNamedPipe      
等待连接
+ CallNamedPipe     
连接命名管道
+  WaitNamedPipe    
等待创建命名管道
+ WaitXXXX  
等待管道可读
+ ReadFile/WriteFile    
读写管道       
## 文件映射内存
1. CreateFile打开文件
2. CreateFileMapping        
    创建Map对象     
    如果传入的文件句柄是INVALID_HANDLE_VALUE,则使用映射分页文件，也就是可以不用映射已知文件。       
    参数中有指定名称，如果名称和某个Map对象相同则获取改对象，如果和某个个Mutex,Semaphore相同则失败
3. MapViewOfFile        
将Map对象的部份映射到地址空间中

## MailSlot     
单向IPC,一个进程注册一个MailSlot作为服务器，而其他进程可以向这个MailSlot种发送数据，MailSlot的所有者可以获得读取邮件操种的数据

+ CreateMailslot        
创建邮件Slot
+ WriteFile
写数据到MailSlot
+ GetMailslotInfo       
获取MailSlot种信息的状态
+ ReadFile      
从邮件槽中读取

## 网络

## RPC