//
// Created by osboxes on 02.11.17.
//

#ifndef V_SYS_TWMAILER_USER_H
#define V_SYS_TWMAILER_USER_H

#include <cstring>
#include <string>
#include <chrono>
#include <ctime>
#include <map>
#include "User.h"

using namespace std;

class User {

private:
    string username;
    string IPAddress;
    string port;
    bool loggedIn;
    int loginTries;
    void setToDefault();

public:
    User(string, string);
    void Login(string);
    string getUsername();
    bool isLoggedIn();
    string getIPAddressAndPort();
    string getIPAddress();
    void incrementLoginTries();
    void Logout();
    bool isTimedOut();

};


#endif //V_SYS_TWMAILER_USER_H
