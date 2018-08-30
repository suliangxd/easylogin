#include "common/function.h"
#include "common/flags.h"
#include "common/log.h"
#include "common/sha256.h"
#include "database/sqlite_wrapper.h"
#include "server/server.h"
#include "thirdparty/bcrypt/bcrypt_wrapper.h"

using namespace easylogin::common;
using namespace easylogin::database;
using namespace easylogin::bcrypt;

const static std::string HELLO = "Hello ";

namespace easylogin
{
namespace server
{

std::string ServiceImpl::getToken()
{
    static const char letter[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    char ans[65];
    srand(time(0));
    for(int i = 0; i < 64; ++i)
    {
        ans[i] = letter[rand() % 64];
    }
    ans[64] = '\0';
    return std::string(ans);
}

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
    string password = request->password();
    string databasePassword;
    string databaseToken;
    string token = username + getToken();
    /*
    uuid_t uuid;
    uuid_generate(uuid);
    string token = uuid_to_string(uuid);
    */
    string sql = "SELECT PASSWORD,TOKEN FROM USERINFO WHERE USERNAME = ?";
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
        databaseToken = stmt->ValueString(1);
    }
    else
    {
        ActionStatus actionStatus
            (ActionStatus::kIncorrectUsernameOrPassword);
        mySqlite.Close();
        finish<LoginResponse*>(actionStatus, response);
        return Status::OK;
    }
    Bcrypt bcrypt;
    if(!bcrypt.checkPassword(password, databasePassword))
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
    string password = request->password();
    Bcrypt bcrypt;
    string hashPassword = bcrypt.generateHashPw(password);
    string token = username + getToken();
    /*
    uuid_t uuid;
    uuid_generate(uuid);
    string token = uuid_to_string(uuid);
    */
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
    sql = "INSERT INTO USERINFO(USERNAME, PASSWORD,  TOKEN)"\
           "VALUES(?,?,?)";
    stmt = mySqlite.Statement(sql);
    if(!stmt->Bind(0, username))
    {
        ActionStatus actionStatus(ActionStatus::kDatabaseError);
        mySqlite.Close();
        finish<RegisterResponse*>(actionStatus, response);
        return Status::OK;
    }
    if(!stmt->Bind(1, hashPassword))
    {
        ActionStatus actionStatus(ActionStatus::kDatabaseError);
        mySqlite.Close();
        finish<RegisterResponse*>(actionStatus, response);
        return Status::OK;
    }
    if(!stmt->Bind(2, token))
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
