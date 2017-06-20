#include "apdurecver.h"
#include <QTimer>
#include <QDateTime>
#include <QDebug>
#include "socketthread.h"
#include "asdu_info.h"
#include "define_104.h"
#include "apdusender.h"
#include "commplugin.h"
#include "devicestudio/main_module.h"
#include "loadfilethread.h"


/*********************************************************************************************************
** \brief CApduRecver(QObject *parent)
** \details 构造函数
** \param argl   父类指针
** \param arg2
** \return void
** \author ww
** \date 2016年5月27日
** \note
********************************************************************************************************/
CApduRecver::CApduRecver(QObject *parent):
    QObject(parent)
{
	//104分析类结构
//     m_nNodeLength = 0;
    m_pComm104Pln = qobject_cast<CCommPlugin*>(parent);
    Q_ASSERT(m_pComm104Pln);
    m_pFileLoad = new CLoadFileThread;
    m_pFileLoad->moveToThread(&m_LoadThread);

	m_LoadThread.start();

    //connect(&m_LoadThread, &QThread::finished, m_pFileLoad, &QObject::deleteLater);
	//
	connect(this,SIGNAL(Signal_RecvFileData(QString , QByteArray )), m_pFileLoad,SLOT(Slot_CreateNewFile(QString , QByteArray )));
}

/*********************************************************************************************************
** \brief OnReceive(char *pBuff, int nLen)
** \details 接收网络数据 进行解析
** \param argl   父类指针
** \param arg2
** \return bool   处理成功失败标识
** \author ww
** \date 2016年5月27日
** \note
********************************************************************************************************/
bool CApduRecver::OnReceive(char *pBuff, int nLen)
{
    if(nLen <=0)
    {
        return false;
    }

    if(!m_104Cache.IsEmpty())
    {
        if(m_104Cache.GetCurByteLength() == 1)//只有一个启动字符
        {
            m_104Cache.CopyData(pBuff,1);//复制长度域
            pBuff++;
            nLen--;
        }

        int nNeedBytes = m_104Cache.GetNeedByteLength();
        if(nNeedBytes <= 0)
        {
            return false;
        }

        if(nNeedBytes > nLen)//需要的长度大于接收到的长度
        {
            m_104Cache.CopyData(pBuff,nLen);
            return true;
        }
        else//接收到的长度大于需要的长度
        {
            //复制需要的部分
            m_104Cache.CopyData(pBuff,nNeedBytes);
            pBuff += nNeedBytes;
            nLen -= nNeedBytes;

            bool ISOK = AnalyseApdu(m_104Cache.GetDataBuff(),m_104Cache.GetDataLen());
            m_104Cache.Clear();
            if(!ISOK)
            {
				return false;
            }

            //解析剩余的部分
            return OnReceive(pBuff,nLen);
        }
    }
    else//缓冲区为空
    {
        //在接收区寻找启动字符
        char cStartChar = STARTCHAR;
        char* pNewBuff = std::find(pBuff,pBuff+nLen,cStartChar);

        if(pNewBuff == pBuff+nLen)
            return false;//没有找到启动字符

        nLen = nLen - (pNewBuff - pBuff);
        pBuff = pNewBuff;

        //复制启动字符至缓冲区
        m_104Cache.CopyData(pBuff,1);
        pBuff += 1;
        nLen  -= 1;

        return OnReceive(pBuff,nLen);
    }

    return true;
}

/*********************************************************************************************************
** \brief AnalyseApdu(char *pBuff, int nLength)
** \details 解析应用协议数据单位数据
** \param argl   一单元十六进制数据包
** \param arg2   数据包的长度
** \return bool   处理成功失败标识
** \author ww
** \date 2016年5月27日
** \note
********************************************************************************************************/
bool CApduRecver::AnalyseApdu(char *pBuff, int nLength)
{

    if((pBuff==NULL) || (nLength<sizeof(APCI)) || (pBuff[0] != STARTCHAR))
    {
        return false;
    }
	//用于做统计
    QByteArray btData(pBuff, nLength);
    emit Signal_recv16Data(btData, nLength);

    APCI* pApci = (APCI*)(pBuff);
    int nType = pApci->GetType();

    if( (nType != TYPE_I) && (nType != TYPE_U) &&  (nType != TYPE_S) )
    {
        return false;
    }

    switch(nType)
    {
    case TYPE_I:
    {
        if(!AnalyseI(pBuff,nLength))
        {
            return false;
        }
    }
        break;
    case TYPE_U:
    {
        if(!AnalyseU(pBuff,nLength))
        {
            return false;
        }
    }
     break;
    case TYPE_S:
    {
        //TODO
    }
        break;
    default:
        break;
    }

	//对序号  kw值  超时检查 
    if(!m_pComm104Pln->OnRecvFrame(nType,pApci->GetSendNum(),pApci->GetRecvNum()))
    {
        return false;
    }

    return true;
}

bool CApduRecver::AnalyseI(char *pBuff, int nLength)
{
    //解析数据
    AnalyseAsdu(pBuff,nLength);
    return true;
}

/*********************************************************************************************************
** \brief AnalyseAsdu(char *pBuff, int nLength)
** \details 解析应用服务数据单元数据
** \param argl   一单元十六进制数据包
** \param arg2   数据包的长度
** \return bool   处理成功失败标识
** \author ww
** \date 2016年5月27日
** \note
********************************************************************************************************/
void CApduRecver::AnalyseAsdu(char *pBuff, int nLength)
{
    ASDU_BASE* pAsduBase = (ASDU_BASE*)(pBuff+sizeof(APCI));

    pBuff =  pBuff + sizeof(APCI);
    nLength = nLength - sizeof(APCI);
	switch (pAsduBase->type)
	{
		//遥信
	case M_SP_NA_1: // 单点信息  ASDU1
	{
		OnRecvBinarySinglePoint(pBuff, nLength);
		break;
	}
	case M_DP_NA_1://双点信息 ASDU3
	{
		OnRecvBinaryDoublePoint(pBuff, nLength);
		break;
	}
	case M_PS_NA_1: // 成组单点信息   ASDU20
	{
		OnRecvBinaryGroupPoint(pBuff, nLength);
		break;
	}
	case M_SP_TA_1://带短时标的单点信息 ASDU2   SOE    M_SP_NA_1
	{
		OnRecvBinarySinglePointShortTime(pBuff, nLength);
		break;
	}
	case M_SP_TB_1://带长时标的单点信息 ASDU30   SOE
	{
		OnRecvBinarySinglePointTime(pBuff, nLength);
		break;
	}
	case M_DP_TA_1://带短时标的双点信息 ASDU4    SOE
	{
		OnRecvBinaryDoublePointShortTime(pBuff, nLength);
		break;
	}
	case M_DP_TB_1://带长时标的双点信息 ASDU31   SOE
	{
		OnRecvBinaryDoublePointTime(pBuff, nLength);
		break;
	}
	//遥测
	case M_ME_NA_1://归一化测量值 ASDU9 M_ME_NC_1
	{
		OnRecvAnalogNormalized(pBuff, nLength);
		break;
	}
	case M_ME_TA_1://带短时标的归一化测量值 ASDU10
	{
	    OnRecvAnalogNormalizedShortTime(pBuff, nLength);
	    break;
	}
	case M_ME_NB_1://标度化测量值 ASDU11   TODO  暂时改为定值
	{
		OnRecvAnalogScaled(pBuff, nLength);

		break;
	}
	case M_ME_TB_1://带短时标的标度化测量值 ASDU12
	{
		OnRecvAnalogScaledShortTime(pBuff, nLength);
		break;
	}
	case M_ME_NC_1://短浮点数测量值 ASDU13
	{
		OnRecvAnalogShortFloat(pBuff, nLength);
		break;
	}
	case M_ME_TC_1://带短时标的短浮点数测量值 ASDU14
	{
		OnRecvAnalogShortFloatShortTime(pBuff, nLength);
		break;
	}
    case M_ME_ND_1://不带品质描述词的归一化测量值 ASDU21
    {
        OnRecvAnalogNormalizedNoQuality(pBuff, nLength);
        break;
    }
	//累计量   遥脉
	case M_IT_NA_1: //累计量 ASDU15
	{
		OnRecvKwh(pBuff, nLength);
		break;
	}
	case M_IT_TA_1: //带短时标的累计量 ASDU16
	{
		OnRecvKwhShortTime(pBuff, nLength);
		break;
	}
	case M_IT_TB_1: //带长时标的累计量 ASDU37
	{
		OnRecvKwhLongTime(pBuff, nLength);
		break;
	}
    case C_IC_NA_1:// 总召唤命令 ASDU100
    {
        //qDebug()<<"总召唤成功w";
        emit Signal_AllCallRespond();
        break;
    }
    case C_RP_NA_1:  //复位进程
    {
        OnRecvResetProcess(pBuff, nLength);
        break;
    }
	case C_SC_NA_1://遥控(单点命令) ASDU45
	{
		OnRecvSetBinarySPAck(pBuff, nLength);
		break;
	}
	case C_DC_NA_1://遥控(双点命令) ASDU46
	{
		OnRecvSetBinaryDPAck(pBuff, nLength);
		break;
	}
	case C_RD_NA_1:// 定值获取 单参
	case D_DV_RD_1://多参
	{
		//OnRecvDevReadRequestAck(pBuff, nLength);
		break;
	}
	case  C_SE_NA_1:
	case D_DV_WR_1:
	{
		OnRecvDevWriteRequestAck(pBuff, nLength);
		break;
	}
    case F_DR_TA_1:
    {
        //召唤目录响应
        //OnRecvResponseCatalog(pBuff, nLength);
        break;
    }
    case F_FR_NA_1:
    {
        //选择文件
        //OnRecvChooseFileResponse(pBuff, nLength);
        break;
    }
    case F_SR_NA_1:
    {
        //请求文件准备就绪
       // OnRectRequestFileReady(pBuff, nLength);
        break;
    }
    case F_SG_NA_1:
    {
        //节传输
       // OnRecvNodeData(pBuff, nLength);
        break;
    }
    case F_LS_NA_1:
    {
        //节最后的段
       // OnRecvLastNodePart(pBuff, nLength);
        break;
    }
    case F_AF_NA_1:
    {
        //节传输确认
        //空
        break;
    }
    case C_GZ_SJ_1:
    {
        //故障事件
        OnRecvDefalutMsg(pBuff, nLength);
        break;
    }
    case P_ME_NC_1:
    {
        //死区响应
        OnRecvZoneMsg(pBuff,nLength);
        break;
    }
	case D_FIX_SWITCH:
	{
		//切换定值区号
		OnRecvSwitchFixArea(pBuff, nLength);
		break;
	}
	case D_FIX_SEARCH:
	{
		//获取当前定值区号
		OnrecvReadCurrentFixArea(pBuff, nLength);
		break;
	}
	case D_FILE_TRANSPORT:
	{
		//文件服务
		OnRecvFileAnalyseInfo(pBuff, nLength);
		break;
	}
	case D_FIX_READ:
	{
		//定值读取
		OnRecvDevReadRequestAck(pBuff, nLength);
		break;
	}
	case D_FIX_WRITE:
	{
		OnRecvDevWriteRequestAck(pBuff, nLength);
		break;
	}

    default:
        break;
    }
}

