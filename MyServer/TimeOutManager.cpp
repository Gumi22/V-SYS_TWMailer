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

