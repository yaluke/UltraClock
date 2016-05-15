#include "LcdTempUpdater.h"
#include <lcd.h>

void LcdTempUpdater::Update()
{
    lcdPosition(m_lcdHandle, m_col, m_line);
    lcdPrintf(m_lcdHandle, "%4.1f%cC", (float)m_data.GetTemp(m_idx)/1000, 0xdf);
}

void LcdTempUpdater::Clean()
{
    lcdPosition(m_lcdHandle, m_col, m_line);
    lcdPrintf(m_lcdHandle, "       ");
}