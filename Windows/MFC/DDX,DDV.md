# DDX/DDV
## Dialog Data Exchange
一种对话框与数据对象进行数据交换的机制。        
如果使用DDX机制，在OnInitDialog或者对话框的构造函数中为空间对应数据对象设置的初始值，会在对话框显示前被DDX机制传递给对话框中的对应控件。                       
默认的实现是在CDialog::OnInitDialog中会调用CWnd::UpdateData来初始化对话框的控件         
同样的机制也用来在用户点击确认按钮时将控件的数据传递对应的成员变量(或者任何使用TRUE为参数调用UpdateData时)。


## Dialog Data Verify
DDV机制用来为控件设置数据元素的校验规则

## DoDataExchange
UpdateData根据BOOL参数可以双向工作。为了完成交换。UpdateData创建一个CDataExchange对象，将它传递给对话框的DoDataExchange函数。       
CDataExchange对象包含交换的信息，例如方向。

在重写DoDataExchange方法时，应当为每个数据成员或者控件调用DDX函数。DDX函数根据CDataExhange判断数据传输的方向。


```
void CTestDialog::DoDataExchange(CDataExchange* pDX)
{
   CDialog::DoDataExchange(pDX);
   DDX_Check(pDX, IDC_MY_CHECKBOX, m_bVal);
   DDX_Text(pDX, IDC_MY_TEXTBOX, m_strName);
   DDV_MaxChars(pDX, m_strName, 20);
}
```