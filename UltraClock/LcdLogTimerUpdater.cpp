#include "LcdLogTimerUpdater.h"
#include <lcd.h>
#include <sstream>
#include <iomanip>
#include <string>

using namespace std;

void LcdLogTimerUpdater::Update()
{
    lcdPosition(m_lcdHandle, m_col, m_line);
    stringstream ss;
    char chDot = m_data.GetLogTimerShowDot() ? '.' : ' ';
    ClockData::LogTimerData timerData = m_data.GetLogTimerData();
    switch( m_data.GetLogTimerSetIdx() )
    {
        case 0:
            ss << setw(3) << to_string(timerData.m_timeLogTimerSecond) << chDot << to_string(timerData.m_timeLogTimerDeci);
            break;
        case 1:
            if(m_data.GetSetShow())
            {
                ss << setw(3) << to_string(timerData.m_timeLogTimerSecond) << "." << to_string(timerData.m_timeLogTimerDeci);
            }
            else
            {
                ss << "     ";
            }
            break;
        case 2:
//todo: second line
            break;
    }
    lcdPuts(m_lcdHandle, ss.str().c_str());
}

void LcdLogTimerUpdater::Clean()
{
    lcdPosition(m_lcdHandle, m_col, m_line);
    lcdPrintf(m_lcdHandle, "     ");
}
