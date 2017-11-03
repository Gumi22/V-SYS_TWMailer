//
// Created by osboxes on 28.10.17.
//

#ifndef V_SYS_TWMAILER_MYSOCKET_H
#define V_SYS_TWMAILER_MYSOCKET_H



#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <cstdio>
#include <cstdlib>

class mySocket {

private:
    int mySoc;

public:
    explicit mySocket(int);
    int getSocket();
    ~mySocket();
};


#endif //V_SYS_TWMAILER_MYSOCKET_H
