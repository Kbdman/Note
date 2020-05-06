# tips
## crontab
crontab除了编辑文件，还可以直接把脚本放到/etc/monthly daily等目录下

每个用户可以通过crontab -e来编写自己的调度文件，文件存放在
/var/spool/cron/crontabs/user中

crontab中除了用数字来控制事件，也可以用
@yearly @monthly @weekly @daily @hourly来控制时间
 
可以用 
at HH::mm DD.MM.YY cmd 来设定定时任务
 
## rsync
rsync使用多个硬连接来备份文件，节省磁盘空间
 
## netstat
netstat列出所有连接 -t tcp -u 类似udp的连接， -a 等待连接的 -n 以数值方式显示，不显示域名
-p列出进程  -c持续刷新连接列表
 
## nmap
nmap测试远端机器的网络
 
## raid
+ 线性raid是将多个磁盘组织成一个大的虚拟磁盘
+ raid0是为了增强性能，在存储数据时会将数据分成多个部分分到每块物理磁盘上，相当于并联了磁盘，提高了带宽
+ raid1，使用2块相同大小的物理磁盘，一块是另外一块的镜像，当一块磁盘出错，另外一块磁盘有用

在raid1中，如果有3块磁盘，前2块磁盘是数据镜像，第三块磁盘是备用磁盘，当前2块磁盘有错误时，raid会在第三块磁盘上构造数据镜像
 
 
## LXC 
LXC Linux Container,将进程按组隔离,并不是正真意义的虚拟机，每个组都有各自的配置，包括硬件，但是它们都用同一个内核
 
## FireWall
Linux内置了防火墙netfilter, iptable ip6table是用户层接口
 
## AppArmor
进程在调用系统调用前，内核会调用appArmor来检查进程是否有足够的权限

## Xorg
X.org是提供GUI程序显示窗口的组件，它提供了一组标准化接口给GUI程序

## LVM
Logica Volume Manager 逻辑卷管理

### LVM的概念
+ PV物理卷   
    可以是磁盘上的分区，一整块磁盘或者其他的块设备
+ VG卷组   
    由一组PV构成
+ LV逻辑卷   
    在VG上的分区

## inetd
inetd是一个用于管理不需要持续运行的服务的服务

它会监听它所管理的所有服务的接口，挡有连接到达这些端口时，inetd会唤起所需的服务

 
  
## ldconfig
ldconfig命令读取搜索/etc/ld.so.conf包含的文件里的目录，找出库文件，生成/etc/ld.so.cache.

loader在加载程序时会根据ld.so.cache来找到程序所需的动态库来加载

如果在ld.so.conf的目录下新增了库，如需使用这些库，需要试用ldconfig更新缓存，或者使用环境变量LD_LIBRARY_PATH来新增搜索目录
 
## find 
+ -name     
根据文件名查找，区分大小写
+ -iname        
根据文件名查找，不区分大小写
+ -exec         
后面可以接命令，对找出的文件进行操作

示例
   > find . -iname "*.txt" -exec chmod -x {} \
    {}代表find的每一行标准输出
 
## apt
apt-get build-dep 软件包，安装构建指定软件包所需要的依赖，需要在源中指定deb-src
 
## ar 
+ ar t archive      
列出归档中的文件列表
+ ar x archive      
将文件从归档中提取到当前目录
+ ar d archive file         
将file从归档中删除
 
### 通过ar来安装deb包
ar x dpkg_1.17.23_amd64.deb
tar -C / -p -xzf data.tar.gz﻿​
 
### Debian软件源sections
+ main      
完全符合Debian规则的自由软件包
+ nonfree       
没有完全遵循Debian规则，但是可以不受限制的分发的包
+ contrib       
需要使用nonfree组件的开源软件包，或者需要用商业工具编译的软件包
 
## 选择列表安装
1. 可以用dpkg --get-selection >file生成一个本机器安装的清单

2. 然后在另外机器上试用dpkg --set-selection设置清单

3. 在调用apt-get dselect-upgrade 来安装和之前那台一样的环境
```
#从apt-cache获取数据更新dpkg数据库的的数据
avail=`mktemp`↵
apt-cache dumpavail > "$avail"↵
dpkg --merge-avail "$avail"↵
rm -f "$avail"↵
#更新dpkg的selection↵
dpkg --set-selections < pkg-list↵
Ask apt-get to install the selected packages↵
apt-get dselect-upgrade
```
## 文档
软件包的文档通常都存放在/usr/share/doc下  以包名 命名
 
