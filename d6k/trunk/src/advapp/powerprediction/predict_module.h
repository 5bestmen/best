#ifndef PREDICTION_MODULE_H
#define PREDICTION_MODULE_H

#include "scadastudio/imodule.h"
#include "define.h"
#include "predict_datainfo.h"

#include <QObject>

class IMainWindow;
class IMainModuleInterface;
class CTreeWidget;
class CPredictTree;
class QAbstractItemModel;
class QStandardItemModel;
class CQuoteItem;
class CPredictMgr;
class QStandardItem;
class CPredictGroup;
class CPowerPredictApi;

CPowerPredictApi *GetPredictModuleApi();

IMainModuleInterface *GetPreIMainInterface();


class CPowerPredictApi : public QObject, public IModule
{
	Q_OBJECT

public:
	CPowerPredictApi();
	~CPowerPredictApi();
public:
	//初始化工程
	virtual void Init(IMainModuleInterface *pCore);

	//卸载工程
	virtual void UnInit();

	//新建工程
	virtual void CreateNewProject(QDomDocument *pXml, QDomElement *pRoot, const QString &szRoot, CQuoteItem *pTopItem);

	//保存工程
	virtual bool SaveProject(QDomDocument *pXml, QDomElement *pRoot, const QString &szRoot);

	//加载工程
	virtual bool LoadProject(QDomDocument *pXml, QDomElement *pRoot, const QString &szRoot);

	//关闭工程
	virtual void CloseProject(QDomDocument *pXml, QDomElement *pRoot, const QString &szRoot, bool bSaveFlag)
	{
		Q_UNUSED(pXml);
		Q_UNUSED(pRoot);
		Q_UNUSED(szRoot);
		Q_UNUSED(bSaveFlag);
	}

	virtual bool ChangeTagNameNodify(QString &tagName, QString &lastTagName, int nType)
	{
		Q_UNUSED(tagName);
		Q_UNUSED(lastTagName);
		Q_UNUSED(nType);
		return true;
	}

	//获取标记
	virtual bool GetModifyFlag() const
	{
		return true;
	}

	virtual bool Check()
	{
		return true;
	}

	virtual bool GetDesPoolArr(std::unordered_map<std::string, int32u> *pHash, std::vector<std::string> *pStringPoolVec, int32u *pDescStringPoolOccNo)
	{
		Q_UNUSED(pStringPoolVec);
		Q_UNUSED(pHash);
		Q_UNUSED(pDescStringPoolOccNo);
		return true;
	}

	IMainModuleInterface * GetIMainInterface()
	{
		return m_pCore;
	}

public:

	//日志输出
	bool LogMsg(const char* szLogTxt, int nLevel);

	void GetPredictItemMap(const QAbstractItemModel* pModel, const QModelIndex& top, int nType, QVector<QModelIndex>& indexVec);

	void SetPredictPath(const QString& path);

	bool LoadPredictNode(QDomElement& domElm, CQuoteItem* pTtem, CPredictGroup* pPrdtGrp,const QString& szRoot);

	bool LoadGroupNode(QDomElement& domElm, CQuoteItem* pItem, CPredictGroup* pPrdtGroup);

	bool LoadItemNode(QDomElement& domElm, CQuoteItem* pItem, CPredictGroup* pPrdtGrp);

	bool LoadPredictFile(CStationData* pStnData, const QString& fileName, const QString& szRoot);

	void CreatePredictGroupUi(CPredictGroup* pPtdtGroup, CQuoteItem* pItem);

private slots:

	//添加组
	void Slot_AddGroup(QModelIndex& index);
	
	//删除组
	void Slot_DelGroup(QModelIndex& index);

	//添加item
	void Slot_AddItem(QModelIndex& index);

	//删除item
	void Slot_DelItem(QModelIndex& index);

	//添加逆变器
	void Slot_AddInverter(QModelIndex& index);

	//删除逆变器
	void Slot_DelInverter(QModelIndex& index);

	//
	void Slot_ItemChanged(QStandardItem* pItem);


	//加载整个厂站
	//void Slot_OnLoadItem(const CPredictItem* pItem, const QModelIndex& index);


private:

	//主模块指针,Init()内初始化
	IMainModuleInterface *m_pCore;

	//主窗口指针,Init()内初始化 
	IMainWindow *m_pUi;

	//预测管理指针,Init()内初始化
	CPredictMgr *m_pPrdtMgr;

	//左侧树model指针,Init()内初始化
	QStandardItemModel *m_pModel;

	//树形控件指针,Init()内初始化
	CPredictTree* m_pPredictTree;

	//路径
	QString m_strPredictPath;

	//树深度
	static int m_nTreeDepth;


};

#endif  //PREDICTION_MODULE_H