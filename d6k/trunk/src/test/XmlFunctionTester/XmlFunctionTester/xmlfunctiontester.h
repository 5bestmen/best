/*! @file xmlfunctiontester.h
<PRE>
********************************************************************************
模块名       :
文件名       :  xmlfunctiontester.h
文件实现功能 :  XML测试工具主画面
作者         :  xingzhibing
版本         :  V1.00
--------------------------------------------------------------------------------
备注         :  <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief   XML基本数据定义
*  @author  xingzhibing
*  @version 1.0
*  @date    2016.09.01
*******************************************************************************/

#ifndef XMLFUNCTIONTESTER_H
#define XMLFUNCTIONTESTER_H
#include <array>
#include <memory>
#include <QtWidgets/QMainWindow>
#include "ui_xmlfunctiontester.h"
#include "xmlreader.h"
#include "xmlwriter.h"
#include "configinfo.h"

class XmlFunctionTester : public QMainWindow
{
	Q_OBJECT

public:
	XmlFunctionTester(QWidget *parent = 0);
	~XmlFunctionTester();

public Q_SLOTS:
	void slot_OnCreateDeviceModule();
	void slot_OnCreateFileInfo();
	void slot_OnLoadFileInfo();
	void slot_OnCreateProcessView(const QString& str);
	void slot_OnLoadProcessView(const QString& str);
	void slot_onRealAllInfo();
private:
	void InitView();
private:
	Ui::XmlFunctionTesterClass ui;
	CXmlReader* m_pXmlReader;
	CXmlWriter* m_pXmlWriter;

	std::shared_ptr <CDeviceMgr> m_pDevMgr;

	int m_AinNum;
	int m_DInNum;
	int m_DevNum;
};

#endif // XMLFUNCTIONTESTER_H
