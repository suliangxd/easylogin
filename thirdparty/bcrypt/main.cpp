#include <iostream>
#include <string>
#include "bcrypt_wrapper.h"

using namespace easylogin::bcrypt;

int main()
{
    Bcrypt bcrypt;
    std::string hash = bcrypt.generateHashPw("helloworld");
    std::cout << hash << std::endl;
    std::cout << bcrypt.checkPassword("helloworld", hash) << std::endl;
}
