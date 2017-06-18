
#ifndef IO_DRIVER_H
#define IO_DRIVER_H

#include "fesdatadef.h"

#include <QtCore/QtGlobal>

#if defined(IO_DRIVER)
#  define IODRV Q_DECL_EXPORT
#else
#  define IODRV Q_DECL_IMPORT
#endif


extern "C"
{
	//IO ��������ʵ�ֵĽӿ�
	IODRV int StartIoDriver(const char * pszName,int32u nNodeOccNo, int32u nChannelOccNo);

	IODRV int StopIoDriver(const char * pszName, int32u nNodeOccNo, int32u nChannelOccNo);

	// ������������
	IODRV char * ConfigDriverParam(const char * szOldConfData);
	// Ĭ�ϵ�����
	IODRV const char * GetDefaultParam();
	IODRV const char * GetDefaultExtParam();
 
}


#endif // IO_DRIVER_H

