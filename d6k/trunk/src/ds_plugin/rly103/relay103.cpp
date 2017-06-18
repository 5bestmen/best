
#include <QDateTime> 
#include <QDebug>

#if 0
#include <chrono>
#include <ctime>
#endif

#include "relay103.h"
#include "relay103_gen.h"

C103Impl::C103Impl()
{
	m_nSrcStationAddr  = 1;
	m_nSrcDeviceAddr   = 1;
	m_nDestStationAddr = 1;
	m_nDestDeviceAddr  = 1;

	m_bReqCatalogFlag  = false;
	m_bBuiltDBFlag     = false;

	m_byRII = 0;
}

C103Impl::~C103Impl()
{

}

bool C103Impl::Log(const char *szLogTxt, int nLevel)
{
	extern bool LogString(const char *szLogTxt, int nLevel);
	return LogString(szLogTxt, nLevel);
}

/*! \fn unsigned int C103Impl::ISendStartDT(unsigned char *pAPDU, unsigned int nLen)const
*********************************************************************************************************
** \brief C103Impl::ISendStartDT
** \details ��װ����֡
** \param pAPDU
** \param nLen
** \return unsigned int
** \author LiJin
** \date 2016��7��5��
** \note
********************************************************************************************************/
unsigned int C103Impl::ISendStartDT(unsigned char *pAPDU, unsigned int nLen)const
{
	Q_ASSERT(pAPDU && nLen >= sizeof(Net103PacketHead));
	if (pAPDU == nullptr || nLen < sizeof(Net103PacketHead))
	{
		return 0;
	}
	Net103PacketHead *pHead = reinterpret_cast <Net103PacketHead*>(pAPDU);
	InitPacketHead(pHead, NETD_FORMAT_U);
	pHead->ControlField1 = 0x07;

	return sizeof(Net103PacketHead);
}

/*! \fn unsigned int C103Impl::IReadGroupCatalog()
*********************************************************************************************************
** \brief C103Impl::IReadGroupCatalog
** \details ����Ŀ¼
** \return unsigned int
** \author LiJin
** \date 2016��7��19��
** \note
********************************************************************************************************/
unsigned int C103Impl::IReadGroupCatalog()
{
	char apdu[100] = { 0 };
	Net103PacketHead *pHead = reinterpret_cast<Net103PacketHead*>(&apdu[0]);

	InitPacketHead(pHead, NETD_FORMAT_I);

	unsigned short nTmpNum = m_nTcpSendNum << 1;

	pHead->ControlField1 = nTmpNum & 0xff;
	pHead->ControlField2 = (nTmpNum >> 8) & 0xff;
	nTmpNum = m_nTcpRecvNum << 1;
	pHead->ControlField3 = nTmpNum & 0xff;
	pHead->ControlField4 = (nTmpNum >> 8) & 0xff;

	pHead->Length = NETDCALHEADSIZE + 8;

	char *pAsdu = &apdu[sizeof(Net103PacketHead)];
	pAsdu[ASDU_TYPE] =  TYPC_GRC_COMMAND;
	pAsdu[ASDU_VSQ]  =  VSQ_COMMON1;
	pAsdu[ASDU_COT]  =  COTC_GRC_READ;
	pAsdu[ASDU_ADDR] = (char)(m_nDestDeviceAddr % 256);

	char *pDataBuf = &pAsdu[ASDU_ADDR + 1];

	pDataBuf[GENREQCOM_FUN] = 254;
	pDataBuf[GENREQCOM_INF] = 240;
	pDataBuf[GENREQCOM_RII] = m_byRII++;
	pDataBuf[GENREQCOM_NOG] = 0;

	size_t nLen = NETDHEADSIZE + 8;
	SendData(apdu, nLen);

	IncreaseNumber(m_nTcpSendNum);

	std::string szLog = "Req catalog from " + GetRemoteEndPoint();

	Log(szLog.c_str(), 1);

	return nLen;
}

