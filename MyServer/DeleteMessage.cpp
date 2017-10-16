//
// Created by alexander on 12.10.17.
//

#include <dirent.h>
#include "DeleteMessage.h"

DeleteMessage::DeleteMessage() : ServerOperation() {
    parameter_count = 0;
    chosen_message = 0;
    username = "";
    statusMessage = "User:";
}

bool DeleteMessage::fillMe(string message) {
    //fill the needed parameter

    switch(parameter_count){
        case 0:
            if(message.length() <= 9 && message.length() > 0){
                parameter_count ++;
                username = message;
                username.pop_back();
                statusMessage = "Message-number:";
                return true;
            }else {
                statusMessage = "invalid username - Max 8 Characters!";
                return false;
            }
        case 1:
            if(!message.empty() && isdigit(message[0]) && message[0] != '0'){
                chosen_message = stoi(message);
                statusMessage = EXECUTEPENDING;
                return false;
            }else{
                statusMessage = "Invalid Message-Number - Must be a number bigger than 0!";
                return false;
            }
        default:
            statusMessage = "Fatal Error! Parameter is not set! Connection closed";
            return false;
    }
}

string DeleteMessage::execute() {
    string result = "";
    string dir = string(MESSAGEDIR) + "/" + username + "/";

    const char * dir_char = dir.c_str();
    int delete_result = remove(dir_char);

    result = to_string(delete_result);
    statusMessage = SUCCESS;

    return result;
}
