#include "apdusender.h"
#include "asdu_info.h"
#include "define_104.h"
#include "socketthread.h"
#include "commplugin.h"
#include "devicestudio/main_module.h"

/*********************************************************************************************************
** \brief CApduSender(QObject *parent)
** \details 发送类构造函数
** \param argl   父类指针
** \param arg2   
** \return void
** \author ww
** \date 2016年5月27日
** \note
********************************************************************************************************/
CApduSender::CApduSender(QObject *parent):
    QObject(parent)
{
	//104管理类
    m_pComm104Pln = qobject_cast<CCommPlugin*>(parent);
    Q_ASSERT(m_pComm104Pln);

    m_k  = 0;
    m_w = 0;
    m_nSendNum = 0;
}

CApduSender::~CApduSender()
{

}

/*********************************************************************************************************
** \brief Send_U(int nType)
** \details 发送U格式类型消息
** \param argl   数据类型
** \param arg2
** \return bool  发送成功失败
** \author ww
** \date 2016年5月27日
** \note
********************************************************************************************************/
bool CApduSender::Send_U(int nType)
{
    APCI apci;
    apci.head = STARTCHAR;
    apci.len = 0x04;
    apci.control0 = nType;
    apci.control1 = 0;
    apci.control2 = 0;
    apci.control3 = 0;
    //发送
    if (m_pComm104Pln == NULL)
    {
        return false;
    }
    if (!m_pComm104Pln->WriteApduData((char*)(&apci),sizeof(APCI)))
    {
        return false;
    }

    m_pComm104Pln->OnSendFrame(TYPE_U,nType);
    return true;
}

/*********************************************************************************************************
** \brief Send_S()
** \details 发送S格式类型消息
** \param argl   
** \param arg2
** \return bool  发送成功失败
** \author ww
** \date 2016年5月27日
** \note
********************************************************************************************************/
bool CApduSender::Send_S()
{
    APCI apci;
    apci.head = STARTCHAR;
    apci.len = 0x04;
    apci.control0 = 0x01;
    apci.control1 = 0;
    apci.SetRecvNum(m_pComm104Pln->m_nRecvNum);

    //发送
    if (m_pComm104Pln == NULL)
    {
        return false;
    }
    if (!m_pComm104Pln->WriteApduData((char*)(&apci),sizeof(APCI)))
    {
        return false;
    }

    m_pComm104Pln->OnSendFrame(TYPE_S);

    return true;
}

bool CApduSender::Send_S_BeforeClose(int nRecvNum)
{
	APCI apci;
	apci.head = STARTCHAR;
	apci.len = 0x04;
	apci.control0 = 0x01;
	apci.control1 = 0;
	apci.SetRecvNum(nRecvNum);

	if (!m_pComm104Pln->WriteApduData((char*)(&apci), sizeof(APCI)))
	{
		return false;
	}

	m_pComm104Pln->OnSendFrame(TYPE_S);

	//m_pImpl->LogN(MCB_SEND,TYPE_S,"After OnSendFrame TYPE_S");

	return true;
}

/*********************************************************************************************************
** \brief Send_I(char *pBuff, int nLen)
** \details 发送I格式类型消息
** \param argl 需要发送的数据
** \param arg2 数据的长度
** \return bool  发送状态编码
** \author ww
** \date 2016年5月27日
** \note
********************************************************************************************************/
int CApduSender::Send_I(char *pBuff, int nLen)
{

    APCI* pApci = (APCI*)pBuff;
    //组织APCI
    FormatAPCI(pBuff,nLen);

    //是否能发送
    if (m_pComm104Pln->m_k >= DEFAULT_MAX_K_VALUE)  //大于设定的值将不发送I帧格式数据
    {
        return SEND_ERROR_K;
    }

    //发送
    if (m_pComm104Pln == NULL)
    {
        return SEND_ERROR_K;
    }

// 	//切换定值区和校验
// 	if ((unsigned char)pBuff[6] == D_FIX_SWITCH && pApci->len == 17)
// 	{
// 		pBuff[17] = checkAllData(pBuff + 2, pApci->len);
// 	}
// 	else if ((unsigned char)pBuff[6] == D_FIX_SEARCH && pApci->len == 15)
// 	{
// 		//查询定值区
// 		pBuff[15] = checkAllData(pBuff + 2, pApci->len);
// 	}
// 	else if ((unsigned char)pBuff[6] == D_FIX_READ && nLen> 2)
// 	{
// 		//定值读取
// 		pBuff[pApci->len] = checkAllData(pBuff + 2, pApci->len);
// 		//结束符
// 		pBuff[pApci->len+1] = 0x16;
// 
// 	}
// 	else if ((unsigned char)pBuff[6] == D_FIX_WRITE && nLen > 2)
// 	{
// 		//后面还有两位
// 		pApci->len += 2;
// 		//定值读取
// 		pBuff[pApci->len] = checkAllData(pBuff + 2, pApci->len);
// 		//结束符
// 		pBuff[pApci->len + 1] = 0x16;
// 
// 	}

	qDebug() << QByteArray(pBuff, pApci->len+2).toHex();
    if (!m_pComm104Pln->WriteApduData(pBuff,pApci->len+2))
    {
        return SEND_ERROR_SOCKET;
    }
	
    m_pComm104Pln->OnSendFrame(TYPE_I);

    return SEND_OK;
}

/*********************************************************************************************************
** \brief FormatAPCI(char* pBuff,int nAsduLen)
** \details 组织apci格式数据
** \param argl 数据
** \param arg2 数据的长度
** \return bool  成功失败标识
** \author ww
** \date 2016年5月27日
** \note
********************************************************************************************************/
bool CApduSender::FormatAPCI(char* pBuff,int nAsduLen)
{
    APCI* pApci = (APCI*)pBuff;

    pApci->head = 0x68;
    pApci->len  = 0x04 + nAsduLen;

    pApci->SetSendNum(m_pComm104Pln->m_nSendNum);
    pApci->SetRecvNum(m_pComm104Pln->m_nRecvNum);

    return true;
}

