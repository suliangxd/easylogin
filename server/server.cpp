#include <iostream>
#include "common/function.h"
#include "common/flags.h"
#include "common/log.h"
#include "common/sha256.h"
#include "database/sqlite_wrapper.h"
#include "server/server.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using userinfo::LoginRequest;
using userinfo::LoginResponse;
using userinfo::RegisterRequest;
using userinfo::RegisterResponse;
using userinfo::TestRequest;
using userinfo::TestResponse;
using userinfo::UserAction;

using namespace easylogin::common;
using namespace easylogin::database;
using namespace std;

const static std::string HELLO = "Hello ";

namespace easylogin
{
namespace server
{

template<typename T>
void ServiceImpl::finish(ActionStatus status, T response)
{
    response->set_ret_code(status.GetCode());
    response->set_ret_msg(status.ToString());
}

Status ServiceImpl::Login(ServerContext* context,
    const LoginRequest* request, LoginResponse* response)
{
    SqliteWrapper mySqlite;
    DbStatus dbStatus = mySqlite.Open(TEST_DB_FILE);
    if (!dbStatus.IsOk())
    {
        LOG_ERROR(dbStatus.ToString().c_str());
        ActionStatus actionStatus(ActionStatus::kDatabaseError);
        mySqlite.Close();
        finish<LoginResponse*>(actionStatus, response);
        return Status::OK;
    }
    string username = request->username();
    // string password = hash256_hex_string(request->password());
    string salt;
    string password = request->password();
    uuid_t uuid;
    uuid_generate(uuid);
    string token = uuid_to_string(uuid);
    string sql = "SELECT SALT FROM USERINFO WHERE USERNAME = ?";
    SqliteStatement* stmt = mySqlite.Statement(sql);
    if(!stmt->Bind(0, username))
    {
        ActionStatus actionStatus(ActionStatus::kDatabaseError);
        mySqlite.Close();
        finish<LoginResponse*>(actionStatus, response);
        return Status::OK;
    }
    if(stmt->NextRow())
    {
        salt = stmt->ValueString(0);
        /*
        if(stmt->ValueString(0) != password)
        {
            ActionStatus actionStatus(ActionStatus::kIncorrectPassword);
            mySqlite.Close();
            finish<LoginResponse*>(actionStatus, response);
            return Status::OK;
        }*/
    }
    else
    {
        ActionStatus actionStatus
            (ActionStatus::kIncorrectUsernameOrPassword);
        mySqlite.Close();
        finish<LoginResponse*>(actionStatus, response);
        return Status::OK;
    }
    password += salt;
    password = hash256_hex_string(password);
    sql = "SELECT PASSWORD FROM USERINFO WHERE USERNAME = ?";
    stmt = mySqlite.Statement(sql);
    if(!stmt->Bind(0, username))
    {
        ActionStatus actionStatus(ActionStatus::kDatabaseError);
        mySqlite.Close();
        finish<LoginResponse*>(actionStatus, response);
        return Status::OK;
    }
    if(stmt->NextRow())
    {
        if(stmt->ValueString(0) != password)
        {
            ActionStatus actionStatus
                (ActionStatus::kIncorrectUsernameOrPassword);
            mySqlite.Close();
            finish<LoginResponse*>(actionStatus, response);
            return Status::OK;
        }
    }
    else
    {
        ActionStatus actionStatus
            (ActionStatus::kIncorrectUsernameOrPassword);
        mySqlite.Close();
        finish<LoginResponse*>(actionStatus, response);
        return Status::OK;
    
    }
    sql = "UPDATE USERINFO SET TOKEN = ? WHERE USERNAME = ?";
    stmt = mySqlite.Statement(sql);
    if(!stmt->Bind(0, token))
    {
        ActionStatus actionStatus(ActionStatus::kDatabaseError);
        mySqlite.Close();
        finish<LoginResponse*>(actionStatus, response);
        return Status::OK;
    }
    if(!stmt->Bind(1, username))
    {
        ActionStatus actionStatus(ActionStatus::kDatabaseError);
        mySqlite.Close();
        finish<LoginResponse*>(actionStatus, response);
        return Status::OK; 
    }
    if(!stmt->Execute())
    {
        ActionStatus actionStatus(ActionStatus::kDatabaseError);
        mySqlite.Close();
        finish<LoginResponse*>(actionStatus, response);
        return Status::OK; 
    }
    else
    {
        ActionStatus actionStatus(ActionStatus::kOk);
        mySqlite.Close();
        finish<LoginResponse*>(actionStatus, response);
        response->set_token(token);
        return Status::OK;
    }
}

Status ServiceImpl::Register(ServerContext* context,
    const RegisterRequest* request, RegisterResponse* response)
{
    SqliteWrapper mySqlite;
    DbStatus dbStatus = mySqlite.Open(TEST_DB_FILE);
    if (!dbStatus.IsOk())
    {
        LOG_ERROR(dbStatus.ToString().c_str());
        ActionStatus actionStatus(ActionStatus::kDatabaseError);
        finish<RegisterResponse*>(actionStatus, response);
        mySqlite.Close();
        return Status::OK;
    }
    string username = request->username();
    uuid_t uuidSalt;
    uuid_generate(uuidSalt);
    string salt = uuid_to_string(uuidSalt);
    string password = request->password();
    password += salt;
    password = hash256_hex_string(password);
    // string password = hash256_hex_string(request->password());
    uuid_t uuid;
    uuid_generate(uuid);
    string token = uuid_to_string(uuid);
    string sql = "SELECT USERNAME FROM USERINFO WHERE USERNAME = ?";
    SqliteStatement* stmt = mySqlite.Statement(sql);
    if(!stmt->Bind(0, username))
    {
        ActionStatus actionStatus(ActionStatus::kDatabaseError);
        mySqlite.Close();
        finish<RegisterResponse*>(actionStatus, response);
        return Status::OK;
    }
    if(stmt->NextRow())
    {
        ActionStatus actionStatus(ActionStatus::kUsernameUsed);
        mySqlite.Close();
        finish<RegisterResponse*>(actionStatus, response);
        return Status::OK;
    }
    sql = "INSERT INTO USERINFO(USERNAME, PASSWORD, SALT, TOKEN)"\
           "VALUES(?,?,?,?)";
    stmt = mySqlite.Statement(sql);
    if(!stmt->Bind(0, username))
    {
        ActionStatus actionStatus(ActionStatus::kDatabaseError);
        mySqlite.Close();
        finish<RegisterResponse*>(actionStatus, response);
        return Status::OK;
    }
    if(!stmt->Bind(1, password))
    {
        ActionStatus actionStatus(ActionStatus::kDatabaseError);
        mySqlite.Close();
        finish<RegisterResponse*>(actionStatus, response);
        return Status::OK;
    }
    if(!stmt->Bind(2, salt))
    {
        ActionStatus actionStatus(ActionStatus::kDatabaseError);
        mySqlite.Close();
        finish<RegisterResponse*>(actionStatus, response);
        return Status::OK;
    }
    if(!stmt->Bind(3, token))
    {
        ActionStatus actionStatus(ActionStatus::kDatabaseError);
        mySqlite.Close();
        finish<RegisterResponse*>(actionStatus, response);
        return Status::OK;
    }
    if(!stmt->Execute())
    {
        ActionStatus actionStatus(ActionStatus::kDatabaseError);
        mySqlite.Close();
        finish<RegisterResponse*>(actionStatus, response);
        return Status::OK; 
    }
    else
    {
        ActionStatus actionStatus(ActionStatus::kOk);
        mySqlite.Close();
        finish<RegisterResponse*>(actionStatus, response);
        response->set_token(token);
        return Status::OK;
    }
}

Status ServiceImpl::Test(ServerContext* context,
        const TestRequest* request, TestResponse* response)
{
    SqliteWrapper mySqlite;
    DbStatus dbStatus = mySqlite.Open(TEST_DB_FILE);
    if (!dbStatus.IsOk())
    {
        LOG_ERROR(dbStatus.ToString().c_str());
        ActionStatus actionStatus(ActionStatus::kDatabaseError);
        mySqlite.Close();
        finish<TestResponse*>(actionStatus, response);
        response->set_ret_str(actionStatus.ToString());
        return Status::OK;
    }
    string token = request->token();
    string sql = "SELECT USERNAME FROM USERINFO WHERE TOKEN = ?";
    SqliteStatement* stmt = mySqlite.Statement(sql);
    if(!stmt->Bind(0, token))
    {
        ActionStatus actionStatus(ActionStatus::kDatabaseError);
        mySqlite.Close();
        finish<TestResponse*>(actionStatus, response);
        response->set_ret_str(actionStatus.ToString());
        return Status::OK;
    }
    if(!stmt->NextRow())
    {
        ActionStatus actionStatus(ActionStatus::kInvalidToken);
        mySqlite.Close();
        finish<TestResponse*>(actionStatus, response);
        response->set_ret_str(actionStatus.ToString());
        return Status::OK;
    }
    else
    {
        string str = request->test_string();
        ActionStatus actionStatus(ActionStatus::kOk);
        mySqlite.Close();
        finish<TestResponse*>(actionStatus, response);
        response->set_ret_str(HELLO + str);
        return Status::OK;
    }
}

}  // namespace server
}  // namespace easylogin
