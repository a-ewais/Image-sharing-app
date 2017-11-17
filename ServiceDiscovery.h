/*
 * ServiceDiscovery.h
 *
 *  Created on: Nov 17, 2017
 *      Author: ewais
 */

#ifndef SERVICEDISCOVERY_H
#define SERVICEDISCOVERY_H
#include "Server.h"
#include "Message.h"
#include <map>
#include <vector>
#include <string>
#include <set>
using namespace std;

class ServiceDiscovery:protected Server{
	private:
		map<string, pair<string,Message> > pending_requests;  // <user_to, pair<user_from, Message>>
		map<string, string> token_user;		//<token, username> to authenticate users
		map<string, int> token_time;	//<token,time_cnt>...Periodically time_cnt--, delete tokens that reaches zero(thread).
		map<string,string> online_users;		//list of <username,ip> of online users...convert to array before send
		map<string, string> users;		//<username,password>
		bool auth(string username, string token); //check if in token_uset? true: false;
		string sign_in(string username, string password);  //check if in (users)?generate random token..add to token user..check if he have pending requests..receive ack...return token: return error.
		vector<string> getListOfOnlineUsers(string username, string token); //auth()? return online_users as vector: return error
		void stillUp(string username, string token); //auth()? update time_cnt in token_time: ignore
		void down(string username, string token); //auth()? delete token from token_time..update online users
		void pendingRequest(string username, string token, string to_user, Message m); //auth()? add to pending requests: ignore.
		Message* doOperation(Message* _message); //overload...dispatcher


};





#endif /* SERVICEDISCOVERY_H_ */
