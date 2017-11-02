//
// Created by alexander on 12.10.17.
//

#ifndef V_SYS_TWMAILER_DELETEMESSAGE_H
#define V_SYS_TWMAILER_DELETEMESSAGE_H

#include "ServerOperation.h"


class DeleteMessage : public ServerOperation{
public:
    DeleteMessage(const char * directory, string username);

    bool fillMe(string);

    string execute();

    string Get_Username(){return username;}
    bool Get_IsLoggedIn(){return is_LoggedIn;}

private:
    int parameter_count, chosen_message;

};


#endif //V_SYS_TWMAILER_DELETEMESSAGE_H
