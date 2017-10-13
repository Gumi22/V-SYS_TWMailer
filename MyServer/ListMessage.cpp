//
// Created by alexander on 12.10.17.
//

#include <regex>
#include "ListMessage.h"
#include <sys/types.h>
#include <dirent.h>



ListMessage::ListMessage(const string & Message) :ServerOperation(Message) {
    //ToDo: -> Read needed parameters and save them, Message is eventually obsolete.
    parse();
}

bool ListMessage::parse() {
    //get username from raw string
    unsigned long end = raw_Message.find("\n");

    if(end == string::npos || end == 0 || end > 8){
        //no \n found, or no user given (\n is first character), or username to long
        return false;
    }

    //ignore \n if found:
    User = raw_Message.substr(0, end);

    return true;
}



string ListMessage::execute() {
    string result = "";
    int count = 0;

    DIR* userDir = opendir(User.c_str()); //Open User Directory

    struct dirent * userDirEntry;



    return result;
}