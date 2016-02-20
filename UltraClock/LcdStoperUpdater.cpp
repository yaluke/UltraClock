//
//  LcdStoperUpdater.cpp
//  UltraClock
//
//  Created by Lukasz Ronka on 29/01/16.
//  Copyright © 2016 Lukasz Ronka. All rights reserved.
//

#include "LcdStoperUpdater.h"
#include <lcd.h>

void LcdStoperUpdater::Update()
{
    lcdPosition(m_lcdHandle, m_col, m_line);
    char chColon = m_data.GetStoperShowColons() ? ':' : ' ';
    ClockData::StoperData stoperData = m_data.GetStoperData();
    lcdPrintf(m_lcdHandle, "ST %2u%c%02u%c%02u.%u", stoperData.m_timeStoperHour, chColon, stoperData.m_timeStoperMinute, chColon, stoperData.m_timeStoperSecond, stoperData.m_timeStoperDeci);
}

void LcdStoperUpdater::Clean()
{
    lcdPosition(m_lcdHandle, m_col, m_line);
    lcdPrintf(m_lcdHandle, "             ");
}