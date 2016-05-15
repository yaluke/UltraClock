#ifndef _INPUTBUTTON_H_
#define _INPUTBUTTON_H_

#include "Input.h"

#define BTN_MODE_IDX        0
#define BTN_DOWN_IDX        1 //+start/stop
#define BTN_UP_IDX          2 //+start/stop
#define BTN_SET_IDX         3
#define BTN_CLOCK_TYPE_IDX  4
#define BTN_SNOOZE_1_IDX    5
#define BTN_SNOOZE_2_IDX    6 //+start/stop +start chess 1
#define NO_OF_BUTTONS       7 //+start/stop +start chess 2

#define GPIO_BTN_MODE       26
#define GPIO_BTN_DOWN       19
#define GPIO_BTN_UP         13
#define GPIO_BTN_SET        6
#define GPIO_BTN_CLOCK_TYPE 5
#define GPIO_BTN_SNOOZE_1   22
#define GPIO_BTN_SNOOZE_2   27

#define BTN_NONE        0
#define BTN_PRESSED     1
#define BTN_LPRESSED    2
//#define BTN_VLPRESSED   4

#define LONG_PRESS_TIME         700
//#define VERY_LONG_PRESS_TIME    1500
#define LP_CHANGE_TIME 100

enum class eBtnState
{
    None = 0,        //0000b
    Pressed = 1,     //0001b
    LPressed = 3,    //0011b
//    VLPressed = 7    //0111b
};

class InputButton : public Input
{
public:
    InputButton(ClockData& a_data, int a_gpio_id, void (ClockData::*a_Action1)() = nullptr, void (ClockData::*a_Action2)() = nullptr, void (ClockData::*a_Action3)() = nullptr, void (ClockData::*a_Action4)() = nullptr)
    : Input(a_data, a_Action1, a_Action2, a_Action3, a_Action4), m_gpio_id(a_gpio_id)
    {
        
    }
    
    void Init(void (ClockData::*a_Action1)(), void (ClockData::*a_Action2)(), void (ClockData::*a_Action3)(), void (ClockData::*a_Action4)())
    {
        Action1 = a_Action1;
        Action2 = a_Action2;
        Action3 = a_Action3;
        Action4 = a_Action4;
    }
    
    virtual void CheckChanges(std::chrono::system_clock::time_point a_now);
    
    eBtnState m_state = eBtnState::None;
    std::chrono::system_clock::time_point m_pressed_time;
    std::chrono::system_clock::time_point m_lp_last_change;
    int m_gpio_id = 0;
};


#endif