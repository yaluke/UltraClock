#ifndef _LCDUPDATER_H_
#define _LCDUPDATER_H_

#include "ClockData.h"

class LcdUpdater
{
public:
    LcdUpdater(int a_lcdHandle, ClockData &a_data, unsigned int a_updateBit, int a_line = 0, int a_col = 0)
    : m_lcdHandle(a_lcdHandle), m_data(a_data), m_updateBit(a_updateBit), m_line(a_line), m_col(a_col)
    {
        
    }
    virtual void Update()
    {
    }
    virtual void Clean()
    {
        
    }
    unsigned int GetUpdateBit()
    {
        return m_updateBit;
    }
    void Move(unsigned int a_line, unsigned int a_col)
    {
        m_line = a_line;
        m_col = a_col;
    }
protected:
    int m_lcdHandle;
    ClockData &m_data;
    unsigned int m_updateBit = 0;
    int m_line = 0;
    int m_col = 0;
};

#endif

