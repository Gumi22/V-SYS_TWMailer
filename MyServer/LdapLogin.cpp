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
}


bool LdapLogin::fillMe(string input) {
    switch(parameter_count){
        case 0:
            username = input;
            parameter_count ++;
            statusMessage = "Password: ";
            break;
        case 1:
            password = input.c_str();
            break;
    }
    return true;
}

std::string LdapLogin::login(string username, const char* password) {
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

    if(rc == LDAP_SUCCESS){
        /* free memory used for result */
        ldap_msgfree(result);
        free(attribs[0]);
        free(attribs[1]);

        ldap_unbind(ld);
        return SUCCESS;
    }else{
        /* free memory used for result */
        ldap_msgfree(result);
        free(attribs[0]);
        free(attribs[1]);

        ldap_unbind(ld);
        return FAILURE;
    }
}

string LdapLogin::execute() {
    login(username, password);
}

