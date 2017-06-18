/*! @file report.h
<PRE>
********************************************************************************
ģ����       :
�ļ���       :  report.h
�ļ�ʵ�ֹ��� :  �������幦��ʵ��
����         :  xingzhibing
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         :  <����˵��>
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

********************************************************************************
</PRE>
*  @brief   �������幦��ʵ��
*  @author  xingzhibing
*  @version 1.0
*  @date    2016.08.31
*******************************************************************************/

#ifndef REPORT_H
#define REPORT_H
#include <array>
#include <memory>
#include <QObject>

#include "report_global.h"
#include "scadastudio/imodule.h"
#include "scadastudio/module_export.h"
class QStandardItem;
class CReportAPI;
class IMainWindow;
class CQuoteItem;
class IModule;
class QDomElement;
class CTree;
class CWidget;
class QAction;
class QMenu;
class QToolBar;
class DColorButton;
class DColorPanel;
class QFontComboBox;
class QComboBox;
class QPrinter;
class CRptFileMgr;
class CRptFile;
class CRptFileGrp;

CReportAPI *GetModuleApi();
IMainWindow *GetMainWindow();

class CReportEditView;

class  CReportAPI:public QObject,public IModule
{
	Q_OBJECT
public:
	CReportAPI();
	~CReportAPI();
public:
	//�����ӿ�
	void Init(IMainModuleInterface * pCore);
	void UnInit();
	IMainWindow *GetCoreUI()
	{
		return m_pUi;
	}
public:
	virtual bool SaveProject(QDomDocument *pXml, QDomElement *pRoot, const QString & szRoot);
	virtual bool LoadProject(QDomDocument *pXml, QDomElement *pRoot, const QString & szRoot);

	// �رչ����ļ�
	virtual void CloseProject(QDomDocument *pXml, QDomElement *pRoot, const QString & szRoot, bool bSaveFlag);
	// �½�һ���յĹ����ļ�
	virtual void CreateNewProject(QDomDocument *pXml, QDomElement *pRoot, const QString & szRoot, CQuoteItem *pTopItem);

	virtual bool GetModifyFlag()const
	{
		return m_bDirtyFlag;
	}

	virtual IMainModuleInterface* GetInterface() const
	{
		return nullptr;
	}
	virtual bool Check()
	{
		return true;
	}

	virtual bool GetDesPoolArr(std::unordered_map<std::string, int32u> *pHash, std::vector<std::string> *pStringPoolVec, int32u *pDescStringPoolOccNo)
	{
		Q_UNUSED(pStringPoolVec);
		Q_UNUSED(pDescStringPoolOccNo);

		return true;
	}

	bool ChangeTagNameNodify(QString &, QString &)
	{
		return true;
	}

	void SetProjectPath(const QString& path)
	{
		m_szProjectPath = path+'/'+"report/";
	}
	QString GetProjectPath() const
	{
		return m_szProjectPath;
	}
protected:
	void InitMenuBar();                 //��<��ʼ���˵���
	void InitToolBar();                 //��<��ʼ��������

	void CreateConnections();           //��<��������
	bool OkToContinue();                //! <�ж��ļ��Ƿ��޸�
	CReportEditView* GetCurrentReportView();    //! <��ȡ��ǰʹ�ñ���
	//�����������
	void SetItemData(int nRow,int nCol,QString nContent);
	//�����ļ�
	bool LoadFile(const QString& nName);
	//���ر���ڵ�
	bool LoadReportNode(QDomElement pElement,CQuoteItem* pItem,CRptFileGrp* pRptGrp,const QString & szRootPath );
	//����������
	bool XmlParaseReportGroup(QDomElement pElement, CQuoteItem* pItem, CRptFileGrp* pRptGrp, const QString & szRootPath);
	//���������ļ�
	bool XmlParaseReportFile(QDomElement pElement, CQuoteItem* pItem, CRptFileGrp* pRptGrp, const QString & szRootPath);
	//���ؼ�����
	void AddChildItem(QString str, CQuoteItem* item, int Role, QIcon Icon);
	//�����ڵ�
	void CreateFileGroupNode(CQuoteItem* pItem, QDomDocument *pDom, QDomElement pElement);
	//����������ڵ�
	void CreateGroupNode(CQuoteItem* pItem, QDomDocument *pDom, QDomElement pElement);
	//���������ļ��ڵ�
	void CreateFileNode(CQuoteItem* pItem, QDomDocument *pDom, QDomElement pElement);

	void SaveRptGrpsToProject(QDomDocument *pXml, QDomElement nElement, CRptFileGrp* pRptGrp,bool bIsAppended);

	//void SaveRptFileToProject(QDomDocument *pXml, QDomElement nElement, CRptFileGrp* pRptGrp);
private:
	IMainModuleInterface *m_pCore;             
	CTree *m_pTree;                     //��<�ļ����ؼ�
	CWidget *m_pTabWidget;              //��<Tab�ؼ�	
	IMainWindow *m_pUi;                 //��<�����ڿؼ�

	QString m_CurrentReportName;        //��<��ǰ����
	int m_nCurrentReportType;           //��<��ǰ��������
	CQuoteItem* m_pTopItem;
	//���б�������
	std::shared_ptr <CRptFileMgr> m_pRptData;
	bool m_bIsInitView;
private:
	QString m_szProjectPath;            //��<����·��
	bool m_bDirtyFlag;
//File  
	QMenu *m_pFileMenu;
	QMenu *m_pEdit;
	QMenu *m_pOptions;
	QToolBar *m_pToolBar;

