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

class ServerOperation {

protected:
    string raw_Message;
    map<string, string> parameters;

    //parses the raw Message into the needed parameters, returns false if not successful, true if successful
    virtual bool parse(string) = 0;
    //adds a parameter to the parameters Map
    virtual bool addParameter(string, string);
    //reads a parameter from the map
    virtual string getParameter(string);

public:
    explicit ServerOperation(string);
    virtual string execute() = 0;
};


#endif //V_SYS_TWMAILER_SERVEROPERATION_H
