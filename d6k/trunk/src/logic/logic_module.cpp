/*! @file
<PRE>
********************************************************************************
ģ����       :
�ļ���       :  logic_module.cpp
�ļ�ʵ�ֹ��� :  ��̬�������
����         :  ww
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         : <����˵��> �߼��༭
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

********************************************************************************
</PRE>
*  @brief
*  @author ww
*  @version 1.0
*  @date    2017.1.4
*/
#include <QMenu>
#include <QAction>
#include <QDir>
#include <QDebug>
#include <QDomElement>
#include <QDomDocument>
#include <QMessageBox>

#include "logic_module.h"
#define MODULE_LIB

#include "scadastudio/treewidget.h"
#include "scadastudio/tabwidget.h"
#include "scadastudio/icore.h"
#include "scadastudio/iprojmgr.h"
#include "scadastudio/define.h"

#include "scadastudio/uimgr.h"
#include "scadastudio/module_export.h"
#include "scadastudio/quoteItem.h"
#include "scadastudiodefine.h"
#include "seqdefine.h"
#include "attrwgt.h"
#include "prjectatrrwgt.h"

#include "fdborderscene.h"
#include "fdborderview.h"
#include "fdbitemchoosewgt.h"
#include "scadastudiodefine.h"
#include "uimodule/rightdock.h"
#include "xmlanalyselogic.h"
#include "logiccontainter.h"
#include "logicmodel.h"
#include "logicxmlwriter.h"
//#include "vld.h"


static CLogic_Module *s_ptrModule = nullptr;

extern "C" SCADA_EXPORT IModule *CreateModule()
{
    s_ptrModule = new CLogic_Module;
    return s_ptrModule;
}

/*! \fn extern "C" LOGIC_EXPORT void DestroyModule()
*********************************************************************************************************
** \brief DestroyModule
** \details ������Դ
** \return extern " SCADA_EXPORT void
** \author LiJin
** \date 2016��8��18��
** \note
********************************************************************************************************/
extern "C" LOGIC_EXPORT void DestroyModule()
{
    if (s_ptrModule)
    {
        delete s_ptrModule;
        s_ptrModule = nullptr;
    }
}

CLogic_Module * GetModuleApi()
{
    return s_ptrModule;
}

IMainWindow *GetMainWindow()
{
	//	return s_ptrModule->GetCoreUI();
	return s_ptrModule->GetMainWindow();
}

IMainModuleInterface *GetIMainInterface()
{
	return s_ptrModule->getIMainInterface();
}

// IMainWindow *GetMainWindow()
// {
//     return s_ptrModule->GetMainWindow();
// }

CLogic_Module::CLogic_Module()
{

    m_pCore = nullptr;
    m_pUi = nullptr;
    m_pSaveXmlFile = nullptr;
    m_pLogicDom = nullptr;

}

CLogic_Module::~CLogic_Module()
{
    delete m_pProjectWgt;
    delete m_pSaveXmlFile;
    m_pFdbItemChooseWgt->deleteLater();
    m_pLogXml->deleteLater();
    delete m_pLogincModelContainter;
    //m_pSceneView->deleteLater();
}

/*! \fn CLogic_Module::Init(IMainModuleInterface *pCore)
*********************************************************************************************************
** \brief CLogic_Module::Init(IMainModuleInterface *pCore)
** \details ��ʼ��
** \param
** \return
** \author ww
** \date 2017��1��4��
** \note
********************************************************************************************************/
void CLogic_Module::Init(IMainModuleInterface *pCore)
{
    Q_ASSERT(pCore);
    if (!pCore)
    {
        return;
    }

    //ѡ���϶��߼����б�
    m_pFdbItemChooseWgt = new CFdbItemChooseWgt;
    //����
    m_pLogXml = new CXmlAnalyseLogic;
    //����ģ��
    m_pLogincModelContainter = new CLogicContainter;

    //����item
    m_pProjectItem = nullptr;

    //��������
    m_pProjectWgt = new CPrjectAtrrWgt;
    m_pProjectWgt->setWindowTitle(tr("Attr-Project"));
    connect(m_pProjectWgt, SIGNAL(Signal_Conform()), this, SLOT(Slot_ProjectConform()));

    m_pCore = pCore;
    m_pUi = pCore->GetUIMgr();

    m_pTabWgt = m_pUi->GetTabWidget();
    Q_ASSERT(m_pTabWgt);

    connect((QObject *)m_pCore->GetUIMgr()->GetLeftTree(), SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(Slot_CustomContextMenuRequested(const QPoint&)));

    connect(m_pCore->GetUIMgr()->GetLeftTree(), SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(Slot_DoubleClickTreeItem(const QModelIndex &)));

    connect((QStandardItemModel*)m_pUi->GetLeftTree()->model(), SIGNAL(itemChanged(QStandardItem *)), this, SLOT(Slot_TreeItemChange(QStandardItem *)));

    connect(m_pTabWgt, SIGNAL(SendCloseWidget(QWidget*)), this, SLOT(Slot_CloseTabWidget(QWidget*)));

    m_pRightWgt = m_pCore->GetUIMgr()->GetRightDock()->widget();

}

void CLogic_Module::UnInit()
{

}

/*! \fn CLogic_Module::SaveProject(QDomDocument *pXml, QDomElement *pRoot, const QString & szRoot)
*********************************************************************************************************
** \brief CLogic_Module::SaveProject(QDomDocument *pXml, QDomElement *pRoot, const QString & szRoot)
** \details ���湤��
** \param
** \return
** \author ww
** \date 2017��1��4��
** \note
********************************************************************************************************/
bool CLogic_Module::SaveProject(QDomDocument *pXml, QDomElement *pRoot, const QString & szRoot)
{
    //���ݱ������
    if (m_pSaveXmlFile == nullptr)
    {
        m_pSaveXmlFile = new CLogicXmlWriter;
    }

    QString strLogicPath = szRoot + LOGIC_PATH;

    //���ԭ���ļ�
    DeleteDir(strLogicPath);

    QString strFileName = szRoot + LOGICFILE;
    QDir tDir(strLogicPath);
    tDir.mkdir(strLogicPath);

    Q_ASSERT(m_pProjectItem);
    if (m_pProjectItem == nullptr)
    {
        return false;
    }

    for each (std::shared_ptr<CNodeLogicContainer> pNodeContainer in m_pLogincModelContainter->GetNodeContainers())
    {
        //����Ŀ¼
        CreateCatalog(strLogicPath + "/" + pNodeContainer->GetName());
        //�����ļ�
        SaveLogicDataToXml(pNodeContainer);
    }

    //���¹����ļ�
    SaveLogicData(pXml, pRoot);
    return true;
}

//���湤���ļ�
/*! \fn CLogic_Module::SaveLogicData(QDomDocument *pXml, QDomElement *pRoot)
*********************************************************************************************************
** \brief CLogic_Module::SaveLogicData(QDomDocument *pXml, QDomElement *pRoot)
** \details ���湤���ļ�
** \param
** \return
** \author ww
** \date 2017��1��4��
** \note
********************************************************************************************************/
void CLogic_Module::SaveLogicData(QDomDocument *pXml, QDomElement *pRoot)
{
    QDomNode n = pRoot->firstChild();

    // logic�ڵ��Ƿ����
    bool bLogicExists = false;
    while (!n.isNull())
    {
        QDomElement e = n.toElement(); // try to convert the node to an element.
        if (!e.isNull())
        {
            QString strTmp = e.tagName();// the node really is an element.

            //�ҵ�logic�ڵ��������
            if (strTmp.compare("Logic") == 0)
            {
                bLogicExists = true;

                if (e.hasChildNodes())
                {
                    QDomNodeList list = e.childNodes();
                    auto nSize = list.size();

                    while (!list.isEmpty())
                    {
                        QDomNode node = list.at(0);
                        node.parentNode().removeChild(node);
                    }
                }

                //����ǰ�ù����ļ�
                SaveLogicProjectData(pXml, e);

            }
        }

        n = n.nextSibling();
    }

    if (!bLogicExists)
    {
        QDomElement tLogicElement = pXml->createElement("Logic");
        pRoot->appendChild(tLogicElement);
        //����ǰ�ù����ļ�
        SaveLogicProjectData(pXml, tLogicElement);
    }

}