	QAction *m_pFileNew;                 //��<�½��ļ�
	QAction *m_pFileOpen;                //��<���ļ�
	QAction *m_pFileSave;                //��<�����ļ�
	QAction *m_pFileSaveAs;              //��<�ļ����Ϊ
	QAction *m_pFileExport;              //��<�ļ�����
	QAction *m_pFilePdf;                 //��<PDF����
	QAction *m_pFilePrint;               //��<��ӡ�ļ�
	QAction *m_pFilePrintOverView;       //��<��ӡ�ļ�Ԥ��
	QAction *m_pFileClose;               //��<�ر��ļ�
//EDIT
	QAction *m_CutAction;                //��<����
	QAction *m_CopyAction;               //��<����
	QAction *m_PasteAction;              //��<ճ��
	QAction *m_DeleteAction;             //��<ɾ��
	QMenu   *m_SelectSubMenu;
	QAction *m_SelectAllAction;          //��<ѡ������
	QAction *m_SelectRowAction;          //��<ѡ����
	QAction *m_SelectColumnAction;       //��<ѡ����
//OPERATION
	QMenu   *m_OperateMenu;
	QAction *m_MergeAction;               //��<�ϲ���Ԫ��      
	QAction *m_UnMergeAction;             //��<����ϲ�
	QAction *m_SetFont;                  //��<��������
	QMenu   *m_PlaceSubMenu;   
	QAction *m_LeftAlign;                //��<�����
	QAction *m_MiddleAlign;              //��<����
	QAction *m_RightAlign;               //��<�Ҷ���
	QAction *m_FontColor;                //��<������ɫ
//OPTIONS
	QAction *m_SelectPoints;             //��<���ѡȡ
	QAction *m_SelectSignalPoint;        //��<����ѡȡ
	QAction *m_SetMyFormula;             //��<���㹫ʽ
//CustomContestMenu �Զ���˵�
	QMenu *m_pCustomMenu;
	QAction* m_ConnForGround;            //!<����ǰ��
	QAction* m_delForGround;             //!<ɾ��ǰ��
	QAction* m_delRow;					 //!<ɾ����
	QAction* m_delCol;                   //!<ɾ����
	QAction* m_AddRow;                   //!<������
	QAction* m_AddCol;					 //!<������
	QAction* m_SetReportTime;            //!<���ñ���ʱ��
	QAction* m_TimeCol;                  //!<����ʱ����
//Font
	DColorButton *m_Color;
	DColorPanel *m_ColorPanel;
	QFontComboBox *m_FontCombo;
	QComboBox *m_Fontsize;

	DColorButton *m_Brush;
	DColorPanel *m_BrushPanel;

	QAction *m_Bold;
	QAction *m_Itilic;
	QAction *m_Underline;
//Bar Pie Curve
	QAction *m_BarAction;                                    //! <��״ͼ
	QAction *m_PieAction;                                    //! <��״ͼ
	QAction *m_CurveAction;                                  //! <����
protected Q_SLOTS:
	void slot_IfInit(bool bRet);
//FILE �ļ�����
	void slot_OnNewFile();
	void slot_OnOpenFile();
	bool slot_OnSaveFile();
	void slot_OnSaveAsFile();
	void slot_OnPrintFile();
	void slot_OnPreViewFile();
	void slot_OnExport();
	void slot_OnPdfExport();
	void slot_OnCloseFile();
	void slot_OnSetSigPointData();                            //!< ���õ�����Ϣ
	void slot_OnSetMultiPointData();                          //!< ���ö����Ϣ
	void slot_OnSetFormula();                                 //!< ���ü��㹫ʽ
	//void slot_OnDealSelData(const QString& );               //!< ����ѡȡ����
//EDIT �༭����
	void setWindowModiy();
//TREE ���ؼ�����
	void slot_OnAddReportFile(QModelIndex& index);
	void slot_OnDelReportFile(QModelIndex& index);
	void slot_OnModReportName(QModelIndex& index);
	void slot_OnModReportType(QModelIndex& index);
	void slot_OnImpReportFile(QModelIndex& index);
	void slot_OnOpenReportFile(QModelIndex& index);
	void slot_OnAddReporGrp(QModelIndex& index);
	void slot_OnRptItemChanged(QModelIndex& index);
//CustomContestMenu �Զ���˵�
	void slot_OnCustomContextMenu(const QPoint& pos);
//Font
	void slot_OnSetColor(const QColor&);
	void slot_OnSetBgColor(const QColor&);
	void slot_OnSetFontTextBold();
	void slot_OnSetFontTextItalic();
	void slot_OnSetFontUnderLine();
	void slot_OnSetFontFamily(int);
	void slot_OnSetFontSize(int);
//�ļ�Ԥ����ӡ
	void slot_OnPreViewReport(QPrinter* printer);
//����
	void slot_OnCopy();
	void slot_OnPaste();
	void slot_OnCut();
	void slot_OnMerge();
	void slot_OnUnMerge();
	void slot_OnAlignLeft();
	void slot_OnAlignRight();
	void slot_OnAlignCenter();
//ͼ��
	void slot_OnBar();
	void slot_OnPie();
	void slot_OnCurve();
	void slot_OnDataChanged(QStandardItem* pItem);
protected:
	bool  SaveFile(QString& fileName);                    //!<<ͨ�ñ���ӿ�
	void SetCurrentFile(const QString& fileName);         //!<<���õ�ǰ�ļ�
	bool IsFileExisted(const QString& fileName);          //!<<�жϵ�ǰ�ļ����Ƿ�����贴���ʼ�
	bool IsFileInTabWidget(CRptFile* pRptFile);
	bool LogMsg(const char*szLogText,int nLevel);         //!<<log��Ϣ
};
extern "C" SCADA_EXPORT IModule *CreateModule();
extern "C" SCADA_EXPORT  void    DestroyModule();
#endif // REPORT_H
