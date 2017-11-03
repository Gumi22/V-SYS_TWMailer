//
// Created by osboxes on 02.11.17.
//

#ifndef V_SYS_TWMAILER_USER_H
#define V_SYS_TWMAILER_USER_H

#include <cstring>
#include <string>

using namespace std;

class User {

private:
    string username;
    string password;
    string IPAddress;
    string port;
    bool loggedIn;
    bool timedOut;
    int loginTries;
    void setToDefault();
    void timeOutThisIP();
    void updateTimeOut();

public:
    User(string, string);
    void Login(string, string);
    void Logout();
    string getUsername();
    string getPassword();
    bool isLoggedIn();
    string getIPAddressAndPort();
    void incrementLoginTries();
    bool isTimedOut();

};


#endif //V_SYS_TWMAILER_USER_H
