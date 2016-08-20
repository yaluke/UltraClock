#ifndef _CLOCKDATA_H_
#define _CLOCKDATA_H_

#include <chrono>
#include <array>
#include <string>
#include <mutex>

#define CHG_NONE            0
#define CHG_TIME            1
#define CHG_DATE            2
#define CHG_ALARM           4
#define CHG_STOPER          8
#define CHG_TIMER           16
#define CHG_MODE            32
#define CHG_TEMP_1          64
#define CHG_TEMP_2          128
#define CHG_TEMP_3          256
#define CHG_CLOCK_TYPE      512
#define CHG_LOG_TIMER       1024
#define CHG_DBL_LOG_TIMER   2048
#define CHG_LCD_ON          4096
#define CHG_ALL_STATES      8191

enum eClockMode
{
    eDateTime,
    eAlarm,
    eStoper,
    eTimer
};

enum class eStoperStatus
{
    eZero,
    eStarted,
    eSplitted,
    eSplittedStopped,
    eStopped
};

enum class eTimerStatus
{
    eZero,
    eStarted,
    eStopped,
    eEnded
};

enum class eSoundType
{
    eNone,
    eBip,   //once 50msec
    eAlarm, //20 sec, 2 bits per sec (100/200/100/600)
    //eSignal //10 sec, 1 bit per sec (100/900)
};

//collects all the data presented by a clock in all the modes
class ClockData
{
public:
    ClockData();
    int CheckChanges(std::chrono::system_clock::time_point a_now);
    void ChangeMode();
    void ChangeClockType();

    //temperature sensors
    int GetTemp(unsigned int a_idx);
    void SetTemp(unsigned int a_idx, int a_newTemp);
    
    //clock mode/type
    void ClockTypeModeUpdated();
    int GetClockType();
    int GetPrevClockType();
    void SetClockType(int a_newClockType);
    int GetClockMode();
    int GetPrevClockMode();
    void SetClockMode(int a_newClockMode);
    
    //date time
    tm GetDate();
    tm GetTime();
    void SetDateTime(tm a_newDateTime);
    
    //alarm
    struct AlarmData
    {
        int m_timeAlarmHour = 7;
        int m_timeAlarmMinute = 0;
    };
    
    AlarmData GetAlarmData();
    int GetAlarmSetIdx();
    
    //stoper
    struct StoperData
    {
        int m_timeStoperHour = 0;
        int m_timeStoperMinute = 0;
        int m_timeStoperSecond = 0;
        int m_timeStoperDeci = 0;
    };
    
    StoperData GetStoperData();
    bool GetStoperShowColons();

    //timer
    struct TimerData
    {
        int m_timeTimerHour = 0;
        int m_timeTimerMinute = 0;
        int m_timeTimerSecond = 0;
    };
    
    TimerData GetTimerData();
    TimerData GetTimerDataInit();
    bool GetTimerShowColons();
    int GetTimerSetIdx();

    //log timer
    enum LogTimerStep
    {
        Step1_12 = 1,
        Step1_6 = 2,
        Step1_3 = 4,
        Step1 = 12
    };
    struct LogTimerData
    {
        int m_timeLogTimerSecond = 8;
        int m_timeLogTimerDeci = 0;
        int m_timeLogId = 0;
        LogTimerStep m_timeLogStep = Step1_3;
    };
    LogTimerData GetLogTimerData();
    LogTimerData GetLogTimerDataInit();
    bool GetLogTimerShowDot();
    int GetLogTimerSetIdx();
    
    //double log timer
    struct DoubleLogTimerData
    {
        int m_timeDoubleLogTimer1Second = 8;
        int m_timeDoubleLogTimer1Deci = 0;
        int m_timeDoubleLog1Id = 0;
        int m_timeDoubleLogTimer2Second = 8;
        int m_timeDoubleLogTimer2Deci = 0;
        int m_timeDoubleLog2Id = 0;
        LogTimerStep m_timeDoubleLogStep = Step1_3;
    };

    DoubleLogTimerData GetDoubleLogTimerData();
    DoubleLogTimerData GetDoubleLogTimerDataInit();
    bool GetDoubleLogTimerShowDot1();
    bool GetDoubleLogTimerShowDot2();
    int GetDoubleLogTimerSetIdx();

    //sounds
    void StartSound(std::chrono::system_clock::time_point a_now, eSoundType a_type = eSoundType::eBip);
    void StopSound();
    bool GetSound();

