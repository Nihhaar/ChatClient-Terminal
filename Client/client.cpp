#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string>
#include <cstring>
#include <unistd.h>

#include "utils.h"

using namespace std;

int main(){

	/* Server Info */
	char SERVERIP[] = "192.168.0.58";
	unsigned short SERVERPORT = 5000;

	/* Connect to Server */
	int sock;
	while((sock = connectToServer(SERVERIP, SERVERPORT)) == -1){
		continue;
	}

	/* Login into server */
	string id;
	id = login(sock);

	while(id == "0"){
	    cout<<"\033[1;31mWrong Username/Password\033[0m\n\n";
		id = login(sock);
	}
	cout<<"\033[1;32mUser Authenticated\033[0m\n\n";


	string cmd;

	while(true){

	/* List of Commands */
	cout<<"\033[1;35mType the command [type 'help' to see list of commands]:\033[0m ";
	getline(cin,cmd);
    if(cmd=="help") {cout<<"You can try the following commands:"<<endl;
					 cout<<"\033[1;32monline:\033[0m  gives list of online friends"<<endl;
					 cout<<"\033[1;32mfriends:\033[0m gives list of your friends"<<endl;
					 cout<<"\033[1;32mlast_seen \033[3m[\033[1;35musername\033[1;32m]:\033[0m Gives last seen time \033[3m[\033[1;35mof your friend\033[1;0m]\033[0m"<<endl;
					 cout<<"\033[1;32mchat \033[1;35musername:\033[0m Start chat \033[1;35m your friend\033[0m"<<endl;
					 cout<<"\033[1;32mexit:\033[0m Quits the chat client\n\n";
				     continue;
					}

	if(cmd=="online") {
		sendDataToServer("ONLINE " + id, sock);

		unsigned int length = 0;
  		char* buffer = 0;
  		readXBytes(sock, sizeof(length), (void*)(&length));
  		length = ntohl(length);
  		buffer = new char[length];
  		readXBytes(sock, length, (void*)buffer);
  		buffer[length] = '\0';
  		cout<<buffer<<endl;

  		delete[] buffer;

		continue;
	}

	if(cmd=="friends") {

		sendDataToServer("FRIENDS " + id, sock);

		unsigned int length = 0;
  		char* buffer = 0;
  		readXBytes(sock, sizeof(length), (void*)(&length));
  		length = ntohl(length);
  		buffer = new char[length];
  		readXBytes(sock, length, (void*)buffer);
  		buffer[length] = '\0';
  		cout<<buffer<<endl;

  		delete[] buffer;

		continue;
	}

	if(cmd=="exit") {
		sendDataToServer("EXIT " + id, sock);
		close(sock);
		return 0;
	}
	
	/* BEGIN 2 WORD COMMANDS */
	vector<string> v = split(cmd,' ');

	if(v[0]=="last_seen") {
		if(v.size()==1){
			sendDataToServer("LAST_SEEN_ALL " + id, sock);

			unsigned int length = 0;
  			char* buffer = 0;
  			readXBytes(sock, sizeof(length), (void*)(&length));
  			length = ntohl(length);
  			buffer = new char[length];
  			readXBytes(sock, length, (void*)buffer);
  			buffer[length] = '\0';
  			cout<<buffer<<endl;
	  		delete[] buffer;

		}
		else if(v.size()==2){
			sendDataToServer("LAST_SEEN " + v[1] + " " + id, sock);

			unsigned int length = 0;
  			char* buffer = 0;
  			readXBytes(sock, sizeof(length), (void*)(&length));
  			length = ntohl(length);
  			buffer = new char[length];
  			readXBytes(sock, length, (void*)buffer);
  			buffer[length] = '\0';
  			cout<<buffer<<endl;
  			delete[] buffer;
	
		}
		else{
			cout<<"Use the command properly!"<<endl<<endl;
		}
		
		continue;
	}

	if(v[0]=="chat") {
		if(v.size()==2){
			sendDataToServer("CHAT " + v[1] + " " + id, sock);
		}
		else{
			cout<<"Use the command properly!"<<endl<<endl;
		}
		
		continue;
	}

	cout<<"Use the command properly! Type 'help to see the list of commands"<<endl<<endl;
	}

	return 0;
}