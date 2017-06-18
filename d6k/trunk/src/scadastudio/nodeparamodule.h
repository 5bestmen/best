#ifndef NODEPARAMODEL_H
#define NODEPARAMODEL_H

#include <QObject>
#include "scadastudio/imodule.h"

class IMainModuleInterface;
class IMainWindow;

namespace Config
{
	class CNodeConfigData;
	class CNodeGroup;
}

class QStandardItem;

class CNodeParaModule : public QObject, public IModule
{
	Q_OBJECT

public:
	CNodeParaModule();
	~CNodeParaModule();

	void Init(IMainModuleInterface *pCore);
	void UnInit()
	{

	}

	// 工程文件读写
	virtual bool SaveProject(QDomDocument *pXml, QDomElement *pRoot, const QString & szRoot);
	virtual bool LoadProject(QDomDocument *pXml, QDomElement *pRoot, const QString & szRoot);
	// 关闭工程文件
	virtual void CloseProject(QDomDocument *pXml, QDomElement *pRoot, const QString & szRoot, bool bSaveFlag);
	// 新建一个空的工程文件
	virtual void CreateNewProject(QDomDocument *pXml, QDomElement *pRoot, const QString & szRoot, CQuoteItem *pTopItem);

	// 由子模块实现，由主模块调用
	// 主程序判断，只要有任何一个子模块的“脏”标志存在，就推出对话框《是、否、取消》，如果是，则主模块调用所有子模块的保存接口
	virtual bool GetModifyFlag() const;

	virtual bool Check()
	{
		return true;
	}

	virtual bool ChangeTagNameNodify(QString &tagName, QString &lastTagName, int nType);
	bool ModifyConfig(QString &tagName, QString &lastTagName, Config::CNodeGroup *pGroup);


	//字符串描述内存池
	virtual bool GetDesPoolArr(std::unordered_map<std::string, int32u> *pHash, std::vector<std::string> *pStringPoolVec, int32u *pDescStringPoolOccNo)
	{
		Q_UNUSED(pHash);
		Q_UNUSED(pStringPoolVec);
		Q_UNUSED(pDescStringPoolOccNo);

		Q_ASSERT(pHash);
		if (!pHash)
		{
			return false;
		}

		Q_ASSERT(pStringPoolVec);
		if (!pStringPoolVec)
		{
			return false;
		}

		Q_ASSERT(pDescStringPoolOccNo);
		if (!pDescStringPoolOccNo)
		{
			return false;
		}

		m_pHash = pHash;
		m_pStringPoolVec = pStringPoolVec;
		m_pDescStringPoolOccNo = pDescStringPoolOccNo;

		return true;
	}

	//fes tagname -> node tagname
	virtual std::string GetNodeNameByFesConfigName(std::string &szFesCongfigName);
	//node tagname -> fes tagname
	virtual std::string GetFesConfigNameByNodeName(std::string &szFesNodeName);

	void TestGetFesConfigName();

public slots:
	void DoubleClicked(const QModelIndex &index);
	void ShowMouseRightButton(const QPoint &point);
	void ItemChanged(QStandardItem *item);

private:
	void AddNodeFesGroup(const QModelIndex &index);
	void AddNodeFesItem(const QModelIndex &index);
	void AddNodeWorkstationGroup(const QModelIndex &index);
	void AddNodeWorkstationItem(const QModelIndex &index);
	void CreateUi();
	void CreateFesNode(CQuoteItem *pItem, Config::CNodeGroup *pGroup);
	void CreateWorkstationNode(CQuoteItem *pItem, Config::CNodeGroup *pGroup);

	void DeleteFesNodeGroup(QModelIndex &index);
	void DeleteFesNodeItem(QModelIndex &index);

	void DeleteWorkstationNodeGroup(QModelIndex &index);
	void DeleteWorkstationNodeItem(QModelIndex &index);
private:
	IMainWindow *m_pUi;
	IMainModuleInterface *m_pCore;
	Config::CNodeConfigData *m_pNodeData;
	//查找字符串内存池
	std::unordered_map<std::string, int32u> *m_pHash;
	std::vector<std::string> *m_pStringPoolVec;
	int32u *m_pDescStringPoolOccNo;
};

#endif // NODEPARAMODEL_H
