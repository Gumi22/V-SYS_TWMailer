//
// Created by alexander on 31.10.17.
//

#include "LdapLogin.h"
#include <ldap.h>

#define LDAP_HOST "ldap.technikum-wien.at"
#define LDAP_PORT 389
#define SEARCHBASE "dc=technikum-wien,dc=at"
#define SCOPE LDAP_SCOPE_SUBTREE
#define BIND_USER NULL        /* anonymous bind with user and pw NULL */
#define BIND_PW NULL


LdapLogin::LdapLogin(const char* directory) : ServerOperation(directory) {
    statusMessage = "Username: ";
    parameter_count = 0;
    login_count = 0;
}


bool LdapLogin::fillMe(std::string input) {
    switch(parameter_count){
        case 0:
            if(input.length() <= 9 && input.length() > 1) {
                username = input;
                parameter_count++;
                statusMessage = "Password: ";
                return true;
            }else{
                statusMessage = "Invalid Sender - MIN 1 Character and MAX 8 Characters!!!";
                return false;
            }
        case 1:
            if(input.length() > 4) {
                parameter_count ++;
                password = input.c_str();
                statusMessage = "Login in Progress!";
                return true;
            }else{
                statusMessage = "Password need to be MIN 4 characters!!";
                return false;
            }
    }
    if(parameter_count == 2){
        statusMessage = EXECUTEPENDING;
        return false;
    }else{
        statusMessage = "Invalid Input! LoginProcess canceled!";
        return false;
    }
}

std::string LdapLogin::login(std::string username, const char* password) {
    LDAP *ld;            /* LDAP resource handle */
    LDAPMessage *result, *e;    /* LDAP result handle */
    BerElement *ber;        /* array of attributes */
    char *attribute;
    char **vals;
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
        perror("ldap_init failed");
        return FAILURE;
    }

    printf("connected to LDAP server %s on port %d\n", LDAP_HOST, LDAP_PORT);

    /* anonymous bind */
    rc = ldap_simple_bind_s(ld, BIND_USER, BIND_PW);

    if (rc != LDAP_SUCCESS) {
        fprintf(stderr, "LDAP error: %s\n", ldap_err2string(rc));
        return FAILURE;
    } else {
        printf("bind successful\n");
    }

    /* perform ldap search */
    rc = ldap_search_s(ld, SEARCHBASE, SCOPE, FILTER, attribs, 0, &result);

    if (rc != LDAP_SUCCESS) {
        fprintf(stderr, "LDAP search error: %s\n", ldap_err2string(rc));
        return FAILURE;
    }

    printf("Total results: %d\n", ldap_count_entries(ld, result));

    e = ldap_first_entry(ld, result);
    dn = ldap_get_dn(ld, e);

    rc = ldap_simple_bind(ld, dn, password);

    /* free memory used for result */
    ldap_msgfree(result);
    free(attribs[0]);
    free(attribs[1]);

    ldap_unbind(ld);

    if(rc == LDAP_SUCCESS){
        return SUCCESS;
    }else{
        return FAILURE;
    }
}

string LdapLogin::execute() {
    login_count ++;
    if(login_count < 4){
        if(login(username, password) == SUCCESS){
            statusMessage = "Login succesfull!";
            is_LoggedIn = true;
        }else{
            statusMessage = "Login Failed!";
            return FAILURE;
        }
    }else{
        statusMessage = "Wrong Username or Password! IP is banned! Try it later again!";
        return FAILURE;
    }
}

string LdapLogin::Get_Username() {
    return username;
}

bool LdapLogin::Get_IsLoggedIn() {
    return is_LoggedIn;
}

