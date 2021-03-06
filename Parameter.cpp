#include "Parameter.h"

Parameter::Parameter(){

}

Parameter::~Parameter(){

}

bool Parameter::setString(std::string _s){
    try{
        s = _s;
        return 1;
    }catch (std::exception& e){
        return 0;
    }
}

bool Parameter::setInt(int _i){
    try{
        i = _i;
        return 1;
    }catch (std::exception& e){
        return 0;
    }
}

bool Parameter::setFloat(float _f){
    try{
        f = _f;
        return 1;
    }catch (std::exception& e){
        return 0;
    }
}

bool Parameter::setVectorString(std::vector<std::string> _vs){
    try{
        vs = _vs;
        return 1;
    }catch (std::exception& e){
        return 0;
    }
}

bool Parameter::setBoolean(bool _b){
    try{
        b = _b;
        return 1;
    }catch (std::exception& e){
        return 0;
    }
}

bool Parameter::setMapSS(std::map<std::string, std::string> _mss){
    try{
        mss = _mss;
        return 1;
    }catch (std::exception& e){
        return 0;
    }
}

std::map<std::string, std::string> Parameter::getMapSS(){
	return mss;
}

bool Parameter::getBoolean(){
    return b;
}

std::string Parameter::getString(){
    return s;
}

int Parameter::getInt(){
    return i;
}

float Parameter::getFloat(){
    return f;
}

std::vector<std::string> Parameter::getVectorString(){
    return vs;
}
