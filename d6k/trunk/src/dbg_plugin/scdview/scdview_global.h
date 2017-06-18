#ifndef SCDVIEW_GLOBAL_H
#define SCDVIEW_GLOBAL_H

#include <QtCore/qglobal.h>

#ifdef SCDVIEW_LIB
# define SCDVIEW_EXPORT Q_DECL_EXPORT
#else
# define SCDVIEW_EXPORT Q_DECL_IMPORT
#endif

#endif // SCDVIEW_GLOBAL_H
