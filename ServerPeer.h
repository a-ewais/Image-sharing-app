#ifndef SERVER_PEER_H
#define SERVER_PEER_H

#include "Server.h"
#include "Client.h"
#include <vector>
#include <string>
#include <opencv2/opencv.hpp>
#include <map>

class ServerPeer: public Server{
    private:
		Client* serviceDiscoveryClient;
		string myPath;
		string myImagesPath;
		string loadedImagesPath;
		map<string, map<string, int>> requesters; //Image name to username to views count
		map<string, map<string, int>> requested;
		map<string, map<string, int>> approvedImages;

        std::vector<std::string> getListofImages(std::string username, std::string token);
        void updateViews(std::string username, std::string token, std::string imageID, int count);
        vector<string> loadFileNames(string path);
        cv::Mat extractImage(string, string, bool);
        void decrementPeerImage(std::string userID, std::string imageID);
        void addRequester(string username, string token, string imageID, int views);
        void addApprovedImage(string username, string token, string imagename, int views, bool approve);

   		Message* doOperation(Message* _message); //overload...dispatcher
    public:
        ServerPeer(char* _listen_hostname, int _listen_port, Client* serviceDiscoveryClient);
        void writePeerImage(string username, string imagename, string image);
        void writeMyImage(string imagePath);
        cv::Mat readPeerImage(string& username, string& imagename);
        vector<string> getListOfMyImages();
        void updateLocalViews(std::string userID, std::string imageID, int count);
        cv::Mat getMyImage(string& imagename);
        std::string getImage(string username, string token, std::string imageID);
        void addRequested(string username, string imageName, int views);
        vector<string> getRequesters(string imageName);
        vector<string> getRequestedImages();
        vector<string> whoCanView(string imageName);
        int viewsCount(string username, string imageName);
        int allowedViews(string username, string imagename);
        void removeRequester(string username, string imageName);

        ~ServerPeer();
};

#endif
