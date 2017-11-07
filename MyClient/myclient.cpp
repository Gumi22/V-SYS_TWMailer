/* myclient.c */
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <termios.h>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <dirent.h>
#include <sys/stat.h>
#include <iostream>

#define BUF 1024

unsigned long myrecv(int, std::string *);
unsigned long mysend(int, const std::string *);

unsigned long mysend(int, char * , unsigned long );
unsigned long myrecv(int, char **);

bool connectSocket(int&, char*, u_int16_t);



int main (int argc, char **argv) {
    //ToDo: Put this in Config file
    const char SUCCESS[4] = "OK\n";
    const char FAILURE[5] = "ERR\n";
    u_int16_t PORT;
    char* SERVERADRESS;
    int create_socket;
    std::string bufferStr;

    if( argc < 3 || argc > 3){
        std::cout << "Usage: "<< argv[0] << "<ServerAdresse> <Port-Nummer>" << std::endl;
        exit(EXIT_FAILURE);
    }
    else{
        PORT = (uint16_t)atoi(argv[2]);
        SERVERADRESS = argv[1];
    }

    if(!connectSocket(create_socket, SERVERADRESS, PORT)){
        return EXIT_FAILURE;
    }

    //connected, do something till user wishes to quit:
    do {

        //request and send next command
        getline(std::cin, bufferStr);
        bufferStr.append("\n");
        mysend(create_socket, &bufferStr);

        //receive response
        myrecv(create_socket, &bufferStr);

        //start parameter sending
        while (strcmp (bufferStr.c_str(), SUCCESS) != 0 && strcmp (bufferStr.c_str(), FAILURE) != 0){ //As long as the end wasn't reached (natural or error)
            //print received instructions (status message)
            std::cout << bufferStr << std::endl;
            //get user input and send
            if(strncasecmp(bufferStr.c_str(), "password:", 9) == 0){
                termios tty;
                tcgetattr(STDIN_FILENO, &tty);
                tty.c_lflag &= ~ECHO;
                tcsetattr(STDIN_FILENO, TCSANOW, &tty);

                getline(std::cin, bufferStr);
                bufferStr.append("\n");

                tty.c_lflag |= ECHO;
                tcsetattr(STDIN_FILENO, TCSANOW, &tty);
            }
            else if(strncasecmp(bufferStr.c_str(), "send_me_this_file: \n", 20) == 0){
                std::string fileName = bufferStr.substr(20);
                bufferStr = "";
                //file öffnen und Inhalte kopieren :D
                std::ifstream file(fileName, std::ios::binary);
                std::vector<char> fileBytes;
                fileBytes.assign( (std::istreambuf_iterator<char>(file)), (std::istreambuf_iterator<char>()) );
                char* buf = new char[fileBytes.size()];
                copy(fileBytes.begin(), fileBytes.end(), buf);

                mysend(create_socket, buf, fileBytes.size()); //send file

                file.close();
                std::string nice;
                myrecv(create_socket, &nice); //revćeive confirmation
                delete[] buf;
            }
            else if(strncasecmp(bufferStr.c_str(), "save_this_file: \n", 17) == 0){
                std::string fileAndPath = bufferStr.substr(17);
                std::string fileName = fileAndPath.substr(fileAndPath.find_last_of(" as ")+1); //only need fileName
                bufferStr = "";
                char** data = new char*;
                mysend(create_socket, &bufferStr); //send confirmation
                unsigned long i = myrecv(create_socket, data);
                if(i > 0) //get data if there was any
                {
                    ///open the file
                    std::fstream am;
                    am.open(fileName, std::fstream::out | std::ios::binary);

                    ///write all information into the attachment file.
                    am.write(*data, i);
                    ///close the file
                    am.close();

                }
                bufferStr = "placeholder";
            }
            else{
                getline(std::cin, bufferStr);
                bufferStr.append("\n");
            }
            mysend(create_socket, &bufferStr);//send parameter of command
            myrecv(create_socket, &bufferStr);//receive next parameter of command
        }

        mysend(create_socket, &bufferStr); //send confirmation
        myrecv(create_socket, &bufferStr); //get result of command!!!!

        //quit if command is quit
        if(strcmp (bufferStr.c_str(), "quit\n") == 0){
            break;
        }

        //Print only result
        std::cout << bufferStr.c_str() << std::endl;
        //ask for next command;
        std::cout << "Please enter your command: " << std::endl;

    } while (true);
    close (create_socket);
    return EXIT_SUCCESS;
}



