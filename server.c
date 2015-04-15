/* A simple server in the internet domain using TCP
   The port number is passed as an argument
	Initially from
		http://www.linuxhowtos.org/data/6/server.c
	subsequently annotated and edited heavily by cburke
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/poll.h>
#include <pthread.h>


pthread_t tid[2];	//global thread holder

int sid[2];

pthread_t writer;	//global writer

int BUFFER_SIZE = 1024;

//called when a syscall fails
void error(const char *msg)
{
	perror(msg);
	exit(1);
}


void* outputAll(void* buf){
	char * buffer = (char *) buf;
	int ii;
	for(ii = 0; ii < 2; ii++){
		printf("\nwriting to socket id %d\n", sid[ii]);
		write(sid[ii],buffer,strlen(buffer));
	}

}


void* handleInput(void* socket){

	int newsockfd = *((int *) socket);
	int n;


	printf("\nthread created with socket id %d\n", newsockfd);

	struct pollfd pfd;

	char buffer[BUFFER_SIZE];	//we'll be reading from the connection to here

	while(1){
		printf("polling\n");

		pfd.fd = newsockfd;
		pfd.events = POLLIN;

		poll(&pfd, 1, -1);

		bzero(buffer,BUFFER_SIZE);				//zero out the buffer

		if(pfd.revents & POLLIN){
			n = read(pfd.fd, buffer, 1024);
			if(n <= 0){
				break;
			}else{
				buffer[BUFFER_SIZE-1] = '\0';
				pthread_create(&writer, NULL, &outputAll, (void *) &buffer);
				printf(">%s",buffer);	//k
			}
		}
	}
	close(newsockfd);
}

int main(int argc, char *argv[])
{
	//file descriptors and a port number
	int sockfd, newsockfd, portno;
	socklen_t clilen;
	char buffer[BUFFER_SIZE];	//we'll be reading from the connection to here

	struct sockaddr_in serv_addr, cli_addr;		//structs containing network connection info

	int n;

	if (argc < 2) {
	    fprintf(stderr,"ERROR, no port provided\n");
	    exit(1);
	}

	sockfd = socket(AF_INET, SOCK_STREAM, 0); //connecting

	if (sockfd < 0) 
	   error("ERROR opening socket");

	bzero((char *) &serv_addr, sizeof(serv_addr));	//zero out serv_addr

	portno = atoi(argv[1]);				//user provoids the port

			//fill out the form- i mean... struct - yeah
	serv_addr.sin_family = AF_INET;			//we're on the internet!
	serv_addr.sin_addr.s_addr = INADDR_ANY;		//"host address"
	serv_addr.sin_port = htons(portno);		//set the port, switch to network order

	//bind to address and port
	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) 
		    error("ERROR on binding... maybe the port is in use?");

	
	listen(sockfd,5);	//set to listen, allow a waiting line of 5 connections

	clilen = sizeof(cli_addr);

	//fail
	if (newsockfd < 0) 
		error("ERROR on accept");

	int ii = 0;

	while(1){

		//instructs the socket to wait until a connection is established
		newsockfd = accept(sockfd, 
				  (struct sockaddr *) &cli_addr, 
				  &clilen);

		if(ii < 2){
			sid[ii] = newsockfd;
			pthread_create(&tid[ii], NULL, &handleInput, (void *) &newsockfd);
			ii++;
		}

	}

	//close files when done
	close(newsockfd);
	close(sockfd);

	return 0;

}
