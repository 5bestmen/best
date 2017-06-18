#include <QMessageBox>
#include <QMenu>
#include <QAction>
#include <QApplication>
#include "cwidget.h"
#include "qspreadsheet.h"
#include "scadastudio/define.h"
#include "reportdefine.h"

CReportEditView::CReportEditView(CTableInfo* nTableInfo, IMainModuleInterface* pCore)
{
	Q_ASSERT(nTableInfo !=nullptr);
	Q_ASSERT(pCore != nullptr);
	m_pTabelInfo  = Q_NULLPTR;
	m_pCore       = Q_NULLPTR;
	m_pRptFile    = Q_NULLPTR;
	m_pTabelInfo  = nTableInfo;	
	m_pCore       = pCore;
	SetModifyFlag(false);
	InitMenuBar();
	setContextMenuPolicy(Qt::CustomContextMenu);
	QObject::connect(this, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(slot_OnCustomMenu(const QPoint&)));
	QObject::connect(this, SIGNAL(modified()), this, SLOT(SetWinModify()));
}

/*! \fn CReportEditView::CReportEditView(CRptFile* nTableInfo, IMainModuleInterface* pCore)
********************************************************************************************************* 
** \brief CReportEditView::CReportEditView 
** \details 新接口，用于报表创建
** \param nTableInfo 
** \param pCore 
** \return  
** \author xingzhibing
** \date 2016年8月15日 
** \note 
********************************************************************************************************/
CReportEditView::CReportEditView(CRptFile* pTableInfo, IMainModuleInterface* pCore)
{
	Q_ASSERT(pTableInfo != nullptr);
	Q_ASSERT(pCore != nullptr);
	m_pTabelInfo  = Q_NULLPTR;
	m_pCore       = Q_NULLPTR;
	m_pRptFile    = Q_NULLPTR;
	m_pRptFile    = pTableInfo;
	m_pCore       = pCore;
	InitMenuBar();
	SetModifyFlag(false);
	setContextMenuPolicy(Qt::CustomContextMenu);
	QObject::connect(this, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(slot_OnCustomMenu(const QPoint&)));
	QObject::connect(this, SIGNAL(modified()), this, SLOT(SetWinModify()));

	InitSpreadSheet(pTableInfo);
}

CReportEditView::~CReportEditView()
{
	if (m_pTabelInfo)
	{
		delete m_pTabelInfo;
		m_pTabelInfo = NULL;
	}
	if (m_pCustomMenu)
	{
		m_pCustomMenu->deleteLater();
	}
}
void CReportEditView::Save()
{
	if (m_pRptFile == nullptr)
	{
		return;
	}
	//m_pRptFile->SaveFile();

#if 0
	//未修改，则不进行保存
	if (GetModifyFlag())
	{
		//int r = QMessageBox::warning(NULL, tr("report"), tr("This Document has been modified!\n""Want To Save?"),
		//	QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
		//if (r == QMessageBox::Yes)
		//{
			if (!writeFile(m_pRptFile->GetRptFileName()))
			{
				m_pCore->LogMsg(REPORT_DES, (char*)tr("save file failed!!!").toStdString().c_str(), LEVEL_1);
		//	}
		}
	}
	SetModifyFlag(false);
#endif

}

void CReportEditView::Refresh()
{

}

/*! \fn CTableInfo* CReportEditView::GetTableInfo()
********************************************************************************************************* 
** \brief CReportEditView::GetTableInfo 
** \details 获取报表信息
** \return CTableInfo* 
** \author xingzhibing
** \date 2016年7月25日 
** \note 
********************************************************************************************************/
CTableInfo* CReportEditView::GetTableInfo()
{
	if (m_pTabelInfo!=NULL)
	{
		return m_pTabelInfo;
	}
	else
	{
		return NULL;
	}	
}

/*! \fn void CReportEditView::slot_OnCustomMenu(const QPoint& pos)
********************************************************************************************************* 
** \brief CReportEditView::slot_OnCustomMenu 
** \details 右键菜单
** \param pos 
** \return void 
** \author xingzhibing
** \date 2016年8月5日 
** \note 
********************************************************************************************************/
void CReportEditView::slot_OnCustomMenu(const QPoint& pos)
{
	Q_UNUSED(pos);
	m_pCustomMenu->exec(QCursor::pos());
}

