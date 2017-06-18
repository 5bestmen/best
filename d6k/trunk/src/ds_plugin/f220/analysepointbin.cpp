/*! @file
<PRE>
********************************************************************************
模块名       :	解析点表bin文件
文件名       :	analysepointbin.cpp
文件实现功能 :
作者         :  ww
版本         :  V1.00
--------------------------------------------------------------------------------
备注         : <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief
*  @author ww
*  @version 1.0
*  @date    2016.9.27
*/
#include <QFile>
#include <QMessageBox>
#include <QDebug>
#include "analysepointbin.h"

/*! \fn CCheckRemoteWgt::CAnalysePointBin(QObject *parent)
*********************************************************************************************************
** \brief CCheckRemoteWgt::CCheckRemoteWgt
** \details   构造函数
** \param  parent
** \return
** \author ww
** \date 2016年09月27日
** \note
********************************************************************************************************/
CAnalysePointBin::CAnalysePointBin(QObject *parent)
    : QObject(parent)
{
}

CAnalysePointBin::~CAnalysePointBin()
{

}

/*! \fn CCheckRemoteWgt::InitData(const QString &strFileName)
*********************************************************************************************************
** \brief CCheckRemoteWgt::InitData(const QString &strFileName)
** \details   打开需要解析的文件
** \param  文件名称
** \return
** \author ww
** \date 2016年09月27日
** \note
********************************************************************************************************/
void CAnalysePointBin::InitData(const QString &strFileName)
{
    //m_mapNeedpoints.clear();
	m_lstAnalog.clear();
	m_lstBinary.clear();
	m_lstControl.clear();
	m_lstKwh.clear();

    QFile filePointBin(strFileName);

    if (filePointBin.open(QIODevice::ReadOnly | QIODevice::Text))
    {

		while (!filePointBin.atEnd())
		{
			QByteArray tArray = filePointBin.readLine();

			QString strLineInfo = QString::fromLocal8Bit(tArray);
			strLineInfo.remove(" ");
			if (strLineInfo.isEmpty())
			{
				continue;
			}
			
			if (strLineInfo.contains(QStringLiteral("引用表类型=")))
			{
				strLineInfo.remove(QStringLiteral("引用表类型="));

				if (strLineInfo.contains("YC"))
				{
					QByteArray tNumbers = filePointBin.readLine();
					QString strNums = QString::fromLocal8Bit(tNumbers);
					strNums.remove(" ");
					strNums.remove(QStringLiteral("引用表数目="));
					int nNumber = strNums.toInt();

					for (int i=0; i<nNumber; i++)
					{
						QByteArray btData = filePointBin.readLine();
						QString strData = QString::fromLocal8Bit(btData);
						strData.remove(" ");

						m_lstAnalog.append(strData.split(",")[1].toInt());
					}
				}
				else if (strLineInfo.contains("YX"))
				{
					QByteArray tNumbers = filePointBin.readLine();
					QString strNums = QString::fromLocal8Bit(tNumbers);
					strNums.remove(" ");
					strNums.remove(QStringLiteral("引用表数目="));
					int nNumber = strNums.toInt();

					for (int i = 0; i<nNumber; i++)
					{
						QByteArray btData = filePointBin.readLine();
						QString strData = QString::fromLocal8Bit(btData);
						strData.remove(" ");

						m_lstBinary.append(strData.split(",")[1].toInt());
					}
				}
				else if (strLineInfo.contains("YM"))
				{
					QByteArray tNumbers = filePointBin.readLine();
					QString strNums = QString::fromLocal8Bit(tNumbers);
					strNums.remove(" ");
					strNums.remove(QStringLiteral("引用表数目="));
					int nNumber = strNums.toInt();

					for (int i = 0; i<nNumber; i++)
					{
						QByteArray btData = filePointBin.readLine();
						QString strData = QString::fromLocal8Bit(btData);
						strData.remove(" ");

						m_lstKwh.append(strData.split(",")[1].toInt());
					}
				}
				else if (strLineInfo.contains("YK"))
				{
					QByteArray tNumbers = filePointBin.readLine();
					QString strNums = QString::fromLocal8Bit(tNumbers);
					strNums.remove(" ");
					strNums.remove(QStringLiteral("引用表数目="));
					int nNumber = strNums.toInt();

					for (int i = 0; i<nNumber; i++)
					{
						QByteArray btData = filePointBin.readLine();
						QString strData = QString::fromLocal8Bit(btData);
						strData.remove(" ");

						m_lstControl.append(strData.split(",")[1].toInt());
					}
				}
			}
		}

        
        filePointBin.close();
    }
    else
    {
        QMessageBox::warning(0, tr("Warninig"),tr("can not find %1 file").arg(strFileName));
    }
}
