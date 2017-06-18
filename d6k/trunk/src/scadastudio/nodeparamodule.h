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

	// �����ļ���д
	virtual bool SaveProject(QDomDocument *pXml, QDomElement *pRoot, const QString & szRoot);
	virtual bool LoadProject(QDomDocument *pXml, QDomElement *pRoot, const QString & szRoot);
	// �رչ����ļ�
	virtual void CloseProject(QDomDocument *pXml, QDomElement *pRoot, const QString & szRoot, bool bSaveFlag);
	// �½�һ���յĹ����ļ�
	virtual void CreateNewProject(QDomDocument *pXml, QDomElement *pRoot, const QString & szRoot, CQuoteItem *pTopItem);

	// ����ģ��ʵ�֣�����ģ�����
	// �������жϣ�ֻҪ���κ�һ����ģ��ġ��ࡱ��־���ڣ����Ƴ��Ի����ǡ���ȡ����������ǣ�����ģ�����������ģ��ı���ӿ�
	virtual bool GetModifyFlag() const;

	virtual bool Check()
	{
		return true;
	}

	virtual bool ChangeTagNameNodify(QString &tagName, QString &lastTagName, int nType);
	bool ModifyConfig(QString &tagName, QString &lastTagName, Config::CNodeGroup *pGroup);


	//�ַ��������ڴ��
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
	//�����ַ����ڴ��
	std::unordered_map<std::string, int32u> *m_pHash;
	std::vector<std::string> *m_pStringPoolVec;
	int32u *m_pDescStringPoolOccNo;
};

#endif // NODEPARAMODEL_H
