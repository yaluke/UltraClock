#ifndef _LCDDOUBLELOGTIMERUPDATER_H_
#define _LCDDOUBLELOGTIMERUPDATER_H_

#include "LcdUpdater.h"
#include <string>

class LcdDoubleLogTimerUpdater : public LcdUpdater
{
public:
    LcdDoubleLogTimerUpdater(int a_lcdHandle, ClockData &a_data, unsigned int a_updateBit, int a_line = 0, int a_col = 0)
    : LcdUpdater(a_lcdHandle, a_data, a_updateBit, a_line, a_col)
    {
        
    }
    virtual void Update();
    virtual void Clean();
    std::string step(ClockData::LogTimerStep s);
};

#endif