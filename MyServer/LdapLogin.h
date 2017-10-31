//
// Created by alexander on 31.10.17.
//

#ifndef V_SYS_TWMAILER_LDAPLOGIN_H
#define V_SYS_TWMAILER_LDAPLOGIN_H

#include <string>
#include <iostream>
#include "ClientHandler.h"

class LdapLogin  {

public:
    std::string login(char* username, char* password);

};


#endif //V_SYS_TWMAILER_LDAPLOGIN_H
