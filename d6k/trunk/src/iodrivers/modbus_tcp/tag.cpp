



#include "tag.h"
#include "fesapi/fesapi.h"

#include <QString>


void CIoAin::LoadData()
{
	Q_ASSERT(m_nOccNo != INVALID_OCCNO);
	bool bRet = GetAinPtr(m_nOccNo, &m_pTag);
	Q_ASSERT(bRet);

	LoadAddress();
}

void CIoDin::LoadData()
{
	Q_ASSERT(m_nOccNo != INVALID_OCCNO);
	bool bRet = GetDinPtr(m_nOccNo, &m_pTag);
	Q_ASSERT(bRet);
	LoadAddress();
}
