#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(SCD_ALARM_LIB)
#  define SCD_ALARM_EXPORT Q_DECL_EXPORT
# else
#  define SCD_ALARM_EXPORT Q_DECL_IMPORT
# endif
#else
# define SCD_ALARM_EXPORT
#endif
