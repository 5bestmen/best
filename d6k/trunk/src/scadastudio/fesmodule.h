#ifndef CFESMODEL_H
#define CFESMODEL_H

#include <QMap>
#include <QModelIndex>
#include "scadastudio/imodule.h"
#include "config_data.h"

class CTreeWidget;
class QStandardItemModel;
class CQuoteItem;
class QMenu;
class QAction;
class QModelIndex;
class IMainWindow;
class CTabWidget;
class CChannel;
class CAlarm;
class CScale;
class CVariable;
class CHisSave;
class CPdr;
class CForwarding;
class QStandardItem;
class CXmlReader;
class QXmlStreamReader;
//class CChannelData;
class QAbstractItemModel;
//class COutputMessagerClient;
class QDomDocument;
class IMainModuleInterface;
class FES_PROJECT_INFO;
class QXmlStreamWriter;

class CFesModule :public QObject,public IModule
{
	Q_OBJECT
public:
	enum FesState{Nomal, FesProjectModify, FesFileModify, NewFes};
	
	CFesModule(IMainModuleInterface *pCore);
	~CFesModule(void);

public:
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

	virtual IMainModuleInterface *GetInterface() const
	{
		return NULL;
	}

	bool Check()
	{
		return true;
	}

	virtual bool GetFesConfigName(std::vector< std::string > & arrFesNames)
	{
		auto arr = m_pConfig->GetFecArr();

		auto it = arr.begin();
		for (; it != arr.end(); ++it)
		{
			arrFesNames.push_back(it->first);
		}
		
		return true;
	}

	virtual QWidget* SelectTagDialog(const std::vector<int>& arrAllowDataTypes, bool bFilterEnWrite, std::string & szNodeTagName, std::string & szTagName, std::string &szFiledName);

	virtual bool ChangeTagNameNodify(QString &tagName, QString &lastTagName, int nType);

	//字符串描述内存池
	virtual bool GetDesPoolArr(std::unordered_map<std::string, int32u> *pHash, std::vector<std::string> *pStringPoolVec, int32u *pDescStringPoolOccNo);

	virtual QString GetOccNoFromScadaVariableUserSourceTagName(const QString &strFes, const QString &strRelatedTagName);

	//virtual void SetModifyFlag(bool bFlag)
	//{
	//	//m_bModifyFlag = bFlag;
	//}

	//static QString GetTagname(const QModelIndex &index, int nType);

	//static Config::CFesData *GetFes(std::vector<Config::CFesData *> &arrFes, QString strTag);

	//static Config::CChannelData *GetChannel(Config::CFesData *pFes, QString &strTag);

	//static Config::CLinearTransform *GetFesLinear(Config::CFesData *pFes, QString &strTag, int nType);

	//static Config::CNonlinearTransform *GetFesNonLinear(Config::CFesData *pFes, QString &strTag, int nType);
	
	//static Config::CTransform *GetFetTransform(Config::CFesData *pFes, QString &strTag);

	//static Config::CAnalogAlarm *GetAnalogAlarm(Config::CFesData *pFes, QString &strTag);

	//static Config::CAinAlarmLimit *GetAnalogAlarmLimit(Config::CAnalogAlarm *pAnalogAlarm, QString& strTag);

	//static Config::CDevice *GetDevice(Config::CChannelData *pChannel, QString &strTag);

	//static Config::CDigtalAlarm *GetDigtitalAlarm(Config::CFesData *pFes, QString &strTag);
	
	//static Config::ALARM_OFFLIMIT_DI *GetDigtitalAlarmLimit(Config::CDigtalAlarm *pDigtalAlarm, QString& strTag);

	//static Config::CFesData *GetFesFromAlarmTagname(std::vector<Config::CFesData *> &arrFes, QString strTag, int nType);

	//根据当前项,向下遍历nType对应的子项
	static void GetItemMap(const QAbstractItemModel *pModel, const QModelIndex &top, int nType, QVector<QModelIndex> &indexVec);
	//item的分组 保留
	static QString GetGroupName(QModelIndex &pIndex, int nType);

	//用户变量组比较特殊，双击组直接显示数据，tabwidget显示递归组
	QString GetUserVaraibleGroupName(QModelIndex &index);
	QString GetLastVaraibleUserGroupName(QModelIndex &index);
private:
	void AddSubFesItem(CQuoteItem *pItem);

	void ShowMenu(QModelIndex &index);

	void AddFes(QModelIndex &index);

	void AddFesGroup(QModelIndex &index);

	void DeleteFesGroup(QModelIndex &index);
	void DeleteFesItem(QModelIndex &index);

	bool CheckSlbingGroupExist(QModelIndex &index);