/*! \fn unsigned int C103Impl::PackTimeSync(unsigned char *pAsdu)
*********************************************************************************************************
** \brief C103Impl::PackTimeSync
** \details ASDU6 ��ʱ
** \param pAsdu
** \return unsigned int
** \author LiJin
** \date 2016��6��29��
** \note
********************************************************************************************************/
unsigned int C103Impl::PackTimeSync(unsigned char *pAsdu)
{
#if 0
	using std::chrono::system_clock;
	system_clock::time_point cur_tp = system_clock::now();
	std::time_t tNow = system_clock::to_time_t(cur_tp);
	struct  tm *pTm = localtime(&tNow);
#endif
	pAsdu[ASDU_TYPE] = TYPC_TIMESYNC;
	pAsdu[ASDU_VSQ]  = VSQ_COMMON1;
	pAsdu[ASDU_COT]  = COTC_TIMESYNC;
	pAsdu[ASDU_ADDR] = 255;
	pAsdu[ASDU_FUN]  = 255;
	pAsdu[ASDU_INF]  = 0;

	QDateTime cur_t = QDateTime::currentDateTime();

	unsigned char *pTimeBuff = &pAsdu[ASDU_INF + 1];

	unsigned int nMs = cur_t.time().second() * 1000 + cur_t.time().msec();

	pTimeBuff[LONGTIME_MSEC]     = nMs % 256;
	pTimeBuff[LONGTIME_MSEC + 1] = nMs / 256;

	pTimeBuff[LONGTIME_MIN]  = cur_t.time().minute();
	pTimeBuff[LONGTIME_HOUR] = cur_t.time().hour();
	pTimeBuff[LONGTIME_DAY]  = cur_t.date().day();
	pTimeBuff[LONGTIME_DAY] |= static_cast<unsigned char> (cur_t.date().dayOfWeek() << 5);
	pTimeBuff[LONGTIME_MON]  = cur_t.date().month();
	pTimeBuff[LONGTIME_YEAR] = cur_t.date().year();

	return 13;
}

/*! \fn unsigned int  C103Impl::PackGeneralQuary(unsigned char *pAsdu)
*********************************************************************************************************
** \brief C103Impl::PackGeneralQuary
** \details �ܲ�ѯ
** \param pAsdu
** \return unsigned int
** \author LiJin
** \date 2016��7��1��
** \note
********************************************************************************************************/
unsigned int  C103Impl::PackGeneralQuary(unsigned char *pAsdu)
{
	pAsdu[ASDU_TYPE] = TYPC_GRC_COMMAND; // ͨ�÷������
	pAsdu[ASDU_VSQ] = VSQ_COMMON1;
	pAsdu[ASDU_COT] = COTC_GENQUERY;

	// ��ַ
	pAsdu[ASDU_ADDR] = 255;  // װ��Ӧ�ò㹫����ַ

	uchar* pDataRec = (uchar*)&pAsdu[4];
	pDataRec[GENREQCOM_FUN] = 254; // ��������
	pDataRec[GENREQCOM_INF] = 245; // ��Ϣ���
	pDataRec[GENREQCOM_RII] = m_byRII++;
	pDataRec[GENREQCOM_NOG] = 1;

	return 13;
}

void C103Impl::IGeneralQuary()
{

}

void C103Impl::InitPacketHead(Net103PacketHead *pHead, unsigned int nFormat) const
{
	memset(pHead, 0, sizeof(Net103PacketHead));

	pHead->Start = STARTCHAR;
	pHead->Length = NETDCALHEADSIZE;
	if (nFormat == NETD_FORMAT_S)
	{// S��ʽ��APDUֻ����APCI.
		pHead->ControlField1 = 0x01;
	}
	else if (nFormat == NETD_FORMAT_U)
	{//  U��ʽ��APDUֻ����APCI
		pHead->ControlField1 = 0x03;
	}
	else
	{// I֡ APDU������һ��ASDU.
		pHead->ControlField1 = 0x00;
	}
	pHead->ControlField2 = 0;
	pHead->ControlField3 = 0;
	pHead->ControlField4 = 0;

	pHead->SrcStationID = m_nSrcStationAddr; // ��λ�����ж�
	pHead->SrcDevID     = m_nSrcDeviceAddr;

	pHead->DestStationID = m_nDestStationAddr;
	pHead->DestDevID     = m_nSrcDeviceAddr;

}

