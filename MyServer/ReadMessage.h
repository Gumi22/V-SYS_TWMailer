//
// Created by alexander on 12.10.17.
//

#ifndef V_SYS_TWMAILER_READMESSAGE_H
#define V_SYS_TWMAILER_READMESSAGE_H


#include "ServerOperation.h"
#include <dirent.h>
#include <fstream>

class ReadMessage : public ServerOperation {
public:
    //Constructor
    ReadMessage(const char * directory, User*);

    bool fillMe(string) override;

    //Searches for message in specified Folder and reads it, returns:
    //  <whole content of specified message> \n
    //or empty string if nothing was found.
    string execute() override;

private:
    //Parses the input parameters
    int MessageNumber;
    int ParameterCount;
    string peek();

};


#endif //V_SYS_TWMAILER_READMESSAGE_H
