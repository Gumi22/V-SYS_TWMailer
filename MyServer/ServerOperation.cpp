//
// Created by adietrich on 12.10.17.
//

#include "ServerOperation.h"

ServerOperation::ServerOperation(string Message) {
    raw_Message = Message;
}

bool ServerOperation::addParameter(string key, string value) {
    if(key == nullptr || value == nullptr){
        return false;
    }
    parameters["key"] = value;
    if(key == "" || value == ""){
        return false;
    }
    return true;
}

string ServerOperation::getParameter(string key){
    if(parameters.find(key) != map::end()){
        return std::__cxx11::string();
    }
    return "";
}