/*********************************************************************************************************
** \brief OnCommand(NBM_TELECTRL* pTelectrl)
** \details 发送召唤数据
** \param argl 召唤结构
** \param arg2 
** \return bool  成功失败标识
** \author ww
** \date 2016年5月27日
** \note
********************************************************************************************************/
bool CApduSender::OnCommand(NBM_TELECTRL* pTelectrl)
{
    bool IsSendOk = false;

    switch(pTelectrl->m_nCtrlType)
    {
	case TELECTRL_REQUEST_KWHGENERALCALL:// 电能脉冲召唤命令
	{
		IsSendOk = OnSendKwhGeneralCall(pTelectrl);
		break;
	}
	case TELECTRL_REQUEST_GENERALCALL:// 总召唤命令
	{
		IsSendOk = OnSendGeneralCall(pTelectrl);
		break;
	}
	case TELECTRL_REQUEST_SYNCTIME://时间同步
	{
		IsSendOk = OnSendSyncTime(pTelectrl);
		break;
	}
    case TELECTRL_RESET_PROCESSCALL:  //复位进程
    {
        IsSendOk = OnSendSetResetProcessRequest(pTelectrl);
        break;
    }
	case TELECTRL_REQUEST_SELECT:
	case TELECTRL_REQUEST_EXECUTE:
	case TELECTRL_REQUEST_UNSELECT:
	{
		int nCommandType = pTelectrl->m_nCommandType;
		if (nCommandType != C_SE_NA_1 && nCommandType != C_SE_NB_1 && nCommandType != C_SE_NC_1
			&& nCommandType != C_SC_NA_1 && nCommandType != C_DC_NA_1)
		{
// 			if (pTelectrl->m_nDataType == DATATYPE_ANALOG)
// 				nCommandType = m_pImpl->GetParam()->m_nAnalogTelectrlCommand;
// 			else
// 				nCommandType = m_pImpl->GetParam()->m_nBinaryTelectrlCommand;
		}
		switch (nCommandType)
		{
		case C_SE_NA_1://归一化设点48
			IsSendOk = OnSendSetAnalogNormalizeRequest(pTelectrl);
			break;
		case C_SE_NB_1://标度化设点49
			IsSendOk = OnSendSetAnalogScaleRequest(pTelectrl);
			break;
		case C_SE_NC_1://短浮点数设点50
			IsSendOk = OnSendSetAnalogShortFloatRequest(pTelectrl);
			break;
		case C_SC_NA_1://单点命令45
			IsSendOk = OnSendSetBinarySPRequest(pTelectrl);
			break;
		case C_DC_NA_1://双点命令46
			IsSendOk = OnSendSetBinaryDPRequest(pTelectrl);
			break;
		default:
			break;
		}
	}
	break;
    default:
        break;
    }

    return IsSendOk;
}

/*********************************************************************************************************
** \brief OnSendGeneralCall(NBM_TELECTRL* pTelectrl)
** \details 总电度召唤
** \param argl 召唤结构
** \param arg2
** \return bool  成功失败标识
** \author ww
** \date 2016年5月27日
** \note
********************************************************************************************************/
bool CApduSender::OnSendKwhGeneralCall(NBM_TELECTRL* pTelectrl)
{
	Q_UNUSED(pTelectrl);

	char buf[255];

	//组织ASDU101
	ASDU101* pAsdu101 = (ASDU101*)(buf + sizeof(APCI));

	pAsdu101->type = C_CI_NA_1;
	pAsdu101->vsq = 0x01;

	pAsdu101->cot.SetCot(COT_ACT);

    int nDeviceAddr = pTelectrl->m_nDeviceID;
	pAsdu101->asduAddr.SetAddr(nDeviceAddr);

	pAsdu101->m_infoaddr.SetAddr(0x00);

	pAsdu101->m_qcc = 0x45;

	int nResult = Send_I(buf, sizeof(ASDU101));

	if (nResult != SEND_OK)
	{
		return false;
	}

	return true;
}
/*********************************************************************************************************
** \brief OnSendGeneralCall(NBM_TELECTRL* pTelectrl)
** \details 组织召唤数据发送
** \param argl 召唤结构
** \param arg2
** \return bool  成功失败标识
** \author ww
** \date 2016年5月27日
** \note
********************************************************************************************************/
bool CApduSender::OnSendGeneralCall(NBM_TELECTRL* pTelectrl)
{
	Q_UNUSED(pTelectrl);
    char buf[255];

    //组织ASDU100
    ASDU100* pAsdu100 = (ASDU100*)(buf+sizeof(APCI));

    pAsdu100->type   = C_IC_NA_1;
    pAsdu100->vsq    = 0x01;

    pAsdu100->cot.SetCot(COT_ACT);

    int nDeviceAddr = pTelectrl->m_nDeviceID;
    pAsdu100->asduAddr.SetAddr(nDeviceAddr);

    pAsdu100->m_infoaddr.SetAddr(0x00);

    pAsdu100->m_qoi = 0x14;

    int nResult = Send_I(buf,sizeof(ASDU100));

    if(nResult != SEND_OK)
    {
        return false;
    }

    return true;
}

/*********************************************************************************************************
** \brief OnSendGeneralCall(NBM_TELECTRL* pTelectrl)
** \details 组织召唤数据发送
** \param argl 召唤结构
** \param arg2
** \return bool  成功失败标识
** \author ww
** \date 2016年5月27日
** \note
********************************************************************************************************/
bool CApduSender::OnSendSyncTime(NBM_TELECTRL* pTelectrl)
{
	Q_UNUSED(pTelectrl);
	char buf[255];

	//组织ASDU103
	ASDU103* pAsdu103 = (ASDU103*)(buf + sizeof(APCI));

	pAsdu103->type = C_CS_NA_1;
	pAsdu103->vsq = 0x01;

	pAsdu103->cot.SetCot(COT_ACT);

    int nDeviceAddr = pTelectrl->m_nDeviceID;
	pAsdu103->asduAddr.SetAddr(nDeviceAddr);

	pAsdu103->m_infoaddr.SetAddr(0x00);

	struct CP56Time2a* cp56time2a = (struct CP56Time2a*)(&(pAsdu103->m_time));

	QDateTime tvTime = QDateTime::currentDateTime(); 
	cp56time2a->Initialize(tvTime.date().year(), tvTime.date().month(), tvTime.date().day(), tvTime.time().hour(), tvTime.time().minute(), tvTime.time().second(), tvTime.time().msec());

	int nResult = Send_I(buf, sizeof(ASDU103));

	if (nResult != SEND_OK)
	{
		return false;
	}

	return true;
}


bool CApduSender::OnSendSetAnalogNormalizeRequest(NBM_TELECTRL* pTelectrl)//归一化设点
{
	char buf[255];

	//组织ASDU48
	ASDU48* pAsdu48 = (ASDU48*)(buf + sizeof(APCI));

	pAsdu48->type = C_SE_NA_1;
	pAsdu48->vsq = 0x01;


    int nDeviceAddr = pTelectrl->m_nDeviceID;
	pAsdu48->asduAddr.SetAddr(nDeviceAddr);

	pAsdu48->m_infoaddr.SetAddr(pTelectrl->m_nDataID);

	pAsdu48->SetValue(pTelectrl->m_fValue);

	pAsdu48->cot.SetCot((pTelectrl->m_nCtrlType == TELECTRL_REQUEST_UNSELECT) ? COT_DEACT : COT_ACT);
	pAsdu48->m_qos.SelectOrExcute = (pTelectrl->m_nCtrlType == TELECTRL_REQUEST_SELECT) ? 1 : 0;
	pAsdu48->m_qos.QL = 0;


	int nResult = Send_I(buf, sizeof(ASDU48));

	if (nResult != SEND_OK)
	{
		return false;
	}

	return true;
}


