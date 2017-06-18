
#include <QThread>
#include <QMessageBox>
#include <QDesktopWidget>
#include <QDockWidget>
#include <QtPrintSupport/QPrinter>
#include <QtPrintSupport/QPrintDialog>
#include <QFileDialog>
#include <QToolBox>
#include <QLayout>
#include <QTableWidget>
#include <QHeaderView>
#include <QCheckBox>

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "graphview.h"
#include "graphscene.h"
#include "graph_file.h"

#include "colour_define.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

	setUnifiedTitleAndToolBarOnMac(true);

	ui->statusBar->showMessage(QStringLiteral("��ӭʹ�û�ͼ���ߣ�"),5000);
	CreateMenus();
	CreateLayerToolBar();

	CreateToolBox();
}

MainWindow::~MainWindow()
{
    delete ui;

// 	delete m_pGraphScene;
// 	m_pGraphScene = nullptr;
// 	delete m_pGraphView;
 	m_pGraphScene->deleteLater();
 	m_pGraphView->deleteLater();

}

void MainWindow::GetScreenInfo()
{
	QDesktopWidget *pDesktopWidget = QApplication::desktop();
	QRectF rcDesktop = pDesktopWidget->availableGeometry();

	QRect rcScreen = pDesktopWidget->screenGeometry();
	int nScreenCount = pDesktopWidget->screenCount();
}

void MainWindow::NewEmptyFile()
{
	m_pGraphScene = new CGraphScene(this);
	//QRectF rcTemp(0,0,1600,800);
	// �趨��������ʾ����  1920 *1280
	//m_pGraphScene->setSceneRect(-rcTemp.width() / 2, -rcTemp.height() / 2, rcTemp.width(), rcTemp.height());
//	m_pGraphScene->setSceneRect(0, 0, rcTemp.width(), rcTemp.height());

	//m_pGraphScene->addLine(10, 10, 150, 300);
	//m_pGraphScene->addRect(20, 20, 20, 20);

	m_pGraphView = new CGraphView(m_pGraphScene, this);
	
//	m_pGraphView->showMaximized();
//	m_pGraphView->setMainWindow(this);
	this->setCentralWidget(m_pGraphView);

	UpdateUI();
//	GetScreenInfo(); 
}

void  MainWindow::CreateMenus()
{
	CreateMenu();
}

void MainWindow::SelectDrawTool()
{
//	QMessageBox::information(this, tr("title"), tr("���ļ�"));
	Q_ASSERT(m_pGraphView);
	if (m_pGraphView == nullptr)
		return;
	Q_ASSERT(m_pDrawToolActGrp);
	if (m_pDrawToolActGrp)
	{
		QAction *pAct = m_pDrawToolActGrp->checkedAction();
		if (pAct)
		{
			//pAct->setChecked(true);
			if (pAct == m_pDrawToolSel)
			{
				m_pGraphView->m_nDrawTool = DRAW_TOOLS_SEL;
			}
			else if (pAct == m_pDrawToolLine)
			{
				m_pGraphView->m_nDrawTool = DRAW_TOOLS_LINE;
			}
			else if (pAct == m_pDrawToolRect)
			{
				m_pGraphView->m_nDrawTool = DRAW_TOOLS_RECTANGLE;
			}
			else if (pAct == m_pDrawToolRound)
			{
				m_pGraphView->m_nDrawTool = DRAW_TOOLS_ROUND;
			}
			else if (pAct == m_pDrawToolRoundRect)
			{
				m_pGraphView->m_nDrawTool = DRAW_TOOLS_ROUNDRECT;
			}
		}
	}

}

