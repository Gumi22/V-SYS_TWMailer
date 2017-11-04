//
// Created by osboxes on 04.11.17.
//

#include <mutex>
#include "TimeOutManager.h"

void TimeOutManager::timeOut(User *) {
    std::lock_guard<mutex> locker(my_mutex); //ROII
    //perform operations here :D

}

TimeOutManager::TimeOutManager() {
    //Open file and read into map/sort out already expired timeouts :D
}

bool TimeOutManager::isTimedOut(User *) {
    std::lock_guard<mutex> locker(my_mutex); //ROII
    //Look if in list

    //Look if still timed out :D
    return false;
}
