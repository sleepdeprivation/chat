client:
	gcc -o client client.c
server:
	gcc -o server server.c
clean:
	rm ./*~ client server