void CApduRecver::OnRecvZoneMsg(char *pBuff, int nLength)
{
    if (nLength == 0)
    {
        return;
    }
    //qDebug() << "Recv2:" << QByteArray(pBuff, nLength).toHex();

    ASDU112_ZOOM* pAsduZone = (ASDU112_ZOOM*)pBuff;

    emit Signal_ZoneFeedBack(pAsduZone->infoaddr.GetAddr(),pAsduZone->m_value,pAsduZone->uType,pAsduZone->cot.GetCot());

}


void CApduRecver::OnRecvCatalogRepond(char *pBuff, int nLength)
{
	if (nLength == 0)
	{
		return;
	}

	FILE_CATALOG_RESPOND* pAsduCatlog = (FILE_CATALOG_RESPOND*)pBuff;


// 	m_pCatalogRespond->asduAddr = pAsduCatlog->asduAddr;
// 	m_pCatalogRespond->cot = pAsduCatlog->cot;
// 	m_pCatalogRespond->m_nCatalogID = pAsduCatlog->m_nCatalogID;
// 	m_pCatalogRespond->m_nOperatorType = pAsduCatlog->m_nOperatorType;
// 
// 	m_pCatalogRespond->m_nResultFlag = pAsduCatlog->m_nResultFlag;
// 	m_pCatalogRespond->m_uFileCount = pAsduCatlog->m_uFileCount;
// 
// 	m_pCatalogRespond->m_uNextFlag = pAsduCatlog->m_uNextFlag;
// 	m_pCatalogRespond->type = pAsduCatlog->type;
// 	m_pCatalogRespond->vsq = pAsduCatlog->vsq;

	char *pStartFileAttr = pBuff + sizeof(FILE_CATALOG_RESPOND);

	for (int i=0; i<pAsduCatlog->m_uFileCount; i++)
	{

		FILE_ATTR_INFO tArrInfo;
		tArrInfo.m_cFileNameLength = pStartFileAttr[0];
		//文件名称
		tArrInfo.m_strFileName = QString::fromLocal8Bit(pStartFileAttr + 1, tArrInfo.m_cFileNameLength);
	   //文件属性
		tArrInfo.m_cFileAttr = pStartFileAttr[1 + tArrInfo.m_cFileNameLength];
	   //文件大小
	   INFOADDR4* tSizeInfo = (INFOADDR4*)(pStartFileAttr + 1 + tArrInfo.m_cFileNameLength +1);

	   tArrInfo.m_FileSize = tSizeInfo->GetAddr();

	   //时间不用

	   pStartFileAttr = pStartFileAttr + 1 + tArrInfo.m_cFileNameLength + 1 + 4 + 7;

	   //m_arrCatalogRespond.append(tArrInfo);
	}

}

//读文件激活确认
void CApduRecver::OnRecvReadActionResond(char *pBuff, int nLenght)
{
	if (nLenght <= 0)
	{
		return;
	}

	ASDU_BASE *pbase = (ASDU_BASE*)pBuff;
	//基础信息

	//激活确认信息
	//操作标识
	char *pActionRespondData = pBuff + sizeof(ASDU_BASE);

	unsigned char bOperatorFlag = pActionRespondData[0];

	unsigned char cResultFlag = pActionRespondData[1];

	unsigned char cFileNameLegth = pActionRespondData[2];

	QString strFileName = QByteArray(pActionRespondData[3], cFileNameLegth);
    //文件标识
	INFOADDR4 *tFileFlag = (INFOADDR4*)(pActionRespondData+3+cFileNameLegth);
	//文件大小
	INFOADDR4 *TFileSize = (INFOADDR4*)(pActionRespondData+3+cFileNameLegth+sizeof(INFOADDR4));

	//

}

//读文件传输数据
void CApduRecver::OnRecvReadFileRespond(char *pBuff, int nLength)
{
	if (nLength <= 0)
	{
		return;
	}

	FILE_BASE *pbase = (FILE_BASE*)pBuff;
	//基础信息

	//激活确认信息
	//操作标识
	char *pFileInfoBase = pBuff + sizeof(FILE_BASE);
	
	//操作标识
	unsigned char uOperatorFlag = pFileInfoBase[0];
	//文件id
	INFOADDR4 *tFileFlag = (INFOADDR4*)(pFileInfoBase+1);
	//数据段号
	INFOADDR4 *tNodeID = (INFOADDR4*)(pFileInfoBase+1+sizeof(INFOADDR4));
	//后续标志
	unsigned char uNextFlag = pFileInfoBase[1+sizeof(INFOADDR4)*2];
	//文件数据
	QByteArray tBtData = QByteArray(2+sizeof(INFOADDR4)*2 + pFileInfoBase, nLength - 3 - sizeof(INFOADDR4) * 2 - sizeof(FILE_BASE));
	//校验码 
	unsigned char pCheckCode = pBuff[nLength-1];

	m_AbyFileData.append(tBtData);

	if (uNextFlag == 0)
	{
		if (m_arrCatalogRespond.isEmpty())
		{
			m_pComm104Pln->getSender()->ClearFilepath();
		}
		//没有后续文件，开始组装数据
		QString strAbsFilename = m_pComm104Pln->getSender()->GetFilePath() + "/" + m_arrCatalogRespond.first().m_strFileName;


		emit Signal_RecvFileData(strAbsFilename, m_AbyFileData);
		
		

		m_AbyFileData.clear();

		m_arrCatalogRespond.removeFirst();

		if (m_arrCatalogRespond.isEmpty())
		{
			m_pComm104Pln->getSender()->ClearFilepath();
		}
		else
		{
			m_pComm104Pln->getSender()->OnSendReadFileAction(m_arrCatalogRespond.first());
		}

		QString strlog = tr("File Download Success!");

		QString strDeviceName = m_pComm104Pln->GetFtpModule()->GetDeviceName();

		m_pComm104Pln->GetFtpModule()->GetMainModule()->LogString(strDeviceName.toLocal8Bit().data(), strlog.toLocal8Bit().data(), 1);


	}

}

void CApduRecver::OnRecvReadConform(char *pBuff, int nLength)
{
	if (nLength <= 0)
	{
		return;
	}
	READ_NODE_FILE_CONFORM *pbase = (READ_NODE_FILE_CONFORM*)pBuff;
	//基础信息

}

//文件 写激活确认
void CApduRecver::OnRecvWriteAction(char *pBuff, int nLength)
{
	if (nLength <= 0)
	{
		return;
	}

	FILE_BASE *pbase = (FILE_BASE*)pBuff;
	//基础信息

	//激活确认信息
	//操作标识
	char *pActionRespondData = pBuff + sizeof(FILE_BASE);
	//操作标识
// 	unsigned char bOperatorFlag = pActionRespondData[0];
	//结果描述子
	unsigned char cResultFlag = pActionRespondData[1];
	//文件名长度
// 	unsigned char cFileNameLegth = pActionRespondData[2];
	//文件名称
// 	QString strFileName = QByteArray(pActionRespondData[3], cFileNameLegth);
// 	//文件标识
// 	INFOADDR4 *tFileFlag = (INFOADDR4*)(pActionRespondData + 3 + cFileNameLegth);
// 	//文件大小
// 	INFOADDR4 *TFileSize = (INFOADDR4*)(pActionRespondData + 3 + cFileNameLegth + sizeof(INFOADDR4));

	QString strDeviceName = m_pComm104Pln->GetFtpModule()->GetDeviceName();

	QString strlog;
	if (cResultFlag == 0)
	{
		strlog = tr("Recv Write Action Success");
		//开始写文件
		m_pComm104Pln->getSender()->OnSendWriteFileData();
	}
	else if (cResultFlag == 1)
	{
		strlog = tr("Unknown Error");
	}
	else if (cResultFlag == 2)
	{
		strlog = tr("FileName Error");
	}
	else if (cResultFlag == 3)
	{
		strlog = tr("Size overflow");
	}

	m_pComm104Pln->GetFtpModule()->GetMainModule()->LogString(strDeviceName.toLocal8Bit().data(), strlog.toLocal8Bit().data(), 1);


}

void CApduRecver::OnRecvWriteConform(char *pBuff, int nLength)
{
	if (nLength <= 0)
	{
		return;
	}

	READ_NODE_FILE_CONFORM *pbase = (READ_NODE_FILE_CONFORM*)pBuff;

	QString strlog;
	QString strDeviceName = m_pComm104Pln->GetFtpModule()->GetDeviceName();

	if (pbase->m_cEndFlag == 0)
	{
		strlog = tr("File Send Success!");
		m_pComm104Pln->GetFtpModule()->GetMainModule()->LogString(strDeviceName.toLocal8Bit().data(), strlog.toLocal8Bit().data(), 1);
	}
	else
	{
		strlog = tr("File Send Error!");
		m_pComm104Pln->GetFtpModule()->GetMainModule()->LogString(strDeviceName.toLocal8Bit().data(), strlog.toLocal8Bit().data(), 1);

	}

}

