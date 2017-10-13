//
// Created by alexander on 12.10.17.
//

#include <netinet/in.h>
#include <queue>
#include <string>
#include <iostream>
#include "SendMessage.h"

SendMessage::SendMessage() : ServerOperation() {

}

string SendMessage::execute() {
    return "NICE TRY :D";
}

bool SendMessage::fillMe(string message) {
    if(message != ".\n"){
        switch(index){
            case 1:
                if(message.length() <= 9){
                    sender = message;
                    index ++;
                    return true;
                }
                statusMessage = "Invalid Sender - Max 8 Characters!";
                return false;
            case 2:
                if(message.length() <= 9){
                    receiver = message;
                    index ++;
                    return true;
                }
                statusMessage = "Invalid Receiver - Max 8 Characters";
                return false;
            case 3:
                if(message.length() <= 81){
                    subject = message;
                    index ++;
                    return true;
                }
                statusMessage = "Invalid Subject - Max 80 Characters!";
                return false;
            default:
                message_final += message;
                return true;
        }
    }
    if(index == 4 && message == ".\n"){
        statusMessage = "OK";
        std::cout << message_final << std::endl;
        return false;
    }else{
        statusMessage = "Invalid Input - Operation cancelled!";
        return false;
    }
}

