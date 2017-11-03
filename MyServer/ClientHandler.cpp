//
// Created by osboxes on 28.10.17.
//


#include "ClientHandler.h"
#include "LdapLogin.h"

ClientHandler::ClientHandler(const char * messagedir) {
    MESSAGEDIR = messagedir;
}

std::thread ClientHandler::handleThisClient(int socket, string clientIP, string clientPort) {
    return std::thread([=] {clientLoop(socket, clientIP, clientPort);} );
}

void ClientHandler::clientLoop(int sock, string clientIP, string clientPort) {
    //Client connected, start command execution loop:

    ServerOperation *command; //command the server executes
    bool commandMatched;

    User* user = new User(clientIP, clientPort);
    int login_count = 0;

    char buffer[BUF];
    long size;

    string commandResult;
    do {
        //get command
        size = recv(sock, buffer, BUF - 1, 0);
        if (size > 0) {
            buffer[size] = '\0';
            printf("Message received: %s", buffer);

            commandMatched = true; //we expect a real command, so reset it to true every command call


            if(!user->isTimedOut()){//Commands user can Access if not timed out:
                if(user->isLoggedIn()) {//Commands user can Access if not timed out and is logged in:
                    if (strncasecmp(buffer, "send", 4) == 0) {
                        command = new SendMessage(MESSAGEDIR, user);
                    } else if (strncasecmp(buffer, "list", 4) == 0) {
                        command = new ListMessage(MESSAGEDIR, user);
                    } else if (strncasecmp(buffer, "read", 4) == 0) {
                        command = new ReadMessage(MESSAGEDIR, user);
                    } else if (strncasecmp(buffer, "del", 3) == 0) {
                        command = new DeleteMessage(MESSAGEDIR, user);
                    } else if (strncasecmp(buffer, "help", 4) == 0) {
                        //ToDo: add help-loggedin command here :D
                    }
                    else{
                        commandMatched = false;
                        commandResult = "No matching command found, please get help!";
                    }
                }else{//Commands user can Access if not timed out and not logged in:
                    if (strncasecmp(buffer, "help", 4) == 0 || strncasecmp(buffer, "?", 1) == 0) {
                        //ToDo: add help-loggedout command here :D
                    }else if (strncasecmp(buffer, "login", 5) == 0) {
                        command = new LdapLogin(MESSAGEDIR, user);
                    }else{
                        commandMatched = false;
                        commandResult = "No matching command found, please login first, or get help!";
                    }
                }
            }else{ //Commands user can Access if user is timed out:
                if (strncasecmp(buffer, "help", 4) == 0) {
                    //ToDo: add help-timeout command here :D
                }
                else{
                    commandMatched = false;
                    commandResult = "No matching command found, you are on timeout, get help!";
                }
            }
            //Commands that can be Accessed all the time, commandmatched must be set true
            if (strncasecmp(buffer, "quit", 4) == 0) {
                command = new Quit(MESSAGEDIR, user);
                commandMatched = true;
            }



            if(!commandMatched){//No commands matched
                char message[] = "placeholder";
                send(sock, FAILURE, strlen(FAILURE), 0);
                recv(sock, message, strlen(message) - 1, 0); //always wait for confirmation
                send(sock, commandResult.c_str(), commandResult.length(), 0);
            }
            else{ //if command was matched - get all the parameters and execute

                //get the parameters if needed
                if(command->getStatus() != EXECUTEPENDING){
                    do {
                        //always send the client confirmation and the instructions what to do
                        string confirmation = command->getStatus();
                        send(sock, confirmation.c_str(), confirmation.length(), 0);

                        //receive next line
                        size = recv(sock, buffer, BUF - 1, 0);
                        if (size > 0) {
                            buffer[size] = '\0';
                            printf("Message received: %s\n", buffer);
                        } else if (size == 0) {
                            printf("Client closed remote socket\n");
                            break;
                        } else {
                            perror("recv error");
                            return;// EXIT_FAILURE;
                        }


                    } while (command->fillMe(buffer)); //Fill Command with parameters till its satisfied
                }

                //were parameters filled correctly? if yes execute, if not return error message
                if (command->getStatus() == EXECUTEPENDING) {
                    commandResult = command->execute(); //finally execute command

                    //send the status to the client:
                    send(sock, command->getStatus().c_str(), command->getStatus().length(),
                         0); //send command status
                } else {
                    commandResult = command->getStatus() + "\n"; //save the status as command result

                    //send Error to the client:
                    send(sock, FAILURE, strlen(FAILURE), 0); //send command status
                }
                char message[] = "placeholder";
                recv(sock, message, strlen(message), 0); //always wait for confirmation
                send(sock, commandResult.c_str(), commandResult.length(), 0); //send command result

                cout << command->getStatus() << "\n"; //ToDo: remove this line later
                cout << commandResult; //ToDo: remove this line later
            }

        } else if (size == 0) {
            close(sock);
            printf("Client closed remote socket\n");
            break;
        } else {
            delete user;
            close(sock);
            perror("recv error");
            return;// EXIT_FAILURE;
        }
    } while (strncmp(buffer, "quit", 4) != 0);
    delete user;
    close(sock);
}