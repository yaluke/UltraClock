#ifndef _LCDALARMUPDATER_H_
#define _LCDALARMUPDATER_H_

#include "LcdUpdater.h"

class LcdAlarmUpdater : public LcdUpdater
{
public:
    LcdAlarmUpdater(int a_lcdHandle, ClockData &a_data, unsigned int a_updateBit, int a_line = 0, int a_col = 0)
    : LcdUpdater(a_lcdHandle, a_data, a_updateBit, a_line, a_col)
    {
        
    }
    virtual void Update();
    virtual void Clean();
};

#endif 