bool CApduSender::OnSendSetAnalogScaleRequest(NBM_TELECTRL* pTelectrl)//标度化设点
{
	char buf[255];

	//组织ASDU49
	ASDU49* pAsdu49 = (ASDU49*)(buf + sizeof(APCI));

	pAsdu49->type = C_SE_NB_1;
	pAsdu49->vsq = 0x01;


    int nDeviceAddr = pTelectrl->m_nDeviceID;
	pAsdu49->asduAddr.SetAddr(nDeviceAddr);

	pAsdu49->m_infoaddr.SetAddr(pTelectrl->m_nDataID );

	pAsdu49->SetValue(pTelectrl->m_fValue);

	pAsdu49->cot.SetCot((pTelectrl->m_nCtrlType == TELECTRL_REQUEST_UNSELECT) ? COT_DEACT : COT_ACT);
	pAsdu49->m_qos.SelectOrExcute = (pTelectrl->m_nCtrlType == TELECTRL_REQUEST_SELECT) ? 1 : 0;
	pAsdu49->m_qos.QL = 0;


	int nResult = Send_I(buf, sizeof(ASDU49));

	if (nResult != SEND_OK)
	{
		return false;
	}

	return true;
}


bool CApduSender::OnSendSetAnalogShortFloatRequest(NBM_TELECTRL* pTelectrl)//短浮点数设点
{
	char buf[255];

	//组织ASDU50
	ASDU50* pAsdu50 = (ASDU50*)(buf + sizeof(APCI));

	pAsdu50->type = C_SE_NC_1;
	pAsdu50->vsq = 0x01;


    int nDeviceAddr = pTelectrl->m_nDeviceID;
	pAsdu50->asduAddr.SetAddr(nDeviceAddr);

	pAsdu50->m_infoaddr.SetAddr(pTelectrl->m_nDataID);

	pAsdu50->SetValue(pTelectrl->m_fValue);

	pAsdu50->cot.SetCot((pTelectrl->m_nCtrlType == TELECTRL_REQUEST_UNSELECT) ? COT_DEACT : COT_ACT);
	pAsdu50->m_qos.SelectOrExcute = (pTelectrl->m_nCtrlType == TELECTRL_REQUEST_SELECT) ? 1 : 0;
	pAsdu50->m_qos.QL = 0;


	int nResult = Send_I(buf, sizeof(ASDU50));

	if (nResult != SEND_OK)
	{
		return false;
	}

	return true;
}


bool CApduSender::OnSendSetBinarySPRequest(NBM_TELECTRL* pTelectrl)
{
	char buf[255];

	//组织ASDU45
	ASDU45* pAsdu45 = (ASDU45*)(buf + sizeof(APCI));

	pAsdu45->type = C_SC_NA_1;
	pAsdu45->vsq = 0x01;

    int nDeviceAddr = pTelectrl->m_nDeviceID;
	pAsdu45->asduAddr.SetAddr(nDeviceAddr);

	int nBinaryTelectrlStartAddr = 0;

	pAsdu45->m_infoaddr.SetAddr(pTelectrl->m_nDataID + nBinaryTelectrlStartAddr);

	pAsdu45->cot.SetCot((pTelectrl->m_nCtrlType == TELECTRL_REQUEST_UNSELECT) ? COT_DEACT : COT_ACT);
	pAsdu45->m_sco.SelectOrExcute = (pTelectrl->m_nCtrlType == TELECTRL_REQUEST_SELECT) ? 1 : 0;
	pAsdu45->m_sco.QU = 0;
	pAsdu45->m_sco.RES = 0;
	pAsdu45->m_sco.SCS = pTelectrl->m_fValue;

	int nResult = Send_I(buf, sizeof(ASDU45));

	if (nResult != SEND_OK)
	{
		return false;
	}
	return true;
}

bool CApduSender::OnSendSetBinaryDPRequest(NBM_TELECTRL* pTelectrl)
{
	char buf[255];

	//组织ASDU46
	ASDU46* pAsdu46 = (ASDU46*)(buf + sizeof(APCI));

	pAsdu46->type = C_DC_NA_1;
	pAsdu46->vsq = 0x01;

    int nDeviceAddr = pTelectrl->m_nDeviceID;
	pAsdu46->asduAddr.SetAddr(nDeviceAddr);

	int nBinaryTelectrlStartAddr = 0;
	pAsdu46->m_infoaddr.SetAddr(pTelectrl->m_nDataID + nBinaryTelectrlStartAddr);

	pAsdu46->cot.SetCot((pTelectrl->m_nCtrlType == TELECTRL_REQUEST_UNSELECT) ? COT_DEACT : COT_ACT);
	pAsdu46->m_dco.SelectOrExcute = (pTelectrl->m_nCtrlType == TELECTRL_REQUEST_SELECT) ? 1 : 0;
	pAsdu46->m_dco.QU = 0;
	pAsdu46->m_dco.DCS = pTelectrl->m_fValue + 1;//双点01分，10合

	int nResult = Send_I(buf, sizeof(ASDU46));

	if (nResult != SEND_OK)
	{
		return true;
	}

	return false;
}

//复位进程
bool CApduSender::OnSendSetResetProcessRequest(NBM_TELECTRL* pTelectrl)
{
    char buf[255];

    //组织ASDU100
    ASDU105* pAsdu105 = (ASDU105*)(buf + sizeof(APCI));

    pAsdu105->type = C_RP_NA_1;
    pAsdu105->vsq = 0x01;

    pAsdu105->cot.SetCot(COT_ACT);

    int nDeviceAddr = pTelectrl->m_nDeviceID;
    pAsdu105->asduAddr.SetAddr(nDeviceAddr);

    pAsdu105->m_infoaddr.SetAddr(0x00);

    pAsdu105->m_qcc = 0x01;

    int nResult = Send_I(buf, sizeof(ASDU100));

    if (nResult != SEND_OK)
    {
        return false;
    }

    return true;
}


