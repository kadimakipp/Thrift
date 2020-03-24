### thrift demo
关键命令 `thrift -r --gen <language> <Thrift filename>`
#### cpp
1. 在'shared.thrift'和'tutorial.thrift'中定义数据传输的类型，支持基本类型和复杂的类型。
2. 使用命令`thrift -r --gen cpp tutorial.thrift`自动生成cpp语言的通信和协议内容。生成内容包含客户端和服务端的内容，文件在gen-cpp下面，幷包含两个含有main的文件。
3. 将gen-cpp下面的文件打包成静态库，需要删除生成的两个含有main的cpp文件。
4. 写客户端和服务端的应用程序。在cpp文件夹下。

测试方法：先启动服务端后启动客户端，进行通信和计算。计算是在服务端进行的。

#### py
1. 使用相同的`*.thrift`文件
2. 使用命令`thrift -r --gen py tutorial.thrift`生成py语言文件。
3. 写客户端和服务端的应用程序。在py文件夹下。


#### py and cpp
上面实现后即使进程间通信，跨语言通信使用不同的服务端和客户端即可