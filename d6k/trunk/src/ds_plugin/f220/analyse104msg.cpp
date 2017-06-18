#include <QXmlStreamReader>
#include <QMessageBox>
#include <QFile>
#include <QDebug>
#include <QApplication>
#include <QTreeWidget>
#include "analyse104msg.h"
#include "define_104.h"
#include "devicestudio/main_module.h"
#include "../../../src/devicestudio/capturemsgwgt.h"

CAnalyse104Msg::CAnalyse104Msg(IMainModule *pCore ,QObject *parent)
    : QObject(parent)
{
    Q_ASSERT(pCore);

    if (pCore == nullptr)
    {
        return;
    }

    m_pICore = pCore;

    m_pTreeWgt = pCore->GetDetailMsgWgt();

    QString strFile = qApp->applicationDirPath() + CAUTUREIECDESC;

    InitDescInfo(strFile);
}

CAnalyse104Msg::~CAnalyse104Msg()
{

}

void CAnalyse104Msg::AnalyseApdu(char *pBuff, int nLength)
{
    if ((pBuff == NULL) || (nLength < sizeof(APCI)) || (pBuff[0] != STARTCHAR))
    {
        return;
    }

    m_pTopTreeItem = new QTreeWidgetItem;
    m_pTopTreeItem->setText(0, tr("Detail Info"));
    m_pTreeWgt->addTopLevelItem(m_pTopTreeItem);
    m_pTopTreeItem->setExpanded(true);

    InsertNodeInfo("68", tr("Start character,indicate new message begin"));

    APCI* pApci = (APCI*)(pBuff);

    //记录信息
    int nMessageLength = pApci->len;
    char cLength[2] = {0};
    cLength[0]= pBuff[1];

    InsertNodeInfo(QString(QByteArray(cLength,1).toHex().toUpper()), tr("Infomation Length:%1,Message Length:%2").arg(QString::number(nMessageLength)).arg(QString::number(nMessageLength + 2)));

    int nType = pApci->GetType();

    if ((nType != TYPE_I) && (nType != TYPE_U) && (nType != TYPE_S))
    {
        return;
    }

    char cType[3] = { 0 };
    cType[0] = pBuff[2];
    cType[1] = pBuff[3];

    char cType2[3] = { 0 };
    cType2[0] = pBuff[4];
    cType2[1] = pBuff[5];

    switch (nType)
    {
    case TYPE_I:
    {
        InsertNodeInfo(ChangeOxType(QByteArray(cType, 2).toHex().toUpper()), tr("Control 1 and 2, means I Type, Send Serial Number:%1").arg(QString::number(pApci->GetSendNum())));
        InsertNodeInfo(ChangeOxType(QByteArray(cType2, 2).toHex().toUpper()), tr("Control 3 and 4, means Recv Serial Number:%1").arg(QString::number(pApci->GetRecvNum())));

        AnalyseI(pBuff, nLength);
        break;
    }
    case TYPE_U:
    {
        AnalyseU(pBuff, nLength);
        break;
    }
    case TYPE_S:
    {
        InsertNodeInfo(ChangeOxType(QByteArray(cType, 2).toHex().toUpper()), tr("Control 1 and 2, means U Type"));
        InsertNodeInfo(ChangeOxType(QByteArray(cType2, 2).toHex().toUpper()), tr("Control 3 and 4, means Recv Serial Number:%1").arg(QString::number(pApci->GetRecvNum())));
        break;
    }
    default:
        break;
    }
}

void CAnalyse104Msg::AnalyseI(char *pBuff, int nLength)
{
    ASDU_BASE* pAsduBase = (ASDU_BASE*)(pBuff + sizeof(APCI));

    pBuff = pBuff + sizeof(APCI);
    nLength = nLength - sizeof(APCI);

    char cTmpData[2] = { 0 };
    cTmpData[0] = pBuff[0];

    //传送类型
    InsertNodeInfo(QString(QByteArray(cTmpData,1).toHex().toUpper()), tr("TypeCode:<%1>  %2").arg(QString::number(pAsduBase->type)).arg(m_mapCodeDesc[pAsduBase->type]));
    //可变结构限定词
    cTmpData[0] = pBuff[1];
    QString strSeq = pAsduBase->IsSQ1() ? tr("Continuous information object") : tr("Discontinuous information object");

    InsertNodeInfo(QString(QByteArray(cTmpData,1).toHex().toUpper()), tr("Variable structure qualifier:%1, number is: %2").arg(strSeq).arg(pAsduBase->vsq & 0x7f));
    cTmpData[0] = pBuff[2];
    cTmpData[1] = pBuff[3];
    InsertNodeInfo(ChangeOxType(QByteArray(cTmpData,2).toHex().toUpper()), tr("Transfer reason:%1").arg(m_mapReason[pAsduBase->cot.GetCot()]));

    char cAddr[3] = { 0 };
    cAddr[0] = pBuff[4];
    cAddr[1] = pBuff[5];

    InsertNodeInfo(ChangeOxType(QByteArray(cAddr,2).toHex().toUpper()), tr("Public Address:%1").arg(pAsduBase->asduAddr.GetAddr()));

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
    case M_ME_NB_1://标度化测量值 ASDU11
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
//     //累计量   遥脉
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
        OnRecvGeneralCall(pBuff, nLength);
        break;
    }
    case C_CI_NA_1: //电度   Asdu 101
    {
        OnRecvKwhGeneralCall(pBuff, nLength);
        break;
    }
    case C_CS_NA_1:  //时钟
    {
        OnRecvSyncTime(pBuff, nLength);
        break;
    }
    case C_SC_NA_1://遥控(单点命令) ASDU45
    {
        OnControlCommand(pBuff, nLength);
        break;
    }
    case C_DC_NA_1://遥控(双点命令) ASDU46
    {
        OnControlDoubleCommand(pBuff, nLength);
        break;
    }
    case C_RD_NA_1:// 定值获取 单参
    case D_DV_RD_1://多参
    {
        OnRecvDevReadCommand(pBuff, nLength);
        break;
    }   
    case  C_SE_NA_1:
    case D_DV_WR_1:
    {
        OnRecvDevWriteCommand(pBuff, nLength);
        break;
    }
//     case F_DR_TA_1:
//     {
//         //召唤目录响应
//         OnRecvResponseCatalog(pBuff, nLength);
//         break;
//     }
//     case F_FR_NA_1:
//     {
//         //选择文件
//         OnRecvChooseFileResponse(pBuff, nLength);
//         break;
//     }
//     case F_SR_NA_1:
//     {
//         //请求文件准备就绪
//         OnRectRequestFileReady(pBuff, nLength);
//         break;
//     }
//     case F_SG_NA_1:
//     {
//         //节传输
//         OnRecvNodeData(pBuff, nLength);
//         break;
//     }
//     case F_LS_NA_1:
//     {
//         //节最后的段
//         OnRecvLastNodePart(pBuff, nLength);
//         break;
//     }
//     case F_AF_NA_1:
//     {
//         //节传输确认
//         //空
//         break;
//     }

    default:
        break;
    }
}

