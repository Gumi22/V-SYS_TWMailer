//
// Created by alexander on 12.10.17.
//

#ifndef V_SYS_TWMAILER_SENDMESSAGE_H
#define V_SYS_TWMAILER_SENDMESSAGE_H


#include <ldap.h>
#include <netinet/in.h>
#include <queue>
#include <string>
#include <iostream>
#include <sys/stat.h>
#include <dirent.h>
#include <uuid/uuid.h>
#include <fstream>
#include "ServerOperation.h"
#include "LdapLogin.h"

class SendMessage : public ServerOperation{

public:
    explicit SendMessage(const char * directory, User*);
    //SendMessage(const char * directory, LdapLogin *User);

    bool fillMe(string Message) override;

    string execute() override;

private:
    string subject, message_final, receiver;
    int index;
};


#endif //V_SYS_TWMAILER_SENDMESSAGE_H