void MainWindow::CreateMenu()
{
	QMenuBar *pBar = menuBar();
	Q_ASSERT(pBar);
	if (pBar)
	{
		m_pFileMenu = pBar->addMenu(QStringLiteral("�ļ�(&F)"));
		Q_ASSERT(m_pFileMenu);
		if (m_pFileMenu)
		{		
			QAction *action = m_pFileMenu->addAction(QStringLiteral("��ӡ(&P)"));
			connect(action, SIGNAL(triggered()), this, SLOT(PrintGraph()));

			action = m_pFileMenu->addAction(QStringLiteral("����(&E)"));
			connect(action, SIGNAL(triggered()), this, SLOT(ExportImage()));

			action = m_pFileMenu->addAction(QStringLiteral("�˳�(&Q)"));
			connect(action, SIGNAL(triggered()), this, SLOT(close()));
		}
		m_pEditMenu = pBar->addMenu(QStringLiteral("�༭(&E)"));
		Q_ASSERT(m_pEditMenu);
		if (m_pEditMenu)
		{
			QAction *action = m_pEditMenu->addAction(QStringLiteral("����(&T)"));
			connect(action, SIGNAL(triggered()), this, SLOT(OnCut()));

			action = m_pEditMenu->addAction(QStringLiteral("����(&C)"));
			connect(action, SIGNAL(triggered()), this, SLOT(OnCopy()));

			action = m_pEditMenu->addAction(QStringLiteral("ճ��(&P)"));
			connect(action, SIGNAL(triggered()), this, SLOT(OnPaste()));

			action = m_pEditMenu->addAction(QStringLiteral("ɾ��(&D)"));
			connect(action, SIGNAL(triggered()), this, SLOT(OnDelete()));

			m_pEditMenu->addSeparator();

			action = m_pEditMenu->addAction(QStringLiteral("���(&G)"));
			connect(action, SIGNAL(triggered()), this, SLOT(OnWidgetsGroup()));

			action = m_pEditMenu->addAction(QStringLiteral("ȡ�����(&U)"));
			connect(action, SIGNAL(triggered()), this, SLOT(OnWidgetsBreak()));

			action = m_pEditMenu->addAction(QStringLiteral("�༭���(&E)"));
			connect(action, SIGNAL(triggered()), this, SLOT(OnWidgetsBreak()));
		}

		m_pElementMenu = pBar->addMenu(QStringLiteral("ͼԪ(&W)"));
		Q_ASSERT(m_pElementMenu);
		if (m_pElementMenu)
		{
			m_pDrawToolActGrp = new QActionGroup(this);
			m_pDrawToolActGrp->setExclusive(true);

			QAction *pAction = m_pElementMenu->addAction(QStringLiteral("ѡ��(&S)"));
			pAction->setCheckable(true);
			connect(pAction, SIGNAL(triggered()), this, SLOT(SelectDrawTool()));
			pAction->setActionGroup(m_pDrawToolActGrp);
			m_pDrawToolSel = pAction;

// 			QAction *pAction = new QAction(this);
// 			m_pDrawToolActGrp->addAction(pAction);
// 			connect(pAction, &QAction::triggered, this, MainWindow::SelectDrawTool);

			pAction = m_pElementMenu->addAction(QStringLiteral("ֱ��(&L)"));
			pAction->setCheckable(true);
			connect(pAction, SIGNAL(triggered()), this, SLOT(SelectDrawTool()));
			pAction->setActionGroup(m_pDrawToolActGrp);
			m_pDrawToolLine = pAction;

			pAction = m_pElementMenu->addAction(QStringLiteral("����(&R)"));
			pAction->setCheckable(true);
			connect(pAction, SIGNAL(triggered()), this, SLOT(SelectDrawTool()));
			pAction->setActionGroup(m_pDrawToolActGrp);
			m_pDrawToolRect = pAction;

			pAction = m_pElementMenu->addAction(QStringLiteral("Բ��(&C)"));
			pAction->setCheckable(true);
			connect(pAction, SIGNAL(triggered()), this, SLOT(SelectDrawTool()));
			pAction->setActionGroup(m_pDrawToolActGrp);
			m_pDrawToolRound = pAction;

			pAction = m_pElementMenu->addAction(QStringLiteral("Բ�Ǿ���(&R)"));
			pAction->setCheckable(true);
			connect(pAction, SIGNAL(triggered()), this, SLOT(SelectDrawTool()));
			pAction->setActionGroup(m_pDrawToolActGrp);
			m_pDrawToolRoundRect = pAction;

		}

		m_pLayerMenu = pBar->addMenu(QStringLiteral("ͼ��(&L)"));
		Q_ASSERT(m_pLayerMenu);
		if (m_pLayerMenu)
		{
			QAction * pAction = m_pLayerMenu->addAction(QStringLiteral("ͼ��1"));

			pAction = m_pLayerMenu->addAction(QStringLiteral("ͼ��2"));
			pAction = m_pLayerMenu->addAction(QStringLiteral("ͼ��3"));
			pAction = m_pLayerMenu->addAction(QStringLiteral("ͼ��4"));
			pAction = m_pLayerMenu->addAction(QStringLiteral("ͼ��5"));
			pAction = m_pLayerMenu->addAction(QStringLiteral("ͼ��6"));
			pAction = m_pLayerMenu->addAction(QStringLiteral("ͼ��7"));
			pAction = m_pLayerMenu->addAction(QStringLiteral("ͼ��8"));
		}
		m_pArrangeMenu = pBar->addMenu(QStringLiteral("����"));
		if (m_pArrangeMenu)
		{
			QAction *pAction = m_pArrangeMenu->addAction(QStringLiteral("�Զ�����"));
			connect(pAction, &QAction::triggered, this, &MainWindow::OnAutoAlign);

			pAction = m_pArrangeMenu->addAction(QStringLiteral("�����"));
			connect(pAction, &QAction::triggered, this, &MainWindow::OnLeftAlign);

			pAction = m_pArrangeMenu->addAction(QStringLiteral("ˮƽ����"));
			connect(pAction, &QAction::triggered, this, &MainWindow::OnCenterHorzAlign);

			pAction = m_pArrangeMenu->addAction(QStringLiteral("�Ҷ���"));
			connect(pAction, &QAction::triggered, this, &MainWindow::OnRightAlign);

			pAction = m_pArrangeMenu->addAction(QStringLiteral("���˶���"));
			connect(pAction, &QAction::triggered, this, &MainWindow::OnTopAlign);

			pAction = m_pArrangeMenu->addAction(QStringLiteral("��ֱ����"));
			connect(pAction, &QAction::triggered, this, &MainWindow::OnCenterVertAlign);

			pAction = m_pArrangeMenu->addAction(QStringLiteral("�׶˶���"));
			connect(pAction, &QAction::triggered, this, &MainWindow::OnButtomAlign);

		}

		m_pDebugMenu = pBar->addMenu(QStringLiteral("����(&D)"));
		Q_ASSERT(m_pDebugMenu);
		if (m_pDebugMenu)
		{
			QAction *action = m_pDebugMenu->addAction(QStringLiteral("ģ��(&S)"));
			connect(action, SIGNAL(triggered()), this, SLOT(Simulation()));
		}
	} 
}

