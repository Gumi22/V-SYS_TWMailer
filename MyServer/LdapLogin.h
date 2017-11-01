//
// Created by alexander on 31.10.17.
//

#ifndef V_SYS_TWMAILER_LDAPLOGIN_H
#define V_SYS_TWMAILER_LDAPLOGIN_H

#include <string>
#include <iostream>
#include "ServerOperation.h"

class LdapLogin : public ServerOperation{
private:
    bool is_LoggedIn = false;
    string username;
    const char * password;
    int parameter_count, login_count;

public:

    LdapLogin(const char* directory);
    std::string login(std::string username, const char* password);

    virtual bool fillMe(string);
    virtual string execute();
    string Get_Username();
    bool Get_IsLoggedIn();

};


#endif //V_SYS_TWMAILER_LDAPLOGIN_H
