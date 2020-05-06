# Debian源码再打包
## 获取源码
apt source 包名  获取源码包
获得的文件
*.dsc  Debian Souce Control 包的一些信息
*.tar.comp *.diff.gz *.debian.tar.comp  大概是debian维护者对原始包的修订信息
 
## 源码解包
dpkg-source -x .dsc 跟觉dsc文件将源码包解出
 
## 再打包
试用dch更新debian/changelog
dch --local text --local会将text加在新版本的末尾
如果构建选项需要变更，变更应当写在debian/rules中
如有需要跟新debian/control
使用dpkg-buildpackage -us  -uc 在当前源码包的基础上构建二进制包