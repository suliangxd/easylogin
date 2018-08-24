cc_binary(
    name = "client-bin",
    srcs = ["main.cpp"],
    copts = [
        "-std=c++11",
        "-D_GLIBCXX_USE_CXX11_ABI=1",
    ],
    deps = [":client",
        "//protocol:user_proto",
        "@com_github_grpc_grpc//:grpc++",
    ],
)

cc_library(
    name = "client",
    srcs = ["client.cpp"],
    hdrs = ["client.h"],
    copts = [
        "-std=c++11",
        "-D_GLIBCXX_USE_CXX11_ABI=1",
    ],
    deps = [
        "//common:common",
        "//protocol:user_proto",
        "@com_github_grpc_grpc//:grpc++",
    ],
)
