
 

#include "device.h"

CIoDevice::CIoDevice()
{
	m_nOccNo = 0;
}

CIoDevice::~CIoDevice()
{
	m_arrAins.clear();
	m_arrDins.clear();
	m_arrAouts.clear();
	m_arrDouts.clear();
}
 