/*! \fn CLogic_Module::SaveLogicProjectData(QDomDocument *pXml, QDomElement &logicDom)
*********************************************************************************************************
** \brief CLogic_Module::SaveLogicProjectData(QDomDocument *pXml, QDomElement &logicDom)
** \details ����logic����������Ϣ
** \param
** \return
** \author ww
** \date 2017��1��4��
** \note
********************************************************************************************************/
void CLogic_Module::SaveLogicProjectData(QDomDocument *pXml, QDomElement &logicDom)
{
    //logic����������Ϣ
    logicDom.setAttribute("Name", m_pLogincModelContainter->GetName());
    logicDom.setAttribute("Protect", m_pLogincModelContainter->GetProtectFlag() ? 1 : 0);
    logicDom.setAttribute("password", m_pLogincModelContainter->GetPassword());
    logicDom.setAttribute("comment", m_pLogincModelContainter->GetComment());

    //�������нڵ�  ����ڵ���Ϣ
    std::vector<std::shared_ptr<CNodeLogicContainer>> tNodeContaninter = m_pLogincModelContainter->GetNodeContainers();
    for each (std::shared_ptr<CNodeLogicContainer> NodeContainer in tNodeContaninter)
    {

        QDomElement tNodeElement = pXml->createElement("LogicNode");
        tNodeElement.setAttribute("TagName", NodeContainer->GetName());
        logicDom.appendChild(tNodeElement);
        //mast
        SaveNodeProjectData(pXml, tNodeElement, NodeContainer->GetMastContainer(),"Mast");
        //fast
        SaveNodeProjectData(pXml, tNodeElement, NodeContainer->GetFastContainer(), "Fast");
        //aux
        SaveNodeProjectData(pXml, tNodeElement, NodeContainer->GetAuxContainer(), "Aux");
    }
}

/*! \fn CLogic_Module::SaveNodeProjectData(QDomDocument *pXml, QDomElement &nodeDom, std::shared_ptr<CTaskContainer> ObjectContainer, const QString &strNodeName)
*********************************************************************************************************
** \brief CLogic_Module::SaveNodeProjectData(QDomDocument *pXml, QDomElement &nodeDom, std::shared_ptr<CTaskContainer> ObjectContainer, const QString &strNodeName)
** \details ��������ģ����Ϣ
** \param
** \return
** \author ww
** \date 2017��1��4��
** \note
********************************************************************************************************/
void CLogic_Module::SaveNodeProjectData(QDomDocument *pXml, QDomElement &nodeDom, std::shared_ptr<CTaskContainer> ObjectContainer, const QString &strNodeName)
{
    //ģ����Ϣ
    QDomElement tDomElement = pXml->createElement(strNodeName);
    tDomElement.setAttribute("RepeatType", ObjectContainer->GetCycleType());
    tDomElement.setAttribute("MSec", ObjectContainer->GetMsec());
    tDomElement.setAttribute("comment", ObjectContainer->GetComment());

    nodeDom.appendChild(tDomElement);

    //�������ж���Ϣ
    for each (std::shared_ptr<CLogicObjectModel> pObjectModel in ObjectContainer->GetSegModels())
    {
        QDomElement tSegElement = pXml->createElement("Seg");
        
        QDomText domText = pXml->createTextNode(pObjectModel->GetName());
        tSegElement.appendChild(domText);

        tDomElement.appendChild(tSegElement);
    }
    
    //��������sr����Ϣ
    for each (std::shared_ptr<CLogicObjectModel> pObjectModel in ObjectContainer->GetSrModels())
    {
        QDomElement tSegElement = pXml->createElement("SrSeg");

        QDomText domText = pXml->createTextNode(pObjectModel->GetName());
        tSegElement.appendChild(domText);

        tDomElement.appendChild(tSegElement);
    }
}

/*! \fn CLogic_Module::LoadProject(QDomDocument *pXml, QDomElement *pRoot, const QString & szRoot)
*********************************************************************************************************
** \brief CLogic_Module::LoadProject(QDomDocument *pXml, QDomElement *pRoot, const QString & szRoot)
** \details ���ع���
** \param
** \return
** \author ww
** \date 2017��1��4��
** \note
********************************************************************************************************/
bool CLogic_Module::LoadProject(QDomDocument *pXml, QDomElement *pRoot, const QString & szRoot)
{

    QStandardItemModel *pTreeModel = ((QStandardItemModel*)m_pUi->GetLeftTree()->model());
    CQuoteItem *pTopItem = (CQuoteItem *)(pTreeModel->item(0, 0));


    if (m_pLogXml->OpenLogicObj(pXml, pRoot, szRoot, m_pLogincModelContainter))
    {

        //����
        CQuoteItem *pLogicProject = new CQuoteItem(m_pLogincModelContainter->GetName());

        pLogicProject->setData(LOGIC_PROJECT, Qt::UserRole);
        pLogicProject->setIcon(QIcon(":/images/logicmove.png"));
        //pLogicProject->setEditable(false);
        pTopItem->appendRow(pLogicProject);
        m_pProjectItem = pLogicProject;
        //��������
        m_pProjectWgt->SetprojectName(m_pLogincModelContainter->GetName());
        m_pProjectWgt->SetProtectFlag(m_pLogincModelContainter->GetProtectFlag());
        m_pProjectWgt->SetComment(m_pLogincModelContainter->GetComment());
        //


        for each (std::shared_ptr<CNodeLogicContainer> tNodeContainer in m_pLogincModelContainter->GetNodeContainers())
        {
            //Tag
            CQuoteItem *pTag = new CQuoteItem(tNodeContainer->GetName());
            pTag->setData(LOGIC_TAG, Qt::UserRole);
            //pTag->setEditable(true);
            pLogicProject->appendRow(pTag);
            m_mapIndexNode.insert(pTag->index(), tNodeContainer);

            CQuoteItem *pTask = new CQuoteItem(tr("Task"));
            pTask->setData(LOGIC_TASK, Qt::UserRole);
            //pTask->setEditable(false);
            pTag->appendRow(pTask);

            //mast
            CreateMaskItem(pTask,tNodeContainer->GetMastContainer());
            //fast
            CreateFastItem(pTask, tNodeContainer->GetFastContainer());
            //aux
            CreateAuxItem(pTask, tNodeContainer->GetAuxContainer());

        }

    }
    else
    {
        //���û�б���  ���ߴ������� 
        CreateNewProject(pXml, pRoot, szRoot, pTopItem);
    }

    return true;
}

void CLogic_Module::CloseProject(QDomDocument *pXml, QDomElement *pRoot, const QString & szRoot, bool bSaveFlag)
{
    Q_UNUSED(pXml);
    Q_UNUSED(pRoot);
    Q_UNUSED(szRoot);
}

