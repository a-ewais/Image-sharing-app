#include "MessageDecoder.h"
#include <sstream>
#include <vector>
#include <iterator>
#include <iostream>
#include <string>

using namespace std;
MessageDecoder::MessageDecoder(){
}

MessageDecoder::~MessageDecoder(){
}
std::string MessageDecoder::encodeIpPort(sockaddr_in raw){
	std::string s = "";
	s += std::to_string(raw.sin_addr.s_addr)+","+ std::to_string(raw.sin_port);
	cout<<"the encoded IP Port is: "<<s<<endl;
	return s;
}
sockaddr_in MessageDecoder::decodeIpPort(std::string s){
	std::string first = "", sec = "";
	bool flag = true;
	for(int i=0;i<s.size();i++)
	{
		if(s[i]==',')
		{
			flag=false;
			continue;
		}
		if(flag)
			first+=s[i];
		else
			sec+=s[i];
	}
	sockaddr_in res;
	cout<<"the ip looks like this: "<<first<<endl;
	res.sin_addr.s_addr = stoul(first);
	res.sin_port = stoi(sec);
	return res;
}

string MessageDecoder::encodeIpPortPair(string hostName, int port){
	string s = hostName+":"+to_string(port);
	cout<<s<<endl;
	return s;
}
pair<string,int> MessageDecoder::decodeIpPortPair(string s){
	std::string first = "", sec = "";
	bool flag = true;
	for(int i=0;i<s.size();i++)
	{
		if(s[i]==':')
		{
			flag=false;
			continue;
		}
		if(flag)
			first+=s[i];
		else
			sec+=s[i];
	}
	cout<<"IP and Port: " <<first<<" "<<sec<<endl;
	return {first,stoi(sec)};
}
void MessageDecoder::encode(Message& _message, std::vector<Parameter>& params, int _operation, MessageType _type){
    
	std::string messageContent;
    _message.setOperation(_operation);
    _message.setMessageType(_type);
    if(_type == Request){
		switch(_operation){
		case 1:
		case 2:
		case 3:
		case 8:{
			messageContent.append(params[0].getString());
			messageContent.append(";");
			messageContent.append(params[1].getString());
			messageContent.append(";");
			messageContent.append(params[2].getString());
			messageContent.append(";");
		}
		break;
		case 4:
		case 6:
		case 7:
		case 10:{
			messageContent.append(params[0].getString());
			messageContent.append(";");
			messageContent.append(params[1].getString());
			messageContent.append(";");
		}
		break;
		case 9:{
				messageContent.append(params[0].getString());
				messageContent.append(";");
				messageContent.append(params[1].getString());
				messageContent.append(";");
				messageContent.append(std::to_string(params[2].getInt()));
				messageContent.append(";");
			}
		break;

		}
    }
    else if(_type == Reply){
    	switch(_operation){
    	case 1:
    	case 2:{
			messageContent.append(params[0].getString());
			messageContent.append(";");
    	}
    	break;
    	case 6:{
    		std::map<std::string, std::string> _mss = params[0].getMapSS();

    		for (std::map<std::string,std::string>::iterator it=_mss.begin(); it!=_mss.end(); ++it){
    			messageContent.append(it->first);
    			messageContent.append(";");
    			messageContent.append(it->second);
    			messageContent.append(";");
    		}
    	}
    	break;
    	case 7:{
    		std::vector<std::string> _vs = params[0].getVectorString();
    		for(std::vector<std::string>::iterator it = _vs.begin(); it != _vs.end(); ++it){
    			messageContent.append(*it);
    			messageContent.append(";");
    		}
    	}
    	break;
    	case 8:{
    		std::string image = params[0].getString();
    		messageContent.append(image);
    	}
    	break;
    	case 10:{
    		messageContent.append(std::to_string(params[0].getBoolean()));
			messageContent.append(";");
    	}
    	break;
    	}
    }

    _message.setMessage(messageContent.c_str(), messageContent.size());
}

void MessageDecoder::decode(Message* _message, std::vector<Parameter>& params){
    int _operation = _message->getOperation();
    MessageType _type = _message->getMessageType();

    std::vector<std::string> tokens;

    char* pch = strtok (_message->getMessage(),";");
    while (pch != NULL)
    {
    	tokens.push_back(pch);
    	pch = strtok (NULL, ";");
    }
    if(_type == Request){
		switch(_operation){
		case 1:
		case 2:
		case 3:
		case 8:
		{
			Parameter arg1, arg2, arg3;
			arg1.setString(tokens[0]);
			arg2.setString(tokens[1]);
			arg3.setString(tokens[2]);

			params.push_back(arg1);
			params.push_back(arg2);
			params.push_back(arg3);
		}
		break;
		case 4:
		case 6:
		case 7:
		case 10:{
			Parameter arg1, arg2;
			arg1.setString(tokens[0]);
			arg2.setString(tokens[1]);

			params.push_back(arg1);
			params.push_back(arg2);

		}
		break;
		case 9:{
			Parameter arg1, arg2, arg3;
			arg1.setString(tokens[0]);
			arg2.setString(tokens[1]);
			arg3.setInt(std::stoi(tokens[2]));

			params.push_back(arg1);
			params.push_back(arg2);
			params.push_back(arg3);
		}
		break;
		}
    }
    else if (_type == Reply){
    	switch(_operation){
    	case 1:
    	case 2:{
    		Parameter arg1;
//    		cout<<"sa7\n";
    		if(tokens.empty())
    			tokens.push_back("");

    		arg1.setString(tokens[0]);
//    		cout<<"3ada";
    		params.push_back(arg1);
    	}
    	break;
    	case 6:{
    		Parameter arg1;
    		std::map<std::string, std::string> _ms;

    		for(std::vector<std::string>::iterator it = tokens.begin(); it != tokens.end(); ++it){
    			std::string username = *it;
    			std::string IP = *(++it);
    			_ms[username] = IP;
    		}
    		arg1.setMapSS(_ms);
    		params.push_back(arg1);
    	}
    	break;
    	case 7:{
    		Parameter arg1;
    		arg1.setVectorString(tokens);
    		params.push_back(arg1);
    	}
    	break;
    	case 8:{
    		Parameter arg1;
    		std::string image(_message->getMessage());
    		arg1.setString(image);
    		params.push_back(arg1);
    	}
    	break;
    	case 10:{
    		Parameter arg1;
    		arg1.setBoolean(stoi(tokens[0]));
    		params.push_back(arg1);
    	}
    	break;
    	}
    }

}

