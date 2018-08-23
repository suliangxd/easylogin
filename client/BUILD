cc_binary(
    name = "client-bin",
    srcs = ["main.cpp"],
    copts = [
        "-std=c++11",
        "-D_GLIBCXX_USE_CXX11_ABI=1",
    ],
    deps = [":client",
        "//protocol:user_proto",
        "@thirdparty_grpc//:grpc++",
    ],
)

cc_library(
    name = "client",
    srcs = ["client.cpp"],
    hdrs = ["client.h"],
    cops = [
        "-std=c++11",
        "-D_GLIBCXX_USE_CXX11_ABI=1",
    ],
    deps = [
        "//common:common",
        "//protocol:user_proto",
        "@thirdparty_grpc//:grpc++",
    ],
)