unsigned long mysend(int socket, const std::string * data) {
    unsigned long size = data->length() + 1;
    unsigned long sizeSent = 0;

    auto * buffer = new char[size];
    char buf [BUF];
    strcpy(buffer, data->c_str());

    while(sizeSent < size){
        sizeSent += send(socket, &buffer[sizeSent], (BUF < size - sizeSent) ? BUF : size - sizeSent, 0); //not sure if calculation is right ^^ -> seems ok
        //std::cout << "sent: " << sizeSent << " from " << size << std::endl;
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

unsigned long mysend(int socket,char * data, unsigned long size) {

    unsigned long sizeSent = 0;
    char buf [BUF];
    //Send size of file first:
    send(socket, std::to_string(size).c_str(), BUF, 0);
    //receive confirmation
    recv(socket, buf , BUF, 0);

    while(sizeSent < size){
        sizeSent += send(socket, &data[sizeSent], (BUF < size - sizeSent) ? BUF : size - sizeSent, 0); //calculate needed size and send

        //receive confirmation (if not all was sent already) and break if something went wrong with confirmation
        if(sizeSent < size){
            if(recv(socket, buf , BUF, 0) <= 0){
                return 0;
            }
        }
    }

    return sizeSent;
}

unsigned long myrecv(int socket, std::string * data) {
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
            std::cout << "Server closed remote socket, or recv failure!" << std::endl;
            return 0;
        }
    }while(!endOfStringFound);
    return totalReceived;
}

unsigned long myrecv(int socket, char **data) {
    std::vector<char> byteBuffer;

    ssize_t sizeReceived = 0;
    unsigned long totalReceived = 0;
    char buffer[BUF];

    //receive file size from client:
    recv(socket, buffer, BUF, 0);
    char* lol;
    long size = strtol(buffer, &lol, 10);
    send(socket, "\0", BUF, 0); //send confirmation


    do{
        //receive next line
        sizeReceived = recv(socket, buffer, BUF, 0);
        if (sizeReceived > 0) {
            totalReceived += sizeReceived;
            for(int i = 0; i < sizeReceived; i++){
                byteBuffer.push_back(buffer[i]);
            }
            if((unsigned)byteBuffer.size() < size){
                send(socket, "\0", BUF, 0); //send confirmation
            }
        }
        else if (sizeReceived == 0){ //not received anything
            break;
        }else{
            std::cout << "Client closed remote socket, or recv failure" << std::endl;
            return 0;
        }
    }while((unsigned)byteBuffer.size() < size);
    //copy received data to our array
    if(*data != nullptr){
        delete[] *data;
    }
    *data = new char[totalReceived];
    std::copy(byteBuffer.begin(), byteBuffer.end(), *data);

    return totalReceived;
}


bool connectSocket(int &create_socket, char* serverAddress, u_int16_t port){
    struct sockaddr_in address = { 0 };

    if ((create_socket = socket (AF_INET, SOCK_STREAM, 0)) == -1)
    {
        std::cerr << "Socket error";
        return false;
    }

    memset(&address,0,sizeof(address));
    address.sin_family = AF_INET;
    address.sin_port = htons (port);
    inet_aton (serverAddress, &address.sin_addr);

    if (connect ( create_socket, (struct sockaddr *) &address, sizeof (address)) == 0)
    {
        std::cout << "Connection with server (" << inet_ntoa(address.sin_addr) << ") established!" << std::endl;
        //expect a response;
        std::string buf = "";
        myrecv(create_socket, &buf);
        std::cout << buf;
    }
    else
    {
        std::cerr << "Connect error - no server available" << std::endl;
        return false;
    }
    return true;
}