/*! \fn CLogic_Module::CreateNewProject(QDomDocument *pXml, QDomElement *pRoot, const QString & szRoot, CQuoteItem *pTopItem)
*********************************************************************************************************
** \brief CLogic_Module::CreateNewProject(QDomDocument *pXml, QDomElement *pRoot, const QString & szRoot, CQuoteItem *pTopItem)
** \details �����¹���
** \param
** \return
** \author ww
** \date 2017��1��4��
** \note
********************************************************************************************************/
void CLogic_Module::CreateNewProject(QDomDocument *pXml, QDomElement *pRoot, const QString & szRoot, CQuoteItem *pTopItem)
{
    Q_UNUSED(pXml);
    Q_UNUSED(pRoot);
    Q_UNUSED(szRoot);
    m_pProjectItem = nullptr;

    //����
    QString strObjName = tr("Logic");
    CQuoteItem *pLogicProject = new CQuoteItem(strObjName);
    pLogicProject->setData(LOGIC_PROJECT, Qt::UserRole);
    pLogicProject->setIcon(QIcon(":/images/logicmove.png"));
    //pLogicProject->setEditable(false);
    pTopItem->appendRow(pLogicProject);

    m_pProjectItem = pLogicProject;
    //��ֵ
    m_pLogincModelContainter->SetName(strObjName);
    m_pLogincModelContainter->SetProtectFlag(false);


    QString strTagname = tr("Tag0");
    CQuoteItem *pTagname = new CQuoteItem(strTagname);
    pTagname->setData(LOGIC_TAG, Qt::UserRole);
    m_pProjectItem->appendRow(pTagname);
    //pTagname->setFlags(pTagname->flags() | Qt::ItemIsEditable);

    std::shared_ptr<CNodeLogicContainer> pTaskModel = m_pLogincModelContainter->CreateNodeContainer(strTagname);

    m_mapIndexNode.insert(pTagname->index(), pTaskModel);

    CreateNewTask(pTagname, pTaskModel);

   
//     //����
//     QDomElement tDomElement = pXml->createElement("Logic");
//     tDomElement.setAttribute("Name", m_pLogincModelContainter->GetName());
//     tDomElement.setAttribute("Protect", m_pLogincModelContainter->GetProtectFlag() ? 1 : 0);
//     tDomElement.setAttribute("password", m_pLogincModelContainter->GetPassword());
//     tDomElement.setAttribute("comment", m_pLogincModelContainter->GetComment());

//    pRoot->appendChild(tDomElement);
}

//�����¹���
/*! \fn CLogic_Module::CreateNewTask(CQuoteItem *pLogicItem ,std::shared_ptr<CNodeLogicContainer> pTaskModel)
*********************************************************************************************************
** \brief CLogic_Module::CreateNewTask(CQuoteItem *pLogicItem ,std::shared_ptr<CNodeLogicContainer> pTaskModel)
** \details �����¹���
** \param
** \return
** \author ww
** \date 2017��1��4��
** \note
********************************************************************************************************/
void CLogic_Module::CreateNewTask(CQuoteItem *pLogicItem ,std::shared_ptr<CNodeLogicContainer> pTaskModel)
{
    //����
    CQuoteItem *pTask = new CQuoteItem(tr("Task"));
    pTask->setData(LOGIC_TASK, Qt::UserRole);
    //pTask->setEditable(false);
    pLogicItem->appendRow(pTask);

    //mast
    CQuoteItem *pMast = new CQuoteItem(tr("Mast"));
    pMast->setData(LOGIC_MAST, Qt::UserRole);
    

    //pMast->setEditable(false);
    pTask->appendRow(pMast);

    //��������
    std::shared_ptr<CTaskContainer> pMaskContainer = pTaskModel->CreateMastContainer();
    pMaskContainer->SetCycleType(1);
    pMaskContainer->SetMsec(0);
    //����ӳ��
    m_mapIndexTask.insert(pMast->index(), pMaskContainer);

    //��
    CQuoteItem *pSeg = new CQuoteItem(tr("Seg"));
    pSeg->setData(LOGIC_MASTSEG, Qt::UserRole);
    //pSeg->setEditable(false);
    pMast->appendRow(pSeg);

    //sr��
    CQuoteItem *pSrSeg = new CQuoteItem(tr("SR Seg"));
    pSrSeg->setData(LOGIC_MASTSRSEG, Qt::UserRole);
    //pSrSeg->setEditable(false);
    pMast->appendRow(pSrSeg);

    //fast
    CQuoteItem *pFast = new CQuoteItem(tr("FAST"));
    pFast->setData(LOGIC_FAST, Qt::UserRole);
    //pFast->setEditable(false);
    pTask->appendRow(pFast);

    //��������
    std::shared_ptr<CTaskContainer> pFaskContainer = pTaskModel->CreateFastContainer();
    pFaskContainer->SetCycleType(1);
    pFaskContainer->SetMsec(0);

    //����ӳ��
    m_mapIndexTask.insert(pFast->index(), pFaskContainer);


    //fast  ��
    CQuoteItem *pFastSeg = new CQuoteItem(tr("Seg"));
    pFastSeg->setData(LOGIC_FAST_SEG, Qt::UserRole);
    //pFastSeg->setEditable(false);
    pFast->appendRow(pFastSeg);

    //fast sr��
    CQuoteItem *pFastSrSeg = new CQuoteItem(tr("SR Seg"));
    pFastSrSeg->setData(LOGIC_FAST_SRSEG, Qt::UserRole);
    //pFastSrSeg->setEditable(false);
    pFast->appendRow(pFastSrSeg); 

    //aux
    CQuoteItem *pAux0 = new CQuoteItem(tr("Aux"));
    pAux0->setData(LOGIC_AUX0, Qt::UserRole);
    //pAux0->setEditable(false);
    pTask->appendRow(pAux0);

    //��������
    std::shared_ptr<CTaskContainer> pAuxContainer = pTaskModel->CreateAuxContainer();
    pAuxContainer->SetCycleType(1);
    pAuxContainer->SetMsec(0);
    //����ӳ��
    m_mapIndexTask.insert(pAux0->index(), pAuxContainer);


    //aux  ��
    CQuoteItem *pAux0Seg = new CQuoteItem(tr("Seg"));
    pAux0Seg->setData(LOGIC_AUX0SEG, Qt::UserRole);
    //pAux0Seg->setEditable(false);
    pAux0->appendRow(pAux0Seg);

    //aux sr��
    CQuoteItem *pAuxSrSeg = new CQuoteItem(tr("SR Seg"));
    pAuxSrSeg->setData(LOGIC_AUX0_SRSEG, Qt::UserRole);
    //pAuxSrSeg->setEditable(false);
    pAux0->appendRow(pAuxSrSeg);
}


bool CLogic_Module::GetModifyFlag() const
{
    return true;
}

bool CLogic_Module::Check()
{
    return true;
}