//收到单点命令确认
void CApduRecver::OnRecvSetBinarySPAck(char* pBuff, int nLength)
{
	if (nLength <= 0)
	{
		return;
	}
	ASDU45* pAsdu45 = (ASDU45*)(pBuff);
	int nDataID = pAsdu45->m_infoaddr.GetAddr();
	int nRecvAckCot = pAsdu45->cot.GetCot();
	int nRecvAckType = pAsdu45->m_sco.SelectOrExcute;

	NBM_TELECTRL telectrl;
	telectrl.m_nStationID = 0;
	telectrl.m_nDeviceID = 0;
	telectrl.m_nDataID = nDataID;
	telectrl.m_nDataType = DATATYPE_BINARY;
	telectrl.m_fValue = pAsdu45->m_sco.SCS;


		if (nRecvAckCot == COT_DEACTCON)
		{
			telectrl.m_nCtrlType = TELECTRL_ACK_UNSELECT;
		}
        else if (nRecvAckType == 0 && nRecvAckCot == COT_ACTCON) //执行
		{
			telectrl.m_nCtrlType = TELECTRL_ACK_EXECUTE;
			int ionFloag = 0;

			if (telectrl.m_fValue>0.99999 && telectrl.m_fValue<1.00001)
			{
				//on
				ionFloag = 0;
			}
			else if (telectrl.m_fValue>-0.000001 && telectrl.m_fValue<0.00001)
			{
				//on
				ionFloag = 1;
			}
			else
			{
				//error
				ionFloag = -1;
			}
			emit Signal_ControlFeedBack(0, telectrl.m_nDataID, ionFloag, "Exec Response");
		}
		else if (nRecvAckType == 1 && nRecvAckCot == COT_ACTCON)//选择
		{
			telectrl.m_nCtrlType = TELECTRL_ACK_SELECT;
			int ionFloag = 0;

			if (telectrl.m_fValue > 0.99999 && telectrl.m_fValue < 1.00001)
			{
				//on
				ionFloag = 0;
			}
			else if (telectrl.m_fValue > -0.000001 && telectrl.m_fValue < 0.00001)
			{
				//off
				ionFloag = 1;
			}
			else
			{
				//error
				ionFloag = -1;
			}
			emit Signal_ControlFeedBack(0, telectrl.m_nDataID, ionFloag, "Preset Response");
			//发出执行请求
			//m_pComm104Pln->SetControlBckCommand(0, telectrl.m_nDataID, ionFloag);
			//更新表格
			//emit Signal_ControlFeedBack(0, telectrl.m_nDataID, ionFloag, "Exec Request");

		}
		else
		{
            emit Signal_ControlFeedBack(0, telectrl.m_nDataID, 0, QString::number(nRecvAckCot));
			telectrl.m_nCtrlType = -1;
		}
	
}

//收到双点命令确认
void CApduRecver::OnRecvSetBinaryDPAck(char* pBuff, int nLength)
{
	if (nLength <= 0)
	{
		return;
	}

	ASDU46* pAsdu46 = (ASDU46*)(pBuff);
	int nDataID = pAsdu46->m_infoaddr.GetAddr();
	int nRecvAckCot = pAsdu46->cot.GetCot();
	int nRecvAckType = pAsdu46->m_dco.SelectOrExcute;

	NBM_TELECTRL telectrl;
	telectrl.m_nStationID = 0;
	telectrl.m_nDeviceID = 0;
	telectrl.m_nDataID = nDataID;
	telectrl.m_nDataType = DATATYPE_BINARY;
	telectrl.m_fValue = pAsdu46->m_dco.DCS - 1;



		if (nRecvAckCot == COT_DEACTCON)
		{
			telectrl.m_nCtrlType = TELECTRL_ACK_UNSELECT;
		}
        else if (nRecvAckType == 0 && nRecvAckCot == COT_ACTCON) //执行
		{
			telectrl.m_nCtrlType = TELECTRL_ACK_EXECUTE;
			int ionFloag = 0;

			if (telectrl.m_fValue > 0.99999 && telectrl.m_fValue < 1.00001)
			{
				//on
				ionFloag = 0;
			}
			else if (telectrl.m_fValue > -0.000001 && telectrl.m_fValue < 0.00001)
			{
				//off
				ionFloag = 1;
			}
			else
			{
				//error
				ionFloag = -1;
			}
			emit Signal_ControlFeedBack(1, telectrl.m_nDataID, ionFloag, "Exec Response");

		}
		else if (nRecvAckType == 1 && nRecvAckCot == COT_ACTCON)//选择
		{
			telectrl.m_nCtrlType = TELECTRL_ACK_SELECT;
			int ionFloag = 0;

			if (telectrl.m_fValue > 0.99999 && telectrl.m_fValue < 1.00001)
			{
				//on
				ionFloag = 0;
			}
			else if (telectrl.m_fValue > -0.000001 && telectrl.m_fValue < 0.00001)
			{
				//off
				ionFloag = 1;
			}
			else
			{
				//error
				ionFloag = -1;
			}
			emit Signal_ControlFeedBack(1, telectrl.m_nDataID, ionFloag, "Preset Response");
			//发出执行请求
			//m_pComm104Pln->SetControlBckCommand(1, telectrl.m_nDataID, ionFloag);
			//更新表格
			//emit Signal_ControlFeedBack(1, telectrl.m_nDataID, ionFloag, "Exec Request");

		}
		else
		{
            emit Signal_ControlFeedBack(0, telectrl.m_nDataID, 0, QString::number(nRecvAckCot));
			telectrl.m_nCtrlType = -1;
		}
	
}

//参数读取
/*********************************************************************************************************
** \brief OnRecvDevReadRequestAck(char *pBuff, int nLength)
** \details 参数读取
** \param argl   一单元十六进制数据包
** \param arg2   数据包的长度
** \return void
** \author ww
** \date 2016年5月27日
** \note
********************************************************************************************************/
void CApduRecver::OnRecvDevReadRequestAck(char* pBuff, int nLength)
{
    Q_UNUSED(pBuff);
    Q_UNUSED(nLength);

	if (nLength == 0)
	{
		return;
	}
	//定值
	DEV_BASE pDevData;
	//
	ASDU_BASE* pAsdudz = (ASDU_BASE*)(pBuff);


	//asdu公共地址
	pDevData.m_nAsduID = pAsdudz->asduAddr.GetAddr();
	//传送原因
	pDevData.m_nCto = pAsdudz->cot.GetCot();

	//定值区号
	//ASDUADDR2 *pAreaCode = (ASDUADDR2*)(pBuff + sizeof(APCI) + sizeof(ASDU_BASE));

	//当前数据在所节点
	int nSetIndex = 0;
	//数据个数

	//数据长度
	int nPagLength = sizeof(ASDU_BASE) + sizeof(ASDUADDR2) + sizeof(unsigned char);

	int nItemCount = pAsdudz->vsq & 0x7f;
	
	for (int i = 0; i < nItemCount; i++)
	{
		DEV_DATA tDevData;
		//信息体地址
		INFOADDR3 *pAddr = (INFOADDR3*)(pBuff + nPagLength + nSetIndex);
		tDevData.nAddress = pAddr->GetAddr();

		//tag类型
		tDevData.nTagType = pBuff[nPagLength + nSetIndex + sizeof(INFOADDR3)];

		//数据长度
		tDevData.nLength = pBuff[nPagLength + nSetIndex + sizeof(INFOADDR3) + 1];
		//数据值
		if (tDevData.nTagType == 2)
		{
			//int
			int *pp = (int *)(pBuff + nPagLength + nSetIndex + sizeof(INFOADDR3) + 2);
			tDevData.strValue = QString::number(*pp);
		}
		else if (tDevData.nTagType == 45)
		{
			//ushort
			unsigned short *puShort = (unsigned short*)(pBuff + nPagLength + nSetIndex + sizeof(INFOADDR3) + 2);
			tDevData.strValue = QString::number(*puShort);
		}
		else if (tDevData.nTagType == 38)
		{
			//float
			 char *pfStart = pBuff + nPagLength + nSetIndex + sizeof(INFOADDR3) + 2;
			 float tt;

			 *(uint32_t *)(&tt) = pfStart[0] + pfStart[1] * 0x100 + pfStart[2] * 0x10000 + pfStart[3] * 0x1000000;

			tDevData.strValue = QString::number(tt);
		}
		else if (tDevData.nTagType == 4)
		{
			//string
			tDevData.strValue = QString::fromLocal8Bit(pBuff + nPagLength + nSetIndex + sizeof(INFOADDR3) + 2,tDevData.nLength);
		}

		pDevData.m_lstData.append(tDevData);

		nSetIndex += nPagLength + sizeof(INFOADDR3) + 2 + tDevData.nLength;
	}

	//发送信号 
	emit Signal_ReadFixData(pDevData);
}

//参数写入
void CApduRecver::OnRecvDevWriteRequestAck(char *pBuff, int nLength)
{
	if (nLength == 0)
	{
		return;
	}
    //qDebug() << "Recv2:" << QByteArray(pBuff, nLength).toHex();

	if (nLength == 9)
	{
		//固化报文
		ASDU_BASE* pAsdudz = (ASDU_BASE*)pBuff;

		emit Signal_devWriteBack(pAsdudz->type, pAsdudz->cot.GetCot(), 0);

	}
}

/*********************************************************************************************************
** \brief OnRecvBinarySinglePoint(char *pBuff, int nLength)
** \details 解析单点遥信数据
** \param argl   一单元十六进制数据包
** \param arg2   数据包的长度
** \return void   
** \author ww
** \date 2016年5月27日
** \note
********************************************************************************************************/
void CApduRecver::OnRecvBinarySinglePoint(char *pBuff, int nLength)
{
	if (nLength == 0)
	{
		return;
	}

    ASDU_BASE* pBase = (ASDU_BASE*)pBuff;

    if(pBase->IsSQ1())
    {
        //连续单点遥信
        ASDU1_SQ1* pAsdu1SQ1 = (ASDU1_SQ1*)pBuff;

        int nDeviceAddr = pAsdu1SQ1->asduAddr.GetAddr();

        int nCount =  pAsdu1SQ1->GetItemCount();

        int nPointID = pAsdu1SQ1->infoaddr.GetAddr();

        for(int i=0;i<nCount;i++)
        {
            int nValue  = pAsdu1SQ1->GetValue(i);

            //发出信号   或者存入共享内存  待定
            emit Signal_OnePointRemote(nDeviceAddr,nPointID,nValue);

            nPointID++;
        }
    }
    else
    {
        ASDU1_SQ0* pAsdu1SQ0 = (ASDU1_SQ0*)pBuff;

        int nDeviceAddr = pAsdu1SQ0->asduAddr.GetAddr();

        int nCount =  pAsdu1SQ0->GetItemCount();

        for(int i=0;i<nCount;i++)
        {
            int nPointID= pAsdu1SQ0->m_data[i].infoaddr.GetAddr();

            int nValue = pAsdu1SQ0->GetValue(i);
            //发出信号   或者存入共享内存  待定
            emit Signal_onePointDisRemote(nDeviceAddr,nPointID,nValue);

        }
    }
}

