#ifndef EASYLOGIN_CLIENT_CLIENT_H
#define EASYLOGIN_CLIENT_CLIENT_H

#include <memory>
#include <string>

#include <grpcpp/grpcpp.h>

#include "protocol/user_info.grpc.pb.h"

using grpc::Channel;
using userinfo::UserAction;

namespace easylogin
{
namespace client
{

class Client
{
public:
    Client(std::shared_ptr<Channel> channel)
        : stub_(UserAction::NewStub(channel)) {}

    void Login();
    void Register();
    void Sso();
private:
    std::unique_ptr<UserAction::Stub> stub_;
    std::string token_;
};

}  // namespace client
}  // namespace easylogin
#endif  // EASYLOGIN_CLIENT_CLIENT_H
