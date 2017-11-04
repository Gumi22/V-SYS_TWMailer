//
// Created by osboxes on 02.11.17.
//

#include "TimeOutManager.h"

User::User(string userIP, string userPort) {
    setToDefault();
    IPAddress = userIP;
    port = userPort;
}

void User::Login(string name, string pw) {
    username = name;
    loggedIn = true;
}

string User::getUsername() {
    return username;
}

/**
 * @return true if User was successfully logged in and false if not
 */
bool User::isLoggedIn() {
    return loggedIn;
}

/**
 * resets username and password to default and loggedIn to false
 */
void User::Logout() {
    setToDefault();
}

/**
 * sets User parameters to default
 */
void User::setToDefault() {
    username = "";
    port = "";
    IPAddress = "";
    loggedIn = false;
    loginTries = 0;
}

string User::getIPAddressAndPort() {
    return IPAddress + ":" + port;
}

void User::incrementLoginTries() {
    loginTries++;
    if(loginTries >= 3){
        auto & instance = TimeOutManager::getSingleton();
        instance.timeOut(this);
    }
}

bool User::isTimedOut() {
    ///Call the function updateTimeOut to set the correct value before return
    auto & instance = TimeOutManager::getSingleton();
    return instance.isTimedOut(this);
}

string User::getIPAddress() {
    return IPAddress;
}
