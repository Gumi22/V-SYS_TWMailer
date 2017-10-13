//
// Created by adietrich on 12.10.17.
//

#include "ServerOperation.h"

ServerOperation::ServerOperation() {
    //nothing has to be done
    statusMessage = "expecting parameter.";
}

string ServerOperation::getStaus() {return statusMessage;}

