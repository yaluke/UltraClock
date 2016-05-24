#include <wiringPi.h>
#include <lcd.h>
#include <iostream>
#include <chrono>
#include <thread>
#include <vector>
#include <map>
#include <fstream>
#include "ClockData.h"
#include "InputButton.h"
#include "LcdTimeUpdater.h"
#include "LcdTimeSmallUpdater.h"
#include "LcdDateUpdater.h"
#include "LcdAlarmUpdater.h"
#include "LcdStoperUpdater.h"
#include "LcdTimerUpdater.h"
#include "LcdLogTimerUpdater.h"
#include "LcdTempUpdater.h"

using namespace std;
using namespace std::chrono;

int initHardware()
{
    wiringPiSetupGpio();
    int fd = lcdInit(4, 20, 4, 7, 8, 18, 23, 24, 25, 0, 0, 0, 0);
    if(fd == -1)
    {
        cout << "InitHardware: wrong lcd handle!" << endl;
        return fd;
    }
    lcdDisplay(fd, true);
    lcdClear(fd);
    lcdHome(fd);
    
    //init buttons
    pinMode(GPIO_BTN_MODE, INPUT);
    pinMode(GPIO_BTN_DOWN, INPUT);
    pinMode(GPIO_BTN_UP, INPUT);
    pinMode(GPIO_BTN_SET, INPUT);
    pinMode(GPIO_BTN_CLOCK_TYPE, INPUT);
    pinMode(GPIO_BTN_SNOOZE_1, INPUT);
    pinMode(GPIO_BTN_SNOOZE_2, INPUT);
    
    //other inputs
    pinMode(12, OUTPUT);    //buzzer
    pinMode(20, OUTPUT);    //enlarger
    pinMode(21, OUTPUT);    //safelight
    
    digitalWrite(20, 1);
    digitalWrite(21, 0);
    
    return fd;
}

ClockData clock_data;
int lcdHandler;

LcdTimeUpdater time_updater(lcdHandler, clock_data, CHG_TIME);
LcdTimeSmallUpdater timeSmall_updater(lcdHandler, clock_data, CHG_TIME);
LcdDateUpdater date_updater(lcdHandler, clock_data, CHG_DATE);
LcdAlarmUpdater alarm_updater(lcdHandler, clock_data, CHG_ALARM);
LcdStoperUpdater stoper_updater(lcdHandler, clock_data, CHG_STOPER);
LcdTimerUpdater timer_updater(lcdHandler, clock_data, CHG_TIMER);
LcdLogTimerUpdater log_timer_updater(lcdHandler, clock_data, CHG_LOG_TIMER);
LcdTempUpdater temp1_updater(lcdHandler, clock_data, CHG_TEMP_1, 0);
LcdTempUpdater temp2_updater(lcdHandler, clock_data, CHG_TEMP_2, 1);
LcdTempUpdater temp3_updater(lcdHandler, clock_data, CHG_TEMP_3, 2);

InputButton btnMode(clock_data, GPIO_BTN_MODE);
InputButton btnDown(clock_data, GPIO_BTN_DOWN);
InputButton btnUp(clock_data, GPIO_BTN_UP);
InputButton btnSet(clock_data, GPIO_BTN_SET);
InputButton btnClockType(clock_data, GPIO_BTN_CLOCK_TYPE);
InputButton btnSnooze1(clock_data, GPIO_BTN_SNOOZE_1);
InputButton btnSnooze2(clock_data, GPIO_BTN_SNOOZE_2);

struct ButtonDef
{
    InputButton* m_button;
    //int m_gpio_id;
    //unsigned int m_change_bit;
    void (ClockData::*Action1)();
    void (ClockData::*Action2)();
    void (ClockData::*Action3)();
    void (ClockData::*Action4)();
};

struct LcdUpdaterDef
{
    LcdUpdater* m_lcdUpdater;
    unsigned int m_line;
    unsigned int m_col;
};

