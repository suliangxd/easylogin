#include "common/function.h"
#include "common/flags.h"
#include "common/log.h"
#include "common/sha256.h"
#include "database/sqlite_wrapper.h"
#include "server/server.h"

using namespace easylogin::common;
using namespace easylogin::database;

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
    string salt;
    string password = request->password();
    string databasePassword;
    string databaseToken;
    uuid_t uuid;
    uuid_generate(uuid);
    string token = uuid_to_string(uuid);
    string sql = "SELECT PASSWORD,SALT,TOKEN FROM USERINFO WHERE USERNAME = ?";
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
        databasePassword = stmt->ValueString(0);
        salt = stmt->ValueString(1);
        databaseToken = stmt->ValueString(2);
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
    if(password != databasePassword)
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
        invalidToken_.insert(databaseToken);
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

Status ServiceImpl::Sso(ServerContext* context, const SsoRequest* request,
        ServerWriter<SsoResponse>* writer)
{
    SsoResponse response;
    string token =  request->token(); 
    while(1)
    {
        if(invalidToken_.erase(token))
        {
            ActionStatus actionStatus(ActionStatus::kInvalidToken);
            finish<SsoResponse*>(actionStatus, &response);
            if (writer->Write(response))
            {
                return Status::OK;
            }
            return Status::CANCELLED;
        }
    }
}

}  // namespace server
}  // namespace easylogin
