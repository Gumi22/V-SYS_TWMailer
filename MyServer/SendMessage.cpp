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
#include <fstream>

SendMessage::SendMessage() : ServerOperation() {
    index = 1;
    statusMessage = "Sender:";
}

string SendMessage::execute() {

    std::chrono::milliseconds ms = std::chrono::duration_cast< std::chrono::milliseconds >(
            std::chrono::system_clock::now().time_since_epoch()
    );

    /// save receiver with \n on the end of the string
    string receiver_print = receiver;
    ///delete \n from the end of the string for routing in directories
    receiver.pop_back();
    ///build correct path into directory of the receiver
    string path_of_Directory = string(MESSAGEDIR) + '/' + receiver;
    ///build a unique path to the mail txt file
    string path_to_file = string(MESSAGEDIR) + '/' + receiver + '/' + std::to_string(ms.count()) + ".txt";

    ///convert all strings into char * for functions opendir, mkdir, fprintf
    const char * path = path_of_Directory.c_str();
    const char * path_to_File = path_to_file.c_str();
    const char * sender_char = sender.c_str();
    const char * subject_char = subject.c_str();
    const char * message_final_char = message_final.c_str();
    //const char * receiver_print_char = receiver_print.c_str(); ->not really needed

    ///check if the directory is available, if not - create new
    DIR* test = opendir(path);
    if(test == nullptr){
        mkdir(path, 0777);
    }
    else{
        closedir(test);
    }


    ///open the file with the unique path
    fstream mail;
    mail.open(path_to_File, fstream::out);
    //FILE *mail = fopen(path_to_File, "w");

    ///write all information into the mail-txt file.
    mail << "Sender: " << sender_char;
    mail << "Subject: " << subject_char;
    mail << message_final_char;
    //fprintf(mail, "Sender: %s", sender_char);
    //fprintf(mail, "%s", receiver_print_char); -> not important because this is the directory the file is in
    //fprintf(mail, "Subject: %s", subject_char);
    //fprintf(mail, "%s", message_final_char);

    ///close the file
    mail.close();
    //fclose(mail);


    statusMessage = SUCCESS;
    return "Message sent!\n";
}

bool SendMessage::fillMe(string message) {
    if(message != ".\n"){
        switch(index){
            case 1:
                if(message.length() <= 9){
                    sender = message;
                    index ++;
                    statusMessage = "Receiver:";
                    return true;
                }
                statusMessage = "Invalid Sender - Max 8 Characters!";
                return false;
            case 2:
                if(message.length() <= 9){
                    receiver = message;
                    index ++;
                    statusMessage = "Subject:";
                    return true;
                }
                statusMessage = "Invalid Receiver - Max 8 Characters";
                return false;
            case 3:
                if(message.length() <= 81){
                    subject = message;
                    index ++;
                    statusMessage = "Message-Line:";
                    return true;
                }
                 statusMessage = "Invalid Subject - Max 80 Characters!";
                return false;
            default:
                message_final += message;
                return true;
        }
    }
    else if(index == 4){
        statusMessage = EXECUTEPENDING;
        return false;
    }else{
        statusMessage = "Invalid Input - Operation cancelled!";
        return false;
    }
}

