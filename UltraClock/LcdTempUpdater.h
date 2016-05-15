#ifndef _LCDTEMPUPDATER_H_
#define _LCDTEMPUPDATER_H_

#include "LcdUpdater.h"

class LcdTempUpdater : public LcdUpdater
{
public:
    LcdTempUpdater(int a_lcdHandle, ClockData &a_data, unsigned int a_updateBit, unsigned int a_idx, int a_line = 0, int a_col = 0)
    : LcdUpdater(a_lcdHandle, a_data, a_updateBit, a_line, a_col), m_idx(a_idx)
    {
        
    }
    virtual void Update();
    virtual void Clean();
protected:
    unsigned int m_idx;
};
#endif
