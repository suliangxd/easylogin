#include "database/sqlite_wrapper.h"

#include <stdint.h>
#include <sstream>

namespace easylogin
{
namespace database
{

string DbStatus::ToString() const
{
    std::stringstream ss;
    ss << "CODE: ";
    switch (code_)
    {
        case DbStatus::kOk:
            ss << "Ok";
            break;
        case DbStatus::kOpenError:
            ss << "Open Error";
            break;
        case DbStatus::kSelectError:
            ss << "Select Error";
            break;
        case DbStatus::kExecuteError:
            ss << "Execute Error";
            break;
        case DbStatus::kUnknowError:
            ss << "Unknow Error";
            break;
    }
    if (!msg_.empty())
    {
        ss << "  MSG: " << msg_;
    }
    return ss.str();
}

SqliteWrapper::SqliteWrapper() : db_(NULL) {}

SqliteWrapper::~SqliteWrapper()
{
    if (db_)
    {
        sqlite3_close(db_);
        db_ = NULL;
    }
}

DbStatus SqliteWrapper::Open(const string& dbFile)
{
    DbStatus status(DbStatus::kOk);
    if (sqlite3_open(dbFile.c_str(), &db_) != SQLITE_OK)
    {
        status.SetCode(DbStatus::kOpenError);
        status.SetMsg(string(sqlite3_errmsg(db_)));
    }
    return status;
}

void SqliteWrapper::Close()
{
    if (db_)
    {
        sqlite3_close(db_);
        db_ = NULL;
    }
}

DbStatus SqliteWrapper::Select(const string& sql, ResultTable& res)
{
    DbStatus status(DbStatus::kOk);
    res.Reset();
    char *errmsg;
    int ret = sqlite3_exec(db_, sql.c_str(), selectCallback,
                            static_cast<void*> (&res), &errmsg);
    if (ret != SQLITE_OK)
    {
        status.SetCode(DbStatus::kSelectError);
        status.SetMsg(string(errmsg));
        sqlite3_free(errmsg);
    }
    return status;
}

DbStatus SqliteWrapper::Execute(const string& sql)
{
    DbStatus status(DbStatus::kOk);
    char *errmsg;
    int ret = sqlite3_exec(db_, sql.c_str(), 0, 0, &errmsg);
    if (ret != SQLITE_OK)
    {
        status.SetCode(DbStatus::kExecuteError);
        status.SetMsg(string(errmsg));
        sqlite3_free(errmsg);
    }

    return status;
}

int SqliteWrapper::selectCallback(void *data, 
        int numFields, char **fields, char **colNames)
{
    ResultTable* res = reinterpret_cast<ResultTable*>(data);
    vector<string> record;
    for (uint32_t i = 0; i < numFields; i++)
    {
        if (fields[i])
        {
            record.push_back(fields[i]);
        }
        else
        {
            record.push_back(NULL);
        }
    }

  res->records_.push_back(record);

  return 0;
}

}  // namespace database
}  // namespace easylogin
