//
// Created by alexander on 12.10.17.
//

#include <regex>
#include "ListMessage.h"


ListMessage::ListMessage(const string & Message) :ServerOperation(Message) {
    //ToDo -> Read needed parameters and save them
    parse();
}

bool ListMessage::parse() {
    //get username from raw string
    return true;
}

string ListMessage::execute() {
    string test = "";
    return test;
}