void C103Impl::InitPacketHead(Net103PacketHead *pHead, unsigned int nFormat, unsigned char nSrcStationAddr, unsigned short nSrcDeviceAddr,
	unsigned char nDestStationAddr, unsigned short nDestDeviceAddr)
{
	pHead->Start = 0x68;
	pHead->Length = NETDCALHEADSIZE;
	if (nFormat == NETD_FORMAT_S)
	{// S��ʽ��APDUֻ����APCI.
		pHead->ControlField1 = 0x01;
	}
	else if (nFormat == NETD_FORMAT_U)
	{//  U��ʽ��APDUֻ����APCI
		pHead->ControlField1 = 0x03;
	}
	else
	{// I֡ APDU������һ��ASDU.
		pHead->ControlField1 = 0x00;
	}
	pHead->ControlField2 = 0;
	pHead->ControlField3 = 0;
	pHead->ControlField4 = 0;

	pHead->SrcStationID = nSrcStationAddr;
	pHead->SrcDevID     = nSrcDeviceAddr;

	if (1)
	{
		pHead->DestStationID = nDestStationAddr;
		pHead->DestDevID     = nSrcDeviceAddr;
	}
	else
	{
		pHead->DestStationID = 0xFF;
		pHead->DestDevID     = 0xFFFF; //�㲥
	}
}

unsigned int C103Impl::GetFrameType(Net103PacketHead *pHead)
{
	if (pHead->ControlField1 & 0x01)
	{
		if (pHead->ControlField1 & 0x02)
		{// U֡
			return NETD_FORMAT_U;
		}
		else
		{
			return NETD_FORMAT_S;
		}
	}
	else
	{// I ֡
		return NETD_FORMAT_I;
	}
	//	return NETD_FORMAT_ERR;
}

/*! \fn bool C103Impl::HandleMsg(const char *pBuff, size_t nLen)
*********************************************************************************************************
** \brief C103Impl::HandleMsg
** \details ������յ�������
** \param pBuff
** \param nLen
** \return bool
** \author LiJin
** \date 2016��7��18��
** \note
********************************************************************************************************/
bool C103Impl::HandleMsg(const char *pBuff, size_t nLen)
{
	Q_ASSERT(pBuff);
	Q_ASSERT(nLen >= sizeof(Net103PacketHead));

	if (pBuff == nullptr || nLen < sizeof(Net103PacketHead))
		return false;

	Net103PacketHead *pHead = reinterpret_cast<Net103PacketHead*> (const_cast<char *>(pBuff));
	Q_ASSERT(pHead->Start == STARTCHAR);
	if (pHead->Start != STARTCHAR)
	{
		Log("sync head err", 1);
		return false;
	}
	unsigned int nRet = GetFrameType(pHead);
	if (nRet == NETD_FORMAT_U)
	{
		return HandleUMsg(pHead);
	}
	else if (nRet == NETD_FORMAT_I)
	{

	}

	return true;
}

/*! \fn bool C103Impl::HandleASDU10(unsigned char *pAsdu, unsigned int nLen)
*********************************************************************************************************
** \brief C103Impl::HandleASDU10
** \details ASDU10��0AH��ͨ�÷���������Ӧ���װ����Ӧ�Ķ�Ŀ¼��
** \param pAsdu
** \param nLen
** \return bool
** \author LiJin
** \date 2016��7��20��
** \note
********************************************************************************************************/
bool C103Impl::HandleASDU10(unsigned char *pAsdu, unsigned int nLen)
{
	Q_ASSERT(pAsdu && nLen > 9);
	if (pAsdu == nullptr || nLen < 9)
	{
		return false;
	}

	IEC103_ASDU_HEAD *pAsduHead = (IEC103_ASDU_HEAD *)pAsdu;

	//����rii���ж�
	if (pAsduHead->rii != m_byRII)
	{
		return false;
	}

	switch (pAsduHead->inf)
	{
	case INF_M_WR_ITEM_CON:
	case INF_M_WR_ITEM_EXE:
	case INF_M_WR_ITEM_CANCEL:
		break;

	case INF_M_RD_ONE_GROUP_ALL_ITM:
		if (pAsduHead->cot == M_COT_RSP_GCRD_VAL)
		{
			AsduParser241(pAsdu, nLen);
		}
		break;

	case INF_M_RD_ONE_ITM_ATTR:
		break;

	case INF_M_GI_END:
		break;

	case INF_M_RD_ALL_GROUP_TLT:
		if (pAsduHead->cot == M_COT_RSP_GCRD_VAL)
		{
			AsduParser240(pAsdu, nLen);
		}
		break;

	case INF_M_RD_ONE_ITM_DIR:
		break;

	default:
		break;

	}

	return true;
}

