//
// Created by adietrich on 12.10.17.
//

#include "ServerOperation.h"

ServerOperation::ServerOperation(const char * directory) {
    //nothing has to be done
    statusMessage = "Expecting Parameter!\n";
    MESSAGEDIR = directory;
}

string ServerOperation::getStatus() {return statusMessage;}

ServerOperation::ServerOperation(const char *directory, string _username) {
    username = _username;
    statusMessage = "Expecting Parameter!";
    MESSAGEDIR = directory;
}