void MainWindow::Simulation()
{
	QMessageBox::information(this, QStringLiteral("ģ������"), QStringLiteral("ģ������..."));
}

void MainWindow::PrintGraph()
{
	QMessageBox::information(this, QStringLiteral("��ӡ"), QStringLiteral("��ӡ..."));
	QPrinter *printer = new QPrinter();
	QPrintDialog *printDialog = new QPrintDialog(printer, this);
	if (printDialog->exec())
	{
// 		QPainter painter(printer);
// 		QRect rect = painter.viewport();
// 		QSize size = mImage->size();
// 		size.scale(rect.size(), Qt::KeepAspectRatio);
// 		painter.setViewport(rect.x(), rect.y(), size.width(), size.height());
// 		painter.setWindow(mImage->rect());
	//	painter.drawImage(0, 0, *mImage);
	}
	delete printer;
}
void MainWindow::ExportImage()
{
	Q_ASSERT(m_pGraphScene);
	if (m_pGraphScene == nullptr)
		return;

	QFileDialog::Options options;
	options = 0;
	QString selectedFilter;
	QString fileName = QFileDialog::getSaveFileName(this, QStringLiteral("����..."), QStringLiteral(".jpg"),
		QStringLiteral("Jpg (*.jpg);;Png (*.png);;Pdf (*.pdf);;Postscript (*.ps)"),
		&selectedFilter,options);
	if (!fileName.isEmpty())
	{
#ifdef DEBUG
		//std::cout << "Selected Filter: " << qPrintable(selectedFilter) << std::endl;
#endif
		if ((selectedFilter == "Pdf (*.pdf)") || (selectedFilter == "Postscript (*.ps)"))
		{
			QRectF rect = m_pGraphScene->itemsBoundingRect(); // Bonding der Elemente in scene
			QPrinter printer;
			printer.setOutputFileName(fileName);
			QSizeF size = printer.paperSize(QPrinter::Millimeter); // definiere Paper mit gleichen Aspectratio wie rect
			size.setHeight(size.width()*rect.height() / rect.width());
			printer.setPaperSize(size, QPrinter::Millimeter);
			printer.setPageMargins(0, 0, 0, 0, QPrinter::Millimeter);
			QPainter painter(&printer);// generate PDF/PS
			painter.setRenderHint(QPainter::Antialiasing);
			m_pGraphScene->render(&painter, QRectF(), rect);
		}
		else
		{
			QPixmap pixmap(1000, 1000);
			pixmap.fill();
			QPainter painter(&pixmap);
			painter.setRenderHint(QPainter::Antialiasing);
			QRectF rect = m_pGraphScene->itemsBoundingRect();
			m_pGraphScene->render(&painter, QRectF(), rect);
			painter.end();

			pixmap.save(fileName);
		}
	}
}
void MainWindow::OnCopy()
{
	Q_ASSERT(m_pGraphView);
	if (m_pGraphView)
	{
		m_pGraphView->OnCopy();
	}
}

