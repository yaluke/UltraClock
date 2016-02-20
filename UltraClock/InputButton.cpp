#include "InputButton.h"

#include <wiringPi.h>

using namespace std;
using namespace std::chrono;

void InputButton::CheckChanges(std::chrono::system_clock::time_point a_now)
{
    if(!digitalRead(m_gpio_id))
    {
        if(m_state == eBtnState::None)
        {
            m_state = eBtnState::Pressed;
            m_pressed_time = a_now;
            if(Action1)
            {
                m_data.StartSound(a_now, eSoundType::eBip);
                (m_data.*Action1)();
            }
        }
        else if( m_state == eBtnState::Pressed)
        {
            duration<double, milli> dur;
            dur = a_now - m_pressed_time;
            if(dur.count() >= LONG_PRESS_TIME)
            {
                m_state = eBtnState::LPressed;
                m_lp_last_change = a_now;
                if(Action2)
                    (m_data.*Action2)();
            }
        }
        else //m_state == eBtnState::LPressed
        {
            duration<double, milli> dur;
            dur = a_now - m_lp_last_change;
            if(dur.count() >= LP_CHANGE_TIME)
            {
                m_lp_last_change = a_now;
                if(Action2)
                    (m_data.*Action2)();
            }
        }
        /*else if(m_state != eBtnState::VLPressed)
        {
            duration<double, milli> dur;
            dur = a_now - m_pressed_time;
            if(dur.count() >= LONG_PRESS_TIME && dur.count() < VERY_LONG_PRESS_TIME)
            {
                m_state = eBtnState::LPressed;
                //m_change_state |= m_change_bit;
                if(Action2)
                    (m_data.*Action2)();
            }
            else if( dur.count() >= VERY_LONG_PRESS_TIME)
            {
                m_state = eBtnState::VLPressed;
                //m_change_state |= m_change_bit;
                if(Action3)
                    (m_data.*Action3)();
            }
        }*/
    }
    else if(m_state != eBtnState::None)
    {
        m_state = eBtnState::None;
        //m_change_state |= m_change_bit;
        if(Action4)
            (m_data.*Action4)();
    }
}
