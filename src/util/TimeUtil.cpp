#include "TimeUtil.h"
#include <time.h>
#include <sys/time.h>

namespace giapi {

    namespace util {

        TimeUtil::TimeUtil():
            running(false),
            done(false){}
        TimeUtil::~TimeUtil(){}

        void TimeUtil::startTimer(){
            gettimeofday(&startTime, NULL);
            running=true;
            done=false;
        }
        void TimeUtil::stopTimer()noexcept(false){
            if(!running){
                throw new std::logic_error("You must call TimeUtil::startTimer before TimeUtil::stopTimer");
            }
            gettimeofday(&endTime, NULL);
            running=false;
            done=true;
        }
        unsigned long long TimeUtil::getElapsedTime(TimeUnit unit)noexcept(false){
            if(!done){
                throw new std::logic_error("You must call TimeUtil::startTimer and TimeUtil::stopTimer before asking for the elapsed time");
            }
            long seconds  = endTime.tv_sec  - startTime.tv_sec;
            unsigned long long useconds = seconds*1000000 + endTime.tv_usec - startTime.tv_usec;
            switch(unit){
                case USEC:
                    return useconds;
                case MSEC:
                    return useconds/1000;
                case SEC:
                    return seconds;
            }
            throw new std::logic_error("Unknown units");
        }
        unsigned long long TimeUtil::getElapsedUSecs()noexcept(false){
            return getElapsedTime(USEC);
        }
        unsigned long long TimeUtil::getElapsedMSecs()noexcept(false){
            return getElapsedTime(MSEC);
        }
        unsigned long long TimeUtil::getElapsedSecs()noexcept(false){
            return getElapsedTime(SEC);
        }
    }
}
