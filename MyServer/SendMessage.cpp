//
// Created by alexander on 12.10.17.
//

#include <netinet/in.h>
#include <queue>
#include "SendMessage.h"

SendMessage::SendMessage(string rawMessage) : ServerOperation(rawMessage) {

}

bool SendMessage::parse(string Message) {

    return false;
}

string SendMessage::execute() {
    return std::__cxx11::string();
}
