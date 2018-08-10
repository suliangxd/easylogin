cc_binary(
    name = "client-bin",
    srcs = ["main.cpp"],
    copts = [
        "-pthread",
        "-I/usr/local/include",
        "-std=c++11",
        "-D_GLIBCXX_USE_CXX11_ABI=1",
    ],
    linkopts = [
        "-L/usr/local/lib",
        "-lgrpc++_reflection",
        "-ldl",
        "-lprotobuf", "-lpthread", "-lgrpc++", "-lgrpc"],
    deps = [
        ":client",
        "//protocol:user_proto",
    ],
)

cc_library(
    name = "client",
    srcs = ["client.cpp"],
    hdrs = ["client.h"],
    copts = [
        "-pthread",
        "-I/usr/local/include",
        "-std=c++11",
        "-D_GLIBCXX_USE_CXX11_ABI=1",
    ],
    linkopts = [
        "-L/usr/local/lib",
        "-lgrpc++_reflection",
        "-ldl",
        "-lprotobuf", "-lpthread", "-lgrpc++", "-lgrpc"],
    deps = [
        "//common:common",
        "//protocol:user_proto",
    ],
)
