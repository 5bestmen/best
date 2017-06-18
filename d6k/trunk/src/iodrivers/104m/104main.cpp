#include "104main.h"
#include "104master.h"
#include "channel.h"


C104Main::C104Main(int32u nOccNo)
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_CHANNEL_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_CHANNEL_OCCNO)
	{
		return;
	}
	m_nOccNo = nOccNo;

	m_pDataUnit = std::make_shared<C104Channel>(nOccNo);

	bool bRet=m_pDataUnit->LoadData(nOccNo);
	Q_ASSERT(bRet);
	if (!bRet)
	{
		return;
	}
	m_p104 = std::make_shared<C104Master>(nullptr,nOccNo,m_pDataUnit);
}


C104Main::~C104Main()
{
}

bool C104Main::Start()
{
	m_p104->ConnectToHost();
	return true;
}

void C104Main::Run()
{

}

void C104Main::Stop()
{

}
