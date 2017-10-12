//
// Created by alexander on 12.10.17.
//

#ifndef V_SYS_TWMAILER_LISTMESSAGE_H
#define V_SYS_TWMAILER_LISTMESSAGE_H


#include "ServerOperation.h"

class ListMessage : public ServerOperation{
public:
    ListMessage(const string &);
    virtual string execute();

private:
    virtual bool parse();
    string User;
};


#endif //V_SYS_TWMAILER_LISTMESSAGE_H
