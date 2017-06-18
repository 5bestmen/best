#ifndef OBJECT_GLOBAL_H
#define OBJECT_GLOBAL_H

#include <QtCore/qglobal.h>

#ifdef OBJECT_LIB
# define OBJECT_EXPORT Q_DECL_EXPORT
#else
# define OBJECT_EXPORT Q_DECL_IMPORT
#endif

#endif // OBJECT_GLOBAL_H
