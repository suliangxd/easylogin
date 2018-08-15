#include <iostream>

#include "client/client.h"

using grpc::Channel;
using grpc::ClientContext;
using userinfo::UserAction;

using std::string;
using namespace easylogin::client;

const static string HOST = "localhost:50051";
const static string LOG = "L";
const static string REG = "R";
const static string QUIT = "Q";
const static string ERR = "Input Not Recognized";

int main(int argc, char* argv[])
{
    Client client(grpc::CreateChannel(
                HOST, grpc::InsecureChannelCredentials()));
    while (1)
    {
        std::cout << "Please Enter One Of The Following Strings." << std::endl;
        std::cout << "L  for login.  R  for register an account." << std::endl;
        std::cout << "Q  for quit." << std::endl;
        std::cout << std::endl;
        string str;
        getline(std::cin, str);
        if (str == LOG)
        {
            client.Login();
        }
        else if (str == REG)
        {
            client.Register();
        }
        else if (str == QUIT)
        {
            break;
        }
        else
        {
            std::cout << ERR << std::endl;
        }
    }
    return 0;
}
