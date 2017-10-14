//
// Created by alexander on 12.10.17.
//

#ifndef V_SYS_TWMAILER_READMESSAGE_H
#define V_SYS_TWMAILER_READMESSAGE_H


#include "ServerOperation.h"

class ReadMessage : public ServerOperation {
public:
    explicit ReadMessage();

    virtual bool fillMe() ;

    virtual string execute(string message);

private:
    string sender, receiver, subject, final_message, chosen_message;
};


#endif //V_SYS_TWMAILER_READMESSAGE_H
