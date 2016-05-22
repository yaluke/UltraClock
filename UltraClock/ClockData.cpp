#include "ClockData.h"
#include <wiringPi.h>

using namespace std;
using namespace std::chrono;

ClockData::ClockData()
{
}

int ClockData::CheckChanges(system_clock::time_point a_now)
{
    std::time_t tt;
    tt = system_clock::to_time_t(a_now);
    tm local_tm = *localtime(&tt);
    SetDateTime(local_tm);
    
    //set blinking
    if(m_timeAlarmSetIdx != 0)
    {
        auto time1 = a_now - m_timeSetClk;
        duration<double, milli> time2(time1);
        int time3 = ((int)time2.count())%1000;
        
        if(m_setShow)
        {
            if( time3 > 600 )
            {
                m_setShow = false;
                SetStateBit(CHG_ALARM);
            }
        }
        else
        {
            if( time3 < 600 )
            {
                m_setShow = true;
                SetStateBit(CHG_ALARM);
            }
        }

        if(time2.count() > 15000)
        {
            m_setShow = true;
            m_timeAlarmSetIdx = 0;
            SetStateBit(CHG_ALARM);
        }

    }
   
    if(m_timeTimerSetIdx != 0)
    {
        auto time1 = a_now - m_timeSetClk;
        duration<double, milli> time2(time1);
        int time3 = ((int)time2.count())%1000;
        
        if(m_setShow)
        {
            if( time3 > 600 )
            {
                m_setShow = false;
                SetStateBit(CHG_TIMER);
            }
        }
        else
        {
            if( time3 < 600 )
            {
                m_setShow = true;
                SetStateBit(CHG_TIMER);
            }
        }
        
        if(time2.count() > 15000)
        {
            m_setShow = true;
            m_timeTimerSetIdx = 0;
            SetStateBit(CHG_TIMER);
        }
        
    }
    
    if(m_timeLogTimerSetIdx != 0)
    {
        auto time1 = a_now - m_timeSetClk;
        duration<double, milli> time2(time1);
        int time3 = ((int)time2.count())%1000;
        
        if(m_setShow)
        {
            if( time3 > 600 )
            {
                m_setShow = false;
                SetStateBit(CHG_LOG_TIMER);
            }
        }
        else
        {
            if( time3 < 600 )
            {
                m_setShow = true;
                SetStateBit(CHG_LOG_TIMER);
            }
        }
        
        if(time2.count() > 15000)
        {
            m_setShow = true;
            m_timeLogTimerSetIdx = 0;
            SetStateBit(CHG_LOG_TIMER);
        }
        
    }
    
    if(m_eSound != eSoundType::eNone)
    {
        auto time1 = a_now - m_timeSoundStarted;
        duration<double, milli> time2(time1);
        int time3 = ((int)time2.count());
        int time4 = time3%1000;
        switch(m_eSound)
        {
            case eSoundType::eBip:
                if(time4 < 50)
                {
                    m_sound = true;
                }
                else
                {
                    m_sound = false;
                    m_eSound = eSoundType::eNone;
                }
                break;
            case eSoundType::eAlarm:
                if(time3 >= 20000)
                {
                    m_sound = false;
                    m_eSound = eSoundType::eNone;
                }
                else if(time4 < 100 || (time4 > 200 && time4 < 300) )
                {
                    m_sound = true;
                }
                else
                {
                    m_sound = false;
                }
                break;
//          case eSoundType::eSignal:
//              break;
            case eSoundType::eNone:
                break;
        }
    }

    //stoper updater
    if(m_clockMode == eStoper)
    {
        if(m_stoperStatus == eStoperStatus::eStarted)
        {
            duration<double, milli> stoperTime = m_stoperTime + (a_now - m_stoperLastStop);
            long decis = stoperTime.count()/100;
            if( decis%10 != m_timeStoperData.m_timeStoperDeci)
            {
                m_timeStoperData.m_timeStoperDeci = decis%10;
                m_timeStoperData.m_timeStoperSecond = (decis/10)%60;
                m_timeStoperData.m_timeStoperMinute = (decis/600)%60;
                m_timeStoperData.m_timeStoperHour = (decis/36000)%24;
                m_stoperShowColons = m_timeStoperData.m_timeStoperDeci < 5;
                SetStateBit(CHG_STOPER);
            }
        }
        else if(m_stoperStatus == eStoperStatus::eSplitted)
        {
            duration<double, milli> stoperTime = m_stoperTime + (a_now - m_stoperLastStop);
            long decis = ((long)stoperTime.count()/100)%10;
            if( (decis < 5 && !m_stoperShowColons) || (decis >=5 && m_stoperShowColons) )
            {
                m_stoperShowColons = decis < 5;
                SetStateBit(CHG_STOPER);
            }
        }
    }
    
    //timer updater
    if( m_clockMode == eTimer && m_timerStatus == eTimerStatus::eStarted )
    {
        duration<double, milli> timerTime = m_timerTime + (a_now - m_timerLastStop);
        long tmp;
        if(m_timeTimerDataInit.m_timeTimerHour == 0 && m_timeTimerDataInit.m_timeTimerMinute == 0 && m_timeTimerDataInit.m_timeTimerSecond == 0)
        {
            tmp = 24*36000;
        }
        else
        {
            tmp = m_timeTimerDataInit.m_timeTimerHour*36000 + m_timeTimerDataInit.m_timeTimerMinute *600 + m_timeTimerDataInit.m_timeTimerSecond*10;
        }
        long decis = tmp - timerTime.count()/100;
        if( decis <= 0)
        {
            m_timerStatus = eTimerStatus::eEnded;
            m_timerShowColons = true;
            SetStateBit(CHG_TIMER);
            StartSound(a_now, eSoundType::eAlarm);
        }
        else if( (decis/10)%60 != m_timeTimerData.m_timeTimerSecond )
        {
            m_timeTimerData.m_timeTimerSecond = (decis/10)%60;
            m_timeTimerData.m_timeTimerMinute = (decis/600)%60;
            m_timeTimerData.m_timeTimerHour = (decis/36000)%24;
            m_timerShowColons = decis%10 < 5;
            SetStateBit(CHG_TIMER);
        }
        else if(m_timerShowColons != (decis%10 < 5) )
        {
            m_timerShowColons = decis%10 < 5;
            SetStateBit(CHG_TIMER);
        }
    }

    //log timer updater
    if( m_timerLogStatus == eTimerStatus::eStarted )
    {
        duration<double, milli> timerTime = m_timerLogTime + (a_now - m_timerLogLastStop);
        long tmp = m_timeLogTimerDataInit.m_timeLogTimerSecond*10 + m_timeLogTimerDataInit.m_timeLogTimerDeci;
        long decis = tmp - timerTime.count()/100;
        if( decis <= 0)
        {
            m_timeLogTimerData.m_timeLogTimerDeci = 0;
            m_timerLogStatus = eTimerStatus::eEnded;
            m_timerLogShowDot = true;
            SetStateBit(CHG_LOG_TIMER);
        }
        else if( (decis%10) != m_timeLogTimerData.m_timeLogTimerDeci )
        {
            m_timeLogTimerData.m_timeLogTimerDeci = decis%10;
            m_timeLogTimerData.m_timeLogTimerSecond = (decis/10)%60;
            m_timerLogShowDot = decis%10 < 5;
            SetStateBit(CHG_LOG_TIMER);
        }
        else if(m_timerLogShowDot != (decis%10 < 5) )
        {
            m_timerLogShowDot = decis%10 < 5;
            SetStateBit(CHG_LOG_TIMER);
        }
    }

    return GetState();
}

