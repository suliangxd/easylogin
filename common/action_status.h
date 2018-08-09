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
        kIncorrectPassword,
        kUsernameUsed,
        kInvalidToken,
        kUnknowError
    };
    ActionStatus(Code code);
    ActionStatus(Code code, string msg);
    ~ActionStatus() {};

    string ToString() const;
private:
    Code code_;
    string msg_;
};

inline ActionStatus::ActionStatus(Code code) : code_(code) {}

inline ActionStatus::ActionStatus(Code code, string msg) :
    code_(code), msg_(msg) {}

}  // namespace common
}  // namespace easylogin

#endif  // EASYLOGIN_COMMON_ACTION_STATUS_H
