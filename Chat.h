#ifndef CHAT_MAN_H
#define CHAT_MAN_H


class Chat;
class Message;
class ChatManager;

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
#include <map>
#include "User.h"
#include <memory>

/*
	Holds messages that will be enstd::queued and handled by the manager
*/
struct Message{

	std::string sender;
	std::string room;
	std::string message;

	Message(std::string u, std::string r, std::string msg){
		sender = u;
		room = r;
		message = msg;
	}	
};

class ChatRoom{

	std::set<int> sIDs;	//socket holder (user sockets)
	//std::queue<std::string> messageQ;	//holds messages that need to be outputted to the sIDs
	std::string name;

	std::mutex writeAllMutex;

	public:

	ChatRoom(std::string n);
	void addUser(User* u);
	void removeUser(User* u);
	void sendAll(Message m);


};


/*
	Manages chatrooms, users, and a std::queue of messages
*/
class ChatManager{

	//i may soon regret using shared ptrs
	std::map<std::string, std::shared_ptr<ChatRoom> > 	roomNames;	//associates chat room names with objects
	std::map<std::string, std::shared_ptr<User> >		userNames;	//associates usernames with objects
	std::queue<Message>					messageQ;	//a std::queue of messages to be sent

	std::mutex mapMutex;	//protects both maps
	std::mutex queueMutex;	//protects the queue

	bool keepPolling = true;

	public:

	//start the poll thread
	ChatManager();

	//stop the poll thread
	~ChatManager();

	//put a new user into the userNames table
	bool newUser(std::string name, int socket);

	//try to put the user in the room it prefers and create it in the map if necessary
	void attemptAddToRoom(User* u);

	//get rid of the user and take it out of the chatroom it's in
	bool removeUser(User* u);

	//move the user with the given username from oldroom to newroom
	//assumes the old room exists
	void changeRoom(std::string username, std::string oldRoom, std::string newRoom);

	//put the given message into the message queue
	//queue is protected by the queuemutex
	void addMessageToQueue(Message m);

	//poll thread runs this function (INFINITE LOOP)
	//stop condition is a flag "keepPolling" protected by the queuemutex
	void pollQueueAndEmpty();

};

#endif

