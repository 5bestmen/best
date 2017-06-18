#ifndef MAIN_MODULE_IMPL_H
#define MAIN_MODULE_IMPL_H

#include <memory>

#include <QObject> 

#include "devicestudio/main_module.h"
#include "modulemgr.h"
 
class IMainWindow;
class CModuleMgr;

class CMainModule : public IMainModule
{
	Q_OBJECT
public:
	CMainModule(QObject *parent);
	~CMainModule();

	//获得main ui指针
	virtual IMainWindow *GetMainWindow();	 
	//加载plugin
	bool LoadModule(const QString &strName, const QString &strDeviceName);

	virtual bool LogString(const char *pszMoudleName, const char *szLogTxt, int nLevel);

	//网络数据报文
	virtual bool CommMsgLog(const char *pszModuleName, const char *pMsg, int nMsgLen, unsigned int nMsgType, const char * pMsgSubType, const char *pSource, const char *pDesc);
    //监控界面
    virtual CCaptureMsgWgt *GetCaptureWidget();
    //监控详细信息
    virtual QTreeWidget *GetDetailMsgWgt();

public slots:
	void DeleteMainModuleSlot();
	//加载插件
	void Slot_RelodePlugin(const QString &strPluginName, const QString &strDeviceName);

private:
	IMainWindow *m_pMainWindow;
	std::shared_ptr<CModuleMgr> m_pModuleMgr;
	//CModuleMgr  *m_pModuleMgr;
};

#endif // MAIN_MODULE_IMPL_H
