/* A simple server in the internet domain using TCP
   The port number is passed as an argument
	Initially from
		http://www.linuxhowtos.org/data/6/server.c
	subsequently annotated by cburke
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/poll.h>


//called when a syscall fails
void error(const char *msg)
{
	perror(msg);
	exit(1);
}

int main(int argc, char *argv[])
{
	//file descriptors and a port number
	int sockfd, newsockfd, portno;
	socklen_t clilen;
	char buffer[256];	//we'll be reading from the connection to here

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

	//instructs the socket to wait until a connection is established
	newsockfd = accept(sockfd, 
			  (struct sockaddr *) &cli_addr, 
			  &clilen);

	//fail
	if (newsockfd < 0) 
		error("ERROR on accept");

	char command[5];

	struct pollfd pfd;

	while(1){


		pfd.fd = newsockfd;
		pfd.events = POLLIN;		

		poll(&pfd, 1, -1);

		bzero(buffer,256);				//zero out the buffer
		bzero(command, 4);				//zero out the command buffer

		if(pfd.revents & POLLIN){
			n = read(pfd.fd, buffer, 1024);
			if(n <= 0){
				//instructs the socket to wait until a connection is established
				newsockfd = accept(sockfd, 
						  (struct sockaddr *) &cli_addr, 
						  &clilen);

			}else{
				printf(">%s\n",buffer);	//k
			}
		}

		/*
		n = read(newsockfd,command,4);			//eat a command size chunk

		printf("read\n");

		if (n < 0) error("ERROR reading from socket");	//err

		printf("no error\n");

		command[4] = '\0';				//null terminate the command string

		if( strcmp(command, "quit") == 0){

			printf("quitting...\n");	
			break;
		}else if( strcmp(command, "/say") == 0){
			printf("saying...\n");
			n = read(newsockfd, buffer, 256);
		}


		n = write(newsockfd,"I got your message",18);	//write back

		if (n < 0) error("ERROR writing to socket");	//nope
*/

	}

	//close files when done
	close(newsockfd);
	close(sockfd);

	return 0;

}
