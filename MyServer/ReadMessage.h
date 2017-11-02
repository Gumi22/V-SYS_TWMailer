//
// Created by alexander on 12.10.17.
//

#ifndef V_SYS_TWMAILER_READMESSAGE_H
#define V_SYS_TWMAILER_READMESSAGE_H


#include "ServerOperation.h"

class ReadMessage : public ServerOperation {
public:
    //Constructor
    ReadMessage(const char * directory, string username);

    bool fillMe(string);

    //Searches for message in specified Folder and reads it, returns:
    //  <whole content of specified message> \n
    //or empty string if nothing was found.
    string execute();

    string Get_Username(){ return username;}
    bool Get_IsLoggedIn(){ return is_LoggedIn;}

private:
    //Parses the input parameters
    int MessageNumber;
    int ParameterCount;
};


#endif //V_SYS_TWMAILER_READMESSAGE_H
