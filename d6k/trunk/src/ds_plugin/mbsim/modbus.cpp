


#include "modbus.h"

#if _MSC_VER >= 1600
#define _WIN32_WINNT  0x0500
#endif

#include <thread>
#include <sstream> 
#include <cassert>

CModbusImpl::CModbusImpl()
{

}
CModbusImpl::~CModbusImpl()
{

}

void CModbusImpl::MBLogString(const char *szLogTxt, int nLevel)
{
	extern bool LogString(const char *szLogTxt, int nLevel);
	LogString(szLogTxt, nLevel);
}

CModbusImpl::MBException CModbusImpl::MBError2Exception(CModbusImpl::MBErrorCode eErrorCode)
{
	MBException    eStatus;

	switch (eErrorCode)
	{
	case MB_ENOERR:
		eStatus = MB_EX_NONE;
		break;

	case MB_ENOREG:
		eStatus = MB_EX_ILLEGAL_DATA_ADDRESS;
		break;

	case MB_ETIMEDOUT:
		eStatus = MB_EX_SLAVE_BUSY;
		break;

	default:
		eStatus = MB_EX_SLAVE_DEVICE_FAILURE;
		break;
	}

	return eStatus;
}

bool  CModbusImpl::MBRegCoilsCallBack(unsigned char * pucRegBuffer, unsigned short usAddress, unsigned short usNCoils, bool bIsRead)
{
	if (m_fnRegCoils)
	{
		return m_fnRegCoils(pucRegBuffer, usAddress, usNCoils, bIsRead);
	}
	return false;
}

CModbusImpl::MBErrorCode   CModbusImpl::MBRegCoilsCB(unsigned char  * pucRegBuffer, unsigned short usAddress, unsigned short usNCoils, CModbusImpl::MBRegisterMode eMode)
{
	MBErrorCode mec = MB_ENOERR;

	if (usAddress == 0 || usNCoils == 0)
	{
		mec = MB_ENOREG;
		return mec;
	}
	if (usAddress >  65535 - usNCoils)
	{
		mec = MB_ENOREG;
		return mec;
	}

	MBRegCoilsCallBack(pucRegBuffer, usAddress, usNCoils, eMode == MB_REG_READ ? true : false);
	return mec;
} 

bool CModbusImpl::MBRegHoldingCallBack(unsigned char * pucRegBuffer, unsigned short usAddress, unsigned short usNRegs, bool bIsRead)
{
	if (m_fnRegHolding)
	{
		return m_fnRegHolding(pucRegBuffer, usAddress, usNRegs, bIsRead);
	}
	return false;
}

CModbusImpl::MBErrorCode   CModbusImpl::MBRegHoldingCB(unsigned char * pucRegBuffer, unsigned short usAddress, unsigned short usNRegs, CModbusImpl::MBRegisterMode eMode)
{
	MBErrorCode mec = MB_ENOERR;

	if (usAddress == 0 || usNRegs == 0)
	{
		mec = MB_ENOREG;
		return mec;
	}
	if (usAddress > 65535 - usNRegs)
	{
		mec = MB_ENOREG;
		return mec;
	}

	MBRegHoldingCallBack(pucRegBuffer, usAddress, usNRegs, eMode == MB_REG_READ ? true : false);
	return mec;
}

bool CModbusImpl::MBRegDiscreteCallBack(unsigned char * pucRegBuffer, unsigned short usAddress, unsigned short usNDiscrete)
{
	if (m_fnRegCoils)
	{
		return m_fnRegDiscrete(pucRegBuffer, usAddress, usNDiscrete);
	}
	return false;
}

bool CModbusImpl::MBRegInputCallBack(unsigned char * pucRegBuffer, unsigned short usAddress, unsigned short usNRegs)
{
	if (m_fnRegInput)
	{
		return m_fnRegInput(pucRegBuffer, usAddress, usNRegs);
	}
	return false;
}

CModbusImpl::MBErrorCode  CModbusImpl::MBRegInputCB(unsigned char * pucRegBuffer, unsigned short usAddress, unsigned short usNRegs)
{
	MBErrorCode mec = MB_ENOERR;

	if (usAddress == 0 || usNRegs == 0)
	{
		mec = MB_ENOREG;
		return mec;
	}
	if (usAddress > 65535 - usNRegs)
	{
		mec = MB_ENOREG;
		return mec;
	}

	MBRegInputCallBack(pucRegBuffer, usAddress, usNRegs);
	return mec;
}

