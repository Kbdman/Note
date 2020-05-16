# Structured Exception Handler X86
windows下提供的单项链表式的异常处理机制
FS[0]指向最新的ExceptionHandler

ExceptionHandler的数据结构:
1. prev     
上一个ExceptionHandler的地址
2. handler  
当前handler的异常处理函数
>只有在32位 Windows下，SEH采用这种链式结构，在64位下，用一个DataDirectory来存储异常处理信息
## 异常处理函数的返回值
+ ExceptionContinueExecution    
让程序从异常的位置继续执行
+ ExceptionContinueSearch   
继续向上层搜索异常处理函数
+ ExceptionNestedException
+ ExceptionCollidedUnwind

## ExceptionHandler的注册与注销
在需要注册的ExceptionHandler的函数中
```
push handler
push fs:[0]
mov fs:[0],esp
```
1. 将要注册的处理函数地址保存到栈中
2. 从fs:[0]中取出上一个handler的地址，存入栈中
3. 将esp存入fs:[0]中

这样就可以通过fs[0]获取最新的ExceptionHandler的地址，再就可以通过prev向前遍历

## Windows异常处理的过程
1. 出现异常
2. 在被调试状态通知调试器
3. 通过ExceptionHandler链表进行异常处理
4. 如果所有的程序内注册的异常处理函数都没有恢复程序的运行，异常最终会被UnhandledExceptionFilter处理

应用程序可以setUnhandledExceptionFilter来设置UnhandledExceptionFilter时调用的函数


## SEH3
SEH3扩展了ExceptionHandler链表元素的成员，多了scope_table指针，以及prev_try_level等

SEH3通过scope_table提供了对多层嵌套的__try{}__except(){...}的支持

SEH3使用统一的异常处理函数_except_handler3.

### scope_table
scope_table元素的数据结构
+ try level
    处理函数关联的try块的信息
+ filter
    过滤函数，由__except(..)中的条件判断代码生成而成
+ actual handler
    实际处理_try{}中发生的符合filter的条件的异常的函数，__except(){...}的{}包含的代码生成

scope_table在编译时构建，根据所在的函数的状况可以由多个元素
### SEH3的异常处理
_try{}块生成的代码前后会有操作当前EH中的try_level的值.      
当由异常发生时,_except_handler3根据fc[0]以及scop_table指针以及try_level的相对位置找到scope_table地址和try_level     
遍历scope_table，找到符合过滤条件的元素调用异常处理函数，如果没有_except_handler3返回ExceptionContinueSearch,继续向上层搜索

## X64下的SEH
X64在.pdata节中位需要的函数创建了RUNTIME_FUNCTION,其中指定了函数的开始位置和结束位置，以及异常处理的信息        
DataDirectory[IMAGE_DIRECTORY_ENTRY_EXCEPTION]指向了这个表