# Qemu搭建linux内核调试环境
## 编译内核
下载内核源码        
进入源码目录        
修改配置，如
```
make menuconfig
```
启用 Compile the kernel with debug info和Provide GDB scripts for kernel debug选项       
make bzImage
>bzImage是引导的压缩内核，vmlinux 包含了内核的符号信息

## 构建initramfs
在加载内核后，会挂载initramfs到/然后执行其中的init脚本
### 通过busybox构建initramfs
busybox是一组unix工具的集合,可以做成initramfs，在加载内核使用
1. 编译静态版本busybox
```
$ cd busybox-1.28.0
$ make menuconfig

Settings  --->
    [*] Build static binary (no shared libs)

$ make -j 20
$ make install
```
2. 组织initramfs
```
$ mkdir initramfs
$ cd initramfs
$ cp ../_install/* -rf ./
$ mkdir dev proc sys
$ sudo cp -a /dev/{null, console, tty, tty1, tty2, tty3, tty4} dev/
$ rm linuxrc
$ vim init
$ chmod a+x init
$ ls
$ bin   dev  init  proc  sbin  sys   usr
```
init内容，挂载/proc /sys
```
#!/bin/busybox sh         
mount -t proc none /proc  
mount -t sysfs none /sys  

exec /sbin/init
```
3. 打包initramfs
```
$ find . -print0 | cpio --null -ov --format=newc | gzip -9 > ../initramfs.cpio.gz
```
## 调试
1. 用qemu启动内核
    ```
    $ qemu-system-x86_64 -s -kernel /path/to/vmlinux -initrd /path/to/initramfs -nographic -append "console=ttyS0 nokaslr" -S
    ```
    参数：      
    + -s -gdb tcp::1234的缩写，指定可以通过tcp:1234进行调试
    + -nographic 取消图形输出窗口
    + -kernel 指定内核
    + -initrd 指定initramfs
    + -append 指定内核命令行参数，console=ttyS0 将输出重定向到console，将会显示在标准输出stdio nokaslr内核不启用aslr
    + -s 加载后不直接运行，可由调试器恢复运行

2. gdb连接调试  
    ```
    gdb path/to/bzImage     
    (gdb)file path/to/vmlinux
    (gdb)b start_kernel
    ...
    (gdb)c
    ```     
    > 在没有使用nokaslr启动内核时，内核可能被加载到随机的地址上，而file加载符号只是从文件中读取符号，不会根据当前的内存映射关系去调整符号信息，这使得调试器无法根据符号下断点或者获得运行的正确信息