//
// Created by osboxes on 28.10.17.
//


#include "ClientHandler.h"

ClientHandler::ClientHandler(const char * messagedir) {
    MESSAGEDIR = messagedir;
}

std::thread ClientHandler::handleThisClient(int socket, string clientIP, string clientPort) {
    return std::thread([=] {clientLoop(socket, clientIP, clientPort);} );
}

void ClientHandler::clientLoop(int sock, string clientIP, string clientPort) {
    //Client connected, start command execution loop:

    ServerOperation *command = nullptr; //command the server executes
    bool commandMatched;

    User* user = new User(clientIP, clientPort);

    string bufferStr;
    long size;

    string commandResult;


    string temp = "Welcome to myserver, Please enter your command:\n";
    mysend(sock, &temp);

    do {
        //get command
        size = myrecv(sock, &bufferStr);

        if (size > 0) {
            cout << "Message received: " << bufferStr << endl;
            commandMatched = true; //we expect a real command, so reset it to true every command call

            if(!user->isTimedOut()){//Commands user can Access if not timed out:
                if(user->isLoggedIn()) {//Commands user can Access if not timed out and is logged in:
                    if (strncasecmp(bufferStr.c_str(), "send", 4) == 0) {
                        command = new SendMessage(MESSAGEDIR, user);
                    } else if (strncasecmp(bufferStr.c_str(), "list", 4) == 0) {
                        command = new ListMessage(MESSAGEDIR, user);
                    } else if (strncasecmp(bufferStr.c_str(), "read", 4) == 0) {
                        command = new ReadMessage(MESSAGEDIR, user);
                    } else if (strncasecmp(bufferStr.c_str(), "del", 3) == 0) {
                        command = new DeleteMessage(MESSAGEDIR, user);
                    } else if (strncasecmp(bufferStr.c_str(), "help", 4) == 0 || strncasecmp(bufferStr.c_str(), "?", 1) == 0) {
                        //ToDo: add help-loggedin command here :D
                    }
                    else{
                        commandMatched = false;
                        commandResult = "No matching command found, please get help!";
                    }
                }else{//Commands user can Access if not timed out and not logged in:
                    if (strncasecmp(bufferStr.c_str(), "help", 4) == 0 || strncasecmp(bufferStr.c_str(), "?", 1) == 0) {
                        //ToDo: add help-loggedout command here :D
                    }else if (strncasecmp(bufferStr.c_str(), "login", 5) == 0) {
                        command = new LdapLogin(MESSAGEDIR, user);
                    }else{
                        commandMatched = false;
                        commandResult = "No matching command found, please login first, or get help!";
                    }
                }
            }else{ //Commands user can Access if user is timed out:
                if (strncasecmp(bufferStr.c_str(), "help", 4) == 0 || strncasecmp(bufferStr.c_str(), "?", 1) == 0) {
                    //ToDo: add help-timeout command here :D
                }
                else{
                    commandMatched = false;
                    commandResult = "No matching command found, you are on timeout, get help!";
                }
            }
            //Commands that can be Accessed all the time, commandmatched must be set true
            if (strncasecmp(bufferStr.c_str(), "quit", 4) == 0) {
                command = new Quit(MESSAGEDIR, user);
                commandMatched = true;
            }

            if(!commandMatched){//No commands matched
                char message[] = "placeholder";
                send(sock, FAILURE, strlen(FAILURE)+1, 0);
                recv(sock, message, strlen(message), 0); //always wait for confirmation
                mysend(sock, &commandResult);
            }
            else{ //if command was matched - get all the parameters and execute

                //get the parameters if needed

                if(command->getStatus() != EXECUTEPENDING){

                    do {
                        //always send the client confirmation and the instructions what to do
                        string confirmation = command->getStatus();

                        mysend(sock, &confirmation);
                        myrecv(sock, &bufferStr);

                    } while (command->fillMe(bufferStr)); //Fill Command with parameters till its satisfied previously filled with buffer;
                }

                //were parameters filled correctly? if yes execute, if not return error message
                if (command->getStatus() == EXECUTEPENDING) {
                    commandResult = command->execute(); //finally execute command

                    //send the status to the client:
                    string status = command->getStatus();
                    mysend(sock, &status);
                } else {
                    commandResult = command->getStatus() + "\n"; //save the status as command result

                    //send Error to the client:
                    send(sock, FAILURE, strlen(FAILURE)+1, 0); //send command status
                }
                char message[] = "placeholder";
                recv(sock, message, strlen(message), 0); //always wait for confirmation
                mysend(sock, &commandResult);
            }

        } else if (size == 0) {
            close(sock);
            cout << "Client closed remote socket" << endl;
            break;
        } else {
            delete user;
            close(sock);
            cerr << "recv error" << endl;
            return;// EXIT_FAILURE;
        }
    } while (strncmp(commandResult.c_str(), "quit\n", 5) != 0);
    cout << "User " << user->getUsername() << "quit from address: " << user->getIPAddressAndPort() << "\n";
    delete user;
    close(sock);
}

unsigned long ClientHandler::mysend(int socket, const string * data) {
    unsigned long size = data->length() + 1;
    unsigned long sizeSent = 0;

    auto * buffer = new char[size];
    auto * buf = new char[size];
    strcpy(buffer, data->c_str());

    while(sizeSent < size){
        sizeSent += send(socket, &buffer[sizeSent], (BUF < size - sizeSent) ? BUF : size - sizeSent, 0); //not sure if calculation is right ^^

        //receive confirmation (if not all was sent already) and break if something went wrong with confirmation
        if(sizeSent < size){
            if(recv(socket, buf , sizeof(char*), 0) <= 0){
                delete[] buffer;
                delete[] buf;
                return 0;
            }
        }
    }
    delete[] buffer;
    delete[] buf;
    return sizeSent;
}

unsigned long ClientHandler::myrecv(int socket, string * data) {
    data->clear();
    ssize_t sizeReceived = 0;
    unsigned long totalReceived = 0;
    char buffer[BUF];
    bool endOfStringFound = false;

    do{
        //receive next line
        sizeReceived = recv(socket, buffer, BUF, 0);

        if (sizeReceived > 0) {
            totalReceived += sizeReceived;
            data->append(buffer);

            if(buffer[sizeReceived-1] == '\0'){ //end of message :D
                endOfStringFound = true;
            }
            else{ //continue receiving, send confirmation :D
                send(socket, buffer, BUF, 0);
            }
        } else{
            cout << "Client closed remote socket, or recv failure" << endl;
            return 0;
        }
    }while(!endOfStringFound);
    return totalReceived;
}
