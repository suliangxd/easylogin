// #include <dlfcn.h>
#include "server/server.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using std::string;
using namespace easylogin::server;

const static string ADDR = "0.0.0.0:50051";
const static string LISTEN = "Server listen on ";

static void RunServer()
{
    string server_address(ADDR);
    ServiceImpl service;
    ServerBuilder builder;
    builder.AddListeningPort(server_address,
            grpc::InsecureServerCredentials());
    builder.RegisterService(&service);
    std::unique_ptr<Server> server(builder.BuildAndStart());
    std::cout << LISTEN << server_address << std::endl;
    server->Wait();
}

int main(int argc, char* argv[])
{
    RunServer();
    return 0;
}
