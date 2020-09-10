# msf
## msfconsole
msfconsole是Metasploit框架中最常用的用户接口
他提供一个交互式的命令行接口，同时也支持脚本操作
能用于访问Metasploit中绝大部分功能
### msf使用Postgres数据库
+ 连接数据库        
  ```
  msf>db_connect user:pw@host/dbname
  ```

+ 查看数据库状态    
  ```
  db_status
  ```
### msf使用模块
```
use path/to/module
```
之后会进入指定的模块的命令行    
使用 show options 命令可以查看模块的参数    
使用 set PARAMETER VALUE 可以设置参数的数值
使用 run 运行模块


#### 模块
1. auxiliary/scanner/ip/ipidseq   
   冒充网络上的其他机器对目标机器进行扫描
```
set RHOSTS 192.168.1.0/24 #设置扫描区间
set THREADS 50            #设置扫描线程数
run
```
   
