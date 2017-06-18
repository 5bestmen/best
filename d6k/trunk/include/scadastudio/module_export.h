#ifndef MODULE_EXPORT_H
#define MODULE_EXPORT_H

#ifdef MODULE_LIB
# define SCADA_EXPORT Q_DECL_EXPORT
#else
# define SCADA_EXPORT Q_DECL_IMPORT
#endif


extern "C" SCADA_EXPORT IModule *CreateModule(); 

#endif // MODULE_EXPORT_H
