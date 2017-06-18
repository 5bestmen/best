#ifndef LOGIC_MODULE_H
#define LOGIC_MODULE_H

#include <QModelIndex>
#include "logic_global.h"
#include "scadastudio/imodule.h"
#include "seqdefine.h"
#include "logiccontainter.h"

class CLogic_Module;
class IMainModuleInterface;
class IMainWindow;

CLogic_Module *GetModuleApi();
//IMainWindow * GetMainWindow();

class CAttrWgt;
class CPrjectAtrrWgt;
class CFdbOrderScene;
class CFdbOrderView;
class CFdbItemChooseWgt;
class CTabWidget;
class CXmlAnalyseLogic;
class QStandardItem;
class CLogicContainter;
class CLogicXmlWriter;


IMainWindow *GetMainWindow();
IMainModuleInterface *GetIMainInterface();

class  CLogic_Module : public QObject, public IModule
{
    Q_OBJECT
public:
    CLogic_Module();
    ~CLogic_Module();

    // 以下为每个模块必须实现的接口
    virtual void Init(IMainModuleInterface *pCore);

    virtual void UnInit();

    // 工程文件读写（此接口，不仅仅保存proj工程文件，另外，子模块的所有内容的保存也在该接口内实现）
    virtual bool SaveProject(QDomDocument *pXml, QDomElement *pRoot, const QString & szRoot);
    virtual bool LoadProject(QDomDocument *pXml, QDomElement *pRoot, const QString & szRoot);
    // 关闭工程文件
    virtual void CloseProject(QDomDocument *pXml, QDomElement *pRoot, const QString & szRoot, bool bSaveFlag);
    // 新建一个空的工程文件
    virtual void CreateNewProject(QDomDocument *pXml, QDomElement *pRoot, const QString & szRoot, CQuoteItem *pTopItem);

    // 由子模块实现，由主模块调用
    // 主程序判断，只要有任何一个子模块的“脏”标志存在，就推出对话框《是、否、取消》，如果是，则主模块调用所有子模块的保存接口
    virtual bool GetModifyFlag() const;

    virtual bool Check();
    //保存project数据
    void SaveLogicDataToXml(std::shared_ptr<CNodeLogicContainer> pNodeContainer);
    //创建文件创房的目录
    void CreateCatalog(const QString &strLogicPath);
    //save mast
    void SaveLogicMastData(std::shared_ptr<CTaskContainer> tTaskContainer);
    //save fast
    void SaveLogicFastData(std::shared_ptr<CTaskContainer> tTaskContainer);
    //save aux
    void SaveLogicAuxData(std::shared_ptr<CTaskContainer> tTaskContainer);
    //创建新工程
    void CreateNewTask(CQuoteItem *pLogicItem, std::shared_ptr<CNodeLogicContainer> pTaskModel);
    //组装mast数据
    void CreateMaskItem(CQuoteItem *pLogicItem, std::shared_ptr<CTaskContainer> tModelData);
    //组装fast数据
    void CreateFastItem(CQuoteItem *pLogicItem, std::shared_ptr<CTaskContainer> tModelData);
    //组装aux数据
    void CreateAuxItem(CQuoteItem *pLogicItem, std::shared_ptr<CTaskContainer> tModelData);
    //
    void DeleteDir(const QString &strDir);

    //保存工程文件
    void SaveLogicData(QDomDocument *pXml, QDomElement *pRoot);
    //
    void SaveLogicProjectData(QDomDocument *pXml, QDomElement &logicDom);
    //
    void SaveNodeProjectData(QDomDocument *pXml, QDomElement &nodeDom, std::shared_ptr<CTaskContainer> ObjectContainer, const QString &strNodeName);
    //重复名称检查
    bool CheckNameLegailty(QStandardItem *pParent, const QString strName);
    //
    bool CheckRenameLegailty(QStandardItem *pParent, QStandardItem *pCurrent, const QString strName);

	virtual bool  ChangeTagNameNodify(QString &tagName, QString &lastTagName);

	virtual bool GetDesPoolArr(std::unordered_map<std::string, int32u> *pHash, std::vector<std::string> *pStringPoolVec, int32u *pDescStringPoolOccNo) 
	{
		return true;
	}

	IMainModuleInterface *getIMainInterface()
	{
		return m_pCore;
	}

	virtual bool ChangeTagNameNodify(QString &, QString &, int)
	{
		return true;
	}

public slots:
    //tree  右击
    void Slot_CustomContextMenuRequested(const QPoint &tPoint);

    //创建新段
    void Slot_CreateNewSeg();
    //mast attr
    void Slot_MastAttr();
    //fast attr
    void Slot_FastAttr();
    //aux attr
    void Slot_AuxAttr();
    //project attr
    void Slot_ProjectAttr();
    //删除tag
    void Slot_DeleteTag();
    //
    void Slot_ProjectConform();

    //双击打开
    void Slot_DoubleClickTreeItem(const QModelIndex &mIndex);

    //关闭tab
    void Slot_CloseTabWidget(QWidget* tCloseWgt);
    //创建新的task
    void Slot_CreateNewTask();
    //
    void Slot_TreeItemChange(QStandardItem * pItem);
    //
    void Slot_UpdateAttr();
    //
    void Slot_DeleteSegItem();
    //重命名
    void Slot_RenameTreeItem();

public:
    IMainWindow *GetMainWindow()
    {
        return m_pUi;
    }

private:
    IMainWindow *m_pUi;
    IMainModuleInterface * m_pCore;

    //project
    CPrjectAtrrWgt *m_pProjectWgt;

    //
    QMap<QModelIndex, QWidget*> m_mapItemWgt;
    //fbd选择界面
    CFdbItemChooseWgt *m_pFdbItemChooseWgt;
    //tabwidget
    CTabWidget *m_pTabWgt;
    //
    QWidget *m_pRightWgt;
    //逻辑编辑工程配置文件
    CXmlAnalyseLogic *m_pLogXml;
    //
    CQuoteItem *m_pProjectItem;
    //工程
    //LOGICDETAIL m_logincDetails;
    //mast seg
    QString m_strMastSeg;
    //mast sr
    QString m_strMastSr;
    //fast seg
    QString m_strFastSeg;
    //fast sr
    QString m_strFastSr;
    //auxo seg
    QString m_strAux0Seg;
    //aux0 sr
    QString m_strAux0Sr;
    //event Timer
    //QString m_strEventTimer;
    //逻辑编辑model
    CLogicContainter *m_pLogincModelContainter;
    //映射  index  到task工程
     QMap<QModelIndex, std::shared_ptr<CNodeLogicContainer> > m_mapIndexNode;
     //任务映射   index  到 mast fast aux
     QMap<QModelIndex, std::shared_ptr<CTaskContainer> > m_mapIndexTask;
     //seg 或sr映射  index 到seg  或  sr
     QMap<QModelIndex, std::shared_ptr<CLogicObjectModel>> m_mapIndexSegOrSr;
    //
    //数据保存
     CLogicXmlWriter *m_pSaveXmlFile;
     //
     QDomElement *m_pLogicDom;
     //修改名称前的name
     QString m_strOldName;
};

#endif // LOGIC_MODULE_H
