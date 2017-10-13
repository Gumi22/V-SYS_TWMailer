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

}

bool SendMessage::fillMe(string message) {
    if(message != "."){
        switch(index){
            case 1:
                if(message.length() <= 8){
                    sender = message;
                    index ++;
                    return true;
                }
                return false;
            case 2:
                if(message.length() <= 8){
                    receiver = message;
                    index ++;
                    return true;
                }
                return false;
            case 3:
                if(message.length() <= 80){
                    subject = message;
                    index ++;
                    return true;
                }
                return false;
            default:
                message_final += message;
                return true;
        }
    }
    return false;
}