void CApduRecver::OnRecvBinaryDoublePoint(char *pBuff, int nLength)
{
	if (nLength == 0)
	{
		return;
	}

	ASDU_BASE* pBase = (ASDU_BASE*)pBuff;
	if (pBase->IsSQ1())
	{
		ASDU3_SQ1* pAsdu3SQ1 = (ASDU3_SQ1*)pBuff;

		int nDeviceAddr = pAsdu3SQ1->asduAddr.GetAddr();

		int nCount = pAsdu3SQ1->GetItemCount();

		int nPointID = pAsdu3SQ1->infoaddr.GetAddr();

		for (int i = 0; i < nCount; i++)
		{
			int nValue = pAsdu3SQ1->GetValue(i);//>m_siq[i];

			emit Signal_DoublePointRemote(nDeviceAddr, nPointID, nValue);
			
			nPointID++;
		}
	}
	else
	{
		ASDU3_SQ0* pAsdu3SQ0 = (ASDU3_SQ0*)pBuff;

		int nDeviceAddr = pAsdu3SQ0->asduAddr.GetAddr();

		int nCount = pAsdu3SQ0->GetItemCount();

		for (int i = 0; i < nCount; i++)
		{
			int nPointID = pAsdu3SQ0->m_data[i].infoaddr.GetAddr();
			
			int nValue = pAsdu3SQ0->GetValue(i);//>m_siq[i].siq;

			emit Signal_DoublePointDisRemote(nDeviceAddr, nPointID, nValue);

		}
	}
}

/*********************************************************************************************************
** \brief OnRecvBinaryGroupPoint(char *pBuff, int nLength)
** \details 成组单点遥信
** \param argl   一单元十六进制数据包
** \param arg2   数据包的长度
** \return void
** \author ww
** \date 2016年5月27日
** \note
********************************************************************************************************/
void CApduRecver::OnRecvBinaryGroupPoint(char* pBuff, int nLength)
{
	if (nLength <= 0)
	{
		return;
	}

	ASDU_BASE* pBase = (ASDU_BASE*)pBuff;
	if (pBase->IsSQ1())
	{
		ASDU20_SQ1* pAsdu20SQ1 = (ASDU20_SQ1*)pBuff;

		int nDeviceAddr = pAsdu20SQ1->asduAddr.GetAddr();

		int nCount = pAsdu20SQ1->GetItemCount();

		int nPointID = pAsdu20SQ1->m_infoaddr.GetAddr();

		for (int i = 0; i < nCount; i++)
		{
			nPointID += 16 * i;

			int nValue = pAsdu20SQ1->GetValue(i);//m_siq[i].SPI;

// 			QDS qds = pAsdu20SQ1->GetQDS(i);
// 
// 			int nQuality = QUALITY_NORMAL;
// 
// 			if (qds.OV == 1) nQuality |= QUALITY_OVERFLOW;
// 			if (qds.BL == 1) nQuality |= QUALITY_BLOCKED;
// 			if (qds.SB == 1) nQuality |= QUALITY_REPLACED;
// 			if (qds.NT == 1) nQuality |= QUALITY_UNCURRENT;
// 			if (qds.IV == 1) nQuality |= QUALITY_INVALID;

			for (int k = 0; k < 16; k++)
			{
				int nBitValue = (nValue & (1 << k)) >> k;
				emit Signal_BinaryGroupPoint(nDeviceAddr, nPointID + k, nBitValue);
			}
		}
	}
	else
	{
		ASDU20_SQ0* pAsdu20SQ0 = (ASDU20_SQ0*)pBuff;

		int nCount = pAsdu20SQ0->GetItemCount();

		for (int i = 0; i < nCount; i++)
		{
			int nPointID = pAsdu20SQ0->m_data[i].m_infoaddr.GetAddr();

			int nDeviceAddr = pAsdu20SQ0->asduAddr.GetAddr();

			int nValue = pAsdu20SQ0->GetValue(i);//

//			QDS qds = pAsdu20SQ0->GetQDS(i);

// 			int nQuality = QUALITY_NORMAL;
// 
// 			if (qds.OV == 1) nQuality |= QUALITY_OVERFLOW;
// 			if (qds.BL == 1) nQuality |= QUALITY_BLOCKED;
// 			if (qds.SB == 1) nQuality |= QUALITY_REPLACED;
// 			if (qds.NT == 1) nQuality |= QUALITY_UNCURRENT;
// 			if (qds.IV == 1) nQuality |= QUALITY_INVALID;

			for (int k = 0; k < 16; k++)
			{
				int nBitValue = (nValue & (1 << k)) >> k;
				emit Signal_BinaryGroupDisPoint(nDeviceAddr, nPointID + k, nBitValue);
			}
		}
	}
}

/*********************************************************************************************************
** \brief OnRecvBinarySinglePointTime(char *pBuff, int nLength)
** \details 带短时标的单点信息  SEO
** \param argl   一单元十六进制数据包
** \param arg2   数据包的长度
** \return void
** \author ww
** \date 2016年5月27日
** \note
********************************************************************************************************/
void CApduRecver::OnRecvBinarySinglePointShortTime(char* pBuff, int nLength)
{
	if (nLength <= 0)
	{
		return;
	}
	ASDU2_SQ0* pAsdu2SQ0 = (ASDU2_SQ0*)pBuff;

	int nDeviceAddr = pAsdu2SQ0->asduAddr.GetAddr();

	int nCount = pAsdu2SQ0->GetItemCount();


	for (int i = 0; i < nCount; i++)
	{
		int nPointID = pAsdu2SQ0->m_data[i].m_infoaddr.GetAddr();

		int nValue = pAsdu2SQ0->GetValue(i);
		CP24Time2a time = pAsdu2SQ0->GetTime(i);
		//TODO   SOE的时间需要重新设置
// 		QDateTime dataTime;
// 		dataTime.setTime(QTime(0, time.minute, MYMAKE16(time.msL, time.msH)));

		SIQ siq = pAsdu2SQ0->GetSIQ(i);

		int nQuality = QUALITY_NORMAL;

		if (siq.BL == 1) nQuality |= QUALITY_BLOCKED;
		if (siq.SB == 1) nQuality |= QUALITY_REPLACED;
		if (siq.NT == 1) nQuality |= QUALITY_UNCURRENT;
		if (siq.IV == 1) nQuality |= QUALITY_INVALID;

        emit Signal_BinarySignalPointShortTime(nDeviceAddr, nPointID, nValue, 0, time.Dump());
	}

}

/*********************************************************************************************************
** \brief OnRecvBinarySinglePointTime(char *pBuff, int nLength)
** \details SOE
** \param argl   一单元十六进制数据包
** \param arg2   数据包的长度
** \return void
** \author ww
** \date 2016年5月27日
** \note
********************************************************************************************************/
void CApduRecver::OnRecvBinarySinglePointTime(char* pBuff, int nLength)
{
	if (nLength == 0)
	{
		return;
	}
	ASDU30_SQ0* pAsdu30SQ0 = (ASDU30_SQ0*)pBuff;
	int nDeviceAddr = pAsdu30SQ0->asduAddr.GetAddr();
	int nCount = pAsdu30SQ0->GetItemCount();

	for (int i = 0; i < nCount; i++)
	{
		int nPointID = pAsdu30SQ0->m_data[i].m_infoaddr.GetAddr();

		int nValue = pAsdu30SQ0->GetValue(i);//>m_data[i].m_siq;
		CP56Time2a time = pAsdu30SQ0->GetTime(i);
// 		int nMilliSec = MYMAKE16(time.msL, time.msH);
// 
// 		QDateTime dateTime(QDate(time.year + 2000, time.month, time.day), QTime(time.hour, time.minute, nMilliSec / 1000, nMilliSec % 1000));

		SIQ siq = pAsdu30SQ0->GetSIQ(i);
		int nQuality = QUALITY_NORMAL;

		if (siq.BL == 1) nQuality |= QUALITY_BLOCKED;
		if (siq.SB == 1) nQuality |= QUALITY_REPLACED;
		if (siq.NT == 1) nQuality |= QUALITY_UNCURRENT;
		if (siq.IV == 1) nQuality |= QUALITY_INVALID;

		emit Signal_SoeBinarySinglePointTime(nDeviceAddr, nPointID, nValue, nQuality, time.Dump());
	}
}

/*********************************************************************************************************
** \brief OnRecvAnalogNormalized(char *pBuff, int nLength)
** \details 带短时标的双点信息  SEO
** \param argl   一单元十六进制数据包
** \param arg2   数据包的长度
** \return void
** \author ww
** \date 2016年5月27日
** \note
********************************************************************************************************/
void CApduRecver::OnRecvBinaryDoublePointShortTime(char* pBuff, int nLength)
{
	if (nLength <= 0)
	{
		return;
	}
	ASDU4_SQ0* pAsdu4SQ0 = (ASDU4_SQ0*)pBuff;

	int nDeviceAddr = pAsdu4SQ0->asduAddr.GetAddr();

	int nCount = pAsdu4SQ0->GetItemCount();

	for (int i = 0; i < nCount; i++)
	{
		int nPointID = pAsdu4SQ0->m_data[i].m_infoaddr.GetAddr();

		int nValue = pAsdu4SQ0->GetValue(i);//>m_data[i].m_siq;
		CP24Time2a time = pAsdu4SQ0->m_data[i].m_time;
// 		QDateTime dataTime;
// 		dataTime.setTime(QTime(0, time.minute, MYMAKE16(time.msL, time.msH)));
		DIQ diq = pAsdu4SQ0->GetDIQ(i);

		int nQuality = QUALITY_NORMAL;

		if (diq.BL == 1) nQuality |= QUALITY_BLOCKED;
		if (diq.SB == 1) nQuality |= QUALITY_REPLACED;
		if (diq.NT == 1) nQuality |= QUALITY_UNCURRENT;
		if (diq.IV == 1) nQuality |= QUALITY_INVALID;

		emit Signal_BinaryDoublePointShortTime(nDeviceAddr, nPointID, nValue, nQuality, time.Dump());
	}
}