CModbusImpl::MBErrorCode  CModbusImpl::MBRegDiscreteCB(unsigned char * pucRegBuffer, unsigned short usAddress, unsigned short usNDiscrete)
{
	MBErrorCode mec = MB_ENOERR;

	if (usAddress == 0 || usNDiscrete == 0)
	{
		mec = MB_ENOREG;
		return mec;
	}
	if (usAddress > 65535 - usNDiscrete)
	{
		mec = MB_ENOREG;
		return mec;
	}

	MBRegDiscreteCallBack(pucRegBuffer, usAddress, usNDiscrete);
	return mec;
}
/*********************************************************************************************************
** 函数名称: MBFuncReadCoils
** 函数名称: MBFuncReadCoils
**
** 功能描述： 解析报文，并执行命令，然后组织成报文返回
**
** 输　入:  unsigned char * pucFrame
** 输　入:  unsigned short * usLen
**
** 输　出:  MBException
**
** 全局变量:
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期:  2008年9月15日
** 备  注:
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
** 备  注:
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
CModbusImpl::MBException CModbusImpl::MBFuncReadCoils(unsigned char * pucFrame, unsigned short * usLen)
{
	unsigned short          usRegAddress = 0;
	unsigned short          usCoilCount = 0;
	unsigned char           ucNBytes = 0;
	unsigned char          *pucFrameCur = nullptr;

	MBException    eStatus = MB_EX_NONE;
	MBErrorCode    eRegStatus = MB_ENOERR;

	if (*usLen == (MB_PDU_FUNC_READ_SIZE + MB_PDU_SIZE_MIN))
	{
		usRegAddress = (unsigned short)(pucFrame[MB_PDU_FUNC_READ_ADDR_OFF] << 8);
		usRegAddress |= (unsigned short)(pucFrame[MB_PDU_FUNC_READ_ADDR_OFF + 1]);
		usRegAddress++;

		usCoilCount = (unsigned short)(pucFrame[MB_PDU_FUNC_READ_COILCNT_OFF] << 8);
		usCoilCount |= (unsigned short)(pucFrame[MB_PDU_FUNC_READ_COILCNT_OFF + 1]);

		/* Check if the number of registers to read is valid. If not
		* return Modbus illegal data value exception.
		*/
		if ((usCoilCount >= 1) &&
			(usCoilCount < MB_PDU_FUNC_READ_COILCNT_MAX))
		{
			/* Set the current PDU data pointer to the beginning. */
			pucFrameCur = &pucFrame[MB_PDU_FUNC_OFF];
			*usLen = MB_PDU_FUNC_OFF;

			/* First byte contains the function code. */
			*pucFrameCur++ = MB_FUNC_READ_COILS;
			*usLen += 1;

			/* Test if the quantity of coils is a multiple of 8. If not last
			* byte is only partially field with unused coils set to zero. */
			if ((usCoilCount & 0x0007) != 0)
			{
				ucNBytes = (unsigned char)(usCoilCount / 8 + 1);
			}
			else
			{
				ucNBytes = (unsigned char)(usCoilCount / 8);
			}
			*pucFrameCur++ = ucNBytes;
			*usLen += 1;
			
			eRegStatus = MBRegCoilsCB( pucFrameCur, usRegAddress, usCoilCount, MB_REG_READ );

			/* If an error occured convert it into a Modbus exception. */
			if (eRegStatus != MB_ENOERR)
			{
				eStatus = MBError2Exception(eRegStatus);
			}
			else
			{
				/* The response contains the function code, the starting address
				* and the quantity of registers. We reuse the old values in the
				* buffer because they are still valid. */
				*usLen += ucNBytes;;
			}
		}
		else
		{
			eStatus = MB_EX_ILLEGAL_DATA_VALUE;
		}
	}
	else
	{
		/* Can't be a valid read coil register request because the length is incorrect. */
		eStatus = MB_EX_ILLEGAL_DATA_VALUE;
	}
	return eStatus;
}
/*********************************************************************************************************
** 函数名称: MBFuncWriteCoil
** 函数名称: MBFuncWriteCoil
**
** 功能描述：写单个线圈
**
** 输　入:  unsigned char * pAduBuf
** 输　入:  unsigned short * usLen
**
** 输　出:  MBException
**
** 全局变量:
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期:  2008年10月13日
** 备  注:
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
** 备  注:
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
CModbusImpl::MBException CModbusImpl::MBFuncWriteCoil(unsigned char * pucFrame, unsigned short * usLen)
{
	unsigned short          usRegAddress = 0;
	unsigned char           ucBuf[2] = { 0 };

	MBException    eStatus = MB_EX_NONE;
	MBErrorCode    eRegStatus;

	if (*usLen == (MB_PDU_FUNC_WRITE_SIZE + MB_PDU_SIZE_MIN))
	{
		usRegAddress = (unsigned short)(pucFrame[MB_PDU_FUNC_WRITE_ADDR_OFF] << 8);
		usRegAddress |= (unsigned short)(pucFrame[MB_PDU_FUNC_WRITE_ADDR_OFF + 1]);
		usRegAddress++;

		if ((pucFrame[MB_PDU_FUNC_WRITE_VALUE_OFF + 1] == 0x00) &&
			((pucFrame[MB_PDU_FUNC_WRITE_VALUE_OFF] == 0xFF) ||
			(pucFrame[MB_PDU_FUNC_WRITE_VALUE_OFF] == 0x00)))
		{
			ucBuf[1] = 0;
			if (pucFrame[MB_PDU_FUNC_WRITE_VALUE_OFF] == 0xFF)
			{
				ucBuf[0] = 1;
			}
			else
			{
				ucBuf[0] = 0;
			}
			eRegStatus = MBRegCoilsCB(&ucBuf[0], usRegAddress, 1, MB_REG_WRITE);

			std::string szLog = GetClient();
		 	std::stringstream ssAdress,ssValue;
			ssAdress << usRegAddress;
			ssValue << ucBuf[0];		 	

			szLog = szLog + "wrote coil . address = " + ssAdress.str() + " value = " + ssValue.str();
			MBLogString(szLog.c_str(), 1);
			
			/* If an error occured convert it into a Modbus exception. */
			if (eRegStatus != MB_ENOERR)
			{
				eStatus = MBError2Exception(eRegStatus);
			}
		}
		else
		{
			eStatus = MB_EX_ILLEGAL_DATA_VALUE;
		}
	}
	else
	{
		/* Can't be a valid write coil register request because the length
		* is incorrect. */
		eStatus = MB_EX_ILLEGAL_DATA_VALUE;
	}
	return eStatus;
}
/*********************************************************************************************************
** 函数名称: MBFuncWriteMultipleCoils
** 函数名称: MBFuncWriteMultipleCoils
**
** 功能描述： 写多个线圈
**
** 输　入:  unsigned char * pAduBuf
** 输　入:  unsigned short * usLen
**
** 输　出:  MBException
**
** 全局变量:
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期:  2008年10月13日
** 备  注:
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
** 备  注:
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
CModbusImpl::MBException CModbusImpl::MBFuncWriteMultipleCoils(unsigned char * pucFrame, unsigned short * usLen)
{
	unsigned short          usRegAddress;
	unsigned short          usCoilCnt;
	unsigned char           ucByteCount;
	unsigned char           ucByteCountVerify;

	MBException    eStatus = MB_EX_NONE;
	MBErrorCode    eRegStatus;

	if (*usLen > (MB_PDU_FUNC_WRITE_SIZE + MB_PDU_SIZE_MIN))
	{
		usRegAddress = (unsigned short)(pucFrame[MB_PDU_FUNC_WRITE_MUL_ADDR_OFF] << 8);
		usRegAddress |= (unsigned short)(pucFrame[MB_PDU_FUNC_WRITE_MUL_ADDR_OFF + 1]);
		usRegAddress++;

		usCoilCnt = (unsigned short)(pucFrame[MB_PDU_FUNC_WRITE_MUL_COILCNT_OFF] << 8);
		usCoilCnt |= (unsigned short)(pucFrame[MB_PDU_FUNC_WRITE_MUL_COILCNT_OFF + 1]);

		ucByteCount = pucFrame[MB_PDU_FUNC_WRITE_MUL_BYTECNT_OFF];

		/* Compute the number of expected bytes in the request. */
		if ((usCoilCnt & 0x0007) != 0)
		{
			ucByteCountVerify = (unsigned char)(usCoilCnt / 8 + 1);
		}
		else
		{
			ucByteCountVerify = (unsigned char)(usCoilCnt / 8);
		}

		if ((usCoilCnt >= 1) &&
			(usCoilCnt <= MB_PDU_FUNC_WRITE_MUL_COILCNT_MAX) &&
			(ucByteCountVerify == ucByteCount))
		{
			eRegStatus = MBRegCoilsCB(&pucFrame[MB_PDU_FUNC_WRITE_MUL_VALUES_OFF],usRegAddress, usCoilCnt, MB_REG_WRITE);

			/* If an error occured convert it into a Modbus exception. */
			if (eRegStatus != MB_ENOERR)
			{
				eStatus = MBError2Exception(eRegStatus);
			}
			else
			{
				/* The response contains the function code, the starting address
				* and the quantity of registers. We reuse the old values in the
				* buffer because they are still valid. */
				*usLen = MB_PDU_FUNC_WRITE_MUL_BYTECNT_OFF;
			}
		}
		else
		{
			eStatus = MB_EX_ILLEGAL_DATA_VALUE;
		}
	}
	else
	{
		/* Can't be a valid write coil register request because the length is incorrect. */
		eStatus = MB_EX_ILLEGAL_DATA_VALUE;
	}
	return eStatus;
}
/*********************************************************************************************************
** 函数名称: MBFuncReadDiscreteInputs
** 函数名称: MBFuncReadDiscreteInputs
**
** 功能描述： 读开入
**
** 输　入:  unsigned char * pAduBuf
** 输　入:  unsigned short * usADULen
**
** 输　出:  MBException
**
** 全局变量:
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期:  2008年10月28日
** 备  注:
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
** 备  注:
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
CModbusImpl::MBException CModbusImpl::MBFuncReadDiscreteInputs(unsigned char * pucFrame, unsigned short * usLen)
{
	unsigned short          usRegAddress;
	unsigned short          usDiscreteCnt;
	unsigned char           ucNBytes;
	unsigned char          *pucFrameCur;

	MBException    eStatus = MB_EX_NONE;
	MBErrorCode    eRegStatus;

	if (*usLen == (MB_PDU_FUNC_READ_SIZE + MB_PDU_SIZE_MIN))
	{
		usRegAddress = (unsigned short)(pucFrame[MB_PDU_FUNC_READ_ADDR_OFF] << 8);
		usRegAddress |= (unsigned short)(pucFrame[MB_PDU_FUNC_READ_ADDR_OFF + 1]);
		usRegAddress++;

		usDiscreteCnt = (unsigned short)(pucFrame[MB_PDU_FUNC_READ_DISCCNT_OFF] << 8);
		usDiscreteCnt |= (unsigned short)(pucFrame[MB_PDU_FUNC_READ_DISCCNT_OFF + 1]);

		/* Check if the number of registers to read is valid. If not
		* return Modbus illegal data value exception.
		*/
		if ((usDiscreteCnt >= 1) &&
			(usDiscreteCnt < MB_PDU_FUNC_READ_DISCCNT_MAX))
		{
			/* Set the current PDU data pointer to the beginning. */
			pucFrameCur = &pucFrame[MB_PDU_FUNC_OFF];
			*usLen = MB_PDU_FUNC_OFF;

			/* First byte contains the function code. */
			*pucFrameCur++ = MB_FUNC_READ_DISCRETE_INPUTS;
			*usLen += 1;

			/* Test if the quantity of coils is a multiple of 8. If not last
			* byte is only partially field with unused coils set to zero. */
			if ((usDiscreteCnt & 0x0007) != 0)
			{
				ucNBytes = (unsigned char)(usDiscreteCnt / 8 + 1);
			}
			else
			{
				ucNBytes = (unsigned char)(usDiscreteCnt / 8);
			}
			*pucFrameCur++ = ucNBytes;
			*usLen += 1;

			eRegStatus = MBRegDiscreteCB(pucFrameCur, usRegAddress, usDiscreteCnt);

			/* If an error occured convert it into a Modbus exception. */
			if (eRegStatus != MB_ENOERR)
			{
				eStatus = MBError2Exception(eRegStatus);
			}
			else
			{
				/* The response contains the function code, the starting address
				* and the quantity of registers. We reuse the old values in the
				* buffer because they are still valid. */
				*usLen += ucNBytes;;
			}
		}
		else
		{
			eStatus = MB_EX_ILLEGAL_DATA_VALUE;
		}
	}
	else
	{
		/* Can't be a valid read coil register request because the length
		* is incorrect. */
		eStatus = MB_EX_ILLEGAL_DATA_VALUE;
	}
	return eStatus;
}


