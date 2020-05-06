# Debian包

## 源码包
源码包通常由三个文件构成

+ .dsc      
Debian source Control ,就像二进制包里的control文件
+ .orig.tar.gz          
原始源码软件包，由原始开发者提供，debian的维护者不会修改原始开发者提供的软件包
+ .debian.tar.gz(或者.diff.gz)      
Debian维护者做的修订    
 
源码包也会有依赖一如编译所需工具
+ Duild-Depends:编译依赖
+ Source：原始包名
+ Binrary:    编译生成的软件包清单，一个原始软件包可以生成多个可运行软件包
使用dpkg-source命令可以从源码包中解出源码
 
 
Debian所有的软件包都是由源码包生成而来，而所有修改也是针对源码包，Debian维护这修改源码包，在编译成二进制报。当源码包被上传到Debian的服务器上时，会被自动生成多个架构的二进制包。通常维护者也会上传一个二进制包


## 二进制包

Debian包的ar存档格式由三个文件组成
+ debian-binary     
文本文件，写了使用的deb文件版本，如2.0
+ control.tar.gz        
包含了软件包的元信息，系统通过这些信息来处理软件包，安装卸载，依赖处理等
+ data.tar.gz       
软件包需要解压的文件，包括可执行文件，文档等，压缩的格式不一定是gz          
 
 
### 软件包元信息 
主要是描述了当前软件包与其他Debian软件包的关系，如依赖或者冲突建议。还提供了在软件包安装删除升级过程中可能用到的脚本

使用apt-cache show 包名  可以查看指定包的元信息
```
apt包的元信息
Package: postfix 
Version: 3.4.8-0+10debu1 
Architecture: amd64 
Maintainer: LaMont Jones <lamont@debian.org> 
Installed-Size: 4333 
Depends: libc6 (>= 2.14), libdb5.3, libicu63 (>= 63.1-1~), libsasl2-2 (>= 2.1.27+dfsg), libssl1.1 (>= 1.1.1), debconf (>= 0.5) | debconf-2.0, netbase, adduser (>= 3.48), dpkg (>= 1.8.3), lsb-base (>= 3.0-6), ssl-cert, cpio, e2fsprogs 
Recommends: python3 
Suggests: procmail, postfix-mysql, postfix-pgsql, postfix-ldap, postfix-pcre, postfix-lmdb, postfix-sqlite, sasl2-bin | dovecot-common, libsasl2-modules | dovecot-common, resolvconf, postfix-cdb, mail-reader, ufw, postfix-doc 
Conflicts: mail-transport-agent, smail 
Replaces: mail-transport-agent 
Provides: mail-transport-agent 
Homepage: http://www.postfix.org 
Priority: optional 
Section: mail 
Filename: pool/main/p/postfix/postfix_3.4.8-0+10debu1_amd64.deb 
Size: 1516404 
SHA256: 66cec0f60cdd03de62dcc62ca4ceb73d585be1ab2fcc8728defaf0583360c6a2 
SHA1: 39ba80522d1f8607ef7357cdfadf4592af390aa1 
MD5sum: 78912874373f5375f193ffe3c9862a51
Description: High-performance mail transport agent
Postfix is Wietse Venema's mail transport agent that started life as an
alternative to the widely-used Sendmail program. Postfix attempts to
be fast, easy to administer, and secure, while at the same time being
sendmail compatible enough to not upset existing users. Thus, the outside
has a sendmail-ish flavor, but the inside is completely different.
Description-md5: 98656fbf1bb45b53edd367037a2e5a90
```
 
重要的字段      
+ Depends        
    指明依赖
+ Conflicts      
    冲突,不能同时存在的包，包括例如同名，或者试用相同端口等
+ Break      
    中断,在安装过程中会中断的软件包
 
 
在依赖冲突break中指定的软件包，除了可以指定显示中的软件包，也可以指明虚拟软件包。

元信息中的Provide字段，可以表明自己提供的一个虚拟软件包。
 
### 虚拟软件包的意义
可能有多个包如A,B提供同样的服务，而某个包C需要一个这样的服务，他不关心服务是由A提供还是B提供，这种状况下A，B可以同时Provide一个虚拟软件包V

而C在描述自己的依赖的时候只需要依赖V。而后续当有更多的包Provide V之后软件包C也不需要更改
Provide虚拟软件包，也可以指定Provide的版本，如
Provides: libdigest-md5-perl (= 1.8)。   
 
 
### 一些规则
相同Provide的软件包之间是互相是冲突的
 
Replace字段指当前软件包中包含了其他一些软件包中同样包含了的文件
 
在指定软件包的版本时 试用软件包名(版本描述) libc6( >= 2.15)  版本描述可以 << >> = <= >= 等 2.6.1-1不等于2.6.1
'，'表示并 ，"|"或，“|”优先级高于‘,'
 
推荐，建议，不是强制的依赖，推荐比建议更重要，通常推荐是指满足后能大幅提升性能，建议满足能提供一些功能的增补
 
Tag字段备注信息
 
/var/lib/dpkg/info/中保存着所有已安装的包的配置脚本。同时还有一个.list后缀的文件表明属于这个包的文件

/var/lib/dpkg/status 文件中保存这包的状态信息，包括从controls中读出的信息。UOS好像没有这个目录

### 配置脚本
.preinst .postinst  dpkg在安装软件包前执行该软件包的preinst,在安装后执行该软件包的postinst
.prerm .postrm   dpkg在删除软件包前执行该软件包的prerm,在删除后执行该软件包的postrm
 
### 升级
就是删除旧版本软件包，安装新版本软件包
 
1. 在升级前 dpkg调用 旧版本额的 prerm upgrade 新版本
2. 然后调用 新版本软件包的 preinst upgrade旧版本
3. 解压新软件包中的文件，如果文件存在则替换旧文件，旧文件做了个临时备份
4. dpkg执行旧包的old-postrm upograde 新版本
5. dpkg更新内部数据，如文件列表，控制脚本等，并移除备份文件
6. dpkg更新配置文件，如果无法自动处理，要求用户处理
7. dpkg调用新版本的postinst configure 版本号进行配置
 
### 新安装
1. 安装前dpkg执行preinst
2. 解压替换文件，备份被替换的文件
3. 更新内部数据
4. 更新配置文件
5. 调用postinst configure 版本号进行配置
 
### 移除
1. dpkg 调用prerm
2. 删除软件包文件，只留下配置文件和脚本
3. 执行post remove移除配置脚本，保留postrm,如果未指明清除则到此结束
4. 如果指明purge则进行purge
 
debian软件包在移除时通常都会留下配置文件已被下次使用，如果要删除这些，加purge选项
 
 
### CheckSum
Md5Sums dpkg支持试用md5对软件包完整性进行校验
### Confiles
Confiles  被当作成配置文件处理的文件，当版本变更的时候，如果这些文件未改动则安装新版本，如果有改动则尝试将改动保存到新版本的该文件中，Dpkg可能会询问试使用那个版本的配置文件
 