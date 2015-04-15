
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


/*
	//read from socket
	n = read(sockfd,buffer,255);
	if (n < 0) 
		error("ERROR reading from socket");

	//what'd the server think about that?
	printf("%s\n",buffer);
*/

