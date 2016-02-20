#include "LcdTimeUpdater.h"
#include <lcd.h>



void LcdTimeUpdater::Update()
{
    lcdPosition(m_lcdHandle, m_col, m_line);
    tm dateTime = m_data.GetTime();
    lcdPrintf(m_lcdHandle, "%2u%c%02u%c%02u"
              , dateTime.tm_hour
              , dateTime.tm_sec%2 ? ' ' : ':'
              , dateTime.tm_min
              , dateTime.tm_sec%2 ? ' ' : ':'
              , dateTime.tm_sec);
    
    /*
    //custom font test
    //1t
    lcdPutchar(m_lcdHandle, ' ');
    lcdPutchar(m_lcdHandle, 0x00);
    //2t
    lcdPutchar(m_lcdHandle, ' ');
    lcdPutchar(m_lcdHandle, 0x03);
    //3t
    lcdPutchar(m_lcdHandle, ' ');
    lcdPutchar(m_lcdHandle, 0x03);
    //4t
    lcdPutchar(m_lcdHandle, 0x00);
    lcdPutchar(m_lcdHandle, 0x04);
    //5t
    lcdPutchar(m_lcdHandle, 0x00);
    lcdPutchar(m_lcdHandle, 0x06);
    //6t
    lcdPutchar(m_lcdHandle, 0x00);
    lcdPutchar(m_lcdHandle, 0x06);
    //7t
    lcdPutchar(m_lcdHandle, 0x00);
    lcdPutchar(m_lcdHandle, 0x02);
    //8t
    lcdPutchar(m_lcdHandle, 0x00);
    lcdPutchar(m_lcdHandle, 0x03);
    //9t
    lcdPutchar(m_lcdHandle, 0x00);
    lcdPutchar(m_lcdHandle, 0x03);
    //0t
    lcdPutchar(m_lcdHandle, 0x00);
    lcdPutchar(m_lcdHandle, 0x02);


    lcdPosition(m_lcdHandle, m_col, m_line+1);
    //1b
    lcdPutchar(m_lcdHandle, ' ');
    lcdPutchar(m_lcdHandle, 0x00);
    //2b
    lcdPutchar(m_lcdHandle, 0x00);
    lcdPutchar(m_lcdHandle, 0x05);
    //3b
    lcdPutchar(m_lcdHandle, ' ');
    lcdPutchar(m_lcdHandle, 0x04);
    //4b
    lcdPutchar(m_lcdHandle, ' ');
    lcdPutchar(m_lcdHandle, 0x00);
    //5b
    lcdPutchar(m_lcdHandle, ' ');
    lcdPutchar(m_lcdHandle, 0x04);
    //6b
    lcdPutchar(m_lcdHandle, 0x00);
    lcdPutchar(m_lcdHandle, 0x04);
    //7b
    lcdPutchar(m_lcdHandle, ' ');
    lcdPutchar(m_lcdHandle, 0x00);
    //8b
    lcdPutchar(m_lcdHandle, 0x00);
    lcdPutchar(m_lcdHandle, 0x04);
    //9b
    lcdPutchar(m_lcdHandle, ' ');
    lcdPutchar(m_lcdHandle, 0x04);
    //0b
    lcdPutchar(m_lcdHandle, 0x00);
    lcdPutchar(m_lcdHandle, 0x04);
*/
}

void LcdTimeUpdater::Clean()
{
    lcdPosition(m_lcdHandle, m_col, m_line);
    lcdPrintf(m_lcdHandle, "        ");
}
