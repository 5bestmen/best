

#include "channel.h"
#include "fesapi/fesapi.h"
#include "log/log.h"
#include "tag.h"
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>


CIoChannel::CIoChannel()
{
	m_nOccNo = 0;
}

CIoChannel::~CIoChannel()
{
	m_arrDevices.clear();
}


void CIoChannel::LogMsg(const char *szLogTxt, int nLevel)
{
	::LogMsg("simu_iodrv", szLogTxt, nLevel,nullptr);
}


C104Channel::C104Channel(int32u nOccNo)
{
	Q_ASSERT(nOccNo !=INVALID_OCCNO && nOccNo <=MAX_CHANNEL_OCCNO);
	if (nOccNo==INVALID_OCCNO || nOccNo >MAX_CHANNEL_OCCNO)
	{
		return;
	}

	m_nOccNo = nOccNo;
}

C104Channel::~C104Channel()
{

}

bool C104Channel::Start()
{
//	m_p104Master->ConnectToHost();

	return true;
}

bool C104Channel::Stop()
{
	return true;
}

bool C104Channel::LoadData(int32u nOccNo)
{
	Q_ASSERT(nOccNo!=INVALID_OCCNO && nOccNo <=MAX_CHANNEL_OCCNO);
	if (nOccNo==INVALID_OCCNO || nOccNo >MAX_CHANNEL_OCCNO)
	{
		return false;
	}
	int32u nCount;
	
	//模拟量
	bool bRet=GetAinCountInChannel(nOccNo, &nCount);
	Q_ASSERT(bRet);
	if (bRet)
	{
		std::unique_ptr <int32u[]> pOccNos(new int32u[nCount]());

		bRet = GetAinOccNosInChannel(nOccNo, pOccNos.get(), nCount);
		Q_ASSERT(bRet);

		if (bRet)
		{
			for (int i = 0; i < nCount; i++)
			{
				std::shared_ptr<CIoAin> pAin = std::make_shared<CIoAin>((pOccNos[i]));
				m_arrAins.push_back(pAin);
				pAin->LoadData();
			}
		}
		else
		{
			//todo log
		}
	}

	// 开关量
	bRet = GetDinCountInChannel(nOccNo, &nCount);
	Q_ASSERT(bRet);
	if (bRet == true)
	{
		if (nCount > 0 && nCount <= MAX_OCCNO)
		{
			std::unique_ptr <int32u[]> pOccNos(new int32u[nCount]());
			bRet = GetDinOccNosInChannel(nOccNo, pOccNos.get(), nCount);
			Q_ASSERT(bRet);
			if (bRet == true)
			{
				for (int i = 0; i < nCount; i++)
				{
					std::shared_ptr<CIoDin> pDin = std::make_shared<CIoDin>((pOccNos[i]));
					m_arrDins.push_back(pDin);
					pDin->LoadData();
				}
			}
			else
			{// todo : log

			}
		}
	}

	for each (auto  var in m_arrAins)
	{
		m_mapAinAddr2OccNo.insert(std::make_pair(var->GetAddress(),var->GetOccNo()));
	}

	for each (auto  var in m_arrDins)
	{
		m_mapDinAddr2OccNo.insert(std::make_pair(var->GetAddress(),var->GetOccNo()));
	}

	return true;
}

int32u C104Channel::GetAinOccNoByAddr(int nAddr)
{
	auto iter = m_mapAinAddr2OccNo.find(nAddr);
		
	if (iter==m_mapAinAddr2OccNo.end())
	{
		return INVALID_OCCNO;
	}
	else
	{
		return iter->second;
	}
}

int32u C104Channel::GetDinOccNoByAddr(int nAddr)
{
	auto iter = m_mapDinAddr2OccNo.find(nAddr);

	if (iter == m_mapDinAddr2OccNo.end())
	{
		return INVALID_OCCNO;
	}
	else
	{
		return iter->second;
	}
}

int32u C104Channel::GetDeviceAddr()
{
	Q_ASSERT(m_nOccNo!=INVALID_OCCNO && m_nOccNo <= MAX_CHANNEL_OCCNO);
	if (m_nOccNo==INVALID_OCCNO || m_nOccNo >MAX_CHANNEL_OCCNO)
	{
		return INVALID_OCCNO;
	}
	return INVALID_OCCNO;
}

