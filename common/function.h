#ifndef EASYLOGIN_COMMON_FUNCTION_H
#define EASYLOGIN_COMMON_FUNCTION_H

#include <uuid/uuid.h>

namespace easylogin
{
namespace common
{

string uuid_to_string(const uuid_t uu)
{
    char str[50];
    for (int i = 0; i < 16; i++)
    {
        sprintf(str + i * 2, "%02X", uu[i]);
    }
    return str;
}

}  // namespace common
}  // namespace easylogin

#endif  // EASYLOGIN_COMMON_COMMON_H
