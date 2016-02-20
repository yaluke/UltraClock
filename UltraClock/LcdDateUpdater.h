#ifndef _LCDDATEUPDATER_H_
#define _LCDDATEUPDATER_H_

#include "LcdUpdater.h"

class LcdDateUpdater : public LcdUpdater
{
public:
    LcdDateUpdater(int a_lcdHandle, ClockData &a_data, unsigned int a_updateBit, int m_line = 0, int m_col = 0)
    : LcdUpdater(a_lcdHandle, a_data, a_updateBit, m_line, m_col)
    {
        
    }
    virtual void Update();
    virtual void Clean();
};
#endif

