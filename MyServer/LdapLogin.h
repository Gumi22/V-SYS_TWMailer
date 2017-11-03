//
// Created by alexander on 31.10.17.
//

#ifndef V_SYS_TWMAILER_LDAPLOGIN_H
#define V_SYS_TWMAILER_LDAPLOGIN_H

#include <string>
#include <iostream>
#include "ServerOperation.h"
#include <ldap.h>

#define LDAP_HOST "ldap.technikum-wien.at"
#define LDAP_PORT 389
#define SEARCHBASE "dc=technikum-wien,dc=at"
#define SCOPE LDAP_SCOPE_SUBTREE

class LdapLogin: public ServerOperation{
private:
    int parameter_count;
    string username = "";
    char* password;

    bool login(std::string username, char* password);


public:
    ~LdapLogin();
    explicit LdapLogin(const char * directory, User*);

    bool fillMe(std::string) override;
    std::string execute() override;

};


#endif //V_SYS_TWMAILER_LDAPLOGIN_H
