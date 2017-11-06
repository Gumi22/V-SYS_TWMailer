//
// Created by adietrich on 12.10.17.
//

#include <iostream>
#include "ServerOperation.h"

ServerOperation::ServerOperation(const char *directory, User* usr) {
    MESSAGEDIR = directory;
    statusMessage = "Expecting Parameter!";
    user = usr;
    dataLength = 0;
}

string ServerOperation::getStatus() {return statusMessage;}

void ServerOperation::setData(char ** dat, unsigned long length) {
    data = dat;
    dataLength = length;
}

char **ServerOperation::getData() {
    return data;
}

unsigned long ServerOperation::getDataLength() {
    return dataLength;
}





