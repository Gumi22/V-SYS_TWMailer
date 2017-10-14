//
// Created by alexander on 12.10.17.
//

#include <netinet/in.h>
#include <queue>
#include <string>
#include <iostream>
#include <sys/stat.h>
#include <dirent.h>
#include "SendMessage.h"
#include <chrono>

SendMessage::SendMessage() : ServerOperation() {

}

string SendMessage::execute() {

    std::chrono::milliseconds ms = std::chrono::duration_cast< std::chrono::milliseconds >(
            std::chrono::system_clock::now().time_since_epoch()
    );

    /// save receiver with \n in string
    string receiver_print = receiver;
    ///delete \n for routing in directories
    receiver.pop_back();
    ///build correct path into directory of the receiver
    string path_of_Directory = "./messages/" + receiver;
    ///build a unique path to the mail txt file
    string path_to_file = "./messages/" + receiver + '/' + std::to_string(ms.count()) + ".txt";

    ///convert all strings into char * for functions
    const char * path = path_of_Directory.c_str();
    const char * path_to_File = path_to_file.c_str();
    const char * sender_char = sender.c_str();
    const char * subject_char = subject.c_str();
    const char * message_final_char = message_final.c_str();
    const char * receiver_print_char = receiver_print.c_str();

    ///check if the directory is available, if not - create new
    if(!opendir(path)){
        mkdir(path, 0777);
    }

    ///open the file with the unique path
    FILE *mail = fopen(path_to_File, "w");

    ///write all information into the mail-txt file.
    fprintf(mail, "%s", sender_char);
    fprintf(mail, "%s", receiver_print_char);
    fprintf(mail, "%s", subject_char);
    fprintf(mail, "%s", message_final_char);

    ///close the file
    fclose(mail);



    return "NICE TRY :D";
}

bool SendMessage::fillMe(string message) {
    if(message != ".\n"){
        switch(index){
            case 1:
                if(message.length() <= 9){
                    sender = message;
                    index ++;
                    return true;
                }
                statusMessage = "Invalid Sender - Max 8 Characters!";
                return false;
            case 2:
                if(message.length() <= 9){
                    receiver = message;
                    index ++;
                    return true;
                }
                statusMessage = "Invalid Receiver - Max 8 Characters";
                return false;
            case 3:
                if(message.length() <= 81){
                    subject = message;
                    index ++;
                    return true;
                }
                 statusMessage = "Invalid Subject - Max 80 Characters!";
                return false;
            default:
                message_final += message;
                return true;
        }
    }
    if(index == 4 && message == ".\n"){
        statusMessage = "OK";
        return false;
    }else{
        statusMessage = "Invalid Input - Operation cancelled!";
        return false;
    }
}

