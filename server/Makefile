server:user_info.pb.o user_info.grpc.pb.o action_status.o sqlite_wrapper.o main.o server.o sha256.o
	g++ -std=c++11 -D_GLIBCXX_USE_CXX11_ABI=1 user_info.pb.o user_info.grpc.pb.o action_status.o sqlite_wrapper.o server.o main.o -L/usr/local/lib `pkg-config --libs protobuf grpc++ grpc` -Wl,--no-as-needed -lgrpc++_reflection -Wl,--as-needed -ldl -o server -lsqlite3 -luuid
user_info.pb.o:
	g++ -std=c++11 -D_GLIBCXX_USE_CXX11_ABI=1 `pkg-config --cflags protobuf grpc` -c -o user_info.pb.o ../protocol/user_info.pb.cc
user_info.grpc.pb.o:
	g++ -std=c++11 -D_GLIBCXX_USE_CXX11_ABI=1 `pkg-config --cflags protobuf grpc` -c -o user_info.grpc.pb.o ../protocol/user_info.grpc.pb.cc
action_status.o:
	g++ -std=c++11 -D_GLIBCXX_USE_CXX11_ABI=1 -c -o action_status.o ../common/action_status.cpp -o action_status.o
sqlite_wrapper.o:
	g++ -std=c++11 -D_GLIBCXX_USE_CXX11_ABI=1 -c ../database/sqlite_wrapper.cpp -o sqlite_wrapper.o -I ../
sha256.o:
	g++ -std=c++11 -D_GLIBCXX_USE_CXX11_ABI=1 -c ../common/sha256.h -o sha256.o
main.o:main.cpp
	g++ -std=c++11 -D_GLIBCXX_USE_CXX11_ABI=1 -c main.cpp -o main.o -I ../
server.o:server.cpp
	g++ -std=c++11 -D_GLIBCXX_USE_CXX11_ABI=1 -c server.cpp -o server.o -I ../

clean:
	rm -rf *.o


