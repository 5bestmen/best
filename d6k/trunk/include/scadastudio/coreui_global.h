#ifndef COREUI_GLOBAL_H
#define COREUI_GLOBAL_H

#include <QtCore/qglobal.h>

#ifdef COREUI_LIB
# define COREUI_EXPORT Q_DECL_EXPORT
#else
# define COREUI_EXPORT Q_DECL_IMPORT
#endif

#endif // COREUI_GLOBAL_H
