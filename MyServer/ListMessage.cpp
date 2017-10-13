//
// Created by alexander on 12.10.17.
//

#include <regex>
#include "ListMessage.h"
#include <sys/types.h>
#include <dirent.h>
#include <fstream>



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
    string subjects = "";

    //for testing
    //ToDo-> make dir finder better and remove this hardcoded shit
    User = "/home/osboxes/Desktop/test/";

    DIR* userDir = opendir(User.c_str()); //Open User Directory

    struct dirent * userDirEntry; //individual entries in the directory.
    ifstream messagefile;
    string line;



    //while folder isn't empty or didn't reach end keep looking:
    while((userDirEntry = readdir(userDir)) != nullptr){
        //only read regular files
        if(userDirEntry->d_type == DT_REG){
            //ToDo: look if filename ends with .msg or .txt
            string test = User + userDirEntry  ->d_name;
            messagefile.open(User + userDirEntry->d_name);
            if(messagefile.is_open()){
                //reading if open
                getline(messagefile, line); //get First line->should be sender... ignore this part for now, maybe add to list later
                getline(messagefile, line); //get second line->should be subject
                //close file again
                messagefile.close();
                //update results
                subjects.append(line);
                subjects.append("\n");
                count ++;
            }
        }
    }
    result.append(to_string(count));
    result.append("\n");
    result.append(subjects);

    return result;
}