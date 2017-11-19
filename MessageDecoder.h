#ifndef MESSAGE_DECODER_H
#define MESSAGE_DECODER_H

#include "Message.h"
#include "Parameter.h"
#include <vector>
#include <string>


class MessageDecoder{
private:

public:
    MessageDecoder();
    ~MessageDecoder();
    Message* encode(std::vector<Parameter>);
    std::vector<Parameter> decode(Message* _message);
};

#endif