/*! \fn void CReportEditView::InitMenuBar()
********************************************************************************************************* 
** \brief CReportEditView::InitMenuBar 
** \details 初始化右键菜单栏
** \return void 
** \author xingzhibing
** \date 2016年8月5日 
** \note 
********************************************************************************************************/
void CReportEditView::InitMenuBar()
{
	m_pCustomMenu = new QMenu(this);
	m_ConnForGround = new QAction(tr("SetData"), m_pCustomMenu);
	m_ConnForGround->setIcon(QIcon(":/images/data_s.png"));
	connect(m_ConnForGround, SIGNAL(triggered()), this, SLOT(slot_OnConnForGd()));

	m_delForGround = new QAction(tr("DelData"), m_pCustomMenu);
	m_delForGround->setIcon(QIcon(":/images/delete_report_file.ico"));
	connect(m_delForGround, SIGNAL(triggered()), this, SLOT(slot_OnDelForGd()));

	m_delRow = new QAction(tr("RemoveRow"), m_pCustomMenu);
	m_delRow->setIcon(QIcon(":/images/del_row.png"));
	connect(m_delRow, SIGNAL(triggered()), this, SLOT(slot_OnDelRow()));

	m_delCol = new QAction(tr("RemoveCol"), m_pCustomMenu);
	m_delCol->setIcon(QIcon(":/images/del_col.png"));
	connect(m_delCol, SIGNAL(triggered()), this, SLOT(slot_onDelCol()));

	m_AddRow = new QAction(tr("AddRow"), m_pCustomMenu);
	m_AddRow->setIcon(QIcon(":/images/add_row.png"));
	connect(m_AddRow, SIGNAL(triggered()), this, SLOT(slot_OnAddRow()));

	m_AddCol = new QAction(tr("AddCol"), m_pCustomMenu);
	m_AddCol->setIcon(QIcon(":/images/add_col.png"));
	connect(m_AddCol, SIGNAL(triggered()), this, SLOT(slot_OnAddCol()));

	m_SetReportTime = new QAction(tr("SetTime"), m_pCustomMenu);
	m_SetReportTime->setIcon(QIcon(":/images/set_time.png"));
	connect(m_SetReportTime, SIGNAL(triggered()), this, SLOT(slot_OnSetRptTime()));

	m_TimeCol = new QAction(tr("TimeCol"), m_pCustomMenu);

	m_pCustomMenu->addAction(m_ConnForGround);
	m_pCustomMenu->addAction(m_delForGround);
	m_pCustomMenu->addAction(m_delRow);
	m_pCustomMenu->addAction(m_delCol);
	m_pCustomMenu->addAction(m_AddRow);
	m_pCustomMenu->addAction(m_AddCol);
	m_pCustomMenu->addAction(m_SetReportTime);
	m_pCustomMenu->addAction(m_TimeCol);
//action
	m_CutAction = new QAction(tr("&Cut"), m_pCustomMenu);
	m_CutAction->setShortcut(QKeySequence::Cut);
	m_CutAction->setIcon(QIcon(":/images/cut.png"));

	m_CopyAction = new QAction(tr("&Copy"), m_pCustomMenu);
	m_CopyAction->setShortcut(QKeySequence::Copy);
	m_CopyAction->setIcon(QIcon(":/images/copy.png"));

	m_PasteAction = new QAction(tr("&Paste"), m_pCustomMenu);
	m_PasteAction->setShortcut(QKeySequence::Paste);
	m_PasteAction->setIcon(QIcon(":/images/paste.png"));

	m_pCustomMenu->addAction(m_CutAction);
	m_pCustomMenu->addAction(m_CopyAction);
	m_pCustomMenu->addAction(m_PasteAction);
	
	connect(m_CutAction, SIGNAL(triggered()), this, SLOT(slot_OnCut()));
	connect(m_CopyAction, SIGNAL(triggered()), this, SLOT(slot_onCopy()));
	connect(m_PasteAction, SIGNAL(triggered()), this, SLOT(slot_onPaste()));

	m_SpanAction = new QAction(tr("&SpanCell"), m_pCustomMenu);
	m_SpanAction->setIcon(QIcon(":/images/spancell.png"));

	m_UnSpanAction = new QAction(tr("&UnSpan"), m_pCustomMenu);
	m_UnSpanAction->setIcon(QIcon(":/images/splitcell.png"));

	m_pCustomMenu->addAction(m_SpanAction);
	m_pCustomMenu->addAction(m_UnSpanAction);
	connect(m_SpanAction, SIGNAL(triggered()), this, SLOT(slot_OnMergeCell()));
	connect(m_UnSpanAction, SIGNAL(triggered()), this, SLOT(slot_OnUnMergeCell()));

	m_pCellMenu = new QMenu(tr("Cells"),this);

	m_SelectSubMenu = new QMenu(tr("&Select"));
	m_SelectSubMenu->setIcon(QIcon(":/images/report_pie.ico"));
	m_SelectAllAction = new QAction(tr("&SelectAll"), m_SelectSubMenu);
	m_SelectRowAction = new QAction(tr("&SelectRow"), m_SelectSubMenu);
	m_SelectColumnAction = new QAction(tr("&SelectColumn"), m_SelectSubMenu);
	m_SelectSubMenu->addAction(m_SelectAllAction);
	m_SelectSubMenu->addAction(m_SelectRowAction);
	m_SelectSubMenu->addAction(m_SelectColumnAction);

	connect(m_SelectAllAction, SIGNAL(triggered()), this, SLOT(slot_OnSelAll()));
	connect(m_SelectRowAction, SIGNAL(triggered()), this, SLOT(slot_OnSelRow()));
	connect(m_SelectColumnAction, SIGNAL(triggered()), this, SLOT(slot_OnSelCol()));

	m_SetFont = new QAction(tr("&Font"), m_pCellMenu);
	m_FontColor = new QAction(tr("&FontColor"), m_pCellMenu);
	connect(m_SetFont, SIGNAL(triggered()), this, SLOT(slot_OnSetFont()));
	connect(m_FontColor, SIGNAL(triggered()), this, SLOT(slot_OnSetFontColor()));

	m_pCellMenu->addMenu(m_SelectSubMenu);
	m_pCellMenu->addAction(m_SetFont);
	m_pCellMenu->addAction(m_FontColor);
	m_pCustomMenu->addMenu(m_pCellMenu);

	m_AlignMenu = new QMenu(tr("Aligns"),this);
	m_LeftAlign = new QAction(tr("&Left"), m_AlignMenu);
	m_LeftAlign->setIcon(QIcon(":/images/ali_left.png"));

	m_MiddleAlign = new QAction(tr("&Middle"), m_AlignMenu);
	m_MiddleAlign->setIcon(QIcon(":/images/ali_center.png"));

	m_RightAlign = new QAction(tr("&Right"), m_AlignMenu);
	m_RightAlign->setIcon(QIcon(":/images/ali_right.png"));
	
	connect(m_LeftAlign, SIGNAL(triggered()), this, SLOT(slot_OnSetAlignLeft()));
	connect(m_MiddleAlign, SIGNAL(triggered()), this, SLOT(slot_OnSetAlignCenter()));
	connect(m_RightAlign, SIGNAL(triggered()), this, SLOT(slot_OnSetAlignRight()));

	m_AlignMenu->addAction(m_LeftAlign);
	m_AlignMenu->addAction(m_MiddleAlign);
	m_AlignMenu->addAction(m_RightAlign);
	m_pCustomMenu->addMenu(m_AlignMenu);	
}

