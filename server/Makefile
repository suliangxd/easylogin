server:user_info.pb.o user_info.grpc.pb.o action_status.o sqlite_wrapper.o main.o server.o
	g++ -std=c++11 -D_GLIBCXX_USE_CXX11_ABI=1 user_info.pb.o user_info.grpc.pb.o action_status.o sqlite_wrapper.o server.o main.o -L/usr/local/lib `pkg-config --libs protobuf grpc++ grpc` -Wl,--no-as-needed -lgrpc++_reflection -Wl,--as-needed -ldl -o server -lsqlite3 -luuid
user_info.pb.o:
	g++ -std=c++11 -D_GLIBCXX_USE_CXX11_ABI=1 `pkg-config --cflags protobuf grpc` -c -o user_info.pb.o /home/git_repository/easylogin/protocol/user_info.pb.cc
user_info.grpc.pb.o:
	g++ -std=c++11 -D_GLIBCXX_USE_CXX11_ABI=1 `pkg-config --cflags protobuf grpc` -c -o user_info.grpc.pb.o /home/git_repository/easylogin/protocol/user_info.grpc.pb.cc
action_status.o:
	g++ -std=c++11 -D_GLIBCXX_USE_CXX11_ABI=1 -c -o action_status.o /home/git_repository/easylogin/common/action_status.cpp -o action_status.o
sqlite_wrapper.o:
	g++ -std=c++11 -D_GLIBCXX_USE_CXX11_ABI=1 -c /home/git_repository/easylogin/database/sqlite_wrapper.cpp -o sqlite_wrapper.o -I ../
main.o:main.cpp
	g++ -std=c++11 -D_GLIBCXX_USE_CXX11_ABI=1 -c main.cpp -o main.o -I ../
server.o:server.cpp
	g++ -std=c++11 -D_GLIBCXX_USE_CXX11_ABI=1 -c server.cpp -o server.o -I ../
