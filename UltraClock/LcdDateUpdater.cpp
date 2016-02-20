#include "LcdDateUpdater.h"
#include "lcd.h"
#include <string>

using namespace std;

string dow[] = {"Nd", "Pn", "Wt", "Sr", "Cz", "Pt", "Sb"};
string mth[] = {"Sty", "Lut", "Mar", "Kwi", "Maj", "Cze", "Lip", "Sie", "Wrz", "Paz", "Lis", "Gru"};

void LcdDateUpdater::Update()
{
    lcdPosition(m_lcdHandle, m_col, m_line);
    tm dateTime = m_data.GetDate();
    lcdPrintf(m_lcdHandle, "%s %2u %s %4u"
              , dow[dateTime.tm_wday].c_str()
              , dateTime.tm_mday
              , mth[dateTime.tm_mon].c_str()
              , dateTime.tm_year + 1900);
}

void LcdDateUpdater::Clean()
{
    lcdPosition(m_lcdHandle, m_col, m_line);
    lcdPrintf(m_lcdHandle, "              ");
}