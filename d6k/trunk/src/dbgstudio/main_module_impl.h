#ifndef MAIN_MODULE_IMPL_H
#define MAIN_MODULE_IMPL_H

#include <memory>

#include <QObject> 

#include "dbgstudio/main_module.h"
#include "modulemgr.h"
 
class IMainWindow;
class CModuleMgr;

class CMainModule : public IMainModule
{
	Q_OBJECT
public:
	CMainModule(QObject *parent);
	~CMainModule();

	//���main uiָ��
	virtual IMainWindow *GetMainWindow();	 
	//����plugin
	bool LoadModule(const QString &strName, const QString &strDeviceName);

	virtual bool LogString(const char *pszMoudleName, const char *szLogTxt, int nLevel);

	//�������ݱ���
	virtual bool CommMsgLog(const char *pszModuleName, const char *pMsg, int nMsgLen, unsigned int nMsgType, const char * pMsgSubType, const char *pSource, const char *pDesc);

public slots:
	void DeleteMainModuleSlot();
	//���ز��
	void Slot_RelodePlugin(const QString &strPluginName, const QString &strDeviceName);

private:
	IMainWindow *m_pMainWindow;
	std::shared_ptr<CModuleMgr> m_pModuleMgr;
	//CModuleMgr  *m_pModuleMgr;
};

#endif // MAIN_MODULE_IMPL_H