/*! \fn CLogic_Module::Slot_CustomContextMenuRequested(const QPoint &tPoint)
*********************************************************************************************************
** \brief CLogic_Module::Slot_CustomContextMenuRequested(const QPoint &tPoint)
** \details �һ�����ʾ�˵�
** \param
** \return
** \author ww
** \date 2017��1��4��
** \note
********************************************************************************************************/
void CLogic_Module::Slot_CustomContextMenuRequested(const QPoint &tPoint)
{
    QModelIndex indexSelect = m_pUi->GetLeftTree()->indexAt(tPoint); 

    QStandardItemModel *pTreeModel = (QStandardItemModel*)m_pUi->GetLeftTree()->model();
    

    int nType = indexSelect.data(Qt::UserRole).toInt();

    //���ݲ�ͬ���ʹ�����ͬ�˵�
    if (nType == LOGIC_MASTSEG || nType == LOGIC_MASTSRSEG|| nType == LOGIC_FAST_SEG || nType == LOGIC_FAST_SRSEG
        || nType == LOGIC_AUX0SEG || nType == LOGIC_AUX0_SRSEG|| nType == LOGIC_TIME_EVENT || nType == IOEVENT)
    {
        //�����ӳ���
        QMenu *pMenu = new QMenu(m_pUi->GetLeftTree());

        QAction *pCreateNewSegAct = new QAction(tr("Create New %1").arg(pTreeModel->data(indexSelect).toString()), m_pUi->GetLeftTree());
        connect(pCreateNewSegAct, SIGNAL(triggered()), this, SLOT(Slot_CreateNewSeg()));


        pMenu->addAction(pCreateNewSegAct);

        pMenu->exec(QCursor::pos());
        pMenu->deleteLater();

    }
    else if (nType == LOGIC_PROJECT)
    {
        //��������
        QMenu *pMenu = new QMenu(m_pUi->GetLeftTree());

        QAction *pAttrAct = new QAction(tr("attribute"), m_pUi->GetLeftTree());
        connect(pAttrAct, SIGNAL(triggered()), this, SLOT(Slot_ProjectAttr()));

        //���ӹ���
        QAction *pCreateTaskAct = new QAction(tr("New"), m_pUi->GetLeftTree());
        connect(pCreateTaskAct, SIGNAL(triggered()), this, SLOT(Slot_CreateNewTask()));


        pMenu->addAction(pAttrAct);
        pMenu->addAction(pCreateTaskAct);

        pMenu->exec(QCursor::pos());
        pMenu->deleteLater();

    }
    else if (nType == LOGIC_TAG)
    {
        //tag
        //��������
        QMenu *pMenu = new QMenu(m_pUi->GetLeftTree());
        //������
        QAction *pRenameAct = new QAction(tr("Rename"), m_pUi->GetLeftTree());
        connect(pRenameAct, SIGNAL(triggered()), this, SLOT(Slot_RenameTreeItem()));
        //ɾ��
        QAction *pDeleteAct = new QAction(tr("Delete"), m_pUi->GetLeftTree());
        connect(pDeleteAct, SIGNAL(triggered()), this, SLOT(Slot_DeleteTag()));

        pMenu->addAction(pRenameAct);
        pMenu->addAction(pDeleteAct);

        pMenu->exec(QCursor::pos());
        pMenu->deleteLater();

    }
    else if (nType == LOGIC_MAST)
    {
        //
        //m_pMastAttrWgt->show();
        //�����ӳ���
        QMenu *pMenu = new QMenu(m_pUi->GetLeftTree());

        QAction *pAttrAct = new QAction(tr("Attribute"), m_pUi->GetLeftTree());
        connect(pAttrAct, SIGNAL(triggered()), this, SLOT(Slot_MastAttr()));

        pMenu->addAction(pAttrAct);

        pMenu->exec(QCursor::pos());
        pMenu->deleteLater();

    }
    else if (nType == LOGIC_FAST)
    {
        //fast
        QMenu *pMenu = new QMenu(m_pUi->GetLeftTree());

        QAction *pAttrAct = new QAction(tr("attribute"), m_pUi->GetLeftTree());
        connect(pAttrAct, SIGNAL(triggered()), this, SLOT(Slot_FastAttr()));

        pMenu->addAction(pAttrAct);

        pMenu->exec(QCursor::pos());
        pMenu->deleteLater();

    }
    else if (nType == LOGIC_AUX0)
    {
        //aux
        QMenu *pMenu = new QMenu(m_pUi->GetLeftTree());

        QAction *pAttrAct = new QAction(tr("attribute"), m_pUi->GetLeftTree());
        connect(pAttrAct, SIGNAL(triggered()), this, SLOT(Slot_AuxAttr()));

        pMenu->addAction(pAttrAct);

        pMenu->exec(QCursor::pos());
        pMenu->deleteLater();

    }
    else if (nType == LOGIC_EDIT_ITEM)
    {
        QMenu *pMenu = new QMenu(m_pUi->GetLeftTree());

        //������
        QAction *pRenameAct = new QAction(tr("Rename"), m_pUi->GetLeftTree());
        connect(pRenameAct, SIGNAL(triggered()), this, SLOT(Slot_RenameTreeItem()));

        QAction *pDeleteAct = new QAction(tr("Delete"), m_pUi->GetLeftTree());
        connect(pDeleteAct, SIGNAL(triggered()), this, SLOT(Slot_DeleteSegItem()));

        pMenu->addAction(pRenameAct);
        pMenu->addAction(pDeleteAct);

        pMenu->exec(QCursor::pos());
        pMenu->deleteLater();
    }
    
}

/*! \fn CLogic_Module::Slot_CreateNewSeg()
*********************************************************************************************************
** \brief CLogic_Module::Slot_CreateNewSeg()
** \details ����seg
** \param
** \return
** \author ww
** \date 2017��1��4��
** \note
********************************************************************************************************/
void CLogic_Module::Slot_CreateNewSeg()
{
    //�ڽ����д���seg �����뵽ģ��������
    QModelIndex indexSelect = m_pUi->GetLeftTree()->currentIndex();
    m_pUi->GetLeftTree()->expand(indexSelect);

    QStandardItemModel *pTreeModel = (QStandardItemModel*)m_pUi->GetLeftTree()->model();

    QStandardItem *pTopItem = (QStandardItem*)pTreeModel->itemFromIndex(indexSelect);

    //m_pUi->GetLeftTree()->setEditTriggers(QAbstractItemView::SelectedClicked);

    //�����¶�
    int nCount = pTopItem->rowCount();

    QString strSrName = pTopItem->parent()->text() + pTopItem->text().remove(" ") + QString::number(nCount);
    //������ƺϷ���
    if (!CheckNameLegailty(pTopItem, strSrName))
    {
        return;
    }
    

    QStandardItem *pSrItem = new QStandardItem(strSrName);

    pSrItem->setData(LOGIC_EDIT_ITEM, Qt::UserRole);

    //pSrItem->setFlags(pSrItem->flags() | Qt::ItemIsEditable);
    pTopItem->appendRow(pSrItem);
    //����ӳ��
    if (pTopItem->data(Qt::UserRole) == LOGIC_MASTSEG || pTopItem->data(Qt::UserRole) == LOGIC_FAST_SEG || pTopItem->data(Qt::UserRole) == LOGIC_AUX0SEG)
    {
        if (pTopItem->parent() != nullptr)
        {
            if (m_mapIndexTask.contains(pTopItem->parent()->index()))
            {
                
                //��������
                std::shared_ptr<CLogicObjectModel> pObjectModel = m_mapIndexTask[pTopItem->parent()->index()]->CreateSegModel(pSrItem->text());
                m_mapIndexSegOrSr.insert(pSrItem->index(),pObjectModel);
            }
        }
    }
    else if (pTopItem->data(Qt::UserRole) == LOGIC_MASTSRSEG || pTopItem->data(Qt::UserRole) == LOGIC_FAST_SRSEG || pTopItem->data(Qt::UserRole) == LOGIC_AUX0_SRSEG)
    {
        if (pTopItem->parent() != nullptr)
        {
            if (m_mapIndexTask.contains(pTopItem->parent()->index()))
            {

                //��������
                std::shared_ptr<CLogicObjectModel> pObjectModel = m_mapIndexTask[pTopItem->parent()->index()]->CreateSrModel(pSrItem->text());
                m_mapIndexSegOrSr.insert(pSrItem->index(), pObjectModel);
            }
        }
    }


}

/*! \fn CLogic_Module::Slot_MastAttr()
*********************************************************************************************************
** \brief CLogic_Module::Slot_MastAttr()
** \details �һ���ʾmast����
** \param
** \return
** \author ww
** \date 2017��1��4��
** \note
********************************************************************************************************/
void CLogic_Module::Slot_MastAttr()
{
    //�ҵ���Ӧ��ģ������  ����ʾ��������
    if (m_mapIndexTask.contains(m_pUi->GetLeftTree()->currentIndex()))
    {
        std::shared_ptr<CTaskContainer> tTaskContainer = m_mapIndexTask[m_pUi->GetLeftTree()->currentIndex()];

        CAttrWgt *pAttrWgt = new CAttrWgt;
        pAttrWgt->SetComment(tTaskContainer->GetComment());
        pAttrWgt->SetCycleType(tTaskContainer->GetCycleType());
        pAttrWgt->SetMsec(tTaskContainer->GetMsec());
        pAttrWgt->SetComboName(m_pUi->GetLeftTree()->model()->data(m_pUi->GetLeftTree()->currentIndex()).toString());

        connect(pAttrWgt, SIGNAL(Signal_Conform()), this, SLOT(Slot_UpdateAttr()));

        pAttrWgt->exec();

        pAttrWgt->deleteLater();

        //m_pMastAttrWgt->exec();

    }
    
}

