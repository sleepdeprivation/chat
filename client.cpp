/*
	Initial client code gotten from:
		http://www.linuxhowtos.org/data/6/client.c
	subsequently annotated and edited heavily by cburke

*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/poll.h>
#include <pthread.h>
#include <iostream>

using namespace std;

pthread_t reader;	//global reader

int BUFFER_SIZE = 1024;

//when the syscall fails
void error(const char *msg)
{
	perror(msg);
	exit(0);
}


/*
	Read what the server is sending us (from the other clients)
*/
void* readFromServer(void* socket){

	int newsockfd = *((int *) socket);	//our fd to the server

	struct pollfd pfd;			//we're gonna keep polling
	int n = -1;

	char buffer[BUFFER_SIZE];	//we'll be reading from the connection to here

	while(1){

		pfd.fd = newsockfd;
		pfd.events = POLLIN;

		poll(&pfd, 1, -1);

		bzero(buffer,BUFFER_SIZE);				//zero out the buffer

		if(pfd.revents & POLLIN){
			n = read(pfd.fd, buffer, BUFFER_SIZE);
			if(n <= 0){
				break;
			}else{
				buffer[BUFFER_SIZE-1] = '\0';
				printf("SERVER:%s",buffer);	//k
			}
		}
	}

}

int main(int argc, char *argv[])
{
	//file desciptors, port numbers
	int sockfd, portno, n;
	
	//form to fill out
	struct sockaddr_in serv_addr;

	//second form for system to fill out for us
	struct hostent *server;

	//our buffer
	char buffer[BUFFER_SIZE];

	//need all the args
	if (argc < 3) {
	  fprintf(stderr,"usage %s hostname port\n", argv[0]);
	  exit(0);
	}

	//parse
	portno = atoi(argv[2]);

	//make the right type of socket
	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	if (sockfd < 0) 
	   error("ERROR opening socket");

	server = gethostbyname(argv[1]);	//generate a hostent from it's name

	if (server == NULL) {
	   fprintf(stderr,"ERROR, no such host\n");
	   exit(0);
	}

	//clear the buffer
	bzero((char *) &serv_addr, sizeof(serv_addr));

	//filling out the form and copying fvalues from our hostent
	serv_addr.sin_family = AF_INET;
	bcopy((char *)server->h_addr, 
		(char *)&serv_addr.sin_addr.s_addr,
		server->h_length);
	serv_addr.sin_port = htons(portno);

	//cue dialup sound
	if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
	   error("ERROR connecting... Something bad happened");

printf(">");
fgets(buffer,BUFFER_SIZE-1,stdin);

double period;

cout << "repeater: ";
cin >> period;
cout << period << endl;

	pthread_create(&reader, NULL, &readFromServer, (void *) &sockfd);
	while(1){

		sleep(period);

		//prompt user to send a message
		//printf(">");

		//kill buffer
		//bzero(buffer,BUFFER_SIZE);

		//eat up stdin into buffer
		//fgets(buffer,BUFFER_SIZE-1,stdin);

		//write to socket
		n = write(sockfd,buffer,strlen(buffer));

		if (n < 0) 
			error("ERROR writing to socket");
	}
	//kill the buffer
	bzero(buffer,BUFFER_SIZE);

	//always remember to close your files
	close(sockfd);

	return 0;
}
