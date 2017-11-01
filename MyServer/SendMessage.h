//
// Created by alexander on 12.10.17.
//

#ifndef V_SYS_TWMAILER_SENDMESSAGE_H
#define V_SYS_TWMAILER_SENDMESSAGE_H


#include <ldap.h>
#include "ServerOperation.h"
#include "LdapLogin.h"

class SendMessage : public ServerOperation{

public:
    explicit SendMessage(const char * directory);
    //SendMessage(const char * directory, LdapLogin *User);

    virtual bool fillMe(string Message);

    virtual string execute();

private:
    string sender, subject, message_final, receiver;
    int index;
};


#endif //V_SYS_TWMAILER_SENDMESSAGE_H