void MainWindow::OnPaste()
{
	Q_ASSERT(m_pGraphView);
	if (m_pGraphView)
	{
		m_pGraphView->OnPaste();
	}
}

void MainWindow::OnDelete()
{
	Q_ASSERT(m_pGraphView);
	if (m_pGraphView)
	{
		m_pGraphView->OnDelete();
	}
}

void MainWindow::OnCut()
{
	Q_ASSERT(m_pGraphView);
	if (m_pGraphView)
	{
		m_pGraphView->OnCut();
	}
}
void MainWindow::OnWidgetsGroup()
{
	Q_ASSERT(m_pGraphView);
	if (m_pGraphView)
	{
		m_pGraphView->OnWidgetsGroup();
	}
}
void MainWindow::OnWidgetsBreak()
{
	Q_ASSERT(m_pGraphView);
	if (m_pGraphView)
	{
		m_pGraphView->OnWidgetsBreak();
	}
}
void MainWindow::OnAutoAlign()
{
	Q_ASSERT(m_pGraphView);
	if (m_pGraphView)
	{
		m_pGraphView->OnAutoAlign();
	}
}
void MainWindow::OnLeftAlign()
{
	Q_ASSERT(m_pGraphView);
	if (m_pGraphView)
	{
		m_pGraphView->OnLeftAlign();
	}
}
void MainWindow::OnRightAlign()
{
	Q_ASSERT(m_pGraphView);
	if (m_pGraphView)
	{
		m_pGraphView->OnRightAlign();
	}
}
void MainWindow::OnCenterHorzAlign()
{
	Q_ASSERT(m_pGraphView);
	if (m_pGraphView)
	{
		m_pGraphView->OnCenterHorzAlign();
	}
}
void MainWindow::OnTopAlign()
{
	Q_ASSERT(m_pGraphView);
	if (m_pGraphView)
	{
		m_pGraphView->OnTopAlign();
	}
}
void MainWindow::OnCenterVertAlign()
{
	Q_ASSERT(m_pGraphView);
	if (m_pGraphView)
	{
		m_pGraphView->OnCenterVertAlign();
	}
}
void MainWindow::OnButtomAlign()
{
	Q_ASSERT(m_pGraphView);
	if (m_pGraphView)
	{
		m_pGraphView->OnButtomAlign();
	}
}
void MainWindow::CreateLayerToolBar()
{
	m_pLayerToolBar = addToolBar(QStringLiteral("ͼ��"));
}

void MainWindow::CreateToolBox()
{
	QDockWidget *pDockWidget = new QDockWidget(this);
	pDockWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
	pDockWidget->setFixedWidth(260);
// 	m_pToolBox = new QToolBox(pDockWidget);
// 	m_pToolBox->layout()->setSpacing(NULL);
// 	pDockWidget->setWidget(m_pToolBox);
 	addDockWidget(Qt::RightDockWidgetArea, pDockWidget);

//	m_pLayerList = new QListWidget(m_pToolBox);

	QTabWidget *pTab = new QTabWidget(pDockWidget);

	pDockWidget->setWidget(pTab);
	m_pLayerList = new QTableWidget;

	pTab->addTab(m_pLayerList, QStringLiteral("ͼ��"));

	m_pLayerList->setColumnCount(3);
	m_pLayerList->setRowCount(16);
	m_pLayerList->horizontalHeader()->setSectionsClickable(false);
	m_pLayerList->setEditTriggers(QAbstractItemView::NoEditTriggers);
	m_pLayerList->verticalHeader()->setVisible(false);
	m_pLayerList->horizontalHeader()->setVisible(false);
	m_pLayerList->setSelectionMode(QAbstractItemView::NoSelection);
//	m_pLayerList->setSelectionBehavior(QAbstractItemView::SelectItems);
	
	m_pLayerList->setColumnWidth(0, 40);
	m_pLayerList->setColumnWidth(1, 120);
	m_pLayerList->setColumnWidth(2, 60);

	m_iconEyeOn.addFile(":images/eye_on.png");
	m_iconEyeOff.addFile(":images/eye_off.png");
	m_iconLeft.addFile(":images/left.png");
}

