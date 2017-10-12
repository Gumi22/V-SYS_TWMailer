//
// Created by adietrich on 12.10.17.
//

#ifndef V_SYS_TWMAILER_SERVEROPERATION_H
#define V_SYS_TWMAILER_SERVEROPERATION_H

#include <string.h>
#include <stdio.h>
#include <string>


using namespace std;

class ServerOperation {

    ServerOperation();


protected:
    string raw_Message;
    virtual string parse(string Message);
public:
    virtual string execute();
};


#endif //V_SYS_TWMAILER_SERVEROPERATION_H
