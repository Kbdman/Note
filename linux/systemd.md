systemctl是systemd的控制工具

服务设备挂载点操作系统的配置信息都存在与unit中
## 查看相关命令
### 列出所有unit
systemctl list-unit-files

enabled 启用 disable禁用 static 被依赖的对象，不能直接启动
### 列出所有服务的unit 
systemctl list-unit-files --type=service
### 查看服务的状态 
systemctl status gdm.service
 
systemctl的配置文件存放在/usr/lib/systemd/system中
 
systemd试用.target定义了一组unit，与sysvinit的runlevel类似
 
 
## 常用命令
+ systemctl start 启动
+ systemctl stop 停止
+ systemctl restart 重启动
+ systemctl enable 设置自启动
+ systemctl disable 禁用自启动