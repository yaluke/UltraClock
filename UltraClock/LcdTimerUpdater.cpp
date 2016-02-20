#include "LcdTimerUpdater.h"
#include <lcd.h>
#include <sstream>
#include <iomanip>
#include <string>

using namespace std;

void LcdTimerUpdater::Update()
{
    lcdPosition(m_lcdHandle, m_col, m_line);
    //long time = m_data.m_timeTimer.count();
    //lcdPrintf(m_lcdHandle, "TR %02u:%02u:%02u", m_data.m_timeTimerHour, m_data.m_timeTimerMinute, m_data.m_timeTimerSecond);
    stringstream ss;
    char chColon = m_data.GetTimerShowColons() ? ':' : ' ';
    ClockData::TimerData timerData = m_data.GetTimerData();
    switch( m_data.GetTimerSetIdx() )
    {
        case 0:
            ss << "TR " << setfill('0') << setw(2) << to_string(timerData.m_timeTimerHour) << chColon << setw(2) << to_string(timerData.m_timeTimerMinute) << chColon << setw(2) << to_string(timerData.m_timeTimerSecond);
            break;
        case 1:
            if(m_data.GetSetShow())
            {
                ss << "TR " <<  setfill('0') << setw(2) << to_string(timerData.m_timeTimerHour) << ":" << setw(2) << to_string(timerData.m_timeTimerMinute) << ":" << setw(2) << to_string(timerData.m_timeTimerSecond);
            }
            else
            {
                ss << "TR " <<  setfill('0') <<  "  :" << setw(2) << to_string(timerData.m_timeTimerMinute) <<  ":" << setw(2) << to_string(timerData.m_timeTimerSecond);
            }
            break;
        case 2:
            if(m_data.GetSetShow())
            {
                ss << "TR " <<  setfill('0') << setw(2) << to_string(timerData.m_timeTimerHour) << ":" << setw(2) << to_string(timerData.m_timeTimerMinute) << ":" << setw(2) << to_string(timerData.m_timeTimerSecond);
            }
            else
            {
                ss << "TR " <<  setfill('0') << setw(2) << to_string(timerData.m_timeTimerHour) << ":  :" << setw(2) << to_string(timerData.m_timeTimerSecond);
            }
            break;
        case 3:
            if(m_data.GetSetShow())
            {
                ss << "TR " <<  setfill('0') << setw(2) << to_string(timerData.m_timeTimerHour) << ":" << setw(2) << to_string(timerData.m_timeTimerMinute) << ":" << setw(2) << to_string(timerData.m_timeTimerSecond);
            }
            else
            {
                ss << "TR " <<  setfill('0') << setw(2) << to_string(timerData.m_timeTimerHour) << ":" << setw(2) << to_string(timerData.m_timeTimerMinute) << ":  ";
            }
            break;
    }
    lcdPuts(m_lcdHandle, ss.str().c_str());
}

void LcdTimerUpdater::Clean()
{
    lcdPosition(m_lcdHandle, m_col, m_line);
    lcdPrintf(m_lcdHandle, "           ");
}
