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
using namespace std;

int main(int argc, char *argv[]){
	int x;
	User* u = new User("10.7.57.199", 64000);
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
