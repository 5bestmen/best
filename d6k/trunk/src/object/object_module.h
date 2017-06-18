#ifndef OBJECT_MODULE_H
#define OBJECT_MODULE_H

#include <memory> 

#include <QObject>
#include <QActionGroup>
#include <QStandardItem>

#include "object_global.h"
#include "scadastudio/imodule.h"

#include "treeobject.h"
#include "widgetobject.h"
#include "datainfo.h"
#include "../src/equipmentmodel/modelinfo.h"

class IMainWindow; 
class QTranslator; 
class QDomDocument;
 

class CObjectApi;

CObjectApi *GetModuleApi();
IMainWindow *GetMainWindow();

class CObjectApi : public QObject,public IModule
{
	Q_OBJECT
public:
	CObjectApi();
	virtual ~CObjectApi();

	void Init(IMainModuleInterface *pUi);

	void UnInit();

    // ����ģ������
    bool LoadModelConfig();

    void CreateConnections();   // �����źŲ�
public:
	IMainWindow *GetMainWindow()
	{
		return m_pUi;
	}
	
	virtual bool SaveProject(QDomDocument *pXml, QDomElement *pRoot, const QString & szRoot);
	virtual bool LoadProject(QDomDocument *pXml, QDomElement *pRoot, const QString & szRoot);

	// �رչ����ļ�
	virtual void CloseProject(QDomDocument *pXml, QDomElement *pRoot, const QString & szRoot , bool bSaveFlag);
	// �½�һ���յĹ����ļ�
	virtual void CreateNewProject(QDomDocument *pXml, QDomElement *pRoot, const QString & szRoot, CQuoteItem *pTopItem);

	virtual bool GetModifyFlag()const
	{
		return true;
	}
	virtual bool Check()
	{
		return true;
	}

	virtual bool GetDesPoolArr(std::unordered_map<std::string, int32u> *pHash, std::vector<std::string> *pStringPoolVec, int32u *pDescStringPoolOccNo)
	{
		return true;
	}

	virtual bool ChangeTagNameNodify(QString &tagName, QString &lastTagName)
	{
		return true;
	}

	void InitObjectRootTreeItem();
	bool LogMsg(const char *szLogTxt, int nLevel) ;

public:

    // ����object�ڵ�
    bool SaveObjectNode(QDomDocument* pDom, QDomElement* pElement);

    // ����object�ӽڵ�
    bool SaveChildNode(QDomDocument* pDom, QDomElement* pElement, CObjectGroup* pObjGrp);

    // ����object��Ϣ
    bool SaveObjectInfo(QDomDocument* pDom, QDomElement* pElement, QString szRoot);

    // ��������Ϣ
    bool SaveGroupInfo(QDomDocument* pDom, QDomElement* pElement, const QString strPath, CObjectGroup* pObjGrp);

    // ����item��Ϣ
    bool SaveItemInfo(QXmlStreamWriter& writer, const QString strPath, CObjectItem* pObjItem);

    // ���ö���·��
    void SetObjectPath(const QString& path);

    bool LoadObjectNode(QDomElement& domElement, CQuoteItem* pItem, CObjectGroup* pObjGrp);
    bool LoadGroupNode(QDomElement& domElement, CQuoteItem* pItem, CObjectGroup* pObjGrp);
    bool LoadItemNode(QDomElement& domElement, CQuoteItem* pItem, CObjectGroup* pObjGrp);

protected:
	void ShowRootMenu(QModelIndex &index);

private slots:
    void Slot_OnAddGroup(QModelIndex& index);

    void Slot_OnAddItem(QModelIndex& index);

    void Slot_OnDelGroup(QModelIndex& index);

    void Slot_OnDelItem(QModelIndex& index);

    // ���item���ļ��м���
    void Slot_LoadItem(const CObjectItem* pObjectItem, const QModelIndex& index);

private:

    void LoadData(CObjectItem* &pObjItem, CModelInfo& data)
    {
        if (&pObjItem == nullptr)
        {
            return;
        }

        pObjItem->m_strGuid = data.m_strGuid;
        
        for each (auto i in data.m_mapContentInfo)
        {
            CItemInfo itemInfo;
            itemInfo.m_strName = i.m_strName.c_str();
            itemInfo.m_strDiscribe = i.m_strDescribe.c_str();
            itemInfo.m_nType = i.m_nDataType;
            itemInfo.m_bRWAttr = i.m_bReadWrite;
            itemInfo.m_strRelationPoint = QStringLiteral("��ť");

            pObjItem->m_vecTableInfo.push_back(itemInfo);
        }
    }

    // ��ȡ����root�ڵ�
    QStandardItem* GetObjectRootNode() const;
    
    // ��ȡ����Ϣ
    CObjectGroup* GetGroupInfo(const QModelIndex &index);

    // ��ȡ����Ϣ
    CObjectGroup* GetGroupInfoFromItem(const QModelIndex &index);

    // ��ȡitem��Ϣ
    CObjectItem* GetItemInfo(const QModelIndex &index);

private:
	IMainWindow *m_pUi;
	IMainModuleInterface * m_pMainModuleInterface;


	int CallBackBeforeCloseTab(void *pData);
	
	enum 
	{
		USER_DATA_ROLE = Qt::UserRole+100,
	};
public:
	
private:	

	std::shared_ptr<QTranslator> m_pTran;

private:
    CModelLib* m_pModelLib; // ԭʼģ��

    // ģ����
    QSet<QString> m_strModelName;

    // �������
    CObjectMgr m_objMgr;

    // ����·����
    QString m_strObjectPath;

    // �������
    static int m_nTreeDepth;
private:
    CObjectTree* m_pObjectTree;     // ���οؼ�
    CObjectWidget* m_pObjectWidget; // tab�ؼ�
};



#endif // GRAPH_H
