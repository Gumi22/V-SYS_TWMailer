//
// Created by osboxes on 02.11.17.
//

#ifndef V_SYS_TWMAILER_USER_H
#define V_SYS_TWMAILER_USER_H

#include <cstring>
#include <string>
#include <chrono>
#include <ctime>

using namespace std;

class User {

private:
    std::map <string, long> IPTimeouts;
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
    void updateFile();

public:
    User(string, string, std::map <string, long>);
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