void CAnalyse104Msg::AnalyseU(char *pBuff, int nLength)
{
    Q_UNUSED(nLength);
    APCI* pApci = (APCI*)pBuff;

    char cType[3] = { 0 };
    cType[0] = pBuff[2];
    cType[1] = pBuff[3];

    char cType2[3] = { 0 };
    cType2[0] = pBuff[4];
    cType2[1] = pBuff[5];

    //1、解析
    int nUType = pApci->GetUType();

    switch (nUType)
    {
    case TESTFR_CON:
    {
        InsertNodeInfo(ChangeOxType(QByteArray(cType, 2).toHex().toUpper()), tr("Control 1 and 2, means: TESTFR_CON "));
        InsertNodeInfo(ChangeOxType(QByteArray(cType2, 2).toHex().toUpper()), tr(""));
        break;
    }
    case TESTFR_ACT:
    {
        InsertNodeInfo(ChangeOxType(QByteArray(cType, 2).toHex().toUpper()), tr("Control 1 and 2, means: TESTFR_ACT "));
        InsertNodeInfo(ChangeOxType(QByteArray(cType2, 2).toHex().toUpper()), tr(""));
        break;
    }
    case STARTDT_CON:
    {
        InsertNodeInfo(ChangeOxType(QByteArray(cType, 2).toHex().toUpper()), tr("Control 1 and 2, means: STARTDT_CON "));
        InsertNodeInfo(ChangeOxType(QByteArray(cType2, 2).toHex().toUpper()), tr(""));
        break;
    }
    case STARTDT_ACT:
    {
        InsertNodeInfo(ChangeOxType(QByteArray(cType, 2).toHex().toUpper()), tr("Control 1 and 2, means: STARTDT_ACT "));
        InsertNodeInfo(ChangeOxType(QByteArray(cType2, 2).toHex().toUpper()), tr(""));
        break;
    }
    case STOPDT_CON://收到STOPDT确认
    {
        InsertNodeInfo(ChangeOxType(QByteArray(cType, 2).toHex().toUpper()), tr("Control 1 and 2, means: STOPDT_CON "));
        InsertNodeInfo(ChangeOxType(QByteArray(cType2, 2).toHex().toUpper()), tr(""));
        break;
    }
    case STOPDT_ACT://收到STOPDT激活
    {
        InsertNodeInfo(ChangeOxType(QByteArray(cType, 2).toHex().toUpper()), tr("Control 1 and 2, means: STOPDT_ACT "));
        InsertNodeInfo(ChangeOxType(QByteArray(cType2, 2).toHex().toUpper()), tr(""));
        break;
    }
    default:
        break;
    }
}

void CAnalyse104Msg::InitDescInfo(const QString &strFilename)
{
    if (strFilename.isEmpty())
    {
        QMessageBox::warning(0, tr("Warning"), tr("read Config file is null,please check!"));
        return;
    }

    QFile fIecDesc(strFilename);
    if (fIecDesc.open(QFile::ReadOnly | QFile::Text))
    {
        AnalyseXmlData(&fIecDesc);
        fIecDesc.close();
    }
    else
    {
        QMessageBox::warning(0, tr("warning"), tr("open file %1 error!").arg(strFilename));
        return;
    }
}

void CAnalyse104Msg::SetTreeWgt(QTreeWidget *pTreeWgt)
{
    Q_ASSERT(pTreeWgt);

    if (pTreeWgt == nullptr)
    {
        return;
    }
}

void CAnalyse104Msg::AnalyseXmlData(QIODevice *pDevice)
{
    QXmlStreamReader xmlReader(pDevice);

    while (!xmlReader.atEnd() && (!xmlReader.hasError()))
    {
        //遍历所有配置
        xmlReader.readNext();
        if (xmlReader.isStartElement())
        {
            if (xmlReader.name().toString() == "IecProtocol")
            {
                //解析数据 写入表格中
                QString strNum = xmlReader.attributes().value("IecType").toString();
                if (strNum == "104")
                {
                    Analyse104NodeInfo(xmlReader);
                }
            }
        }
    }
    //qDebug() << "!!!!!!!";
}

void CAnalyse104Msg::Analyse104NodeInfo(QXmlStreamReader &xmlReader)
{
    while (!xmlReader.atEnd() && (!xmlReader.hasError()))
    {
        xmlReader.readNext();

        if (xmlReader.isStartElement())
        {
            if (xmlReader.name().toString() == "ResInfo")
            {
                int nResKey = xmlReader.attributes().value("ResKey").toInt();
                QString strDesc = xmlReader.attributes().value("ResDesc").toString();
                m_mapReason.insert(nResKey, strDesc);
            }
            else if (xmlReader.name().toString() == "CodeItem")
            {
                int nCodeKey = xmlReader.attributes().value("CodeKey").toInt();
                QString strDesc = xmlReader.attributes().value("CodeDesc").toString();
                m_mapCodeDesc.insert(nCodeKey,strDesc);
            }
        }

        if (xmlReader.isEndElement())
        {
            if (xmlReader.name().toString() == "IecProtocol")
            {
                break;
            }
        }
    }
}

void CAnalyse104Msg::Slot_ClickOneCaptureInfo(QByteArray &byData)
{
    //解析数据
    AnalyseApdu(byData.data(),byData.length());
}

void CAnalyse104Msg::InsertNodeInfo(QString strCode, QString strDesc)
{
    QTreeWidgetItem *pDescItem = new QTreeWidgetItem;
    pDescItem->setText(0, strCode);
    pDescItem->setText(1, strDesc);

    m_pTopTreeItem->addChild(pDescItem);
}

void CAnalyse104Msg::OnRecvBinarySinglePoint(char* pBuff, int nLength)
{
    if (nLength == 0)
    {
        return;
    }

    //qDebug() << QByteArray(pBuff, nLength).toHex();

    ASDU_BASE* pBase = (ASDU_BASE*)pBuff;

    if (pBase->IsSQ1())
    {
        //连续单点遥信
        ASDU1_SQ1* pAsdu1SQ1 = (ASDU1_SQ1*)pBuff;

        int nCount = pAsdu1SQ1->GetItemCount();

        int nPointID = pAsdu1SQ1->infoaddr.GetAddr();

        char cAddr[4] = { 0 };
        cAddr[0] = pBuff[6];
        cAddr[1] = pBuff[7];
        cAddr[2] = pBuff[8];


        InsertNodeInfo(ChangeOxType(QByteArray(cAddr,3).toHex().toUpper()),tr("Infomation Address:%1").arg(QString::number(nPointID)));

        for (int i = 0; i < nCount; i++)
        {
            char cValue[2] = { 0 };
            cValue[0] = pBuff[9 + i];

            InsertNodeInfo(QString(QByteArray(cValue, 1).toHex().toUpper()), tr("SerialNumber:%1 IV:%7 NT:%6 SB:%5 BL:%4 RES:%3 SPI:%2").arg(nPointID)
                  .arg(pAsdu1SQ1->m_data[i].SPI).arg(pAsdu1SQ1->m_data[i].RES).arg(pAsdu1SQ1->m_data[i].BL)
                  .arg(pAsdu1SQ1->m_data[i].SB).arg(pAsdu1SQ1->m_data[i].NT).arg(pAsdu1SQ1->m_data[i].IV));

            nPointID++;
        }
    }
    else
    {
        ASDU1_SQ0* pAsdu1SQ0 = (ASDU1_SQ0*)pBuff;

        int nCount = pAsdu1SQ0->GetItemCount();

        for (int i = 0; i < nCount; i++)
        {
            int nPointID = pAsdu1SQ0->m_data[i].infoaddr.GetAddr();

            //int nValue = pAsdu1SQ0->GetValue(i);


            char cAddrs[4] = { 0 };
            cAddrs[0] = pBuff[6 + i*4];
            cAddrs[1] = pBuff[7 + i * 4];
            cAddrs[3] = pBuff[8 + i * 4];
            
            char cValue[2] = { 0 };
            cValue[0] = pBuff[8 + i * 4];

            InsertNodeInfo(ChangeOxType(QByteArray(cAddrs, 3).toHex().toUpper()), tr("SerialNumber:%1").arg(nPointID));

            InsertNodeInfo(ChangeOxType(QByteArray(cValue, 1).toHex().toUpper()), tr("IV:%6 NT:%5 SB:%4 BL:%3 RES:%2 SPI:%1")
                .arg(pAsdu1SQ0->m_data[i].siq.SPI).arg(pAsdu1SQ0->m_data[i].siq.RES).arg(pAsdu1SQ0->m_data[i].siq.BL)
                .arg(pAsdu1SQ0->m_data[i].siq.SB).arg(pAsdu1SQ0->m_data[i].siq.NT).arg(pAsdu1SQ0->m_data[i].siq.IV));
        }
    }
}

