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

#define BUF 1024
#define PORT 6543

int main(void) {

    ServerOperation *command; //command the server executes

    int create_socket, new_socket;
    socklen_t addrlen;
    char buffer[BUF];
    long size;
    struct sockaddr_in address, cliaddress;

    create_socket = socket(AF_INET, SOCK_STREAM, 0);

    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    bool commandMatched = false;
    string commandResult = "";

    ///check if directory already exists, if not - create one
    if(!opendir("messages")) {
        mkdir("messages", 0777);
    }

    if (bind(create_socket, (struct sockaddr *) &address, sizeof(address)) != 0) {
        perror("bind error");
        return EXIT_FAILURE;
    }
    listen(create_socket, 5);

    addrlen = sizeof(struct sockaddr_in);

    while (true) {
        printf("Waiting for connections...\n");
        new_socket = accept(create_socket, (struct sockaddr *) &cliaddress, &addrlen);
        if (new_socket > 0) {
            printf("Client connected from %s:%d...\n", inet_ntoa(cliaddress.sin_addr), ntohs(cliaddress.sin_port));
            strcpy(buffer, "Welcome to myserver, Please enter your command:\n");
            send(new_socket, buffer, strlen(buffer), 0);
        }
        do {
            size = recv(new_socket, buffer, BUF - 1, 0);
            if (size > 0) {
                buffer[size] = '\0';
                printf("Message received: %s\n", buffer);

                commandMatched = true;
                //parse received command and create the received command
                if (strncasecmp(buffer, "send", 4) == 0) {
                    command = new SendMessage();
                } else if (strncasecmp(buffer, "list", 4) == 0) {
                    command = new ListMessage();
                } else if (strncasecmp(buffer, "read", 4) == 0) {
                    command = new ReadMessage();
                } else if (strncasecmp(buffer, "del", 3) == 0) {
                    //ToDo: Add del command when ready and delete following code:
                    char message[] = "ERR\n";
                    send(new_socket, message, strlen(message), 0);
                    recv(new_socket, message, strlen(message) - 1, 0); //always wait for confirmation
                    commandMatched = false;
                    commandResult = "No matching command found.\n";
                    send(new_socket, commandResult.c_str(), commandResult.length(), 0);
                } else if (strncasecmp(buffer, "quit", 4) == 0) {
                    //quit
                    //ToDo: make quit a real command :D
                    char message[] = "OK\n";
                    send(new_socket, message, strlen(message), 0);
                    recv(new_socket, message, strlen(message) - 1, 0); //always wait for confirmation
                    //commandMatched = false;
                    commandResult = "quit\n";
                    send(new_socket, commandResult.c_str(), commandResult.length(), 0);
                    break;
                } else {
                    //No commands matched
                    char message[] = "ERR\n";
                    send(new_socket, message, strlen(message), 0);
                    recv(new_socket, message, strlen(message) - 1, 0); //always wait for confirmation
                    commandMatched = false;
                    commandResult = "No matching command found.\n";
                    send(new_socket, commandResult.c_str(), commandResult.length(), 0);
                }

                //if command was matched - get all the parameters
                if(commandMatched) {
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
                    commandResult = command->execute(); //finally execute command

                    //send the result to the client:
                    send(new_socket, command->getStatus().c_str(), command->getStatus().length(), 0); //send command status
                    char message[] = "OK\n";
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
    }
    close(create_socket);
    return EXIT_SUCCESS;
}
