#include "Parameter.h"
#include <vector>
#include <string>
#include <iostream>
#include "Message.h"
#include "MessageDecoder.h"


using namespace std;

int main(){

    std::vector<Parameter> params;
    int operation = 1;
    MessageType type = Request;
    Message _message(type);
    Parameter username, password;

    username.setString("Amr");
    password.setString("YALLA123");
    params.push_back(username);
    params.push_back(password);

    MessageDecoder md;

    md.encode(_message, params, operation, type);

    int _operation;
    MessageType _type;
    std::vector<Parameter> _params;
    md.decode(_message, _params, _operation, _type);

    cout << "Parameters are: " << _params[0].getString() << ", " << _params[1].getString() << endl;
    cout << "Type of message is: " << _type << endl;
    cout << "Operation number is: " << _operation << endl;

    return 0;
}