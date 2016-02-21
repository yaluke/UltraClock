#include "LcdTimeUpdater.h"
#include <lcd.h>
#include <string>

using namespace std;


unsigned char numbers[10][4] = {{0x00, 0x02, 0x00, 0x04},   //0
                                {0xa0, 0x00, 0xa0, 0x00},   //1
                                {0xa0, 0x03, 0x00, 0x05},   //2
                                {0xa0, 0x03, 0xa0, 0x04},   //3
                                {0x00, 0x04, 0xa0, 0x00},   //4
                                {0x00, 0x06, 0xa0, 0x04},   //5
                                {0x00, 0x06, 0x00, 0x04},   //6
                                {0x00, 0x02, 0xa0, 0x00},   //7
                                {0x00, 0x03, 0x00, 0x04},   //8
                                {0x00, 0x03, 0xa0, 0x04},   //9
                        };

void LcdTimeUpdater::Update()
{
    lcdPosition(m_lcdHandle, m_col, m_line);
    tm dateTime = m_data.GetTime();
    
    lcdPosition(m_lcdHandle, m_col, m_line);
    
    //hour top line
    lcdPutchar(m_lcdHandle, dateTime.tm_hour/10 ? numbers[dateTime.tm_hour/10][0] : ' ');
    lcdPutchar(m_lcdHandle, dateTime.tm_hour/10 ? numbers[dateTime.tm_hour/10][1] : ' ');
    lcdPutchar(m_lcdHandle, numbers[dateTime.tm_hour%10][0]);
    lcdPutchar(m_lcdHandle, numbers[dateTime.tm_hour%10][1]);
    
    lcdPutchar(m_lcdHandle, dateTime.tm_sec%2 ? ' ' : 0xa5);
    
    //minute top line
    lcdPutchar(m_lcdHandle, numbers[dateTime.tm_min/10][0]);
    lcdPutchar(m_lcdHandle, numbers[dateTime.tm_min/10][1]);
    lcdPutchar(m_lcdHandle, numbers[dateTime.tm_min%10][0]);
    lcdPutchar(m_lcdHandle, numbers[dateTime.tm_min%10][1]);

    lcdPutchar(m_lcdHandle, dateTime.tm_sec%2 ? ' ' : 0xa5);

    //second top line
    lcdPutchar(m_lcdHandle, numbers[dateTime.tm_sec/10][0]);
    lcdPutchar(m_lcdHandle, numbers[dateTime.tm_sec/10][1]);
    lcdPutchar(m_lcdHandle, numbers[dateTime.tm_sec%10][0]);
    lcdPutchar(m_lcdHandle, numbers[dateTime.tm_sec%10][1]);


    lcdPosition(m_lcdHandle, m_col, m_line+1);

    //hour bottom line
    lcdPutchar(m_lcdHandle, dateTime.tm_hour/10 ? numbers[dateTime.tm_hour/10][2] : ' ');
    lcdPutchar(m_lcdHandle, dateTime.tm_hour/10 ? numbers[dateTime.tm_hour/10][3] : ' ');
    lcdPutchar(m_lcdHandle, numbers[dateTime.tm_hour%10][2]);
    lcdPutchar(m_lcdHandle, numbers[dateTime.tm_hour%10][3]);

    lcdPutchar(m_lcdHandle, dateTime.tm_sec%2 ? ' ' : 0xa5);
    
    //minute bottom line
    lcdPutchar(m_lcdHandle, numbers[dateTime.tm_min/10][2]);
    lcdPutchar(m_lcdHandle, numbers[dateTime.tm_min/10][3]);
    lcdPutchar(m_lcdHandle, numbers[dateTime.tm_min%10][2]);
    lcdPutchar(m_lcdHandle, numbers[dateTime.tm_min%10][3]);

    lcdPutchar(m_lcdHandle, dateTime.tm_sec%2 ? ' ' : 0xa5);
    
    //second bottom line
    lcdPutchar(m_lcdHandle, numbers[dateTime.tm_sec/10][2]);
    lcdPutchar(m_lcdHandle, numbers[dateTime.tm_sec/10][3]);
    lcdPutchar(m_lcdHandle, numbers[dateTime.tm_sec%10][2]);
    lcdPutchar(m_lcdHandle, numbers[dateTime.tm_sec%10][3]);

/*
    lcdPrintf(m_lcdHandle, "%2u%c%02u%c%02u"
              , dateTime.tm_hour
              , dateTime.tm_sec%2 ? ' ' : ':'
              , dateTime.tm_min
              , dateTime.tm_sec%2 ? ' ' : ':'
              , dateTime.tm_sec);
    */
    
    
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
    lcdPrintf(m_lcdHandle, "              ");
    lcdPosition(m_lcdHandle, m_col, m_line + 1);
    lcdPrintf(m_lcdHandle, "              ");
}
