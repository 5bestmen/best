#ifndef SCDMAINVIEW_H
#define SCDMAINVIEW_H

#include "scdview_global.h"
#include "dbgstudio/plugin_module.h"
#include "scddatamodel.h"
#include <ace/Event.h>
#include <QObject>
#include <memory>
#include <unordered_map>

class CViewMemDB;
class IMainModule;
class IPluginModule;
class IMainWindow;
class QStandardItem;
class CFesDBR;
struct DEVICE;
class CScdDataModel;
class QTableView;
class QTimer;
class QLabel;
class CScdMainView : public IPluginModule
{
	Q_OBJECT
public:
	CScdMainView();
	~CScdMainView();
public:
	virtual void Init(IMainModule *pCore);
	virtual void UnInit();
	virtual void SetLoadDeviceName(const QString &strDeviceName);
protected:
	void InitView();
	void InitTreeView();
	void InitNode(QStandardItem*, int32u nNodeOccNo);
	void InitChannel(QStandardItem*, CFesDBR *,int32u nChannelOccNo);
	void InitDevice(QStandardItem*, CFesDBR *, int32u nChannelOccNo, int32u nDevOccNo);
	void SetHeaderList(CScdDataModel* pModel, int nType);
	void SetModelData(CScdDataModel* pModel, const int& nType, const int32u& nNodeOccNo, const int32u& nChannelOccNo, const int32u& nDevOccNo);
	bool IsFileAdded(QString szName);
	void FormatNameString(int nType, const int32u& nNodeOccNo, const int32u& nChannelOccNo, const int32u& nDevOccNo,QString& szName);
	bool IsDBAlive(int nTimeOut);

	bool LoadDBMem();
	bool UnLoadDBMem();

	void CloseAllTabs();
protected Q_SLOTS:
	void slot_TreeDbClicked(const QModelIndex& index);
	void slot_TabViewAdd(const int& nType, const int32u& nNodeOccNo, const int32u& nChannelOccNo, const int32u& nDevOccNo);
	void slot_onTimer();
	void slot_CloseTabWidget(int nIndex);
	void slot_TabChanged(int nIndex);
private:
	IMainModule * m_pCore;
	QString m_szDeviceName;
	CViewMemDB_DEF  m_pMem;
private:
	QStringList m_szHeaderList;
	QStandardItem* m_pRootItem;
	QTimer* m_pTimer;
	QLabel* m_pLabel;
	QTableView* m_pCurrentView;
private:
	bool m_bInitalize;   //≥ı ºªØ
	bool m_bUnInitalize; //–∂‘ÿ¡À
	std::shared_ptr<ACE_Event> m_pDBAliveFlag;
	std::map<QString, QTableView* > m_mapView;
Q_SIGNALS:
	void sig_DataView(const int&, const int32u&, const int32u&, const int32u&);
};

extern "C" SCDVIEW_EXPORT IPluginModule  *CreateModule();
extern "C" SCDVIEW_EXPORT  void    DestroyModule();

#endif // SCDMAINVIEW_H
