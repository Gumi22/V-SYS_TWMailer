//
// Created by alexander on 12.10.17.
//

#include <dirent.h>
#include <fstream>
#include <iostream>
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
    int count = 0;
    int delete_result = 1;
    string dir = string(MESSAGEDIR) + "/" + username;

    DIR* userDir = opendir(dir.c_str()); //Open User Directory

    struct dirent * userDirEntry; //individual entries in the directory.

    //if no directory found return 0
    if(userDir == nullptr){
        statusMessage = FAILURE;
        closedir(userDir);
        return "No such User \"" + username + "\" found\n";
    }

    //while directory isn't empty or didn't reach right file keep looking:
    while ((userDirEntry = readdir(userDir)) != nullptr && count <= chosen_message) {
        //only read regular files
        if (userDirEntry->d_type == DT_REG) {
            //count as countable file
            count++;
            if (count == chosen_message) {
                //deleting if reached right file
                dir += "/" + string(userDirEntry->d_name);
                delete_result = remove(dir.c_str());
                if(delete_result == 0){
                    statusMessage = SUCCESS;
                }else{
                    statusMessage = FAILURE;
                }
            }
        }
    }

    if(delete_result == 0){
        return "Message deleted";
    }else{
        return "Delete not successful";
    }
}
