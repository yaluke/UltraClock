#ifndef _LCDTIMEUPDATER_H_
#define _LCDTIMEUPDATER_H_

#include "LcdUpdater.h"

class LcdTimeUpdater : public LcdUpdater
{
public:
    LcdTimeUpdater(int a_lcdHandle, ClockData &a_data, unsigned int a_updateBit, int a_line = 0, int a_col = 0)
    : LcdUpdater(a_lcdHandle, a_data, a_updateBit, a_line, a_col)
    {
    }
    virtual void Update();
    virtual void Clean();
};

#endif