void CAnalyse104Msg::OnRecvBinaryDoublePoint(char* pBuff, int nLength)
{
    if (nLength == 0)
    {
        return;
    }

    //qDebug() << QByteArray(pBuff, nLength).toHex();

    ASDU_BASE* pBase = (ASDU_BASE*)pBuff;

    if (pBase->IsSQ1())
    {
        //连续单点遥信
        ASDU3_SQ1* pAsdu1SQ1 = (ASDU3_SQ1*)pBuff;

        int nCount = pAsdu1SQ1->GetItemCount();

        int nPointID = pAsdu1SQ1->infoaddr.GetAddr();

        char cAddr[4] = { 0 };
        cAddr[0] = pBuff[6];
        cAddr[1] = pBuff[7];
        cAddr[2] = pBuff[8];

        InsertNodeInfo(ChangeOxType(QByteArray(cAddr, 3).toHex().toUpper()), tr("Infomation Address:%1").arg(QString::number(nPointID)));

        for (int i = 0; i < nCount; i++)
        {
            char cValue[2] = { 0 };
            cValue[0] = pBuff[9 + i];

            InsertNodeInfo(QString(QByteArray(cValue, 1).toHex().toUpper()), tr("SerialNumber:%1 IV:%7 NT:%6 SB:%5 BL:%4  RES:%3 DPI:%2").arg(nPointID)
                .arg(pAsdu1SQ1->m_data[i].DPI).arg(pAsdu1SQ1->m_data[i].RES).arg(pAsdu1SQ1->m_data[i].BL)
                .arg(pAsdu1SQ1->m_data[i].SB).arg(pAsdu1SQ1->m_data[i].NT).arg(pAsdu1SQ1->m_data[i].IV));

            nPointID++;
        }
    }
    else
    {
        ASDU3_SQ0* pAsdu1SQ0 = (ASDU3_SQ0*)pBuff;

        int nCount = pAsdu1SQ0->GetItemCount();

        for (int i = 0; i < nCount; i++)
        {
            int nPointID = pAsdu1SQ0->m_data[i].infoaddr.GetAddr();

            //int nValue = pAsdu1SQ0->GetValue(i);


            char cAddrs[4] = { 0 };
            cAddrs[0] = pBuff[6 + i * 4];
            cAddrs[1] = pBuff[7 + i * 4];
            cAddrs[2] = pBuff[8 + i * 4];

            char cValue[2] = { 0 };
            cValue[0] = pBuff[8 + i * 4];

            InsertNodeInfo(ChangeOxType(QByteArray(cAddrs, 3).toHex().toUpper()), tr("SerialNumber:%1").arg(nPointID));

            InsertNodeInfo(QString(QByteArray(cValue, 1).toHex().toUpper()), tr("IV:%6 NT:%5 SB:%4 BL:%3  RES:%2 DPI:%1")
                .arg(pAsdu1SQ0->m_data[i].diq.DPI).arg(pAsdu1SQ0->m_data[i].diq.RES).arg(pAsdu1SQ0->m_data[i].diq.BL)
                .arg(pAsdu1SQ0->m_data[i].diq.SB).arg(pAsdu1SQ0->m_data[i].diq.NT).arg(pAsdu1SQ0->m_data[i].diq.IV));
        }
    }
}