//temperature sensors
int ClockData::GetTemp(unsigned int a_idx)
{
    if(a_idx < m_temp.size())
    {
        ZeroStateBit(CHG_TEMP_1 << a_idx);
        return m_temp[a_idx];
    }
    return 0;
}

void ClockData::SetTemp(unsigned int a_idx, int a_newTemp)
{
    if(a_idx < m_temp.size() && a_newTemp != m_temp[a_idx])
    {
        m_temp[a_idx] = a_newTemp;
        SetStateBit(CHG_TEMP_1 << a_idx);
    }
}

//clock mode/type
void ClockData::ClockTypeModeUpdated()
{
    ZeroStateBit(CHG_CLOCK_TYPE);
    ZeroStateBit(CHG_MODE);
}

int ClockData::GetClockType()
{
    //ZeroStateBit(CHG_CLOCK_TYPE);
    return m_clockType;
}

int ClockData::GetPrevClockType()
{
    return m_prevClockType;
}

void ClockData::SetClockType(int a_newClockType)
{
    if(a_newClockType != m_clockType)
    {
        m_prevClockType = m_clockType;
        m_clockType = a_newClockType;
        SetStateBit(CHG_CLOCK_TYPE);
    }
}

int ClockData::GetClockMode()
{
    //ZeroStateBit(CHG_MODE);
    return m_clockMode;
}