/*********************************************************************************************************
** \brief OnRecvAnalogNormalized(char *pBuff, int nLength)
** \details 带长时标的双点信息  SEO
** \param argl   一单元十六进制数据包
** \param arg2   数据包的长度
** \return void
** \author ww
** \date 2016年5月27日
** \note
********************************************************************************************************/
void CApduRecver::OnRecvBinaryDoublePointTime(char* pBuff, int nLength)
{
	if (nLength <= 0)
	{
		return;
	}
	ASDU31_SQ0* pAsdu31SQ0 = (ASDU31_SQ0*)pBuff;

	int nDeviceAddr = pAsdu31SQ0->asduAddr.GetAddr();

	int nCount = pAsdu31SQ0->GetItemCount();

	for (int i = 0; i < nCount; i++)
	{
		int nPointID = pAsdu31SQ0->m_data[i].m_infoaddr.GetAddr();

		int nValue = pAsdu31SQ0->GetValue(i);//>m_data[i].m_siq;
		CP56Time2a time = pAsdu31SQ0->m_data[i].m_time;
// 		QDateTime dateTime(QDate(time.year, time.month, time.day), QTime(time.hour, time.minute, MYMAKE16(time.msL, time.msH)));

		DIQ diq = pAsdu31SQ0->GetDIQ(i);

		int nQuality = QUALITY_NORMAL;

		if (diq.BL == 1) nQuality |= QUALITY_BLOCKED;
		if (diq.SB == 1) nQuality |= QUALITY_REPLACED;
		if (diq.NT == 1) nQuality |= QUALITY_UNCURRENT;
		if (diq.IV == 1) nQuality |= QUALITY_INVALID;

		emit Signal_BinaryDoublePointShortTime(nDeviceAddr, nPointID, nValue, 1, time.Dump());
	}
}

/*********************************************************************************************************
** \brief OnRecvAnalogNormalized(char *pBuff, int nLength)
** \details 归一化遥测数据解析
** \param argl   一单元十六进制数据包
** \param arg2   数据包的长度
** \return void
** \author ww
** \date 2016年5月27日
** \note
********************************************************************************************************/
void CApduRecver::OnRecvAnalogNormalized(char* pBuff, int nLength)
{
	if (nLength == 0)
	{
		return;
	}
	ASDU_BASE* pAsduBase = (ASDU_BASE*)pBuff;
    QMap<int, short> mapPointValue;

	if (pAsduBase->IsSQ1())
	{
		ASDU9_SQ1* pAsdu9SQ1 = (ASDU9_SQ1*)pBuff;

		int nDeviceAddr = pAsdu9SQ1->asduAddr.GetAddr();

		int nCount = pAsdu9SQ1->GetItemCount();

		int nPointID = pAsdu9SQ1->infoaddr.GetAddr();


		for (int i = 0; i < nCount; i++)
		{
			float fValue = pAsdu9SQ1->GetValue(i);

			QDS qds = pAsdu9SQ1->GetQDS(i);

			int nQuality = QUALITY_NORMAL;

			if (qds.OV == 1) nQuality |= QUALITY_OVERFLOW;
			if (qds.BL == 1) nQuality |= QUALITY_BLOCKED;
			if (qds.SB == 1) nQuality |= QUALITY_REPLACED;
			if (qds.NT == 1) nQuality |= QUALITY_UNCURRENT;
			if (qds.IV == 1) nQuality |= QUALITY_INVALID;

			emit Signal_AnalogNormal(nDeviceAddr, nPointID, fValue, nQuality);
            //定值修改
            mapPointValue[nPointID] = fValue;


			nPointID++;
		}
	}
	else
	{
		ASDU9_SQ0* pAsdu9SQ0 = (ASDU9_SQ0*)pBuff;

		int nDeviceAddr = pAsdu9SQ0->asduAddr.GetAddr();

		int nCount = pAsdu9SQ0->GetItemCount();

		for (int i = 0; i < nCount; i++)
		{
			int nPointID = pAsdu9SQ0->m_data[i].infoaddr.GetAddr();

			float fValue = pAsdu9SQ0->GetValue(i);

			QDS qds = pAsdu9SQ0->GetQDS(i);

			int nQuality = QUALITY_NORMAL;

			if (qds.OV == 1) nQuality |= QUALITY_OVERFLOW;
			if (qds.BL == 1) nQuality |= QUALITY_BLOCKED;
			if (qds.SB == 1) nQuality |= QUALITY_REPLACED;
			if (qds.NT == 1) nQuality |= QUALITY_UNCURRENT;
			if (qds.IV == 1) nQuality |= QUALITY_INVALID;

            emit Signal_AnalogNormal(nDeviceAddr, nPointID, fValue, nQuality);

            //定值修改
            mapPointValue[nPointID] = fValue;

		}
	}

    if (!mapPointValue.isEmpty())
    {
   	    emit Signal_DevReadBack(mapPointValue);
    }
}

/*********************************************************************************************************
** \brief OnRecvAnalogShortFloat(char *pBuff, int nLength)
** \details 带短时标的归一化测量值
** \param argl   一单元十六进制数据包
** \param arg2   数据包的长度
** \return void
** \author ww
** \date 2016年5月27日
** \note
********************************************************************************************************/
void CApduRecver::OnRecvAnalogNormalizedShortTime(char* pBuff, int nLen)
{
	if (nLen <= 0)
	{
		return;
	}
	ASDU10_SQ0* pAsdu10SQ0 = (ASDU10_SQ0*)pBuff;
    QMap<int, short> mapPointValue;


	int nCount = pAsdu10SQ0->GetItemCount();
	int nDeviceAddr = pAsdu10SQ0->asduAddr.GetAddr();

	for (int i = 0; i < nCount; i++)
	{
		int nPointID = pAsdu10SQ0->m_data[i].m_infoaddr.GetAddr();

		float fValue = pAsdu10SQ0->GetValue(i);

		QDS qds = pAsdu10SQ0->GetQDS(i);

		int nQuality = QUALITY_NORMAL;

		if (qds.OV == 1) nQuality |= QUALITY_OVERFLOW;
		if (qds.BL == 1) nQuality |= QUALITY_BLOCKED;
		if (qds.SB == 1) nQuality |= QUALITY_REPLACED;
		if (qds.NT == 1) nQuality |= QUALITY_UNCURRENT;
		if (qds.IV == 1) nQuality |= QUALITY_INVALID;

        emit Signal_AnalogNormal(nDeviceAddr, nPointID, fValue, nQuality);

        mapPointValue[nPointID] = fValue;

	}

    if (!mapPointValue.isEmpty())
    {
        emit Signal_DevReadBack(mapPointValue);
    }
}

/*********************************************************************************************************
** \brief OnRecvAnalogShortFloat(char *pBuff, int nLength)
** \details 标度化测量值
** \param argl   一单元十六进制数据包
** \param arg2   数据包的长度
** \return void
** \author ww
** \date 2016年5月27日
** \note
********************************************************************************************************/
void CApduRecver::OnRecvAnalogScaled(char* pBuff, int nLength)
{
	if (nLength <= 0)
	{
		return;
	}
	ASDU_BASE* pAsduBase = (ASDU_BASE*)pBuff;

    QMap<int, short> mapPointValue;

	if (pAsduBase->IsSQ1())
	{
		ASDU11_SQ1* pAsdu11SQ1 = (ASDU11_SQ1*)pBuff;

		int nDeviceAddr = pAsdu11SQ1->asduAddr.GetAddr();

		int nCount = pAsdu11SQ1->GetItemCount();

		int nPointID = pAsdu11SQ1->infoaddr.GetAddr();

		for (int i = 0; i < nCount; i++)
		{
			float fValue = pAsdu11SQ1->GetValue(i);

			QDS qds = pAsdu11SQ1->GetQDS(i);

			int nQuality = QUALITY_NORMAL;

			if (qds.OV == 1) nQuality |= QUALITY_OVERFLOW;
			if (qds.BL == 1) nQuality |= QUALITY_BLOCKED;
			if (qds.SB == 1) nQuality |= QUALITY_REPLACED;
			if (qds.NT == 1) nQuality |= QUALITY_UNCURRENT;
			if (qds.IV == 1) nQuality |= QUALITY_INVALID;
			//通过同一信号发出去  这边没必要进行区分
            emit Signal_ScaledAnalogNormal(nDeviceAddr, nPointID, fValue, nQuality);

            //定值修改
            mapPointValue[nPointID] = fValue;

			nPointID++;
		}
	}
	else
	{
		ASDU11_SQ0* pAsdu11SQ0 = (ASDU11_SQ0*)pBuff;

		int nDeviceAddr = pAsdu11SQ0->asduAddr.GetAddr();

		int nCount = pAsdu11SQ0->GetItemCount();

		for (int i = 0; i < nCount; i++)
		{
			int nPointID = pAsdu11SQ0->m_data[i].infoaddr.GetAddr();

			float fValue = pAsdu11SQ0->GetValue(i);

			QDS qds = pAsdu11SQ0->GetQDS(i);

			int nQuality = QUALITY_NORMAL;

			if (qds.OV == 1) nQuality |= QUALITY_OVERFLOW;
			if (qds.BL == 1) nQuality |= QUALITY_BLOCKED;
			if (qds.SB == 1) nQuality |= QUALITY_REPLACED;
			if (qds.NT == 1) nQuality |= QUALITY_UNCURRENT;
			if (qds.IV == 1) nQuality |= QUALITY_INVALID;
			//通过同一信号发出去  这边没必要进行区分
			emit Signal_AnalogNormalShortTime(nDeviceAddr, nPointID, fValue, nQuality);

            //定值修改
            mapPointValue[nPointID] = fValue;
		}
	}

    if (!mapPointValue.isEmpty())
    {
        emit Signal_DevReadBack(mapPointValue);
    }
}

