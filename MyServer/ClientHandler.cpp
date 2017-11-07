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
    vector<char> _data;

    long size;

    string commandResult;


    string temp = "Welcome to Server Synergy of Doom, Please enter your command:\n";
    mysend(sock, &temp);

    do {
        //get command
        size = myrecv(sock, &bufferStr);

        if (size > 0) {
            cout << "Command <" << bufferStr.substr(0,bufferStr.find_last_of('\n')) << "> received from " << (user->isLoggedIn()? "user <" + user->getUsername() + ">": "anonymous user") << endl;
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
                        command = new help(MESSAGEDIR, user);
                    }
                    else{
                        commandMatched = false;
                        commandResult = "No matching command found, please get help!";
                    }
                }else{//Commands user can Access if not timed out and not logged in:
                    if (strncasecmp(bufferStr.c_str(), "help", 4) == 0 || strncasecmp(bufferStr.c_str(), "?", 1) == 0) {
                        command = new help(MESSAGEDIR, user);
                    }else if (strncasecmp(bufferStr.c_str(), "login", 5) == 0) {
                        command = new LdapLogin(MESSAGEDIR, user);
                    }


                    else{
                        commandMatched = false;
                        commandResult = "No matching command found, please login first, or get help!";
                    }
                }
            }else{ //Commands user can Access if user is timed out:
                if (strncasecmp(bufferStr.c_str(), "help", 4) == 0 || strncasecmp(bufferStr.c_str(), "?", 1) == 0) {
                    command = new help(MESSAGEDIR, user);
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
                string status = FAILURE;
                mysend(sock, &status);
                myrecv(sock, &status); //always wait for confirmation
                mysend(sock, &commandResult);
            }
            else{ //if command was matched - get all the parameters and execute

                //get the parameters if needed

                if(command->getStatus() != EXECUTEPENDING){

                    do {
                        //always send the client confirmation and the instructions what to do
                        string confirmation = command->getStatus();

                        mysend(sock, &confirmation);
                        if(strncasecmp(confirmation.c_str(), "send_me_this_file: \n", 20) == 0){ //If I sent filerequest use different read

                            unsigned long sizeOfData = myrecv(sock, _data);
                            command->setData(_data, sizeOfData);
                        }
                        else if(strncasecmp(confirmation.c_str(), "save_this_file: \n", 17) == 0){ //If I sent filesaverequest use different send
                            myrecv(sock, &bufferStr); //get confirmation
                            sendFile(sock, &confirmation);//send the file in binary
                            myrecv(sock, &bufferStr); //get confirmation
                        }
                        else{
                            myrecv(sock, &bufferStr); //Use other receive
                        }

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
                    string status = FAILURE;
                    mysend(sock, &status);
                }
                myrecv(sock, &bufferStr); //always wait for confirmation
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
    char buf [BUF];
    strcpy(buffer, data->c_str());

    while(sizeSent < size){
        sizeSent += send(socket, &buffer[sizeSent], (BUF < size - sizeSent) ? BUF : size - sizeSent, 0); // calculation of lenght and send
        //receive confirmation (if not all was sent already) and break if something went wrong with confirmation
        if(sizeSent < size){
            if(recv(socket, buf , BUF, 0) <= 0){
                delete[] buffer;
                return 0;
            }
        }
    }
    delete[] buffer;
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
            *data += buffer;
            if(buffer[sizeReceived-1] == '\0'){ //end of message :D
                endOfStringFound = true;
            }
            else{ //continue receiving, send confirmation :D
                send(socket, "\0", BUF, 0); //vorher buffer gesendet
            }
        } else{
            cout << "Client closed remote socket, or recv failure" << endl;
            return 0;
        }
    }while(!endOfStringFound);
    return totalReceived;
}

unsigned long ClientHandler::myrecv(int socket, vector<char>& data) {
    data.clear();

    ssize_t sizeReceived = 0;
    unsigned long totalReceived = 0;
    char buffer[BUF];

    //receive file size from client:
    recv(socket, buffer, BUF, 0);
    char* lol;
    long size = strtol(buffer, &lol, 10);
    send(socket, "\0", BUF, 0); //send confirmation


    while((unsigned)data.size() < size){
        //receive next line
        sizeReceived = recv(socket, buffer, BUF, 0);
        if (sizeReceived > 0) {
            totalReceived += sizeReceived;
            for(int i = 0; i < sizeReceived; i++){
                data.push_back(buffer[i]);
            }
            if((unsigned)data.size() < size){
                send(socket, "\0", BUF, 0); //send confirmation
            }
        }
        else if (sizeReceived == 0){ //not received anything
            break;
        }else{
            cout << "Client closed remote socket, or recv failure" << endl;
            return 0;
        }
    }
    //copy received data to our array

    return totalReceived;
}

unsigned long ClientHandler::mysend(int socket, vector<char>& data) {

    unsigned long sizeSent = 0;
    char buf [BUF];
    //Send size of file first:
    send(socket, std::to_string(data.size()).c_str(), std::to_string(data.size()).length(), 0);
    //receive confirmation
    recv(socket, buf , BUF, 0);

    while(sizeSent < data.size()){
        sizeSent += send(socket, &data[sizeSent], (BUF < data.size() - sizeSent) ? BUF : data.size() - sizeSent, 0); //not sure if calculation is right ^^ -> seems ok

        //receive confirmation (if not all was sent already) and break if something went wrong with confirmation
        if(sizeSent < data.size()){
            if(recv(socket, buf , BUF, 0) <= 0){
                return 0;
            }
        }
    }

    return sizeSent;
}

bool ClientHandler::sendFile(int sock, string * path) {
    std::string PathAndFilename = path->substr(17); //get path + as + filename
    string delim = " as ";//delimiter between readl filename and user filename

    //file öffnen und Inhalte kopieren :D
    std::ifstream file(PathAndFilename.substr(0, PathAndFilename.find(delim.c_str())), std::ios::binary);

    std::vector<char> fileBytes;
    fileBytes.assign( (std::istreambuf_iterator<char>(file)), (std::istreambuf_iterator<char>()) );

    if(fileBytes.empty()){
        string temp = "corrupted";
        mysend(sock, &temp);
        return false;
    }
    mysend(sock, fileBytes);

    file.close();

    return true;
}