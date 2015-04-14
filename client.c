/*
	Initial client code gotten from:
		http://www.linuxhowtos.org/data/6/client.c
	subsequently annotated by cburke

*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

//when the syscall fails
void error(const char *msg)
{
	perror(msg);
	exit(0);
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
	char buffer[256];

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
	while(1){
		//prompt user to send a message
		printf(">");

		//kill buffer
		bzero(buffer,256);

		//eat up stdin into buffer
		fgets(buffer,255,stdin);

		//write to socket
		n = write(sockfd,buffer,strlen(buffer));

		if (n < 0) 
			error("ERROR writing to socket");
	}
	//kill the buffer
	bzero(buffer,256);

/*
	//read from socket
	n = read(sockfd,buffer,255);
	if (n < 0) 
		error("ERROR reading from socket");

	//what'd the server think about that?
	printf("%s\n",buffer);
*/

	//always remember to close your files
	close(sockfd);

	return 0;
}
