#ifndef PARAMETER_H
#define PARAMETER_H

#include <string>
#include <vector>
#include <map>

class Parameter{
private:
    std::string s;
    int i = 0;
    float f = 0;
    std::vector<std::string> vs;
    bool b = 0;
    std::map<std::string, std::string> mss;
public:
    Parameter();
    ~Parameter();

    bool setString(std::string);
    bool setInt(int);
    bool setFloat(float);
    bool setVectorString(std::vector<std::string>);
    bool setBoolean(bool);
    bool setMapSS(std::map<std::string, std::string>);

    std::string getString();
    int getInt();
    float getFloat();
    bool getBoolean();
    std::vector<std::string> getVectorString();
    std::map<std::string, std::string> getMapSS();
};

#endif
