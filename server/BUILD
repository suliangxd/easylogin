cc_binary(
    name = "server-bin",
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
        ":server",
        "//protocol:user_proto",
    ],
)
cc_library(
    name = "server",
    srcs = ["server.cpp"],
    hdrs = ["server.h"],
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
        "-lprotobuf", "-lpthread", "-lgrpc++", "-lgrpc",
        "-lsqlite3", "-luuid"],
    deps = [
        "//common:common",
		"//database:sqlite_wrapper",
        "//protocol:user_proto",
    ],
)
