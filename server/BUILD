cc_binary(
    name = "server-bin",
    srcs = ["main.cpp"],
    copts = [
        "-std=c++11",
        "-D_GLIBCXX_USE_CXX11_ABI=1",
    ],
    deps = [
        ":server",
        "//protocol:user_proto",
        "@thirdparty_grpc//:grpc++",
    ],
)

cc_library(
    name = "server",
    srcs = ["server.cpp"],
    hdrs = ["server.h"],
    copts = [
        "-std=c++11",
        "-D_GLIBCXX_USE_CXX11_ABI=1",
    ],
    deps = [
        "//common:common",
        "//database:sqlite_wrapper",
        "//protocol:user_proto",
        "//third_party/bcrypt:bcrypt",
        "//third_party/sqlite3:sqlite3",
        "//third_party/uuid:uuid",
        "@thirdparty_grpc//:grpc++",
    ],
)