/*! \fn void CReportEditView::slot_OnConnForGd()
********************************************************************************************************* 
** \brief CReportEditView::slot_OnConnForGd 
** \details 连接前景
** \return void 
** \author xingzhibing
** \date 2016年8月6日 
** \note 
********************************************************************************************************/
void CReportEditView::slot_OnConnForGd()
{

}

/*! \fn void CReportEditView::slot_OnDelForGd()
********************************************************************************************************* 
** \brief CReportEditView::slot_OnDelForGd 
** \details 删除前景
** \return void 
** \author xingzhibing
** \date 2016年8月6日 
** \note 
********************************************************************************************************/
void CReportEditView::slot_OnDelForGd()
{
	del();
}

/*! \fn void CReportEditView::slot_OnDelRow()
********************************************************************************************************* 
** \brief CReportEditView::slot_OnDelRow 
** \details 删除一行操作
** \return void 
** \author xingzhibing
** \date 2016年8月6日 
** \note 
********************************************************************************************************/
void CReportEditView::slot_OnDelRow()
{
	QList<QTableWidgetSelectionRange>ranges = selectedRanges();
//	int count = ranges.count();
	int topRow = ranges.at(0).topRow();
	int bottonRow = ranges.at(0).bottomRow();
	for (int i = topRow; i <= bottonRow; i++)
	{
		removeRow(topRow);
	}
}

