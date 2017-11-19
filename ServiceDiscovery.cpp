#include "ServiceDiscovery.h"
#include "ServerPeer.h"
#include <iostream>
#include <stdio.h>      /* printf, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
using namespace std;


ServiceDiscovery::ServiceDiscovery(char* _listen_hostname, int _listen_port):Server(_listen_hostname, _listen_port){
    startListen();
}//constructor

bool ServiceDiscovery::auth(string username, string token){
    if(token_user[token]==username)
        return true;
    return false;
} //check if in token_user? true: false;

string ServiceDiscovery::signUp(string username, string password){
    srand(time(NULL));
    string token;
    users[username]=password;
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

vector<string> ServiceDiscovery::getListOfOnlineUsers(string username, string token){
    if(auth(username,token)){
        vector<string> online;
            for (std::map<string,string>::iterator it=online_users.begin(); it!=online_users.end(); ++it)
                online.push_back(it->first);
        return online;
    }
    return {NULL};   
} //auth()? return online_users as vector: return error

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
} //auth()? delete token from token_time..update online users

// void ServiceDiscovery::pendingRequest(string username, string token, string to_user, Message m){
//     if(auth(username,token)){

//      }
// } //auth()? add to pending requests: ignore.

Message* ServiceDiscovery::doOperation(Message* _message){
    
}
ServiceDiscovery:: ~ServiceDiscovery(){

}//destructor