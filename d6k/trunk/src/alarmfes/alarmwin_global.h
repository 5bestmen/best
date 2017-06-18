#ifndef ALARMWIN_GLOBAL_H
#define ALARMWIN_GLOBAL_H

#include <QtCore/qglobal.h>

#ifdef ALARMWIN_LIB
# define ALARMWIN_EXPORT Q_DECL_EXPORT
#else
# define ALARMWIN_EXPORT Q_DECL_IMPORT
#endif

#endif // ALARMWIN_GLOBAL_H
