#include "User.h"
#include "Chat.h"
#include <string>
#include <thread>

using namespace std;

	static const int BUFFER_SIZE = 1024;

		User::User(string n, int sID, ChatManager* cm){
			name = n;
			socketID = sID;
			chatman = cm;
			pollThread = thread (&User::pollSocket, this, sID); // :0 is this python or what?
			pollThread.detach();
		}

		void User::writeTo(string message){
			lock_guard<mutex> lock(writeMutex);	//only one thread writes at a time
			const char* buffer = message.c_str();
			write(socketID, buffer, strlen(buffer));
			
		}

		string User::readFrom(){
			lock_guard<mutex> lock(readMutex);	//only one thread can look at the buffer
			if(needsRead){
				needsRead = false;
				return buffer;
			}
		}


		void User::pollSocket(int newsockfd){
			int n;
			struct pollfd pfd;
			char buffer[BUFFER_SIZE];	//we'll be reading from the connection to here
			while(1){
				pfd.fd = newsockfd;
				pfd.events = POLLIN;
				poll(&pfd, 1, -1);
				bzero(buffer,BUFFER_SIZE);

				string temp;
				if(pfd.revents & POLLIN){
					n = read(pfd.fd, buffer, BUFFER_SIZE);
					if(n <= 0){
						break;
					}else{
						//buffer[BUFFER_SIZE-1] = '\0';
						temp = string(buffer);
						Message m = Message(name, room, temp);
						chatman->addMessageToQueue(m);
						//thread writeAll (outputAll, temp);
						//pthread_create(&writer, NULL, &outputAll, (void *) &buffer);
						//printf(">%s",buffer);	//k
					}
				}
			}
			close(newsockfd);
			chatman->removeUser(this);
		}