/*! \fn void CReportEditView::slot_onDelCol()
********************************************************************************************************* 
** \brief CReportEditView::slot_onDelCol 
** \details 删除一列操作
** \return void 
** \author xingzhibing
** \date 2016年8月6日 
** \note 
********************************************************************************************************/
void CReportEditView::slot_onDelCol()
{
	QList<QTableWidgetSelectionRange>ranges = selectedRanges();
	int leftCol = ranges.at(0).leftColumn();
	int rightColumn = ranges.at(0).rightColumn();
	int nCount = rightColumn - leftCol;
	for (int i = leftCol; i < leftCol + nCount + 1; i++)
	{
		removeColumn(i);
	}

}

/*! \fn void CReportEditView::slot_OnAddRow()
********************************************************************************************************* 
** \brief CReportEditView::slot_OnAddRow 
** \details 添加一行操作
** \return void 
** \author xingzhibing
** \date 2016年8月6日 
** \note 
********************************************************************************************************/
void CReportEditView::slot_OnAddRow()
{
	int nRow = currentRow();
	insertRow(nRow + 1);
}

/*! \fn void CReportEditView::slot_OnAddCol()
********************************************************************************************************* 
** \brief CReportEditView::slot_OnAddCol 
** \details 添加一列操作
** \return void 
** \author xingzhibing
** \date 2016年8月6日 
** \note 
********************************************************************************************************/
void CReportEditView::slot_OnAddCol()
{
	int nCol = currentColumn();
	insertColumn(nCol + 1);
}

/*! \fn void CReportEditView::slot_OnSetRptTime()
********************************************************************************************************* 
** \brief CReportEditView::slot_OnSetRptTime 
** \details 设置报表时间
** \return void 
** \author xingzhibing
** \date 2016年8月6日 
** \note 
********************************************************************************************************/
void CReportEditView::slot_OnSetRptTime()
{

}

/*! \fn void CReportEditView::slot_onTimeCol()
********************************************************************************************************* 
** \brief CReportEditView::slot_onTimeCol 
** \details 时间列设置
** \return void 
** \author xingzhibing
** \date 2016年8月6日 
** \note 
********************************************************************************************************/
void CReportEditView::slot_onTimeCol()
{

}

/*! \fn void CReportEditView::slot_onCopy()
********************************************************************************************************* 
** \brief CReportEditView::slot_onCopy 
** \details 复制操作
** \return void 
** \author xingzhibing
** \date 2016年8月6日 
** \note 
********************************************************************************************************/
void CReportEditView::slot_onCopy()
{
	copy();
}

/*! \fn void CReportEditView::slot_OnCut()
********************************************************************************************************* 
** \brief CReportEditView::slot_OnCut 
** \details 剪切操作
** \return void 
** \author xingzhibing
** \date 2016年8月6日 
** \note 
********************************************************************************************************/
void CReportEditView::slot_OnCut()
{
	copy();
	del();
}

/*! \fn void CReportEditView::slot_onPaste()
********************************************************************************************************* 
** \brief CReportEditView::slot_onPaste 
** \details 黏贴操作
** \return void 
** \author xingzhibing
** \date 2016年8月6日 
** \note 
********************************************************************************************************/
void CReportEditView::slot_onPaste()
{
	paste();
}