struct ClockMode
{
    std::vector<ButtonDef> m_buttons;
    std::vector<LcdUpdaterDef> m_lcdUpdaters;
};

//key = {clock type, clock mode}
//value = full clock config
std::map<std::pair<unsigned int, unsigned int>, ClockMode> clockConfigMap =
{
    {
        {0, 0},
        {
            {
                {&btnMode, &ClockData::ChangeMode, nullptr, nullptr, nullptr},
                {&btnDown, nullptr, nullptr, nullptr, nullptr},
                {&btnUp, nullptr, nullptr, nullptr, nullptr},
                {&btnSet, nullptr, nullptr, nullptr, nullptr},
                {&btnClockType, &ClockData::ChangeClockType, nullptr, nullptr, nullptr},
                {&btnSnooze1, nullptr, nullptr, nullptr, nullptr},
                {&btnSnooze2, nullptr, nullptr, nullptr, nullptr}
            },
            {
                {&time_updater, 0, 3},
                {&date_updater, 2, 3},
                {&temp1_updater, 3, 0},
                {&temp2_updater, 3, 7},
                {&temp3_updater, 3, 14}
            }
        }
    },
    {
        {0, 1},
        {
            {
                {&btnMode, &ClockData::ChangeMode, nullptr, nullptr, nullptr},
                {&btnDown, &ClockData::ChangeAlarmHourDown, &ClockData::ChangeAlarmHourDown, nullptr, nullptr},
                {&btnUp, &ClockData::ChangeAlarmHourUp, &ClockData::ChangeAlarmHourUp, nullptr, nullptr},
                {&btnSet, &ClockData::ChangeAlarmSet, nullptr, nullptr, nullptr},
                {&btnClockType, &ClockData::ChangeClockType, nullptr, nullptr, nullptr},
                {&btnSnooze1, nullptr, nullptr, nullptr, nullptr},
                {&btnSnooze2, nullptr, nullptr, nullptr, nullptr}
            },
            {
                {&time_updater, 0, 3},
                {&alarm_updater, 2, 3},
                {&temp1_updater, 3, 0},
                {&temp2_updater, 3, 7},
                {&temp3_updater, 3, 14}
            }
        }
    },
    {
        {0, 2},
        {
            {
                {&btnMode, &ClockData::ChangeMode, nullptr, nullptr, nullptr},
                {&btnDown, &ClockData::ChangeStoperStartStop, nullptr, nullptr, nullptr},
                {&btnUp, &ClockData::ChangeStoperStartStop, nullptr, nullptr, nullptr},
                {&btnSet, &ClockData::ChangeStoperSplitReset, nullptr, nullptr, nullptr},
                {&btnClockType, &ClockData::ChangeClockType, nullptr, nullptr, nullptr},
                {&btnSnooze1, &ClockData::ChangeStoperStartStop, nullptr, nullptr, nullptr},
                {&btnSnooze2, &ClockData::ChangeStoperStartStop, nullptr, nullptr, nullptr},
            },
            {
                {&time_updater, 0, 3},
                {&stoper_updater, 2, 3},
                {&temp1_updater, 3, 0},
                {&temp2_updater, 3, 7},
                {&temp3_updater, 3, 14}
            }
        }
    },
    {
        {0, 3},
        {
            {
                {&btnMode, &ClockData::ChangeMode, nullptr, nullptr, nullptr},
                {&btnDown, &ClockData::ChangeTimerStartStopDown, &ClockData::ChangeTimerStartStopDown, nullptr, nullptr},
                {&btnUp, &ClockData::ChangeTimerStartStopUp, &ClockData::ChangeTimerStartStopUp, nullptr, nullptr},
                {&btnSet, &ClockData::ChangeTimerSetReset, nullptr, nullptr, nullptr},
                {&btnClockType, &ClockData::ChangeClockType, nullptr, nullptr, nullptr},
                {&btnSnooze1, &ClockData::ChangeTimerStartStop, nullptr, nullptr, nullptr},
                {&btnSnooze2, &ClockData::ChangeTimerStartStop, nullptr, nullptr, nullptr},            },
            {
                {&time_updater, 0, 3},
                {&timer_updater, 2, 3},
                {&temp1_updater, 3, 0},
                {&temp2_updater, 3, 7},
                {&temp3_updater, 3, 14}
            }
        }
    },
    {
        {1, 0},
        {
            {
                {&btnMode, &ClockData::ChangeMode, nullptr, nullptr, nullptr},
                {&btnDown, nullptr, nullptr, nullptr, nullptr},
                {&btnUp, nullptr, nullptr, nullptr, nullptr},
                {&btnSet, nullptr, nullptr, nullptr, nullptr},
                {&btnClockType, &ClockData::ChangeClockType, nullptr, nullptr, nullptr},
                {&btnSnooze1, nullptr, nullptr, nullptr, nullptr},
                {&btnSnooze2, nullptr, nullptr, nullptr, nullptr},            },
            {
                {&temp1_updater, 0, 14},
                {&temp2_updater, 1, 14},
                {&temp3_updater, 2, 14},
                {&timeSmall_updater, 3, 12},
            }
        }
    },
    {
        {1, 1},
        {
            {
                {&btnMode, &ClockData::ChangeMode, nullptr, nullptr, nullptr},
                {&btnDown, nullptr, nullptr, nullptr, nullptr},
                {&btnUp, nullptr, nullptr, nullptr, nullptr},
                {&btnSet, nullptr, nullptr, nullptr, nullptr},
                {&btnClockType, &ClockData::ChangeClockType, nullptr, nullptr, nullptr},
                {&btnSnooze1, nullptr, nullptr, nullptr, nullptr},
                {&btnSnooze2, nullptr, nullptr, nullptr, nullptr},            },
            {
                {&timeSmall_updater, 0, 6},
            }
        }
    },
    {
        {1, 2},
        {
            {
                {&btnMode, &ClockData::ChangeMode, nullptr, nullptr, nullptr},
                {&btnDown, nullptr, nullptr, nullptr, nullptr},
                {&btnUp, nullptr, nullptr, nullptr, nullptr},
                {&btnSet, nullptr, nullptr, nullptr, nullptr},
                {&btnClockType, &ClockData::ChangeClockType, nullptr, nullptr, nullptr},
                {&btnSnooze1, nullptr, nullptr, nullptr, nullptr},
                {&btnSnooze2, nullptr, nullptr, nullptr, nullptr},            },
            {
                {&timeSmall_updater, 3, 6},
            }
        }
    },
    {
        {2, 0},
        {
            {
                {&btnMode, &ClockData::ChangeMode, nullptr, nullptr, nullptr},
                {&btnDown, &ClockData::ChangeLogTimerDown, &ClockData::ChangeLogTimerDown, nullptr, nullptr},
                {&btnUp, &ClockData::ChangeLogTimerUp, &ClockData::ChangeLogTimerUp, nullptr, nullptr},
                {&btnSet, &ClockData::ChangeLogTimerSetReset, nullptr, nullptr, nullptr},
                {&btnClockType, &ClockData::ChangeClockType, nullptr, nullptr, nullptr},
                {&btnSnooze1, &ClockData::ChangeLogTimerStartStop, nullptr, nullptr, nullptr},
                {&btnSnooze2, &ClockData::ChangeLogTimerStartStop, nullptr, nullptr, nullptr},            },
            {
                {&log_timer_updater, 0, 4},
                {&timeSmall_updater, 3, 6},
            }
        }
    },
    {
        {2, 1},
        {
            {
                {&btnMode, &ClockData::ChangeMode, nullptr, nullptr, nullptr},
                {&btnDown, nullptr, nullptr, nullptr, nullptr},
                {&btnUp, nullptr, nullptr, nullptr, nullptr},
                {&btnSet, nullptr, nullptr, nullptr, nullptr},
                {&btnClockType, &ClockData::ChangeClockType, nullptr, nullptr, nullptr},
                {&btnSnooze1, nullptr, nullptr, nullptr, nullptr},
                {&btnSnooze2, nullptr, nullptr, nullptr, nullptr},            },
            {
                {&timeSmall_updater, 0, 6},
            }
        }
    },
};

