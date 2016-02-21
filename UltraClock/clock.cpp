#include <wiringPi.h>
#include <lcd.h>
#include <iostream>
#include <chrono>
#include <thread>
#include <vector>
#include <fstream>
#include "ClockData.h"
#include "InputButton.h"
#include "LcdTimeUpdater.h"
#include "LcdDateUpdater.h"
#include "LcdAlarmUpdater.h"
#include "LcdStoperUpdater.h"
#include "LcdTimerUpdater.h"
#include "LcdTemp1Updater.h"
#include "LcdTemp2Updater.h"
#include "LcdTemp3Updater.h"

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
    pinMode(12, OUTPUT);
    
    return fd;
}

struct ButtonDef
{
    int m_gpio_id;
    //unsigned int m_change_bit;
    void (ClockData::*Action1)();
    void (ClockData::*Action2)();
    void (ClockData::*Action3)();
    void (ClockData::*Action4)();
};

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
    
    //darkroom multitimer
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

};

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
unsigned char char_1[] = {0x01, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03};
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
        a_data.SetTemp1(newVal);
        fileTemp2.seekg(0);
        getline(fileTemp2, strLine);
        getline(fileTemp2, strLine);
        string strTemp2(strLine, strLine.find("t=") + 2);
        newVal = stoi(strTemp2);
        a_data.SetTemp2(newVal);
        fileTemp3.seekg(0);
        getline(fileTemp3, strLine);
        getline(fileTemp3, strLine);
        string strTemp3(strLine, strLine.find("t=") + 2);
        newVal = stoi(strTemp3);
        a_data.SetTemp3(newVal);
        this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}

int main(void)
{
    int lcdHandler = initHardware();
    if(lcdHandler == -1)
    {
        cout << "Init hardware failed! Exiting!" << endl;
    }
    
    DefineCustomChars(lcdHandler);
    
    system_clock::time_point now;
    
    ClockData clock;
    
    std::vector<InputButton> vecButtons;
    for(int i = 0; i < 7; ++i)
    {
        vecButtons.push_back(InputButton(clock, arrButtonData[i].m_gpio_id, arrButtonData[i].Action1, arrButtonData[i].Action2, arrButtonData[i].Action3, arrButtonData[i].Action4));
    }
    
    //init updaters
    LcdTimeUpdater time_updater(lcdHandler, clock, CHG_TIME, 0, 3);
    LcdDateUpdater date_updater(lcdHandler, clock, CHG_DATE, 2, 3);
    LcdAlarmUpdater alarm_updater(lcdHandler, clock, CHG_ALARM, 2, 3);
    LcdStoperUpdater stoper_updater(lcdHandler, clock, CHG_STOPER, 2, 3);
    LcdTimerUpdater timer_updater(lcdHandler, clock, CHG_TIMER, 2, 3);
    LcdTemp1Updater temp1_updater(lcdHandler, clock, CHG_TEMP_1, 3, 0);
    LcdTemp2Updater temp2_updater(lcdHandler, clock, CHG_TEMP_2, 3, 7);
    LcdTemp3Updater temp3_updater(lcdHandler, clock, CHG_TEMP_3, 3, 14);
    
    unsigned int updateState = 0;
    
    thread tempReader(ReadTempThread, ref(clock));
    
    vector<LcdUpdater*> vecUpdaters;
    vecUpdaters.push_back(&time_updater);
    vecUpdaters.push_back(&date_updater);
    vecUpdaters.push_back(&temp1_updater);
    vecUpdaters.push_back(&temp2_updater);
    vecUpdaters.push_back(&temp3_updater);
    
    //main program loop
    while(true)
    {
        now = system_clock::now();
        
        for(InputButton& btn : vecButtons)
        {
            btn.CheckChanges(now);
        }
        
        updateState = clock.GetState();
        
        digitalWrite(12, clock.GetSound() ? 1 : 0);
        
        //show clock type & mode
        //lcdPosition(lcdHandler, 18, 0);
        //lcdPrintf(lcdHandler, "%u%u", clock.GetClockType(), clock.GetClockMode());
        
        if(updateState & CHG_MODE)
        {
            int start = clock.GetClockMode()*7;
            for(int i = 0; i < 7; ++i)
            {
                vecButtons[i].Action1 = arrButtonData[start+i].Action1;
                vecButtons[i].Action2 = arrButtonData[start+i].Action2;
                vecButtons[i].Action3 = arrButtonData[start+i].Action3;
                vecButtons[i].Action4 = arrButtonData[start+i].Action4;
            }
            
            for(LcdUpdater* pUpd : vecUpdaters)
            {
                pUpd->Clean();
            }
            
            vecUpdaters.clear();
            switch(clock.GetClockMode())
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
            for(LcdUpdater* pUpd: vecUpdaters)
            {
                clock.SetStateBit(pUpd->GetUpdateBit());
            }
        }
        
        
        updateState = clock.CheckChanges(now);
        
        //show clock update state
        //lcdPosition(lcdHandler, 0, 0);
        //lcdPrintf(lcdHandler, "%03u", updateState);

        
        for(LcdUpdater* pUpd: vecUpdaters)
        {
            if(updateState & pUpd->GetUpdateBit())
            {
                pUpd->Update();
            }
        }
        this_thread::sleep_for(std::chrono::milliseconds(50));
        
    }
    return 0;
}


