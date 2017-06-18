#ifndef MODULE_EXPORT_H
#define MODULE_EXPORT_H

#include <QtCore/QtGlobal>

#ifdef MODULE_LIB
# define FES_EXPORT Q_DECL_EXPORT
#else
# define FES_EXPORT Q_DECL_IMPORT
#endif

 

#endif // MODULE_EXPORT_H
