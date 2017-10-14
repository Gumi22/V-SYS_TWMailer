//
// Created by adietrich on 12.10.17.
//

#include "ServerOperation.h"

ServerOperation::ServerOperation() {
    //nothing has to be done
    statusMessage = "expecting parameter.\n";
}

string ServerOperation::getStatus() {return statusMessage;}

