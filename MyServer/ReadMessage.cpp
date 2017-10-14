//
// Created by alexander on 12.10.17.
//

#include "ReadMessage.h"

ReadMessage::ReadMessage() : ServerOperation() {

}

bool ReadMessage::fillMe() {
    return false;
}

string ReadMessage::execute(string message) {
    return "nice try :D";
}
