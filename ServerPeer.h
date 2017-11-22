#ifndef SERVER_PEER_H
#define SERVER_PEER_H

#include "Server.h"
#include "Client.h"
#include <vector>
#include <string>
#include <opencv2/opencv.hpp>

class ServerPeer: public Server{
    private:
		Client* serviceDiscoveryClient;
		string myPath;
		string myImagesPath;
		string loadedImagesPath;

        std::vector<std::string> getListofImages(std::string username, std::string token);
        std::string getImage(std::string username, std::string token, std::string imageID);
        void updateViews(std::string token, std::string userID, int count);
        void revokeViews(std::string token, std::string userID);
        vector<string> loadFileNames(string path);

   		Message* doOperation(Message* _message); //overload...dispatcher
    public:
        ServerPeer(char* _listen_hostname, int _listen_port, Client* serviceDiscoveryClient);
        void writePeerImage(string& username, string& imagename, cv::Mat& image);
        cv::Mat readPeerImage(string& username, string& imagename);
        vector<string> getListOfMyImages();
        cv::Mat getMyImage(string& imagename);
        ~ServerPeer();
};

#endif
