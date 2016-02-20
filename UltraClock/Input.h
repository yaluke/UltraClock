#ifndef _INPUT_H_
#define _INPUT_H_

#include "ClockData.h"
#include <chrono>

class Input
{
public:
    Input(ClockData& a_data, void (ClockData::*a_Action1)(), void (ClockData::*a_Action2)(), void (ClockData::*a_Action3)(), void (ClockData::*a_Action4)())
    : m_data(a_data), Action1(a_Action1), Action2(a_Action2), Action3(a_Action3), Action4(a_Action4)
    {
        
    }
    virtual void CheckChanges(std::chrono::system_clock::time_point a_now)
    {
        
    }
    ClockData& m_data;
    void (ClockData::*Action1)() = nullptr;
    void (ClockData::*Action2)() = nullptr;
    void (ClockData::*Action3)() = nullptr;
    void (ClockData::*Action4)() = nullptr;
    
};


#endif

