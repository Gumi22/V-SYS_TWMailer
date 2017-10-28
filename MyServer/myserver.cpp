/* myserver.c */
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <dirent.h>
#include <sys/stat.h>
#include "ListMessage.h"
#include "ReadMessage.h"
#include "SendMessage.h"
#include "DeleteMessage.h"
#include "mySocket.h"
#include "ClientHandler.h"

#define BUF 1024

void createWorkingDirectories(const char *, string);

int createSocket();

int main(int argc, char **argv) {
    //ToDo: Put this in Config file
    const char *MESSAGEDIR;
    const char USERDIR[6] = "users";
    const char SUCCESS[4] = "OK\n";
    const char FAILURE[5] = "ERR\n";
    const char EXECUTEPENDING[4] = "EP\n";

    int PORT;
    char buffer[BUF];
    long size;

    ClientHandler* myClientHandler;

    ServerOperation *command; //command the server executes

    //parse arguments
    if (argc == 3) {
        PORT = atoi(argv[1]);
        MESSAGEDIR = argv[2];
        myClientHandler = new ClientHandler(argv[2]);
    } else {
        cout << "No Port or Path to Mailpool directory specified.\nUsage: myserver <port number> <path>\n";
        return EXIT_FAILURE;
    }

    //create needed directories
    createWorkingDirectories(MESSAGEDIR, USERDIR);

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
            strcpy(buffer, "Welcome to myserver, Please enter your command:\n");
            send(new_socket, buffer, strlen(buffer), 0);
        }
        //Client connected, start command execution loop:

        std::thread clientThread = myClientHandler->handleThisClient(new_socket);
        clientThread.detach();

/*
        bool commandMatched;
        string commandResult;
        do {
            //get command
            size = recv(new_socket, buffer, BUF - 1, 0);
            if (size > 0) {
                buffer[size] = '\0';
                printf("Message received: %s", buffer);

                commandMatched = true; //we expect a real command, so reset it to true every command call

                //create the received command depending on string that was sent
                if (strncasecmp(buffer, "send", 4) == 0) {
                    command = new SendMessage(MESSAGEDIR);
                } else if (strncasecmp(buffer, "list", 4) == 0) {
                    command = new ListMessage(MESSAGEDIR);
                } else if (strncasecmp(buffer, "read", 4) == 0) {
                    command = new ReadMessage(MESSAGEDIR);
                } else if (strncasecmp(buffer, "del", 3) == 0) {
                    command = new DeleteMessage(MESSAGEDIR);
                } else if (strncasecmp(buffer, "quit", 4) == 0) {
                    //quit
                    //ToDo: make quit a real command :D
                    char message[] = "placeholder";
                    send(new_socket, SUCCESS, strlen(SUCCESS), 0);
                    recv(new_socket, message, strlen(message) - 1, 0); //always wait for confirmation
                    commandResult = "quit\n";
                    send(new_socket, commandResult.c_str(), commandResult.length(), 0);
                    break; //quit command loop -> new client can connect now
                } else {
                    //No commands matched
                    char message[] = "placeholder";
                    send(new_socket, FAILURE, strlen(FAILURE), 0);
                    recv(new_socket, message, strlen(message) - 1, 0); //always wait for confirmation
                    commandMatched = false;
                    commandResult = "No matching command found.\n";
                    send(new_socket, commandResult.c_str(), commandResult.length(), 0);
                }

                //if command was matched - get all the parameters and execute
                if (commandMatched) {
                    //get the parameters
                    do {
                        //always send the client confirmation and the instructions what to do
                        string confirmation = command->getStatus();
                        send(new_socket, confirmation.c_str(), confirmation.length(), 0);

                        //receive next line
                        size = recv(new_socket, buffer, BUF - 1, 0);
                        if (size > 0) {
                            buffer[size] = '\0';
                            printf("Message received: %s\n", buffer);
                        } else if (size == 0) {
                            printf("Client closed remote socket\n");
                            break;
                        } else {
                            perror("recv error");
                            return EXIT_FAILURE;
                        }


                    } while (command->fillMe(buffer)); //Fill Command with parameters till its satisfied

                    //were parameters filled correctly? if yes execute, if not return error message
                    if (command->getStatus() == EXECUTEPENDING) {
                        commandResult = command->execute(); //finally execute command

                        //send the status to the client:
                        send(new_socket, command->getStatus().c_str(), command->getStatus().length(),
                             0); //send command status
                    } else {
                        commandResult = command->getStatus() + "\n"; //save the status as command result

                        //send Error to the client:
                        send(new_socket, FAILURE, strlen(FAILURE), 0); //send command status
                    }
                    char message[] = "placeholder";
                    recv(new_socket, message, strlen(message), 0); //always wait for confirmation
                    send(new_socket, commandResult.c_str(), commandResult.length(), 0); //send command result

                    cout << command->getStatus() << "\n"; //ToDo: remove this line later
                    cout << commandResult; //ToDo: remove this line later
                }

            } else if (size == 0) {
                printf("Client closed remote socket\n");
                break;
            } else {
                perror("recv error");
                return EXIT_FAILURE;
            }
        } while (strncmp(buffer, "quit", 4) != 0);
        close(new_socket);
        */
    }
    delete(mySoc);
    return EXIT_SUCCESS;
}

//checks if directories already exist, if not - creates them
void createWorkingDirectories(const char * msgdir, string usrdir) {
    //check messages directory
    if (!opendir(msgdir)) {
        mkdir(msgdir, 0777);
    }
    //ToDo: check user directory;
}