#include "LcdTemp2Updater.h"
#include <lcd.h>

void LcdTemp2Updater::Update()
{
    lcdPosition(m_lcdHandle, m_col, m_line);
    lcdPrintf(m_lcdHandle, "%4.1f%cC", (float)m_data.GetTemp2()/1000, 0xdf);
}

void LcdTemp2Updater::Clean()
{
    lcdPosition(m_lcdHandle, m_col, m_line);
    lcdPrintf(m_lcdHandle, "       ");
}