#ifndef ALARMWIN_H
#define ALARMWIN_H

#include "alarmwin_global.h"
class QWidget;

extern "C"
{
	ALARMWIN_EXPORT int  StartAlarmWin(const QString& pszDataPath);
	ALARMWIN_EXPORT QWidget* GetAlarmMainWindow(const QString& pszDataPath);
	ALARMWIN_EXPORT void StopAlarmWin();
}

#endif // ALARMWIN_H
