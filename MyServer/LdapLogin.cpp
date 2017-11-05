//
// Created by alexander on 31.10.17.
//

#include "LdapLogin.h"



LdapLogin::LdapLogin(const char* directory, User* usr) : ServerOperation(directory, usr) {
    statusMessage = "Username: ";
    parameter_count = 0;
}


bool LdapLogin::fillMe(std::string input) {
    switch(parameter_count){
        case 0:
            if(input.length() <= 9 && input.length() > 1) {
                ///Check if there is a newline on the end of the string. If there is one, delete it
                unsigned long end = input.find('\n');
                username = input.substr(0,end);
                parameter_count++;
                statusMessage = "Password: ";
                return true;
            }else{
                statusMessage = "Invalid Username - MIN 1 Character and MAX 8 Characters!!!";
                return false;
            }
        case 1:
            if(input.length() >= 8) {
                parameter_count ++;
                password = new char[input.length() + 1];
                strcpy(password, input.c_str());
                ///Check all Index of the Array for an \n or a \0 -> if found, delete it
                for(unsigned int i = 0; i < input.length(); i++){
                    if(password[i] == '\0' || password[i] == '\n') {
                        memmove(&password[i], &password[i + 1], strlen(password) - i);
                    }
                }
                statusMessage = EXECUTEPENDING;
                return false;
            }else{
                statusMessage = "Password need to be MIN 8 characters!!";
                return false;
            }
        default:
            ///just because the IDE was crying that there is no default :D never reach this default case
            statusMessage = "You failed as fuck! Go home and cry! :D";
            return false;
    }
}

bool LdapLogin::login(std::string username, char* password) {
    LDAP *ld;            /* LDAP resource handle */
    LDAPMessage *result, *e;    /* LDAP result handle */;
    std::string filter = "(uid=" + username + ")";

    const char * FILTER = filter.c_str();
    char * dn;

    int rc = 0;

    char *attribs[3];        /* attribute array for search */

    attribs[0] = strdup("uid");        /* return uid and cn of entries */
    attribs[1] = strdup("cn");
    attribs[2] = NULL;        /* array must be NULL terminated */


    /* setup LDAP connection */
    if ((ld = ldap_init(LDAP_HOST, LDAP_PORT)) == NULL) {
        statusMessage = FAILURE;
        free(attribs[0]);
        free(attribs[1]);
        return false;
    }

    /* anonymous bind */
    rc = ldap_simple_bind_s(ld, NULL, NULL);

    if (rc != LDAP_SUCCESS) {
        statusMessage = FAILURE;
        free(attribs[0]);
        free(attribs[1]);
        return false;
    }

    /* perform ldap search */
    rc = ldap_search_s(ld, SEARCHBASE, SCOPE, FILTER, attribs, 0, &result);

    if (rc != LDAP_SUCCESS) {
        statusMessage = FAILURE;
        free(attribs[0]);
        free(attribs[1]);
        return false;
    }

    cout << ldap_count_entries(ld, result)  << endl;
    if(ldap_count_entries(ld,result) < 1){
        statusMessage = FAILURE;
        free(attribs[0]);
        free(attribs[1]);
        return false;
    }

    e = ldap_first_entry(ld, result);
    dn = ldap_get_dn(ld, e);

    rc = ldap_simple_bind_s(ld, dn, password);

    if(rc != LDAP_SUCCESS) {
        statusMessage = FAILURE;
        free(attribs[0]);
        free(attribs[1]);
        return false;
    }
    /* free memory used for result */
    ldap_msgfree(result);
    free(attribs[0]);
    free(attribs[1]);

    ldap_unbind(ld);
    statusMessage = SUCCESS;
    return true;
}

string LdapLogin::execute() {
    bool check = login(username, password);
    check = true; //ToDo: remove this later;
    if (check) {
        user->Login(username);
        return "Login successful for user: " + username + " from address: " + user->getIPAddressAndPort() + "\n";
    } else {
        user->incrementLoginTries();
        return "Login failed for user: " + username + " from address: " + user->getIPAddressAndPort() + "\n";
    }

}

LdapLogin::~LdapLogin() {
    delete[] password;
}
