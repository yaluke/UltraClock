#include "LcdDoubleLogTimerUpdater.h"
#include <lcd.h>
#include <sstream>
#include <iomanip>
#include <string>

using namespace std;

string LcdDoubleLogTimerUpdater::step(ClockData::LogTimerStep s)
{
    string str;
    switch(s)
    {
        case ClockData::Step1_12:
            str =  "1/12-s";
            break;
        case ClockData::Step1_6:
            str = " 1/6-s";
            break;
        case ClockData::Step1_3:
            str = " 1/3-s";
            break;
        case ClockData::Step1:
            str = "   1-s";
            break;
    }
    return str;
}

void LcdDoubleLogTimerUpdater::Update()
{
/*    lcdPosition(m_lcdHandle, m_col, m_line);
    stringstream ss1, ss2;
    string str;
    char chDot = m_data.GetDoubleLogTimerShowDot1() ? '.' : ' ';
    ClockData::LogTimerData timerData = m_data.GetLogTimerData();
    switch( m_data.GetLogTimerSetIdx() )
    {
        case 0:
            ss << setw(3) << to_string(timerData.m_timeLogTimerSecond) << chDot << to_string(timerData.m_timeLogTimerDeci) << "s";
            str = step(timerData.m_timeLogStep);
            break;
        case 1:
            if(m_data.GetSetShow())
            {
                ss << setw(3) << to_string(timerData.m_timeLogTimerSecond) << "." << to_string(timerData.m_timeLogTimerDeci) << "s";
            }
            else
            {
                ss << "      ";
            }
            str = step(timerData.m_timeLogStep);
            break;
        case 2:
            ss << setw(3) << to_string(timerData.m_timeLogTimerSecond) << "." << to_string(timerData.m_timeLogTimerDeci) << "s";
            if(m_data.GetSetShow())
            {
                str = step(timerData.m_timeLogStep);
            }
            else
            {
                str = "      ";
            }
            break;
    }
    lcdPuts(m_lcdHandle, ss.str().c_str());
    lcdPosition(m_lcdHandle, m_col, m_line+1);
    lcdPuts(m_lcdHandle, str.c_str());*/
}

void LcdDoubleLogTimerUpdater::Clean()
{
    lcdPosition(m_lcdHandle, m_col, m_line);
    lcdPrintf(m_lcdHandle, "      ");
}
