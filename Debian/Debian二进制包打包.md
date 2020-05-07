+ 为被打包的项目创建一个文件夹，
+ 在目录中为项目的各个组件按照安装目录创建对应的文件夹
+ 例如组件a要放在/usr/bin下，则创建目录usr/bin,并将组件a放置在usr/bin下
+ 在项目目录中新建一个DEBIAN文件夹
+ 在DEBIAN文件夹下新建一个control文件，在里面填入相应的信息
+ 在DEBIAN目录下生成md5sums文件

+ 回到上层目录
使用   dpkg-deb -b 目录名 包名  生成deb包
 
+ 解包
 dpkg-deb -R 包名
 
+ 安装dpkg -i



