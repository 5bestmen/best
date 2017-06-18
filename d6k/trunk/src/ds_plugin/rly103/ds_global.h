#ifndef DS_GLOBAL_H
#define DS_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(DS_LIBRARY)
#  define RELAY_SHARED_EXPORT Q_DECL_EXPORT
#else
#  define RELAY_SHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // F220_GLOBAL_H