/*********************************************************************************************************
** \brief OnRecvAnalogShortFloat(char *pBuff, int nLength)
** \details 带短时标的标度化测量值
** \param argl   一单元十六进制数据包
** \param arg2   数据包的长度
** \return void
** \author ww
** \date 2016年5月27日
** \note
********************************************************************************************************/
void CApduRecver::OnRecvAnalogScaledShortTime(char* pBuff, int nLen)
{
	if (nLen <= 0)
	{
		return;
	}
	ASDU12_SQ0* pAsdu12SQ0 = (ASDU12_SQ0*)pBuff;

    QMap<int, short> mapPointValue;

	int nCount = pAsdu12SQ0->GetItemCount();
	int nDeviceAddr = pAsdu12SQ0->asduAddr.GetAddr();

	for (int i = 0; i < nCount; i++)
	{
		int nPointID = pAsdu12SQ0->m_data[i].m_infoaddr.GetAddr();

		float fValue = pAsdu12SQ0->GetValue(i);

		QDS qds = pAsdu12SQ0->GetQDS(i);

		int nQuality = QUALITY_NORMAL;

		if (qds.OV == 1) nQuality |= QUALITY_OVERFLOW;
		if (qds.BL == 1) nQuality |= QUALITY_BLOCKED;
		if (qds.SB == 1) nQuality |= QUALITY_REPLACED;
		if (qds.NT == 1) nQuality |= QUALITY_UNCURRENT;
		if (qds.IV == 1) nQuality |= QUALITY_INVALID;
		//通过同一信号发出去  这边没必要进行区分
        emit Signal_ScaledAnalogNormal(nDeviceAddr, nPointID, fValue, nQuality);

        //定值修改
        mapPointValue[nPointID] = fValue;
	}

    if (!mapPointValue.isEmpty())
    {
        emit Signal_DevReadBack(mapPointValue);
    }
}

/*********************************************************************************************************
** \brief OnRecvAnalogShortFloat(char *pBuff, int nLength)
** \details 累计量  遥脉
** \param argl   一单元十六进制数据包
** \param arg2   数据包的长度
** \return void
** \author ww
** \date 2016年5月27日
** \note
********************************************************************************************************/
void CApduRecver::OnRecvKwh(char* pBuff, int nLength)
{
	if (nLength <= 0)
	{
		return;
	}
	ASDU_BASE* pBase = (ASDU_BASE*)pBuff;
	if (pBase->IsSQ1())
	{
		ASDU15_SQ1* pAsdu15SQ1 = (ASDU15_SQ1*)pBuff;
		int nDeviceID = pAsdu15SQ1->asduAddr.GetAddr();
		int nCount = pAsdu15SQ1->GetItemCount();
		int nPointID = pAsdu15SQ1->infoaddr.GetAddr();

		for (int i = 0; i < nCount; i++)
		{
			int nValue = pAsdu15SQ1->m_data[i].m_nValue;
			emit Signal_KwhNormal(nDeviceID, nPointID, nValue);

			nPointID++;
		}
	}
	else
	{
		ASDU15_SQ0* pAsdu15SQ0 = (ASDU15_SQ0*)pBuff;
		int nDeviceID = pAsdu15SQ0->asduAddr.GetAddr();
		int nCount = pAsdu15SQ0->GetItemCount();

		for (int i = 0; i < nCount; i++)
		{
			int nValue = pAsdu15SQ0->m_data[i].m_nValue;
			int nPointID = pAsdu15SQ0->m_data[i].m_infoaddr.GetAddr();
			emit Signal_KwhDisNormal(nDeviceID, nPointID, nValue);
		}
	}
}

/*********************************************************************************************************
** \brief OnRecvKwhShortTime(char *pBuff, int nLength)
** \details 带短时标的累计量  遥脉
** \param argl   一单元十六进制数据包
** \param arg2   数据包的长度
** \return void
** \author ww
** \date 2016年5月27日
** \note
********************************************************************************************************/
void CApduRecver::OnRecvKwhShortTime(char* pBuff, int nLength)
{
	if (nLength <= 0)
	{
		return;
	}
	ASDU16_SQ0* pAsdu16SQ0 = (ASDU16_SQ0*)pBuff;
	int nCount = pAsdu16SQ0->GetItemCount();
	int nDeviceID = pAsdu16SQ0->asduAddr.GetAddr();

	for (int i = 0; i < nCount; i++)
	{
		float fValue = pAsdu16SQ0->m_data[i].m_fValue;
		int nPointID = pAsdu16SQ0->m_data[i].m_infoaddr.GetAddr();

		emit Signal_KwhShortTimeNormal(nDeviceID, nPointID, fValue);
	}
}

/*********************************************************************************************************
** \brief OnRecvKwhLongTime(char *pBuff, int nLength)
** \details 带长时标的累计量  遥脉
** \param argl   一单元十六进制数据包
** \param arg2   数据包的长度
** \return void
** \author ww
** \date 2016年5月27日
** \note
********************************************************************************************************/
void CApduRecver::OnRecvKwhLongTime(char* pBuff, int nLength)
{
	if (nLength <= 0)
	{
		return;
	}
	ASDU37_SQ0* pAsdu37SQ0 = (ASDU37_SQ0*)pBuff;
	int nCount = pAsdu37SQ0->GetItemCount();
	int nDeviceID = pAsdu37SQ0->asduAddr.GetAddr();

	for (int i = 0; i < nCount; i++)
	{
		float fValue = pAsdu37SQ0->m_data[i].m_bcr.number;
		int nPointID = pAsdu37SQ0->m_data[i].m_infoaddr.GetAddr();
		emit Signal_KwhLongTimeNormal(nDeviceID, nPointID, fValue);
	}
}

/*********************************************************************************************************
** \brief OnRecvAnalogShortFloat(char *pBuff, int nLength)
** \details 浮点型遥测数据解析
** \param argl   一单元十六进制数据包
** \param arg2   数据包的长度
** \return void
** \author ww
** \date 2016年5月27日
** \note
********************************************************************************************************/
void CApduRecver::OnRecvAnalogShortFloat(char* pBuff, int nLength)
{
	if (nLength == 0)
	{
		return;
	}
	ASDU_BASE* pAsduBase = (ASDU_BASE*)pBuff;

    QMap<int, short> mapPointValue;

	if (pAsduBase->IsSQ1())
	{
		ASDU13_SQ1* pAsdu13SQ1 = (ASDU13_SQ1*)pBuff;

		int nDeviceAddr = pAsdu13SQ1->asduAddr.GetAddr();

		int nCount = pAsdu13SQ1->GetItemCount();

		int nPointID = pAsdu13SQ1->infoaddr.GetAddr();

		for (int i = 0; i < nCount; i++)
		{
			float fValue = pAsdu13SQ1->GetValue(i);

			QDS qds = pAsdu13SQ1->GetQDS(i);

			int nQuality = QUALITY_NORMAL;

			if (qds.OV == 1) nQuality |= QUALITY_OVERFLOW;
			if (qds.BL == 1) nQuality |= QUALITY_BLOCKED;
			if (qds.SB == 1) nQuality |= QUALITY_REPLACED;
			if (qds.NT == 1) nQuality |= QUALITY_UNCURRENT;
			if (qds.IV == 1) nQuality |= QUALITY_INVALID;

			emit Signal_AnalogShortFloat(nDeviceAddr, nPointID, fValue, nQuality);

            //定值修改
            mapPointValue[nPointID] = fValue;

			nPointID++;
		}
	}
	else
	{
		ASDU13_SQ0* pAsdu13SQ0 = (ASDU13_SQ0*)pBuff;

		int nDeviceAddr = pAsdu13SQ0->asduAddr.GetAddr();

		int nCount = pAsdu13SQ0->GetItemCount();

		for (int i = 0; i < nCount; i++)
		{
			int nPointID = pAsdu13SQ0->m_data[i].infoaddr.GetAddr();

			float fValue = pAsdu13SQ0->GetValue(i);

			QDS qds = pAsdu13SQ0->GetQDS(i);

			int nQuality = QUALITY_NORMAL;

			if (qds.OV == 1) nQuality |= QUALITY_OVERFLOW;
			if (qds.BL == 1) nQuality |= QUALITY_BLOCKED;
			if (qds.SB == 1) nQuality |= QUALITY_REPLACED;
			if (qds.NT == 1) nQuality |= QUALITY_UNCURRENT;
			if (qds.IV == 1) nQuality |= QUALITY_INVALID;

            emit Signal_AnalogShortFloat(nDeviceAddr, nPointID, fValue, nQuality);


            //定值修改
            mapPointValue[nPointID] = fValue;
		}
	}

    if (!mapPointValue.isEmpty())
    {
        emit Signal_DevReadBack(mapPointValue);
    }
}

//带短时标的短浮点数测量值
/*********************************************************************************************************
** \brief OnRecvAnalogShortFloatShortTime(char *pBuff, int nLength)
** \details 带短时标的短浮点数测量值
** \param argl   一单元十六进制数据包
** \param arg2   数据包的长度
** \return void
** \author ww
** \date 2016年5月27日
** \note
********************************************************************************************************/
void CApduRecver::OnRecvAnalogShortFloatShortTime(char* pBuff, int nLen)
{
	if (nLen <= 0)
	{
		return;
	}
	ASDU14_SQ0* pAsdu14SQ0 = (ASDU14_SQ0*)pBuff;

    QMap<int, short> mapPointValue;

	int nCount = pAsdu14SQ0->GetItemCount();
	int nDeviceAddr = pAsdu14SQ0->asduAddr.GetAddr();

	for (int i = 0; i < nCount; i++)
	{
		int nPointID = pAsdu14SQ0->m_data[i].m_infoaddr.GetAddr();

		float fValue = pAsdu14SQ0->GetValue(i);

		QDS qds = pAsdu14SQ0->GetQDS(i);

		int nQuality = QUALITY_NORMAL;

		if (qds.OV == 1) nQuality |= QUALITY_OVERFLOW;
		if (qds.BL == 1) nQuality |= QUALITY_BLOCKED;
		if (qds.SB == 1) nQuality |= QUALITY_REPLACED;
		if (qds.NT == 1) nQuality |= QUALITY_UNCURRENT;
		if (qds.IV == 1) nQuality |= QUALITY_INVALID;
		//通过同一信号发出去  这边没必要进行区分
        emit Signal_AnalogShortFloat(nDeviceAddr, nPointID, fValue, nQuality);

        //定值修改
        mapPointValue[nPointID] = fValue;
	}

    if (!mapPointValue.isEmpty())
    {
        emit Signal_DevReadBack(mapPointValue);
    }
}

