//
// Created by alexander on 12.10.17.
//

#ifndef V_SYS_TWMAILER_SENDMESSAGE_H
#define V_SYS_TWMAILER_SENDMESSAGE_H


#include "ServerOperation.h"

class SendMessage : public ServerOperation{
public:
    SendMessage(string rawMessage);

    bool parse(string Message);

    string execute();

private:
    string sender, subject, message, receiver;
};


#endif //V_SYS_TWMAILER_SENDMESSAGE_H
