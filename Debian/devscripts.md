# devscripts
devscripts package 包含了很多帮助Debian开发者工作的工具
## debuild 
    通过dpkg-buildpackage生成一个包，并运行lintian来检查与Debian Policy的兼容性

## debclean 
在二进制包生成后清理源码包
## dch
在源码包中快速方便的编辑debian/changelog.
## uscan
检查上游开发者是否发布了新版本的源码包。这需要在debian/watch中配置检查新release的位置
## debi debc
 dpkg -i

 dpkg -c

## bts
以命令行的方式控制bug追踪系统，这个程序会自动生成合适的email

## debrelease
将一个刚生成的包上传到一个远端的服务器，不需要输入全名，以及相关的.change文件路径
## debsign
对.dsc和.changes签名

## uupdate 
将新上游版本发布后，新版本包的创建自动化