void CAnalyse104Msg::OnRecvBinaryGroupPoint(char* pBuff, int nLength)
{
    if (nLength <= 0)
    {
        return;
    }

    ASDU_BASE* pBase = (ASDU_BASE*)pBuff;
    if (pBase->IsSQ1())
    {
        ASDU20_SQ1* pAsdu20SQ1 = (ASDU20_SQ1*)pBuff;

        int nCount = pAsdu20SQ1->GetItemCount();

        int nPointID = pAsdu20SQ1->m_infoaddr.GetAddr();

        char cAddr[4] = { 0 };
        cAddr[0] = pBuff[6];
        cAddr[1] = pBuff[7];
        cAddr[2] = pBuff[8];

        InsertNodeInfo(QString(QByteArray(cAddr, 3).toHex()), tr("Infomation Address:%1").arg(QString::number(nPointID)));

        for (int i = 0; i < nCount; i++)
        {
            nPointID += 16 * i;

            int nValue = pAsdu20SQ1->GetValue(i);

            for (int k = 0; k < 16; k++)
            {
                int nBitValue = (nValue & (1 << k)) >> k;

                char cValue[5] = { 0 };
                if (k == 0)
                {
                    cValue[0] = pBuff[9 + i * 5];
                    cValue[1] = pBuff[10 + i * 5];
                    cValue[2] = pBuff[11 + i * 5];
                    cValue[3] = pBuff[12 + i * 5];
                    cValue[4] = pBuff[13 + i * 5];
                    InsertNodeInfo(QString(QByteArray(cValue, 5).toHex().toUpper()), tr("SerialNumber:%1  SCD:%8  IV:%7 NT:%6 SB:%5 BL:%4  RES:%3 OV:%2  BitValue:%9  ").arg(nPointID + k)
                        .arg(pAsdu20SQ1->m_data[i].m_qds.OV).arg(pAsdu20SQ1->m_data[i].m_qds.RES).arg(pAsdu20SQ1->m_data[i].m_qds.BL)
                        .arg(pAsdu20SQ1->m_data[i].m_qds.SB).arg(pAsdu20SQ1->m_data[i].m_qds.NT).arg(pAsdu20SQ1->m_data[i].m_qds.IV).arg(pAsdu20SQ1->m_data[i].m_nSCD).arg(nBitValue));

                }
                else
                {
                    InsertNodeInfo("", tr("SerialNumber:%1  SCD:%8  IV:%7 NT:%6 SB:%5 BL:%4  RES:%3 OV:%2  BitValue:%9  ").arg(nPointID + k)
                        .arg(pAsdu20SQ1->m_data[i].m_qds.OV).arg(pAsdu20SQ1->m_data[i].m_qds.RES).arg(pAsdu20SQ1->m_data[i].m_qds.BL)
                        .arg(pAsdu20SQ1->m_data[i].m_qds.SB).arg(pAsdu20SQ1->m_data[i].m_qds.NT).arg(pAsdu20SQ1->m_data[i].m_qds.IV).arg(pAsdu20SQ1->m_data[i].m_nSCD).arg(nBitValue));

                }
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

            int nValue = pAsdu20SQ0->GetValue(i);//

            for (int k = 0; k < 16; k++)
            {
                int nBitValue = (nValue & (1 << k)) >> k;

                char cValue[8] = { 0 };
                
                if (k == 0)
                {
                    cValue[0] = pBuff[6 + i * 8];
                    cValue[1] = pBuff[7 + i * 8];
                    cValue[2] = pBuff[8 + i * 8];
                    cValue[3] = pBuff[9 + i * 8];
                    cValue[4] = pBuff[10 + i * 8];
                    cValue[5] = pBuff[11 + i * 8];
                    cValue[6] = pBuff[12 + i * 8];
                    cValue[7] = pBuff[13 + i * 8];

                    InsertNodeInfo(QString(QByteArray(cValue, 8).toHex().toUpper()), tr("SerialNumber:%1  SCD:%8  IV:%7 NT:%6 SB:%5 BL:%4  RES:%3 OV:%2  BitValue:%9 ").arg(nPointID + k)
                        .arg(pAsdu20SQ0->m_data[i].m_qds.OV).arg(pAsdu20SQ0->m_data[i].m_qds.RES).arg(pAsdu20SQ0->m_data[i].m_qds.BL)
                        .arg(pAsdu20SQ0->m_data[i].m_qds.SB).arg(pAsdu20SQ0->m_data[i].m_qds.NT).arg(pAsdu20SQ0->m_data[i].m_qds.IV).arg(pAsdu20SQ0->m_data[i].m_nSCD).arg(nBitValue));

                }
                else
                {
                    InsertNodeInfo("", tr("SerialNumber:%1  SCD:%8  IV:%7 NT:%6 SB:%5 BL:%4  RES:%3 OV:%2  BitValue:%9 ").arg(nPointID + k)
                        .arg(pAsdu20SQ0->m_data[i].m_qds.OV).arg(pAsdu20SQ0->m_data[i].m_qds.RES).arg(pAsdu20SQ0->m_data[i].m_qds.BL)
                        .arg(pAsdu20SQ0->m_data[i].m_qds.SB).arg(pAsdu20SQ0->m_data[i].m_qds.NT).arg(pAsdu20SQ0->m_data[i].m_qds.IV).arg(pAsdu20SQ0->m_data[i].m_nSCD).arg(nBitValue));

                }

            }
        }
    }
}

void CAnalyse104Msg::OnRecvAnalogNormalized(char* pBuff, int nLength)
{
    if (nLength == 0)
    {
        return;
    }

    ASDU_BASE* pAsduBase = (ASDU_BASE*)pBuff;

    if (pAsduBase->IsSQ1())
    {
        ASDU9_SQ1* pAsdu9SQ1 = (ASDU9_SQ1*)pBuff;

        int nCount = pAsdu9SQ1->GetItemCount();

        int nPointID = pAsdu9SQ1->infoaddr.GetAddr();

        char cAddr[4] = { 0 };
        cAddr[0] = pBuff[6];
        cAddr[1] = pBuff[7];
        cAddr[2] = pBuff[8];

        InsertNodeInfo(ChangeOxType(QByteArray(cAddr, 3).toHex().toUpper()), tr("Infomation Address:%1").arg(QString::number(nPointID)));


        for (int i = 0; i < nCount; i++)
        {
            float fValue = pAsdu9SQ1->GetValue(i);

            QDS qds = pAsdu9SQ1->GetQDS(i);

            char cNodeInfo[4] = { 0 };
            cNodeInfo[0] = pBuff[9 + i * 3];
            cNodeInfo[1] = pBuff[10 + i * 3];
            cNodeInfo[2] = pBuff[11 + i * 3];

            InsertNodeInfo(ChangeOxType(QByteArray(cNodeInfo, 3).toHex().toUpper()), tr("SerialNumber:%1 IV:%8  NT:%7 SB:%6    "
                " BL:%5  RES:%4  OV:%3  Value:%2 ").arg(nPointID).arg(fValue).arg(qds.OV).arg(qds.RES)
                .arg(qds.BL).arg(qds.SB).arg(qds.NT).arg(qds.IV));

            nPointID++;
        }
    }
    else
    {
        ASDU9_SQ0* pAsdu9SQ1 = (ASDU9_SQ0*)pBuff;

        int nCount = pAsdu9SQ1->GetItemCount();

        for (int i = 0; i < nCount; i++)
        {
            int nPointID = pAsdu9SQ1->m_data[i].infoaddr.GetAddr();
            float fValue = pAsdu9SQ1->GetValue(i);
            QDS qds = pAsdu9SQ1->GetQDS(i);

            char cNodeInfo[7] = { 0 };
            cNodeInfo[0] = pBuff[6 + i * 6];
            cNodeInfo[1] = pBuff[7 + i * 6];
            cNodeInfo[2] = pBuff[8 + i * 6];
            cNodeInfo[3] = pBuff[9 + i * 6];
            cNodeInfo[4] = pBuff[10 + i * 6];
            cNodeInfo[5] = pBuff[11 + i * 6];

            InsertNodeInfo(ChangeOxType(QByteArray(cNodeInfo, 6).toHex().toUpper()), tr("SerialNumber:%1 IV:%8  NT:%7  SB:%6   "
                " BL:%5  RES:%4  OV:%3  Value:%2").arg(nPointID).arg(fValue).arg(qds.OV).arg(qds.RES)
                .arg(qds.BL).arg(qds.SB).arg(qds.NT).arg(qds.IV));

        }
        
    }
}

void CAnalyse104Msg::OnRecvKwh(char* pBuff, int nLength)
{
    if (nLength <= 0)
    {
        return;
    }

    ASDU_BASE* pBase = (ASDU_BASE*)pBuff;
    if (pBase->IsSQ1())
    {
        ASDU15_SQ1* pAsdu15SQ1 = (ASDU15_SQ1*)pBuff;
        int nCount = pAsdu15SQ1->GetItemCount();
        int nPointID = pAsdu15SQ1->infoaddr.GetAddr();

        char cAddr[4] = { 0 };
        cAddr[0] = pBuff[6];
        cAddr[1] = pBuff[7];
        cAddr[2] = pBuff[8];

        InsertNodeInfo(ChangeOxType(QByteArray(cAddr, 3).toHex().toUpper()), tr("Infomation Address:%1").arg(QString::number(nPointID)));

        for (int i = 0; i < nCount; i++)
        {
            int nValue = pAsdu15SQ1->m_data[i].m_nValue;

            char cNodeInfo[6] = { 0 };
            cNodeInfo[0] = pBuff[9 + i * 5];
            cNodeInfo[1] = pBuff[10 + i * 5];
            cNodeInfo[2] = pBuff[11 + i * 5];
            cNodeInfo[3] = pBuff[12 + i * 5];
            cNodeInfo[4] = pBuff[13 + i * 5];

            InsertNodeInfo(ChangeOxType(QByteArray(cNodeInfo, 5).toHex().toUpper()), tr("SerialNumber:%1   Value:%2   DestriberFlag:%3")
                .arg(nPointID).arg(nValue).arg(int(pAsdu15SQ1->m_data[i].m_bcrparam)));


            nPointID++;
        }
    }
    else
    {
        ASDU15_SQ0* pAsdu15SQ0 = (ASDU15_SQ0*)pBuff;
        int nCount = pAsdu15SQ0->GetItemCount();

        for (int i = 0; i < nCount; i++)
        {
            int nValue = pAsdu15SQ0->m_data[i].m_nValue;
            int nPointID = pAsdu15SQ0->m_data[i].m_infoaddr.GetAddr();

            char cNodeInfo[9] = { 0 };
            cNodeInfo[0] = pBuff[6 + i * 8];
            cNodeInfo[1] = pBuff[7 + i * 8];
            cNodeInfo[2] = pBuff[8 + i * 8];
            cNodeInfo[3] = pBuff[9 + i * 8];
            cNodeInfo[4] = pBuff[10 + i * 8];
            cNodeInfo[5] = pBuff[11 + i * 8];
            cNodeInfo[6] = pBuff[12 + i * 8];
            cNodeInfo[7] = pBuff[13 + i * 8];

            InsertNodeInfo(ChangeOxType(QByteArray(cNodeInfo, 8).toHex().toUpper()), tr("SerialNumber:%1  Value:%2  DestriberFlag:%3")
                .arg(nPointID).arg(nValue).arg(int(pAsdu15SQ0->m_data[i].m_bcrparam)));
        }
    }
}

QString CAnalyse104Msg::ChangeOxType(QByteArray &btData)
{
    QString strNewHex;

    for (int i = 0; i < btData.count(); i++)
    {
        if (i % 2 == 0 && i != 0)
        {
            strNewHex.append(" ");
        }

        strNewHex.append(btData.at(i));
    }

    return strNewHex;
}

void CAnalyse104Msg::OnRecvAnalogNormalizedShortTime(char* pBuff, int nLength)
{
    if (nLength <= 0)
    {
        return;
    }

    ASDU10_SQ0* pAsdu10SQ0 = (ASDU10_SQ0*)pBuff;

    int nCount = pAsdu10SQ0->GetItemCount();

    for (int i = 0; i < nCount; i++)
    {
        int nPointID = pAsdu10SQ0->m_data[i].m_infoaddr.GetAddr();

        float fValue = pAsdu10SQ0->GetValue(i);

        QDS qds = pAsdu10SQ0->GetQDS(i);

        char cNodeInfo[10] = { 0 };
        cNodeInfo[0] = pBuff[6 + i * 9];
        cNodeInfo[1] = pBuff[7 + i * 9];
        cNodeInfo[2] = pBuff[8 + i * 9];
        cNodeInfo[3] = pBuff[9 + i * 9];
        cNodeInfo[4] = pBuff[10 + i * 9];
        cNodeInfo[5] = pBuff[11 + i * 9];
        cNodeInfo[6] = pBuff[12 + i * 9];
        cNodeInfo[7] = pBuff[13 + i * 9];
        cNodeInfo[8] = pBuff[14 + i * 9];

        InsertNodeInfo(ChangeOxType(QByteArray(cNodeInfo, 9).toHex().toUpper()), tr("SerialNumber:%1 IV:%8  NT:%7  SB:%6   "
            " BL:%5  RES:%4  OV:%3  Value:%2   Time: %9").arg(nPointID).arg(fValue).arg(qds.OV).arg(qds.RES)
            .arg(qds.BL).arg(qds.SB).arg(qds.NT).arg(qds.IV).arg(pAsdu10SQ0->GetTime(i).Dump()));


    }
}

void CAnalyse104Msg::OnRecvAnalogScaled(char* pBuff, int nLength)
{
    if (nLength <= 0)
    {
        return;
    }

    ASDU_BASE* pAsduBase = (ASDU_BASE*)pBuff;

    if (pAsduBase->IsSQ1())
    {
        ASDU11_SQ1* pAsdu11SQ1 = (ASDU11_SQ1*)pBuff;

        int nCount = pAsdu11SQ1->GetItemCount();

        int nPointID = pAsdu11SQ1->infoaddr.GetAddr();

        for (int i = 0; i < nCount; i++)
        {
            float fValue = pAsdu11SQ1->GetValue(i);

            QDS qds = pAsdu11SQ1->GetQDS(i);

            char cNodeInfo[4] = { 0 };
            cNodeInfo[0] = pBuff[9 + i * 3];
            cNodeInfo[1] = pBuff[10 + i * 3];
            cNodeInfo[2] = pBuff[11 + i * 3];

            InsertNodeInfo(ChangeOxType(QByteArray(cNodeInfo, 3).toHex().toUpper()), tr("SerialNumber:%1 IV:%8  NT:%7 SB:%6    "
                " BL:%5  RES:%4  OV:%3  Value:%2 ").arg(nPointID).arg(fValue).arg(qds.OV).arg(qds.RES)
                .arg(qds.BL).arg(qds.SB).arg(qds.NT).arg(qds.IV));

            nPointID++;
        }
    }
    else
    {
        ASDU11_SQ0* pAsdu11SQ0 = (ASDU11_SQ0*)pBuff;

        int nCount = pAsdu11SQ0->GetItemCount();

        for (int i = 0; i < nCount; i++)
        {
            int nPointID = pAsdu11SQ0->m_data[i].infoaddr.GetAddr();

            float fValue = pAsdu11SQ0->GetValue(i);

            QDS qds = pAsdu11SQ0->GetQDS(i);

            char cNodeInfo[7] = { 0 };
            cNodeInfo[0] = pBuff[6 + i * 6];
            cNodeInfo[1] = pBuff[7 + i * 6];
            cNodeInfo[2] = pBuff[8 + i * 6];
            cNodeInfo[3] = pBuff[9 + i * 6];
            cNodeInfo[4] = pBuff[10 + i * 6];
            cNodeInfo[5] = pBuff[11 + i * 6];

            InsertNodeInfo(ChangeOxType(QByteArray(cNodeInfo, 6).toHex().toUpper()), tr("SerialNumber:%1 IV:%8  NT:%7  SB:%6   "
                " BL:%5  RES:%4  OV:%3  Value:%2").arg(nPointID).arg(fValue).arg(qds.OV).arg(qds.RES)
                .arg(qds.BL).arg(qds.SB).arg(qds.NT).arg(qds.IV));
        }
    }
}

void CAnalyse104Msg::OnRecvAnalogScaledShortTime(char* pBuff, int nLength)
{
    if (nLength <= 0)
    {
        return;
    }

    ASDU12_SQ0* pAsdu12SQ0 = (ASDU12_SQ0*)pBuff;

    int nCount = pAsdu12SQ0->GetItemCount();

    for (int i = 0; i < nCount; i++)
    {
        int nPointID = pAsdu12SQ0->m_data[i].m_infoaddr.GetAddr();

        float fValue = pAsdu12SQ0->GetValue(i);

        QDS qds = pAsdu12SQ0->GetQDS(i);

        char cNodeInfo[10] = { 0 };
        cNodeInfo[0] = pBuff[6 + i * 9];
        cNodeInfo[1] = pBuff[7 + i * 9];
        cNodeInfo[2] = pBuff[8 + i * 9];
        cNodeInfo[3] = pBuff[9 + i * 9];
        cNodeInfo[4] = pBuff[10 + i * 9];
        cNodeInfo[5] = pBuff[11 + i * 9];
        cNodeInfo[6] = pBuff[12 + i * 9];
        cNodeInfo[7] = pBuff[13 + i * 9];
        cNodeInfo[8] = pBuff[14 + i * 9];

        InsertNodeInfo(ChangeOxType(QByteArray(cNodeInfo, 9).toHex().toUpper()), tr("SerialNumber:%1 IV:%8  NT:%7  SB:%6   "
            " BL:%5  RES:%4  OV:%3  Value:%2   Time: %9").arg(nPointID).arg(fValue).arg(qds.OV).arg(qds.RES)
            .arg(qds.BL).arg(qds.SB).arg(qds.NT).arg(qds.IV).arg(pAsdu12SQ0->GetTime(i).Dump()));
    }
}

void CAnalyse104Msg::OnRecvAnalogShortFloat(char* pBuff, int nLength)
{
    if (nLength == 0)
    {
        return;
    }

    ASDU_BASE* pAsduBase = (ASDU_BASE*)pBuff;

    if (pAsduBase->IsSQ1())
    {
        ASDU13_SQ1* pAsdu13SQ1 = (ASDU13_SQ1*)pBuff;

        int nCount = pAsdu13SQ1->GetItemCount();

        int nPointID = pAsdu13SQ1->infoaddr.GetAddr();

        char cAddr[4] = { 0 };
        cAddr[0] = pBuff[6];
        cAddr[1] = pBuff[7];
        cAddr[2] = pBuff[8];

        InsertNodeInfo(ChangeOxType(QByteArray(cAddr, 3).toHex().toUpper()), tr("Infomation Address:%1").arg(QString::number(nPointID)));


        for (int i = 0; i < nCount; i++)
        {
            float fValue = pAsdu13SQ1->GetValue(i);

            QDS qds = pAsdu13SQ1->GetQDS(i);

            char cNodeInfo[5] = { 0 };
            cNodeInfo[0] = pBuff[9 + i * 5];
            cNodeInfo[1] = pBuff[10 + i * 5];
            cNodeInfo[2] = pBuff[11 + i * 5];
            cNodeInfo[3] = pBuff[12 + i * 5];
            cNodeInfo[4] = pBuff[13 + i * 5];

            InsertNodeInfo(ChangeOxType(QByteArray(cNodeInfo, 5).toHex().toUpper()), tr("SerialNumber:%1 IV:%8  NT:%7 SB:%6    "
                " BL:%5  RES:%4  OV:%3  Value:%2 ").arg(nPointID).arg(fValue).arg(qds.OV).arg(qds.RES)
                .arg(qds.BL).arg(qds.SB).arg(qds.NT).arg(qds.IV));

            nPointID++;
        }
    }
    else
    {
        ASDU13_SQ0* pAsdu13SQ0 = (ASDU13_SQ0*)pBuff;

        int nCount = pAsdu13SQ0->GetItemCount();

        for (int i = 0; i < nCount; i++)
        {
            int nPointID = pAsdu13SQ0->m_data[i].infoaddr.GetAddr();

            float fValue = pAsdu13SQ0->GetValue(i);

            QDS qds = pAsdu13SQ0->GetQDS(i);

            char cNodeInfo[8] = { 0 };
            cNodeInfo[0] = pBuff[6 + i * 8];
            cNodeInfo[1] = pBuff[7 + i * 8];
            cNodeInfo[2] = pBuff[8 + i * 8];
            cNodeInfo[3] = pBuff[9 + i * 8];
            cNodeInfo[4] = pBuff[10 + i * 8];
            cNodeInfo[5] = pBuff[11 + i * 8];
            cNodeInfo[6] = pBuff[12 + i * 8];
            cNodeInfo[7] = pBuff[13 + i * 8];

            InsertNodeInfo(ChangeOxType(QByteArray(cNodeInfo, 8).toHex().toUpper()), tr("SerialNumber:%1 IV:%8  NT:%7  SB:%6   "
                " BL:%5  RES:%4  OV:%3  Value:%2").arg(nPointID).arg(fValue).arg(qds.OV).arg(qds.RES)
                .arg(qds.BL).arg(qds.SB).arg(qds.NT).arg(qds.IV));
        }
    }
}

void CAnalyse104Msg::OnRecvAnalogShortFloatShortTime(char* pBuff, int nLength)
{
    if (nLength <= 0)
    {
        return;
    }
    ASDU14_SQ0* pAsdu14SQ0 = (ASDU14_SQ0*)pBuff;

    int nCount = pAsdu14SQ0->GetItemCount();

    for (int i = 0; i < nCount; i++)
    {
        int nPointID = pAsdu14SQ0->m_data[i].m_infoaddr.GetAddr();

        float fValue = pAsdu14SQ0->GetValue(i);

        QDS qds = pAsdu14SQ0->GetQDS(i);

        char cNodeInfo[11] = { 0 };
        cNodeInfo[0] = pBuff[6 + i * 11];
        cNodeInfo[1] = pBuff[7 + i * 11];
        cNodeInfo[2] = pBuff[8 + i * 11];
        cNodeInfo[3] = pBuff[9 + i * 11];
        cNodeInfo[4] = pBuff[10 + i * 11];
        cNodeInfo[5] = pBuff[11 + i * 11];
        cNodeInfo[6] = pBuff[12 + i * 11];
        cNodeInfo[7] = pBuff[13 + i * 11];
        cNodeInfo[8] = pBuff[14 + i * 11];
        cNodeInfo[9] = pBuff[15 + i * 11];
        cNodeInfo[10] = pBuff[16 + i * 11];

        InsertNodeInfo(ChangeOxType(QByteArray(cNodeInfo, 11).toHex().toUpper()), tr("SerialNumber:%1 IV:%8  NT:%7  SB:%6   "
            " BL:%5  RES:%4  OV:%3  Value:%2   Time: %9").arg(nPointID).arg(fValue).arg(qds.OV).arg(qds.RES)
            .arg(qds.BL).arg(qds.SB).arg(qds.NT).arg(qds.IV).arg(pAsdu14SQ0->GetTime(i).Dump()));
    }
}

void CAnalyse104Msg::OnRecvAnalogNormalizedNoQuality(char* pBuff, int nLength)
{
    if (nLength <= 0)
    {
        return;
    }

    ASDU_BASE* pAsduBase = (ASDU_BASE*)pBuff;
    if (pAsduBase->IsSQ1())
    {
        ASDU21_SQ1* pAsdu21SQ1 = (ASDU21_SQ1*)pBuff;

        int nCount = pAsdu21SQ1->GetItemCount();

        int nPointID = pAsdu21SQ1->m_infoaddr.GetAddr();

        char cAddr[4] = { 0 };
        cAddr[0] = pBuff[6];
        cAddr[1] = pBuff[7];
        cAddr[2] = pBuff[8];

        InsertNodeInfo(ChangeOxType(QByteArray(cAddr, 3).toHex().toUpper()), tr("Infomation Address:%1").arg(QString::number(nPointID)));

        for (int i = 0; i < nCount; i++)
        {
            float fValue = pAsdu21SQ1->GetValue(i);
            //
            char cNodeInfo[2] = { 0 };
            cNodeInfo[0] = pBuff[9 + i * 5];
            cNodeInfo[1] = pBuff[10 + i * 5];

            InsertNodeInfo(ChangeOxType(QByteArray(cNodeInfo, 2).toHex().toUpper()), tr("SerialNumber:%1   Value:%2")
                .arg(nPointID).arg(fValue));


            nPointID++;
        }
    }
    else
    {
        ASDU21_SQ0* pAsdu21SQ0 = (ASDU21_SQ0*)pBuff;

        int nCount = pAsdu21SQ0->GetItemCount();

        for (int i = 0; i < nCount; i++)
        {
            int nPointID = pAsdu21SQ0->m_data[i].m_infoaddr.GetAddr();

            float fValue = pAsdu21SQ0->GetValue(i);
            //
            char cNodeInfo[5] = { 0 };
            cNodeInfo[0] = pBuff[6 + i * 5];
            cNodeInfo[1] = pBuff[7 + i * 5];
            cNodeInfo[2] = pBuff[8 + i * 5];
            cNodeInfo[3] = pBuff[9 + i * 5];
            cNodeInfo[4] = pBuff[10 + i * 5];

            InsertNodeInfo(ChangeOxType(QByteArray(cNodeInfo, 5).toHex().toUpper()), tr("SerialNumber:%1   Value:%2").arg(nPointID).arg(fValue));
        }
    }
}

void CAnalyse104Msg::OnRecvKwhShortTime(char* pBuff, int nLength)
{
    if (nLength <= 0)
    {
        return;
    }

    ASDU16_SQ0* pAsdu16SQ0 = (ASDU16_SQ0*)pBuff;
    int nCount = pAsdu16SQ0->GetItemCount();

    for (int i = 0; i < nCount; i++)
    {
        float fValue = pAsdu16SQ0->m_data[i].m_fValue;
        int nPointID = pAsdu16SQ0->m_data[i].m_infoaddr.GetAddr();

        char cNodeInfo[11] = { 0 };
        cNodeInfo[0] = pBuff[6 + i * 11];
        cNodeInfo[1] = pBuff[7 + i * 11];
        cNodeInfo[2] = pBuff[8 + i * 11];
        cNodeInfo[3] = pBuff[9 + i * 11];
        cNodeInfo[4] = pBuff[10 + i * 11];
        cNodeInfo[5] = pBuff[11 + i * 11];
        cNodeInfo[6] = pBuff[12 + i * 11];
        cNodeInfo[7] = pBuff[13 + i * 11];
        cNodeInfo[8] = pBuff[14 + i * 11];
        cNodeInfo[9] = pBuff[15 + i * 11];
        cNodeInfo[10] = pBuff[16 + i * 11];

        InsertNodeInfo(ChangeOxType(QByteArray(cNodeInfo, 11).toHex().toUpper()), tr("SerialNumber:%1  Value:%2  DescriberCode:%3  Time: %4")
            .arg(nPointID).arg(fValue).arg(int(pAsdu16SQ0->m_data[i].m_bcrparam)).arg(pAsdu16SQ0->m_data[i].m_time.Dump()));

    }
}

void CAnalyse104Msg::OnRecvKwhLongTime(char* pBuff, int nLength)
{
    if (nLength <= 0)
    {
        return;
    }
    ASDU37_SQ0* pAsdu37SQ0 = (ASDU37_SQ0*)pBuff;
    int nCount = pAsdu37SQ0->GetItemCount();

    for (int i = 0; i < nCount; i++)
    {
        float fValue = pAsdu37SQ0->m_data[i].m_bcr.number;
        int nPointID = pAsdu37SQ0->m_data[i].m_infoaddr.GetAddr();

        char cNodeInfo[15] = { 0 };
        cNodeInfo[0] = pBuff[6 + i * 15];
        cNodeInfo[1] = pBuff[7 + i * 15];
        cNodeInfo[2] = pBuff[8 + i * 15];
        cNodeInfo[3] = pBuff[9 + i * 15];
        cNodeInfo[4] = pBuff[10 + i * 15];
        cNodeInfo[5] = pBuff[11 + i * 15];
        cNodeInfo[6] = pBuff[12 + i * 15];
        cNodeInfo[7] = pBuff[13 + i * 15];
        cNodeInfo[8] = pBuff[14 + i * 15];
        cNodeInfo[9] = pBuff[15 + i * 15];
        cNodeInfo[10] = pBuff[16 + i * 15];
        cNodeInfo[11] = pBuff[17 + i * 15];
        cNodeInfo[12] = pBuff[18 + i * 15];
        cNodeInfo[13] = pBuff[19 + i * 15];
        cNodeInfo[14] = pBuff[20 + i * 15];

        InsertNodeInfo(ChangeOxType(QByteArray(cNodeInfo, 15).toHex().toUpper()), tr("SerialNumber:%1  TimerNumber:%2  IV:%3  CA: %4  CY: %5   SQ:%6  Time: %7")
            .arg(nPointID).arg(fValue).arg(pAsdu37SQ0->m_data[i].m_bcr.param.IV).arg(pAsdu37SQ0->m_data[i].m_bcr.param.CA).arg(pAsdu37SQ0->m_data[i].m_bcr.param.CY)
            .arg(pAsdu37SQ0->m_data[i].m_bcr.param.SQ).arg(pAsdu37SQ0->m_data[i].m_time.Dump()));
    }
}

void CAnalyse104Msg::OnRecvBinarySinglePointShortTime(char* pBuff, int nLength)
{
    if (nLength <= 0)
    {
        return;
    }
    ASDU2_SQ0* pAsdu2SQ0 = (ASDU2_SQ0*)pBuff;

    int nCount = pAsdu2SQ0->GetItemCount();


    for (int i = 0; i < nCount; i++)
    {
        int nPointID = pAsdu2SQ0->m_data[i].m_infoaddr.GetAddr();

        SIQ siq = pAsdu2SQ0->GetSIQ(i);

        char cNodeInfo[7] = { 0 };
        cNodeInfo[0] = pBuff[6 + i * 7];
        cNodeInfo[1] = pBuff[7 + i * 7];
        cNodeInfo[2] = pBuff[8 + i * 7];
        cNodeInfo[3] = pBuff[9 + i * 7];
        cNodeInfo[4] = pBuff[10 + i * 7];
        cNodeInfo[5] = pBuff[11 + i * 7];
        cNodeInfo[6] = pBuff[12 + i * 7];

        InsertNodeInfo(ChangeOxType(QByteArray(cNodeInfo, 7).toHex().toUpper()), tr("SerialNumber:%1 IV:%2  NT:%3  SB:%4   "
            " BL:%5  RES:%6  SPI:%7   Time: %8").arg(nPointID).arg(siq.IV).arg(siq.NT)
            .arg(siq.SB).arg(siq.BL).arg(siq.RES).arg(siq.SPI).arg(pAsdu2SQ0->GetTime(i).Dump()));
    }
}

void CAnalyse104Msg::OnRecvBinarySinglePointTime(char* pBuff, int nLength)
{
    if (nLength == 0)
    {
        return;
    }
    ASDU30_SQ0* pAsdu30SQ0 = (ASDU30_SQ0*)pBuff;
    int nCount = pAsdu30SQ0->GetItemCount();

    for (int i = 0; i < nCount; i++)
    {
        int nPointID = pAsdu30SQ0->m_data[i].m_infoaddr.GetAddr();

        SIQ siq = pAsdu30SQ0->GetSIQ(i);

        char cNodeInfo[11] = { 0 };
        cNodeInfo[0] = pBuff[6 + i * 11];
        cNodeInfo[1] = pBuff[7 + i * 11];
        cNodeInfo[2] = pBuff[8 + i * 11];
        cNodeInfo[3] = pBuff[9 + i * 11];
        cNodeInfo[4] = pBuff[10 + i * 11];
        cNodeInfo[5] = pBuff[11 + i * 11];
        cNodeInfo[6] = pBuff[12 + i * 11];
        cNodeInfo[7] = pBuff[13 + i * 11];
        cNodeInfo[8] = pBuff[14 + i * 11];
        cNodeInfo[9] = pBuff[15 + i * 11];
        cNodeInfo[10] = pBuff[16 + i * 11];

        InsertNodeInfo(ChangeOxType(QByteArray(cNodeInfo, 11).toHex().toUpper()), tr("SerialNumber:%1 IV:%2  NT:%3  SB:%4   "
            " BL:%5  RES:%6  SPI:%7   Time: %8").arg(nPointID).arg(siq.IV).arg(siq.NT)
            .arg(siq.SB).arg(siq.BL).arg(siq.RES).arg(siq.SPI).arg(pAsdu30SQ0->GetTime(i).Dump()));
    }
}

void CAnalyse104Msg::OnRecvBinaryDoublePointShortTime(char* pBuff, int nLength)
{
    if (nLength <= 0)
    {
        return;
    }
    ASDU4_SQ0* pAsdu4SQ0 = (ASDU4_SQ0*)pBuff;

    int nCount = pAsdu4SQ0->GetItemCount();

    for (int i = 0; i < nCount; i++)
    {
        int nPointID = pAsdu4SQ0->m_data[i].m_infoaddr.GetAddr();

        DIQ diq = pAsdu4SQ0->GetDIQ(i);

        char cNodeInfo[7] = { 0 };
        cNodeInfo[0] = pBuff[6 + i * 7];
        cNodeInfo[1] = pBuff[7 + i * 7];
        cNodeInfo[2] = pBuff[8 + i * 7];
        cNodeInfo[3] = pBuff[9 + i * 7];
        cNodeInfo[4] = pBuff[10 + i * 7];
        cNodeInfo[5] = pBuff[11 + i * 7];
        cNodeInfo[6] = pBuff[12 + i * 7];

        InsertNodeInfo(ChangeOxType(QByteArray(cNodeInfo, 7).toHex().toUpper()), tr("SerialNumber:%1 IV:%2  NT:%3  SB:%4   "
            " BL:%5  RES:%6  SPI:%7   Time: %8").arg(nPointID).arg(diq.IV).arg(diq.NT)
            .arg(diq.SB).arg(diq.BL).arg(diq.RES).arg(diq.DPI).arg(pAsdu4SQ0->GetTime(i).Dump()));
    }
}

void CAnalyse104Msg::OnRecvBinaryDoublePointTime(char* pBuff, int nLength)
{
    if (nLength <= 0)
    {
        return;
    }
    ASDU31_SQ0* pAsdu31SQ0 = (ASDU31_SQ0*)pBuff;

    int nCount = pAsdu31SQ0->GetItemCount();

    for (int i = 0; i < nCount; i++)
    {
        int nPointID = pAsdu31SQ0->m_data[i].m_infoaddr.GetAddr();

        DIQ diq = pAsdu31SQ0->GetDIQ(i);

        char cNodeInfo[11] = { 0 };
        cNodeInfo[0] = pBuff[6 + i * 11];
        cNodeInfo[1] = pBuff[7 + i * 11];
        cNodeInfo[2] = pBuff[8 + i * 11];
        cNodeInfo[3] = pBuff[9 + i * 11];
        cNodeInfo[4] = pBuff[10 + i * 11];
        cNodeInfo[5] = pBuff[11 + i * 11];
        cNodeInfo[6] = pBuff[12 + i * 11];
        cNodeInfo[7] = pBuff[13 + i * 11];
        cNodeInfo[8] = pBuff[14 + i * 11];
        cNodeInfo[9] = pBuff[15 + i * 11];
        cNodeInfo[10] = pBuff[16 + i * 11];

        InsertNodeInfo(ChangeOxType(QByteArray(cNodeInfo, 11).toHex().toUpper()), tr("SerialNumber:%1 IV:%2  NT:%3  SB:%4   "
            " BL:%5  RES:%6  SPI:%7   Time: %8").arg(nPointID).arg(diq.IV).arg(diq.NT)
            .arg(diq.SB).arg(diq.BL).arg(diq.RES).arg(diq.DPI).arg(pAsdu31SQ0->GetTime(i).Dump()));
    }
}

void CAnalyse104Msg::OnRecvGeneralCall(char* pBuff, int nLength)
{
    if (nLength <= 0)
    {
        return;
    }
    //组织ASDU100
    ASDU100* pAsdu100 = (ASDU100*)(pBuff);

    char cAddr[4] = { 0 };
    cAddr[0] = pBuff[6];
    cAddr[1] = pBuff[7];
    cAddr[2] = pBuff[8];

    InsertNodeInfo(ChangeOxType(QByteArray(cAddr, 3).toHex().toUpper()), tr("Serial Number:%1").arg(pAsdu100->m_infoaddr.GetAddr()));

    char cCode[1] = { 0 };
    cCode[0] = pBuff[9];

    InsertNodeInfo(ChangeOxType(QByteArray(cCode, 1).toHex().toUpper()), tr("code:%1 means:General Call").arg(int(cCode[0])));
}

void CAnalyse104Msg::OnControlCommand(char* pBuff, int nLength)
{
    if (nLength <= 0)
    {
        return;
    }

    ASDU45* pAsdu45 = (ASDU45*)(pBuff);

    char cAddr[4] = { 0 };
    cAddr[0] = pBuff[6];
    cAddr[1] = pBuff[7];
    cAddr[2] = pBuff[8];

    char cValue[1] = { 0 };
    cValue[0] = pBuff[9];

    InsertNodeInfo(ChangeOxType(QByteArray(cAddr, 3).toHex().toUpper()), tr("Serial Number:%1 ").arg(pAsdu45->m_infoaddr.GetAddr()));

    InsertNodeInfo(ChangeOxType(QByteArray(cValue, 1).toHex().toUpper()), tr("SelectOrExcute: %1  QU: %2  RES: %3  SCS: %4")
        .arg(pAsdu45->m_sco.SelectOrExcute).arg(pAsdu45->m_sco.QU).arg(pAsdu45->m_sco.RES).arg(pAsdu45->m_sco.SCS));
}

void CAnalyse104Msg::OnControlDoubleCommand(char* pBuff, int nLength)
{
    if (nLength <= 0)
    {
        return;
    }

    ASDU46* pAsdu46 = (ASDU46*)(pBuff);

    char cAddr[4] = { 0 };
    cAddr[0] = pBuff[6];
    cAddr[1] = pBuff[7];
    cAddr[2] = pBuff[8];

    char cValue[1] = { 0 };
    cValue[0] = pBuff[9];

    InsertNodeInfo(ChangeOxType(QByteArray(cAddr, 3).toHex().toUpper()), tr("Serial Number:%1 ").arg(pAsdu46->m_infoaddr.GetAddr()));

    InsertNodeInfo(ChangeOxType(QByteArray(cValue, 1).toHex().toUpper()), tr("SelectOrExcute: %1  QU: %2  DCS: %3")
        .arg(pAsdu46->m_dco.SelectOrExcute).arg(pAsdu46->m_dco.QU).arg(pAsdu46->m_dco.DCS));

}

void CAnalyse104Msg::OnRecvSyncTime(char* pBuff, int nLength)
{
    if (nLength <= 0)
    {
        return;
    }
    //组织ASDU101
    ASDU103* pAsdu103 = (ASDU103*)(pBuff);

    char cAddr[4] = { 0 };
    cAddr[0] = pBuff[6];
    cAddr[1] = pBuff[7];
    cAddr[2] = pBuff[8];

    InsertNodeInfo(ChangeOxType(QByteArray(cAddr, 3).toHex().toUpper()), tr("Serial Number:%1").arg(pAsdu103->m_infoaddr.GetAddr()));

    char cTime[7] = { 0 };
    cTime[0] = pBuff[9];
    cTime[1] = pBuff[10];
    cTime[2] = pBuff[11];
    cTime[3] = pBuff[12];
    cTime[4] = pBuff[13];
    cTime[5] = pBuff[14];
    cTime[6] = pBuff[15];

    InsertNodeInfo(ChangeOxType(QByteArray(cTime, 7).toHex().toUpper()), tr("Time:%1").arg(pAsdu103->m_time.Dump()));

}

void CAnalyse104Msg::OnRecvKwhGeneralCall(char* pBuff, int nLength)
{
    if (nLength <= 0)
    {
        return;
    }
    //组织ASDU102
    ASDU101* pAsdu101 = (ASDU101*)(pBuff);

    char cAddr[4] = { 0 };
    cAddr[0] = pBuff[6];
    cAddr[1] = pBuff[7];
    cAddr[2] = pBuff[8];

    InsertNodeInfo(ChangeOxType(QByteArray(cAddr, 3).toHex().toUpper()), tr("Serial Number:%1").arg(pAsdu101->m_infoaddr.GetAddr()));

    char cCode[1] = { 0 };
    cCode[0] = pBuff[9];

    InsertNodeInfo(ChangeOxType(QByteArray(cCode, 1).toHex().toUpper()), tr("Qcc:%1 ").arg(pAsdu101->m_qcc));
}

void CAnalyse104Msg::OnRecvDevReadCommand(char* pBuff, int nLength)
{
    if (nLength == 0)
    {
        return;
    }

    ASDUDZ2* pAsdudz = (ASDUDZ2*)pBuff;

    char cAddr[4] = { 0 };
    cAddr[0] = pBuff[6];
    cAddr[1] = pBuff[7];
    cAddr[2] = pBuff[8];

    InsertNodeInfo(ChangeOxType(QByteArray(cAddr, 3).toHex().toUpper()), tr("Serial Number:%1").arg(pAsdudz->m_data[0].infoaddr.GetAddr()));

}

void CAnalyse104Msg::OnRecvDevWriteCommand(char* pBuff, int nLength)
{
    if (nLength == 0)
    {
        return;
    }

//     ASDUDZ* pAsdudz = (ASDUDZ*)pBuff;
// 
//     char cAddr[4] = { 0 };
//     cAddr[0] = pBuff[6];
//     cAddr[1] = pBuff[7];
//     cAddr[2] = pBuff[8];
// 
//     InsertNodeInfo(ChangeOxType(QByteArray(cAddr, 3).toHex().toUpper()), tr("Serial Number:%1").arg(pAsdudz->infoaddr.GetAddr()));
// 
// 
//     int nCount = pAsdudz->GetItemCount();
// 
//     for (int i = 0; i < nCount; i++)
//     {
//         short nValue = pAsdudz->m_data[i].m_nValue;
// 
// 
//         char cNodeInfo[3] = { 0 };
//         cNodeInfo[0] = pBuff[9 + i * 2];
//         cNodeInfo[1] = pBuff[10 + i * 2];
// 
//         InsertNodeInfo(ChangeOxType(QByteArray(cNodeInfo, 2).toHex().toUpper()), tr("Value:  %1 ").arg(nValue));
//     }
// 
//     DZQOS diq = pAsdudz->m_dzQos;
//     char cDiq[1] = { 0 };
//     cDiq[0] = diq.DQ;
// 
//     InsertNodeInfo(ChangeOxType(QByteArray(cDiq, 1).toHex().toUpper()), tr("QOS:  %1 ").arg(diq.DQ));
}

