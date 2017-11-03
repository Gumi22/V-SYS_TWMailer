//
// Created by osboxes on 02.11.17.
//

#include "User.h"

User::User(string userIP, string userPort) {
    IPAddress = userIP;
    port = userPort;
    setToDefault();
}

void User::Login(string name, string pw) {
    username = name;
    password = pw;
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

string User::getPassword() {
    return password;
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
    password = "";
    port = "";
    IPAddress = "";
    loggedIn = false;
    timedOut = false;
    loginTries = 0;
}

string User::getIPAddressAndPort() {
    return IPAddress + ":" + port;
}

void User::incrementLoginTries() {
    loginTries++;
    if(loginTries >= 3){
        timeOutThisIP();
    }
}

void User::timeOutThisIP() {
    //ToDo: save ip in timeout blacklist file!!! Add timeout time inline
}

bool User::isTimedOut() {
    return timedOut;
}

void User::updateTimeOut() {
    //ToDo: load timedout from file if exists :D
}