/*! \fn CLogic_Module::Slot_FastAttr()
*********************************************************************************************************
** \brief CLogic_Module::Slot_FastAttr()
** \details �һ���ʾfast����
** \param
** \return
** \author ww
** \date 2017��1��4��
** \note
********************************************************************************************************/
void CLogic_Module::Slot_FastAttr()
{
    //m_pFastAttrWgt->exec();
    if (m_mapIndexTask.contains(m_pUi->GetLeftTree()->currentIndex()))
    {
        std::shared_ptr<CTaskContainer> tTaskContainer = m_mapIndexTask[m_pUi->GetLeftTree()->currentIndex()];

        CAttrWgt *pAttrWgt = new CAttrWgt;
        pAttrWgt->SetComment(tTaskContainer->GetComment());
        pAttrWgt->SetCycleType(tTaskContainer->GetCycleType());
        pAttrWgt->SetMsec(tTaskContainer->GetMsec());
        pAttrWgt->SetComboName(m_pUi->GetLeftTree()->model()->data(m_pUi->GetLeftTree()->currentIndex()).toString());

        connect(pAttrWgt, SIGNAL(Signal_Conform()), this, SLOT(Slot_UpdateAttr()));

        pAttrWgt->exec();

        pAttrWgt->deleteLater();

        //m_pMastAttrWgt->exec();

    }
}

/*! \fn CLogic_Module::Slot_AuxAttr()
*********************************************************************************************************
** \brief CLogic_Module::Slot_AuxAttr()
** \details �һ���ʾaux����
** \param
** \return
** \author ww
** \date 2017��1��4��
** \note
********************************************************************************************************/
void CLogic_Module::Slot_AuxAttr()
{
    //m_pAuxAttrWgt->exec();
    if (m_mapIndexTask.contains(m_pUi->GetLeftTree()->currentIndex()))
    {
        std::shared_ptr<CTaskContainer> tTaskContainer = m_mapIndexTask[m_pUi->GetLeftTree()->currentIndex()];

        CAttrWgt *pAttrWgt = new CAttrWgt;
        pAttrWgt->SetComment(tTaskContainer->GetComment());
        pAttrWgt->SetCycleType(tTaskContainer->GetCycleType());
        pAttrWgt->SetMsec(tTaskContainer->GetMsec());
        pAttrWgt->SetComboName(m_pUi->GetLeftTree()->model()->data(m_pUi->GetLeftTree()->currentIndex()).toString());

        connect(pAttrWgt, SIGNAL(Signal_Conform()), this, SLOT(Slot_UpdateAttr()));

        pAttrWgt->exec();

        pAttrWgt->deleteLater();

        //m_pMastAttrWgt->exec();

    }
}

/*! \fn CLogic_Module::Slot_ProjectAttr()
*********************************************************************************************************
** \brief CLogic_Module::Slot_ProjectAttr()
** \details �һ���ʾproject����
** \param
** \return
** \author ww
** \date 2017��1��4��
** \note
********************************************************************************************************/
void CLogic_Module::Slot_ProjectAttr()
{
    QModelIndex indexSelect = m_pUi->GetLeftTree()->currentIndex();
    QStandardItemModel *pTreeModel = (QStandardItemModel*)m_pUi->GetLeftTree()->model();
    QStandardItem *pTopItem = (QStandardItem*)pTreeModel->itemFromIndex(indexSelect);

    m_pProjectWgt->SetprojectName(pTopItem->text());
    m_pProjectWgt->exec();

}

/*! \fn CLogic_Module::Slot_ProjectConform()
*********************************************************************************************************
** \brief CLogic_Module::Slot_ProjectConform()
** \details ��project���Ե��ȷ�� ����ģ������
** \param
** \return
** \author ww
** \date 2017��1��4��
** \note
********************************************************************************************************/
void CLogic_Module::Slot_ProjectConform()
{
    QModelIndex indexSelect = m_pUi->GetLeftTree()->currentIndex();
    QStandardItemModel *pTreeModel = (QStandardItemModel*)m_pUi->GetLeftTree()->model();
    QStandardItem *pTopItem = (QStandardItem*)pTreeModel->itemFromIndex(indexSelect);
    //���¹�������
    pTopItem->setText(m_pProjectWgt->GetProjectName());
    //����ģ������
    m_pLogincModelContainter->SetComment(m_pProjectWgt->GetComment());
    m_pLogincModelContainter->SetName(m_pProjectWgt->GetProjectName());
    m_pLogincModelContainter->SetPassword(m_pProjectWgt->GetPassWord());
    m_pLogincModelContainter->SetProtectFlag(m_pProjectWgt->GetCheckFlag());

}

/*! \fn CLogic_Module::Slot_DoubleClickTreeItem(const QModelIndex &mIndex)
*********************************************************************************************************
** \brief CLogic_Module::Slot_DoubleClickTreeItem(const QModelIndex &mIndex)
** \details ˫�������߼�ͼ����
** \param
** \return
** \author ww
** \date 2017��1��4��
** \note
********************************************************************************************************/
void CLogic_Module::Slot_DoubleClickTreeItem(const QModelIndex &mIndex)
{
    QStandardItemModel *pTreeModel = (QStandardItemModel*)m_pUi->GetLeftTree()->model();
    QStandardItem *pClickItem = (QStandardItem*)pTreeModel->itemFromIndex(mIndex);

    //���ݵ�ǰҶ�ӽڵ� �ж��Ƿ��Ƕ�
    if (pClickItem->data(Qt::UserRole) != LOGIC_EDIT_ITEM)
    {
        m_pCore->GetUIMgr()->GetRightDock()->setWidget(m_pRightWgt);
        return;
    }

    if (m_mapItemWgt.contains(mIndex))
    {
        //��tabû�ر�ʱ ֱ��ת����tab
        m_pTabWgt->setCurrentWidget(m_mapItemWgt[mIndex]);
        m_pTabWgt->setTabText(m_pTabWgt->currentIndex(), pClickItem->text());
    }
    else
    {
        //��tab�رպ� ���´�������
        if (m_mapIndexSegOrSr.contains(pClickItem->index()))
        {
            //����������
            CFdbOrderScene *pScene = new CFdbOrderScene(this, m_mapIndexSegOrSr[pClickItem->index()]);
            CFdbOrderView *pView = new CFdbOrderView(pScene);
            //����ģ������
            pScene->UpdateModelData();

            m_mapItemWgt.insert(mIndex, pView);
            m_pTabWgt->addTab(pView, pClickItem->text());
            m_pTabWgt->setCurrentWidget(pView);

        }

    }

    m_pCore->GetUIMgr()->GetRightDock()->setWidget(m_pFdbItemChooseWgt);
}

/*! \fn CLogic_Module::Slot_CloseTabWidget(QWidget* tCloseWgt)
*********************************************************************************************************
** \brief CLogic_Module::Slot_CloseTabWidget(QWidget* tCloseWgt)
** \details �ر�tab ɾ��widget����
** \param
** \return
** \author ww
** \date 2017��1��4��
** \note
********************************************************************************************************/
void CLogic_Module::Slot_CloseTabWidget(QWidget* tCloseWgt)
{
    if (tCloseWgt != NULL)
    {
        m_mapItemWgt.remove(m_mapItemWgt.key(tCloseWgt));
//         m_pTabWgt->removeTab(nIndex);
        tCloseWgt->deleteLater();
    }
}

