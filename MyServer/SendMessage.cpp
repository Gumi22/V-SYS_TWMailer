//
// Created by alexander on 12.10.17.
//

#include <netinet/in.h>
#include <queue>
#include <string>
#include "SendMessage.h"

SendMessage::SendMessage() : ServerOperation() {

}

string SendMessage::execute() {

    return std::__cxx11::string();
}

bool SendMessage::fillMe(string message) {
    if(message != "."){
        switch(index){
            case 1:
                sender = message;
                return true;
            case 2:
                receiver = message;
                return true;
            case 3:
                subject = message;
                return true;
            default:
                message_final += message;
                return true;
        }
    }
    return false;
}

