main:bcrypt.o crypt_blowfish.o crypt_gensalt.o wrapper.o main.o
	g++ -std=c++11 *.o -o main
bcrypt.o:bcrypt.c
	g++ -std=c++11 -c bcrypt.c -o bcrypt.o
crypt_blowfish.o:crypt_blowfish.c
	g++ -std=c++11 -c crypt_blowfish.c -o crypt_blowfish.o
crypt_gensalt.o:crypt_gensalt.c
	g++ -std=c++11 -c crypt_gensalt.c -o crypt_gensalt.o
wrapper.o:
	g++ -std=c++11 -c wrapper.c -o wrapper.o
main.o:main.cpp
	g++ -std=c++11 -c main.cpp -o main.o
clean:
	rm -rf *.o main
