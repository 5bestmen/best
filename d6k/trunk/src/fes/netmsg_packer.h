/*! @file netmsg_packer.h
<PRE>
********************************************************************************
模块名       :
文件名       :  netmsg_packer.h
文件实现功能 :  以太网消息打包器
作者         :  LiJin
版本         :  V1.00
--------------------------------------------------------------------------------
备注         :  <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief   以太网消息打包器
*  @author  LiJin
*  @version 1.0
*  @date    2016.11.29
*******************************************************************************/
#ifndef NET_PACKETAGE_H
#define NET_PACKETAGE_H

#include "log/log.h"
#include <vector>
#include <tuple>
#include <cassert>
#include <QObject>



class CPacketHelper
{
public:
	enum
	{
		MAX_BUFF_SIZE = 800 * 1024,
	};
	/*! \fn static size_t MsgSizeCheck(size_t nPreLen, const unsigned char* const pInputData[], const size_t pLenArray[], size_t nNum)
	********************************************************************************************************* 
	** \brief CPacketHelper::MsgSizeCheck 
	** \details  计算报文的长度，并检查报文长度是否合法
	** \param nPreLen 引导区的报文头（同步头、报文长度），通常是8个字节，4字节的同步头，4字节的长度
	** \param pInputData 输入缓冲区（多个），，
	** \param pLenArray 每个输入缓冲区的长度
	** \param nNum 缓冲区个数
	** \return size_t 返回-1 表示报文长度非法，溢出
	** \author LiJin 
	** \date 2017年1月11日 
	** \note 
	********************************************************************************************************/
	static size_t MsgSizeCheck(size_t nPreLen, const unsigned char* const pInputData[], const size_t pLenArray[], size_t nNum)
	{
		if (pInputData == nullptr || pLenArray == nullptr)
		{
			assert(false);
			return -1;
		}
		
		auto nTotalLen = nPreLen;
		auto nLastTotalLen = nTotalLen;

		for (size_t i = 0; i < nNum; ++i)
		{
			if (nullptr != pInputData[i])
			{
				nTotalLen += pLenArray[i];
				if (nLastTotalLen > nTotalLen || nTotalLen < 1 || nTotalLen >MAX_BUFF_SIZE) //overflow
				{
					::LogMsg("FES", QObject::tr("pack msg error: length exceeded the MAX_BUFF_SIZE!").toStdString().c_str(), 1, nullptr);
					return -1;
				}
				nLastTotalLen = nTotalLen;
			}
		}
		return nTotalLen;
	}
};

template<typename MsgType>
class INetMsgPacker
{
public:
	typedef MsgType msg_type;
	typedef const msg_type msg_ctype;

protected:
	virtual ~INetMsgPacker() 
	{

	}
public:
	virtual void ResetState()
	{
	}
	// 继承类必须实现的接口
	virtual msg_type PackMsg(const unsigned char* const pInputData[], const size_t pLenArray[], size_t nNum, bool bNative = false) = 0;
	virtual unsigned char* GetRawData(msg_type& msg) const
	{
		std::ignore = (msg);
		return nullptr;
	}
	virtual const unsigned char* GetRawData(msg_ctype& msg) const
	{
		std::ignore = (msg);
		return nullptr; 
	}
	virtual size_t GetRawDataLen(msg_ctype& msg) const
	{
		std::ignore = (msg);
		return 0; 
	}

	msg_type PackMsg(const unsigned char* pInputData, size_t nLen, bool bNative = false)
	{ 
		return PackMsg(&pInputData, &nLen, 1, bNative);
	}

	msg_type PackMsg(const std::vector<unsigned char>& arrInputData, bool bNative = false)
	{ 
		return PackMsg(arrInputData.data(), arrInputData.size(), bNative);
	}
};

class CNetMsgPacker : public INetMsgPacker <std::vector<unsigned char> >
{
public:
	enum
	{
		NETMSG_SYNC_LEN = 4,
		NETMSG_TOTAL_LEN = 4,  //!< 整封报文的长度
		NETMSG_HEAD_LEN = 8,   //!< 报文头，4个字节的标识符（0X4C49 ,0XEB90）, 4字节的报文长度
	};
	using INetMsgPacker<msg_type>::PackMsg;

	/*! \fn virtual msg_type PackMsg(const unsigned char* const pInputData[], const size_t pLenArray[], size_t nNum, bool bNative = false)
	********************************************************************************************************* 
	** \brief CNetMsgPacker::PackMsg 
	** \details 打包
	** \param pInputData 输入缓冲区
	** \param pLenArray  输入缓冲区的长度
	** \param nNum       输入缓冲区的个数
	** \param bNative    是否增加报文头
	** \return INetMsgPacker<std::vector<unsigned char>>::msg_type 
	** \author LiJin 
	** \date 2017年1月11日 
	** \note 
	********************************************************************************************************/
	virtual msg_type PackMsg(const unsigned char* const pInputData[], const size_t pLenArray[], size_t nNum, bool bNative = false)
	{
		msg_type msg;
		auto nPreLen = bNative ? 0 : NETMSG_HEAD_LEN;

		const unsigned char arrHead[NETMSG_SYNC_LEN] = { 0x4c, 0x49, 0xeb, 0x90 };

		auto nTotalLen = CPacketHelper::MsgSizeCheck(nPreLen, pInputData, pLenArray, nNum);

		size_t i = 0, j = 0;

		if (nTotalLen == static_cast<size_t>(-1))
			return msg;
		else if (nTotalLen > nPreLen)
		{
			msg.clear();
			msg.reserve(nTotalLen);

			if (bNative == false)
			{// 增加上报文头
				//auto head_len = nTotalLen;
				// 同步头 
				for ( i = 0; i < NETMSG_SYNC_LEN;i++)
				{
					msg.emplace_back(arrHead[i]);
				}
				// 报文长度
				unsigned char *pTemp = reinterpret_cast<unsigned char *>(&nTotalLen);
				for (i = 0; i < NETMSG_TOTAL_LEN; i++)
				{
					msg.emplace_back(pTemp[i]);
				}
			}

			for (i = 0; i < nNum; ++i)
			{
				if (nullptr != pInputData[i])
				{
					for (j = 0; j < pLenArray[i];++j)
					{
						msg.emplace_back(pInputData[i][j]);
					}
				}
			}
		}
		else
		{
			assert(false);
		}
		return msg;
	}

};

  

#endif // NET_PACKETAGE_H


/** @}*/

