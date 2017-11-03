//
// Created by osboxes on 02.11.17.
//

#ifndef V_SYS_TWMAILER_QUIT_H
#define V_SYS_TWMAILER_QUIT_H


#include "ServerOperation.h"

class Quit : public ServerOperation {
private:
    string descision;

public:
    Quit(const char *, User *);
    bool fillMe(std::string) override;
    std::string execute() override;

};


#endif //V_SYS_TWMAILER_QUIT_H