	//void ReadFes(QXmlStreamReader &reader, CQuoteItem *pFesItem, Config::CFesData *pFesData);
	
	/*void ReadChannel(QXmlStreamReader &reader, Config::CFesData *pFesData);
	void ReadC(QXmlStreamReader &reader, Config::CFesData *pFesData);

	void ReadDevice(QXmlStreamReader &reader, Config::CFesData *pFesData);
	void ReadD(QXmlStreamReader &reader, Config::CFesData *pFesData);

	void ReadData(QXmlStreamReader &reader, Config::CFesData *pFesData);
	void ReadAIN(QXmlStreamReader &reader, Config::CFesData *pFesData);
	void ReadAi(QXmlStreamReader &reader, Config::CFesData *pFesData);

	void ReadDIN(QXmlStreamReader &reader, Config::CFesData *pFesData);
	void ReadDi(QXmlStreamReader &reader, Config::CFesData *pFesData);

	void ReadAOUT(QXmlStreamReader &reader, Config::CFesData *pFesData);
	void ReadAo(QXmlStreamReader &reader, Config::CFesData *pFesData);

	void ReadDOUT(QXmlStreamReader &reader, Config::CFesData *pFesData);
	void ReadDo(QXmlStreamReader &reader, Config::CFesData *pFesData);


	void ReadTransform(QXmlStreamReader &reader, Config::CFesData *pFesData);
	void ReadLinear(QXmlStreamReader &reader, Config::CFesData *pFesData);
	void ReadNonlinear(QXmlStreamReader &reader, Config::CFesData *pFesData);

	void ReadL(QXmlStreamReader &reader, Config::CFesData *pFesData);
	void ReadNl(QXmlStreamReader &reader, Config::CFesData *pFesData);
	void Readnl(QXmlStreamReader &reader, Config::CNonlinearTransform *pData);
	void ReadP(QXmlStreamReader &reader, Config::CNonlinearTransform *pData);

	void ReadAlarm(QXmlStreamReader &reader, Config::CFesData *pFesData);
	void ReadAINAlarm(QXmlStreamReader &reader, Config::CFesData *pFesData);
	void ReadAa(QXmlStreamReader &reader, Config::CAnalogAlarm *pAlarm);
	void Readaa(QXmlStreamReader &reader, Config::CAnalogAlarm *pAlarm);

	void ReadDINAlarm(QXmlStreamReader &reader, Config::CFesData *pFesData);
	void ReadDa(QXmlStreamReader &reader, Config::CDigtalAlarm *pDigital);
	void Readda(QXmlStreamReader &reader, Config::CDigtalAlarm *pDigital);*/

	//根据前置文件添加通道
	void InitChannel(CQuoteItem *pItem, Config::CFesData *pFesData);
	bool InitChannelGroup(CQuoteItem *pItem, Config::CChannelGroup *pGroup);

	//界面 加载转发通道
	void InitForwardingChannel(CQuoteItem *pItem, Config::CFesData *pFesData);
	bool InitForwardingChannelGroup(CQuoteItem *pItem, Config::CForwardingChannelGroup *pGroup);

	//根据前置文件添加装置
	void InitDevice(CQuoteItem *pItem, std::vector <Config::CDevice *> &pTmp);

	//根据前置文件添加组
	void InitGroup(CQuoteItem *pItem, QString &desc, QMap<QString, CQuoteItem *> &groupItem, QStringList &list);

	//根据前置文件初始化转换关系
	void InitTransform(CQuoteItem *pItem, Config::CFesData *pFesData);

	void InitLinearTransform(CQuoteItem *pItem, QMap<QString, CQuoteItem *> &pGroup, Config::CFesData *pFesData);
	bool InitTransformGroup(CQuoteItem *pItem, Config::CTransformGroup *pGroup);
	bool InitNonTransformGroup(CQuoteItem *pItem, Config::CTransformGroup *pGroup);

	void InitNonlinearTransform(CQuoteItem *pItem, QMap<QString, CQuoteItem *> &pGroup, Config::CFesData *pFesData);

	//根据前置文件初始化报警
	void InitAlarm(CQuoteItem *pItem, Config::CFesData *pFesData);

	void InitAIAlarm(CQuoteItem *pItem, QMap<QString, CQuoteItem *> &pGroup, Config::CFesData *pFesData);
	bool InitAIAlarmGroup(CQuoteItem *pItem, Config::CAnalogAlarmGroup *pGroup);
	void InitAIOfflimit(CQuoteItem *pItem, std::vector<Config::CAinAlarmLimit *> &m_arrAlarmOfflimit);

