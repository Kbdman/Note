# TLS
Thread Local Storage

c++11新增了一个thread_local字段用于定义一个tls变量
```
thread_local int a=1;
```
在生成的文件中，tls变量被存在一个tls段中，每当一个创建一个线程时，都会以tls段为副本创建一个线程相关的tls段

## 线程使用TLS
在windows下，线程通过fs找到tib,通过tib.tls_array指针获得tls列表，然后用__tls_index作为索引查找tls位置


## TLS_CALLBACK
tls_callback是每个线程的TLS设置后，线程启动前会被调用的代码。可以用来做反调试