//定值获取
bool CApduSender::OnSendDevDataRequest(DEV_BASE *pRequestDz)
{

	if (pRequestDz->m_nCommandType == D_FIX_READ)         //定值读取
	{
		char buf[255] = { 0 };

		//组织ASDU46
		ASDUDZ* pAsdudz = (ASDUDZ*)(buf + sizeof(APCI));

		pAsdudz->type = pRequestDz->m_nCommandType;

		pAsdudz->vsq = 0x01;


		pAsdudz->asduAddr.SetAddr(pRequestDz->m_nAsduID);
		//传送原因
		pAsdudz->cot.SetCot(pRequestDz->m_nCto);

		//定值区号
		pAsdudz->infoFixCode.SetAddr(m_pComm104Pln->GetFtpModule()->GetFixCode());

		int nSetIndex = 0;

		for (int i = 0; i < pRequestDz->m_lstData.count(); i++)
		{
			//赋值

			pAsdudz->m_data[nSetIndex].infoaddr.SetAddr(pRequestDz->m_lstData.at(i).nAddress);


			if ((i + 1) % (pAsdudz->MAX_DATA_PER_ASDUDZ_RD) == 0)
			{
				//当达到最大数时  发送数据包
				pAsdudz->SetItemCount(nSetIndex + 1);
				int nResult = Send_I(buf, sizeof(ASDUDZ));
				if (nResult != SEND_OK)
				{
					return false;
				}
				//清空数据
				memset(pAsdudz->m_data, 0, pAsdudz->MAX_DATA_PER_ASDUDZ_RD);
				nSetIndex = 0;
			}

			nSetIndex++;
		}

		if (nSetIndex != pAsdudz->MAX_DATA_PER_ASDUDZ_RD)
		{
			pAsdudz->SetItemCount(nSetIndex);

			int nResult = Send_I(buf, pAsdudz->GetAsduDzLength());

			if (nResult != SEND_OK)
			{
				return false;
			}
		}
	}
	else if (pRequestDz->m_nCommandType == D_FIX_WRITE)
	{
		//定值写入
		char buf[255] = { 0 };

		//定值写入
		ASDU_BASE* pAsdudz = (ASDU_BASE*)(buf + sizeof(APCI));

		pAsdudz->type = pRequestDz->m_nCommandType;

		pAsdudz->vsq = 0x01;

		//asdu公共地址
		pAsdudz->asduAddr.SetAddr(pRequestDz->m_nAsduID);
		//传送原因
		pAsdudz->cot.SetCot(pRequestDz->m_nCto);

		//定值区号
		ASDUADDR2 *pAreaCode = (ASDUADDR2*)(buf + sizeof(APCI) + sizeof(ASDU_BASE));
		pAreaCode->SetAddr(m_pComm104Pln->GetFtpModule()->GetFixCode());

		//参数特征标识   TODO.....................
		DVFLAG *pFlag = (DVFLAG*)(buf+sizeof(APCI) + sizeof(ASDU_BASE) + sizeof(ASDUADDR2));
		pFlag->CONT = 0;
		pFlag->RES = 0;
		pFlag->CR = 0;
		pFlag->SE = 1;
		//当前数据在所节点
		int nSetIndex = 0;
		//数据个数

		//数据长度
		int nPagLength = sizeof(APCI) + sizeof(ASDU_BASE) + sizeof(ASDUADDR2) + sizeof(unsigned char);

		for (int i = 0; i < pRequestDz->m_lstData.count(); i++)
		{
			//信息体地址
			INFOADDR3 *pAddr = (INFOADDR3*)(buf+nPagLength+nSetIndex);

			pAddr->SetAddr(pRequestDz->m_lstData.at(i).nAddress);

			//tag类型
			buf[nPagLength + nSetIndex+sizeof(INFOADDR3)] = pRequestDz->m_lstData.at(i).nTagType;

			//数据长度
			buf[nPagLength + nSetIndex + sizeof(INFOADDR3)+1] = pRequestDz->m_lstData.at(i).nLength;
			//数据值
			if (pRequestDz->m_lstData.at(i).nTagType == 2)
			{
				//int
				int *pp = (int *)(buf+nPagLength + nSetIndex + sizeof(INFOADDR3)+2);
				*pp = pRequestDz->m_lstData.at(i).strValue.toInt();
			}
			else if (pRequestDz->m_lstData.at(i).nTagType == 45)
			{
				//u short
				unsigned short *puData = (unsigned short *)(buf + nPagLength + nSetIndex + sizeof(INFOADDR3) + 2);
				*puData = pRequestDz->m_lstData.at(i).strValue.toUShort();
			}
			else if (pRequestDz->m_lstData.at(i).nTagType == 4)
			{
				//string
				strncpy(buf + nPagLength + nSetIndex + sizeof(INFOADDR3) + 2, pRequestDz->m_lstData.at(i).strValue.toLocal8Bit().data(), pRequestDz->m_lstData.at(i).nLength);
			}
			else if (pRequestDz->m_lstData.at(i).nTagType == 1)
			{
				//bool
				bool *pp = (bool *)(buf + nPagLength + nSetIndex + sizeof(INFOADDR3) + 2);
				*pp = pRequestDz->m_lstData.at(i).strValue.toInt();
			}
			else if (pRequestDz->m_lstData.at(i).nTagType == 38)
			{
				//float
				char *pp = (buf + nPagLength + nSetIndex + sizeof(INFOADDR3) + 2);

				float tt = pRequestDz->m_lstData.at(i).strValue.toFloat();
				pp[0] = LSB(LSW(*(uint32_t *)(&tt)));
				pp[1] = MSB(LSW(*(uint32_t *)(&tt)));
				pp[2] = LSB(MSW(*(uint32_t *)(&tt)));
				pp[3] = MSB(MSW(*(uint32_t *)(&tt)));

				*pp = pRequestDz->m_lstData.at(i).strValue.toFloat();

			}



			nSetIndex += nPagLength + sizeof(INFOADDR3) + 2 + pRequestDz->m_lstData.at(i).nLength;
		}

		int nResult = Send_I(buf, nSetIndex- sizeof(APCI));

		if (nResult != SEND_OK)
		{
			return false;
		}



		//发送确认消息
		char bufg[255];

		//组织ASDU100
		ASDU203_GH* pAsduGh = (ASDU203_GH*)(bufg + sizeof(APCI));


		pAsduGh->type = pRequestDz->m_nCommandType;

		pAsduGh->vsq = 0x01;


		pAsduGh->asduAddr.SetAddr(pRequestDz->m_nAsduID);
		//传送原因
		pAsduGh->cot.SetCot(pRequestDz->m_nCto);

		//定值区号
		pAsduGh->m_infoFixCode.SetAddr(m_pComm104Pln->GetFtpModule()->GetFixCode());
		//TODO

		pAsduGh->m_featureCode.CONT = 0;
		pAsduGh->m_featureCode.RES = 0;
		pAsduGh->m_featureCode.CR = 0;
		pAsduGh->m_featureCode.SE = 0;

		nResult = Send_I(bufg, sizeof(ASDU203_GH));

		if (nResult != SEND_OK)
		{
			return false;
		}

		return true;

	}


//     if (pRequestDz->m_nCommandType == 48 || pRequestDz->m_nCommandType == 136)
//     {
//         char buf[255] = { 0 };
// 
//         //组织ASDU46
//         ASDUDZ* pAsdudz = (ASDUDZ*)(buf + sizeof(APCI));
// 
//         pAsdudz->type = pRequestDz->m_nCommandType;
// 
//         pAsdudz->vsq = 0x01;
// 
// 
//         pAsdudz->asduAddr.SetAddr(pRequestDz->m_nAsduID);
//         //传送原因
//         pAsdudz->cot.SetCot(pRequestDz->m_nCto);
// 
//         int nSetIndex = 0;
// 
//         for (int i = 0; i < pRequestDz->m_lstData.count(); i++)
//         {
//             //赋值
//             if (0 == nSetIndex)
//             {
//                 pAsdudz->infoaddr.SetAddr(pRequestDz->m_lstData.at(i).nAddress);
//             }
//             //赋值
//             //pAsdudz->m_data[nSetIndex].infoaddr.SetAddr(pRequestDz->m_lstData.at(i).nAddress);
//             pAsdudz->m_data[nSetIndex].m_nValue = pRequestDz->m_lstData.at(i).nValue;
//             //pAsdudz->m_dzQos.DQ = (pRequestDz->m_nQos == TELECTRL_REQUEST_SELECT) ? 0 : 1;
//             pAsdudz->m_dzQos.DQ = 0;
//             pAsdudz->m_dzQos.QOS = 0;
//             pAsdudz->m_dzQos.RES = 0;
//             pAsdudz->m_dzQos.QU = 0;
// 
//             if ((i + 1) % (pAsdudz->MAX_DATA_PER_ASDUDZ_RD) == 0)
//             {
//                 //当达到最大数时  发送数据包
//                 pAsdudz->SetItemCount(nSetIndex + 1);
//                 int nResult = Send_I(buf, sizeof(ASDUDZ));
//                 if (nResult != SEND_OK)
//                 {
//                     return false;
//                 }
//                 //清空数据
//                 memset(pAsdudz->m_data, 0, pAsdudz->MAX_DATA_PER_ASDUDZ_RD);
//                 nSetIndex = 0;
//             }
//             else
//             {
//                 nSetIndex++;
//             }
// 
//             
//         }
// 
//         if (nSetIndex != pAsdudz->MAX_DATA_PER_ASDUDZ_RD && nSetIndex != 0)
//         {
//             pAsdudz->SetItemCount(nSetIndex);
//             int nResult = Send_I(buf, pAsdudz->GetAsduDzLength());
// 
//             if (nResult != SEND_OK)
//             {
//                 return false;
//             }
//         }
//     }
//     else if (pRequestDz->m_nCommandType == 102 || pRequestDz->m_nCommandType == 132)
//     {
//         char buf[255] = { 0 };
// 
//         //组织ASDU46
//         ASDUDZ2* pAsdudz = (ASDUDZ2*)(buf + sizeof(APCI));
// 
//         pAsdudz->type = pRequestDz->m_nCommandType;
// 
//         pAsdudz->vsq = 0x01;
// 
// 
//         pAsdudz->asduAddr.SetAddr(pRequestDz->m_nAsduID);
//         //传送原因
//         pAsdudz->cot.SetCot(pRequestDz->m_nCto);
// 
//         
// 
//         int nSetIndex = 0;
// 
//         for (int i = 0; i < pRequestDz->m_lstData.count(); i++)
//         {
//             //赋值
//             if (i == 0)
//             {
//                 pAsdudz->m_data[nSetIndex].infoaddr.SetAddr(pRequestDz->m_lstData.at(i).nAddress);
//             }
//             
// 
//             if ((i + 1) % (pAsdudz->MAX_DATA_PER_ASDUDZ2_RD) == 0)
//             {
//                 //当达到最大数时  发送数据包
//                 pAsdudz->SetItemCount(nSetIndex + 1);
//                 int nResult = Send_I(buf, sizeof(ASDUDZ2));
//                 if (nResult != SEND_OK)
//                 {
//                     return false;
//                 }
//                 //清空数据
//                 memset(pAsdudz->m_data, 0, pAsdudz->MAX_DATA_PER_ASDUDZ2_RD);
//                 nSetIndex = 0;
//             }
// 
//             nSetIndex++;
//         }
// 
//         if (nSetIndex != pAsdudz->MAX_DATA_PER_ASDUDZ2_RD)
//         {
//             pAsdudz->SetItemCount(nSetIndex);
//             
//             int nResult = Send_I(buf, pAsdudz->GetAsduDzLength());
// 
//             if (nResult != SEND_OK)
//             {
//                 return false;
//             }
//         }
//     }


	return true;
}


