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

//    ListMessage lm;
//    lm.fillMe("/home/osboxes/Desktop/test/\n");
//
//
//    string test = lm.execute();
//
//    cout << test;

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

    ///check if directory already exists, if not - create one
    if(!opendir("messages")) {
        mkdir("messages", 0777);
    }

//    SendMessage sm;
//    sm.fillMe("if16b063\n");
//    sm.fillMe("if23v054\n");
//    sm.fillMe("das ist der betreff\n");
//    sm.fillMe("Das ist die nachricht\n");
//    sm.fillMe("Was machst du heute nocht?\n");
//    sm.fillMe(".\n");
//
//
//    cout << sm.getStaus() << std::endl;
//    sm.execute();

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

                //parse received command and create the received command
                if (strncasecmp(buffer, "send", 4) == 0) {
                    command = new SendMessage();
                    //TODO -> find a way to put it in a loop
//                    while(command->fillMe(buffer)){
//                        size = recv(new_socket, buffer, BUF - 1, 0);
//                        if(size > 0){
//                            buffer[size] = '\0';
//                            cout << "Message received: " << buffer << endl;
//                            command->fillMe(buffer);
//                        }
                    }
                } else if (strncasecmp(buffer, "list", 4) == 0) {
                    command = new ListMessage();
                } else if (strncasecmp(buffer, "read", 4) == 0) {
                    //ToDo: Add read command when ready
                } else if (strncasecmp(buffer, "del", 3) == 0) {
                    //ToDo: Add del command when ready
                } else if (strncasecmp(buffer, "quit", 4) == 0) {
                    //quit
                } else {
                    //No commands matched
                    char message[] = "No matching command found\n";
                    send(new_socket, message, strlen(message), 0);
                    break;
                }

                //if command was matched - get all the parameters
                do {
                    //always send the client confirmation
                    //ToDo: remove this later, its just for debugging purposes
                    string confirmation = command->getStaus();
                    send(new_socket, confirmation.c_str(), confirmation.length(), 0);

                    //ToDo: remove this next line, its for debugging purposes
                    cout << command->getStaus();

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
                cout << command->getStaus();
                string result = command->execute(); //finally execute command

                //send the result to the client:
                send(new_socket, result.c_str(), result.length(), 0);

                cout << result; //ToDo: remove this line later;

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
