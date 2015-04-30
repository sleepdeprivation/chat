client: client.cpp
	g++ -std=c++11 -o client client.cpp -lpthread
server: server.cpp Chat.o User.o
	g++ -g -std=c++11 -o server -pthread server.cpp Chat.o User.o
User.o: Chat.h User.h User.cpp
	g++ -c -std=c++11 -o User.o User.cpp
Chat.o: Chat.h Chat.cpp User.h
	g++ -c -std=c++11 -o Chat.o Chat.cpp

clean:
	rm ./*~ client server User.o Chat.o
