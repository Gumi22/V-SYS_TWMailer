//
// Created by osboxes on 02.11.17.
//

#include "Quit.h"

Quit::Quit(const char *directory, User *user) : ServerOperation(directory, user){
    statusMessage = "Are you sure [J/N]: ";
}

bool Quit::fillMe(std::string answer) {
    descision = answer;
    statusMessage = EXECUTEPENDING;
    return false;
}

std::string Quit::execute() {
    if(strncasecmp(descision.c_str(), "j", 1) == 0){
        statusMessage = SUCCESS;
        return "quit\n";
    } else if(strncasecmp(descision.c_str(), "n", 1) == 0){
        statusMessage = SUCCESS;
        return "quit aborted\n";
    }else{
        statusMessage = FAILURE;
        return "wrong input, aborted\n";
    }
}
