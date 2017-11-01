//
// Created by osboxes on 28.10.17.
//


#include "ClientHandler.h"
#include "LdapLogin.h"

ClientHandler::ClientHandler(const char * messagedir) {
    MESSAGEDIR = messagedir;
}

std::thread ClientHandler::handleThisClient(int socket) {
    return std::thread([=] {clientLoop(socket);} );
}

void ClientHandler::clientLoop(int sock) {

    //Client connected, start command execution loop:

//    LdapLogin *UserLogin;
    ServerOperation *command; //command the server executes
    bool commandMatched;

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

            //create the received command depending on string that was sent
            if (strncasecmp(buffer, "send", 4) == 0) {
                command = new SendMessage(MESSAGEDIR);
//                if(UserLogin == nullptr){
//                    //TODO: Fehlermeldung ausgeben weil, nicht angemeldet. Bei erfolgreichen test, auf andere commands ausweiten
//                }else{
//                    command = new SendMessage(MESSAGEDIR, UserLogin);
//                }
            } else if (strncasecmp(buffer, "list", 4) == 0) {
                command = new ListMessage(MESSAGEDIR);
            } else if (strncasecmp(buffer, "read", 4) == 0) {
                command = new ReadMessage(MESSAGEDIR);
            } else if (strncasecmp(buffer, "del", 3) == 0) {
                command = new DeleteMessage(MESSAGEDIR);
            } else if (strncasecmp(buffer, "login", 5) == 0){
                command = new LdapLogin(MESSAGEDIR);
            }else if (strncasecmp(buffer, "quit", 4) == 0) {
                //quit
                //ToDo: make quit a real command :D
                char message[] = "placeholder";
                send(sock, SUCCESS, strlen(SUCCESS), 0);
                recv(sock, message, strlen(message) - 1, 0); //always wait for confirmation
                commandResult = "quit\n";
                send(sock, commandResult.c_str(), commandResult.length(), 0);
                break; //quit command loop -> new client can connect now
            } else {
                //No commands matched
                char message[] = "placeholder";
                send(sock, FAILURE, strlen(FAILURE), 0);
                recv(sock, message, strlen(message) - 1, 0); //always wait for confirmation
                commandMatched = false;
                commandResult = "No matching command found.\n";
                send(sock, commandResult.c_str(), commandResult.length(), 0);
            }

            //if command was matched - get all the parameters and execute
            if (commandMatched) {
                //get the parameters
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
            printf("Client closed remote socket\n");
            break;
        } else {
            perror("recv error");
            return;// EXIT_FAILURE;
        }
    } while (strncmp(buffer, "quit", 4) != 0);
    close(sock);
}

