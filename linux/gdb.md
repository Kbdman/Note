# gdb
## 常用命令
### 运行  
+ 从头开始运行      
run [commandline params]
+ 继续执行      
continue,c
+ 附加      
attach pid

+ 单步执行  
    + next ,n       
    执行一行源码，step over
    + step ,p
    执行一行源码,step in
    + nexti
    执行一条指令,step over
    + stepi
    执行一条指令，step in
+ 执行部分代码      
    + finish 执行到当前函数退出
    + until [位置] 执行到指定位置或者代码行号超过当前行号为止
### 断点   
+ 设置断点          
b/breakpoint position [if 条件]     
在满足条件的情况下执行指定位置的代码触发中断
position支持 函数符号,*地址，文件:行号，+/-代码偏移     
条件，是表达式，支持试用符号
+ 设置监视点        
watch/rwatch/awatch     
在数据被访问时中断
+ 删除断点或者监视点    
delete 断点编号     
clear  位置    
+ 忽视断点      
ignore 断点编号 忽视次数
+ 禁用/启用断点  
disable 断点编号    
enable  断点编号
+ 断点命令
    ```
    command 端点编号
    cmd ...
    end
    ```
    在指定编号的断点触发后执行
### 线程
+ 列出线程      
info thread
+ 切换到线程
thread 线程编号
### 调用堆栈  
+ 查看调用堆栈
bt,backtrace
+ 切换栈帧
frame 栈帧编号

### 数据操作
+ 查看变量
    print/p[/显示方式] 变量名/$寄存器名 
    显示方式：        
    + x 十六进制
    + d 带符号十进制数
    + u 不带符号十进制数
    + o 八进制
    + t 二进制
    + a Ascii字符
    + f 浮点树
    + s 字符串      

    printf formation *address 以指定格式打印地址指定的内容

+ 查看寄存器    
    + info reg [寄存器名称]
    + p $寄存器  
        + p的结果会被gdb存储下来，可以通过p $n来查看记录下的值
+ 查看内存数据
    x/格式  地址
+ 反汇编
    disassemble 符号/地址

+ 改变变量值    
set variable var = value
### 重复操作
command 执行次数
command 支持：
+ continue
+ step
+ stepi
+ next
+ nexti
### 生成转储文件  
generate-core-file
### 查看进程信息
+ info proc all         
列出/proc下的所有信息
+ info proc cmdline     
列出进程的命令行参数
+ info proc cwd     
列出进程的当前工作目录
+ info proc exe     
列出可执行文件的绝对路径
+ info proc mappings    
列出内存映射信息
+ info proc stat        
列出/proc/PID/stat下的信息
+ info proc status      
列出/proc/PID/status下的信息

### Set searching directory for source
```
set substitue-path ${old path in symbol} ${real path in host}
```   

### script
```
gdb -x ${script_file}
```