/*
ButtonDef arrButtonData[] {
    //regular clock
    //DateTime Mode
    {GPIO_BTN_MODE, &ClockData::ChangeMode, nullptr, nullptr, nullptr},
    {GPIO_BTN_DOWN, nullptr, nullptr, nullptr, nullptr},
    {GPIO_BTN_UP, nullptr, nullptr, nullptr, nullptr},
    {GPIO_BTN_SET, nullptr, nullptr, nullptr, nullptr},
    {GPIO_BTN_CLOCK_TYPE, &ClockData::ChangeClockType, nullptr, nullptr, nullptr},
    {GPIO_BTN_SNOOZE_1, nullptr, nullptr, nullptr, nullptr},
    {GPIO_BTN_SNOOZE_2, nullptr, nullptr, nullptr, nullptr},
    //Alarm Mode
    {GPIO_BTN_MODE, &ClockData::ChangeMode, nullptr, nullptr, nullptr},
    {GPIO_BTN_DOWN, &ClockData::ChangeAlarmHourDown, &ClockData::ChangeAlarmHourDown, nullptr, nullptr},
    {GPIO_BTN_UP, &ClockData::ChangeAlarmHourUp, &ClockData::ChangeAlarmHourUp, nullptr, nullptr},
    {GPIO_BTN_SET, &ClockData::ChangeAlarmSet, nullptr, nullptr, nullptr},
    {GPIO_BTN_CLOCK_TYPE, &ClockData::ChangeClockType, nullptr, nullptr, nullptr},
    {GPIO_BTN_SNOOZE_1, nullptr, nullptr, nullptr, nullptr},
    {GPIO_BTN_SNOOZE_2, nullptr, nullptr, nullptr, nullptr},
    //Stoper Mode
    {GPIO_BTN_MODE, &ClockData::ChangeMode, nullptr, nullptr, nullptr},
    {GPIO_BTN_DOWN, &ClockData::ChangeStoperStartStop, nullptr, nullptr, nullptr},
    {GPIO_BTN_UP, &ClockData::ChangeStoperStartStop, nullptr, nullptr, nullptr},
    {GPIO_BTN_SET, &ClockData::ChangeStoperSplitReset, nullptr, nullptr, nullptr},
    {GPIO_BTN_CLOCK_TYPE, &ClockData::ChangeClockType, nullptr, nullptr, nullptr},
    {GPIO_BTN_SNOOZE_1, &ClockData::ChangeStoperStartStop, nullptr, nullptr, nullptr},
    {GPIO_BTN_SNOOZE_2, &ClockData::ChangeStoperStartStop, nullptr, nullptr, nullptr},
    //TimerMode
    {GPIO_BTN_MODE, &ClockData::ChangeMode, nullptr, nullptr, nullptr},
    {GPIO_BTN_DOWN, &ClockData::ChangeTimerStartStopDown, &ClockData::ChangeTimerStartStopDown, nullptr, nullptr},
    {GPIO_BTN_UP, &ClockData::ChangeTimerStartStopUp, &ClockData::ChangeTimerStartStopUp, nullptr, nullptr},
    {GPIO_BTN_SET, &ClockData::ChangeTimerSetReset, nullptr, nullptr, nullptr},
    {GPIO_BTN_CLOCK_TYPE, &ClockData::ChangeClockType, nullptr, nullptr, nullptr},
    {GPIO_BTN_SNOOZE_1, &ClockData::ChangeTimerStartStop, nullptr, nullptr, nullptr},
    {GPIO_BTN_SNOOZE_2, &ClockData::ChangeTimerStartStop, nullptr, nullptr, nullptr},
    
    //darkroom multitimer (film development)
    //timer 1
    {GPIO_BTN_MODE, &ClockData::ChangeMode, nullptr, nullptr, nullptr},
    {GPIO_BTN_DOWN, nullptr, nullptr, nullptr, nullptr},
    {GPIO_BTN_UP, nullptr, nullptr, nullptr, nullptr},
    {GPIO_BTN_SET, nullptr, nullptr, nullptr, nullptr},
    {GPIO_BTN_CLOCK_TYPE, &ClockData::ChangeClockType, nullptr, nullptr, nullptr},
    {GPIO_BTN_SNOOZE_1, nullptr, nullptr, nullptr, nullptr},
    {GPIO_BTN_SNOOZE_2, nullptr, nullptr, nullptr, nullptr},
    //timer 2
    {GPIO_BTN_MODE, &ClockData::ChangeMode, nullptr, nullptr, nullptr},
    {GPIO_BTN_DOWN, nullptr, nullptr, nullptr, nullptr},
    {GPIO_BTN_UP, nullptr, nullptr, nullptr, nullptr},
    {GPIO_BTN_SET, nullptr, nullptr, nullptr, nullptr},
    {GPIO_BTN_CLOCK_TYPE, &ClockData::ChangeClockType, nullptr, nullptr, nullptr},
    {GPIO_BTN_SNOOZE_1, nullptr, nullptr, nullptr, nullptr},
    {GPIO_BTN_SNOOZE_2, nullptr, nullptr, nullptr, nullptr},
    //timer 3
    {GPIO_BTN_MODE, &ClockData::ChangeMode, nullptr, nullptr, nullptr},
    {GPIO_BTN_DOWN, nullptr, nullptr, nullptr, nullptr},
    {GPIO_BTN_UP, nullptr, nullptr, nullptr, nullptr},
    {GPIO_BTN_SET, nullptr, nullptr, nullptr, nullptr},
    {GPIO_BTN_CLOCK_TYPE, &ClockData::ChangeClockType, nullptr, nullptr, nullptr},
    {GPIO_BTN_SNOOZE_1, nullptr, nullptr, nullptr, nullptr},
    {GPIO_BTN_SNOOZE_2, nullptr, nullptr, nullptr, nullptr},
    
    //darkroom timer (enlarger timer)
    //...

};
 */