int ClockData::GetPrevClockMode()
{
    return m_prevClockMode;
}

void ClockData::SetClockMode(int a_newClockMode)
{
    if(a_newClockMode != m_clockMode)
    {
        m_prevClockMode = m_clockMode;
        m_clockMode = a_newClockMode;
        SetStateBit(CHG_MODE);
    }
}

//date time
tm ClockData::GetDate()
{
    ZeroStateBit(CHG_DATE);
    return m_dateTime;
}

tm ClockData::GetTime()
{
    ZeroStateBit(CHG_TIME);
    return m_dateTime;
}

void ClockData::SetDateTime(tm a_newDateTime)
{
    if(a_newDateTime.tm_hour != m_dateTime.tm_hour
       || a_newDateTime.tm_min != m_dateTime.tm_min
       || a_newDateTime.tm_sec != m_dateTime.tm_sec)
    {
        if(a_newDateTime.tm_year != m_dateTime.tm_year
           || a_newDateTime.tm_mon != m_dateTime.tm_mon
           || a_newDateTime.tm_mday != m_dateTime.tm_mday
           || a_newDateTime.tm_wday != m_dateTime.tm_wday)
        {
            m_dateTime = a_newDateTime;
            SetStateBit(CHG_DATE);
            SetStateBit(CHG_TIME);
        }
        else
        {
            m_dateTime = a_newDateTime;
            SetStateBit(CHG_TIME);
        }
    }
}

//alarm
ClockData::AlarmData ClockData::GetAlarmData()
{
    ZeroStateBit(CHG_ALARM);
    return m_timeAlarmData;
}

int ClockData::GetAlarmSetIdx()
{
    return m_timeAlarmSetIdx;
}

//stoper
ClockData::StoperData ClockData::GetStoperData()
{
    ZeroStateBit(CHG_STOPER);
    return m_timeStoperData;
}

bool ClockData::GetStoperShowColons()
{
    return m_stoperShowColons;
}

//timer
ClockData::TimerData ClockData::GetTimerData()
{
    ZeroStateBit(CHG_TIMER);
    return m_timeTimerData;
}

ClockData::TimerData ClockData::GetTimerDataInit()
{
    ZeroStateBit(CHG_TIMER);
    return m_timeTimerDataInit;
}

bool ClockData::GetTimerShowColons()
{
    return m_timerShowColons;
}

int ClockData::GetTimerSetIdx()
{
    return m_timeTimerSetIdx;
}

//log timer
ClockData::LogTimerData ClockData::GetLogTimerData()
{
    ZeroStateBit(CHG_LOG_TIMER);
    return m_timeLogTimerData;
}

ClockData::LogTimerData ClockData::GetLogTimerDataInit()
{
    ZeroStateBit(CHG_LOG_TIMER);
    return m_timeLogTimerDataInit;
}

bool ClockData::GetLogTimerShowDot()
{
    return m_timerLogShowDot;
}

int ClockData::GetLogTimerSetIdx()
{
    return m_timeLogTimerSetIdx;
}

//sound
void ClockData::StartSound(std::chrono::system_clock::time_point a_now, eSoundType a_type)
{
    m_eSound = a_type;
    m_timeSoundStarted = a_now;
}

void ClockData::StopSound()
{
    m_eSound = eSoundType::eNone;
    m_sound = false;
}

bool ClockData::GetSound()
{
    return m_sound;
}

//helpers
bool ClockData::GetSetShow()
{
    return m_setShow;
}

//changers
//mode changer: calls by button - change clock more
void ClockData::ChangeMode()
{
    m_prevClockMode = m_clockMode;
    switch(m_clockType)
    {
        case 0:
            m_clockMode = (m_clockMode+1)%4;
            m_timeAlarmSetIdx= 0;
            m_timeTimerSetIdx = 0;
            m_timeLogTimerSetIdx = 0;
            m_setShow = true;
            break;
        case 1:
            m_clockMode = (m_clockMode+1)%3;
            break;
        case 2:
            m_clockMode = (m_clockMode+1)%2;
            break;
    }
    SetStateBit(CHG_MODE);
}

void ClockData::ChangeClockType()
{
    m_prevClockType = m_clockType;
    m_clockType = (m_clockType+1)%3;
    m_clockMode = 0;
    m_timeAlarmSetIdx= 0;
    m_timeTimerSetIdx = 0;
    m_timeLogTimerSetIdx = 0;
    SetStateBit(CHG_MODE | CHG_CLOCK_TYPE);
}

