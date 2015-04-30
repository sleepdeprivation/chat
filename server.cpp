/* A simple server in the internet domain using TCP
   The port number is passed as an argument
	Initially from
		http://www.linuxhowtos.org/data/6/server.c
	subsequently annotated and edited heavily by cburke
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/poll.h>
#include <stack>
#include <vector>
#include <thread>
#include <set>
#include <queue>
#include <mutex>
#include "User.h"
#include "Chat.h"

using namespace std;




//vector<thread> tIDs;	//global thread holder

int BUFFER_SIZE = 1024;

//called when a syscall fails
void error(const char *msg)
{
	perror(msg);
	exit(1);
}



void handleInput(int socket){

	int newsockfd = socket;

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

	ChatManager m;

	while(1){

		//instructs the socket to wait until a connection is established
		newsockfd = accept(sockfd, 
				  (struct sockaddr *) &cli_addr, 
				  &clilen);
		//User tempUser = User("username", newsockfd);
		m.newUser("username" + char(ii), newsockfd);
		//thread inputHandler(handleInput, newsockfd);
		//inputHandler.detach();
		//sIDs.insert(newsockfd);
		ii++;

	}

	//close files when done
	close(newsockfd);
	close(sockfd);

	return 0;

}
