# 为apt创建一个repo
启动一个http Server，将/srv/vhosts/packages/ 作为http服务的根目录

归档的管理是委托给mini-dinstall.它会监视incoming目录，并等待这里的新包。当有一个新的包被上传啦，它将会被安装打破/ser/vhosts/packages的Debian归档中。mini-dinstall读取Debian包创建时创建的.changes文件。这个文件包含了与包版本有关的所有其他文件的列表，这使得 mini-dinstall知道要安装哪些文件。.change文件同样也在debian/changelog的最后一条中提到了目标版本（通常是unstable），mini-dinstall用这些信息来判断包应该安装到的位置。这就是管理员在构建包前经常修改它的原因，根据目标的位置将其设置为internal或者updates。然后mini-dinstall生成APT所需的文件,如Packages.gz. 

mini-dinstall需要一个~/.mini-dinstall.conf的配置文件

mini-dinstall可以作为一个后台服务运行，每半小时检查一次incoming的内容，将新内容归档，并更新package.gz和source.gz
mini-dinstall 也可以用-b选项以批处理的方式运行，每次运行时检查一次

由于APT会校验包的签名，在试用私有源时apt可能会一直报警。
为此mini-dinstall包含了一个release_signscript的配置选项，它允许设定一个脚本来生成签名。mini-dinstall在/usr/share/doc/mini-dinstall/examples/里提供了一个签名脚本的例子，可以自行修改
 
## apt-ftparchive
如果mini-dinstall相对于需求过于复杂，可以使用apt-ftparchive命令。这个工具扫描目录的内容，并通过标准输出输出一个对应的packages文件。

```
$ cd /srv/vhosts/packages
$ apt-ftparchive packages updates >updates/Packages
$ gzip updates/Packages
$ apt-ftparchive packages internal >internal/Packages
$ gzip internal/Packages
```
apt-ftparchive sources 命令可以以类似的方式创建Sources.gz files