void  MainWindow::slotItemClicked(QTableWidgetItem* pItem)
{
	Q_ASSERT(pItem);
	if (pItem == nullptr)
		return;
	Q_ASSERT(m_pGraphView);
	if (m_pGraphView == nullptr)
		return;

	int nCol = pItem->column();
	int nRow = pItem->row();
	
	if (nCol == 0)
	{
		QVariant varIdx = pItem->data(Qt::UserRole + 1);
		int nIdx = varIdx.toInt();

		QVariant var = pItem->data(Qt::UserRole);
		bool bDsp = false;
		if (var.toInt() != 0)
		{
			pItem->setIcon(m_iconEyeOff);
			pItem->setData(Qt::UserRole, 0);
			bDsp = false;
		}
		else
		{
			pItem->setIcon(m_iconEyeOn);
			pItem->setData(Qt::UserRole, 1);
			bDsp = true;
		}
		// 
		SetLayerVisible(nIdx - 1, bDsp);
	}
	else if ( nCol == 1 )
	{// ���ѡ��ǰ�༭��ͼ��
		// ���⣬�����ж��ͬʱ�ڱ༭
		QTableWidgetItem* pEditbleItem = m_pLayerList->item(nRow, 0);
		QVariant var = pEditbleItem->data(Qt::UserRole);
		if (var.toInt() == 0)
		{// ���ɱ༭, 

		}
		else
		{// ���Ա༭
			SetLayerRowSelState(nRow, true);

			m_pGraphView->SetCurEditingLayer(nRow + 1);
			// ������Ա༭ ���õ�ǰ��ͼ���Ϊ��ǰ�ı༭ ����������ͼ��ȡ��״̬
			SetOtherItemsUnSelected(nRow);
		}
	}
	else if (nCol == 2)
	{
		// ���⣬�����ж��ͬʱ�ڱ༭
		QTableWidgetItem* pEditbleItem = m_pLayerList->item(nRow, 0);
		QVariant var = pEditbleItem->data(Qt::UserRole);
		if (var.toInt() == 0)
		{// ���ɱ༭, 

		}
		else
		{// ���Ա༭		 
			SetLayerRowSelState(nRow, true);
			m_pGraphView->SetCurEditingLayer(nRow + 1);
			// ������Ա༭ ���õ�ǰ��ͼ���Ϊ��ǰ�ı༭ ����������ͼ��ȡ��״̬
			SetOtherItemsUnSelected(nRow);
		}
	}
}

void MainWindow::SetOtherItemsUnSelected(int nCurRow)
{
	int nRowCount = m_pLayerList->rowCount();
	int i = 0;
	 
	for (i = 0; i < nRowCount; i++)
	{
		if (i != nCurRow)
		{
			SetLayerRowSelState(i, false);		 
		} 
	}	
}

void MainWindow::slotCellClicked(int row, int column)
{
	Q_ASSERT(m_pGraphView);
	if (m_pGraphView == nullptr)
		return;

	if (column == 1)
	{// �ڶ���
	//	QWidget *pWidget = m_pLayerList->item(row, column);
		QTableWidgetItem *pItem = dynamic_cast<QTableWidgetItem*> (m_pLayerList->item(row, column));
		Q_ASSERT(pItem);
		if (pItem == nullptr)
			return;
// 		QBrush br = pItem->background();
// 		br.setColor(Qt::blue);
// 		pItem->setBackground(br);
		pItem->setBackgroundColor(Qt::blue);
	}
}

