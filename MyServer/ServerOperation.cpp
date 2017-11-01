//
// Created by adietrich on 12.10.17.
//

#include "ServerOperation.h"

ServerOperation::ServerOperation(const char * directory) {
    //nothing has to be done
    statusMessage = "expecting parameter.\n";
    MESSAGEDIR = directory;
}

//ServerOperation::ServerOperation(const char *directory, LdapLogin* User) {
//    statusMessage = "expecting paramenter.\n";
//    MESSAGEDIR = directory;
//    UserAccount = User;
//
//}

string ServerOperation::getStatus() {return statusMessage;}