//死区设置
bool CApduSender::OnSendZoomDataRequest(ZOOM_BASE *pZoomData)
{

    char buf[255] = { 0 };

    //组织
    ASDU112_ZOOM* pAsduZoom = (ASDU112_ZOOM*)(buf + sizeof(APCI));

    pAsduZoom->type = 0x70;
    pAsduZoom->vsq = 0x01;

    pAsduZoom->asduAddr.SetAddr(pZoomData->nBaseAddr);
    //传送原因
    pAsduZoom->cot.SetCot(pZoomData->nReason);
    //设置个数
    pAsduZoom->SetItemCount(pZoomData->nNumber);
    //设置值
    pAsduZoom->m_value = pZoomData->fValue;
    //
    pAsduZoom->uType = pZoomData->qpm;
    //
    pAsduZoom->infoaddr.SetAddr(pZoomData->nAddr);

    int nResult = Send_I(buf, sizeof(ASDU112_ZOOM));
    if (nResult != SEND_OK)
    {
        return false;
    }
    
    return true;
}


bool CApduSender::OnSendRecordCatalog(LB_DATA dLbData)
{
    //清除一个
    m_lstLbData.removeFirst();
    //保存录波数据
    m_lbData = dLbData;
    //召唤目录
    char buf[255];

    //组织ASDUZGML
    ASDUZGML* pAsduzgml = (ASDUZGML*)(buf + sizeof(APCI));

    pAsduzgml->type = F_SC_NA_1;
    pAsduzgml->vsq = 0x01;

    pAsduzgml->cot.SetCot(COT_REQ);

    int nDeviceAddr = 1;
    pAsduzgml->asduAddr.SetAddr(nDeviceAddr);

    pAsduzgml->m_infoaddr.SetAddr(dLbData.m_nCatalogIndex);

    pAsduzgml->m_infoFileIndex.SetAddr(0);
    pAsduzgml->m_reserve3 = 0x0;
    pAsduzgml->m_reserve4 = 0x2;

    int nResult = Send_I(buf, sizeof(ASDUZGML));

    QString strDeviceName = m_pComm104Pln->GetFtpModule()->GetDeviceName();


    if (nResult != SEND_OK)
    {
        m_lbData.m_bValidFlag = false;
        m_pComm104Pln->GetFtpModule()->GetMainModule()->LogString(strDeviceName.toLocal8Bit().data(), tr("Send Request Catalog failed").toLocal8Bit().data(), 1);
        return false;
    }

    m_pComm104Pln->GetFtpModule()->GetMainModule()->LogString(strDeviceName.toLocal8Bit().data(), tr("Send Request Catalog Success").toLocal8Bit().data(), 1);
    return true;
}

