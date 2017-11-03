//
// Created by osboxes on 02.11.17.
//

#include <map>
#include <fstream>
#include <iostream>
#include "User.h"

User::User(string userIP, string userPort, std::map <string, long> IPTimeout) {
    setToDefault();
    IPAddress = userIP;
    port = userPort;
    IPTimeouts = IPTimeout;
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
    timedOut = true;
    char filename[ ] = "IPTimeouts.txt";
    std::chrono::milliseconds ms = std::chrono::duration_cast< std::chrono::milliseconds >(
            std::chrono::system_clock::now().time_since_epoch()
    );
    fstream appendFileToWorkWith;
    appendFileToWorkWith.open(filename, std::fstream::in | std::fstream::out | std::fstream::app);

    /// use existing file and write ip address and timestamp into it
    appendFileToWorkWith << IPAddress << endl << std::to_string(ms.count()) << endl;
    appendFileToWorkWith.close();
}

bool User::isTimedOut() {
    updateTimeOut();
    return timedOut;
}

void User::updateTimeOut() {
    std::chrono::milliseconds ms = std::chrono::duration_cast< std::chrono::milliseconds >(
            std::chrono::system_clock::now().time_since_epoch()
    );
    long Timestamp = ms.count();
    auto parameter = IPTimeouts.find(IPAddress);
    if(parameter != IPTimeouts.end()){
        if(Timestamp - parameter->second >= 60000){
            IPTimeouts.erase(IPAddress);
            updateFile();
            timedOut = false;
        }else{
            timedOut = true;
        }
    }
}

void User::updateFile(){
    char filename[ ] = "IPTimeouts.txt";
    fstream appendFileToWorkWith;
    appendFileToWorkWith.open(filename,  std::fstream::out | std::fstream::trunc);

    for (auto elem : IPTimeouts) {
        if(elem.second != 0) {
            appendFileToWorkWith << elem.first << endl << elem.second << endl;
        }
    }
    appendFileToWorkWith.close();
}
