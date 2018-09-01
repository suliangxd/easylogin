cc_binary(
    name = "client-bin",
    srcs = ["main.cpp"],
    copts = [
        "-pthread",
        "-std=c++11",
        "-D_GLIBCXX_USE_CXX11_ABI=1",
    ],
    linkopts = [
        "-lpthread",
        "-ldl"
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
        "-pthread",
        "-std=c++11",
        "-D_GLIBCXX_USE_CXX11_ABI=1",
    ],
    linkopts = ["-ldl","-lpthread"],
    deps = [
        "//common:common",
        "//protocol:user_proto",
        "@com_github_grpc_grpc//:grpc++",
    ],
)
