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
    // grpc::SslServerCredentialsOptions::PemKeyCertPair pkcp = {"a", "b"};
    // grpc::SslServerCredentialsOptions ssl_opts;
    // ssl_opts.pem_root_certs = "";
    // ssl_opts.pem_key_cert_pairs.push_back(pkcp);
    builder.AddListeningPort(server_address,
            grpc::InsecureServerCredentials());
    // builder.AddListeningPort(server_address,
            // grpc::SslServerCredentials(ssl_opts));
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
