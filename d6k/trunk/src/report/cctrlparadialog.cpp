#include <QMessageBox>
#include "ctrlparadialog.h"
#include "configinfo.h"

CCtrlParaDialog::CCtrlParaDialog(CCtrlData* pData,QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	Q_ASSERT(pData);

	m_CtrlData = Q_NULLPTR;
	m_pCurrentLineEdit = Q_NULLPTR;

	m_CtrlData = pData;	
	//限定窗口大小
	setFixedSize(size());
	//窗口置顶
	setWindowFlags(Qt::WindowStaysOnTopHint);

	InitUI();
}

CCtrlParaDialog::~CCtrlParaDialog()
{
		m_pCurrentLineEdit = Q_NULLPTR;
}

void CCtrlParaDialog::InitUI()
{

	//TYPE
	int nType = m_CtrlData->GetCtrlType();

	switch (nType)
	{
		case ChartType:
		{
			ui.m_ChartRadio->setChecked(true);
			ui.m_BarRadio->setChecked(false);
			ui.m_PieRadio->setChecked(false);

			ui.m_ChartRadio->setDisabled(true);
			ui.m_BarRadio->setDisabled(true);
			ui.m_PieRadio->setDisabled(true);

			break;
		}	
		case BarType:
		{
			ui.m_ChartRadio->setChecked(false);
			ui.m_BarRadio->setChecked(true);
			ui.m_PieRadio->setChecked(false);

			ui.m_ChartRadio->setDisabled(true);
			ui.m_BarRadio->setDisabled(true);
			ui.m_PieRadio->setDisabled(true);
			break;
		}		
		case PieType:
		{
			ui.m_ChartRadio->setChecked(false);
			ui.m_BarRadio->setChecked(false);
			ui.m_PieRadio->setChecked(true);

			ui.m_ChartRadio->setDisabled(true);
			ui.m_BarRadio->setDisabled(true);
			ui.m_PieRadio->setDisabled(true);
			break;
		}		
		default:
			break;
	}

	//NAME
	ui.m_CtrlTagName->setText(m_CtrlData->GetCtrlTagName());
	ui.m_CtrlTagName->setDisabled(true);


	//注册过滤事件
	ui.m_XaisDes->installEventFilter(this);
	ui.m_XasiData->installEventFilter(this);
	ui.m_YAisData->installEventFilter(this);
	ui.m_YaisDes->installEventFilter(this);
	
	//信号槽
	QObject::connect(ui.m_FileRadio, SIGNAL(clicked()), this, SLOT(slot_OnFileSrcSel()));
	QObject::connect(ui.m_RptRadio, SIGNAL(clicked()), this, SLOT(slot_OnReportSel()));
	QObject::connect(ui.m_BtnOk, SIGNAL(clicked()), this, SLOT(slot_OnOk()));
	QObject::connect(ui.m_BtnCancel, SIGNAL(clicked()), this, SLOT(slot_OnCancel()));
}

/*! \fn CCtrlData* CCtrlParaDialog::GetCtrlData()
********************************************************************************************************* 
** \brief CCtrlParaDialog::GetCtrlData 
** \details 获取配置数据
** \return CCtrlData* 
** \author xingzhibing
** \date 2016年8月25日 
** \note 
********************************************************************************************************/
CCtrlData* CCtrlParaDialog::GetCtrlData()
{
	return m_CtrlData;
}

/*! \fn void CCtrlParaDialog::slot_OnOk()
********************************************************************************************************* 
** \brief CCtrlParaDialog::slot_OnOk 
** \details 确认
** \return void 
** \author xingzhibing
** \date 2016年8月25日 
** \note 
********************************************************************************************************/
void CCtrlParaDialog::slot_OnOk()
{
	if (ui.m_ChartName->text().isEmpty())
	{
		QMessageBox::information(this,tr("Name Missising"),("Please Fill ChartName!!!"));
		return;
	}
	if (ui.m_XaisDes->text().isEmpty())
	{
		QMessageBox::information(this, tr("Description Missising"), ("Please Fill X_Ais Description!!!"));
		return;
	}
	if (ui.m_XasiData->text().isEmpty())
	{
		QMessageBox::information(this, tr("Data Missising"), ("Please Fill X_Ais Data!!!"));
		return;
	}
	if (ui.m_YAisData->text().isEmpty())
	{
		QMessageBox::information(this, tr("Data Missising"), ("Please Fill Y_Ais Data!!!"));
		return;
	}
	if (ui.m_YaisDes->text().isEmpty())
	{
		QMessageBox::information(this, tr("Description Missising"), ("Please Fill Y_Ais Description!!!"));
		return;
	}
	
	QString szChartname = ui.m_ChartName->text();

	QString szXasiDes = ui.m_XaisDes->text();
	QString szYasiDES = ui.m_YaisDes->text();


	QString szXaisData = ui.m_XasiData->text();
	QString szYAisData = ui.m_YAisData->text();

	m_CtrlData->SetCtrlName(szChartname);
	m_CtrlData->SetXaisContent(szXasiDes);
	m_CtrlData->SetYaisContent(szYasiDES);
	m_CtrlData->SetRptCtrlData(szXaisData);


	QDialog::accept();
}

