/*! @file cwidget.h
<PRE>
********************************************************************************
模块名       :
文件名       :  cwidget.h
文件实现功能 :  报表控件
作者         :  xingzhibing
版本         :  V1.00
--------------------------------------------------------------------------------
备注         :  <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief   报表控件
*  @author  xingzhibing
*  @version 1.0
*  @date    2016.08.31
*******************************************************************************/


#pragma once
#include <QTableWidget>
#include "scadastudio/base.h"
#include "scadastudio/tabwidget.h"
#include "qspreadsheet.h"




class CReportEditView:public CSpreadSheet,public IBase
{
	Q_OBJECT
public:
	//原始接口，已弃用
	CReportEditView(CTableInfo* nTableInfo, IMainModuleInterface* pCore);
	//新接口，使用中
	CReportEditView(CRptFile* nTableInfo, IMainModuleInterface* pCore);
	~CReportEditView();
public:
	virtual void Save();
	virtual void Refresh();
	virtual void SetModifyFlag(bool bFlag)
	{ 
		m_bFlag = bFlag;
	}
	virtual bool GetModifyFlag()
	{
		return m_bFlag;
	}
	CRptFile* GetRptFile()
	{
		Q_ASSERT(m_pRptFile);
		if (m_pRptFile)
		{
			return m_pRptFile;
		}
		else
		{
			return Q_NULLPTR;
		}
	}
	
public:
	CTableInfo* GetTableInfo();
	void GetCellData();
private:
	CTableInfo* m_pTabelInfo;
	IMainModuleInterface* m_pCore;
	bool   m_bFlag;
	CRptFile *m_pRptFile;
private:
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
//基本操作
	QAction *m_CutAction;                //！<剪切
	QAction *m_CopyAction;               //！<复制
	QAction *m_PasteAction;              //！<粘贴
//单元格操作
	QAction *m_SpanAction;               //！<合并单元格      
	QAction *m_UnSpanAction;             //！<解除合并

	QMenu *m_pCellMenu;                  //！<单元格操作

	QMenu   *m_SelectSubMenu;
	QAction *m_SelectAllAction;          //！<选中所有
	QAction *m_SelectRowAction;          //！<选择行
	QAction *m_SelectColumnAction;       //！<选择列

	QAction *m_SetFont;                  //！<设置字体
	QAction *m_FontColor;                //！<字体颜色
//对齐方式
	QMenu *m_AlignMenu;
	QAction *m_LeftAlign;                //！<左对齐
	QAction *m_MiddleAlign;              //！<居中
	QAction *m_RightAlign;               //！<右对齐


protected Q_SLOTS:
	void SetWinModify();
	void slot_OnCustomMenu(const QPoint& pos); 	// !< 右键菜单	
	void slot_OnConnForGd();                    // !< 连接前景
	void slot_OnDelForGd();                     // !< 删除前景
	void slot_OnDelRow();                       // !< 删除行	
	void slot_onDelCol();                       // !< 删除列	
	void slot_OnAddRow();                       // !< 增加行	
	void slot_OnAddCol();                       // !< 增加列	
	void slot_OnSetRptTime();                   // !< 设置报表时间	
	void slot_onTimeCol();                      // !< 设置时间列参数	
	void slot_onCopy();                         // !< 复制	
	void slot_OnCut();                          // !< 剪切	
	void slot_onPaste();                        // !< 黏贴	
	void slot_OnMergeCell();                    // !< 合并单元格	
	void slot_OnUnMergeCell();                  // !< 拆分单元格	
	void slot_OnSelRow();                       // !< 选择行	
	void slot_OnSelCol();                       // !< 选择列	
	void slot_OnSelAll();                       // !< 选择所有	
	void slot_OnSetFont();                      // !< 设置字体	 
	void slot_OnSetFontColor();                 // !< 设置字体颜色	
	void slot_OnSetAlignRight();                // !< 右对齐	
	void slot_OnSetAlignLeft();                 // !< 左对齐	
	void slot_OnSetAlignCenter();               // !< 居中对齐
private:
	void InitMenuBar();
};



class CWidget:public QWidget
{
	Q_OBJECT
public:
	CWidget(CTabWidget* pTableWidget,IMainModuleInterface* pCore);
	~CWidget();
public:
	//公共接口函数
	void AddTabCtrl(CTableInfo* tableInfo, bool isNewFile);        //!<添加Tab控件
	void AddTabCtrl(CRptFile* tableInfo);                          //!<添加Tab控件
	CReportEditView *GetCurrerntTable();                           //!<获取当前控件
private:
	CTabWidget *m_pTableWidget;                            //!<接受Tab控件
	CReportEditView *m_pCurrentReportWidget;               //!<当前报表控件
	QList< CTableInfo* >m_TabList;                         //!<表单列表集合
	bool IsItemExist(CTableInfo*);                         //!<表单是否存在
Q_SIGNALS:
	void sig_TabClicked(bool);
private Q_SLOTS:
	void slot_OnRemoveItem( QWidget* );
	void slot_OnSelectChanged(int nIndex);
	void slot_TabClicked(int ,int );
private:
	IMainModuleInterface *m_pCore;
};

