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
    ///save the actual timestamp in ms
    std::chrono::milliseconds ms = std::chrono::duration_cast< std::chrono::milliseconds >(
            std::chrono::system_clock::now().time_since_epoch()
    );
    ///Open the file which is created from the server at serverstart
    fstream appendFileToWorkWith;
    appendFileToWorkWith.open(filename, std::fstream::in | std::fstream::out | std::fstream::app);

    /// use existing file and write ip address and timestamp into it
    appendFileToWorkWith << IPAddress << endl << std::to_string(ms.count()) << endl;
    appendFileToWorkWith.close();
}

bool User::isTimedOut() {
    ///Call the function updateTimeOut to set the correct value before return
    updateTimeOut();
    return timedOut;
}

void User::updateTimeOut() {
    ///Get actual timestamp in ms
    std::chrono::milliseconds ms = std::chrono::duration_cast< std::chrono::milliseconds >(
            std::chrono::system_clock::now().time_since_epoch()
    );
    ///save the timestamp as a long datatype
    long Timestamp = ms.count();
    ///Search for the correct IP Address in the MAP of TimeOuts
    auto parameter = IPTimeouts.find(IPAddress);
    ///If you find one Value, check if Timeout is over "60 sec" in this case
    if(parameter != IPTimeouts.end()){
        if(Timestamp - parameter->second >= 60000){
            IPTimeouts.erase(IPAddress);
            ///Call the function update File, to keep the File up to date
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
    ///Open file with parameter trunc - Clear the file
    appendFileToWorkWith.open(filename,  std::fstream::out | std::fstream::trunc);

    ///Write all Key-Value Pairs into the file
    for (auto elem : IPTimeouts) {
        if(elem.second != 0) {
            appendFileToWorkWith << elem.first << endl << elem.second << endl;
        }
    }
    appendFileToWorkWith.close();
}
