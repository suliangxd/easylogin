#ifndef EASYLOGIN_DATABASE_SQLITE_WRAPPER_H
#define EASYLOGIN_DATABASE_SQLITE_WRAPPER_H

#include <string>
#include <vector>

#include "sqlite3.h"

using std::string;
using std::vector;

namespace easylogin
{
namespace database
{

class DbStatus
{
public:
    enum Code
    {
        kOk = 0,
        kOpenError,
        kSelectError,
        kExecuteError,
        kUnknowError
    };
    DbStatus(Code code);
    DbStatus(Code code, string msg);
    ~DbStatus() {}
    void operator=(const DbStatus& s);

    bool IsOk() const;
    string ToString() const;
    void SetCode(Code code);
    void SetMsg(string msg);
private:
    Code code_;
    string msg_;
};

class SqliteStatement
{
private:
    friend class SqliteWrapper;
    SqliteStatement(const string& statement, sqlite3* db);
public:
    SqliteStatement();
    ~SqliteStatement();
    
    string ValueString(int pos_zero_indexed);
    bool Bind(int pos_zero_indexed, const string& value);
    bool Bind(int pos_zero_indexed, double value);
    bool Bind(int pos_zero_indexed, int value);

    bool Execute();
    bool NextRow();
    bool Reset();
    bool RestartSelect();
private:
    sqlite3_stmt* stmt_;
};

class SqliteWrapper
{
public:
    SqliteWrapper();
    ~SqliteWrapper();

    struct ResultTable {
        vector<vector<string> > records_;

        void Reset()
        {
            records_.clear();
        }
    };

    DbStatus Open(const string& dbFile);
    void Close();

    DbStatus Select(const string& sql, ResultTable& res);
    DbStatus Execute(const string& sql);
    SqliteStatement* Statement(const string& stmt);
private:
    sqlite3* db_;
    SqliteStatement* statment_;
	static int selectCallback(void *data, int numFields, 
            char **fields, char **colNames);
};

inline DbStatus::DbStatus(Code code) : code_(code) {}

inline DbStatus::DbStatus(Code code, string msg) : code_(code), msg_(msg) {}

inline void DbStatus::operator=(const DbStatus& s)
{
    code_ = s.code_;
    msg_  = s.msg_;
}

inline bool DbStatus::IsOk() const
{
    return code_ == kOk;
}

inline void DbStatus::SetCode(Code code)
{
    code_ = code;
}

inline void DbStatus::SetMsg(string msg)
{
    msg_ = msg;
}

}  // namespace database
}  // namespace easylogin
#endif  // EASYLOGIN_DATABASE_SQLITER_WRAPPER_H
