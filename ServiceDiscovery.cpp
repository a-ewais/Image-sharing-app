#include "ServiceDiscovery.h"
#include "ServerPeer.h"
#include <iostream>
#include <stdio.h>      /* printf, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include "DatabaseLinker.h"
using namespace std;


ServiceDiscovery::ServiceDiscovery(char* _listen_hostname, int _listen_port):Server(_listen_hostname, _listen_port){
	void* arg = (void*) this;
    startListen(arg);
}

bool ServiceDiscovery::auth(string username, string token){
	cout << "SERVICEDISCOVERY::AUTH!";
    if(DL.retreive(username, "TOKEN")==token)
        return true;
    return false;
} //check if in token_user? true: false;

string ServiceDiscovery::signUp(string username, string password, string addr){
	cout << "SERVICEDISCOVERY::SIGNUP!";
    srand(time(NULL));
    string token;
//    users[username] = password;
    do{
        token = to_string(rand()%10000);
    }while(token_user.count(token)>0);

    DL.insert(username, password, addr, true, token);

//    token_user[token] = username;
//    online_users[username]=addr;
    // pending_requests needs username or token?
    return token;
}

string ServiceDiscovery::signIn(string username, string password, string addr){
	cout << "SERVICEDISCOVERY::SIGNIN!";
    srand(time(NULL));
    string token;
    if(DL.retreive(username, "PASSWORD")==password){
        do{
            token = to_string(rand()%10000);
        }while(token_user.count(token)>0);

        DL.update(username, "TOKEN", token);
		DL.update(username, "ONLINE", "true");
		DL.update(username, "IP", addr);
//        token_user[token] = username;
//        online_users[username]=addr;
        // pending_requests needs username or token?
        return token;
    }
    return token = "";
}  //check if in (users)?generate random token..add to token_user..check if he has pending requests..receive ack...return token: return error.

map<string, string> ServiceDiscovery::getListOfOnlineUsers(string username, string token){
	cout << "SERVICEDISCOVERY::getListofOnlineUsers!";
    vector<string> rec1, rec2;

    if(auth(username,token)){
    	return online_users;
    }

    map<string, string> mss;
    rec1 = DL.getOnline("USERNAME");
    rec2 = DL.getOnline("IP");

    for (int i = 0; i < rec1.size(); i++)
    	mss[rec1[i]] = rec2[i];

    return mss;
}

void ServiceDiscovery::stillUp(string username, string token, string addr){
	cout << "SERVICEDISCOVERY::stillUp!";
    if(auth(username,token)){
        token_time[token]++;
    }
    DL.update(username, "IP", addr);
//    online_users[username]=addr;
} //auth()? update time_cnt in token_time: ignore

void ServiceDiscovery::down(string username, string token){
	cout << "SERVICEDISCOVERY::DOWN!";
    if(auth(username,token)){
        token_time.erase(token);
//        online_users.erase(username);
        DL.update(username, "ONLINE", "0");
    }
}

//TODO: Handle Pending Requests
 void ServiceDiscovery::pendingRequest(string username, string token, string to_user, string imageID){
     if(auth(username,token)){

      }
 }

Message* ServiceDiscovery::doOperation(Message* _message){
	cout << "SERVICEDISCOVERY::doOperation!";
	_message->print();
	Message* reply_message = new Message(Reply, _message->getRPCId());

    vector<Parameter> args;
    vector<Parameter> reply_args;

    int operation = _message->getOperation();

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
	reply_message->print();
	cout << "SERVICE DISCOVERY OUT!" << endl;
    return reply_message;
}
ServiceDiscovery:: ~ServiceDiscovery(){

}//destructor
