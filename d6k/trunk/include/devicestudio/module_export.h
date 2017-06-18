#ifndef MODULE_EXPORT_H
#define MODULE_EXPORT_H

#include <QtCore/QtGlobal>

#ifdef MODULE_LIB
# define DS_EXPORT Q_DECL_EXPORT
#else
# define DS_EXPORT Q_DECL_IMPORT
#endif


extern "C" DS_EXPORT IPluginModule *CreateModule();

#endif // MODULE_EXPORT_H