//定值区切换
bool CApduSender::OnSendFixAreaCode(ASDU200 *pRequestArea)
{
	char buf[255] = { 0 };

	//组织
	ASDU200* pAsduFix = (ASDU200*)(buf + sizeof(APCI));

	pAsduFix->type = pRequestArea->type;
	pAsduFix->vsq = pRequestArea->vsq;
	pAsduFix->cot.SetCot(pRequestArea->cot.GetCot());
	pAsduFix->asduAddr.SetAddr(pRequestArea->asduAddr.GetAddr());
	pAsduFix->m_infoaddr.SetAddr(pRequestArea->m_infoaddr.GetAddr());
	pAsduFix->m_infAreaIndex.SetAddr(pRequestArea->m_infAreaIndex.GetAddr());


	int nResult = Send_I(buf, sizeof(ASDU200));
	QString strDeviceName = m_pComm104Pln->GetFtpModule()->GetDeviceName();
	if (nResult != SEND_OK)
	{
		m_pComm104Pln->GetFtpModule()->GetMainModule()->LogString(strDeviceName.toLocal8Bit().data(), tr("Send Switch Fix Area Code Failed").toLocal8Bit().data(), 1);

		return false;
	}

	m_pComm104Pln->GetFtpModule()->GetMainModule()->LogString(strDeviceName.toLocal8Bit().data(), tr("Send Switch Fix Area Code Success").toLocal8Bit().data(), 1);

	return true;
}


LB_DATA CApduSender::GetLbData() const
{
    return m_lbData;
}

bool CApduSender::OnSendChooseLbFile()
{

    //召唤目录
    char buf[255];

    //组织ASDUZGML
    ASDUZGML* pAsduzgml = (ASDUZGML*)(buf + sizeof(APCI));

    pAsduzgml->type = F_SC_NA_1;
    pAsduzgml->vsq = 0x01;

    pAsduzgml->cot.SetCot(COT_FILE);

    int nDeviceAddr = 1;
    pAsduzgml->asduAddr.SetAddr(nDeviceAddr);

    pAsduzgml->m_infoaddr.SetAddr(m_lbData.m_nCatalogIndex);

    pAsduzgml->m_infoFileIndex.SetAddr(m_lbData.m_nFilenameIndex);
    
    pAsduzgml->m_reserve3 = 0x0;
    //01表示选择文件
    pAsduzgml->m_reserve4 = 0x1;

    int nResult = Send_I(buf, sizeof(ASDUZGML));

    QString strDeviceName = m_pComm104Pln->GetFtpModule()->GetDeviceName();


    if (nResult != SEND_OK)
    {
        m_lbData.m_bValidFlag = false;
        m_pComm104Pln->GetFtpModule()->GetMainModule()->LogString(strDeviceName.toLocal8Bit().data(), tr("Send Choose File failed").toLocal8Bit().data(), 1);
        return false;
    }

    m_pComm104Pln->GetFtpModule()->GetMainModule()->LogString(strDeviceName.toLocal8Bit().data(), tr("Send Choose File Success").toLocal8Bit().data(), 1);
    return true;
}

bool CApduSender::OnSendRequestFile(int nIndex)
{
    //召唤目录
    char buf[255];

    //组织ASDUZGML
    ASDUZGML* pAsduzgml = (ASDUZGML*)(buf + sizeof(APCI));

    pAsduzgml->type = F_SC_NA_1;
    pAsduzgml->vsq = 0x01;

    pAsduzgml->cot.SetCot(COT_FILE);

    int nDeviceAddr = 1;
    pAsduzgml->asduAddr.SetAddr(nDeviceAddr);

    pAsduzgml->m_infoaddr.SetAddr(m_lbData.m_nCatalogIndex);

    pAsduzgml->m_infoFileIndex.SetAddr(m_lbData.m_nFilenameIndex);

    pAsduzgml->m_reserve3 = 0x0;
    //01表示选择文件
    pAsduzgml->m_reserve4 = nIndex;

    int nResult = Send_I(buf, sizeof(ASDUZGML));

    QString strDeviceName = m_pComm104Pln->GetFtpModule()->GetDeviceName();

    if (nResult != SEND_OK)
    {
        m_lbData.m_bValidFlag = false;
        m_pComm104Pln->GetFtpModule()->GetMainModule()->LogString(strDeviceName.toLocal8Bit().data(), tr("Send Requese File Failed").toLocal8Bit().data(), 1);
        return false;
    }

    m_pComm104Pln->GetFtpModule()->GetMainModule()->LogString(strDeviceName.toLocal8Bit().data(), tr("Send Requese File Success").toLocal8Bit().data(), 1);
    return true;
}

bool CApduSender::OnSendFixSearch()
{
	char buf[255] = { 0 };

	//组织
	ASDU201_C* pAsduFix = (ASDU201_C*)(buf + sizeof(APCI));

	pAsduFix->type = D_FIX_SEARCH;
	pAsduFix->vsq = 0x01;

	pAsduFix->cot.SetCot(COT_ACT);

	pAsduFix->asduAddr.SetAddr(m_pComm104Pln->GetFtpModule()->GetDeviceAddr());

	pAsduFix->m_infoaddr.SetAddr(0);


	int nResult = Send_I(buf, sizeof(ASDU201_C));

	QString strDeviceName = m_pComm104Pln->GetFtpModule()->GetDeviceName();
	if (nResult != SEND_OK)
	{
		m_pComm104Pln->GetFtpModule()->GetMainModule()->LogString(strDeviceName.toLocal8Bit().data(), tr("Send Search Area Code Failed").toLocal8Bit().data(), 1);

		return false;
	}

	m_pComm104Pln->GetFtpModule()->GetMainModule()->LogString(strDeviceName.toLocal8Bit().data(), tr("Send Search Fix Area Code Success").toLocal8Bit().data(), 1);

	return true;

}

void CApduSender::SetLbAllInfo(const QList<LB_DATA> &lstLbData)
{
    m_lstLbData = lstLbData;
}

QList<LB_DATA> CApduSender::GetLbAllData()
{
    return m_lstLbData;
}

