#include <iostream>
#include "common/flags.h"
#include "common/log.h"
#include "database/sqlite_wrapper.h"

using namespace std;
using namespace easylogin::common;
using namespace easylogin::database;

int main()
{
    SqliteWrapper mySqlite;
    DbStatus dbStatus = mySqlite.Open(TEST_DB_FILE);
    if (!dbStatus.IsOk())
    {
        LOG_ERROR(dbStatus.ToString().c_str());
        return 0;
    }
    string sql = "INSERT INTO USER (USERNAME, PASSWD) " \
                 "VALUES ('tom2', '12345678')";

    dbStatus = mySqlite.Execute(sql);
    if (!dbStatus.IsOk())
    {
        LOG_ERROR(dbStatus.ToString().c_str());
        return 0;
    }
    else
    {
        LOG_INFO("insert success");
    }
    
    sql = "SELECT PASSWD FROM USER WHERE USERNAME = 'tom'";
    SqliteWrapper::ResultTable retTable;
    dbStatus = mySqlite.Select(sql, retTable);
    if (!dbStatus.IsOk())
    {
        LOG_ERROR(dbStatus.ToString().c_str());
        return 0;
    }
    else
    {
        LOG_INFO("select success");
        cout << retTable.records_[0][0] << endl;
    }
    mySqlite.Close();
    return 0;
}
