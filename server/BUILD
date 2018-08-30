cc_binary(
    name = "server-bin",
    srcs = ["main.cpp"],
    copts = [
        "-pthread",
        "-std=c++11",
        "-D_GLIBCXX_USE_CXX11_ABI=1",
    ],
    linkopts = [
        "-ldl",
        "-lpthread"
    ],
    deps = [
        ":server",
        "//protocol:user_proto",
        "@com_github_grpc_grpc//:grpc++",
    ],
)

cc_library(
    name = "server",
    srcs = ["server.cpp"],
    hdrs = ["server.h"],
    copts = [
        "-pthread",
        "-std=c++11",
        "-D_GLIBCXX_USE_CXX11_ABI=1",
    ],
    linkopts = ["-ldl","-lpthread"],
    deps = [
        "//common:common",
        "//protocol:user_proto",
        "//database:sqlite_wrapper",
        "//thirdparty/bcrypt:bcrypt",
        "//thirdparty/sqlite3:sqlite3",
        "@com_github_grpc_grpc//:grpc++",
    ],
)