/*********************************************************************************************************
** 函数名称: MBFuncWriteHoldingRegister
** 函数名称: MBFuncWriteHoldingRegister
**
** 功能描述： 写单个寄存器
**
** 输　入:  unsigned char * pucFrame
** 输　入:  unsigned short * usLen
**
** 输　出:  MBException
**
** 全局变量:
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期:  2008年10月13日
** 备  注:
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
** 备  注:
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
CModbusImpl::MBException CModbusImpl::MBFuncWriteHoldingRegister(unsigned char * pucFrame, unsigned short * usLen)
{
	unsigned short   usRegAddress = 0;
	MBException    eStatus = MB_EX_NONE;
	MBErrorCode    eRegStatus;

	if (*usLen == (MB_PDU_FUNC_WRITE_SIZE + MB_PDU_SIZE_MIN))
	{
		usRegAddress = (unsigned short)(pucFrame[MB_PDU_FUNC_WRITE_ADDR_OFF] << 8);
		usRegAddress |= (unsigned short)(pucFrame[MB_PDU_FUNC_WRITE_ADDR_OFF + 1]);
		usRegAddress++;

		/* Make callback to update the value. */
		eRegStatus = MBRegHoldingCB(&pucFrame[MB_PDU_FUNC_WRITE_VALUE_OFF],usRegAddress, 1, MB_REG_WRITE);

		std::string szLog = GetClient();
		std::stringstream ssAdress, ssValue;
		ssAdress << usRegAddress;
		ssValue <<  static_cast<unsigned short> (pucFrame[MB_PDU_FUNC_WRITE_VALUE_OFF] << 8 | pucFrame[MB_PDU_FUNC_WRITE_VALUE_OFF+1]);

		szLog = szLog + "wrote holding register . address = " + ssAdress.str() + " value = " + ssValue.str();
		MBLogString(szLog.c_str(), 1);
		
		/* If an error occured convert it into a Modbus exception. */
		if (eRegStatus != MB_ENOERR)
		{
			eStatus = MBError2Exception(eRegStatus);
		}
	}
	else
	{
		/* Can't be a valid request because the length is incorrect. */
		eStatus = MB_EX_ILLEGAL_DATA_VALUE;
	}
	return eStatus;
}
/*********************************************************************************************************
** 函数名称: MBFuncWriteMultipleHoldingRegister
** 函数名称: MBFuncWriteMultipleHoldingRegister
**
** 功能描述： 写多个寄存器
**
** 输　入:  unsigned char * pucFrame
** 输　入:  unsigned short * usLen
**
** 输　出:  MBException
**
** 全局变量:
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期:  2008年10月13日
** 备  注:
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
** 备  注:
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
CModbusImpl::MBException CModbusImpl::MBFuncWriteMultipleHoldingRegister(unsigned char * pucFrame, unsigned short * usLen)
{
	unsigned short          usRegAddress = 0;
	unsigned short          usRegCount = 0;
	unsigned char           ucRegByteCount = 0;

	MBException    eStatus = MB_EX_NONE;
	MBErrorCode    eRegStatus;

	if (*usLen >= (MB_PDU_FUNC_WRITE_MUL_SIZE_MIN + MB_PDU_SIZE_MIN))
	{
		usRegAddress = (unsigned short)(pucFrame[MB_PDU_FUNC_WRITE_MUL_ADDR_OFF] << 8);
		usRegAddress |= (unsigned short)(pucFrame[MB_PDU_FUNC_WRITE_MUL_ADDR_OFF + 1]);
		usRegAddress++;

		usRegCount = (unsigned short)(pucFrame[MB_PDU_FUNC_WRITE_MUL_REGCNT_OFF] << 8);
		usRegCount |= (unsigned short)(pucFrame[MB_PDU_FUNC_WRITE_MUL_REGCNT_OFF + 1]);

		ucRegByteCount = pucFrame[MB_PDU_FUNC_WRITE_MUL_BYTECNT_OFF];

		if ((usRegCount >= 1) &&
			(usRegCount <= MB_PDU_FUNC_WRITE_MUL_REGCNT_MAX) &&
			(ucRegByteCount == (unsigned char)(2 * usRegCount)))
		{
			/* Make callback to update the register values. */
			eRegStatus = MBRegHoldingCB(&pucFrame[MB_PDU_FUNC_WRITE_MUL_VALUES_OFF], usRegAddress, usRegCount, MB_REG_WRITE);

			/* If an error occured convert it into a Modbus exception. */
			if (eRegStatus != MB_ENOERR)
			{
				eStatus = MBError2Exception(eRegStatus);
			}
			else
			{
				/* The response contains the function code, the starting
				* address and the quantity of registers. We reuse the
				* old values in the buffer because they are still valid.
				*/
				*usLen = MB_PDU_FUNC_WRITE_MUL_BYTECNT_OFF;
			}
		}
		else
		{
			eStatus = MB_EX_ILLEGAL_DATA_VALUE;
		}
	}
	else
	{
		/* Can't be a valid request because the length is incorrect. */
		eStatus = MB_EX_ILLEGAL_DATA_VALUE;
	}
	return eStatus;
}
/*! \fn  MBException MBFuncReadHoldingRegister( unsigned char * pucFrame, unsigned short * usLen )
*********************************************************************************************************
** \brief   读保持型寄存器
** \details
** \param     pucFrame
** \param     usLen
** \return    MBException
** \author   LiJin
** \date    2008年10月10日
** \note
********************************************************************************************************/
CModbusImpl::MBException CModbusImpl::MBFuncReadHoldingRegister(unsigned char * pucFrame, unsigned short * usLen)
{
	unsigned short          usRegAddress = 0;
	unsigned short          usRegCount = 0;
	unsigned char          *pucFrameCur = nullptr;

	MBException    eStatus = MB_EX_NONE;
	MBErrorCode    eRegStatus;

	if (*usLen == (MB_PDU_FUNC_READ_SIZE + MB_PDU_SIZE_MIN))
	{
		//寄存器地址
		usRegAddress = (unsigned short)(pucFrame[MB_PDU_FUNC_READ_ADDR_OFF] << 8);
		usRegAddress |= (unsigned short)(pucFrame[MB_PDU_FUNC_READ_ADDR_OFF + 1]);
		usRegAddress++;

		//寄存器个数
		usRegCount = (unsigned short)(pucFrame[MB_PDU_FUNC_READ_REGCNT_OFF] << 8);
		usRegCount = (unsigned short)(pucFrame[MB_PDU_FUNC_READ_REGCNT_OFF + 1]);

		/* Check if the number of registers to read is valid. If not
		* return Modbus illegal data value exception.
		*/
		if ((usRegCount >= 1) && (usRegCount <= MB_PDU_FUNC_READ_REGCNT_MAX))
		{
			//开始准备组织报文，复位指针头
			//Set the current PDU data pointer to the beginning.  
			pucFrameCur = &pucFrame[MB_PDU_FUNC_OFF];
			*usLen = MB_PDU_FUNC_OFF;

			// First byte contains the function code.  
			//功能码 1个字节
			*pucFrameCur++ = MB_FUNC_READ_HOLDING_REGISTER;
			*usLen += 1;

			//Second byte in the response contain the number of bytes.  
			//字节长度
			*pucFrameCur++ = (unsigned char)(usRegCount * 2);
			*usLen += 1;

			/* Make callback to fill the buffer. */
			eRegStatus = MBRegHoldingCB(pucFrameCur, usRegAddress, usRegCount, MB_REG_READ);
			/* If an error occured convert it into a Modbus exception. */
			if (eRegStatus != MB_ENOERR)
			{
				eStatus = MBError2Exception(eRegStatus);
			}
			else
			{
				*usLen += usRegCount * 2;
			}
		}
		else
		{
			eStatus = MB_EX_ILLEGAL_DATA_VALUE;
		}
	}
	else
	{
		/* Can't be a valid request because the length is incorrect. */
		eStatus = MB_EX_ILLEGAL_DATA_VALUE;
	}
	return eStatus;
}
/*********************************************************************************************************
** 函数名称: MBFuncReadWriteMultipleHoldingRegister
** 函数名称: MBFuncReadWriteMultipleHoldingRegister
**
** 功能描述： 读/写多个寄存器
**
** 输　入:  unsigned char * pAduBuf
** 输　入:  unsigned short * usLen
**
** 输　出:  extern MBException
**
** 全局变量:
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期:  2008年10月13日
** 备  注:
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
** 备  注:
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
CModbusImpl::MBException CModbusImpl::MBFuncReadWriteMultipleHoldingRegister(unsigned char * pucFrame, unsigned short * usLen)
{
	unsigned short          usRegReadAddress = 0;
	unsigned short          usRegReadCount = 0;
	unsigned short          usRegWriteAddress = 0;
	unsigned short          usRegWriteCount = 0;
	unsigned char           ucRegWriteByteCount = 0;
	unsigned char          *pucFrameCur = nullptr;

	MBException    eStatus = MB_EX_NONE;
	MBErrorCode    eRegStatus;

	if (*usLen >= (MB_PDU_FUNC_READWRITE_SIZE_MIN + MB_PDU_SIZE_MIN))
	{
		usRegReadAddress = (unsigned short)(pucFrame[MB_PDU_FUNC_READWRITE_READ_ADDR_OFF] << 8U);
		usRegReadAddress |= (unsigned short)(pucFrame[MB_PDU_FUNC_READWRITE_READ_ADDR_OFF + 1]);
		usRegReadAddress++;

		usRegReadCount = (unsigned short)(pucFrame[MB_PDU_FUNC_READWRITE_READ_REGCNT_OFF] << 8U);
		usRegReadCount |= (unsigned short)(pucFrame[MB_PDU_FUNC_READWRITE_READ_REGCNT_OFF + 1]);

		usRegWriteAddress = (unsigned short)(pucFrame[MB_PDU_FUNC_READWRITE_WRITE_ADDR_OFF] << 8U);
		usRegWriteAddress |= (unsigned short)(pucFrame[MB_PDU_FUNC_READWRITE_WRITE_ADDR_OFF + 1]);
		usRegWriteAddress++;

		usRegWriteCount = (unsigned short)(pucFrame[MB_PDU_FUNC_READWRITE_WRITE_REGCNT_OFF] << 8U);
		usRegWriteCount |= (unsigned short)(pucFrame[MB_PDU_FUNC_READWRITE_WRITE_REGCNT_OFF + 1]);

		ucRegWriteByteCount = pucFrame[MB_PDU_FUNC_READWRITE_BYTECNT_OFF];

		if ((usRegReadCount >= 1) && (usRegReadCount <= 0x7D) &&
			(usRegWriteCount >= 1) && (usRegWriteCount <= 0x79) &&
			((2 * usRegWriteCount) == ucRegWriteByteCount))
		{
			/* Make callback to update the register values. */
			eRegStatus = MBRegHoldingCB(&pucFrame[MB_PDU_FUNC_READWRITE_WRITE_VALUES_OFF],usRegWriteAddress, usRegWriteCount, MB_REG_WRITE);

			if (eRegStatus == MB_ENOERR)
			{
				/* Set the current PDU data pointer to the beginning. */
				pucFrameCur = &pucFrame[MB_PDU_FUNC_OFF];
				*usLen = MB_PDU_FUNC_OFF;

				/* First byte contains the function code. */
				*pucFrameCur++ = MB_FUNC_READWRITE_MULTIPLE_REGISTERS;
				*usLen += 1;

				/* Second byte in the response contain the number of bytes. */
				*pucFrameCur++ = (unsigned char)(usRegReadCount * 2);
				*usLen += 1;

				/* Make the read callback. */
				eRegStatus = MBRegHoldingCB(pucFrameCur, usRegReadAddress, usRegReadCount, MB_REG_READ);
				if (eRegStatus == MB_ENOERR)
				{
					*usLen += 2 * usRegReadCount;
				}
			}
			if (eRegStatus != MB_ENOERR)
			{
				eStatus = MBError2Exception(eRegStatus);
			}
		}
		else
		{
			eStatus = MB_EX_ILLEGAL_DATA_VALUE;
		}
	}
	return eStatus;
}

