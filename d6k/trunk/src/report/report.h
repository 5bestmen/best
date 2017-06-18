/*! @file report.h
<PRE>
********************************************************************************
模块名       :
文件名       :  report.h
文件实现功能 :  报表整体功能实现
作者         :  xingzhibing
版本         :  V1.00
--------------------------------------------------------------------------------
备注         :  <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief   报表整体功能实现
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
	//框架虚接口
	void Init(IMainModuleInterface * pCore);
	void UnInit();
	IMainWindow *GetCoreUI()
	{
		return m_pUi;
	}
public:
	virtual bool SaveProject(QDomDocument *pXml, QDomElement *pRoot, const QString & szRoot);
	virtual bool LoadProject(QDomDocument *pXml, QDomElement *pRoot, const QString & szRoot);

	// 关闭工程文件
	virtual void CloseProject(QDomDocument *pXml, QDomElement *pRoot, const QString & szRoot, bool bSaveFlag);
	// 新建一个空的工程文件
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
	void InitMenuBar();                 //！<初始化菜单栏
	void InitToolBar();                 //！<初始化工具栏

	void CreateConnections();           //！<创建连接
	bool OkToContinue();                //! <判断文件是否修改
	CReportEditView* GetCurrentReportView();    //! <获取当前使用报表
	//表格内容设置
	void SetItemData(int nRow,int nCol,QString nContent);
	//加载文件
	bool LoadFile(const QString& nName);
	//加载报表节点
	bool LoadReportNode(QDomElement pElement,CQuoteItem* pItem,CRptFileGrp* pRptGrp,const QString & szRootPath );
	//解析报表组
	bool XmlParaseReportGroup(QDomElement pElement, CQuoteItem* pItem, CRptFileGrp* pRptGrp, const QString & szRootPath);
	//解析报表文件
	bool XmlParaseReportFile(QDomElement pElement, CQuoteItem* pItem, CRptFileGrp* pRptGrp, const QString & szRootPath);
	//树控件操作
	void AddChildItem(QString str, CQuoteItem* item, int Role, QIcon Icon);
	//创建节点
	void CreateFileGroupNode(CQuoteItem* pItem, QDomDocument *pDom, QDomElement pElement);
	//创建报表组节点
	void CreateGroupNode(CQuoteItem* pItem, QDomDocument *pDom, QDomElement pElement);
	//创建报表文件节点
	void CreateFileNode(CQuoteItem* pItem, QDomDocument *pDom, QDomElement pElement);

	void SaveRptGrpsToProject(QDomDocument *pXml, QDomElement nElement, CRptFileGrp* pRptGrp,bool bIsAppended);

	//void SaveRptFileToProject(QDomDocument *pXml, QDomElement nElement, CRptFileGrp* pRptGrp);
private:
	IMainModuleInterface *m_pCore;             
	CTree *m_pTree;                     //！<文件树控件
	CWidget *m_pTabWidget;              //！<Tab控件	
	IMainWindow *m_pUi;                 //！<主窗口控件

	QString m_CurrentReportName;        //！<当前报表
	int m_nCurrentReportType;           //！<当前报表类型
	CQuoteItem* m_pTopItem;
	//所有报表数据
	std::shared_ptr <CRptFileMgr> m_pRptData;
	bool m_bIsInitView;
private:
	QString m_szProjectPath;            //！<工程路径
	bool m_bDirtyFlag;
//File  
	QMenu *m_pFileMenu;
	QMenu *m_pEdit;
	QMenu *m_pOptions;
	QToolBar *m_pToolBar;

	QAction *m_pFileNew;                 //！<新建文件
	QAction *m_pFileOpen;                //！<打开文件
	QAction *m_pFileSave;                //！<保存文件
	QAction *m_pFileSaveAs;              //！<文件另存为
	QAction *m_pFileExport;              //！<文件导出
	QAction *m_pFilePdf;                 //！<PDF导出
	QAction *m_pFilePrint;               //！<打印文件
	QAction *m_pFilePrintOverView;       //！<打印文件预览
	QAction *m_pFileClose;               //！<关闭文件
//EDIT
	QAction *m_CutAction;                //！<剪切
	QAction *m_CopyAction;               //！<复制
	QAction *m_PasteAction;              //！<粘贴
	QAction *m_DeleteAction;             //！<删除
	QMenu   *m_SelectSubMenu;
	QAction *m_SelectAllAction;          //！<选中所有
	QAction *m_SelectRowAction;          //！<选择行
	QAction *m_SelectColumnAction;       //！<选择列
//OPERATION
	QMenu   *m_OperateMenu;
	QAction *m_MergeAction;               //！<合并单元格      
	QAction *m_UnMergeAction;             //！<解除合并
	QAction *m_SetFont;                  //！<设置字体
	QMenu   *m_PlaceSubMenu;   
	QAction *m_LeftAlign;                //！<左对齐
	QAction *m_MiddleAlign;              //！<居中
	QAction *m_RightAlign;               //！<右对齐
	QAction *m_FontColor;                //！<字体颜色
//OPTIONS
	QAction *m_SelectPoints;             //！<多点选取
	QAction *m_SelectSignalPoint;        //！<单点选取
	QAction *m_SetMyFormula;             //！<计算公式
//CustomContestMenu 自定义菜单
	QMenu *m_pCustomMenu;
	QAction* m_ConnForGround;            //!<连接前景
	QAction* m_delForGround;             //!<删除前景
	QAction* m_delRow;					 //!<删除行
	QAction* m_delCol;                   //!<删除列
	QAction* m_AddRow;                   //!<增加行
	QAction* m_AddCol;					 //!<增加列
	QAction* m_SetReportTime;            //!<设置报表时间
	QAction* m_TimeCol;                  //!<设置时间列
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
	QAction *m_BarAction;                                    //! <棒状图
	QAction *m_PieAction;                                    //! <饼状图
	QAction *m_CurveAction;                                  //! <曲线
protected Q_SLOTS:
	void slot_IfInit(bool bRet);
//FILE 文件操作
	void slot_OnNewFile();
	void slot_OnOpenFile();
	bool slot_OnSaveFile();
	void slot_OnSaveAsFile();
	void slot_OnPrintFile();
	void slot_OnPreViewFile();
	void slot_OnExport();
	void slot_OnPdfExport();
	void slot_OnCloseFile();
	void slot_OnSetSigPointData();                            //!< 设置单点信息
	void slot_OnSetMultiPointData();                          //!< 设置多点信息
	void slot_OnSetFormula();                                 //!< 设置计算公式
	//void slot_OnDealSelData(const QString& );               //!< 处理选取数据
//EDIT 编辑操作
	void setWindowModiy();
//TREE 树控件操作
	void slot_OnAddReportFile(QModelIndex& index);
	void slot_OnDelReportFile(QModelIndex& index);
	void slot_OnModReportName(QModelIndex& index);
	void slot_OnModReportType(QModelIndex& index);
	void slot_OnImpReportFile(QModelIndex& index);
	void slot_OnOpenReportFile(QModelIndex& index);
	void slot_OnAddReporGrp(QModelIndex& index);
	void slot_OnRptItemChanged(QModelIndex& index);
//CustomContestMenu 自定义菜单
	void slot_OnCustomContextMenu(const QPoint& pos);
//Font
	void slot_OnSetColor(const QColor&);
	void slot_OnSetBgColor(const QColor&);
	void slot_OnSetFontTextBold();
	void slot_OnSetFontTextItalic();
	void slot_OnSetFontUnderLine();
	void slot_OnSetFontFamily(int);
	void slot_OnSetFontSize(int);
//文件预览打印
	void slot_OnPreViewReport(QPrinter* printer);
//操作
	void slot_OnCopy();
	void slot_OnPaste();
	void slot_OnCut();
	void slot_OnMerge();
	void slot_OnUnMerge();
	void slot_OnAlignLeft();
	void slot_OnAlignRight();
	void slot_OnAlignCenter();
//图形
	void slot_OnBar();
	void slot_OnPie();
	void slot_OnCurve();
	void slot_OnDataChanged(QStandardItem* pItem);
protected:
	bool  SaveFile(QString& fileName);                    //!<<通用保存接口
	void SetCurrentFile(const QString& fileName);         //!<<设置当前文件
	bool IsFileExisted(const QString& fileName);          //!<<判断当前文件夹是否存在需创建问价
	bool IsFileInTabWidget(CRptFile* pRptFile);
	bool LogMsg(const char*szLogText,int nLevel);         //!<<log信息
};
extern "C" SCADA_EXPORT IModule *CreateModule();
extern "C" SCADA_EXPORT  void    DestroyModule();
#endif // REPORT_H
