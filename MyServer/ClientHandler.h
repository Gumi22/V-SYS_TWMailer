//
// Created by osboxes on 28.10.17.
//

#ifndef V_SYS_TWMAILER_CLIENTHANDLER_H
#define V_SYS_TWMAILER_CLIENTHANDLER_H


#include <thread>
#include <pthread.h>
#include <sys/socket.h>
#include <iostream>
#include <unistd.h>
#include "ListMessage.h"
#include "ReadMessage.h"
#include "SendMessage.h"
#include "DeleteMessage.h"
#include "Quit.h"
#include "User.h"

#define BUF 1024

class ClientHandler {

private:
    const char *MESSAGEDIR;
    const char SUCCESS[4] = "OK\n";
    const char FAILURE[5] = "ERR\n";
    const char EXECUTEPENDING[4] = "EP\n";

public:
    explicit ClientHandler(const char*);

    unsigned long mysend(int, const string*);
    unsigned long myrecv(int, string*);

    void clientLoop(int, string, string, std::map <string, long>&);
    std::thread handleThisClient(int, string, string, std::map <string, long>&);

};


#endif //V_SYS_TWMAILER_CLIENTHANDLER_H
