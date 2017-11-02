//
// Created by osboxes on 28.10.17.
//

#include "mySocket.h"

mySocket::mySocket(int port) {
    struct sockaddr_in address;

    mySoc = socket(AF_INET, SOCK_STREAM, 0);

    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons((uint16_t)port);

    if (bind(mySoc, (struct sockaddr *) &address, sizeof(address)) != 0) {
        throw "binding error";
    }
    listen(mySoc, 5);
}

int mySocket::getSocket() {
    return mySoc;
}

mySocket::~mySocket() {
    close(mySoc);
}
