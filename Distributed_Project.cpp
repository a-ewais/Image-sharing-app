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
	vector<string> ips;
	cout<< print_addresses(AF_INET, ips)<<endl;
	int x;
	cout<< ips[2]<<endl;
	User* u = new User("10.7.57.199", 64000, ips[2] , 64000);
	if(u->signUp("3wais", "1234"))
		cout << "WOHOO\n";

	if(!u->signIn("3waiss", "123"))
		cout << "Not Authorized!\n";
//	cin>>x;
	if(u->signIn("3wais", "1234"))
		cout << "Authorized!\n";
//	cin>>x;
	vector<string> Online = u->viewOnlineUsers();
	for(int i=0;i<Online.size();i++)
		cout << Online[i] << endl;
	vector<string> images = u->viewMyImages();
	cout << images[0] << ", " << images[1] << endl;
	cv::Mat img = u->viewMyImage("Banana.jpg");
	cv::imshow("Image 1", img);
	cv::waitKey(0);
	cout<<"gowa el peer\n";
	images = u->getListOfPeerImages("Amrsaeed");

	for(int i=0;i<images.size();i++)
		cout << images[i] << endl;
	int numOfViews = u->requestPeerImage("Amrsaeed", "Banana.jpg");



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
