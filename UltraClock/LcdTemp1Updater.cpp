#include "LcdTemp1Updater.h"
#include <lcd.h>

void LcdTemp1Updater::Update()
{
    lcdPosition(m_lcdHandle, m_col, m_line);
    lcdPrintf(m_lcdHandle, "%4.1f%cC", (float)m_data.GetTemp1()/1000, 0xdf);
}

void LcdTemp1Updater::Clean()
{
    lcdPosition(m_lcdHandle, m_col, m_line);
    lcdPrintf(m_lcdHandle, "       ");
}