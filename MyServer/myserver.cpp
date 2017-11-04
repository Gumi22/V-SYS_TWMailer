/* myserver.c */
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <dirent.h>
#include <sys/stat.h>
#include "ListMessage.h"
#include "mySocket.h"
#include "ClientHandler.h"
#include <mutex>

#define BUF 1024

void createWorkingDirectories(char *);

int main(int argc, char **argv) {
    char * MESSAGEDIR;
    char filename[ ] = "IPTimeouts.txt";
    int PORT;
    std::map <string, long>  IPTimeouts;

    ClientHandler* myClientHandler;

    //parse arguments
    if (argc == 3) {
        PORT = atoi(argv[2]);
        MESSAGEDIR = argv[1];
    } else {
        cout << "No Port or Path to Mailspool directory specified.\nUsage: myserver <path> <port number>\n";
        return EXIT_FAILURE;
    }

    //create needed directories
    createWorkingDirectories(MESSAGEDIR);

    fstream IPTimeout;

    IPTimeout.open(filename, std::fstream::in | std::fstream::out | std::fstream::app);


    // If file does not exist, Create new file
    if (!IPTimeout) {
        IPTimeout.open(filename, fstream::in | fstream::out | fstream::trunc);
        IPTimeout.close();

    }else{
        ///Create all needed Variables
        string line, ipAddress;
        char * _timestamp;
        long timestamp;
        ifstream infile;
        ///Open the file
        infile.open (filename);
        ///Read line by line out of the file until the end of file is reached
        while(!infile.eof()) // To get you all the lines.
        {
            ///2 Lines are always 1 key-value pair.. First line is the Key "IP - Adress"
            getline(infile,line); // Get the line with the IP Address
            ipAddress = line;
            ///Second line is the Timestamp when the Timeout was set
            getline(infile,line); // Get the line with with the Timestamp
            ///Create a new Char Array to transform it into a long datatype and finally save the Pair into the Map
            _timestamp = new char[line.length() + 1];
            strcpy(_timestamp, line.c_str());
            timestamp = strtoul(_timestamp, NULL, 0);
            IPTimeouts.insert( pair <string, long>(ipAddress, timestamp));
        }
        infile.close();
    }
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
    struct sockaddr_in cliaddress;

    //start server loop:
    while (true) {
        printf("Waiting for connections...\n");
        new_socket = accept(mySoc->getSocket(), (struct sockaddr *) &cliaddress, &addrlen);
        if (new_socket > 0) {
            printf("Client connected from %s:%d...\n", inet_ntoa(cliaddress.sin_addr), ntohs(cliaddress.sin_port));

            myClientHandler = new ClientHandler(argv[1]);

            //Client connected, start command execution loop:
            std::thread clientThread = myClientHandler->handleThisClient(new_socket, inet_ntoa(cliaddress.sin_addr), to_string(ntohs(cliaddress.sin_port)), IPTimeouts);
            clientThread.detach();
        }
    }
    delete(mySoc);
    return EXIT_SUCCESS;
}

//checks if directories already exist, if not - creates them
void createWorkingDirectories(char * msgdir) {
    //check messages directory
    if (!opendir(msgdir)) {
        mkdir(msgdir, 0754);
    }
    //ToDo: check user directory;
}