/*! @file xmlreader.h
<PRE>
********************************************************************************
ģ����       :
�ļ���       :  xmlreader.h
�ļ�ʵ�ֹ��� :  XML���Զ��ļ�����
����         :  xingzhibing
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         :  <����˵��>
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

********************************************************************************
</PRE>
*  @brief   XML���Զ��ļ�����
*  @author  xingzhibing
*  @version 1.0
*  @date    2016.09.01
*******************************************************************************/

#ifndef XMLREADER_H
#define XMLREADER_H

#include <QObject>
#include <array>
#include <memory>
#include <QXmlStreamReader>
#include "configinfo.h"

class CXmlReader : public QObject
{
	Q_OBJECT

public:
	CXmlReader(QObject *parent=Q_NULLPTR);
	~CXmlReader();
public:
	void SetFileInfo(std::shared_ptr<CDeviceMgr> pMgr);
	void ReadFileInfo(const QString& szName);
	void ReadConfigElement();
	void SkipUnknownElement();
	void ReadDevElement();	
	void ReadAinElement(CDevice* pDev);
	void ReadAinDataElement(CDevice* pDev);
	void ReadDinElement(CDevice* pDev);
	void ReadDinDataElement(CDevice* pDev);
Q_SIGNALS:
	void sig_ProcessInfo(const QString& info);
private:
	std::shared_ptr <CDeviceMgr> m_pDevMgr;
	QXmlStreamReader m_XmlReader;
	unsigned int m_StartTime;
private:
	unsigned int  GetProessInfo();
};

#endif // XMLREADER_H
