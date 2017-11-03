//
// Created by alexander on 12.10.17.
//


#include "ListMessage.h"


ListMessage::ListMessage(const char * directory, User* user) :ServerOperation(directory, user) {
    statusMessage = EXECUTEPENDING;
}


//returns false either way, because we know the username already
bool ListMessage::fillMe(string line) {
    return false;
}





string ListMessage::execute() {
    string result = "", subjects = "";
    int count = 0;
    string dir = string(MESSAGEDIR) + "/" + user->getUsername();
    DIR* userDir = opendir(dir.c_str()); //Open User Directory

    struct dirent * userDirEntry; //individual entries in the directory.
    ifstream messageFile;
    string line;

    //if no directory found return 0
    if(userDir == nullptr){
        statusMessage = FAILURE;
        closedir(userDir);
        return "No such User \"" + user->getUsername() + "\" found\n";
    }

    //while directory isn't empty or didn't reach end keep looking:
    while((userDirEntry = readdir(userDir)) != nullptr){
        //only read regular files
        if(userDirEntry->d_type == DT_REG){
            //ToDo: look if filename ends with .msg or .txt
            messageFile.open(dir + "/" + userDirEntry->d_name);
            if(messageFile.is_open()){
                //reading if open
                getline(messageFile, line); //get First line->should be sender... ignore this part for now, maybe add to list later
                getline(messageFile, line); //get second line->should be subject
                //close file again
                messageFile.close();
                //update results
                count ++;
                subjects.append(to_string(count) + "...");
                subjects.append(line);
                subjects.append("\n");
            }
        }
    }
    result.append(to_string(count));
    result.append("\n");
    result.append(subjects);

    statusMessage = SUCCESS;

    closedir(userDir);
    return result;
}