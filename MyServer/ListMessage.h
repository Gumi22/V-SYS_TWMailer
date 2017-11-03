//
// Created by alexander on 12.10.17.
//

#ifndef V_SYS_TWMAILER_LISTMESSAGE_H
#define V_SYS_TWMAILER_LISTMESSAGE_H


#include "ServerOperation.h"

class ListMessage : public ServerOperation{
public:
    //Constructor
    ListMessage(const char * directory, User*);

    bool fillMe(string) override;

    //Searches for messages in specified Folder and lists them:
    //  <Number of Messages for specified User ?(0 if none were found)> \n
    //  <subject 1> \n
    //  <subject 2> \n
    //  ...
    //  <subject N> \n
    string execute() override;
};


#endif //V_SYS_TWMAILER_LISTMESSAGE_H