//不带品质描述词的归一化测量值
/*********************************************************************************************************
** \brief OnRecvAnalogNormalizedNoQuality(char *pBuff, int nLength)
** \details 不带品质描述词的归一化测量值
** \param argl   一单元十六进制数据包
** \param arg2   数据包的长度
** \return void
** \author ww
** \date 2016年5月27日
** \note
********************************************************************************************************/
void CApduRecver::OnRecvAnalogNormalizedNoQuality(char* pBuff, int nLen)
{
	if (nLen <= 0)
	{
		return;
	}
	ASDU_BASE* pAsduBase = (ASDU_BASE*)pBuff;
    

	if (pAsduBase->IsSQ1())
	{
		ASDU21_SQ1* pAsdu21SQ1 = (ASDU21_SQ1*)pBuff;

		int nCount = pAsdu21SQ1->GetItemCount();

		int nPointID = pAsdu21SQ1->m_infoaddr.GetAddr();

		int nDeviceAddr = pAsdu21SQ1->asduAddr.GetAddr();
		for (int i = 0; i < nCount; i++)
		{
			float fValue = pAsdu21SQ1->GetValue(i);
			//通过同一信号发出去  这边没必要进行区分
            emit Signal_AnalogShortFloat(nDeviceAddr, nPointID, fValue, 0);

			nPointID++;
		}
	}
	else
	{
		ASDU21_SQ0* pAsdu21SQ0 = (ASDU21_SQ0*)pBuff;

		int nCount = pAsdu21SQ0->GetItemCount();

		int nDeviceAddr = pAsdu21SQ0->asduAddr.GetAddr();

		for (int i = 0; i < nCount; i++)
		{
			int nPointID = pAsdu21SQ0->m_data[i].m_infoaddr.GetAddr();

			float fValue = pAsdu21SQ0->GetValue(i);
			//通过同一信号发出去  这边没必要进行区分
            emit Signal_AnalogShortFloat(nDeviceAddr, nPointID, fValue, 0);
		}
	}

}

/*********************************************************************************************************
** \brief AnalyseU(char *pBuff, int nLength)
** \details 解析U格式类型数据
** \param argl   一单元十六进制数据包
** \param arg2   数据包的长度
** \return void
** \author ww
** \date 2016年5月27日
** \note
********************************************************************************************************/
bool CApduRecver::AnalyseU(char *pBuff, int nLength)
{
	Q_UNUSED(nLength);
    APCI* pApci = (APCI*)pBuff;

    //1、解析
    int nUType = pApci->GetUType();

    switch(nUType)
    {
    case TESTFR_CON://收到TESTFR 确认
    {
        //qDebug()<<"收到TESTFR确认信息";
    }
        break;
    case TESTFR_ACT://收到TESTFR 激活
    {
        //发送TESTFR确认
        m_pComm104Pln->getSender()->Send_U(TESTFR_CON);
    }
        break;
    case STARTDT_CON://收到STARTDT确认
    {
		SendRequestAllMsg();
    }
        break;
    case STARTDT_ACT://收到STARTDT激活
    {
        m_pComm104Pln->getSender()->Send_U(STARTDT_CON);
    }
        break;
    case STOPDT_CON://收到STOPDT确认
    {
        //TODO
    }
        break;
    case STOPDT_ACT://收到STOPDT激活
    {
		m_pComm104Pln->getSender()->Send_U(STOPDT_CON);
    }
        break;
    default:
        break;
    }

    return true;
}

/*********************************************************************************************************
** \brief AnalyseU(char *pBuff, int nLength)
** \details 解析U格式类型数据
** \param argl   一单元十六进制数据包
** \param arg2   数据包的长度
** \return void
** \author ww
** \date 2016年5月27日
** \note
********************************************************************************************************/
void CApduRecver::SendRequestAllMsg()
{
	//启动时 将会发送三种请求数据  召唤全数据  召唤总电度  对时求情    
	//召唤全数据
	NBM_TELECTRL telectrl;
	telectrl.m_nStationID = 0;
	telectrl.m_nDeviceID = 0;
	telectrl.m_nCtrlType = TELECTRL_REQUEST_GENERALCALL;
	bool IsOK = m_pComm104Pln->OnCommand(&telectrl);
	if (!IsOK)
	{
		return;
	}

	//召唤全电度
	telectrl.m_nCtrlType = TELECTRL_REQUEST_SYNCTIME;
	IsOK = m_pComm104Pln->OnCommand(&telectrl);
	if (!IsOK)
	{
		return;
	}

	//对时请求
	telectrl.m_nCtrlType = TELECTRL_REQUEST_KWHGENERALCALL;
	IsOK = m_pComm104Pln->OnCommand(&telectrl);
	if (!IsOK)
	{
		return;
	}
}

//收到召唤响应报文
// void CApduRecver::OnRecvResponseCatalog(char *pBuff, int nLength)
// {
//     if (nLength == 0)
//     {
//         return;
//     }
// 
//     //发送数据合法性判断
//     if (!m_pComm104Pln->getSender()->GetLbData().m_bValidFlag)
//     {
//         return;
//     }
// 
//     QString strDeviceName = m_pComm104Pln->GetFtpModule()->GetDeviceName();
//     m_pComm104Pln->GetFtpModule()->GetMainModule()->LogString(strDeviceName.toLocal8Bit().data(), tr("Recv Response Catalog").toLocal8Bit().data(), 1);
// 
//     ASDUMLXY* pAsduZgml = (ASDUMLXY*)pBuff;
//     //文件数量
//     int nCount = pAsduZgml->GetItemCount();
// 
//     for (int i = 0; i < nCount; i++)
//     {
//         //当存在需要请求的文件时发送文件选择命令
//         if (pAsduZgml->GetValue(i) == m_pComm104Pln->getSender()->GetLbData().m_nFilenameIndex)
//         {
//             m_pComm104Pln->getSender()->OnSendChooseLbFile();
//         }
//     }
// 
// }

//选择文件
// void CApduRecver::OnRecvChooseFileResponse(char *pBuff, int nLength)
// {
//     if (nLength == 0)
//     {
//         return;
//     }
// 
//     //发送数据合法性判断
//     if (!m_pComm104Pln->getSender()->GetLbData().m_bValidFlag)
//     {
//         return;
//     }
//     QString strDeviceName = m_pComm104Pln->GetFtpModule()->GetDeviceName();
//     m_pComm104Pln->GetFtpModule()->GetMainModule()->LogString(strDeviceName.toLocal8Bit().data(), tr("Recv Choose File Response").toLocal8Bit().data(), 1);
// 
// 
//     ASDUCHRQ* pAsduZgml = (ASDUCHRQ*)pBuff;
// 
//     if (pAsduZgml->m_infoFileIndex.GetAddr() == m_pComm104Pln->getSender()->GetLbData().m_nFilenameIndex)
//     {
//         if (pAsduZgml->m_frq == 0)
//         {
//             //0表示确认
//             //2 表示请求报文
//             m_pComm104Pln->getSender()->OnSendRequestFile(2);
//         }
//     }
// 
// }


//请求文件准备就绪
// void CApduRecver::OnRectRequestFileReady(char *pBuff, int nLength)
// {
//     if (nLength == 0)
//     {
//         return;
//     }
// 
//     //发送数据合法性判断
//     if (!m_pComm104Pln->getSender()->GetLbData().m_bValidFlag)
//     {
//         return;
//     }
// 
// 
//     ASDUQQWJ* pAsduZgml = (ASDUQQWJ*)pBuff;
// 
//     if (pAsduZgml->m_infoFileIndex.GetAddr() == m_pComm104Pln->getSender()->GetLbData().m_nFilenameIndex)
//     {
//         if (pAsduZgml->m_frq == 0)
//         {
//             //0表示确认
//             //记录结长度
//             m_nNodeLength = pAsduZgml->m_FileLeg.GetAddr();
//             //请求节
//             if (m_nNodeLength > 0)
//             {
//                 //6表示请求节
//                 m_pComm104Pln->getSender()->OnSendRequestFile(6);
//                 m_byNodeData.clear();
//             }
//         }
//     }
// }

//节数据传输
// void CApduRecver::OnRecvNodeData(char *pBuff, int nLength)
// {
//     if (nLength == 0)
//     {
//         return;
//     }
// 
//     //发送数据合法性判断
//     if (!m_pComm104Pln->getSender()->GetLbData().m_bValidFlag)
//     {
//         return;
//     }
// 
//     ASDUFILENODE* pAsduZgml = (ASDUFILENODE*)pBuff;
// 
//     if (pAsduZgml->m_infoFileIndex.GetAddr() == m_pComm104Pln->getSender()->GetLbData().m_nFilenameIndex)
//     {
//         //节数据还没发完
//         if (m_nNodeLength > 0)
//         {
//             //
//             m_byNodeData.append(pAsduZgml->m_data, pAsduZgml->m_NodeLegth);
//         }
// 
//         m_nNodeLength--;
//     }
// }

//一节最后段传输
// void CApduRecver::OnRecvLastNodePart(char *pBuff, int nLength)
// {
//     if (nLength == 0)
//     {
//         return;
//     }
// 
//     //发送数据合法性判断
//     if (!m_pComm104Pln->getSender()->GetLbData().m_bValidFlag)
//     {
//         return;
//     }
// 
//     ASDULASTNODE* pAsduZgml = (ASDULASTNODE*)pBuff;
// 
//     if (pAsduZgml->m_infoFileIndex.GetAddr() == m_pComm104Pln->getSender()->GetLbData().m_nFilenameIndex)
//     {
//         //最后的节
//         if (pAsduZgml->m_reserve == 1)
//         {
//             //整个文件的和校验
//             m_AbyFileData.append(m_byNodeData);
// 
//             if (pAsduZgml->m_frq == checkAllData((unsigned char *)m_AbyFileData.data(), m_AbyFileData.length()))
//             {
//                 //和校验通过  最后的文件
//                 //下发下一个文件
//                 //发送认可节
//                 m_pComm104Pln->getSender()->OnSendFileConformNode();
// 
//                 if (m_pComm104Pln->getSender()->GetLbAllData().count() > 0)
//                 {
//                     //保存文件
//                     m_pFileLoad->CreateNewFile(m_pComm104Pln->getSender()->GetLbData().m_strFileName, m_AbyFileData);
//                     m_AbyFileData.clear();
//                     //end
//                     QString strDeviceName = m_pComm104Pln->GetFtpModule()->GetDeviceName();
//                     QString strLog = tr("No") + m_pComm104Pln->getSender()->GetLbData().m_nFilenameIndex + tr(" File Recv Success!");
//                     m_pComm104Pln->GetFtpModule()->GetMainModule()->LogString(strDeviceName.toLocal8Bit().data(), tr("Recv Choose File Response").toLocal8Bit().data(), 1);
// 
//                     //当成功后
//                     m_pComm104Pln->getSender()->OnSendRecordCatalog(m_pComm104Pln->getSender()->GetLbAllData().at(0));
//                 }
//             }
//             else
//             {
//                 //和校验不通过
//             }
//         }
//         else
//         {
//             //不是最后的节
//             //和校验
//             if (pAsduZgml->m_frq == checkAllData((unsigned char *)m_byNodeData.data(), m_byNodeData.length()))
//             {
//                 m_AbyFileData.append(m_byNodeData);
//             }
//             else
//             {
//                 //和校验失败
//             }
//         }
// 
//     }
// }

