//
// Created by osboxes on 04.11.17.
//

#ifndef V_SYS_TWMAILER_TIMEOUTMANAGER_H
#define V_SYS_TWMAILER_TIMEOUTMANAGER_H

#include <map>
#include <mutex>
#include "User.h"

#define TIMEOUT_TIME_MS 30000
#define IP_TIMEOUT_FILE "IPTimeouts.txt"

class TimeOutManager {
private:
    std::mutex mapMutex, fileMutex;
    std::map <string, long> timeOuts;

    void updateFile();

public:
    static TimeOutManager& getSingleton(){
        static TimeOutManager t { };
        return t;
    }

    void timeOut(User *);

    bool isTimedOut(User *);

};


#endif //V_SYS_TWMAILER_TIMEOUTMANAGER_H
