#ifndef MESSAGE_DECODER_H
#define MESSAGE_DECODER_H

#include "Message.h"
#include <vector>
#include <string>


class MessageDecoder{
private:

public:
    MessageDecoder();
    ~MessageDecoder();
    Message* encode(std::vector<std::string>);
    std::vector<std::string> decode(Message* _message);
};

#endif