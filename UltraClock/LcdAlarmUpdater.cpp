#include "LcdAlarmUpdater.h"
#include <lcd.h>
#include <sstream>
#include <iomanip>
#include <string>

using namespace std;

void LcdAlarmUpdater::Update()
{
    lcdPosition(m_lcdHandle, m_col, m_line);
    stringstream ss;
    ClockData::AlarmData timeAlarmData = m_data.GetAlarmData();
    switch( m_data.GetAlarmSetIdx() )
    {
        case 0:
            ss << "AL " << setfill('0') << setw(2) << to_string(timeAlarmData.m_timeAlarmHour) << ":" << setw(2) << to_string(timeAlarmData.m_timeAlarmMinute);
            break;
        case 1:
            if(m_data.GetSetShow())
            {
                ss << "AL " <<  setfill('0') << setw(2) << to_string(timeAlarmData.m_timeAlarmHour) << ":" << setw(2) << to_string(timeAlarmData.m_timeAlarmMinute);
            }
            else
            {
                ss << "AL " <<  setfill('0') <<  "  :" << setw(2) << to_string(timeAlarmData.m_timeAlarmMinute);
            }
            break;
        case 2:
            if(m_data.GetSetShow())
            {
                ss << "AL " <<  setfill('0') << setw(2) << to_string(timeAlarmData.m_timeAlarmHour) << ":" << setw(2) << to_string(timeAlarmData.m_timeAlarmMinute);
            }
            else
            {
                ss << "AL " <<  setfill('0') << setw(2) << to_string(timeAlarmData.m_timeAlarmHour) << ":  ";
            }
            break;
    }
    lcdPuts(m_lcdHandle, ss.str().c_str());
}

void LcdAlarmUpdater::Clean()
{
    lcdPosition(m_lcdHandle, m_col, m_line);
    lcdPrintf(m_lcdHandle, "        ");
}