//alarm changers
void ClockData::ChangeAlarmSet()
{
    m_timeAlarmSetIdx = (m_timeAlarmSetIdx+1)%3;
    if(m_timeAlarmSetIdx)
    {
        m_timeSetClk = std::chrono::system_clock::now();
    }
    else if(!m_setShow)
    {
        m_setShow = true;
        SetStateBit(CHG_ALARM);
    }
}

void ClockData::ChangeAlarmHourUp()
{
    switch(m_timeAlarmSetIdx)
    {
        case 1:
            m_timeAlarmData.m_timeAlarmHour = (m_timeAlarmData.m_timeAlarmHour + 1)%24;
            SetStateBit(CHG_ALARM);
            m_timeSetClk = std::chrono::system_clock::now();
            break;
        case 2:
            m_timeAlarmData.m_timeAlarmMinute = (m_timeAlarmData.m_timeAlarmMinute + 1)%60;
            SetStateBit(CHG_ALARM);
            m_timeSetClk = std::chrono::system_clock::now();
            break;
    }
}

void ClockData::ChangeAlarmHourDown()
{
    switch(m_timeAlarmSetIdx)
    {
        case 1:
            m_timeAlarmData.m_timeAlarmHour = m_timeAlarmData.m_timeAlarmHour ? (m_timeAlarmData.m_timeAlarmHour - 1) : 23;
            SetStateBit(CHG_ALARM);
            m_timeSetClk = std::chrono::system_clock::now();
            break;
        case 2:
            m_timeAlarmData.m_timeAlarmMinute = m_timeAlarmData.m_timeAlarmMinute ? (m_timeAlarmData.m_timeAlarmMinute - 1) : 59;
            SetStateBit(CHG_ALARM);
            m_timeSetClk = std::chrono::system_clock::now();
            break;
    }
}

//stoper changers
void ClockData::ChangeStoperStartStop()
{
    switch(m_stoperStatus)
    {
        case eStoperStatus::eZero:
            m_stoperTime = std::chrono::duration<double, std::milli>::zero();
            m_stoperLastStop = std::chrono::system_clock::now();
            m_stoperStatus = eStoperStatus::eStarted;
            break;
        case eStoperStatus::eStarted:
            m_stoperStatus = eStoperStatus::eStopped;
            m_stoperNow = std::chrono::system_clock::now();
            m_stoperTime += (m_stoperNow - m_stoperLastStop);
            m_stoperShowColons = true;
            SetStateBit(CHG_STOPER);
            break;
        case eStoperStatus::eSplitted:
            m_stoperStatus = eStoperStatus::eSplittedStopped;
            m_stoperNow = std::chrono::system_clock::now();
            m_stoperTime += (m_stoperNow - m_stoperLastStop);
            m_stoperShowColons = true;
            SetStateBit(CHG_STOPER);
            break;
        case eStoperStatus::eSplittedStopped:
            m_stoperStatus = eStoperStatus::eSplitted;
            break;
        case eStoperStatus::eStopped:
            m_stoperLastStop = std::chrono::system_clock::now();
            m_stoperStatus = eStoperStatus::eStarted;
            break;
    }
}

void ClockData::ChangeStoperSplitReset()
{
    long decis;
    switch(m_stoperStatus)
    {
        case eStoperStatus::eZero:
            //do nothing
            break;
        case eStoperStatus::eStarted:
            m_stoperStatus = eStoperStatus::eSplitted;
            break;
        case eStoperStatus::eSplitted:
            m_stoperStatus = eStoperStatus::eStarted;
            break;
        case eStoperStatus::eSplittedStopped:
            m_stoperStatus = eStoperStatus::eStopped;
            decis = m_stoperTime.count()/100;
            m_timeStoperData.m_timeStoperDeci = decis%10;
            m_timeStoperData.m_timeStoperSecond = (decis/10)%60;
            m_timeStoperData.m_timeStoperMinute = (decis/600)%60;
            m_timeStoperData.m_timeStoperHour = (decis/36000)%24;
            SetStateBit(CHG_STOPER);
            break;
        case eStoperStatus::eStopped:
            m_timeStoperData.m_timeStoperHour = 0;
            m_timeStoperData.m_timeStoperMinute = 0;
            m_timeStoperData.m_timeStoperSecond = 0;
            m_timeStoperData.m_timeStoperDeci = 0;
            m_stoperStatus = eStoperStatus::eZero;
            m_stoperTime = std::chrono::duration<double, std::milli>::zero();
            m_stoperShowColons = true;
            SetStateBit(CHG_STOPER);
            break;
    }
}

