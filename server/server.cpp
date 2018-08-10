#include "common/action_status.h"
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

const static std::string HELLO = "Hello ";

namespace easylogin
{
namespace server
{
static string uuid_to_string(const uuid_t uu)
{
    char str[50];
    for (int i = 0; i < 16; i++)
    {
        sprintf(str + i * 2, "%02X", uu[i]);
    }
    return str;
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
        response->set_ret_code(ActionStatus::kDatabaseError);
        response->set_ret_msg(actionStatus.ToString());
        response->set_token(actionStatus.ToString());
        return Status::OK;
    }
    string username = request->username();
    string password = hash256_hex_string(request->password());
    uuid_t uu;
    uuid_generate(uu);
    string token = uuid_to_string(uu);
    string sql = "SELECT PASSWORD FROM USERINFO WHERE USERNAME = '";
    sql = sql + username + "'";
    SqliteWrapper::ResultTable retTable;
    dbStatus = mySqlite.Select(sql, retTable);
    if (!dbStatus.IsOk())
    {
        LOG_ERROR(dbStatus.ToString().c_str());
        ActionStatus actionStatus(ActionStatus::kDatabaseError);
        response->set_ret_code(ActionStatus::kDatabaseError);
        response->set_ret_msg(actionStatus.ToString());
        response->set_token(actionStatus.ToString());
        mySqlite.Close();
        return Status::OK;
    }
    if (retTable.records_.empty() || retTable.records_[0][0] == "NULL"
            ||password != retTable.records_[0][0])
    {
        ActionStatus actionStatus(ActionStatus::kIncorrectPassword);
        response->set_ret_code(ActionStatus::kIncorrectPassword);
        response->set_ret_msg(actionStatus.ToString());
        response->set_token(actionStatus.ToString());
        mySqlite.Close();
        return Status::OK;
    }
    sql = "UPDATE USERINFO SET TOKEN = '";
    sql = sql + token + "' WHERE USERNAME = '" + username + "'";
    dbStatus = mySqlite.Execute(sql);
    if (!dbStatus.IsOk())
    {
        LOG_ERROR(dbStatus.ToString().c_str());
        ActionStatus actionStatus(ActionStatus::kDatabaseError);
        response->set_ret_code(ActionStatus::kDatabaseError);
        response->set_ret_msg(actionStatus.ToString());
        response->set_token(actionStatus.ToString());
        mySqlite.Close();
        return Status::OK;
    }
    else
    {
        ActionStatus actionStatus(ActionStatus::kOk);
        response->set_ret_code(ActionStatus::kOk);
        response->set_ret_msg(actionStatus.ToString());
        response->set_token(token);
        mySqlite.Close();
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
        response->set_ret_code(ActionStatus::kDatabaseError);
        response->set_ret_msg(actionStatus.ToString());
        response->set_token(actionStatus.ToString());
        mySqlite.Close();
        return Status::OK;
    }
    string username = request->username();
    string password = hash256_hex_string(request->password());
    uuid_t uu;
    uuid_generate(uu);
    string token = uuid_to_string(uu);
    string sql = "SELECT USERNAME FROM USERINFO WHERE USERNAME = '";
    sql = sql + username + "'";
    SqliteWrapper::ResultTable retTable;
    dbStatus = mySqlite.Select(sql, retTable);
    if (!dbStatus.IsOk())
    {
        LOG_ERROR(dbStatus.ToString().c_str());
        ActionStatus actionStatus(ActionStatus::kDatabaseError);
        response->set_ret_code(ActionStatus::kDatabaseError);
        response->set_ret_msg(actionStatus.ToString());
        response->set_token(actionStatus.ToString());
        mySqlite.Close();
        return Status::OK;
    }
    if (retTable.records_.empty() || retTable.records_[0][0] == "NULL")
    {
        sql = "INSERT INTO USERINFO (USERNAME, PASSWORD, TOKEN)"\
                "VALUES ('";
        sql = sql + username + "', '" + password + "', '" + token + "')";
        dbStatus = mySqlite.Execute(sql);
        if (!dbStatus.IsOk())
        {
            LOG_ERROR(dbStatus.ToString().c_str());
            ActionStatus actionStatus(ActionStatus::kDatabaseError);
            response->set_ret_code(ActionStatus::kDatabaseError);
            response->set_ret_msg(actionStatus.ToString());
            response->set_token(actionStatus.ToString());
            mySqlite.Close();
            return Status::OK;
        }
        ActionStatus actionStatus(ActionStatus::kOk);
        response->set_ret_code(ActionStatus::kOk);
        response->set_ret_msg(actionStatus.ToString());
        response->set_token(token);
        mySqlite.Close();
        return Status::OK;
    }
    else
    {
        ActionStatus actionStatus(ActionStatus::kUsernameUsed);
        response->set_ret_code(ActionStatus::kUsernameUsed);
        response->set_ret_msg(actionStatus.ToString());
        response->set_token(actionStatus.ToString());
        mySqlite.Close();
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
        response->set_ret_code(ActionStatus::kDatabaseError);
        response->set_ret_msg(actionStatus.ToString());
        response->set_ret_str(actionStatus.ToString());
        mySqlite.Close();
        return Status::OK;
    }
    string token = request->token();
    string sql = "SELECT USERNAME FROM USERINFO WHERE TOKEN = '";
    sql = sql + token + "'";
    SqliteWrapper::ResultTable retTable;
    dbStatus = mySqlite.Select(sql, retTable);
    if (!dbStatus.IsOk())
    {
        LOG_ERROR(dbStatus.ToString().c_str());
        ActionStatus actionStatus(ActionStatus::kDatabaseError);
        response->set_ret_code(ActionStatus::kDatabaseError);
        response->set_ret_msg(actionStatus.ToString());
        response->set_ret_str(actionStatus.ToString());
        mySqlite.Close();
        return Status::OK;
    }
    else if (retTable.records_.empty()|| retTable.records_[0][0] == "NULL")
    {
        ActionStatus actionStatus(ActionStatus::kInvalidToken);
        response->set_ret_code(ActionStatus::kInvalidToken);
        response->set_ret_msg(actionStatus.ToString());
        response->set_ret_str(actionStatus.ToString());
        mySqlite.Close();
        return Status::OK;
    }
    else
    {
        string str = request->test_string();
        ActionStatus actionStatus(ActionStatus::kOk);
        response->set_ret_code(ActionStatus::kOk);
        response->set_ret_msg(actionStatus.ToString());
        response->set_ret_str(HELLO + str);
        mySqlite.Close();
        return Status::OK;
    }
}

}  // namespace server
}  // namespace easylogin
