// -*- C++ -*-
/*
To use ACE with MFC libraries, also add the following to your config.h file. 
Notice that if you want to spawn a new thread with CWinThread, 
make sure you spawn the thread with THR_USE_AFX flag set.

//#define ACE_HAS_MFC 1
//ACE_USES_STATIC_MFC 

Static version of ACE libraries are built with ACE_AS_STATIC_LIBS defined. 
This macro should also be used in application projects that link to static ACE libraries
Optionally you can also add the line 
#define ACE_NO_INLINE

*/
#ifndef ACE_CONFIG_H
#define ACE_CONFIG_H

#define ACE_DISABLE_WIN32_ERROR_WINDOWS
#define ACE_DISABLE_WIN32_INCREASE_PRIORITY
/*zyj*/

#define ACE_HAS_STANDARD_CPP_LIBRARY 1
#define ACE_LACKS_CDR_ALIGNMENT

#include /**/ "ace/config-win32.h"

#endif /* ACE_CONFIG_H */