//文件服务  目录召唤
bool CApduSender::OnSendGetCatalogRequest(const FILE_CATALOG_REQUEST_1 &lbCatalog)
{
	char buf[255] = { 0 };


	if (!m_strFilePath.isEmpty())
	{
		QString strDeviceName = m_pComm104Pln->GetFtpModule()->GetDeviceName();

		m_pComm104Pln->GetFtpModule()->GetMainModule()->LogString(strDeviceName.toLocal8Bit().data(), tr("other file is loading, please wait!").toLocal8Bit().data(), 1);

		return true;

	}

	//
	m_pComm104Pln->GetRecver()->ClearFileInfos();
	//组织
	FILE_CATALOG_REQUEST_1* pAsduCatalog = (FILE_CATALOG_REQUEST_1*)(buf + sizeof(APCI));

	pAsduCatalog->type = lbCatalog.type;
	pAsduCatalog->vsq = 0x01;

	pAsduCatalog->cot.SetCot(COT_REQ);

	pAsduCatalog->asduAddr.SetAddr(m_pComm104Pln->GetFtpModule()->GetDeviceAddr());

	pAsduCatalog->m_asduObjID.SetAddr(0);
	pAsduCatalog->m_addrType = 2;    //文件传输

	pAsduCatalog->m_nOperatorType = lbCatalog.m_nOperatorType;
	//目录id
	pAsduCatalog->m_nCatalogID = lbCatalog.m_nCatalogID;
	//目录长度
	pAsduCatalog->m_uCatalogLength = lbCatalog.m_uCatalogLength;
	// 目录名称
	strcpy(pAsduCatalog->m_cCatalogName, lbCatalog.m_cCatalogName);

	//信息体2
	FILE_CATALOG_REQUEST_2 *pCatalogFirst = (FILE_CATALOG_REQUEST_2 *)(buf + sizeof(APCI) + pAsduCatalog->GetAsduLength());

	pCatalogFirst->m_nCallFlag = pAsduCatalog->m_nCallFlag;
	pCatalogFirst->m_dateStart = pCatalogFirst->m_dateStart;
	pCatalogFirst->m_dataEnd = pCatalogFirst->m_dataEnd;

	//
	m_pComm104Pln->GetRecver()->SetFileInfo(lbCatalog.m_fileInfo);

	int nResult = Send_I(buf, pAsduCatalog->GetAsduLength()+sizeof(FILE_CATALOG_REQUEST_2));

	QString strDeviceName = m_pComm104Pln->GetFtpModule()->GetDeviceName();
	if (nResult != SEND_OK)
	{
		m_pComm104Pln->GetFtpModule()->GetMainModule()->LogString(strDeviceName.toLocal8Bit().data(), tr("Send Catalog Request Failed").toLocal8Bit().data(), 1);

		return false;
	}

	m_pComm104Pln->GetFtpModule()->GetMainModule()->LogString(strDeviceName.toLocal8Bit().data(), tr("Send Search Catalog Request  Success").toLocal8Bit().data(), 1);

	return true;

}

//读文件激活
bool CApduSender::OnSendReadFileAction(const FILE_ATTR_INFO& fileAttr)
{
	//目录文件不做处理
	if (fileAttr.m_cFileAttr == 1)
	{
		return true;
	}

	//m_pComm104Pln->GetRecver()->InsertToFileLists(fileAttr);

	char buf[255] = { 0 };

	//读文件激活
	FILE_BASE* pAsduCatalog = (FILE_BASE*)(buf + sizeof(APCI));


	pAsduCatalog->type = fileAttr.type;
	pAsduCatalog->asduAddr = fileAttr.asduAddr;
	pAsduCatalog->vsq = 0x01;

	//file head
	pAsduCatalog->m_asduObjID.SetAddr(0);
	pAsduCatalog->m_addrType = 2;         //文件传输
	//TODO

	pAsduCatalog->cot.SetCot(COT_ACT);

	pAsduCatalog->asduAddr.SetAddr(m_pComm104Pln->GetFtpModule()->GetDeviceAddr());


	//信息体
	char *pReadActionData = buf + sizeof(APCI) + sizeof(FILE_BASE);

	pReadActionData[0] = 3;          //读文件激活
	
	pReadActionData[1] = fileAttr.m_cFileNameLength;    //文件长度
	
	//文件名称
	strncpy(pReadActionData + 2, fileAttr.m_strFileName.toLocal8Bit().data(), fileAttr.m_cFileNameLength);


	int nResult = Send_I(buf, sizeof(FILE_BASE)+2+ fileAttr.m_cFileNameLength);

	QString strDeviceName = m_pComm104Pln->GetFtpModule()->GetDeviceName();
	if (nResult != SEND_OK)
	{
		m_pComm104Pln->GetFtpModule()->GetMainModule()->LogString(strDeviceName.toLocal8Bit().data(), tr("Send Read Action Request Failed").toLocal8Bit().data(), 1);

		return false;
	}

	m_pComm104Pln->GetFtpModule()->GetMainModule()->LogString(strDeviceName.toLocal8Bit().data(), tr("Send Read Action Request  Success").toLocal8Bit().data(), 1);

	return true;

}

//写文件服务  激活
bool CApduSender::OnSendWriteFileAction(const FILE_ATTR_INFO& fileAttr)
{
/*
	if (!m_btWriteData.isEmpty())
	{
		QString strDeviceName = m_pComm104Pln->GetFtpModule()->GetDeviceName();

		m_pComm104Pln->GetFtpModule()->GetMainModule()->LogString(strDeviceName.toLocal8Bit().data(), tr("Other file is writing,please operator later").toLocal8Bit().data(), 1);

		return true;
	}
	*/
	m_btWriteData = fileAttr.m_btArrayData;

	char buf[255] = { 0 };

	//读文件激活
	FILE_BASE* pFileAction = (FILE_BASE*)(buf + sizeof(APCI));

	//file head
	pFileAction->m_asduObjID.SetAddr(0);
	pFileAction->m_addrType = 2;         //文件传输

	pFileAction->type = fileAttr.type;    //文件传输code
	pFileAction->vsq = 0x01;

	pFileAction->cot.SetCot(COT_ACT);

	pFileAction->asduAddr.SetAddr(m_pComm104Pln->GetFtpModule()->GetDeviceAddr());


	//信息体
	char *pReadActionData = buf + sizeof(APCI) + sizeof(FILE_BASE);

	pReadActionData[0] = 7;
	pReadActionData[1] = fileAttr.m_cFileNameLength;

	strncpy(pReadActionData + 2, fileAttr.m_strFileName.toLocal8Bit().data(), fileAttr.m_cFileNameLength);

	//文件id  TODO
	INFOADDR4 *infoFileID = (INFOADDR4*)(pReadActionData+2+fileAttr.m_cFileNameLength);
	 
	//文件大小
	INFOADDR4 *infoFileSize = (INFOADDR4*)(pReadActionData + 2 + fileAttr.m_cFileNameLength+sizeof(INFOADDR4));

	int nResult = Send_I(buf, sizeof(FILE_BASE) + 2 + fileAttr.m_cFileNameLength + sizeof(INFOADDR4)*2);

	QString strDeviceName = m_pComm104Pln->GetFtpModule()->GetDeviceName();
	if (nResult != SEND_OK)
	{
		m_pComm104Pln->GetFtpModule()->GetMainModule()->LogString(strDeviceName.toLocal8Bit().data(), tr("Send Write Action Request Failed").toLocal8Bit().data(), 1);

		return false;
	}

	m_pComm104Pln->GetFtpModule()->GetMainModule()->LogString(strDeviceName.toLocal8Bit().data(), tr("Send Write Action Request  Success").toLocal8Bit().data(), 1);

	return true;

}

