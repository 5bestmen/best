#ifndef SCADAVARIABLEMODULE_H
#define SCADAVARIABLEMODULE_H

#include "scadastudio/imodule.h"

#include <QObject>

namespace Config
{
	class CScadaVariableConfigData;
	class CScadaUserVariableGroup;
	class CScadaAnalogAlarmGroup;
	class CScadaAinAlarmLimit;
	class CScadaAnalogAlarm;
	class CScadaDigitalAlarmGroup;
	class CScadaDigitalAlarm;
	class CScadaTransformGroup;
	class CScadaData;
	class CScadaGroup;
}

class QStandardItem;
class QAbstractItemModel;
class QModelIndex;

class CScadaVariableModule : public QObject, public IModule
{
	Q_OBJECT

public:
	CScadaVariableModule(IMainModuleInterface *pCore);
	~CScadaVariableModule();

	virtual void Init(IMainModuleInterface *pUi);

	virtual void UnInit();

	virtual bool SaveProject(QDomDocument *pXml, QDomElement *pRoot, const QString & szRoot);
	virtual bool LoadProject(QDomDocument *pXml, QDomElement *pRoot, const QString & szRoot);
	virtual void CreateNewProject(QDomDocument *pXml, QDomElement *pRoot, const QString & szRoot, CQuoteItem *pTopItem);
	virtual void CloseProject(QDomDocument *pXml, QDomElement *pRoot, const QString & szRoot, bool bSaveFlag);

	virtual bool GetModifyFlag() const
	{
		return true;
	}

	virtual bool Check()
	{
		return true;
	}

	//前置DEVICE_AI DEVICE_DI DEVICE_AO DEVICE_DO 点tagname修改,通知绑定点的模块做出相应的修改 删除tagname为空 
	virtual bool ChangeTagNameNodify(QString &tagName, QString &lastTagName, int nType);


	//字符串描述内存池
	virtual bool GetDesPoolArr(std::unordered_map<std::string, int32u> *pHash, std::vector<std::string> *pStringPoolVec, int32u *pDescStringPoolOccNo);


	//clear module data
	virtual bool ClearModuleData();

	virtual bool GetScadaConfigName(std::vector< std::string > & arrScadaNames);

public slots:
	void showMouseRightButton(const QPoint& point);
	void DoubleClicked(const QModelIndex &index);

	void itemChanged(QStandardItem *item);

private:
	
	void ShowMouseRightConfig(QModelIndex &index);
	void ShowMouseRightScada(QModelIndex &index);
	void AddScadaGroup(QModelIndex &index);
	void AddScadaItem(QModelIndex &index);
	void DeleteScadaGroup(QModelIndex &index);
	void DeleteScadaItem(QModelIndex &index);
	void DeleteScadaModuleWidgetInTabWidget(QModelIndex &index);

	void AddScadaVariableUserGroup(QModelIndex &index);
	void AddScadaVariableUserGroup(QModelIndex &index, Config::CScadaUserVariableGroup *pGroup);
	void GetItemMap(const QAbstractItemModel *pModel, const QModelIndex &top, int nType, QVector<QModelIndex> &indexVec);
	QString GetGroupName(QModelIndex &index);
	QString GetLastGroupName(QModelIndex &index);
	void DeleteUserVariableGroup(QModelIndex &index);

	void CreateNewUI();
	void CreateScadaGroupUi(Config::CScadaGroup *pScadaGroup, CQuoteItem *pItem);
	void InitVariable(CQuoteItem *pItem, Config::CScadaData *pScadaData);
	bool InitScadaUserGroup(CQuoteItem *pItem, Config::CScadaUserVariableGroup *pGroup);
	void InitAlarm(CQuoteItem *pItem, Config::CScadaData *pScadaData);
	bool InitAIAlarmGroup(CQuoteItem *pItem, Config::CScadaAnalogAlarmGroup *pGroup);
	void InitAIOfflimit(CQuoteItem *pItem, Config::CScadaAnalogAlarm *pAlarm);
	bool InitDIAlarmGroup(CQuoteItem *pItem, Config::CScadaDigitalAlarmGroup *pGroup);
	void InitDIOfflimit(CQuoteItem *pItem, Config::CScadaDigitalAlarm *pAlarm);
	void InitTransfrom(CQuoteItem *pItem, Config::CScadaData *pScadaData);
	bool InitTransformGroup(CQuoteItem *pItem, Config::CScadaTransformGroup *pGroup);
	bool InitNonTransformGroup(CQuoteItem *pItem, Config::CScadaTransformGroup *pGroup);

	void AddAnalogAlarm(QModelIndex &index);
	void AddAnalogAlarmGroup(QModelIndex &index);

	void AddDigitalAlarm(QModelIndex &index);
	void AddDigitalAlarmGroup(QModelIndex &index);

	void ModifyAnalogAlarmGroup(QStandardItem *item);
	void ModifyAnalogAlarmItem(QStandardItem *item);
	void ModifyDigitalAlarmGroup(QStandardItem *item);
	void ModifyDigitalAlarmItem(QStandardItem *item);

	void ShowAnalogAlarmView(const QModelIndex &index);
	void ShowAnalogAlarmLimitView(const QModelIndex &index);
	void ShowDigitalAlarmView(const QModelIndex &index);
	void ShowDigitalAlarmLimitView(const QModelIndex &index);

	void DeleteAnalogAlarmGroup(QModelIndex &index);
	void DeleteAnalogAlarmItem(QModelIndex &index);
	void DeleteDigitalAlarmGroup(QModelIndex &index);
	void DeleteDigitalAlarmItem(QModelIndex &index);

	void CreateFesUi(Config::CScadaVariableConfigData *pConfig);

	void AddLinearScale(QModelIndex &index);
	void AddLinearScaleGroup(QModelIndex &index);
	void AddNonLinearScale(QModelIndex &index);
	void AddNonLinearScaleGroup(QModelIndex &index);

	void ShowScaleLinearView(const QModelIndex &index);

	void ModifyScaleLinearItem(QStandardItem *item);
	bool DeleteScaleLinearItem(QModelIndex &index);
	bool DeleteLinearScaleWidgetInTabWidget(const QModelIndex &index, Config::CScadaData *pScada);
	bool DeleteScaleLinearGroup(QModelIndex &index);

	void ModifyScaleLinearGroup(QStandardItem *item);

	void ShowScaleNonLinearView(const QModelIndex &index);
	void ModifyScaleNonLinearItem(QStandardItem *item);
	void ModifyScaleNonLinearGroup(QStandardItem *item);
	bool DeleteScaleNonLinearItem(QModelIndex &index);
	bool DeleteNonLinearScaleWidgetInTabWidget(const QModelIndex &index, Config::CScadaData *pScada);
	bool DeleteScaleNonLinearGroup(QModelIndex &index);

	Config::CScadaData *GetScada(QModelIndex index);

private:
	Config::CScadaVariableConfigData *m_pScadaVirableConfig;
	IMainModuleInterface *m_pCore;
	//查找字符串内存池
	std::unordered_map<std::string, int32u> *m_pHash;
	std::vector<std::string> *m_pStringPoolVec;
	int32u *m_pDescStringPoolOccNo;
};

#endif // SCADAVARIABLEMODULE_H
