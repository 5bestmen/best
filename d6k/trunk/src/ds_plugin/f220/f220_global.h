#ifndef F220_GLOBAL_H
#define F220_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(F220_LIBRARY)
#  define F220SHARED_EXPORT Q_DECL_EXPORT
#else
#  define F220SHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // F220_GLOBAL_H
