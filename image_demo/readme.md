### image demo
python做服务器, cpp做客户端
python方面放深度学习的代码

#### 1.note:
| thrift | python | cpp |
| -:|-:|
|list| list|vector|
|map| dict|map|
|set| set|set|

thrift 中的结构体在python和cpp中都是用class来实现的。
#### 2.cpp
1. cpp服务端要实现一个句柄类，来继承实现生成的虚类
- 假设thrift中定义的服务器名称为 `something`,那么就要继承`somethingIf`
然后实现thrift中定义的函数。
- 如果在thrift中使用`include "xxx.thrift"`并且其中也有定义的服务器，并且在当前服务器中继承了它`extends`,那么需要在句柄类中实现这个函数。
- 除了实现句柄类以外一些例子中还实现了一个`clone Factory`类，并且在工厂类中初始化socket和先前创建的句柄类。
- 例子中还定义了一些错误处理和结构体等其他操作。
2. cpp客户端比较简单，实现相应的参数处理，然后调用呼叫服务端，幷进行相应的错误处理

#### 3.python
python的服务端，跟cpp类似，但是不需要继承，他是通过参数传递给process的。
其他的与cpp类似，可以参考上面内容。
