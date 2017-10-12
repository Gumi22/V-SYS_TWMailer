//
// Created by adietrich on 12.10.17.
//

#ifndef V_SYS_TWMAILER_SERVEROPERATION_H
#define V_SYS_TWMAILER_SERVEROPERATION_H

#include <string.h>
#include <stdio.h>
#include <string>
#include <map>


using namespace std;


//Class ServerOperation is the base Class for all the Operations this server can execute.
class ServerOperation {

protected:
    string raw_Message;

    //parses the raw Message into the needed parameters, returns false if not successful, true if successful
    virtual bool parse() = 0;

public:
    //Constructor needs raw message (Part after the command name) es parameter
    explicit ServerOperation(const string &);

    //Executes the operation returns the server response as a string
    virtual string execute() = 0;
};


#endif //V_SYS_TWMAILER_SERVEROPERATION_H
