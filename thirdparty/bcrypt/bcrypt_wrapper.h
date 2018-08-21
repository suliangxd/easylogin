#ifndef EASYLOGIN_THIRDPARTY_BCRYPT_BCRYPT_WRAPPER_H
#define EASYLOGIN_THIRDPARTY_BCRYPT_BCRYPT_WRAPPER_H

#include <stdexcept>
#include <string>

#include "thirdparty/bcrypt/bcrypt.h"

namespace easylogin
{
namespace bcrypt
{

class Bcrypt
{
public:
    static std::string generateHashPw(const std::string& password,
            int workFactor = 12)
    {
        char salt[BCRYPT_HASHSIZE];
        char hash[BCRYPT_HASHSIZE];
        int ans;
        ans = bcrypt_gensalt(workFactor, salt);
        if(ans != 0)
        {
            throw std::runtime_error{"bcrypt: can not generate salt"};
        }
        ans = bcrypt_hashpw(password.c_str(), salt, hash);
        if(ans != 0)
        {
            throw std::runtime_error{"bcrypt: can not generate hash"};
        }
        return std::string{hash};
    }

    static bool checkPassword(const std::string& password,
            const std::string& hash)
    {
        int ans = bcrypt_checkpw(password.c_str(), hash.c_str());
        if(ans == 0)
        {
            return true;
        }
        return false;
    }
};

}  // namespace bcrypt
}  // namespace easylogin

#endif  // EASYLOGIN_THIRDPARTY_BCRYPT_BCRYPT_WRAPPER_H
