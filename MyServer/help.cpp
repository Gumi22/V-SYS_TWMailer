//
// Created by alexander on 07.11.17.
//

#include "help.h"

help::help(const char * directory, User* user) : ServerOperation(directory, user) {
    ///just go threw the steps and send the help to the client.
    statusMessage = EXECUTEPENDING;

}

bool help::fillMe(std::string) {
    return false;
}

std::string help::execute() {
    statusMessage = SUCCESS;
    return "If you are logged in, use Commands (send, help, delete or list)! \nIf you are timed-out, wait 30 Minutes!\nIf you aren't logged in - use command \"login\" first!\n";
}
