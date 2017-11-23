#include <iostream>

#include "Client.h"
#include "ServerPeer.h"
#include "ServiceDiscovery.h"
#include "UDPServerSocket.h"
#include <cstring>
#include<string>
#include<cstdio>
#include <fstream>
#include <algorithm>
#include <streambuf>
#include <ctime>
using namespace std;

int main(int argc, char *argv[]){

//	if(argv[1][0]=='0'){
//		Client* m  = new Client("localhost", 64000);
////		ServerPeer* n = new ServerPeer("localhost", 6403, m);
//		sockaddr_in x;
//		x.sin_addr.s_addr= 784;
//		x.sin_port = 1234;
//		string token = m->signup("3wais","3waispassword",x);
//		cout<<"Token: "<<token<<endl;
//		map<string,string> listOfUsers = m->requestListOfOnlineUsers(token, "3wais");
//		for(auto it=listOfUsers.begin();it!=listOfUsers.end();it++)
//			cout<<"username: "<<it->first<<" IP and Port: "<<it->second<<endl;
//		bool a1 = m->auth("3wais",token);
//		bool a2 = m->auth("3wais", "1234");
//		bool a3 = m->auth("3wwais", token);
//		cout<<a1<<" "<<a2<<" "<<a3<<endl;
//		delete m;
//	}
//	else{
//		ServiceDiscovery* m = new ServiceDiscovery("localhost", 64000);
//		delete m;
//	}

	return 0;
}
