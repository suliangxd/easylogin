client:user_info.pb.o user_info.grpc.pb.o md5.o main.o client.o
	g++ -std=c++11 -D_GLIBCXX_USE_CXX11_ABI=1 user_info.pb.o user_info.grpc.pb.o md5.o client.o main.o -L/usr/local/lib `pkg-config --libs protobuf grpc++ grpc` -Wl,--no-as-needed -lgrpc++_reflection -Wl,--as-needed -ldl -o client
user_info.pb.o:
	g++ -std=c++11 -D_GLIBCXX_USE_CXX11_ABI=1 `pkg-config --cflags protobuf grpc` -c -o user_info.pb.o ../protocol/user_info.pb.cc
user_info.grpc.pb.o:
	g++ -std=c++11 -D_GLIBCXX_USE_CXX11_ABI=1 `pkg-config --cflags protobuf grpc` -c -o user_info.grpc.pb.o ../protocol/user_info.grpc.pb.cc
md5.o:
	g++ -std=c++11 -D_GLIBCXX_USE_CXX11_ABI=1 -c -o md5.o ../common/md5.cpp 
main.o:main.cpp
	g++ -std=c++11 -D_GLIBCXX_USE_CXX11_ABI=1 -c main.cpp -o main.o -I ../
client.o:client.cpp
	g++ -std=c++11 -D_GLIBCXX_USE_CXX11_ABI=1 -c client.cpp -o client.o -I ../

clean:
	rm -rf *.o
