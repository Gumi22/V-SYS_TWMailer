//
// Created by alexander on 31.10.17.
//

#ifndef V_SYS_TWMAILER_LDAPLOGIN_H
#define V_SYS_TWMAILER_LDAPLOGIN_H

#include <string>
#include <iostream>
#include "ServerOperation.h"

class LdapLogin: public ServerOperation{
private:
    bool is_LoggedIn = false;
    std::string username = "";
    char * password;
    int parameter_count;

public:
    ~LdapLogin();
    LdapLogin(const char* directory);
    std::string login(std::string username, char* password);

    bool fillMe(std::string);
    std::string execute();
    std::string Get_Username();
    bool Get_IsLoggedIn();

};


#endif //V_SYS_TWMAILER_LDAPLOGIN_H