/*********************************************************************************************************
** 函数名称: MBFuncReadInputRegister
** 函数名称: MBFuncReadInputRegister
**
** 功能描述： 读模入
**
** 输　入:  unsigned char * pAduBuf
** 输　入:  unsigned short * usLen
**
** 输　出:  MBException
**
** 全局变量:
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期:  2008年10月13日
** 备  注:
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
** 备  注:
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
CModbusImpl::MBException CModbusImpl::MBFuncReadInputRegister(unsigned char * pucFrame, unsigned short * usLen)
{
	unsigned short          usRegAddress = 0;
	unsigned short          usRegCount = 0;
	unsigned char          *pucFrameCur = nullptr;

	MBException    eStatus = MB_EX_NONE;
	MBErrorCode    eRegStatus;

	if (*usLen == (MB_PDU_FUNC_READ_SIZE + MB_PDU_SIZE_MIN))
	{
		usRegAddress = (unsigned short)(pucFrame[MB_PDU_FUNC_READ_ADDR_OFF] << 8);
		usRegAddress |= (unsigned short)(pucFrame[MB_PDU_FUNC_READ_ADDR_OFF + 1]);
		usRegAddress++;

		usRegCount = (unsigned short)(pucFrame[MB_PDU_FUNC_READ_REGCNT_OFF] << 8);
		usRegCount |= (unsigned short)(pucFrame[MB_PDU_FUNC_READ_REGCNT_OFF + 1]);

		/* Check if the number of registers to read is valid. If not
		* return Modbus illegal data value exception.
		*/
		if ((usRegCount >= 1)
			&& (usRegCount < MB_PDU_FUNC_READ_REGCNT_MAX))
		{
			/* Set the current PDU data pointer to the beginning. */
			pucFrameCur = &pucFrame[MB_PDU_FUNC_OFF];
			*usLen = MB_PDU_FUNC_OFF;

			/* First byte contains the function code. */
			*pucFrameCur++ = MB_FUNC_READ_INPUT_REGISTER;
			*usLen += 1;

			/* Second byte in the response contain the number of bytes. */
			*pucFrameCur++ = (unsigned char)(usRegCount * 2);
			*usLen += 1;

			eRegStatus = MBRegInputCB(pucFrameCur, usRegAddress, usRegCount);

			/* If an error occured convert it into a Modbus exception. */
			if (eRegStatus != MB_ENOERR)
			{
				eStatus = MBError2Exception(eRegStatus);
			}
			else
			{
				*usLen += usRegCount * 2;
			}
		}
		else
		{
			eStatus = MB_EX_ILLEGAL_DATA_VALUE;
		}
	}
	else
	{
		/* Can't be a valid read input register request because the length is incorrect. */
		eStatus = MB_EX_ILLEGAL_DATA_VALUE;
	}
	return eStatus;
}

