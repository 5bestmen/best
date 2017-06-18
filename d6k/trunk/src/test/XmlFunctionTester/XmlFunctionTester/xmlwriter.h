/*! @file xmlwriter.h
<PRE>
********************************************************************************
ģ����       :
�ļ���       :  xmlwriter.h
�ļ�ʵ�ֹ��� :  XML����д�ļ�����
����         :  xingzhibing
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         :  <����˵��>
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

********************************************************************************
</PRE>
*  @brief   XML����д�ļ�����
*  @author  xingzhibing
*  @version 1.0
*  @date    2016.09.01
*******************************************************************************/

#ifndef XMLWRITER_H
#define XMLWRITER_H

#include <QObject>
#include <array>
#include <memory>

#include "configinfo.h"
class QXmlStreamWriter;

class CXmlWriter : public QObject
{
	Q_OBJECT

public:
	CXmlWriter(QObject *parent = Q_NULLPTR);
	~CXmlWriter();
	void SetFileInfo(std::shared_ptr <CDeviceMgr> pMgr);
	void WriteFile(const QString& szName);
	void WriteDevInfo(QXmlStreamWriter* pWriter, CDevice* pDev);
	void WriteAinInfo(QXmlStreamWriter* pWriter, AIN* pAin);
	void WriteDinInfo(QXmlStreamWriter* pWriter, DIN* pDin);
Q_SIGNALS:
	void sig_ProcessInfo(const QString& info);
private:
	std::shared_ptr <CDeviceMgr> m_pDevMgr;
	unsigned int m_StartTime;
private:
	unsigned int  GetProessInfo();
};

#endif // XMLWRITER_H
