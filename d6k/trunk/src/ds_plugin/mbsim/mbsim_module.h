#ifndef MBSIM_MODULE_H
#define MBSIM_MODULE_H

#include <QMap>
#include <memory>

#include "ds_global.h"

#include "devicestudio/plugin_module.h"

bool LogString(char *pszMoudleName, char *szLogTxt, int nLevel);
 
class IMainWindow;
class CModbusSvc;

class DS_SHARED_EXPORT CMbSimModule : public IPluginModule
{
	Q_OBJECT
public:
    CMbSimModule();
	//往树结构中插入数据
	void CreateTreeItem();

	bool LogString(const char *pszMoudleName,const char *szLogTxt, int nLevel);
	bool CommMsgLog(const char *pszModuleName, const char *pMsg, int nMsgLen, unsigned int nMsgType, const char * pMsgSubType, const char *pSource, const char *pDesc) ;

public:
	virtual void Init(IMainModule *pCore) ;
	virtual void UnInit() ;
	//设置加载的插件的名称
	virtual void SetLoadDeviceName(const QString &strDeviceName);
protected:
	void ShowMenu(QModelIndex &index);
	void ShowRootMenu(QModelIndex &index);

public slots:
	void Slot_ClickLeftTreeItem(const QModelIndex &mIndex);
	void Slot_CloseOneTabWidget(int iIndex);
	void showMouseRightButton(const QPoint &point);

private:
	//模块加载工具
	IMainModule *m_pMainModule;
	//主界面加载控件对象
	IMainWindow *m_pMainWindow;
	//树结构 编号  对应的tab页
	QMap<int, QWidget*> m_mapIndexTab;
	//树结构 tab页  编号
	QMap<QWidget*, int> m_mapTabIndex;

	std::shared_ptr<CModbusSvc> m_pMBSvc;
};

#endif // MBSIM_MODULE_H
