//
// Created by osboxes on 04.11.17.
//

#include <mutex>
#include <fstream>
#include "TimeOutManager.h"


void TimeOutManager::timeOut(User * user) {
    std::lock_guard<mutex> locker(fileMutex); //ROII
    //perform operations here :D
    ///save the actual timestamp in ms
    std::chrono::milliseconds ms = std::chrono::duration_cast< std::chrono::milliseconds >(
            std::chrono::system_clock::now().time_since_epoch()
    );
    ///Open the file which is created from the server at serverstart
    fstream appendFileToWorkWith;

    appendFileToWorkWith.open(IP_TIMEOUT_FILE, std::fstream::in | std::fstream::out | std::fstream::app);

    /// use existing file and write ip address and timestamp into it
    appendFileToWorkWith << user->getIPAddress() << endl << std::to_string(ms.count()) << endl;
    appendFileToWorkWith.close();
    std::lock_guard<mutex> lock(mapMutex);
    timeOuts.insert(pair <string, long>(user->getIPAddress(), ms.count()));

}

bool TimeOutManager::isTimedOut(User * user) {
    std::lock_guard<mutex> locker(mapMutex); //ROII
    //Look if in list
    std::chrono::milliseconds ms = std::chrono::duration_cast< std::chrono::milliseconds >(
            std::chrono::system_clock::now().time_since_epoch()
    );
    ///save the timestamp as a long datatype
    long Timestamp = ms.count();
    ///Search for the correct IP Address in the MAP of TimeOuts
    auto parameter = timeOuts.find(user->getIPAddress());
    ///If you find one Value, check if Timeout is over "60 sec" in this case
    if(parameter != timeOuts.end()){
        if(Timestamp - parameter->second >= TIMEOUT_TIME_MS){
            timeOuts.erase(user->getIPAddress());
            ///Call the function update File, to keep the File up to date
            updateFile();
            return false;
        }else{
            return true;
        }
    }
}

void TimeOutManager::updateFile() {
    std::lock_guard<mutex> locker(fileMutex);
    fstream appendFileToWorkWith;
    ///Open file with parameter trunc - Clear the file
    appendFileToWorkWith.open(IP_TIMEOUT_FILE,  std::fstream::out | std::fstream::trunc);

    ///Write all Key-Value Pairs into the file
    for (auto elem : timeOuts) {
        if(elem.second != 0) {
            appendFileToWorkWith << elem.first << endl << elem.second << endl;
        }
    }
    appendFileToWorkWith.close();
}

void TimeOutManager::addFileToMap() {
    std::lock_guard<mutex> locker(fileMutex);
    fstream IPTimeout;

    IPTimeout.open(IP_TIMEOUT_FILE, std::fstream::in | std::fstream::out | std::fstream::app);


    // If file does not exist, Create new file
    if (!IPTimeout) {
        IPTimeout.open(IP_TIMEOUT_FILE, fstream::in | fstream::out | fstream::trunc);
        IPTimeout.close();

    }else{
        ///Create all needed Variables
        string line, ipAddress;
        char * _timestamp;
        long timestamp;
        ifstream infile;
        ///Open the file
        infile.open (IP_TIMEOUT_FILE);
        ///Read line by line out of the file until the end of file is reached
        while(!infile.eof()) // To get you all the lines.
        {
            ///2 Lines are always 1 key-value pair.. First line is the Key "IP - Adress"
            getline(infile,line); // Get the line with the IP Address
            ipAddress = line;
            ///Second line is the Timestamp when the Timeout was set
            getline(infile,line); // Get the line with with the Timestamp
            ///Create a new Char Array to transform it into a long datatype and finally save the Pair into the Map
            _timestamp = new char[line.length() + 1];
            strcpy(_timestamp, line.c_str());
            timestamp = strtoul(_timestamp, NULL, 0);
            timeOuts.insert( pair <string, long>(ipAddress, timestamp));
            delete[] _timestamp;
        }
        infile.close();
    }
}