    //helpers
    bool GetSetShow();
    
    //Alarm changers
    void ChangeAlarmSet();
    void ChangeAlarmHourUp();
    void ChangeAlarmHourDown();
    
    //Stoper changers
    void ChangeStoperStartStop();
    void ChangeStoperSplitReset();
    
    //Timer changers
    void ChangeTimerStartStop();
    void ChangeTimerStartStopUp();
    void ChangeTimerStartStopDown();
    void ChangeTimerSetReset();

    //Log timer changers
    void ChangeLogTimerStartStop();
    void ChangeLogTimerUp();
    void ChangeLogTimerDown();
    void ChangeLogTimerSetReset();
    void ChangeLights();
    
    //state bit - thread safe
    unsigned int GetState();
    void SetStateBit(unsigned int a_changeBit);
    void ZeroStateBit(unsigned int a_changeBit);
    
    //lcd on/off
    bool GetLcdOn();
    void SetLcdOn(bool a_value);
    
    //lcd on/off changer
    void ChangeLcdOn();
    
protected:

    std::mutex m_mtxChange;
    int m_clockMode = 0;
    int m_prevClockMode = 0;
    int m_clockType = 0;    //normal clock, darkroom multitimer, enlarger stoper
    int m_prevClockType = 0;
    tm m_dateTime;
    //alarm data
    AlarmData m_timeAlarmData;
    int m_timeAlarmSetIdx = 0;   //which part of alarm data is currently set
    
    //stoper data
    StoperData m_timeStoperData;
    bool m_stoperShowColons = true;
    
    eStoperStatus m_stoperStatus = eStoperStatus::eZero;
    std::chrono::duration<double, std::milli> m_stoperTime;
    std::chrono::system_clock::time_point m_stoperLastStop;
    std::chrono::system_clock::time_point m_stoperNow;
    
    //timer data
    TimerData m_timeTimerData;
    TimerData m_timeTimerDataInit;

    int m_timeTimerSetIdx = 0;
    bool m_timerShowColons = true;
    
    std::chrono::duration<double, std::milli> m_timerTime;
    std::chrono::system_clock::time_point m_timerLastStop;
    std::chrono::system_clock::time_point m_timerNow;
    
    eTimerStatus m_timerStatus = eTimerStatus::eZero;
    
    //log timer data
    LogTimerData m_timeLogTimerData;
    LogTimerData m_timeLogTimerDataInit;
    
    int m_timeLogTimerSetIdx = 0;
    bool m_timerLogShowDot = true;
    
    std::chrono::duration<double, std::milli> m_timerLogTime;
    std::chrono::system_clock::time_point m_timerLogLastStop;
    //std::chrono::system_clock::time_point m_timerLogNow;  //unused
    
    eTimerStatus m_timerLogStatus = eTimerStatus::eZero;
    int m_enlargerState = 1;
    
    //double log timer data
    DoubleLogTimerData m_timeDoubleLogTimerData;
    DoubleLogTimerData m_timeDoubleLogTimerDataInit;
    
    int m_timeDoubleLogTimerSetIdx = 0;
    bool m_timerDoubleLogShowDot1 = false;
    bool m_timerDoubleLogShowDot2 = false;
    
    std::chrono::duration<double, std::milli> m_timerDoubleLogTime1;
    std::chrono::duration<double, std::milli> m_timerDoubleLogTime2;
    std::chrono::system_clock::time_point m_timerDoubleLogLastStop1;
    std::chrono::system_clock::time_point m_timerDoubleLogLastStop2;
    
    //temperature
    std::array<int, 3> m_temp = {{0}};
    //temperature from sensor 1 (internal)
    //int m_temp1 = 0;

    //temperature from sensor 2 & 3 (external)
    //int m_temp2 = 0;
    //int m_temp3 = 0;

    //helpers
    std::chrono::system_clock::time_point m_timeSetClk; //time when set btn was clicked (for smooth blinking)
    bool m_setShow = true;  //indicates if currently set value is shown or not (blinking)
    
    //sound
    eSoundType m_eSound = eSoundType::eNone;
    bool m_sound = false;
    std::chrono::system_clock::time_point m_timeSoundStarted;
    
    //lcd on/off
    bool m_lcdOn = true;
    
    unsigned int m_change_state_ts = CHG_ALL_STATES;
};

#endif
