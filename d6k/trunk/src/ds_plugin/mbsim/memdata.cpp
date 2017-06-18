

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
** \param nIdx 从1开始
** \return unsigned char 
** \author LiJin 
** \date 2016年8月2日 
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
** 函数名称: MBRegCoilsCB
** 函数名称: MBRegCoilsCB
**
** 功能描述： 读取DO，MB等类型的寄存器
**
** 输　入:  unsigned char * pucRegBuffer
** 输　入:  unsigned short usAddress 从1开始。
** 输　入:  unsigned short usNCoils
** 输　入:  MBRegisterMode eMode
**
** 输　出:  MBErrorCode
**
** 全局变量:
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期:  2008年9月17日
** 备  注:
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
** 备  注:
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
				for (offset = 0; offset < 8; offset++, i++)		// 将Q值写入一个字节
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
		{//写其实也只是单个线圈的
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
** \brief   保持型寄存器MW AQ的数据读写操作接口
** \details
** \param     pucRegBuffer
** \param     usAddress 地址从1开始
** \param     usNRegs
** \param     eMode
** \return    MBErrorCode
** \author   LiJin
** \date      2008年9月17日
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
** 函数名称: MBRegDiscreteCB
** 函数名称: MBRegDiscreteCB
**
** 功能描述： 读取Di
**
** 输　入:  unsigned char * pucRegBuffer
** 输　入:  unsigned short usAddress
** 输　入:  unsigned short usNDiscrete
**
** 输　出:  MBErrorCode
**
** 全局变量:
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期:  2008年9月17日
** 备  注:
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
** 备  注:
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
			for (offset = 0; offset < 8; offset++, i++)		// 将DI值写入一个字节
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
** 函数名称: MBRegInputCB
** 函数名称: MBRegInputCB
**
** 功能描述： 从内存中读取输入寄存器AI的测值
**
** 输　入:  unsigned char * pucRegBuffer
** 输　入:  unsigned short usAddress
** 输　入:  unsigned short usNRegs
**
** 输　出:  MBErrorCode
**
** 全局变量:
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期:  2008年9月16日
** 备  注:
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
** 备  注:
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
bool CMemData::MBRegInputCB(unsigned char * pucRegBuffer, unsigned short usAddress, unsigned short usNRegs)
{
	bool bRet = true;
	unsigned short         RegIndex = 0;
	unsigned short         Value = 0;

	if ((usAddress >= 1) && (usAddress <= MAX_ITEM_NUM - usNRegs))
	{
		//iRegIndex从0开始
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