/*! \fn void CCtrlParaDialog::slot_OnCancel()
********************************************************************************************************* 
** \brief CCtrlParaDialog::slot_OnCancel 
** \details 取消
** \return void 
** \author xingzhibing
** \date 2016年8月25日 
** \note 
********************************************************************************************************/
void CCtrlParaDialog::slot_OnCancel()
{
	QDialog::reject();
}

/*! \fn void CCtrlParaDialog::slot_OnCtrlTypeSel()
********************************************************************************************************* 
** \brief CCtrlParaDialog::slot_OnCtrlTypeSel 
** \details 控件类型选取
** \return void 
** \author xingzhibing
** \date 2016年8月25日 
** \note 
********************************************************************************************************/
void CCtrlParaDialog::slot_OnCtrlTypeSel()
{
	
}

/*! \fn void CCtrlParaDialog::slot_OnDataSrcSel()
********************************************************************************************************* 
** \brief CCtrlParaDialog::slot_OnDataSrcSel 
** \details 数据源选取
** \return void 
** \author xingzhibing
** \date 2016年8月25日 
** \note 
********************************************************************************************************/
void CCtrlParaDialog::slot_OnFileSrcSel()
{
	ui.tab->setEnabled(true);
	ui.tab_2->setDisabled(true);
}


/*! \fn void CCtrlParaDialog::slot_OnReportSel()
*********************************************************************************************************
** \brief CCtrlParaDialog::slot_OnReportSel
** \details 从报表界面读取数据
** \return void
** \author xingzhibing
** \date 2016年8月25日
** \note
********************************************************************************************************/
void CCtrlParaDialog::slot_OnReportSel()
{
	ui.tab->setDisabled(true);
	ui.tab_2->setEnabled(true);
}

/*! \fn bool CCtrlParaDialog::eventFilter(QObject *, QEvent *)
********************************************************************************************************* 
** \brief CCtrlParaDialog::eventFilter 
** \details 事件过滤类型
** \param * 
** \param * 
** \return bool 
** \author xingzhibing
** \date 2016年8月25日 
** \note 
********************************************************************************************************/
bool CCtrlParaDialog::eventFilter(QObject *pObject, QEvent *e)
{
	if (pObject == ui.m_XaisDes)
	{
		if (e->type()==QEvent::FocusIn)
		{
			QPalette p = QPalette();
			p.setColor(QPalette::Base, Qt::gray);
			ui.m_XaisDes->setPalette(p);
		}
		else if (e->type() == QEvent::FocusOut)
		{
			QPalette p = QPalette();
			p.setColor(QPalette::Base, Qt::white);
			ui.m_XaisDes->setPalette(p);
			m_pCurrentLineEdit = ui.m_XaisDes;
		}		
	}
	if (pObject == ui.m_XasiData)
	{
		if (e->type() == QEvent::FocusIn)
		{
			QPalette p = QPalette();
			p.setColor(QPalette::Base, Qt::gray);
			ui.m_XaisDes->setPalette(p);
		}
		else if (e->type() == QEvent::FocusOut)
		{
			QPalette p = QPalette();
			p.setColor(QPalette::Base, Qt::white);
			ui.m_XaisDes->setPalette(p);
			m_pCurrentLineEdit = ui.m_XasiData;
		}	
	}
	if (pObject == ui.m_YAisData)
	{
		if (e->type() == QEvent::FocusIn)
		{
			QPalette p = QPalette();
			p.setColor(QPalette::Base, Qt::gray);
			ui.m_XaisDes->setPalette(p);
		}
		else if (e->type() == QEvent::FocusOut)
		{
			QPalette p = QPalette();
			p.setColor(QPalette::Base, Qt::white);
			ui.m_XaisDes->setPalette(p);
			m_pCurrentLineEdit = ui.m_YAisData;
		}		
	}
	if (pObject == ui.m_YaisDes)
	{
		if (e->type() == QEvent::FocusIn)
		{
			QPalette p = QPalette();
			p.setColor(QPalette::Base, Qt::gray);
			ui.m_XaisDes->setPalette(p);
		}
		else if (e->type() == QEvent::FocusOut)
		{
			QPalette p = QPalette();
			p.setColor(QPalette::Base, Qt::white);
			ui.m_XaisDes->setPalette(p);
			m_pCurrentLineEdit = ui.m_YaisDes;
		}		
	}
	return  QWidget::eventFilter(pObject, e);;
}


/*! \fn void CCtrlParaDialog::slot_OnDealData(const QString&)
********************************************************************************************************* 
** \brief CCtrlParaDialog::slot_OnDealData 
** \details 处理界面接收数据
** \param & 
** \return void 
** \author xingzhibing
** \date 2016年8月25日 
** \note 
********************************************************************************************************/
void CCtrlParaDialog::slot_OnDealData(const QString& str)
{
	if (str.isEmpty())
	{
		return;
	}
	if (m_pCurrentLineEdit)
	{
		m_pCurrentLineEdit->setText(str);
	}	
}
