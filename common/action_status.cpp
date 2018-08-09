#include <sstream>

#include "action_status.h"

namespace easylogin
{
namespace common
{

string ActionStatus::ToString() const
{
    std::stringstream ss;
    switch (code_)
    {
        case ActionStatus::kOk:
            ss << "OK";
            break;
        case ActionStatus::kDatabaseError:
            ss << "Database Error";
            break;
        case ActionStatus::kIncorrectPassword:
            ss << "Please Enter Correct Password";
            break;
        case ActionStatus::kUsernameUsed:
            ss << "The Username Has Been Registered";
            break;
        case ActionStatus::kInvalidToken:
            ss << "Your Token Has Expired";
            break;
        case ActionStatus::kUnknowError:
            ss << "Unknow Error";
            break;
    }
    if (!msg_.empty())
    {
        ss << "  Msg:  " << msg_;
    }
    return ss.str();
}

}  // namespace common
}  // namespace easylogin
