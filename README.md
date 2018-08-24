# easylogin
	本项目为了设计一套单终端登录系统，具有注册，登录，单点登录的功能
----
## 设计思路
  基于grpc框架，后端数据使用sqlite存储。提供了注册，登录，单点登录检测（登录成功后自动调用）三个功能。用户登录状态由一个通过uuid生成的token值来维护，每个用户的登录始终只维持最新的一份token。密码在客户端通过MD5加密，传送到服务器之后对其进行加盐二次哈希。同时预防sql注入。

## 运行流程
**注册**  

![flowchart-1](https://github.com/xiaohan-orange/easylogin/blob/master/doc/flowchart-4.png)

1：输入用户名和密码（两次）。  

2：检测两次输入的密码是否相同，如果两次输入的密码不相同，注册失败。  

3：服务器在数据库中检测用户名是否已被注册，如果用户名已被注册，注册失败。  

4：对加密后的密码进行加盐二次哈希。  

5：将用户信息存入数据库。  

6：注册成功。

**登录**  

![flowchart-2](https://github.com/xiaohan-orange/easylogin/blob/master/doc/flowchart-5.png)

1：输入用户名和密码。  

2：服务器从数据库中取出盐值和加盐后的密码，对用户输入的密码进行加盐二次哈希。  

3：判断加盐二次哈希之后的密码是否和数据库中的密码相同，如果不相同，登陆失败。  

4：生成新的token值，更新数据库中的登录信息。  

5：登陆成功。

**单点登录检测**  

![flowchart-3](https://github.com/xiaohan-orange/easylogin/blob/master/doc/flowchart-6.png)

1：服务器查询已登录用户的token值是否失效。  

2：若失效，则返回信息通知token值失效（即有其他用户登录）。

## 代码结构

``` c++
├── README.md  // 文档  

├── WORKSPACE  		  

├── client // 客户端代码       

├── common  // 公共函数   

├── database  //数据库封装   

├── doc  // 文档及图片  

├── proto  // protobuf文件    

└── server  // 服务器端代码  

```


## 数据库

### USERINFO表
字段 | 字段名 | 类型 | 可空 
---- | ---- | ---- | ---- 
用户名 | USERNAME | CHAR(128) | NOT NULL  
密码 | PASSWORD | CHAR(512) | NOT NULL  
盐 | SALT | CHAR(256)| NOT NULL  
token | TOKEN | CHAR(256) | NOT NULL  

**请注意将表建立在/tmp/test.db中！**

```sql
CREATE TABLE USERINFO( 
  USERNAME CHAR(128) NOT NULL,
  PASSWORD CHAR(512) NOT NULL,
  TOKEN CHAR(128) NOT NULL
  );
```


## 安装构建

+ install sqlite3, uuid_dev

```bash
sudo apt-get install sqlite3
sudo apt-get install uuid-dev
```
##Bazel构建
``` c++  
cd easylogin/server
bazel build server-bin
bazel run server-bin

cd easylogin/client
bazel build client-bin
bazel run client-bin
```

## RoadMap
### todo list
- [ ] grpc调用使用SSL/TLS
- [ ] 配置参数可使用gflag库，现在为了方便实现，直接定义为常量
- [ ] 目前只有端到端的测试，和一些公共类库的简单测试，可以用gtest去加强ut覆盖


## 运行截图

[运行截图](https://github.com/xiaohan-orange/easylogin/blob/master/doc/runScreenshot.md)  





# 
