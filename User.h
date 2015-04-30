#ifndef USER_H
#define USER_H

class User;

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
#include "Chat.h"


/*
	Constructor will spin up a thread that continuously checks the socket for new messages
*/
class User{

	std::mutex readMutex;
	std::mutex writeMutex;

	bool needsRead;
	std::string buffer;
	ChatManager* chatman;

	std::thread pollThread;
	void pollSocket(int newsockfd);

	public:

	std::string room = 	"default";
	std::string		name;		//uniquely identifies the user
	int			socketID;	//the socket we write to

	User(std::string n, int sID, ChatManager* cm);
	//~User();
	void writeTo(std::string message);
	std::string readFrom();

};

#endif