//写文件数据
bool CApduSender::OnSendWriteFileData()
{
	//分包发送文件
	if (m_btWriteData.isEmpty())
	{
		return true;
	}

	int nMaxDataSize = MAX_ASDU_SIZE - sizeof(FILE_BASE) - 11;

	int nBagCount = m_btWriteData.size() % nMaxDataSize == 0 ? m_btWriteData.size() / nMaxDataSize : m_btWriteData.size() / nMaxDataSize + 1;

	QString strDeviceName = m_pComm104Pln->GetFtpModule()->GetDeviceName();


	for (int i = 0; i < nBagCount; i++)
	{
		char buf[255] = { 0 };

		//读文件激活
		FILE_BASE* pFileAction = (FILE_BASE*)(buf + sizeof(APCI));

		//file head
		pFileAction->m_asduObjID.SetAddr(0);
		pFileAction->m_addrType = 2;         //文件传输

		pFileAction->type = D_FILE_TRANSPORT;    //文件传输code
		pFileAction->vsq = 0x01;

		pFileAction->cot.SetCot(COT_REQ);

		pFileAction->asduAddr.SetAddr(m_pComm104Pln->GetFtpModule()->GetDeviceAddr());


		//信息体
		char *pReadActionData = buf + sizeof(APCI) + sizeof(FILE_BASE);

		pReadActionData[0] = 9;     //写文件数据

		//文件id	
		INFOADDR4 *infoFileID = (INFOADDR4*)(pReadActionData + 1);
		infoFileID->SetAddr(0);
		//数据段号
		INFOADDR4 *infoNodeID = (INFOADDR4*)(pReadActionData + 1 + sizeof(INFOADDR4));
		infoNodeID->SetAddr(i);

		//后续标识
		int nResult = 0;
		int nCurrentLength = 0;

		if (i == nBagCount-1)
		{
			//最后一个包
			pReadActionData[1 + sizeof(INFOADDR4)*2] = 0;
			//
			strcpy(pReadActionData + 2 + sizeof(INFOADDR4) * 2, m_btWriteData.mid(i*nMaxDataSize));

			//pReadActionData[2 + sizeof(INFOADDR4) * 2 + nMaxDataSize] = checkAllData(m_btWriteData.mid(i*nMaxDataSize, nMaxDataSize).data(), nMaxDataSize);

			nCurrentLength = m_btWriteData.size() - nMaxDataSize*i;

			//和校验
			pReadActionData[2 + sizeof(INFOADDR4) * 2 + nMaxDataSize] = checkAllData(m_btWriteData.mid(i*nMaxDataSize).data(), nCurrentLength);


		}
		else
		{
			//中间包
			pReadActionData[1 + sizeof(INFOADDR4)*2] = 1;

			strcpy(pReadActionData +2 + sizeof(INFOADDR4) * 2, m_btWriteData.mid(i*nMaxDataSize,nMaxDataSize));

			//和校验
			pReadActionData[2 + sizeof(INFOADDR4) * 2 + nMaxDataSize] = checkAllData(m_btWriteData.mid(i*nMaxDataSize, nMaxDataSize).data(),nMaxDataSize);
			nCurrentLength = nMaxDataSize;

		}

		nResult = Send_I(buf, sizeof(FILE_BASE) + 2 + sizeof(INFOADDR4) * 2 + nCurrentLength + 1);

		if (nResult != SEND_OK)
		{
			m_pComm104Pln->GetFtpModule()->GetMainModule()->LogString(strDeviceName.toLocal8Bit().data(), tr("Send Write Action Request Failed").toLocal8Bit().data(), 1);

			//return false;
		}

	}

	m_pComm104Pln->GetFtpModule()->GetMainModule()->LogString(strDeviceName.toLocal8Bit().data(), tr("Send Write Action Request  Success").toLocal8Bit().data(), 1);


	return true;
}

unsigned char CApduSender::checkAllData(const char *data, int length)
{
	unsigned char lValue = 0;
	for (int i = 0; i < length; i++)
	{
		lValue += data[i];
	}
	return lValue;
}

void CApduSender::OnSendFileConformNode()
{
    //召唤目录
    char buf[255];

    //组织ASDUZGML
    ASDUZGML* pAsduzgml = (ASDUZGML*)(buf + sizeof(APCI));

    pAsduzgml->type = F_AF_NA_1;
    pAsduzgml->vsq = 0x01;

    pAsduzgml->cot.SetCot(COT_FILE);

    int nDeviceAddr = 1;
    pAsduzgml->asduAddr.SetAddr(nDeviceAddr);

    pAsduzgml->m_infoaddr.SetAddr(m_lbData.m_nCatalogIndex);

    pAsduzgml->m_infoFileIndex.SetAddr(m_lbData.m_nFilenameIndex);

    pAsduzgml->m_reserve3 = 0x0;
    //01表示选择文件
    pAsduzgml->m_reserve4 = 1;

    int nResult = Send_I(buf, sizeof(ASDUZGML));

    QString strDeviceName = m_pComm104Pln->GetFtpModule()->GetDeviceName();

    if (nResult != SEND_OK)
    {
        m_lbData.m_bValidFlag = false;
        m_pComm104Pln->GetFtpModule()->GetMainModule()->LogString(strDeviceName.toLocal8Bit().data(), tr("Send Last Conform File Failed").toLocal8Bit().data(), 1);
    }

    m_pComm104Pln->GetFtpModule()->GetMainModule()->LogString(strDeviceName.toLocal8Bit().data(), tr("Send Last Conform File Success").toLocal8Bit().data(), 1);
}

bool CApduSender::OnSendSycsOwnTime(int nDeviceId, QDateTime &tSycsTime)
{
    char buf[255];

    //组织ASDU103
    ASDU103* pAsdu103 = (ASDU103*)(buf + sizeof(APCI));

    pAsdu103->type = C_CS_NA_1;
    pAsdu103->vsq = 0x01;

    pAsdu103->cot.SetCot(COT_ACT);

    int nDeviceAddr = nDeviceId;
    pAsdu103->asduAddr.SetAddr(nDeviceAddr);

    pAsdu103->m_infoaddr.SetAddr(0x00);

    struct CP56Time2a* cp56time2a = (struct CP56Time2a*)(&(pAsdu103->m_time));

    //QDateTime tvTime = QDateTime::currentDateTime();
    cp56time2a->Initialize(tSycsTime.date().year(), tSycsTime.date().month(), tSycsTime.date().day(), tSycsTime.time().hour(), tSycsTime.time().minute(), tSycsTime.time().second(), tSycsTime.time().msec());

    int nResult = Send_I(buf, sizeof(ASDU103));

    if (nResult != SEND_OK)
    {
        return false;
    }

    return true;
}


