#include <stdlib.h>
#include "common/flags.h"
#include "common/log.h"
#include "database/sqlite_wrapper.h"
#include "server/server.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using std::string;
using namespace easylogin::common;
using namespace easylogin::database;
using namespace easylogin::server;

static const char* ADDR = "0.0.0.0:50051";

static void InitDb();
static void InitServer();
static void RunServer();

int main(int argc, char* argv[])
{
    InitServer();
    RunServer();

    return 0;
}

static void RunServer()
{
    string server_address(ADDR);
    ServiceImpl service;
    ServerBuilder builder;
    builder.AddListeningPort(server_address,
            grpc::InsecureServerCredentials());
    builder.RegisterService(&service);
    std::unique_ptr<Server> server(builder.BuildAndStart());
    LOG_INFO("Server run success!");
    server->Wait();
}

static void InitServer()
{
    InitDb();
}

static void InitDb()
{
    SqliteWrapper mySqlite;
    DbStatus status = mySqlite.Open(TEST_DB_FILE);
    if (!status.IsOk())
    {
        LOG_ERROR(status.ToString().c_str());
        exit(1);
    }
    string sql = "CREATE TABLE IF NOT EXISTS USERINFO("      \
                 "USERNAME CHAR(128) PRIMARY KEY NOT NULL,"  \
                 "PASSWORD CHAR(512) NOT NULL,"              \
                 "TOKEN CHAR(258) NOT NULL)";

    status = mySqlite.Execute(sql);
    if (!status.IsOk())
    {
        LOG_ERROR("Create table error");
        LOG_ERROR(status.ToString().c_str());
        exit(1);
    }
    LOG_INFO("Db init success!");
}
