cc_library(
    name = "user_proto",
    srcs = glob(["*.cc"]),
    hdrs = glob(["*.h"]),
	copts = [
        "-pthread",
        "-I/usr/local/include",
        "-std=c++11",
        "-D_GLIBCXX_USE_CXX11_ABI=1",
    ],
    visibility = ["//client:__pkg__", "//server:__pkg__"],
)