bool C103Impl::AsduParser240(unsigned char *pAsdu, unsigned int nLen)
{
	NGD_DATA ngdData;
	NGD *pNgdTemp;
	uint8_t nWidth, bufPtr;
	uint8_t ngdNum = pAsdu[ASDU_HEAD_LEN] & 0x3f;

	if (this->CalcGrcDataLen(pAsdu) >= nLen)
	{
		//���յ��ı��ĳ���С��ʵ��ʵ�����ݵĳ���
		return false;
	}

	ngdData.ngd_num = ngdNum;
	for (uint8_t ngdIndex = 0; ngdIndex < ngdNum; ngdIndex++)
	{
		pNgdTemp = (NGD *)&ngdData.ngd[ngdIndex];
		bufPtr = ASDU_HEAD_LEN + 1;
		pNgdTemp->gin.group = pAsdu[bufPtr++];
		pNgdTemp->gin.entry = pAsdu[bufPtr++];
		if (pNgdTemp->gin.entry != 0)
		{
			//��Ŀ��һ��Ϊ0
			return false;
		}
		pNgdTemp->kod = pAsdu[bufPtr++];
		if ((pNgdTemp->kod != IEC103_KOD_DESCRIPTION) || (pNgdTemp->kod >= MAX_IEC103_KOD_TYPE))
		{
			//KOD����Ϊ����
			return false;
		}
		pNgdTemp->gdd.dataType = pAsdu[bufPtr++];
		if (pNgdTemp->gdd.dataType != IEC103_GDD_TYPE_ASCII)
		{
			//�������ͱ���ΪASCII
			return false;
		}
		pNgdTemp->gdd.dataSize = pAsdu[bufPtr++];
		pNgdTemp->gdd.contNum  = pAsdu[bufPtr++];
		if (pNgdTemp->gdd.contNum != 1)
		{
			//������Ŀ����Ϊ1
			return false;
		}
		nWidth = pNgdTemp->gdd.dataSize * pNgdTemp->gdd.contNum;
		memcpy(pNgdTemp->gid, &pAsdu[bufPtr], nWidth);
		bufPtr += nWidth;
	}

	//������Ŀ¼���

	return true;
}

bool C103Impl::AsduParser241(unsigned char *pAsdu, unsigned int nLen)
{
	NGD_DATA ngdData;
	NGD *pNgdTemp;
	uint8_t nWidth, bufPtr;
	uint8_t ngdNum = pAsdu[ASDU_HEAD_LEN] & 0x3f;

	if (this->CalcGrcDataLen(pAsdu) >= nLen)
	{
		//���յ��ı��ĳ���С��ʵ��ʵ�����ݵĳ���
		return false;
	}

	ngdData.ngd_num = ngdNum;
	for (uint8_t ngdIndex = 0; ngdIndex < ngdNum; ngdIndex++)
	{
		pNgdTemp = (NGD *)&ngdData.ngd[ngdIndex];
		bufPtr = ASDU_HEAD_LEN + 1;
		pNgdTemp->gin.group = pAsdu[bufPtr++];
		pNgdTemp->gin.entry = pAsdu[bufPtr++];
		pNgdTemp->kod       = pAsdu[bufPtr++];
		if ((pNgdTemp->kod != IEC103_KOD_ACTUAL_V) || (pNgdTemp->kod >= MAX_IEC103_KOD_TYPE))
		{
			//KOD����Ϊʵ��ֵ
			return false;
		}
		pNgdTemp->gdd.dataType = pAsdu[bufPtr++];
		pNgdTemp->gdd.dataSize = pAsdu[bufPtr++];
		pNgdTemp->gdd.contNum  = pAsdu[bufPtr++];
		nWidth = pNgdTemp->gdd.dataSize * pNgdTemp->gdd.contNum;
		memcpy(pNgdTemp->gid, &pAsdu[bufPtr], nWidth);
		bufPtr += nWidth;
	}

	//������Ŀ¼���

	return true;
}

