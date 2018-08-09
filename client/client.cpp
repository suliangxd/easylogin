#include <iostream>

#include "client/client.h"
#include "common/flags.h"
#include "common/log.h"
#include "common/md5.h"

const static string RPC = "PRC failed";
const static string PASS = "Inconsistent Password";

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using userinfo::LoginRequest;
using userinfo::LoginResponse;
using userinfo::RegisterRequest;
using userinfo::RegisterResponse;
using userinfo::TestRequest;
using userinfo::TestResponse;
using userinfo::UserAction;

using std::cin;
using std::cout;
using std::endl;
using std::string;

using namespace easylogin::common;

namespace easylogin
{
namespace client
{

void Client::Login()
{
    cout << "Please Enter Your Username:" << endl;
    string username;
    getline(cin, username);
    cout << "Please Enter Your Password:" << endl;
    string password;
    getline(cin, password);
    LoginRequest request;
    password = MD5(password).toStr();
    request.set_username(username);
    request.set_password(password);
    LoginResponse response;
    ClientContext context;
    Status status = stub_->Login(&context, request, &response);
    if (status.ok())
    {
        cout << response.ret_msg() << endl;
        token_ = response.token();
    }
    else
    {
        cout << status.error_code() << ":" << status.error_message() << endl;
        cout << RPC << endl;
    }
}

void Client::Register()
{
    cout << "Welcome To Register An Acount!" << endl;
    cout << "Please Enter Your Username:" << endl;
    string username;
    getline(cin, username);
    cout << "Please Enter Your Password:" << endl;
    string password1;
    getline(cin, password1);
    cout << "Please Enter Your Password Again:" << endl;
    string password2;
    getline(cin, password2);
    if (password1 != password2)
    {
        cout << PASS << endl;
    }
    RegisterRequest request;
    password1 = MD5(password1).toStr();
    request.set_username(username);
    request.set_password(password1);
    RegisterResponse response;
    ClientContext context;
    Status status = stub_->Register(&context, request, &response);
    if (status.ok() )
    {
        cout << response.ret_msg() << endl;
    }
    else
    {
        cout << status.error_code() << ":" << status.error_message() << endl;
        cout << RPC <<endl;
    }
}

void Client::Test()
{
    cout << "Welcode To Test,Please Enter A String." << endl;
    string  str;
    cin >> str;
    TestRequest request;
    request.set_token(token_);
    request.set_test_string(str);
    TestResponse response;
    ClientContext context;
    Status status = stub_->Test(&context, request, &response);
    if (status.ok())
    {
        cout << response.ret_str() << endl;
    }
    else
    {
        cout << status.error_code() << ":"
            << status.error_message() << endl;
        cout << RPC << endl;
    }
}
}  // namespace client
}  // namespace easylogin