//�����ļ�������Ŀ¼
/*! \fn CLogic_Module::CreateCatalog(const QString &strLogicPath)
*********************************************************************************************************
** \brief CLogic_Module::CreateCatalog(const QString &strLogicPath)
** \details �����ļ�������Ŀ¼
** \param
** \return
** \author ww
** \date 2017��1��4��
** \note
********************************************************************************************************/
void CLogic_Module::CreateCatalog(const QString &strLogicPath)
{
    QDir tDir(strLogicPath);
    tDir.mkdir(strLogicPath);
    tDir.mkdir(strLogicPath + LOGICMAST);

    m_strMastSeg = strLogicPath + LOGICMASTSEG;

    tDir.mkdir(m_strMastSeg);

    m_strMastSr = strLogicPath + LOGICMASTSR;
    tDir.mkdir(m_strMastSr);


    tDir.mkdir(strLogicPath + LOGICAUX0);

    m_strAux0Seg = strLogicPath + LOGICAUX0SEG;
    tDir.mkdir(m_strAux0Seg);

    m_strAux0Sr = strLogicPath + LOGICAUX0SR;
    tDir.mkdir(m_strAux0Sr);


    tDir.mkdir(strLogicPath + LOGICFAST);

    m_strFastSeg = strLogicPath + LOGICFASTSEG;
    tDir.mkdir(m_strFastSeg);

    m_strFastSr = strLogicPath + LOGICFASTSR;
    tDir.mkdir(m_strFastSr);
}


/*! \fn CLogic_Module::SaveLogicDataToXml(std::shared_ptr<CNodeLogicContainer> pNodeContainer)
*********************************************************************************************************
** \brief CLogic_Module::SaveLogicDataToXml(std::shared_ptr<CNodeLogicContainer> pNodeContainer)
** \details ��������ģ�͵�xml��
** \param
** \return
** \author ww
** \date 2017��1��4��
** \note
********************************************************************************************************/
void CLogic_Module::SaveLogicDataToXml(std::shared_ptr<CNodeLogicContainer> pNodeContainer)
{
    //����mast
    SaveLogicMastData(pNodeContainer->GetMastContainer());
    //����fast
    SaveLogicFastData(pNodeContainer->GetFastContainer());
    //����aux
    SaveLogicAuxData(pNodeContainer->GetAuxContainer());
}

//save mast
/*! \fn CLogic_Module::SaveLogicMastData(std::shared_ptr<CTaskContainer> tTaskContainer)
*********************************************************************************************************
** \brief CLogic_Module::SaveLogicMastData(std::shared_ptr<CTaskContainer> tTaskContainer)
** \details ����mast���ݵ�xml��
** \param
** \return
** \author ww
** \date 2017��1��4��
** \note
********************************************************************************************************/
void CLogic_Module::SaveLogicMastData(std::shared_ptr<CTaskContainer> tTaskContainer)
{
    for each (std::shared_ptr<CLogicObjectModel> tObjectModel in tTaskContainer->GetSegModels())
    {
        //seg
        m_pSaveXmlFile->SaveDataToXml(m_strMastSeg + "/" + tObjectModel->GetName() + ".xml", tObjectModel);
    }

    for each (std::shared_ptr<CLogicObjectModel> tObjectModel in tTaskContainer->GetSrModels())
    {
        //sr
        m_pSaveXmlFile->SaveDataToXml(m_strMastSr + "/" + tObjectModel->GetName() + ".xml", tObjectModel);
    }
}


/*! \fn CLogic_Module::SaveLogicMastData(std::shared_ptr<CTaskContainer> tTaskContainer)
*********************************************************************************************************
** \brief CLogic_Module::SaveLogicMastData(std::shared_ptr<CTaskContainer> tTaskContainer)
** \details ����fast����ģ�͵�xml��
** \param
** \return
** \author ww
** \date 2017��1��4��
** \note
********************************************************************************************************/
void CLogic_Module::SaveLogicFastData(std::shared_ptr<CTaskContainer> tTaskContainer)
{
    for each (std::shared_ptr<CLogicObjectModel> tObjectModel in tTaskContainer->GetSegModels())
    {
        //seg
        m_pSaveXmlFile->SaveDataToXml(m_strFastSeg + "/" + tObjectModel->GetName() + ".xml", tObjectModel);
    }

    for each (std::shared_ptr<CLogicObjectModel> tObjectModel in tTaskContainer->GetSrModels())
    {
        //sr
        m_pSaveXmlFile->SaveDataToXml(m_strFastSr + "/" + tObjectModel->GetName() + ".xml", tObjectModel);
    }
}

/*! \fn CLogic_Module::SaveLogicAuxData(std::shared_ptr<CTaskContainer> tTaskContainer)
*********************************************************************************************************
** \brief CLogic_Module::SaveLogicAuxData(std::shared_ptr<CTaskContainer> tTaskContainer)
** \details ����aux����ģ�͵�xml��
** \param
** \return
** \author ww
** \date 2017��1��4��
** \note
********************************************************************************************************/
void CLogic_Module::SaveLogicAuxData(std::shared_ptr<CTaskContainer> tTaskContainer)
{
    for each (std::shared_ptr<CLogicObjectModel> tObjectModel in tTaskContainer->GetSegModels())
    {
        //seg
        m_pSaveXmlFile->SaveDataToXml(m_strAux0Seg + "/" + tObjectModel->GetName() + ".xml", tObjectModel);
    }

    for each (std::shared_ptr<CLogicObjectModel> tObjectModel in tTaskContainer->GetSrModels())
    {
        //sr
        m_pSaveXmlFile->SaveDataToXml(m_strAux0Sr + "/" + tObjectModel->GetName() + ".xml", tObjectModel);
    }
}

/*! \fn CLogic_Module::Slot_CreateNewTask()
*********************************************************************************************************
** \brief CLogic_Module::Slot_CreateNewTask()
** \details �����¹���
** \param
** \return
** \author ww
** \date 2017��1��4��
** \note
********************************************************************************************************/
void CLogic_Module::Slot_CreateNewTask()
{
    int nTagNum = m_pProjectItem->rowCount();

    QString strTagname = tr("Tag%1").arg(nTagNum);

    if (!CheckNameLegailty(m_pProjectItem,strTagname))
    {
        return;
    }

    CQuoteItem *pTagname = new CQuoteItem(strTagname);
    pTagname->setData(LOGIC_TAG, Qt::UserRole);
    m_pProjectItem->appendRow(pTagname);
    //pTagname->setFlags(pTagname->flags() | Qt::ItemIsEditable);
    //���ݸ��µ�ģ����
    std::shared_ptr<CNodeLogicContainer> pTaskModel = m_pLogincModelContainter->CreateNodeContainer(strTagname);

    m_mapIndexNode.insert(pTagname->index(), pTaskModel);

    CreateNewTask(pTagname, pTaskModel);

}