void MainWindow::UpdateUI()
{
	Q_ASSERT(m_pGraphScene);
	if (m_pGraphScene == nullptr)
		return;

	m_pGraphScene->NewFile();

	std::shared_ptr<CGraphFile> pGrahFile = m_pGraphScene->GetGraphFile();

	unsigned int nLayerCount = pGrahFile->GetLayerCount();	

	QTableWidgetItem *pItem = nullptr;
	std::shared_ptr<CGraphicsLayer> pLayer;
	//CGraphicsLayer* pLayer=nullptr;
	unsigned int i = 0;
	for (i = 1; i <= CGraphFile::MAX_LAYER_CNT && i<=nLayerCount ; i++)
	{
		pLayer = pGrahFile->GetLayer(i);
		pItem = new QTableWidgetItem;
		pItem->setIcon(m_iconEyeOn);
		m_pLayerList->setItem(i-1, 0, pItem);
		pItem->setData(Qt::UserRole, 1);
		pItem->setData(Qt::UserRole + 1, i + 1);

		QString szTxt = QString(QStringLiteral("ͼ�� %1").arg(i));

		pItem = new QTableWidgetItem(szTxt);
		pItem->setIcon(QIcon(":images/layers.png"));
		m_pLayerList->setItem(i-1, 1, pItem);
		pItem->setData(Qt::UserRole, i + 1); // ͼ���+1

		pItem = new QTableWidgetItem;
		pItem->setData(Qt::UserRole, i + 1); // ͼ���+1
		m_pLayerList->setItem(i - 1, 2, pItem);
	}
	unsigned int nCurEditingLayerIdx = m_pGraphView->GetCurEditingLayerIdx();
	if (nCurEditingLayerIdx)
	{
		SetLayerRowSelState(nCurEditingLayerIdx-1, true);
		SetOtherItemsUnSelected(nCurEditingLayerIdx-1);
	}
	else
	{
		SetOtherItemsUnSelected(-1);
	}

	connect(m_pLayerList, SIGNAL(itemClicked(QTableWidgetItem*)), this, SLOT(slotItemClicked(QTableWidgetItem*)));
//	connect(m_pLayerList, SIGNAL(cellClicked(int, int)), this, SLOT(slotCellClicked( int,int)));
}
/*! \fn void MainWindow::SetLayerVisible(unsigned int nLayIdx, bool bDisp)
********************************************************************************************************* 
** \brief MainWindow::SetLayerVisible 
** \details ����ĳ��ͼ���Ƿ�ɼ�
** \param nLayIdx 
** \param bDisp 
** \return void 
** \author LiJin
** \date 2016��2��1�� 
** \note 
********************************************************************************************************/
void MainWindow::SetLayerVisible(unsigned int nLayIdx, bool bDisp)
{
	m_pGraphView->SetLayerVisible(nLayIdx, bDisp);

	unsigned int nCurEditingIdx = m_pGraphView->GetCurEditingLayerIdx();
	// �����ǰ���ص�ͼ���������ڱ༭��ͼ�㣬���л�״̬
	if (nCurEditingIdx > 0)
	{
		SetOtherItemsUnSelected(nCurEditingIdx - 1);
		SetLayerRowSelState(nCurEditingIdx - 1, true);
	}
	else
	{// ���еĶ����ɼ��ˣ���ȡ��
		SetOtherItemsUnSelected(-1);
	}
}
void MainWindow::SetLayerRowSelState(int nRow, bool bSelected)
{
	int nRowCount = m_pLayerList->rowCount();

	Q_ASSERT(nRow >= 0 && nRow < nRowCount);

	QTableWidgetItem *pSecondItem = nullptr;
	QTableWidgetItem *pThirdItem = nullptr;

	pSecondItem = m_pLayerList->item(nRow, 1);
	pThirdItem = m_pLayerList->item(nRow, 2);
	Q_ASSERT(pSecondItem);
	Q_ASSERT(pThirdItem);

	if (bSelected)
	{
		if (pSecondItem)
		{
			pSecondItem->setBackgroundColor(XGraphics::LightBlue);
		}
		if (pThirdItem)
		{
			pThirdItem->setIcon(m_iconLeft);
		}
	}
	else
	{
		if (pSecondItem)
		{
			pSecondItem->setBackgroundColor(Qt::white);
		}
		if (pThirdItem)
		{
			pThirdItem->setIcon(m_iconNull);
		}
	}
}
