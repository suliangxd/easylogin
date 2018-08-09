#ifndef EASYLOGIN_SERVER_SERVER_H
#define EASYLOGIN_SERVER_SERVER_H

#include <grpcpp/grpcpp.h>
#include <memory>
#include <string>

#include "protocol/user_info.grpc.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using userinfo::LoginRequest;
using userinfo::LoginResponse;
using userinfo::RegisterRequest;
using userinfo::RegisterResponse;
using userinfo::TestRequest;
using userinfo::TestResponse;
using userinfo::UserAction;

namespace easylogin
{
namespace server
{
class ServiceImpl final : public UserAction::Service
{
    Status Login(ServerContext* context, const LoginRequest* request,
            LoginResponse* response) override;

    Status Register(ServerContext* context, const RegisterRequest* request,
            RegisterResponse* response) override;

    Status Test(ServerContext* context, const TestRequest* request,
            TestResponse* response) override;
};

}  // namespace server 
}  // namespace easylogin

#endif  // EASYLOGIN_SERVER_SERVER_H