/*! \fn CLogic_Module::Slot_TreeItemChange(QStandardItem * pItem)
*********************************************************************************************************
** \brief CLogic_Module::Slot_TreeItemChange(QStandardItem * pItem)
** \details ������
** \param
** \return
** \author ww
** \date 2017��1��4��
** \note
********************************************************************************************************/
void CLogic_Module::Slot_TreeItemChange(QStandardItem * pItem)
{

    if (pItem->data(Qt::UserRole) == LOGIC_TAG)
    {
		if (!CheckRenameLegailty(pItem->parent(), pItem, pItem->text()))
		{
			pItem->setText(m_strOldName);
			return;
		}

        if (m_mapIndexNode.contains(pItem->index()))
        {
            std::shared_ptr<CNodeLogicContainer> pTagContainer = m_mapIndexNode[pItem->index()];

            pTagContainer->SetName(pItem->text());
        }
    }
    else if (pItem->data(Qt::UserRole) == LOGIC_EDIT_ITEM)
    {
		if (!CheckRenameLegailty(pItem->parent(), pItem, pItem->text()))
		{
			pItem->setText(m_strOldName);
			return;
		}

        if (m_mapIndexSegOrSr.contains(pItem->index()))
        {
            std::shared_ptr<CLogicObjectModel> pLogicObject = m_mapIndexSegOrSr[pItem->index()];

            pLogicObject->SetName(pItem->text());
        }
    }

    //pItem->setFlags(pItem->flags() & (~Qt::ItemIsEditable));
}

/*! \fn CLogic_Module::CreateMaskItem(CQuoteItem *pLogicItem, std::shared_ptr<CTaskContainer> tModelData)
*********************************************************************************************************
** \brief CLogic_Module::CreateMaskItem(CQuoteItem *pLogicItem, std::shared_ptr<CTaskContainer> tModelData)
** \details ��������ģ�ʹ���mast����
** \param
** \return
** \author ww
** \date 2017��1��4��
** \note
********************************************************************************************************/
void CLogic_Module::CreateMaskItem(CQuoteItem *pLogicItem, std::shared_ptr<CTaskContainer> tModelData)
{
    if (tModelData == nullptr)
    {
        return;
    }

    CQuoteItem *pTaskType = new CQuoteItem(tr("Mast"));
    pTaskType->setData(LOGIC_MAST, Qt::UserRole);
    //pTaskType->setEditable(false);
    pLogicItem->appendRow(pTaskType);
    m_mapIndexTask.insert(pTaskType->index(), tModelData);


    CQuoteItem *pSeg = new CQuoteItem(tr("Seg"));
    pSeg->setData(LOGIC_MASTSEG, Qt::UserRole);
    //pSeg->setEditable(false);
    pTaskType->appendRow(pSeg);

    //����MAST��seg��Ϣ
    for each (std::shared_ptr<CLogicObjectModel> segModel in tModelData->GetSegModels())
    {
        CQuoteItem *pSegItem = new CQuoteItem(segModel->GetName());

        pSegItem->setData(LOGIC_EDIT_ITEM, Qt::UserRole);
        //pSegItem->setEditable(true);
        pSeg->appendRow(pSegItem);

        m_mapIndexSegOrSr.insert(pSegItem->index(), segModel);
    }

    CQuoteItem *pSr = new CQuoteItem(tr("Sr Seg"));
    pSr->setData(LOGIC_MASTSRSEG, Qt::UserRole);
    //pSr->setEditable(false);
    pTaskType->appendRow(pSr);

    //����mast��sr��Ϣ
    for each (std::shared_ptr<CLogicObjectModel> segModel in tModelData->GetSrModels())
    {
        CQuoteItem *pSrItem = new CQuoteItem(segModel->GetName());

        pSrItem->setData(LOGIC_EDIT_ITEM, Qt::UserRole);
        //pSrItem->setEditable(true);
        pSr->appendRow(pSrItem);

        m_mapIndexSegOrSr.insert(pSrItem->index(), segModel);
    }
}

/*! \fn CLogic_Module::CreateFastItem(CQuoteItem *pLogicItem, std::shared_ptr<CTaskContainer> tModelData)
*********************************************************************************************************
** \brief CLogic_Module::CreateFastItem(CQuoteItem *pLogicItem, std::shared_ptr<CTaskContainer> tModelData)
** \details ��������ģ�ʹ���fast����
** \param
** \return
** \author ww
** \date 2017��1��4��
** \note
********************************************************************************************************/
void CLogic_Module::CreateFastItem(CQuoteItem *pLogicItem, std::shared_ptr<CTaskContainer> tModelData)
{
    if (tModelData == nullptr)
    {
        return;
    }

    CQuoteItem *pTaskType = new CQuoteItem(tr("Fast"));
    pTaskType->setData(LOGIC_FAST, Qt::UserRole);
    //pTaskType->setEditable(false);
    pLogicItem->appendRow(pTaskType);
    m_mapIndexTask.insert(pTaskType->index(), tModelData);

    CQuoteItem *pSeg = new CQuoteItem(tr("Seg"));
    pSeg->setData(LOGIC_FAST_SEG, Qt::UserRole);
    //pSeg->setEditable(false);
    pTaskType->appendRow(pSeg);

    //����fast��seg��Ϣ
    for each (std::shared_ptr<CLogicObjectModel> segModel in tModelData->GetSegModels())
    {
        CQuoteItem *pSegItem = new CQuoteItem(segModel->GetName());

        pSegItem->setData(LOGIC_EDIT_ITEM, Qt::UserRole);
        //pSegItem->setEditable(true);
        pSeg->appendRow(pSegItem);

        m_mapIndexSegOrSr.insert(pSegItem->index(), segModel);
    }

    CQuoteItem *pSr = new CQuoteItem(tr("Sr Seg"));
    pSr->setData(LOGIC_FAST_SRSEG, Qt::UserRole);
    //pSr->setEditable(false);
    pTaskType->appendRow(pSr);

    //����fast��sr��Ϣ
    for each (std::shared_ptr<CLogicObjectModel> segModel in tModelData->GetSrModels())
    {
        CQuoteItem *pSrItem = new CQuoteItem(segModel->GetName());

        pSrItem->setData(LOGIC_EDIT_ITEM, Qt::UserRole);
        //pSrItem->setEditable(true);
        pSr->appendRow(pSrItem);

        m_mapIndexSegOrSr.insert(pSrItem->index(), segModel);
    }
}

/*! \fn CLogic_Module::CreateAuxItem(CQuoteItem *pLogicItem, std::shared_ptr<CTaskContainer> tModelData)
*********************************************************************************************************
** \brief CLogic_Module::CreateAuxItem(CQuoteItem *pLogicItem, std::shared_ptr<CTaskContainer> tModelData)
** \details ��������ģ�ʹ���aux����
** \param
** \return
** \author ww
** \date 2017��1��4��
** \note
********************************************************************************************************/
void CLogic_Module::CreateAuxItem(CQuoteItem *pLogicItem, std::shared_ptr<CTaskContainer> tModelData)
{
    if (tModelData == nullptr)
    {
        return;
    }

    CQuoteItem *pTaskType = new CQuoteItem(tr("Aux"));
    pTaskType->setData(LOGIC_AUX0, Qt::UserRole);
    //pTaskType->setEditable(false);
    pLogicItem->appendRow(pTaskType);
    m_mapIndexTask.insert(pTaskType->index(), tModelData);

    CQuoteItem *pSeg = new CQuoteItem(tr("Seg"));
    pSeg->setData(LOGIC_AUX0SEG, Qt::UserRole);
    //pSeg->setEditable(false);
    pTaskType->appendRow(pSeg);

    //����aux��seg��Ϣ
    for each (std::shared_ptr<CLogicObjectModel> segModel in tModelData->GetSegModels())
    {
        CQuoteItem *pSegItem = new CQuoteItem(segModel->GetName());

        pSegItem->setData(LOGIC_EDIT_ITEM, Qt::UserRole);
        //pSegItem->setEditable(true);
        pSeg->appendRow(pSegItem);

        m_mapIndexSegOrSr.insert(pSegItem->index(), segModel);
    }

    CQuoteItem *pSr = new CQuoteItem(tr("Sr Seg"));
    pSr->setData(LOGIC_AUX0_SRSEG, Qt::UserRole);
    //pSr->setEditable(false);
    pTaskType->appendRow(pSr);

    //����AUX��sr��Ϣ
    for each (std::shared_ptr<CLogicObjectModel> segModel in tModelData->GetSrModels())
    {
        CQuoteItem *pSrItem = new CQuoteItem(segModel->GetName());

        pSrItem->setData(LOGIC_EDIT_ITEM, Qt::UserRole);
        //pSrItem->setEditable(true);
        pSr->appendRow(pSrItem);

        m_mapIndexSegOrSr.insert(pSrItem->index(), segModel);
    }
}

