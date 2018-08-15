#ifndef EASYLOGIN_SERVER_SERVER_H
#define EASYLOGIN_SERVER_SERVER_H

#include <memory>
#include <string>
#include <set>

#include <grpcpp/grpcpp.h>

#include "common/action_status.h"
#include "protocol/user_info.grpc.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::ServerReaderWriter;
using grpc::ServerWriter;
using grpc::Status;
using userinfo::LoginRequest;
using userinfo::LoginResponse;
using userinfo::RegisterRequest;
using userinfo::RegisterResponse;
using userinfo::SsoRequest;
using userinfo::SsoResponse;
using userinfo::UserAction;
using namespace easylogin::common;

namespace easylogin
{
namespace server
{
class ServiceImpl final : public UserAction::Service
{
private:
    Status Login(ServerContext* context, const LoginRequest* request,
            LoginResponse* response) override;

    Status Register(ServerContext* context, const RegisterRequest* request,
            RegisterResponse* response) override;

    Status Sso(ServerContext* context, const SsoRequest* request,
            ServerWriter<SsoResponse>* writer) override;

    template<typename T>   
    void finish(ActionStatus status, T response);

private:
    std::set<std::string> invalidToken_;
};

}  // namespace server 
}  // namespace easylogin

#endif  // EASYLOGIN_SERVER_SERVER_H