bool CModbusImpl::MBPoll(unsigned char * pPdu, unsigned short * pLen)
{
	MBException eExcept;

	assert(pPdu && pLen);
	if (pPdu == nullptr || pLen == nullptr)
		return false;

	unsigned char nFuncCode = pPdu[MB_PDU_FUNC_OFF];

	switch (nFuncCode)
	{
		// 读线圈状态
	case MB_FUNC_READ_COILS: 
		eExcept = MBFuncReadCoils(pPdu, pLen);
		break;
		// 读离散输入状态
	case MB_FUNC_READ_DISCRETE_INPUTS: 
		eExcept = MBFuncReadDiscreteInputs(pPdu, pLen);
		break;
		// 读保持型寄存器
	case MB_FUNC_READ_HOLDING_REGISTER:
		eExcept = MBFuncReadHoldingRegister(pPdu, pLen);
		break;
		// 读输入寄存器
	case MB_FUNC_READ_INPUT_REGISTER:
		eExcept = MBFuncReadInputRegister(pPdu, pLen);
		break;
		// 强制单个线圈
	case MB_FUNC_WRITE_SINGLE_COIL:
		eExcept = MBFuncWriteCoil(pPdu, pLen);
		break;
		// 预置单个寄存器 
	case MB_FUNC_WRITE_REGISTER:
		eExcept = MBFuncWriteHoldingRegister(pPdu, pLen);
		break;
		// 写多个线圈
	case MB_FUNC_WRITE_MULTIPLE_COILS:
		eExcept = MBFuncWriteMultipleCoils(pPdu, pLen);
		break;
		// 写多个寄存器
	case MB_FUNC_WRITE_MULTIPLE_REGISTERS:
		eExcept = MBFuncWriteMultipleHoldingRegister(pPdu, pLen);
		break;
		// 读写多个寄存器
	case MB_FUNC_READWRITE_MULTIPLE_REGISTERS:
		eExcept = MBFuncReadWriteMultipleHoldingRegister(pPdu, pLen);
		break;
	default:
		return false;
	}

	/* An exception occured. Build an error frame. */
	if (eExcept != MB_EX_NONE)
	{
		int nLen = 0;
		pPdu[nLen++] = (unsigned char)(nFuncCode | MB_FUNC_ERROR);
		pPdu[nLen++] = eExcept;
		//	nLen
	}
	return true;
}