/*! \fn void CReportEditView::slot_OnSpanCell()
********************************************************************************************************* 
** \brief CReportEditView::slot_OnSpanCell 
** \details 合并单元格
** \return void 
** \author xingzhibing
** \date 2016年8月6日 
** \note 
********************************************************************************************************/
void CReportEditView::slot_OnMergeCell()
{
	SetMerge();
}

/*! \fn void CReportEditView::slot_OnSplitCell()
********************************************************************************************************* 
** \brief CReportEditView::slot_OnSplitCell 
** \details 拆分单元格
** \return void 
** \author xingzhibing
** \date 2016年8月6日 
** \note 
********************************************************************************************************/
void CReportEditView::slot_OnUnMergeCell()
{
	SetUnMerge();
}

/*! \fn void CReportEditView::slot_OnSelRow()
********************************************************************************************************* 
** \brief CReportEditView::slot_OnSelRow 
** \details 选取行
** \return void 
** \author xingzhibing
** \date 2016年8月6日 
** \note 
********************************************************************************************************/
void CReportEditView::slot_OnSelRow()
{
	selectCurrentRow();
}

/*! \fn void CReportEditView::slot_OnSelCol()
********************************************************************************************************* 
** \brief CReportEditView::slot_OnSelCol 
** \details 选取列
** \return void 
** \author xingzhibing
** \date 2016年8月6日 
** \note 
********************************************************************************************************/
void CReportEditView::slot_OnSelCol()
{
	selectCurrentColumn();
}

/*! \fn void CReportEditView::slot_OnSelAll()
********************************************************************************************************* 
** \brief CReportEditView::slot_OnSelAll 
** \details 选中所有
** \return void 
** \author xingzhibing
** \date 2016年8月6日 
** \note 
********************************************************************************************************/
void CReportEditView::slot_OnSelAll()
{
	selectAll();
}

/*! \fn void CReportEditView::slot_OnSetFont
********************************************************************************************************* 
** \brief CReportEditView::slot_OnSetFont 
** \details 设置文本字体
** \return void 
** \author xingzhibing
** \date 2016年8月6日 
** \note 
********************************************************************************************************/
void CReportEditView::slot_OnSetFont()
{
	SetMyFont();
}

/*! \fn void CReportEditView::slot_OnSetFontColor()
********************************************************************************************************* 
** \brief CReportEditView::slot_OnSetFontColor 
** \details 设置字体颜色
** \return void 
** \author xingzhibing
** \date 2016年8月6日 
** \note 
********************************************************************************************************/
void CReportEditView::slot_OnSetFontColor()
{
	SetFontColor();
}

/*! \fn void CReportEditView::slot_OnSetAlognRight()
********************************************************************************************************* 
** \brief CReportEditView::slot_OnSetAlognRight 
** \details 字体右对齐
** \return void 
** \author xingzhibing
** \date 2016年8月6日 
** \note 
********************************************************************************************************/
void CReportEditView::slot_OnSetAlignRight()
{
	SetFontRight();
}

/*! \fn void CReportEditView::slot_OnSetAlignLeft()
********************************************************************************************************* 
** \brief CReportEditView::slot_OnSetAlignLeft 
** \details 字体左对齐
** \return void 
** \author xingzhibing
** \date 2016年8月6日 
** \note 
********************************************************************************************************/
void CReportEditView::slot_OnSetAlignLeft()
{
	SetFontLeft();
}

/*! \fn void CReportEditView::slot_OnSetAlignCenter()
********************************************************************************************************* 
** \brief CReportEditView::slot_OnSetAlignCenter 
** \details 字体居中
** \return void 
** \author xingzhibing
** \date 2016年8月6日 
** \note 
********************************************************************************************************/
void CReportEditView::slot_OnSetAlignCenter()
{
	SetFontMiddle();
}

/*! \fn void CReportEditView::SetWinModify()
********************************************************************************************************* 
** \brief CReportEditView::SetWinModify 
** \details 文件修改标记
** \return void 
** \author xingzhibing
** \date 2016年8月16日 
** \note 
********************************************************************************************************/
void CReportEditView::SetWinModify()
{
	if (GetRptFile())
	{
		//设置文档状态未Dirty状态
		GetRptFile()->SetModifyFlag(true);
	}
	SetModifyFlag(true);
}

