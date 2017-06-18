/*! @file cwidget.h
<PRE>
********************************************************************************
ģ����       :
�ļ���       :  cwidget.h
�ļ�ʵ�ֹ��� :  ����ؼ�
����         :  xingzhibing
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         :  <����˵��>
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

********************************************************************************
</PRE>
*  @brief   ����ؼ�
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
	//ԭʼ�ӿڣ�������
	CReportEditView(CTableInfo* nTableInfo, IMainModuleInterface* pCore);
	//�½ӿڣ�ʹ����
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
//��������
	QAction *m_CutAction;                //��<����
	QAction *m_CopyAction;               //��<����
	QAction *m_PasteAction;              //��<ճ��
//��Ԫ�����
	QAction *m_SpanAction;               //��<�ϲ���Ԫ��      
	QAction *m_UnSpanAction;             //��<����ϲ�

	QMenu *m_pCellMenu;                  //��<��Ԫ�����

	QMenu   *m_SelectSubMenu;
	QAction *m_SelectAllAction;          //��<ѡ������
	QAction *m_SelectRowAction;          //��<ѡ����
	QAction *m_SelectColumnAction;       //��<ѡ����

	QAction *m_SetFont;                  //��<��������
	QAction *m_FontColor;                //��<������ɫ
//���뷽ʽ
	QMenu *m_AlignMenu;
	QAction *m_LeftAlign;                //��<�����
	QAction *m_MiddleAlign;              //��<����
	QAction *m_RightAlign;               //��<�Ҷ���


protected Q_SLOTS:
	void SetWinModify();
	void slot_OnCustomMenu(const QPoint& pos); 	// !< �Ҽ��˵�	
	void slot_OnConnForGd();                    // !< ����ǰ��
	void slot_OnDelForGd();                     // !< ɾ��ǰ��
	void slot_OnDelRow();                       // !< ɾ����	
	void slot_onDelCol();                       // !< ɾ����	
	void slot_OnAddRow();                       // !< ������	
	void slot_OnAddCol();                       // !< ������	
	void slot_OnSetRptTime();                   // !< ���ñ���ʱ��	
	void slot_onTimeCol();                      // !< ����ʱ���в���	
	void slot_onCopy();                         // !< ����	
	void slot_OnCut();                          // !< ����	
	void slot_onPaste();                        // !< ���	
	void slot_OnMergeCell();                    // !< �ϲ���Ԫ��	
	void slot_OnUnMergeCell();                  // !< ��ֵ�Ԫ��	
	void slot_OnSelRow();                       // !< ѡ����	
	void slot_OnSelCol();                       // !< ѡ����	
	void slot_OnSelAll();                       // !< ѡ������	
	void slot_OnSetFont();                      // !< ��������	 
	void slot_OnSetFontColor();                 // !< ����������ɫ	
	void slot_OnSetAlignRight();                // !< �Ҷ���	
	void slot_OnSetAlignLeft();                 // !< �����	
	void slot_OnSetAlignCenter();               // !< ���ж���
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
	//�����ӿں���
	void AddTabCtrl(CTableInfo* tableInfo, bool isNewFile);        //!<���Tab�ؼ�
	void AddTabCtrl(CRptFile* tableInfo);                          //!<���Tab�ؼ�
	CReportEditView *GetCurrerntTable();                           //!<��ȡ��ǰ�ؼ�
private:
	CTabWidget *m_pTableWidget;                            //!<����Tab�ؼ�
	CReportEditView *m_pCurrentReportWidget;               //!<��ǰ����ؼ�
	QList< CTableInfo* >m_TabList;                         //!<���б���
	bool IsItemExist(CTableInfo*);                         //!<���Ƿ����
Q_SIGNALS:
	void sig_TabClicked(bool);
private Q_SLOTS:
	void slot_OnRemoveItem( QWidget* );
	void slot_OnSelectChanged(int nIndex);
	void slot_TabClicked(int ,int );
private:
	IMainModuleInterface *m_pCore;
};

