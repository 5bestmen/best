/*! @file xmlreader.h
<PRE>
********************************************************************************
模块名       :
文件名       :  xmlreader.h
文件实现功能 :  XML测试读文件功能
作者         :  xingzhibing
版本         :  V1.00
--------------------------------------------------------------------------------
备注         :  <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief   XML测试读文件功能
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
