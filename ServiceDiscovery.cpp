#include "ServiceDiscovery.h"
#include "ServerPeer.h"
#include <iostream>
#include <stdio.h>      /* printf, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
using namespace std;


ServiceDiscovery::ServiceDiscovery(char* _listen_hostname, int _listen_port):Server(_listen_hostname, _listen_port){
    startListen();
}

bool ServiceDiscovery::auth(string username, string token){
	cout << "SERVICEDISCOVERY::AUTH!";
    if(token_user[token]==username)
        return true;
    return false;
} //check if in token_user? true: false;

string ServiceDiscovery::signUp(string username, string password, string addr){
	cout << "SERVICEDISCOVERY::SIGNUP!";
    srand(time(NULL));
    string token;
    users[username] = password;
    do{
        token = to_string(rand()%10000);
    }while(token_user.count(token)>0);

    token_user[token] = username;
    online_users[username]=addr;
    // pending_requests needs username or token?
    return token;
}

string ServiceDiscovery::signIn(string username, string password, string addr){
	cout << "SERVICEDISCOVERY::SIGNIN!";
    srand(time(NULL));
    string token;
    if(users[username]==password){
        do{
            token = to_string(rand()%10000);
        }while(token_user.count(token)>0);

        token_user[token] = username;
        online_users[username]=addr;
        // pending_requests needs username or token?
        return token;
    }

    return NULL;
}  //check if in (users)?generate random token..add to token_user..check if he has pending requests..receive ack...return token: return error.

map<string, string> ServiceDiscovery::getListOfOnlineUsers(string username, string token){
	cout << "SERVICEDISCOVERY::getListofOnlineUsers!";
    if(auth(username,token)){
    	return online_users;
    }
    std::map<string, string> mss;
    return mss;
}

void ServiceDiscovery::stillUp(string username, string token, string addr){
	cout << "SERVICEDISCOVERY::stillUp!";
    if(auth(username,token)){
        token_time[token]++;
    }
    online_users[username]=addr;
} //auth()? update time_cnt in token_time: ignore

void ServiceDiscovery::down(string username, string token){
	cout << "SERVICEDISCOVERY::DOWN!";
    if(auth(username,token)){
        token_time.erase(token);
        online_users.erase(username); 
    }
}

//TODO: Handle Pending Requests
// void ServiceDiscovery::pendingRequest(string username, string token, string to_user, Message m){
//     if(auth(username,token)){

//      }
// } //auth()? add to pending requests: ignore.

Message* ServiceDiscovery::doOperation(Message* _message){
	cout << "SERVICEDISCOVERY::doOperation!";
	Message* reply_message = new Message(Reply);

    vector<Parameter> args;
    vector<Parameter> reply_args;

    int operation = _message->getOperation();
    MessageType _type = _message->getMessageType();

    MessageDecoder::decode(_message, args);

    switch(operation){
    case 1:{
    	string token = signUp(args[0].getString(), args[1].getString(), args[2].getString());
    	Parameter arg1;
    	arg1.setString(token);
    	reply_args.push_back(arg1);
    }
    	break;
    case 2:{
    	string token = signIn(args[0].getString(), args[1].getString(), args[2].getString());
    	Parameter arg1;
    	arg1.setString(token);
    	reply_args.push_back(arg1);
    }
    	break;
    case 3:{
    	stillUp(args[0].getString(), args[1].getString(), args[2].getString());
    }
    	break;
    case 4:{
    	down(args[0].getString(), args[1].getString());
    }
    	break;
    case 5:{
    	//pendingrequest
    }
    	break;
    case 6:{
    	//GetListofOnlineUsers
    	map<string, string> _mss = getListOfOnlineUsers(args[0].getString(), args[1].getString());
    	Parameter arg1;
    	arg1.setMapSS(_mss);
    	reply_args.push_back(arg1);
    }
    	break;
    case 10:{
    	//Auth
    	bool _auth = auth(args[0].getString(), args[1].getString());
    	Parameter arg1;
    	arg1.setBoolean(_auth);
    	reply_args.push_back(arg1);
    }
    	break;
    }

	MessageDecoder::encode(*reply_message, reply_args, operation, Reply);
    return reply_message;
}
ServiceDiscovery:: ~ServiceDiscovery(){

}//destructor