//timer changers
void ClockData::ChangeTimerStartStop()
{
    if(!m_timeTimerSetIdx)
    {
        switch(m_timerStatus)
        {
            case eTimerStatus::eZero:
                m_timerTime = std::chrono::duration<double, std::milli>::zero();
                m_timerLastStop = std::chrono::system_clock::now();
                m_timerStatus = eTimerStatus::eStarted;
                //start
                break;
            case eTimerStatus::eStarted:
                m_timerStatus = eTimerStatus::eStopped;
                m_timerNow = std::chrono::system_clock::now();
                m_timerTime += (m_timerNow - m_timerLastStop);
                m_timerShowColons = true;
                SetStateBit(CHG_TIMER);
                break;
            case eTimerStatus::eStopped:
                m_timerStatus = eTimerStatus::eStarted;
                m_timerLastStop = std::chrono::system_clock::now();
                break;
            case eTimerStatus::eEnded:
                //do nothing
                break;
        }
    }
}

void ClockData::ChangeTimerStartStopUp()
{
    if(!m_timeTimerSetIdx)
    {
        ChangeTimerStartStop();
    }
    else
    {
        switch(m_timeTimerSetIdx)
        {
            case 1:
                m_timeTimerData.m_timeTimerHour = m_timeTimerDataInit.m_timeTimerHour = (m_timeTimerData.m_timeTimerHour + 1)%24;
                SetStateBit(CHG_TIMER);
                m_timeSetClk = std::chrono::system_clock::now();
                break;
            case 2:
                m_timeTimerData.m_timeTimerMinute = m_timeTimerDataInit.m_timeTimerMinute = (m_timeTimerData.m_timeTimerMinute + 1)%60;
                SetStateBit(CHG_TIMER);
                m_timeSetClk = std::chrono::system_clock::now();
                break;
            case 3:
                m_timeTimerData.m_timeTimerSecond = m_timeTimerDataInit.m_timeTimerSecond = (m_timeTimerData.m_timeTimerSecond + 1)%60;
                SetStateBit(CHG_TIMER);
                m_timeSetClk = std::chrono::system_clock::now();
                break;
        }
    }
    
}

void ClockData::ChangeTimerStartStopDown()
{
    if(!m_timeTimerSetIdx)
    {
        ChangeTimerStartStop();
    }
    else
    {
        switch(m_timeTimerSetIdx)
        {
            case 1:
                m_timeTimerData.m_timeTimerHour = m_timeTimerDataInit.m_timeTimerHour = m_timeTimerData.m_timeTimerHour ? (m_timeTimerData.m_timeTimerHour - 1) : 23;
                SetStateBit(CHG_TIMER);
                m_timeSetClk = std::chrono::system_clock::now();
                break;
            case 2:
                m_timeTimerData.m_timeTimerMinute = m_timeTimerDataInit.m_timeTimerMinute = m_timeTimerData.m_timeTimerMinute ? (m_timeTimerData.m_timeTimerMinute - 1) : 59;
                SetStateBit(CHG_TIMER);
                m_timeSetClk = std::chrono::system_clock::now();
                break;
            case 3:
                m_timeTimerData.m_timeTimerSecond = m_timeTimerDataInit.m_timeTimerSecond = m_timeTimerData.m_timeTimerSecond ? (m_timeTimerData.m_timeTimerSecond - 1) : 59;
                SetStateBit(CHG_TIMER);
                m_timeSetClk = std::chrono::system_clock::now();
                break;
        }
    }
    
}

void ClockData::ChangeTimerSetReset()
{
    switch(m_timerStatus)
    {
        case eTimerStatus::eZero:
            m_timeTimerSetIdx = (m_timeTimerSetIdx+1)%4;
            if(m_timeTimerSetIdx)
            {
                m_timeSetClk = std::chrono::system_clock::now();
            }
            else if(!m_setShow)
            {
                m_setShow = true;
                SetStateBit(CHG_TIMER);
            }
            break;
        case eTimerStatus::eEnded:
        case eTimerStatus::eStopped:
        case eTimerStatus::eStarted:
            m_timerStatus = eTimerStatus::eZero;
            m_timeTimerData.m_timeTimerHour = m_timeTimerDataInit.m_timeTimerHour;
            m_timeTimerData.m_timeTimerMinute = m_timeTimerDataInit.m_timeTimerMinute;
            m_timeTimerData.m_timeTimerSecond = m_timeTimerDataInit.m_timeTimerSecond;
            m_timerShowColons = true;
            SetStateBit(CHG_TIMER);
            break;
            
    }
}

