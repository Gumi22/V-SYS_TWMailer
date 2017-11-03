//
// Created by adietrich on 12.10.17.
//

#include "ServerOperation.h"

ServerOperation::ServerOperation(const char *directory, User* usr) {
    MESSAGEDIR = directory;
    statusMessage = "Expecting Parameter!";
    user = usr;
}

string ServerOperation::getStatus() {return statusMessage;}





