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

    // ����Ϊÿ��ģ�����ʵ�ֵĽӿ�
    virtual void Init(IMainModuleInterface *pCore);

    virtual void UnInit();

    // �����ļ���д���˽ӿڣ�����������proj�����ļ������⣬��ģ����������ݵı���Ҳ�ڸýӿ���ʵ�֣�
    virtual bool SaveProject(QDomDocument *pXml, QDomElement *pRoot, const QString & szRoot);
    virtual bool LoadProject(QDomDocument *pXml, QDomElement *pRoot, const QString & szRoot);
    // �رչ����ļ�
    virtual void CloseProject(QDomDocument *pXml, QDomElement *pRoot, const QString & szRoot, bool bSaveFlag);
    // �½�һ���յĹ����ļ�
    virtual void CreateNewProject(QDomDocument *pXml, QDomElement *pRoot, const QString & szRoot, CQuoteItem *pTopItem);

    // ����ģ��ʵ�֣�����ģ�����
    // �������жϣ�ֻҪ���κ�һ����ģ��ġ��ࡱ��־���ڣ����Ƴ��Ի����ǡ���ȡ����������ǣ�����ģ�����������ģ��ı���ӿ�
    virtual bool GetModifyFlag() const;

    virtual bool Check();
    //����project����
    void SaveLogicDataToXml(std::shared_ptr<CNodeLogicContainer> pNodeContainer);
    //�����ļ�������Ŀ¼
    void CreateCatalog(const QString &strLogicPath);
    //save mast
    void SaveLogicMastData(std::shared_ptr<CTaskContainer> tTaskContainer);
    //save fast
    void SaveLogicFastData(std::shared_ptr<CTaskContainer> tTaskContainer);
    //save aux
    void SaveLogicAuxData(std::shared_ptr<CTaskContainer> tTaskContainer);
    //�����¹���
    void CreateNewTask(CQuoteItem *pLogicItem, std::shared_ptr<CNodeLogicContainer> pTaskModel);
    //��װmast����
    void CreateMaskItem(CQuoteItem *pLogicItem, std::shared_ptr<CTaskContainer> tModelData);
    //��װfast����
    void CreateFastItem(CQuoteItem *pLogicItem, std::shared_ptr<CTaskContainer> tModelData);
    //��װaux����
    void CreateAuxItem(CQuoteItem *pLogicItem, std::shared_ptr<CTaskContainer> tModelData);
    //
    void DeleteDir(const QString &strDir);

    //���湤���ļ�
    void SaveLogicData(QDomDocument *pXml, QDomElement *pRoot);
    //
    void SaveLogicProjectData(QDomDocument *pXml, QDomElement &logicDom);
    //
    void SaveNodeProjectData(QDomDocument *pXml, QDomElement &nodeDom, std::shared_ptr<CTaskContainer> ObjectContainer, const QString &strNodeName);
    //�ظ����Ƽ��
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
    //tree  �һ�
    void Slot_CustomContextMenuRequested(const QPoint &tPoint);

    //�����¶�
    void Slot_CreateNewSeg();
    //mast attr
    void Slot_MastAttr();
    //fast attr
    void Slot_FastAttr();
    //aux attr
    void Slot_AuxAttr();
    //project attr
    void Slot_ProjectAttr();
    //ɾ��tag
    void Slot_DeleteTag();
    //
    void Slot_ProjectConform();

    //˫����
    void Slot_DoubleClickTreeItem(const QModelIndex &mIndex);

    //�ر�tab
    void Slot_CloseTabWidget(QWidget* tCloseWgt);
    //�����µ�task
    void Slot_CreateNewTask();
    //
    void Slot_TreeItemChange(QStandardItem * pItem);
    //
    void Slot_UpdateAttr();
    //
    void Slot_DeleteSegItem();
    //������
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
    //fbdѡ�����
    CFdbItemChooseWgt *m_pFdbItemChooseWgt;
    //tabwidget
    CTabWidget *m_pTabWgt;
    //
    QWidget *m_pRightWgt;
    //�߼��༭���������ļ�
    CXmlAnalyseLogic *m_pLogXml;
    //
    CQuoteItem *m_pProjectItem;
    //����
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
    //�߼��༭model
    CLogicContainter *m_pLogincModelContainter;
    //ӳ��  index  ��task����
     QMap<QModelIndex, std::shared_ptr<CNodeLogicContainer> > m_mapIndexNode;
     //����ӳ��   index  �� mast fast aux
     QMap<QModelIndex, std::shared_ptr<CTaskContainer> > m_mapIndexTask;
     //seg ��srӳ��  index ��seg  ��  sr
     QMap<QModelIndex, std::shared_ptr<CLogicObjectModel>> m_mapIndexSegOrSr;
    //
    //���ݱ���
     CLogicXmlWriter *m_pSaveXmlFile;
     //
     QDomElement *m_pLogicDom;
     //�޸�����ǰ��name
     QString m_strOldName;
};

#endif // LOGIC_MODULE_H