	void InitDIAlarm(CQuoteItem *pItem, QMap<QString, CQuoteItem *> &pGroup, Config::CFesData *pFesData);
	bool InitDIAlarmGroup(CQuoteItem *pItem, Config::CDigitalAlarmGroup *pGroup);
	void InitDIOfflimit(CQuoteItem *pItem, std::vector<Config::CDinAlarmLimit *> &m_arrOfflimit);


	//根据前置文件初始化变量
	void InitVariable(CQuoteItem *pItem, Config::CFesData *pFesData);
	void InitSystemVariable(CQuoteItem *pItem, Config::CFesData *pFesData);
	void InitUserVariable(CQuoteItem *pItem, Config::CFesData *pFesData);
	bool InitUserVariableGroup(CQuoteItem *pItem, Config::CUserVariableGroup *pGroup);

	QString FindChannelGroup(QStandardItem *pItem);

	void forEach(QMap<QString, QStandardItem *> map, QAbstractItemModel* model, QModelIndex parent = QModelIndex());

	//配置前置
	void ConfigFes(QModelIndex &index);

	void CreateFesUi(Config::CFesConfigData *m_pConfig);
	void CreateFesGroupUi(Config::CFesGroup *pFesGroup, CQuoteItem *pItem);

	//bool SaveFesFile(QString &strFileName, Config::CFesData *pFes, const QString & szRoot);
	//bool SaveChannel(QXmlStreamWriter &writer, Config::CFesData *pFes);
	//bool SaveDevice(QXmlStreamWriter &writer, Config::CFesData *pFes);
	//
	//bool SaveData(QXmlStreamWriter &writer, Config::CFesData *pFes);
	//bool SaveAI(QXmlStreamWriter &writer, Config::CFesData *pFes);
	//bool SaveDI(QXmlStreamWriter &writer, Config::CFesData *pFes);
	//bool SaveAO(QXmlStreamWriter &writer, Config::CFesData *pFes);
	//bool SaveDO(QXmlStreamWriter &writer, Config::CFesData *pFes);

	//bool Savetransform(QXmlStreamWriter &writer, Config::CFesData *pFes);
	//bool SaveLinear(QXmlStreamWriter &writer, Config::CFesData *pFes);
	//bool SaveNonlinear(QXmlStreamWriter &writer, Config::CFesData *pFes);

	//bool SaveAlarm(QXmlStreamWriter &writer, Config::CFesData *pFes);
	//bool SaveAnalogAlarm(QXmlStreamWriter &writer, Config::CFesData *pFes);
	//bool SaveDigtalAlarm(QXmlStreamWriter &writer, Config::CFesData *pFes);


	void DeleteFesModuleWidgetInTabWidget(QModelIndex &index);
	bool Delete_FesForwardingChannelWidget_InTabWidget(QModelIndex &index);
	bool Delete_FesAlarm_Scale_Varaible_Widget_InTabWidget(QModelIndex &index);
private slots:
	void showMouseRightButton(const QPoint &point);

	void itemChanged(QStandardItem *pItem);

	void Save();

	void expanded(const QModelIndex &index);
	void collapsed(const QModelIndex &index);
	void doubleClicked(const QModelIndex &index);
private:
	QStandardItemModel *m_pModel;

	CTreeWidget *m_pTree;

	CQuoteItem *m_pTopFesItem;

	CQuoteItem *m_pTopProjectItem;

	IMainWindow *m_pUi;

	CTabWidget *m_pTabWidget;

	CChannel *m_pChannel;

	CAlarm *m_pAlarm;

	CScale *m_pScale;

	CVariable *m_pVariable;

	CHisSave *m_pHisSave;

	CPdr *m_pPdr;
	CForwarding *m_pForwarding;

	//CXmlReader *m_pXmlReader;

	//前置数据

	std::vector<Config::CFesData *> m_arrFes;
	/*std::vector<Config::CChannelData *> m_vecChannel;
	std::vector<Config::CLinearTransform *> m_arrLinear;
	std::vector<Config::CNonlinearTransform *> m_arrNonLinear;
	std::vector<Config::CAnalogAlarm *> m_arrAIAlarm;
	std::vector<Config::CDigtalAlarm *> m_arrDIAlarm;*/


	QMap<QString, QStandardItem *> m_arrChannelMap;

	//遍历通道
	QMap<QString, QStandardItem *> m_arrIteChannelMap;

	//COutputMessagerClient *m_pClient;

	IMainModuleInterface *m_pCore;

	//Config::CFesData *m_pFesData;
	Config::CFesConfigData *m_pConfig;

	//查找字符串内存池
	std::unordered_map<std::string, int32u> *m_pHash;
	std::vector<std::string> *m_pStringPoolVec;
	int32u *m_pDescStringPoolOccNo;
};

#endif // CFESMODE_H
