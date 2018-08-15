# easylogin
	本项目为了设计一套单终端登录系统，具有注册，登录，单点登录的功能
----
## 设计思路
  基于grpc框架，后端数据使用sqlite存储。提供了注册，登录，单点登录检测（登录成功后自动调用）三个功能。用户登录状态由一个通过uuid生成的token值来维护，每个用户的登录始终只维持最新的一份token。密码在客户端通过MD5加密，传送到服务器之后对其进行加盐二次哈希。同时预防sql注入。

## 运行流程
**注册**

1：输入用户名和密码（两次）。  

2：检测两次输入的密码是否相同，如果两次输入的密码不相同，注册失败。  

3：服务器在数据库中检测用户名是否已被注册，如果用户名已被注册，注册失败。  

4：对加密后的密码进行加盐二次哈希。  

5：将用户信息存入数据库。  

6：注册成功。

**登录**

1：输入用户名和密码。  

2：服务器从数据库中取出盐值和加盐后的密码，对用户输入的密码进行加盐二次哈希。  

3：判断加盐二次哈希之后的密码是否和数据库中的密码相同，如果不相同，登陆失败。  

4：生成新的token值，更新数据库中的登录信息。  

5：登陆成功。

**单点登录检测**

1：服务器查询已登录用户的token值是否失效。  

2：若失效，则返回信息通知token值失效（即有其他用户登录）。

## 代码结构

``` c++
├── README.md  // 文档  

├── WORKSPACE  		  

├── client // 客户端代码      

│    ├── BUILD	  

│    ├── Makefile   

│    ├── client.cpp  

│    ├── client.h  

│    └── main.cpp  

├── common  // 公共函数  

│    ├── test   

│    ├── BUILD  

│    ├── action_status.cpp    

│    ├── action_status.h  // 操作返回状态  

│    ├── flags.h  // flags  

│    ├── function.h  // 公共函数  

│    ├── log.h  // 日志  

│    ├── md5.cpp  

│    ├── md5.h // md5加密算法  

│    └── sha256.h // sha256加密算法  

├── database  //数据库封装  

│    ├── test  

│    ├── BUILD  

│    ├── sqlite_wrapper.cpp  

│    └── sqlite_wrapper.h  

├── doc  // 文档及图片  

├── proto  // protobuf文件  

│    ├── BUILD  

│    ├── Makefile  

│    ├── user_info.grpc.pb.cc  

│    ├── user_info.grpc.pb.h  

│    ├── user_info.pb.cc  

│    ├── user_info.pb.h  

│    └── user_info.proto   

└── server  // 服务器端代码  

│    ├── BUILD  
    
│    |── Makefile  
    
│    ├── main.cpp  
    
│    ├── server.cpp  
    
│    └── server.h
```


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



## RoadMap
### todo list
- [ ] grpc调用使用SSL/TLS
- [ ] 配置参数可使用gflag库，现在为了方便实现，直接定义为常量
- [ ] 目前只有端到端的测试，和一些公共类库的简单测试，可以用gtest去加强ut覆盖


## 安装构建

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

## 运行截图

运行截图
https://github.com/xiaohan-orange/easylogin/blob/master/doc/runScreenshot.md



# 