/*! \fn CLogic_Module::Slot_UpdateAttr()
*********************************************************************************************************
** \brief CLogic_Module::Slot_UpdateAttr()
** \details �޸����Ժ����ģ����Ϣ
** \param
** \return
** \author ww
** \date 2017��1��4��
** \note
********************************************************************************************************/
void CLogic_Module::Slot_UpdateAttr()
{
    CAttrWgt *pAttrWgt = qobject_cast<CAttrWgt*>(sender());
    Q_ASSERT(pAttrWgt);

    if (pAttrWgt != nullptr)
    {
        if (m_mapIndexTask.contains(m_pCore->GetUIMgr()->GetLeftTree()->currentIndex()))
        {
            //����ģ����Ϣ
            std::shared_ptr<CTaskContainer> pTcontainer = m_mapIndexTask[m_pCore->GetUIMgr()->GetLeftTree()->currentIndex()];

            pTcontainer->SetComment(pAttrWgt->GetComment());
            pTcontainer->SetCycleType(pAttrWgt->GetRepeatType());
            pTcontainer->SetMsec(pAttrWgt->GetMsec());

        }
    }
}

/*! \fn CLogic_Module::DeleteDir(const QString &strDir)
*********************************************************************************************************
** \brief CLogic_Module::DeleteDir(const QString &strDir)
** \details ɾ��Ŀ¼����Ŀ¼�������ļ�
** \param
** \return
** \author ww
** \date 2017��1��4��
** \note
********************************************************************************************************/
void CLogic_Module::DeleteDir(const QString &strDir)
{
    QDir tDir(strDir);

    for each(QFileInfo inf in tDir.entryInfoList(QDir::Dirs | QDir::Files)) 
    {
        if (inf.isFile())
        {
            tDir.remove(inf.absoluteFilePath());
        }
        else if (inf.isDir() && inf.fileName() != "." &&inf.fileName() != "..")
        {
            DeleteDir(inf.absolutePath() + "/" + inf.fileName());
        }
    }

    tDir.rmdir(tDir.absolutePath());
}

/*! \fn CLogic_Module::Slot_DeleteTag
*********************************************************************************************************
** \brief CLogic_Module::Slot_DeleteTag
** \details ɾ��tah��tag�ӽڵ�
** \param
** \return
** \author ww
** \date 2017��1��4��
** \note
********************************************************************************************************/
void CLogic_Module::Slot_DeleteTag()
{
    m_pLogincModelContainter->RemoveNodeContainer(m_pUi->GetLeftTree()->model()->data(m_pUi->GetLeftTree()->currentIndex()).toString());

    m_mapIndexNode.remove(m_pUi->GetLeftTree()->currentIndex());

    m_pProjectItem->removeRow(m_pUi->GetLeftTree()->currentIndex().row());

}

/*! \fn CLogic_Module::Slot_DeleteSegItem()
*********************************************************************************************************
** \brief CLogic_Module::Slot_DeleteSegItem()
** \details ɾ������seg��Ϣ
** \param
** \return
** \author ww
** \date 2017��1��4��
** \note
********************************************************************************************************/
void CLogic_Module::Slot_DeleteSegItem()
{
    m_mapIndexSegOrSr.remove(m_pUi->GetLeftTree()->currentIndex());

    QString strItemName = m_pUi->GetLeftTree()->model()->data(m_pUi->GetLeftTree()->currentIndex()).toString();

    QStandardItemModel *pTreeModel = (QStandardItemModel*)m_pUi->GetLeftTree()->model();
    QStandardItem *pParentItem = (QStandardItem*)pTreeModel->itemFromIndex(m_pUi->GetLeftTree()->currentIndex().parent());

    if (pParentItem->data(Qt::UserRole) == LOGIC_MASTSEG || pParentItem->data(Qt::UserRole) == LOGIC_FAST_SEG
        || pParentItem->data(Qt::UserRole) == LOGIC_AUX0SEG)
    {
        //seg
        if (m_mapIndexTask.contains(m_pUi->GetLeftTree()->currentIndex().parent().parent()))
        {
            std::shared_ptr<CTaskContainer> tContainer = m_mapIndexTask[m_pUi->GetLeftTree()->currentIndex().parent().parent()];
            tContainer->RemoveSegModel(strItemName);
        }
    }
    else if (pParentItem->data(Qt::UserRole) == LOGIC_MASTSRSEG || pParentItem->data(Qt::UserRole) == LOGIC_FAST_SRSEG
        || pParentItem->data(Qt::UserRole) == LOGIC_AUX0_SRSEG)
    {
        //sr
        std::shared_ptr<CTaskContainer> tContainer = m_mapIndexTask[m_pUi->GetLeftTree()->currentIndex().parent().parent()];
        tContainer->RemoveSrModel(strItemName);
    }

    if (m_mapItemWgt.contains(m_pUi->GetLeftTree()->currentIndex()))
    {
//         m_pTabWgt->setCurrentWidget(pView);

        m_mapItemWgt[m_pUi->GetLeftTree()->currentIndex()]->deleteLater();
        m_mapItemWgt.remove(m_pUi->GetLeftTree()->currentIndex());
    }

    if (pParentItem != nullptr)
    {
        pParentItem->removeRow(m_pUi->GetLeftTree()->currentIndex().row());
    }

}

void CLogic_Module::Slot_RenameTreeItem()
{
    m_strOldName = m_pCore->GetUIMgr()->GetLeftTree()->model()->data(m_pCore->GetUIMgr()->GetLeftTree()->currentIndex()).toString();
    
    m_pCore->GetUIMgr()->GetLeftTree()->edit(m_pCore->GetUIMgr()->GetLeftTree()->currentIndex());
}

/*! \fn CheckNameLegailty(CQuoteItem *pParent)
*********************************************************************************************************
** \brief CheckNameLegailty(CQuoteItem *pParent)
** \details ���pParent�ڵ����Ƿ����ظ�
** \param
** \return
** \author ww
** \date 2017��1��4��
** \note
********************************************************************************************************/
bool CLogic_Module::CheckNameLegailty(QStandardItem *pParent, const QString strName)
{
    Q_ASSERT(pParent);
    if (pParent == nullptr)
    {
        return false;
    }
    for (int i = 0; i < pParent->rowCount();i++)
    {
        if (pParent->child(i)->text() == strName)
        {
            QMessageBox::warning(0, tr("Warning"), tr("%1 has exists,please rename").arg(strName));
            return false;
        }
    }

    return true;
}

bool CLogic_Module::CheckRenameLegailty(QStandardItem *pParent, QStandardItem *pCurrent, const QString strName)
{
    Q_ASSERT(pParent);
    Q_ASSERT(pCurrent);

    if (pParent == nullptr || pCurrent == nullptr)
    {
        return false;
    }

    for (int i = 0; i < pParent->rowCount(); i++)
    {
        if (pParent->child(i)->text() == strName && pParent->child(i)!= pCurrent)
        {
            QMessageBox::warning(0, tr("Warning"), tr("%1 has exists,please rename").arg(strName));
            return false;
        }
    }

    return true;
}

bool CLogic_Module::ChangeTagNameNodify(QString &tagName, QString &lastTagName)
{

	Q_UNUSED(tagName);
	Q_UNUSED(lastTagName);
	return true;
}