## 文件名编码转换
文件名编码可以用convmv来转换，简单的文本文件可以使用recode来转码
 
## 用户信息
/usr/passwd存放这用户列表
/usr/shadow存放这加密后的密码
 
### 禁用用户登录
    passwd -l 用户名
### 改密码
    passwd [用户名]
 
## Boot相关
### MBR
MBR位于第一块磁盘的前512字节，Bios启动后加载MBR，并将控制权交给MBR。通常在安装MBR时会擦除之前的MBR

### udev
udev大概是管理uPnp设备的服务，会根据设备的出现或者消失自动在/dev/中为设备创建文件
 
### 磁盘相关
磁盘分区编号从1开始
 
Dos分区格式不支持2t以上的磁盘，且不能有4个以上的主分区

GPT是新的磁盘分区模式，支持大容量磁盘
 
在/dev/disk/by-id下保存这磁盘的名字到sd*的映射关系

### bootloader 
#### lilo
lilo是老的boot loader，它将要引导的内核的物理地址写到了mbr,所以每次更新后都要重新执行lilo，否则可能会无法启动，很不灵活

Lilo的配置文件
 boot lilo安装到的磁盘
root 根目录所在分区
default=Linux  默认启动项
下面是启动项配置
 image=/vmlinux 启动镜像
   label=启动项名
    initrd=initrd镜像
#### grub
grub2可以识别文件系统，安装新内核后可以只需要更新配置文件，而不需要重新安装grub

grub2的配置文件/etc/grub/grub.cfg，不应手动修改，否则在下次生成时改动会被覆盖。
要新建条目可以新建 /boot/grub/custom.cfg 文件或修改 /etc/grub.d/50_custom 文件
 
### 双系统时间问题
应把 CMOS 时钟配置成 UTC (把机码 HKLM\SYSTEM\CurrentControlSet\Control\TimeZoneInformation\RealTimeIsUniversal 设置为 “1” 做为 DWORD)，或使用 Debian 系统的 hwclock --localtime --set 设置硬件时钟并标记追踪其为本地时间 (并且在春秋两季手段检查时钟的正确性)。    
 
### boot相关

Debian 将内核镜像 vmlinuz-version 其配置 config-version 和符号表System.map-version放在boot中
配置脚本自动生成initrd( Init Ramdisk).
Initrd是一个运行在内存中的迷你系统，由bootloader加载。linux内核用它来加载访问存放完整debian系统的设备所需的模块。安装完成后post-install脚本更新/vmlinuz /vmlinuz.old /initrd.img /initrd.img.old的符号连接
 
#### systemd boot顺序 
BIOS -》 bootloader ->kernel ->iniramfs->systemd->启动其他服务

##### 日志
journalctl用来访问systmd-journald.service写的systemd日志，不带参数会返回日志的所有内容 
-u指定日志相关的unit
#### inited boot顺序
BIOS -》 bootloader ->kernel ->iniramfs->init  init根据级别执行相应目录下的启动脚本
 
## 权限
suid，sgid ,可执行文件设置了suid/sgid后，执行的人会以文件所有人/文件所属组的身份去执行

sgid可以用于目录，设置了sgid的目录下创建的项自动属于该目录所属组
 
粘着位，设定了的目录下的文件只有所有人或者父目录的所有人可以删除
 
如果要设置setuid setgid sticky,可以通过chmod 四位数字来指定，第一个数字为特殊权限，setuid 4 setgid 2 sticky 1
 
chown user:grp file，可以一次新改变文件的用户和组

## wayland
wayland是个描述了显示服务器和其client交互的协议，C语言实现

wayland的主要目标是用一个更现代更简单的窗口系统来替代X Window系统

## lsof
lsof 列出打开的文件
+ -i           
打开的接口  
+ -i 6        
打开的ipv6接口
+ -i@ip    
与指定ip有关的接口
+ -i   -sTCP:LISTEN  
监听状态的tcp接口
+ -i   -sTCP:ESTABLISHED  
已经建立连接的tcp接口
 
+ -u 用户名  
列出指定用户打开的文件
+ -u ^用户名 
指定用户名之外文件用的文件
+ -t 
只列出进程号，可以通过这个来关闭用户运行的所有程序
>kill -9 'lsof -t -u aaa'
 
+ -c    
指定命令打开的文件
+ -p    
指定pid

## perf
linux 下的性能分析工具
## 查看某个进程的执行过程
perf stat
## 查看当前系统的性能状况
perf top
## 记录下某个进程的执行消耗，然后查看
perf record cmd

perf report