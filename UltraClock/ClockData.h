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
#define CHG_ALL_STATES      2047

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
        Step1_12 = 0,
        Step1_6 = 1,
        Step1_3 = 2,
        Step1 = 3
    };
    struct LogTimerData
    {
        int m_timeLogTimerSecond = 0;
        int m_timeLogTimerDeci = 0;
        LogTimerStep m_timeLogStep = Step1_3;
    };
    
    LogTimerData GetLogTimerData();
    LogTimerData GetLogTimerDataInit();
    bool GetLogTimerShowDot();
    int GetLogTimerSetIdx();
    
    
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
    void ChangeLogTimerStartStopUp();
    void ChangeLogTimerStartStopDown();
    void ChangeLogTimerSetReset();
    
    //state bit - thread safe
    unsigned int GetState();
    void SetStateBit(unsigned int a_changeBit);
    void ZeroStateBit(unsigned int a_changeBit);
    
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
    std::chrono::system_clock::time_point m_timerLogNow;
    
    eTimerStatus m_timerLogStatus = eTimerStatus::eZero;
    
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
    
    unsigned int m_change_state_ts = CHG_ALL_STATES;
};

#endif
