#ifndef EASYLOGIN_COMMON_ACTION_STATUS_H
#define EASYLOGIN_COMMON_ACTION_STATUS_H

#include <string>

using std::string;

namespace easylogin
{
namespace common
{

class ActionStatus
{
public:
    enum Code
    {
        kOk = 0,
        kDatabaseError,
        kIncorrectUsernameOrPassword,
        kUsernameUsed,
        kInvalidToken,
        kUnknowError
    };
    ActionStatus(Code code);
    ActionStatus(Code code, string msg);
    ~ActionStatus() {};

    string ToString() const;

    Code GetCode() const;
private:
    Code code_;
    string msg_;
};

inline ActionStatus::ActionStatus(Code code) : code_(code) {}

inline ActionStatus::ActionStatus(Code code, string msg) :
    code_(code), msg_(msg) {}

inline ActionStatus::Code ActionStatus::GetCode() const
{
    return code_;
}

}  // namespace common
}  // namespace easylogin

#endif  // EASYLOGIN_COMMON_ACTION_STATUS_H
