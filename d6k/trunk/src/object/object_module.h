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

    // 加载模型配置
    bool LoadModelConfig();

    void CreateConnections();   // 创建信号槽
public:
	IMainWindow *GetMainWindow()
	{
		return m_pUi;
	}
	
	virtual bool SaveProject(QDomDocument *pXml, QDomElement *pRoot, const QString & szRoot);
	virtual bool LoadProject(QDomDocument *pXml, QDomElement *pRoot, const QString & szRoot);

	// 关闭工程文件
	virtual void CloseProject(QDomDocument *pXml, QDomElement *pRoot, const QString & szRoot , bool bSaveFlag);
	// 新建一个空的工程文件
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

    // 保存object节点
    bool SaveObjectNode(QDomDocument* pDom, QDomElement* pElement);

    // 保存object子节点
    bool SaveChildNode(QDomDocument* pDom, QDomElement* pElement, CObjectGroup* pObjGrp);

    // 保存object信息
    bool SaveObjectInfo(QDomDocument* pDom, QDomElement* pElement, QString szRoot);

    // 保存组信息
    bool SaveGroupInfo(QDomDocument* pDom, QDomElement* pElement, const QString strPath, CObjectGroup* pObjGrp);

    // 保存item信息
    bool SaveItemInfo(QXmlStreamWriter& writer, const QString strPath, CObjectItem* pObjItem);

    // 设置对象路径
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

    // 点击item从文件中加载
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
            itemInfo.m_strRelationPoint = QStringLiteral("按钮");

            pObjItem->m_vecTableInfo.push_back(itemInfo);
        }
    }

    // 获取对象root节点
    QStandardItem* GetObjectRootNode() const;
    
    // 获取组信息
    CObjectGroup* GetGroupInfo(const QModelIndex &index);

    // 获取组信息
    CObjectGroup* GetGroupInfoFromItem(const QModelIndex &index);

    // 获取item信息
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
    CModelLib* m_pModelLib; // 原始模型

    // 模型名
    QSet<QString> m_strModelName;

    // 对象管理
    CObjectMgr m_objMgr;

    // 对象路径名
    QString m_strObjectPath;

    // 树的深度
    static int m_nTreeDepth;
private:
    CObjectTree* m_pObjectTree;     // 树形控件
    CObjectWidget* m_pObjectWidget; // tab控件
};



#endif // GRAPH_H
