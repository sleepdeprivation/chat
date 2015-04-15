client:
	gcc -o client client.c -lpthread
server:
	gcc -o server server.c -lpthread
clean:
	rm ./*~ client server
