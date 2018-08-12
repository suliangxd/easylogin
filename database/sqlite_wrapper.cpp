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

SqliteWrapper::SqliteWrapper() : db_(NULL), statment_(NULL){}

SqliteWrapper::~SqliteWrapper()
{
    if (statment_)
    {
        delete statment_;
        statment_ = NULL;
    }
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

SqliteStatement* SqliteWrapper::Statement(const string& statement)
{
    if (statment_)
    {
        delete statment_;
        statment_ = NULL;
    }
    SqliteStatement* stmt;
    try
    {
        stmt = new SqliteStatement(statement, db_);
        statment_ = stmt;
        return stmt;
    }
    catch(const char* e)
    {
        return 0;
    }
}

SqliteStatement::SqliteStatement(const string& statement, sqlite3* db)
{
    if(sqlite3_prepare(db, statement.c_str(), -1, &stmt_, 0) != SQLITE_OK)
    {
        throw sqlite3_errmsg(db);
    }
    if(!stmt_)
    {
        throw "stmt_ is 0";
    }
}

SqliteStatement::~SqliteStatement()
{
    if(stmt_)
    {
        sqlite3_finalize(stmt_);
    }
}

SqliteStatement::SqliteStatement():stmt_(0){}

bool SqliteStatement::Bind(int pos_zero_indexed, const string& value)
{
    if(sqlite3_bind_text(stmt_, pos_zero_indexed + 1, value.c_str(),
                value.length(), SQLITE_TRANSIENT) != SQLITE_OK)
    {
        return false;
    }
    return true;
}

bool SqliteStatement::Bind(int pos_zero_indexed, double value)
{
    if(sqlite3_bind_double(stmt_, pos_zero_indexed + 1, value) != SQLITE_OK)
    {
        return false;
    }
    return true;
}

bool SqliteStatement::Bind(int pos_zero_indexed, int value)
{
    if(sqlite3_bind_int(stmt_, pos_zero_indexed + 1, value) != SQLITE_OK)
    {
        return false;
    }
    return true;
}

bool SqliteStatement::Execute()
{
    int rc = sqlite3_step(stmt_);
    if(rc == SQLITE_BUSY || rc == SQLITE_ERROR || rc == SQLITE_MISUSE
            || rc != SQLITE_DONE)
    {
        return false;
    }
    sqlite3_reset(stmt_);
    return true;
}

bool SqliteStatement::Reset()
{
    int rc = sqlite3_step(stmt_);
    sqlite3_reset(stmt_);
    if(rc == SQLITE_ROW)
    {
        return true;
    }
    return false;
}

bool SqliteStatement::NextRow()
{
    int rc = sqlite3_step(stmt_);
    if(rc == SQLITE_ROW)
    {
        return true;
    }
    if(rc == SQLITE_DONE)
    {
        sqlite3_reset(stmt_);
        return false;
    }
    else
    {
        return false;
    }
}

bool SqliteStatement::RestartSelect()
{
    sqlite3_reset(stmt_);
    return true;
}

string SqliteStatement::ValueString(int pos_zero_indexed)
{
    return string(reinterpret_cast<const char*>
            (sqlite3_column_text(stmt_, pos_zero_indexed)));
}
}  // namespace database
}  // namespace easylogin
