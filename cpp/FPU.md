# FPU
## 寄存器
8个80位浮点寄存器和一个栈顶指针。

st(0)~st(7),表示当前栈顶到栈底得8个浮点寄存器，而并不是某个寄存器得固有名字
## 常用指令
+ fld       
加载浮点数到寄存器栈顶
### 不带p的计算指令
+ fmul      
乘法
+ fdiv            
除法
+ fadd      
加法
+ fsub       
减法
#### 指令格式 
+ op        
st(1) op st(0),结果保存在st(1),在弹出st(0)
+ op var        
st(0)=st(0) op var
+ op st(i) st(j)
必有一个操作数为st(0),st(i)=st(i) op st(j)
### 带弹出的计算指令
+ fmulp      
乘法
+ fdivp            
除法
+ faddp      
加法
+ fsubp       
减法
#### 指令格式
+ op st(i),st(0)        
st(i)=st(i) op st(0),弹出st(0)
