#include "LcdTimeSmallUpdater.h"
#include <lcd.h>

using namespace std;

void LcdTimeSmallUpdater::Update()
{
    lcdPosition(m_lcdHandle, m_col, m_line);
    tm dateTime = m_data.GetTime();

     lcdPrintf(m_lcdHandle, "%2u%c%02u%c%02u"
     , dateTime.tm_hour
     , dateTime.tm_sec%2 ? ' ' : ':'
     , dateTime.tm_min
     , dateTime.tm_sec%2 ? ' ' : ':'
     , dateTime.tm_sec);
}

void LcdTimeSmallUpdater::Clean()
{
    lcdPosition(m_lcdHandle, m_col, m_line);
    lcdPrintf(m_lcdHandle, "        ");
}
