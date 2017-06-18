

#include <boost/thread/locks.hpp>

#include "memdata.h"

#if _MSC_VER >= 1600
#define _WIN32_WINNT  0x0500
#endif
 

void CMemData::InitMemData()
{
	unsigned int nCnt = 0;

	auto func_coil = [&](CCoilOutput &coil)
	{
		nCnt++;
		coil.UISetValue((nCnt % 2) == 0 ? 0 : 1);
		coil.InitAddress(nCnt);
	};
	std::for_each(m_arrCoils.begin(), m_arrCoils.end(), func_coil);

	nCnt = 0;
	auto func_di = [&](CDigitalInput &di)
	{
		nCnt++;
		di.UISetValue((nCnt % 2) == 0 ? 0 : 1);
		di.InitAddress(nCnt);
	};
	std::for_each(m_arrDigitals.begin(), m_arrDigitals.end(), func_di);

	nCnt = 0;
	auto func_hr = [&](CHoldingRegister &holding)
	{
		nCnt++;
		holding.UISetValue( static_cast<unsigned short>(nCnt) );
		holding.InitAddress(nCnt);

	};
	std::for_each(m_arrRegisters.begin(), m_arrRegisters.end(), func_hr);

	nCnt = 0;
	auto func_input = [&](CAnalogueInput &ai)
	{
		nCnt++;
		ai.UISetValue(static_cast<unsigned short>(nCnt));
		ai.InitAddress(nCnt);

	};
	std::for_each(m_arrAnalogues.begin(), m_arrAnalogues.end(), func_input);
}


CMemData::CMemData()
{
	m_nMBAddress = 1;
	InitMemData();
}

CMemData::~CMemData()
{

}
/*! \fn unsigned char CMemData::GetCoilValue(unsigned short nIdx)
********************************************************************************************************* 
** \brief CMemData::GetCoilValue 
** \details 
** \param nIdx ��1��ʼ
** \return unsigned char 
** \author LiJin 
** \date 2016��8��2�� 
** \note 
********************************************************************************************************/
unsigned char CMemData::GetCoilValue(unsigned short nIdx)
{
	if ( nIdx>0 && nIdx <= MAX_ITEM_NUM )
	{
		boost::shared_lock<boost::shared_mutex> lock(m_CoilMutex);
		unsigned char nVal = m_arrCoils.at(nIdx - 1).GetValue();
		nVal = !nVal;
		return !nVal;	
	}
	return 0;
}
unsigned char CMemData::GetDiscreteValue(unsigned short nIdx)
{
	if (nIdx > 0 && nIdx <= MAX_ITEM_NUM)
	{
		return m_arrDigitals.at(nIdx - 1).GetValue();
	}
	return 0;
}
unsigned short CMemData::GetHoldingValue(unsigned short nIdx)
{
	if (nIdx > 0 && nIdx <= MAX_ITEM_NUM)
	{
		boost::shared_lock<boost::shared_mutex> lock(m_HoldingMutex);
		return m_arrRegisters.at(nIdx - 1).GetValue();
	}
	return 0;
}
unsigned short CMemData::GetInputValue(unsigned short nIdx)
{
	if (nIdx > 0 && nIdx <= MAX_ITEM_NUM)
	{
		return m_arrAnalogues.at(nIdx - 1).GetValue();
	}
	return 0;
}

void CMemData::SetCoilValue(unsigned short nIdx, unsigned char nValue)
{
	unsigned char nNew = nValue ? 1 : 0;
	if (nIdx > 0 && nIdx <= MAX_ITEM_NUM)
	{
		m_CoilMutex.lock_upgrade();
		m_arrRegisters.at(nIdx - 1).SetValue(nNew);
		m_CoilMutex.unlock();
	}
}

void  CMemData::SetHoldingValue(unsigned short nIdx, unsigned short nValue)
{
	if (nIdx > 0 && nIdx <= MAX_ITEM_NUM)
	{
		m_HoldingMutex.lock_upgrade();
		m_arrRegisters.at(nIdx - 1).SetValue(nValue);
		m_HoldingMutex.unlock();
	}
}

unsigned short  CMemData::GetCoilAddr(unsigned short nIdx)
{
	if (nIdx > 0 && nIdx <= MAX_ITEM_NUM)
	{
	 	unsigned short nVal = m_arrCoils.at(nIdx - 1).GetAddress();
		 
		return nVal;
	}
	return 0;
}

