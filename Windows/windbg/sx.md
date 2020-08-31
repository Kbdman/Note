# sx, sxd, sxe, sxi, sxn, sxr, sx- (Set Exceptions)

sx* 指令控制调试器是如何处理被调试程序中出现的异常或者事件的    
```
sx 

sx{e|d|i|n} [-c "Cmd1"] [-c2 "Cmd2"] [-h] {Exception|Event|*} 

sx- [-c "Cmd1"] [-c2 "Cmd2"] {Exception|Event|*} 

sxr
```
## 参数

+ -c "Cmd1"     
指定异常或者事件出现时执行的命令。这个命令在异常出现时调试器第一次处理时执行，无视异常是否会进入调试器.命令必须放在引号中，命令可以是;分割的多条指令


+ -c2"Cmd2"       
指定异常或者事件没有被first chance处理时执行的命令。这个命令在异常出现时调试器第二次处理时执行，无视异常是否会进入调试器.命令必须放在引号中，命令可以是;分割的多条指令

+ -h      
变更事件的处理状态 If Event is cc, hc, bpec, or ssec, you do not have to use the -h option.

+ Exception     
指定异常号

+ Event 
指定事件.事件用缩写表示

|   evcode  |   desciption  |   Default break status |    
| ------ | ------ | ------ |
|asrt| Assertion failure| Break|
|av| Access violation| Break|
|dm| Data misaligned| Break|
|dz| Integer division by zero| Break|
|c000008e|Floating point division by zero| Break| 
|eh|C++ EH exception| Second-chance break| 
|gp| Guard page violation| Break| 
|ii| Illegal instruction| Second-chance break|
|iov| Integer overflow| Break|
|ip|In-page I/O error| Break| 
|isc| Invalid system call| Break|
|lsq| Invalid lock sequence | Break|
|sbo| Stack buffer overflow| Break|
|sov| Stack overflow| Break|
|wkd| Wake debugger| Break|
|aph| Application hang This exception is triggered if the Windows operating system concludes that a process has stopped responding (that is, is hung).| Break|
|3c| Child application termination | Second-chance break|
|chhc| Invalid handle | Break|
|Number|Any numbered exception| Second-chance break|
 
+ \*    
影响违背显示sx显式设置名称的异常
 
## 备注
sx 列出当前进程所有的异常和事件的处理行为       
sxe sxd sxn sxi指令控制调试器对这些异常或者事件的处理
sxr 重置异常和事件处理到默认状态        
sx- 指令不会修改事件或者异常的处理状态或者中断状态。而是设置first-chance的命令
如果指定了-h选项，sxe sxd sxn sxi指令会控制对异常或者事件的处理状态。       

+ sxe       
在异常出现时，调试器在任何异常处理手段之前接管被调试程序，被称为firstchance，在恢复后执行后异常被认为是handled
+ sxd
在没有其他异常处理手段处理异常导致程序无法执行行，进入secondchance
+ sxn
异常出现时显示一个信息
+ sxi
忽略世界或者异常

sxd，sxi,sxn在恢复执行后异常被认为是unhandled



 