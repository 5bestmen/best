
#ifndef HMI_API_H
#define HMI_API_H
 
#include "datatypes.h"
#include "../variant.h"

#include <QtCore/QtGlobal>

#ifdef HMI_SDK_LIB
# define HMI_API Q_DECL_EXPORT
#else
# define HMI_API Q_DECL_IMPORT)
#endif
 


extern "C"
{
	// Æô¶¯
	HMI_API bool StartHMI(const char * pszProjectName);
	// ¹Ø±Õ
	HMI_API bool StopHMI(const char * pszProjectName);
}


#endif // SCADA_API_H

