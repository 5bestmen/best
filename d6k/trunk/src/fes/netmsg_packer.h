/*! @file netmsg_packer.h
<PRE>
********************************************************************************
ģ����       :
�ļ���       :  netmsg_packer.h
�ļ�ʵ�ֹ��� :  ��̫����Ϣ�����
����         :  LiJin
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         :  <����˵��>
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

********************************************************************************
</PRE>
*  @brief   ��̫����Ϣ�����
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
	** \details  ���㱨�ĵĳ��ȣ�����鱨�ĳ����Ƿ�Ϸ�
	** \param nPreLen �������ı���ͷ��ͬ��ͷ�����ĳ��ȣ���ͨ����8���ֽڣ�4�ֽڵ�ͬ��ͷ��4�ֽڵĳ���
	** \param pInputData ���뻺���������������
	** \param pLenArray ÿ�����뻺�����ĳ���
	** \param nNum ����������
	** \return size_t ����-1 ��ʾ���ĳ��ȷǷ������
	** \author LiJin 
	** \date 2017��1��11�� 
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
	// �̳������ʵ�ֵĽӿ�
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
		NETMSG_TOTAL_LEN = 4,  //!< ���ⱨ�ĵĳ���
		NETMSG_HEAD_LEN = 8,   //!< ����ͷ��4���ֽڵı�ʶ����0X4C49 ,0XEB90��, 4�ֽڵı��ĳ���
	};
	using INetMsgPacker<msg_type>::PackMsg;

	/*! \fn virtual msg_type PackMsg(const unsigned char* const pInputData[], const size_t pLenArray[], size_t nNum, bool bNative = false)
	********************************************************************************************************* 
	** \brief CNetMsgPacker::PackMsg 
	** \details ���
	** \param pInputData ���뻺����
	** \param pLenArray  ���뻺�����ĳ���
	** \param nNum       ���뻺�����ĸ���
	** \param bNative    �Ƿ����ӱ���ͷ
	** \return INetMsgPacker<std::vector<unsigned char>>::msg_type 
	** \author LiJin 
	** \date 2017��1��11�� 
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
			{// �����ϱ���ͷ
				//auto head_len = nTotalLen;
				// ͬ��ͷ 
				for ( i = 0; i < NETMSG_SYNC_LEN;i++)
				{
					msg.emplace_back(arrHead[i]);
				}
				// ���ĳ���
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