//regular
unsigned char char_1[] = {0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01};
unsigned char char_2[] = {0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
unsigned char char_3[] = {0x1f, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01};
unsigned char char_4[] = {0x1f, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x1f};
unsigned char char_5[] = {0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x1f};
unsigned char char_6[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f};
unsigned char char_7[] = {0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f};

//bold
/*
unsigned char char_1[] = {0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03};
unsigned char char_2[] = {0x1f, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
unsigned char char_3[] = {0x1f, 0x1f, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03};
unsigned char char_4[] = {0x1f, 0x1f, 0x03, 0x03, 0x03, 0x03, 0x1f, 0x1f};
unsigned char char_5[] = {0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x1f, 0x1f};
unsigned char char_6[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0x1f};
unsigned char char_7[] = {0x1f, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x1f, 0x1f};
*/

void DefineCustomChars(int a_lcdHandle)
{
    lcdCharDef(a_lcdHandle, 0, char_1);
    lcdCharDef(a_lcdHandle, 1, char_2);
    lcdCharDef(a_lcdHandle, 2, char_3);
    lcdCharDef(a_lcdHandle, 3, char_4);
    lcdCharDef(a_lcdHandle, 4, char_5);
    lcdCharDef(a_lcdHandle, 5, char_6);
    lcdCharDef(a_lcdHandle, 6, char_7);
}

void ReadTempThread(ClockData &a_data)
{
    ifstream fileTemp1("//sys//bus//w1//devices//28-0115a32491ff//w1_slave");
    ifstream fileTemp2("//sys//bus//w1//devices//28-0000075b9964//w1_slave");
    ifstream fileTemp3("//sys//bus//w1//devices//28-00000758c102//w1_slave");
    string strLine;
    int newVal = 0;
    
    while(true)
    {
        fileTemp1.seekg(0);
        getline(fileTemp1, strLine);
        getline(fileTemp1, strLine);
        string strTemp1(strLine, strLine.find("t=") + 2);
        newVal = stoi(strTemp1);
        a_data.SetTemp(0, newVal);
        fileTemp2.seekg(0);
        getline(fileTemp2, strLine);
        getline(fileTemp2, strLine);
        string strTemp2(strLine, strLine.find("t=") + 2);
        newVal = stoi(strTemp2);
        a_data.SetTemp(1, newVal);
        fileTemp3.seekg(0);
        getline(fileTemp3, strLine);
        getline(fileTemp3, strLine);
        string strTemp3(strLine, strLine.find("t=") + 2);
        newVal = stoi(strTemp3);
        a_data.SetTemp(2, newVal);
        this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}

int main(void)
{
    lcdHandler = initHardware();
    if(lcdHandler == -1)
    {
        cout << "Init hardware failed! Exiting!" << endl;
    }
    
    DefineCustomChars(lcdHandler);
    
    system_clock::time_point now;
    
//    std::vector<InputButton> vecButtons;
//    for(int i = 0; i < 7; ++i)
//    {
//        vecButtons.push_back(InputButton(clock_data, arrButtonData[i].m_gpio_id, arrButtonData[i].Action1, arrButtonData[i].Action2, arrButtonData[i].Action3, arrButtonData[i].Action4));
//    }
    
    //init updaters
    //moved to global;
    
    unsigned int updateState = 0;
    
    thread tempReader(ReadTempThread, ref(clock_data));
    
    //for(ButtonDef& btn : clockConfigMap[std::make_pair<unsigned int, unsigned int>(0, 0)].m_buttons)
    //{
    //    btn.m_button->Init(btn.Action1, btn.Action2, btn.Action3, btn.Action4);
    //}
    
    //vector<LcdUpdater*> vecUpdaters;
    //vecUpdaters.push_back(&time_updater);
    //vecUpdaters.push_back(&date_updater);
    //vecUpdaters.push_back(&temp1_updater);
    //vecUpdaters.push_back(&temp2_updater);
    //vecUpdaters.push_back(&temp3_updater);
    
    //main program loop
    while(true)
    {
        now = system_clock::now();
        
        for(ButtonDef& btn : /*vecButtons*/clockConfigMap[std::make_pair<unsigned int, unsigned int>(clock_data.GetClockType(), clock_data.GetClockMode())].m_buttons)
        {
            btn.m_button->CheckChanges(now);
        }
        
        updateState = clock_data.GetState();
        
        
        digitalWrite(12, clock_data.GetSound() ? 1 : 0);
        
        //show update state
        //lcdPosition(lcdHandler, 16, 0);
        //lcdPrintf(lcdHandler, "%4u", updateState);
        
        if(updateState & CHG_MODE || updateState & CHG_CLOCK_TYPE)
        {
            //clean prev data
            //for(LcdUpdaterDef& pUpd: clockConfigMap[make_pair<unsigned int, unsigned int>(clock_data.GetPrevClockType(), clock_data.GetPrevClockMode())].m_lcdUpdaters)
            //{
            //    pUpd.m_lcdUpdater->Clean();
            //}
            //reinitialize + clean whole screen
            //lcdHandler = initHardware(); //doesn't work (crashes after few changes)
            lcdClear(lcdHandler);
            lcdCursor(lcdHandler, false);
            lcdCursorBlink(lcdHandler, false);
            DefineCustomChars(lcdHandler);
            //lcdDisplay(lcdHandler, false); //turns off the lcd
            
            //show current type&mode
            lcdPosition(lcdHandler, 0, 0);
            lcdPrintf(lcdHandler, "%1u%1u", clock_data.GetClockType(), clock_data.GetClockMode());

        
            //set actions
            for(ButtonDef& btn : clockConfigMap[std::make_pair<unsigned int, unsigned int>(clock_data.GetClockType(), clock_data.GetClockMode())].m_buttons)
            {
                btn.m_button->Init(btn.Action1, btn.Action2, btn.Action3, btn.Action4);
            }
            //move lcdupdaters
            
            //int start = clock_data.GetClockMode()*7;
            //for(int i = 0; i < 7; ++i)
            //{
            //    vecButtons[i].Action1 = arrButtonData[start+i].Action1;
            //    vecButtons[i].Action2 = arrButtonData[start+i].Action2;
            //    vecButtons[i].Action3 = arrButtonData[start+i].Action3;
            //    vecButtons[i].Action4 = arrButtonData[start+i].Action4;
            //}
            
            //for(LcdUpdater* pUpd : vecUpdaters)
            //{
            //    pUpd->Clean();
            //}
            
            //vecUpdaters.clear();
            /*switch(clock_data.GetClockType())
            {
            case 0:
                switch(clock_data.GetClockMode())
                {
                case 0:
                    vecUpdaters.push_back(&time_updater);
                    vecUpdaters.push_back(&date_updater);
                    vecUpdaters.push_back(&temp1_updater);
                    vecUpdaters.push_back(&temp2_updater);
                    vecUpdaters.push_back(&temp3_updater);
                    break;
                case 1:
                    vecUpdaters.push_back(&time_updater);
                    vecUpdaters.push_back(&alarm_updater);
                    vecUpdaters.push_back(&temp1_updater);
                    vecUpdaters.push_back(&temp2_updater);
                    vecUpdaters.push_back(&temp3_updater);
                    break;
                case 2:
                    vecUpdaters.push_back(&time_updater);
                    vecUpdaters.push_back(&stoper_updater);
                    vecUpdaters.push_back(&temp1_updater);
                    vecUpdaters.push_back(&temp2_updater);
                    vecUpdaters.push_back(&temp3_updater);
                    break;
                case 3:
                    vecUpdaters.push_back(&time_updater);
                    vecUpdaters.push_back(&timer_updater);
                    vecUpdaters.push_back(&temp1_updater);
                    vecUpdaters.push_back(&temp2_updater);
                    vecUpdaters.push_back(&temp3_updater);
                    break;
                }
                break;
            case 1:
                switch(clock_data.GetClockMode())
                {
                    case 0:
                        break;
                    case 1:
                        break;
                    case 2:
                        break;
                }
                break;
            }
             */
            for(LcdUpdaterDef& pUpd: /*vecUpdaters*/clockConfigMap[make_pair<unsigned int, unsigned int>(clock_data.GetClockType(), clock_data.GetClockMode())].m_lcdUpdaters)
            {
                pUpd.m_lcdUpdater->Move(pUpd.m_line, pUpd.m_col);
                clock_data.SetStateBit(pUpd.m_lcdUpdater->GetUpdateBit());
            }
            
            clock_data.ClockTypeModeUpdated();
        }
        
        
        updateState = clock_data.CheckChanges(now);
        
        //show clock update state
        //lcdPosition(lcdHandler, 0, 0);
        //lcdPrintf(lcdHandler, "%03u", updateState);

        //show current type&mode
        //lcdPosition(lcdHandler, 0, 0);
        //lcdPrintf(lcdHandler, "%1u%1u", clock_data.GetClockType(), clock_data.GetClockMode());
        
        for(LcdUpdaterDef& pUpd: /*vecUpdaters*/clockConfigMap[make_pair<unsigned int, unsigned int>(clock_data.GetClockType(), clock_data.GetClockMode())].m_lcdUpdaters)
        {
            if(updateState & pUpd.m_lcdUpdater->GetUpdateBit())
            {
                pUpd.m_lcdUpdater->Update();
            }
        }
        this_thread::sleep_for(std::chrono::milliseconds(50));
        
    }
    return 0;
}


