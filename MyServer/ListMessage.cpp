//
// Created by alexander on 12.10.17.
//

#include <regex>
#include "ListMessage.h"
//#include <sys/types.h>
#include <dirent.h>
#include <fstream>



ListMessage::ListMessage() :ServerOperation() {
}

//returns false either way, because first line after LIST command has to be the username
bool ListMessage::fillMe(string line) {

    unsigned long end = line.find('\n');

    if(end == string::npos || end == 0 || end > 8){
        //no \n found, or no user given (\n is first character), or username to long
        return false; //eventually return true if we want to give the sender another chance of not being a total dick and sending bullshit.
    }

    //ignore \n if found:
    User = line.substr(0, end);
    return false;
}



string ListMessage::execute() {
    string result = "", subjects = "";
    int count = 0;

    DIR* userDir = opendir(User.c_str()); //Open User Directory

    struct dirent * userDirEntry; //individual entries in the directory.
    ifstream messageFile;
    string line;

    //if no directory found return 0
    if(userDir == nullptr){
        return "0\n";
    }

    //while directory isn't empty or didn't reach end keep looking:
    while((userDirEntry = readdir(userDir)) != nullptr){
        //only read regular files
        if(userDirEntry->d_type == DT_REG){
            //ToDo: look if filename ends with .msg or .txt
            string test = User + userDirEntry  ->d_name;
            messageFile.open(User + userDirEntry->d_name);
            if(messageFile.is_open()){
                //reading if open
                getline(messageFile, line); //get First line->should be sender... ignore this part for now, maybe add to list later
                getline(messageFile, line); //get second line->should be subject
                //close file again
                messageFile.close();
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