#ifndef PARAMETER_H
#define PARAMETER_H

#include <string>
#include <vector>

class Parameter{
private:
    std::string s;
    int i;
    float f;
    std::vector<std::string> vs;
    bool b;
public:
    Parameter();
    ~Parameter();

    bool setString(std::string);
    bool setInt(int);
    bool setFloat(float);
    bool setVectorString(std::vector<std::string>);
    bool setBoolean(bool);

    std::string getString();
    int getInt();
    float getFloat();
    bool getBoolean();
    std::vector<std::string> getVectorString();
};

#endif