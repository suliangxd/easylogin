server:user_info.pb.o user_info.grpc.pb.o action_status.o sqlite_wrapper.o sha256.o bcrypt.o crypt_blowfish.o crypt_gensalt.o wrapper.o main.o server.o
	g++ -std=c++11 -D_GLIBCXX_USE_CXX11_ABI=1 user_info.pb.o user_info.grpc.pb.o action_status.o sqlite_wrapper.o bcrypt.o crypt_blowfish.o crypt_gensalt.o wrapper.o main.o server.o -L/usr/local/lib `pkg-config --libs protobuf grpc++ grpc` -Wl,--no-as-needed -lgrpc++_reflection -Wl,--as-needed -ldl -o server -lsqlite3 -luuid
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
bcrypt.o:
	g++ -std=c++11 -D_GLIBCXX_USE_CXX11_ABI=1 -c ../thirdparty/bcrypt/bcrypt.c -o bcrypt.o -I ../
crypt_blowfish.o:
	g++ -std=c++11 -D_GLIBCXX_USE_CXX11_ABI=1 -c ../thirdparty/bcrypt/crypt_blowfish.c -o crypt_blowfish.o -I ../
crypt_gensalt.o:
	g++ -std=c++11 -D_GLIBCXX_USE_CXX11_ABI=1 -c ../thirdparty/bcrypt/crypt_gensalt.c -o crypt_gensalt.o -I ../
wrapper.o:
	g++ -std=c++11 -D_GLIBCXX_USE_CXX11_ABI=1 -c ../thirdparty/bcrypt/wrapper.c -o wrapper.o -I ../
main.o:main.cpp
	g++ -std=c++11 -D_GLIBCXX_USE_CXX11_ABI=1 -c main.cpp -o main.o -I ../
server.o:server.cpp
	g++ -std=c++11 -D_GLIBCXX_USE_CXX11_ABI=1 -c server.cpp -o server.o -I ../

clean:
	rm -rf *.o


