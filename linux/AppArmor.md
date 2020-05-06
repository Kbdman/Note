# AppArmor

进程在调用系统调用前，内核会调用appArmor来检查进程是否有足够的权限

AppArmor为每个进程都应用一组规则，这些规则由内核基于执行的应用程序的安装位置来设定。而不是想SELinux基于用户来设定。

任意用户在执行同一程序时，AppAmor设定的规则都是一致的

AppArmor的配置都存放在/etc/apparmor.d中，其中还包含了一组对资源的访问控制规则。这些规则会有apparmor_parser编译并加载到内核个中

AppArmor 可以执行在enforce模式或者complain模式
+ enforce模式       
    下对超出权限的行为会阻止并log
+ complain模式下    
    对超出权限的行为不组织，但是会留下log

可以使用aa-enforce或者aa-complain来切换指定profile的工作模式

可以使用aa-disable或者aa-audit来禁用或者启用profile
 
大部分的程序都没有对应的profile,可以试用aa-unconfined来查看没有profile，且有活跃的网络sokect的程序，试用--paranoid可以列出所有的在运行的程序