//log timer changers
void ClockData::ChangeLogTimerStartStop()
{
    if(!m_timeLogTimerSetIdx)
    {
        switch(m_timerLogStatus)
        {
            case eTimerStatus::eZero:
                m_timerLogTime = std::chrono::duration<double, std::milli>::zero();
                m_timerLogLastStop = std::chrono::system_clock::now();
                m_timerLogStatus = eTimerStatus::eStarted;
                //start
                break;
            case eTimerStatus::eStarted:
                m_timerLogStatus = eTimerStatus::eStopped;
                m_timerLogNow = std::chrono::system_clock::now();
                m_timerLogTime += (m_timerLogNow - m_timerLogLastStop);
                m_timerLogShowDot = true;
                SetStateBit(CHG_LOG_TIMER);
                break;
            case eTimerStatus::eStopped:
                m_timerLogStatus = eTimerStatus::eStarted;
                m_timerLogLastStop = std::chrono::system_clock::now();
                break;
            case eTimerStatus::eEnded:
                //do nothing
                break;
        }
    }
}

void ClockData::ChangeLogTimerStartStopUp()
{
    if(!m_timeLogTimerSetIdx)
    {
        ChangeLogTimerStartStop();
    }
    else
    {
        switch(m_timeLogTimerSetIdx)
        {
            case 1: //seconds - from predefined set base on step
                m_timeLogTimerData.m_timeLogTimerSecond = m_timeLogTimerDataInit.m_timeLogTimerSecond = (m_timeLogTimerData.m_timeLogTimerSecond + 1)%256;
                SetStateBit(CHG_LOG_TIMER);
                m_timeSetClk = std::chrono::system_clock::now();
                break;
            case 2:
                SetStateBit(CHG_LOG_TIMER);
                m_timeSetClk = std::chrono::system_clock::now();
                break;
        }
    }
    
}

void ClockData::ChangeLogTimerStartStopDown()
{
    if(!m_timeLogTimerSetIdx)
    {
        ChangeTimerStartStop();
    }
    else
    {
        switch(m_timeLogTimerSetIdx)
        {
            case 1:
                m_timeLogTimerData.m_timeLogTimerSecond = m_timeLogTimerDataInit.m_timeLogTimerSecond = m_timeLogTimerData.m_timeLogTimerSecond ? (m_timeLogTimerData.m_timeLogTimerSecond - 1) : 256;
                SetStateBit(CHG_LOG_TIMER);
                m_timeSetClk = std::chrono::system_clock::now();
                break;
            case 2:
                SetStateBit(CHG_LOG_TIMER);
                m_timeSetClk = std::chrono::system_clock::now();
                break;
        }
    }
    
}

void ClockData::ChangeLogTimerSetReset()
{
    switch(m_timerLogStatus)
    {
        case eTimerStatus::eZero:
            m_timeLogTimerSetIdx = (m_timeLogTimerSetIdx+1)%2;
            if(m_timeLogTimerSetIdx)
            {
                m_timeSetClk = std::chrono::system_clock::now();
            }
            else if(!m_setShow)
            {
                m_setShow = true;
                SetStateBit(CHG_LOG_TIMER);
            }
            break;
        case eTimerStatus::eEnded:
        case eTimerStatus::eStopped:
        case eTimerStatus::eStarted:
            m_timerLogStatus = eTimerStatus::eZero;
            m_timeLogTimerData.m_timeLogTimerSecond = m_timeLogTimerDataInit.m_timeLogTimerSecond;
            m_timeLogTimerData.m_timeLogTimerDeci = m_timeLogTimerDataInit.m_timeLogTimerDeci;
            m_timerLogShowDot = true;
            SetStateBit(CHG_LOG_TIMER);
            break;
            
    }
}

//state bit - thread safe
unsigned int ClockData::GetState()
{
    return m_change_state_ts;
}

void ClockData::SetStateBit(unsigned int a_changeBit)
{
    std::lock_guard<std::mutex> lck(m_mtxChange);
    m_change_state_ts |= a_changeBit;
}

void ClockData::ZeroStateBit(unsigned int a_changeBit)
{
    std::lock_guard<std::mutex> lck(m_mtxChange);
    m_change_state_ts &= ~a_changeBit;
}


