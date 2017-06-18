/*! @file xmlwriter.h
<PRE>
********************************************************************************
模块名       :
文件名       :  xmlwriter.h
文件实现功能 :  XML测试写文件功能
作者         :  xingzhibing
版本         :  V1.00
--------------------------------------------------------------------------------
备注         :  <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief   XML测试写文件功能
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