/*! \fn void CReportEditView::GetCellData()
********************************************************************************************************* 
** \brief CReportEditView::GetCellData 
** \details 获取文件中各个Cell状态值
** \return void 
** \author xingzhibing
** \date 2016年8月20日 
** \note 
********************************************************************************************************/
void CReportEditView::GetCellData()
{
	
	int nRow = GetEndRow();
	int nCol = GetEndColumn();

	CRptFile* pRptFile = GetRptFile();
	Q_ASSERT(pRptFile);

	QMap< int, float > mapRowHeight;
	QMap< int, float > mapColWidth;

	for (int i = 0; i < nRow;++i)
	{
		mapRowHeight[i] = rowHeight(i);
	}

	for (int i = 0; i < nCol;++i)
	{
		mapColWidth[i] = columnWidth(i);
	}

	pRptFile->GetRptInfo()->m_RowHeight = mapRowHeight;
	pRptFile->GetRptInfo()->m_ColWidth = mapColWidth;

	CheckMergeInfo();

	pRptFile->GetRptInfo()->m_MergeItemList = GetSpanData();

	//表格内容获取	
	QList<CCellData* >lstCellData;
	CCellData* pCellData = Q_NULLPTR;
	for (int i = 0; i < nRow ; ++i )
	{
		for (int j = 0; j < nCol ; ++j )
		{
			QString szContent = Formula(i, j);
			if (!szContent.isEmpty())
			{
				pCellData = new CCellData;
				*pCellData = cell(i, j)->GetFormatData();
				lstCellData.append(pCellData);
			}			
		}
	}
	if (GetRptFile())
	{
		GetRptFile()->SetCellList(lstCellData);
	}	
}


CWidget::CWidget(CTabWidget* pTableWidget,IMainModuleInterface* pCore) 
{
	Q_ASSERT(pTableWidget ||pCore);

	m_pTableWidget = pTableWidget;

	m_pCore = pCore;

	m_pCurrentReportWidget = Q_NULLPTR;

	QObject::connect(m_pTableWidget, SIGNAL(SendCloseWidget(QWidget*)), this, SLOT(slot_OnRemoveItem(QWidget*)));

	QObject::connect(m_pTableWidget, SIGNAL(currentChanged(int )), this, SLOT(slot_OnSelectChanged(int )));
}


CWidget::~CWidget()
{
	//QObject::disconnect(m_pTableWidget, SIGNAL(SendCloseWidget(QWidget*)), this, SLOT(slot_OnRemoveItem(QWidget*)));

	//QObject::disconnect(m_pTableWidget, SIGNAL(currentChanged(int)), this, SLOT(slot_OnSelectChanged(int)));
}
#if 0
/*! \fn void CWidget::AddTabCtrl(CTableInfo* tableInfo)
********************************************************************************************************* 
** \brief CWidget::AddTabCtrl 
** \details 添加Tab控件
** \param tableInfo 
** \return void 
** \author xingzhibing
** \date 2016年7月25日 
** \note 
********************************************************************************************************/
void CWidget::AddTabCtrl(CTableInfo* tableInfo, bool isNewFile)
{
	Q_ASSERT(tableInfo);
	if (IsItemExist(tableInfo))
	{
		//若文件信息存在，则删除已有信息
		delete tableInfo;
		tableInfo = NULL;
		return;
	}

	CReportEditView *pBaseWidget = new CReportEditView(tableInfo,m_pCore);

	m_TabList.append(tableInfo);
	if (!isNewFile)
	{
		//加载文件
		pBaseWidget->readFile(tableInfo->m_szTablePath);

		tableInfo->m_nTableType = pBaseWidget->GetTableType();
		tableInfo->m_szTableName = QFileInfo(tableInfo->m_szTablePath).baseName();
		tableInfo->m_RowCount = pBaseWidget->GetRowCount();
		tableInfo->m_ColCount = pBaseWidget->GetColumnCount();
	}

	m_pTableWidget->AddTab(pBaseWidget, tableInfo->m_szTableName, tableInfo->m_nTableType, "");
}
#endif
/*! \fn void CWidget::AddTabCtrl(CRptFile* tableInfo)
********************************************************************************************************* 
** \brief CWidget::AddTabCtrl 
** \details 添加报表页面
** \param tableInfo 
** \return void 
** \author xingzhibing
** \date 2016年8月15日 
** \note 
********************************************************************************************************/
void CWidget::AddTabCtrl(CRptFile* pRptFile)
{
	Q_ASSERT(pRptFile);

	if ( pRptFile == Q_NULLPTR )
	{
		return;
	}

	int nCount = m_pTableWidget->count();

	for (int i = 0; i < nCount; ++i)
	{
		CReportEditView* pView = dynamic_cast<CReportEditView*>(m_pTableWidget->widget(i));
		if (pView)
		{
			CRptFile *pFile = pView->GetRptFile(); 
			if (pFile && (pFile == pRptFile))
			{
				//文件已经存在
				m_pTableWidget->setCurrentIndex(i);
				return;
			}
		}
	}

  	CReportEditView *pBaseWidget = new CReportEditView(pRptFile, m_pCore);

	QObject::connect(pBaseWidget, SIGNAL(cellPressed(int, int)), this, SLOT(slot_TabClicked(int, int)));

  	QFileInfo szName =QFileInfo(pRptFile->GetRptFileName());

	m_pTableWidget->AddTab(pBaseWidget,szName.baseName(), 1, "");
}



