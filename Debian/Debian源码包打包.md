## 下载上游源码包       
通常格式为 xxxx-version.tar.gz
## 解压进入源码     
进入源码目录，目录名  xxxx-version
>如果没有上游源码包，可以在 下面dh_make 时加入参数 --createorig,会自动根据当前源码目录，创建上游源码包
## dh_make/debmake

生成debian目录，在目录下创建debian包所需的描述文件，包括control，changelog，copyright等
>dh_make时会让选择参数 s/i/m之类的，singal binary,代表源码包生成的程序是一个单独的二进制包


## 按需编辑这些文件
### rules
rules文件通常包含了一组用于在一个专用的子目录下配置，构建，安装软件的规则。这个目录中的内容会被打包在deb包中，就好象他是根目录一样。
rules文件可以像makefile文件一样使用。
>dh_make好像会要求必须有个在源码目录中有个顶级的makefile,这个makefile中需要有all,clean,install，uninstall等基本命令才能被rules正确的使用

### 安装其他文件
可以在debian目录下创建一个${package}.install的文件，将一些文件打包进deb包中

如将源码目录下的data/*放到usr/share中
>data/* usr/share/ 

也可以直接在debian下新建install文件来定义文件的安装

对于makefile中没有涉及到的而又需要安装到目标系统的文件，可以通过install来设定

## 如果上源包有修改，试用dpkg-source --commit 将改动做成patch

## debuild/dpkg-buildpackge生成源码包
### dpkg-buildpackge
1. 设置构建所需的环境变量。运行init hook,调用dpkg-source --befor-build(除非使用了 -T 或者 --target).

2. 检查编译依赖或者冲突是否满足 (除非指定了 -d 或者--no-check-builddeps).

3. 如果通过-T或者--target指定了一个或者多个target,调用这些target然后停止。否则执行preclean hook,然后调用 fakeroot debian/rulse clean 来清理构建树（除非指定了-nc or --no-pre-clean ） 
4. 执行source hook,并调用 dpkg-source -b，生成源码包（如果通过--build指定了生成源码）

5. 执行build hook,调用 debian/ruls build-target。然后执行binary hook,然后再调用 fakeroot debian/rules binary-target(除非用--build=source指定了只构建源码包)Note that build-target and binary-target are either build and binary (default case, or if an  any  and  all  build  has  been  requested  with  --build  or equivalent  options),  or  build-arch  and  binary-arch  (if  an  any  and not all build has been requested with --build or equivalent options), or build-indep and binary-indep (if an all and not any build has been requested with --build or equivalent options).
6. 执行buildinfo hook,调用，dpkg-genbuildinfo生成一个buildinfo文件。多个dpkg-buildpackage选项都会调用dpkg-genbuildinfo

7. 运行changes hook,调用dpkg-genchanges生成一个.change文件。.change的文件名是基于build的类型 and will be as specific as necessary but not more;    for a build that includes  any  the  name  will  be source-name_binary-version_arch.changes,  or otherwise  for a build that includes all the name will be source-name_binary-version_all.changes,or  otherwise  for  a  build  that  includes  source  the  name   will   be   source-name_source-          version_source.changes.  Many dpkg-buildpackage options are forwarded to dpkg-genchanges.

8. 执行post-clean hook,如果指定了 -tc 或者--post-clean,调用fakeroot debian/rules clean
9. 调用dpkg-source --after-build.
10. 执行check hook,如果在DEB_CHECK_COMMAND 或者--check-command中指定了检查命令，执行检查命令
11. 执行sign hook,调用gpg2或者gpg来前面.dsc文件,buildinfo文件和.change文件
12. 执行done hook


### lintian
lintian是一个从Debian Policy创建出的检查器


### 制作生成多个包的包

debmake -b ',pa1,pa2' 在生成模板时指定生成多个包的模板，pa1,pa2为其他包的包名
在生成的debian/下编辑pa1.install pa2.install来设定包的安装


### install文件格式
>源文件 目标目录
源文件可以用*之类的匹配，符合规则的将会被打包到目标目录下
如果只有源文件，则表示当前包包含这些文件，目标目录和源文件的目录一致

>如果是CMake进行配置的包，源文件应该从CMakeList.txt里设置的安装目录去匹配.具体的路径，可以查看CMake生成的install_manifest