unsigned int C103Impl::CalcGrcDataLen(unsigned char *pGdBuf)
{
	Q_ASSERT(pGdBuf);
	if (pGdBuf == nullptr)
	{
		return 0;
	}

	unsigned int nLen = 0;
	unsigned char *pHead = pGdBuf;

	// ͨ�÷������ݼ���Ŀ
	unsigned int nCalalogNum = static_cast <unsigned char> (pGdBuf[0] & 0x3f);
	if (nCalalogNum > 0)
	{
		unsigned int i = 0;
		for (i = 0; i < nCalalogNum; i++)
		{
			if (pHead[0] == 2) //GDD->TYPE
			{// !TODO ����λ�� 
				Q_ASSERT(false);
			}
			else
			{// 6 = 2�ֽ�GIN 1�ֽ�KOD 3�ֽ�GDD
				nLen  += 6 + pHead[GENDATAREC_GDD + GDDTYPE_SIZ] * pHead[GENDATAREC_GDD + GDDTYPE_NUM];
				pHead += 6 + pHead[GENDATAREC_GDD + GDDTYPE_SIZ] * pHead[GENDATAREC_GDD + GDDTYPE_NUM];
			}
		}
	}

	return nLen;
}

bool C103Impl::HandleReadCatalog(unsigned char *pData, unsigned int nLen)
{
	unsigned char *pNGD = &pData[GENDATACOM_NGD];

	return true;
}

/*! \fn bool C103Impl::HandleUMsg(Net103PacketHead *pHead)
*********************************************************************************************************
** \brief C103Impl::HandleUMsg
** \details ����U֡
** \param pHead
** \return bool
** \author LiJin
** \date 2016��7��18��
** \note
********************************************************************************************************/
bool C103Impl::HandleUMsg(Net103PacketHead *pHead)
{
	Q_ASSERT(pHead);
	if (pHead == nullptr)
		return false;

	//�յ�����֡
	if (pHead->ControlField1 == TESTFR_ACT)
	{
		// log �յ�����֡
		std::string szLog = "Recv test frame from " + GetRemoteEndPoint();

		Log(szLog.c_str(), 1);

		Net103PacketHead newHead;
		InitPacketHead(&newHead, NETD_FORMAT_U);
		newHead.ControlField1 = TESTFR_CON;

		newHead.SrcStationID = m_nSrcStationAddr;
		newHead.SrcDevID     = m_nSrcDeviceAddr;

		newHead.DestStationID = m_nDestStationAddr;
		newHead.DestDevID     = m_nSrcDeviceAddr;

		szLog = "Send test confirm frame to " + GetRemoteEndPoint();
		Log(szLog.c_str(), 1);

		SendData(&newHead, nullptr);
	}

	return true;
}

/*! \fn bool C103Impl::SendData(Net103PacketHead *pHead, const char *pDataBuf)
*********************************************************************************************************
** \brief C103Impl::SendData
** \details ��������
** \param pHead
** \param pDataBuf
** \return bool
** \author LiJin
** \date 2016��7��18��
** \note
********************************************************************************************************/
bool C103Impl::SendData(Net103PacketHead *pHead, const char *pDataBuf)
{
	Q_ASSERT(m_fnSendData);
	if (m_fnSendData)
	{
		if (pDataBuf)
		{
			const char*  arrString[2] = { nullptr, nullptr };
			size_t arrLen[2] = { 0, 0 };
			arrString[0] = reinterpret_cast<char*> (pHead);
			arrLen[0] = sizeof(Net103PacketHead);

			arrString[1] = (pDataBuf);
			arrLen[1] = pHead->Length - NETDCALHEADSIZE;

			return m_fnSendData(arrString, arrLen, 2);
		}
		else
		{
			const char*  arrString[1] = { nullptr };
			size_t arrLen[1] = { 0 };
			arrString[0] = reinterpret_cast<char*> (pHead);
			arrLen[0] = sizeof(Net103PacketHead);

			return m_fnSendData(arrString, arrLen, 1);
		}
	}
	return false;
}

bool C103Impl::SendData(const char *pDataBuf, size_t nLen)
{
	Q_ASSERT(pDataBuf && nLen > 0);
	if (pDataBuf == nullptr || nLen == 0 || m_fnSendData == nullptr)
	{
		return false;
	}
	return m_fnSendData(&pDataBuf, &nLen, 1);
}

void  C103Impl::IncreaseNumber(unsigned short& nNumber)
{
	if (nNumber >= MAX_DATA_NUMBER)
		nNumber = 0;
	else
		nNumber++;
}