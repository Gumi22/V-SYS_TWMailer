//
// Created by osboxes on 04.11.17.
//

#ifndef V_SYS_TWMAILER_TIMEOUTMANAGER_H
#define V_SYS_TWMAILER_TIMEOUTMANAGER_H

#include <map>
#include "User.h"

#define TIMEOUT_TIME_MS 30000
#define IP_TIMEOUT_FILE "IPTimeouts.txt"

class TimeOutManager {
private:
    static TimeOutManager* instance;
    std::mutex my_mutex;
    std::map <string, long> timeOuts; //could be <User*, long> later :D
    TimeOutManager();

public:
    static TimeOutManager* getInstance(){
        if(instance == nullptr){
            instance = new TimeOutManager();
        }
        return instance;
    }

    void timeOut(User *);
    bool isTimedOut(User *);

};


#endif //V_SYS_TWMAILER_TIMEOUTMANAGER_H