/*********************************************************************************************************
** ��������: MBRegCoilsCB
** ��������: MBRegCoilsCB
**
** ���������� ��ȡDO��MB�����͵ļĴ���
**
** �䡡��:  unsigned char * pucRegBuffer
** �䡡��:  unsigned short usAddress ��1��ʼ��
** �䡡��:  unsigned short usNCoils
** �䡡��:  MBRegisterMode eMode
**
** �䡡��:  MBErrorCode
**
** ȫ�ֱ���:
** ����ģ��: ��
**
** ������:  LiJin
** �ա���:  2008��9��17��
** ��  ע:
**-------------------------------------------------------------------------------------------------------
** �޸���:
** �ա���:
** ��  ע:
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
bool CMemData::MBRegCoilsCB(unsigned char * pucRegBuffer, unsigned short usAddress, unsigned short usNCoils, bool bIsRead)
{
//	MBErrorCode  Status = MB_ENOERR;
	bool bRet = true;
	unsigned char offset = 0;
	unsigned short i = 0;
	unsigned char  *pByte = pucRegBuffer;
	unsigned short   RegIndex = 0;

	if ((usAddress >= 1) && (usAddress <= MAX_ITEM_NUM - usNCoils))
	{
		if (bIsRead == true)
		{
			while (i < usNCoils)
			{
				*pByte = 0;
				for (offset = 0; offset < 8; offset++, i++)		// ��Qֵд��һ���ֽ�
				{
					if (i < usNCoils)
					{
					 	RegIndex = usAddress + i;
						*pByte |= GetCoilValue(RegIndex) << offset;
					}
				}
				pByte++;
			}
		}
		else if (bIsRead == false)
		{//д��ʵҲֻ�ǵ�����Ȧ��
			unsigned short nVal = 0;
			nVal = pucRegBuffer[0] << 8 | pucRegBuffer[1];
			if (nVal)
			{
				SetCoilValue(usAddress, 1);
			}
			else
			{
				SetCoilValue(usAddress, 0);
			}	 
		}
	}	 
	else
	{
		bRet = false;
	}
	return bRet;
}
/*! \fn  MBErrorCode MBRegHoldingCB( unsigned char * pucRegBuffer, unsigned short usAddress, unsigned short usNRegs, MBRegisterMode eMode )
*********************************************************************************************************
** \brief   �����ͼĴ���MW AQ�����ݶ�д�����ӿ�
** \details
** \param     pucRegBuffer
** \param     usAddress ��ַ��1��ʼ
** \param     usNRegs
** \param     eMode
** \return    MBErrorCode
** \author   LiJin
** \date      2008��9��17��
** \note
********************************************************************************************************/
bool CMemData::MBRegHoldingCB(unsigned char * pucRegBuffer, unsigned short usAddress, unsigned short usNRegs, bool bIsRead)
{
	bool bRet = true;
	unsigned int         iRegIndex = 0;
	unsigned short         Value = 0;

	if ((usAddress >= 1) && (usAddress <= MAX_ITEM_NUM - usNRegs))
	{
		iRegIndex = (unsigned int)(usAddress);
		if (bIsRead == true)
		{
			while (usNRegs > 0)
			{
				Value = GetHoldingValue(iRegIndex);
				*pucRegBuffer++ = (unsigned char)(Value >> 8);
				*pucRegBuffer++ = (unsigned char)(Value & 0xFF);
				iRegIndex++;
				usNRegs--;
			}
		}
		else if (bIsRead == false)
		{
			
			while (usNRegs > 0)
			{
				Value = (*pucRegBuffer++ << 8) | (*pucRegBuffer++);
				SetHoldingValue(usAddress, Value);
				Value = GetHoldingValue(usAddress);
				*pucRegBuffer++ = (unsigned char)(Value >> 8);
				*pucRegBuffer++ = (unsigned char)(Value & 0xFF);
				iRegIndex++;
				usNRegs--;
			}
		}
	}
	else
	{
		bRet = false;
	}
	return bRet;
}
/*********************************************************************************************************
** ��������: MBRegDiscreteCB
** ��������: MBRegDiscreteCB
**
** ���������� ��ȡDi
**
** �䡡��:  unsigned char * pucRegBuffer
** �䡡��:  unsigned short usAddress
** �䡡��:  unsigned short usNDiscrete
**
** �䡡��:  MBErrorCode
**
** ȫ�ֱ���:
** ����ģ��: ��
**
** ������:  LiJin
** �ա���:  2008��9��17��
** ��  ע:
**-------------------------------------------------------------------------------------------------------
** �޸���:
** �ա���:
** ��  ע:
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
bool CMemData::MBRegDiscreteCB(unsigned char * pucRegBuffer, unsigned short usAddress, unsigned short usNDiscrete)
{
	bool bRet = true;
	unsigned char offset = 0;
	unsigned short i = 0;
	unsigned char  *pByte = pucRegBuffer;
	unsigned short  RegIndex = 0;

	if ((usAddress >= 1) && (usAddress <= MAX_ITEM_NUM - usNDiscrete))
	{
		while (i < usNDiscrete)
		{
			*pByte = 0;
			for (offset = 0; offset < 8; offset++, i++)		// ��DIֵд��һ���ֽ�
			{
				if (i < usNDiscrete)
				{
					RegIndex = usAddress  + i;
					*pByte |= GetDiscreteValue(RegIndex) << offset;
				}
			}
			pByte++;
		}
	}
	else
	{
		bRet = false;
	}
	return bRet;
}
/*********************************************************************************************************
** ��������: MBRegInputCB
** ��������: MBRegInputCB
**
** ���������� ���ڴ��ж�ȡ����Ĵ���AI�Ĳ�ֵ
**
** �䡡��:  unsigned char * pucRegBuffer
** �䡡��:  unsigned short usAddress
** �䡡��:  unsigned short usNRegs
**
** �䡡��:  MBErrorCode
**
** ȫ�ֱ���:
** ����ģ��: ��
**
** ������:  LiJin
** �ա���:  2008��9��16��
** ��  ע:
**-------------------------------------------------------------------------------------------------------
** �޸���:
** �ա���:
** ��  ע:
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
bool CMemData::MBRegInputCB(unsigned char * pucRegBuffer, unsigned short usAddress, unsigned short usNRegs)
{
	bool bRet = true;
	unsigned short         RegIndex = 0;
	unsigned short         Value = 0;

	if ((usAddress >= 1) && (usAddress <= MAX_ITEM_NUM - usNRegs))
	{
		//iRegIndex��0��ʼ
		RegIndex = (unsigned short)(usAddress);
		while (usNRegs > 0)
		{
			Value = GetInputValue(RegIndex + 1);

			*pucRegBuffer++ = (unsigned char)(Value >> 8);
			*pucRegBuffer++ = (unsigned char)(Value & 0xFF);
			RegIndex++;
			usNRegs--;
		}
	}
	else
	{
		bRet = false;
	}
	return bRet;
}