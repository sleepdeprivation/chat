#include "Chat.h"
#include <iostream>

using namespace std;

	static const int UPDATE_SECONDS = 1;

	ChatRoom::ChatRoom(string n){
		name = n;
	}

	void ChatRoom::addUser(User* u){
		sIDs.insert(u->socketID);
	}
	void ChatRoom::removeUser(User* u){
		sIDs.erase(u->socketID);
	}

	void ChatRoom::sendAll(Message m){

		cout << " SIZE :: :: ::: " << sIDs.size() << endl;

		lock_guard<mutex> lock(writeAllMutex);

		const char * buffer = m.message.c_str();
		for(auto sid : sIDs){
			printf("\nwriting to socket id %d\n", sid);
			write(sid, buffer, strlen(buffer));
		}
	}

	ChatManager::ChatManager(){
		keepPolling = true;
		thread temp (&ChatManager::pollQueueAndEmpty, this);
		temp.detach();
	}

	ChatManager::~ChatManager(){
		lock_guard<mutex> lock(queueMutex);
		keepPolling = false;
	}

	bool ChatManager::newUser(string name, int socket){
		lock_guard<mutex> lock(mapMutex);
		map<string, shared_ptr<User> >::const_iterator it = userNames.find(name);
		map<string, shared_ptr<User> >::const_iterator endIt = userNames.end();	//AAAAAAAA!!!!
		if(it == endIt){
			shared_ptr<User> temp = shared_ptr<User>(new User(name, socket, this));
			userNames[name] = temp;
			attemptAddToRoom(temp.get());
			return true;
		}
		return false;
	}

	/*
		Add the given user to the room they want to be in
		if it doesn't exist create it
	*/
	void ChatManager::attemptAddToRoom(User* u){
		map<string, shared_ptr<ChatRoom> >::const_iterator it = roomNames.find(u->room);
		map<string, shared_ptr<ChatRoom> >::const_iterator endIt = roomNames.end();	//AAAAAAAA!!!!
		if(it == endIt){
			roomNames[u->room] = shared_ptr<ChatRoom>(new ChatRoom(u->room));
		}
		roomNames[u->room].get()->addUser(u);
	}

	void ChatManager::changeRoom(string username, string oldRoom, string newRoom){
		lock_guard<mutex> lock(mapMutex);
		User* u = userNames[username].get();
		ChatRoom* o = roomNames[oldRoom].get();
		o->removeUser(u);
		attemptAddToRoom(u);
		u->room = newRoom;
		
	}

	bool ChatManager::removeUser(User* u){
		userNames.erase(u->name);
		roomNames[u->room].get()->removeUser(u);
	}

	//
	void ChatManager::addMessageToQueue(Message m){
		lock_guard<mutex> lock(queueMutex);	//lock the queue while we put a new message in it
		messageQ.push(m);
	}

	void ChatManager::pollQueueAndEmpty(){
		while(1){
			sleep(UPDATE_SECONDS);			//we don't need to spam this
			cout << "Checking Queue\n";
			lock_guard<mutex> lock(queueMutex);	//lock the queue while we get the messages out
			if(!keepPolling) break;			//if this were the while condition we would need another mutex
			while(messageQ.size() != 0){
				Message m = messageQ.front();
				messageQ.pop();
				ChatRoom* r = roomNames[m.room].get();
				cout << m.sender << "::" << m.message << " (" << m.room << ")\n";
				cout << "Will attempt sendall" << endl;
				r->sendAll(m);
			}
		}
	}






