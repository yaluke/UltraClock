#include "LcdTemp3Updater.h"
#include <lcd.h>

void LcdTemp3Updater::Update()
{
    lcdPosition(m_lcdHandle, m_col, m_line);
    lcdPrintf(m_lcdHandle, "%4.1f%cC", (float)m_data.GetTemp3()/1000, 0xdf);
}

void LcdTemp3Updater::Clean()
{
    lcdPosition(m_lcdHandle, m_col, m_line);
    lcdPrintf(m_lcdHandle, "       ");
}