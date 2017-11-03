//
// Created by alexander on 12.10.17.
//

#include <dirent.h>
#include <fstream>
#include "DeleteMessage.h"

DeleteMessage::DeleteMessage(const char * directory, User* usr) : ServerOperation(directory, usr) {
    parameter_count = 0;
    chosen_message = 0;
    statusMessage = "Message-number: ";
}

bool DeleteMessage::fillMe(string message) {
    //fill the needed parameter

    switch(parameter_count){
        case 0:
            ///check if second input is valid, if yes save the messagenumber.
            if(!message.empty() && isdigit(message[0]) && message[0] != '0'){
                chosen_message = stoi(message);
                ///change status to "ready for execute"
                statusMessage = EXECUTEPENDING;
                return false;
            }else{
                statusMessage = "Invalid Message-Number - Must be a number bigger than 0!";
                return false;
            }
        default:
            ///shouldn't be called anyway - if there is. return false and break this function.
            statusMessage = "Fatal Error! Parameter is not set! Connection closed";
            return false;
    }

}

string DeleteMessage::execute() {
    int count = 0;
    int delete_result = 1;
    ///create and parse a path to the correct user directory
    string dir = string(MESSAGEDIR) + "/" + user->getUsername();

    ///open the user directory with the input path
    DIR* userDir = opendir(dir.c_str()); //Open User Directory

    struct dirent * userDirEntry; //individual entries in the directory.

    ///if no directory found return 0
    if(userDir == nullptr){
        statusMessage = FAILURE;
        closedir(userDir);
        return "No such User \"" + user->getUsername() + "\" found\n";
    }

    ///while directory isn't empty or didn't reach right file keep looking:
    while ((userDirEntry = readdir(userDir)) != nullptr && count <= chosen_message) {
        ///check if the file is a regular type
        if (userDirEntry->d_type == DT_REG) {
            //count as countable file
            count++;
            if (count == chosen_message) {
                //deleting if reached correct file
                dir += "/" + string(userDirEntry->d_name);
                delete_result = remove(dir.c_str());
                ///check if remove function returned 0 for success or other code for failure
                if(delete_result == 0){
                    statusMessage = SUCCESS;
                }else{
                    statusMessage = FAILURE;
                }
            }
        }
    }

    ///return the correct string to give user the information about success or failure
    if(delete_result == 0){
        return "Message deleted";
    }else{
        return "Delete not successful";
    }
}