/*! \fn  CReportEditView* CWidget::GetCurrerntTable()
********************************************************************************************************* 
** \brief CWidget::GetCurrerntTable 
** \details 获取当前报表
** \return QSpreadSheet* 
** \author xingzhibing
** \date 2016年7月25日 
** \note 
********************************************************************************************************/
CReportEditView* CWidget::GetCurrerntTable()
{
	if (m_pCurrentReportWidget!=Q_NULLPTR)
	{
		return m_pCurrentReportWidget;
	}
	else
	{
		return Q_NULLPTR;
	}	
}

void CWidget::slot_OnRemoveItem(QWidget* pWidget)
{
	Q_ASSERT(pWidget);
	auto pTmp = dynamic_cast<CReportEditView*>(pWidget);
	if (pTmp)
	{
		CTableInfo *pInfo = pTmp->GetTableInfo();
		if (pInfo)
		{
			m_TabList.removeOne(pInfo);
		}
	}
}

/*! \fn bool CWidget::IsItemExist(CTableInfo* pItem)
********************************************************************************************************* 
** \brief CWidget::IsItemExist 
** \details 防止重复添加
** \param pItem 
** \return bool 
** \author xingzhibing
** \date 2016年7月25日 
** \note 
********************************************************************************************************/
bool CWidget::IsItemExist(CTableInfo* pItem)
{
	Q_ASSERT(pItem);
	if (pItem == nullptr)
		return false;

	if (m_TabList.isEmpty())
	{
		return false;
	}
	for each (CTableInfo* var in m_TabList)
	{
		if (var)
		{
			if (var->m_szTablePath == pItem->m_szTablePath)
			{
				return true;
			}
		}
		else
		{
			m_TabList.removeOne(var);
		}
	}
	return false;
}

/*! \fn void CWidget::slot_OnSelectChanged(int nIndex)
********************************************************************************************************* 
** \brief CWidget::slot_OnSelectChanged 
** \details 页面切换操作，获取当前操作界面，获取需要操作报表文件
** \param nIndex 
** \return void 
** \author xingzhibing
** \date 2016年7月25日 
** \note 
********************************************************************************************************/
void CWidget::slot_OnSelectChanged(int nIndex)
{
	CReportEditView* pWidget =dynamic_cast<CReportEditView* >( m_pTableWidget->widget(nIndex));
	if (pWidget==Q_NULLPTR)
	{
		//若切换页面不为报表页面，则设置当前报表页面为空
		m_pCurrentReportWidget = Q_NULLPTR;
		return;
	}
	m_pCurrentReportWidget = pWidget;
}

void CWidget::slot_TabClicked(int, int)
{
	emit sig_TabClicked(true);
}
