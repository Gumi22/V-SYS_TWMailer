//
// Created by alexander on 12.10.17.
//

#ifndef V_SYS_TWMAILER_DELETEMESSAGE_H
#define V_SYS_TWMAILER_DELETEMESSAGE_H

#include "ServerOperation.h"


class DeleteMessage : public ServerOperation{
public:
    DeleteMessage(const char * directory, User*);

    bool fillMe(string) override;

    string execute() override;


private:
    int parameter_count, chosen_message;

};


#endif //V_SYS_TWMAILER_DELETEMESSAGE_H
