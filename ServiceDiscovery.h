#ifndef SERVICEDISCOVERY_H
#define SERVICEDISCOVERY_H
#include <iostream>
#include "Server.h"
#include "Message.h"
#include <map>
#include <vector>
#include <string>
#include <set>
#include <cstdlib>
#include <ctime>
#include "MessageDecoder.h"
#include "DatabaseLinker.h"
#include "Parameter.h"
using namespace std;

class ServiceDiscovery:protected Server{
	public:
		ServiceDiscovery(char* _listen_hostname, int _listen_port);
		~ServiceDiscovery();
	private:
		map<string, pair<string,Message> > pending_requests;  // <user_to, pair<user_from, Message>>
		map<string, string> token_user;		//<token, username> to authenticate users
		map<string, int> token_time;	//<token,time_cnt>...Periodically time_cnt--, delete tokens that reaches zero(thread).
		map<string,string> online_users;		//list of <username,ip> of online users...convert to array before send
		map<string, string> users;		//<username,password>
		DatabaseLinker DL;

		bool auth(string username, string token); //check if in token_user? true: false;
		string signIn(string username, string password, string addr);  //check if in (users)?generate random token..add to token_user..check if he has pending requests..receive ack...return token: return error.
		string signUp(string username, string password, string addr);
		map<string, string> getListOfOnlineUsers(string username, string token); //auth()? return online_users as vector: return error
		void stillUp(string username, string token, string addr); //auth()? update time_cnt in token_time: ignore
		void down(string username, string token); //auth()? delete token from token_time..update online users
		void pendingRequest(string username, string token, string to_user, string imageID); //auth()? add to pending requests: ignore.
		Message* doOperation(Message* _message); //overload...dispatcher
};
#endif /* SERVICEDISCOVERY_H_ */
