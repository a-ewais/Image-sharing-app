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
#include "User.h"
#include <stropts.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <linux/netdevice.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <vector>
using namespace std;
int print_addresses(const int domain, vector<string>& ips)
{
int s;
struct ifconf ifconf;
struct ifreq ifr[50];
int ifs;
int i;

s = socket(domain, SOCK_STREAM, 0);
	if (s < 0) {
	  perror("socket");
	  return 0;
	}

	ifconf.ifc_buf = (char *) ifr;
	ifconf.ifc_len = sizeof ifr;

	if (ioctl(s, SIOCGIFCONF, &ifconf) == -1) {
	  perror("ioctl");
	  return 0;
	}

	ifs = ifconf.ifc_len / sizeof(ifr[0]);
	printf("interfaces = %d:\n", ifs);
	for (i = 0; i < ifs; i++) {
		 char ip[INET_ADDRSTRLEN];
		 struct sockaddr_in *s_in = (struct sockaddr_in *) &ifr[i].ifr_addr;
		if (!inet_ntop(domain, &s_in->sin_addr, ip, sizeof(ip))) {
			perror("inet_ntop");
			return 0;
		}
		ips.push_back(string(ip));
		printf("%s - %s\n", ifr[i].ifr_name, ip);
  }

  close(s);

  return 1;
}
int main(int argc, char *argv[]){
	ServiceDiscovery* m = new ServiceDiscovery("localhost", 64000);
	delete m;


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
