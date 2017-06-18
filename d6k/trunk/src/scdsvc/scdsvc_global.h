#ifndef SCDSVC_GLOBAL_H
#define SCDSVC_GLOBAL_H

#include <QtCore/QtGlobal>

#if defined(SCDSVC_LIBRARY)
#  define SCDSVC_API Q_DECL_EXPORT
#else
#  define SCDSVC_API Q_DECL_IMPORT
#endif

#endif // SCDSVC_GLOBAL_H
