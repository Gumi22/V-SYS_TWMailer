/* myserver.c */
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <dirent.h>
#include <sys/stat.h>
#include "ListMessage.h"
#include "mySocket.h"
#include "ClientHandler.h"
#include "TimeOutManager.h"

#define BUF 1024

void createWorkingDirectories(char *);

int main(int argc, char **argv) {
    char * MESSAGEDIR;
    int PORT;

    ClientHandler* myClientHandler;

    //parse arguments
    if (argc == 3) {
        PORT = atoi(argv[2]);
        MESSAGEDIR = argv[1];
    } else {
        cout << "No Port or Path to Mailspool directory specified." << endl << "Usage: myserver <path> <port number>" << endl;
        return EXIT_FAILURE;
    }

    //create needed directories
    createWorkingDirectories(MESSAGEDIR);

    auto & instance = TimeOutManager::getSingleton();
    instance.addFileToMap();

    //create socket
    mySocket* mySoc;
    try{
        mySoc = new mySocket(PORT);
    }
    catch(char* param){
        cout << "Exception thrown: " << param << endl;
        return EXIT_FAILURE;
    }
    int new_socket;

    socklen_t addrlen = sizeof(struct sockaddr_in);
    struct sockaddr_in cliaddress = { 0 };

    //start server loop:
    while (true) {
        cout << "Waiting for connection ..." << endl;
        new_socket = accept(mySoc->getSocket(), (struct sockaddr *) &cliaddress, &addrlen);
        if (new_socket > 0) {
            cout << "Client connected from " << inet_ntoa(cliaddress.sin_addr) << ":" << ntohs(cliaddress.sin_port) << endl;

            myClientHandler = new ClientHandler(argv[1]);

            //Client connected, start command execution loop:
            std::thread clientThread = myClientHandler->handleThisClient(new_socket, inet_ntoa(cliaddress.sin_addr), to_string(ntohs(cliaddress.sin_port)));
            clientThread.detach();
        }
    }
    delete myClientHandler;
    delete(mySoc);
    return EXIT_SUCCESS;
}

//checks if directories already exist, if not - creates them
void createWorkingDirectories(char * msgdir) {
    //check messages directory
    if (!opendir(msgdir)) {
        mkdir(msgdir, 0754);
    }
}