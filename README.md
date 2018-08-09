# 设计思路

​	本项目为了设计一套单终端登录系统，具有注册，登录，测试的功能。

​	基于grpc架构，用户信息采用sqlite管理。

​	对于客户端来说，首先需要用户注册，用户输入需要注册的用户名和密码（密码需要重复输入两次），服务器获取用户输入的用户名之后，在数据库中查询，如果该用户名未被注册，则将信息写入数据库，并返回给客户端ok，表示注册成功。如果用户名在数据库中已经存在，则返回“用户名已经被注册”的信息给客户端。

​	对于客户端的登录功能，用户输入用户名和密码，服务器获取用户的输入信息之后在数据库中进行查找，如果查找成功，用户名和密码都匹配，则返回给用户ok，为了支持切换终端登录的时候，其他登录的终端会被踢出，同时还需要返回token信息，token信息是通过一个UUID来唯一生成的，当用户在下一台机器上登录的时候，之前的token信息失效，用户无法操作，只有获取最新token的用户才可以进行操作。若输入的密码错误，则返回给客户端相应的信息以此来提示客户端。

​	对于用户的登录和注册的操作，密码在客户端都会通过MD5加密之后，才会发送给服务器，这样保证了密码的安全。

​	对于已经登录的用户来说，它可以进行测试，输入一个字符串给服务端，当token信息有效（既未被其他用户提出）时，服务端会返回ok，当token信息无效（即用户已经被踢出）的时候，服务端会返回token无效。

# 代码结构

​	项目的全部代码存放在client,common,database,protocol,server这5个目录下，doc用来存放一些文档信息。接下来将逐一对存放代码的5个目录进行介绍。

![图2-1](https://github.com/xiaohan-orange/easylogin/blob/master/doc/2-1.png)

​	protocol目录下主要是一个user_info.protoc文件和几个由它编译生成的文件（内含Makefile）。		 	protoc文件中定义了如上图（图2-1）的请求和返回信息，上图展示的是登录的请求和返回信息（注册和测试类似，不再展示），登录需要用户名和密码，服务器返回的信息包括返回码，返回字符串和token信息。

​	同时还定义了三个rpc服务，分别对应登录，注册，测试，如下图所示（图2-2）：

![图2-2](https://github.com/xiaohan-orange/easylogin/blob/master/doc/2-2.png)

​	

​	database目录：主要是封装了sqlite数据库的操作和一个状态类。

![图2-3](https://github.com/xiaohan-orange/easylogin/blob/master/doc/2-3.png)

​	上图（图2-3）展示的是状态类的封装，该类的主要目的是返回数据库操作的状态。

​	下图（图2-4）展示的对数据库操作的封装。

![图2-4](https://github.com/xiaohan-orange/easylogin/blob/master/doc/2-4.png)



​	common目录主要是定义了一些工具类。例如MD5加密算法的头文件和实现，function.h中定义了一个uuid转字符串的函数（以后如果项目有了扩展，类似的函数也可以放在这个文件中），log.h中定义了一个简单的输出日志，flags.h中定义了一个表示数据库位置的常量。以上文件均不用图片展示。action.h中（图2-5）中定义了一个客户端行为的状态类，用于返回客户端操作的状态。该类的实现类似于database目录下的数据库状态类。

![图2-5](https://github.com/xiaohan-orange/easylogin/blob/master/doc/2-5.png)

​	client目录下定义了客户端的相关操作。token也存储在client类中（图2-6）。

![图2-6](https://github.com/xiaohan-orange/easylogin/blob/master/doc/2-6.png)

​	server目录下定义了三个rpc服务的具体实现（图2-7）。

![图2-7](https://github.com/xiaohan-orange/easylogin/blob/master/doc/2-7.png)



# RoadMap

​	1：使用SSL/TLS 对客户端和服务端交换的所有数据进行了加密。（未实现）

​	2:

# 安装构建

+ grpc安装（参照官网）
  https://grpc.io/docs/quickstart/cpp.html

+ install sqlite3, uuid_dev

```bash
sudo apt-get install sqlite3
sudo apt-get install uuid-dev
```

+ 创建库表
```sql
CREATE TABLE USERINFO( 
  USERNAME CHAR(64) NOT NULL,
  PASSWORD CHAR(64) NOT NULL,
  TOKEN CHAR(128) );
```
+ 在protocol目录，client目录，server目录中输入make命令即可完成编译

# 运行截图

+ 首先。我们运行服务器，运行结果如图5-1所示。

![图5-1](https://github.com/xiaohan-orange/easylogin/blob/master/doc/5-1.png)

+ 接下来，我们运行客户端。运行结果如图5-2所示。

![图5-2](https://github.com/xiaohan-orange/easylogin/blob/master/doc/5-2.png)

+ 当我们在注册账号的过程中，两次的输入密码不相同的时候，会看到图5-3的运行结果。

![图5-3](https://github.com/xiaohan-orange/easylogin/blob/master/doc/5-3.png)

+ 如果我们两次密码输入相同，并且该用户名没有被注册过的话，我们会注册成功，如图5-4所示。

![图5-4](https://github.com/xiaohan-orange/easylogin/blob/master/doc/5-4.png)

+ 当我们试图注册一个已经被注册过的用户名时，运行结果如图5-5所示。

![图5-5](https://github.com/xiaohan-orange/easylogin/blob/master/doc/5-5.png)

+ 登录时，当我们输入错密码的时候，运行结果如图5-6所示。

![图5-6](https://github.com/xiaohan-orange/easylogin/blob/master/doc/5-6.png)

+ 登录的用户名和密码都正确的时候，运行结果如图5-7所示。

![图5-7](https://github.com/xiaohan-orange/easylogin/blob/master/doc/5-7.png)

+ 登陆成功后（登陆成功后才会返回正确的token），我们进行测试（模拟用户登录成功后的各种操作），运行结果如图5-8所示。

![图5-8](https://github.com/xiaohan-orange/easylogin/blob/master/doc/5-8.png)

+ 接下来我们重新开一个终端，模拟另一个用户登录的情形，在另一个用户登陆成功之后，对上一个终端的用户再次执行测试（它的token信息已经失效），运行结果如图5-9所示。

![图5-9](https://github.com/xiaohan-orange/easylogin/blob/master/doc/5-9.png)



# 