unsigned char CApduRecver::checkAllData(const unsigned char *data, int length)
{
    unsigned char lValue = 0;
    for (int i = 0; i < length; i++)
    {
        lValue += data[i];
    }
    return lValue;
}

CApduRecver::~CApduRecver()
{
	if (!m_LoadThread.wait(1000)) 
	{
		m_LoadThread.terminate(); 
		m_LoadThread.wait(); 
	}
    delete m_pFileLoad;
}


void CApduRecver::OnRecvResetProcess(char *pBuff, int nLength)
{
    //记录

    if (nLength <= 0)
    {
        return;
    }

    ASDU105* pAsdu105 = (ASDU105*)pBuff;

    if (pAsdu105->cot.GetCot() == 7)
    {
        QByteArray byDestr = tr("Receive Reset Process Response Scuss").toLocal8Bit();

        m_pComm104Pln->GetFtpModule()->GetMainModule()->LogString(m_pComm104Pln->GetFtpModule()->GetDeviceName().toStdString().c_str(), byDestr.data(), 1);

    }
}

//切换定值区
void CApduRecver::OnRecvSwitchFixArea(char *pBuff, int nLength)
{
	if (nLength <= 0)
	{
		return;
	}

	ASDU200* pAsdu200 = (ASDU200*)pBuff;

	if (pAsdu200->cot.GetCot() == 7)
	{
		QByteArray byDestr = tr("Receive Switch Fix Area Response Scuss").toLocal8Bit();

		m_pComm104Pln->GetFtpModule()->GetMainModule()->LogString(m_pComm104Pln->GetFtpModule()->GetDeviceName().toStdString().c_str(), byDestr.data(), 1);
	}
}


void CApduRecver::OnrecvReadCurrentFixArea(char *pBuff, int nLength)
{
	//获取当前定值区号
	if (nLength <= 0)
	{
		return;
	}

	ASDU201_M* pAsdu201 = (ASDU201_M*)pBuff;

	if (pAsdu201->cot.GetCot() == 7)
	{
		QByteArray byDestr = tr("Current Fix Area Code is: %1").arg(pAsdu201->m_infoCurrentAreaIndex.GetAddr()).toLocal8Bit();

		m_pComm104Pln->GetFtpModule()->GetMainModule()->LogString(m_pComm104Pln->GetFtpModule()->GetDeviceName().toStdString().c_str(), byDestr.data(), 1);
	}
}

void CApduRecver::OnRecvFileAnalyseInfo(char *pBuff, int nLength)
{
	if (nLength <= 0)
	{
		return;
	}

	FILE_BASE_HEADER *pFileHeader = (FILE_BASE_HEADER*)pBuff;

	switch (pFileHeader->m_nOperatorType)
	{
	case E_FILE_CATALOG_RESPOND:
	{
		//目录召唤确认
		OnRecvCatalogRespond(pBuff,nLength);
		break;
	}
	case FILE_READ_ACTION_ACT:
	{
		//读取激活确认
		OnRecvFileAction(pBuff,nLength);
		break;
	}
	case FILE_READ_TRANSPORT:
	{
		//读文件数据传输
		OnRecvReadFileRespond(pBuff, nLength);
		break;
	}
	case FILE_READ_TRANSPORT_ACT:
	{
		//读取传输确认
		OnRecvReadConform(pBuff, nLength);
		break;
	}
	case FILE_WRITE_ACTION_ACT:
	{
		//写入激活确认
		OnRecvWriteAction(pBuff, nLength);
		break;
	}
	case FILE_WRITE_TRANSPORT:
	{
		//写入数据
		break;
	}
	case FILE_WRITE_TRANSPORT_ACT:
	{
		// 写入传输确认
		break;
	}
	default:
		break;
	}
}

//目录召唤响应
void CApduRecver::OnRecvCatalogRespond(char *pBuff, int nLength)
{
	if (nLength <= 0)
	{
		return;
	}

	FILE_CATALOG_RESPOND *pCatalogRespond = (FILE_CATALOG_RESPOND*)pBuff;

	if (pCatalogRespond->m_nResultFlag == 1)
	{
		//失败
		QByteArray byDestr = tr("Dir Respond Reject").toLocal8Bit();
		m_pComm104Pln->GetFtpModule()->GetMainModule()->LogString(m_pComm104Pln->GetFtpModule()->GetDeviceName().toStdString().c_str(), byDestr.data(), 1);
		m_pComm104Pln->getSender()->ClearFilepath();
		
		return;
	}

	if (m_fileAttrInfo.m_nOperatorType == 9)
	{
		//只招目录
		return;
	}
	else if (m_fileAttrInfo.m_nOperatorType == 8)
	{

		//报文长度
		int nBagLength = pCatalogRespond->GetLength();
		//现有文件长度
		int nFileLengths = 0;


		for (int i = 0; i < pCatalogRespond->m_uFileCount; i++)
		{
			//文件名称长度
			int nFileNameLength = pBuff[nBagLength + nFileLengths];   //
																	  //文件名称
			QString strFilename = QString::fromLocal8Bit(pBuff + nBagLength + 1 + nFileLengths, nFileNameLength);
			//文件属性
			unsigned char uFileAttr = pBuff[nBagLength + 1 + nFileNameLength + nFileLengths];
			//文件大小
			INFOADDR4 *infoSize = (INFOADDR4*)(pBuff + nBagLength + 2 + nFileNameLength + nFileLengths);

			nFileLengths += 2 + nFileNameLength + sizeof(INFOADDR4) + 7;

			int nFileSize = infoSize->GetAddr();


			Catalog_Info tFileAttr;

			tFileAttr.m_cFileNameLength = nFileNameLength;
			tFileAttr.m_strFileName = strFilename;
			tFileAttr.m_FileSize = nFileSize;
			tFileAttr.m_cFileAttr = uFileAttr;

			m_lstFiles.append(tFileAttr);

		}

		if (pCatalogRespond->m_uNextFlag == 0)
		{
			//开始读取文件 
			if (m_lstFiles.count() != 0)
			{
				emit Signal_FIleCatalogINfo(m_lstFiles);
			}
		}



		return;
	}

	//报文长度
	int nBagLength = pCatalogRespond->GetLength();
	//现有文件长度
	int nFileLengths = 0;


	for (int i=0; i<pCatalogRespond->m_uFileCount; i++)
	{
		//文件名称长度
		int nFileNameLength = pBuff[nBagLength + nFileLengths];   //
		//文件名称
		QString strFilename = QString::fromLocal8Bit(pBuff + nBagLength + 1 + nFileLengths, nFileNameLength);
		//文件属性
		unsigned char uFileAttr = pBuff[nBagLength+1+nFileNameLength + nFileLengths];
		//文件大小
		INFOADDR4 *infoSize = (INFOADDR4*)(pBuff+nBagLength+2+nFileNameLength + nFileLengths);

		nFileLengths += 2 + nFileNameLength + sizeof(INFOADDR4) + 7;

		int nFileSize = infoSize->GetAddr();

		if (uFileAttr == 1)
		{
			//目录不做处理
			continue;
		}

		if (strFilename.contains(m_fileAttrInfo.m_strFileName))
		{
			FILE_ATTR_INFO tFileAttr;

			tFileAttr.m_cFileNameLength = nFileNameLength;
			tFileAttr.m_strFileName = strFilename;
			tFileAttr.m_FileSize = nFileSize;
			tFileAttr.m_cFileAttr = uFileAttr;
			tFileAttr.type = D_FILE_TRANSPORT;

			m_arrCatalogRespond.append(tFileAttr);

		}

	}

	if (pCatalogRespond->m_uNextFlag == 0)
	{
		//无后续
		//开始读取文件 
		if (m_arrCatalogRespond.count() != 0)
		{
			m_pComm104Pln->getSender()->OnSendReadFileAction(m_arrCatalogRespond.first());
			//m_arrCatalogRespond.removeFirst();

			if (m_arrCatalogRespond.isEmpty())
			{
				//文件下载结束
				m_pComm104Pln->getSender()->ClearFilepath();
			}
		}
	}


}

//读文件激活确认
void CApduRecver::OnRecvFileAction(char *pBuff, int nLength)
{
	if (nLength <= 0)
	{
		return;
	}

	FILE_CATALOG_RESPOND *pActionRespond = (FILE_CATALOG_RESPOND*)pBuff;

	if (pActionRespond->m_nResultFlag == 1)
	{
		//失败
		QByteArray byDestr = tr("Action ").toLocal8Bit();
		m_pComm104Pln->GetFtpModule()->GetMainModule()->LogString(m_pComm104Pln->GetFtpModule()->GetDeviceName().toStdString().c_str(), byDestr.data(), 1);
		m_pComm104Pln->getSender()->ClearFilepath();

		return;
	}

}

//读取文件数据
void CApduRecver::OnRecvFileData(char *pBuff, int nLength)
{
	if (nLength <= 0)
	{
		return;
	}


}

//读文件传输确认
void CApduRecver::OnRecvFileConform(char *pBuff, int nLength)
{

}

//故障事件
void CApduRecver::OnRecvDefalutMsg(char *pBuff, int nLength)
{
    //
    if (nLength <= 0)
    {
        return;
    }

    ASDUGZ* pAsduGZ = (ASDUGZ*)pBuff;


	emit Signal_MalFuction(*pAsduGZ);
	if (pAsduGZ->m_AnalogType == 1)
	{
		//short
		
	}
	else
	{
		//float

	}
    
}
