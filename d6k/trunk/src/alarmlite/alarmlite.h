#pragma once
#include "scd_alarm_global.h"
class QWidget;

extern "C"
{
	SCD_ALARM_EXPORT int  StartAlarmWin(const QString& pszDataPath);
	SCD_ALARM_EXPORT QWidget* GetAlarmMainWindow(const QString& pszDataPath);
	SCD_ALARM_EXPORT void StopAlarmWin();
}