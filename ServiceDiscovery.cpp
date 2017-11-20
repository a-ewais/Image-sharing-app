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
    if(token_user[token]==username)
        return true;
    return false;
} //check if in token_user? true: false;

string ServiceDiscovery::signUp(string username, string password){
    srand(time(NULL));
    string token;
    users[username] = password;
    do{
        token = to_string(rand()%10000);
    }while(token_user.count(token)>0);

    token_user[token] = username;
    // online_users[username]=ip;
        // pending_requests needs username or token?
    return token;

    // return NULL;
}

//TO DO
//Recieve IP
string ServiceDiscovery::signIn(string username, string password){
    srand(time(NULL));
    string token;
    if(users[username]==password){
        do{
            token = to_string(rand()%10000);
        }while(token_user.count(token)>0);

        token_user[token] = username;
        // online_users[username]=ip;
        // pending_requests needs username or token?
        return token;
    }

    return NULL;
}  //check if in (users)?generate random token..add to token_user..check if he has pending requests..receive ack...return token: return error.

map<string, string> ServiceDiscovery::getListOfOnlineUsers(string username, string token){
    if(auth(username,token)){
    	return online_users;
    }
    return {NULL};
}

void ServiceDiscovery::stillUp(string username, string token){
    if(auth(username,token)){
        token_time[token]++;
    }
} //auth()? update time_cnt in token_time: ignore

void ServiceDiscovery::down(string username, string token){
    if(auth(username,token)){
        token_time.erase(token);
        online_users.erase(username); 
    }
}

// void ServiceDiscovery::pendingRequest(string username, string token, string to_user, Message m){
//     if(auth(username,token)){

//      }
// } //auth()? add to pending requests: ignore.

Message* ServiceDiscovery::doOperation(Message* _message){
	Message* reply_message;

    vector<Parameter> args;
    vector<Parameter> reply_args;

    int operation = _message->getOperation();
    MessageType = _message->getMessageType();

    MessageDecoder::decode(_message, args);

    switch(operation){
    case 1:
    	//signup
    	string token = this->signUp(args[0].getString(), args[1].getString());
    	Parameter arg1;
    	arg1.setString(token);
    	reply_args.push_back(arg1);
    	break;
    case 2:
    	//signin
    	string token = this->signIn(args[0].getString(), args[1].getString());
    	Parameter arg1;
    	arg1.setString(token);
    	reply_args.push_back(arg1);
    	break;
    case 3:
    	//stillup
    	break;
    case 4:
    	//down
    	break;
    case 5:
    	//pendingrequest
    	break;
    case 6:
    	//GetListofOnlineUsers
    	break;
    case 7:
    	//GetListofImages
    	break;
    case 8:
    	//GetImage
    	break;
    case 9:
    	//UpdateViews
    	break;
    case 10:
    	//Auth
    	break;
    }

	MessageDecoder::encode(*reply_message, reply_args, operation, Reply);
    return reply_message;
}
ServiceDiscovery:: ~ServiceDiscovery(){

}//destructor
