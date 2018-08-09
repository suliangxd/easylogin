#ifndef EASYLOGIN_COMMON_LOG_H
#define EASYLOGIN_COMMON_LOG_H

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>

using std::string;

namespace easylogin
{
namespace common
{

static string getCurrentTime();

#define LOG_INFO(logMsg)                                              \
    fprintf(stdout, "[%s  %s:%d]:  %s\n", getCurrentTime().c_str(),    \
            __FILE__, __LINE__, logMsg);                              \

#define LOG_ERROR(logMsg)                                             \
    fprintf(stderr, "[%s  %s:%d]:  %s\n", getCurrentTime().c_str(),    \
            __FILE__, __LINE__, logMsg);                              \


static string getCurrentTime()
{
    time_t timep;
    time(&timep);
    char tmp[64];
    strftime(tmp, sizeof(tmp), "%Y-%m-%d %H:%M:%S", localtime(&timep));
    return tmp;
}

}  // namespace common
}  // namespace easylogin
#endif  // EASYLOGIN_COMMON_LOG_H
