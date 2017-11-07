//
// Created by alexander on 07.11.17.
//

#ifndef V_SYS_TWMAILER_HELP_H
#define V_SYS_TWMAILER_HELP_H

#include "ServerOperation.h"
#include "User.h"
#include <iostream>

class help : public ServerOperation {
public:
    help(const char*, User*);
    bool fillMe(std::string) override;
    std::string execute() override;
};


#endif //V_SYS_TWMAILER_HELP_H
