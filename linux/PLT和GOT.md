# GOT和PLT
## 结构概述
GOT，Global Offset Table，导入地址表        
Plt，Procedure Linkage Table，过程连接表        
Plt和Got几乎是一一对应的，每一对对应一个库函数        
Plt是一组简单的代码段组成，x86下 3个指令
1. 跳转到对应GOT项保存的地址中
2. push一个参数
3. 调用loader的函数更新GOT项        
在未初始化状态Got表项保存着对应plt项加载依赖函数的代码的地址。

## 加载过程
当程序第一次调用某个库函数时，由于GOT保存着更新GOT的代码地址，第一次调用会更新这个GOT项，之后的调用则可以在PLT第一个指令直接跳转到目标的函数
