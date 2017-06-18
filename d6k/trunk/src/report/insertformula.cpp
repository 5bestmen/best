#include "insertformula.h"

QInsertFormula::QInsertFormula(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	InitData();
	setFixedSize(this->width(), this->height());
}

QInsertFormula::~QInsertFormula()
{

}

void QInsertFormula::slot_on_ok()
{
	foreach(QLineEdit *temp, m_Edit)
	{
		if (temp->isEnabled() && !(temp->text().isEmpty()))
		{
			m_DataList.append(temp->text());
		}
	}
	m_szResult = m_DataList.join(",");

	QDialog::accept();
}

void QInsertFormula::slot_on_cancel()
{
	QDialog::reject();
}

void QInsertFormula::slot_on_dealData(const QString&mydata)
{
	m_CurrentEdit->setText(mydata);
	emit sig_getresult(GetRsult());
}

bool QInsertFormula::eventFilter(QObject *watched, QEvent *event)
{
	if (watched == ui.lineEdit_1)         //首先判断控件(这里指 lineEdit1)  
	{
		if (event->type() == QEvent::FocusIn)     //然后再判断控件的具体事件 (这里指获得焦点事件)  
		{
			QPalette p = QPalette();
			p.setColor(QPalette::Base, Qt::gray);
			ui.lineEdit_1->setPalette(p);
		}
		else if (event->type() == QEvent::FocusOut)    // 这里指 lineEdit1 控件的失去焦点事件  
		{
			QPalette p = QPalette();
			p.setColor(QPalette::Base, Qt::red);
			ui.lineEdit_1->setPalette(p);
			m_CurrentEdit = ui.lineEdit_1;
		}
	}
	if (watched == ui.lineEdit_2)           //这里来处理 lineEdit2 , 和处理lineEdit1 是一样的  
	{
		if (event->type() == QEvent::FocusIn)
		{
			QPalette p = QPalette();
			p.setColor(QPalette::Base, Qt::green);
			ui.lineEdit_2->setPalette(p);
		}
		else if (event->type() == QEvent::FocusOut)
		{
			QPalette p = QPalette();
			p.setColor(QPalette::Base, Qt::white);
			ui.lineEdit_2->setPalette(p);
			m_CurrentEdit = ui.lineEdit_2;
		}

	}
	if (watched == ui.lineEdit_3)
	{
		if (event->type() == QEvent::FocusOut)
		{
			m_CurrentEdit = ui.lineEdit_3;
		}
	}
	if (watched == ui.lineEdit_4)
	{
		if (event->type() == QEvent::FocusOut)
		{
			m_CurrentEdit = ui.lineEdit_4;
		}
	}
	if (watched == ui.lineEdit_5)
	{
		if (event->type() == QEvent::FocusOut)
		{
			m_CurrentEdit = ui.lineEdit_5;
		}
	}
	if (watched == ui.lineEdit_6)
	{
		if (event->type() == QEvent::FocusOut)
		{
			m_CurrentEdit = ui.lineEdit_6;
		}
	}
	if (watched == ui.lineEdit_7)
	{
		if (event->type() == QEvent::FocusOut)
		{
			m_CurrentEdit = ui.lineEdit_7;
		}

	}
	if (watched == ui.lineEdit_8)
	{
		if (event->type() == QEvent::FocusOut)
		{
			m_CurrentEdit = ui.lineEdit_8;
		}
	}

	return QWidget::eventFilter(watched, event);     // 最后将事件交给上层对话框  
}

void QInsertFormula::InitData()
{

	this->setWindowFlags(Qt::WindowStaysOnTopHint);
	m_Edit.append(ui.lineEdit_1);
	m_Edit.append(ui.lineEdit_2);
	m_Edit.append(ui.lineEdit_3);
	m_Edit.append(ui.lineEdit_4);
	m_Edit.append(ui.lineEdit_5);
	m_Edit.append(ui.lineEdit_6);
	m_Edit.append(ui.lineEdit_7);
	m_Edit.append(ui.lineEdit_8);

	ui.lineEdit_1->installEventFilter(this);
	ui.lineEdit_2->installEventFilter(this);
	ui.lineEdit_3->installEventFilter(this);
	ui.lineEdit_4->installEventFilter(this);
	ui.lineEdit_5->installEventFilter(this);
	ui.lineEdit_6->installEventFilter(this);
	ui.lineEdit_7->installEventFilter(this);
	ui.lineEdit_8->installEventFilter(this);
	installEventFilter(this);
}

void QInsertFormula::SetPlusTrue()
{
	foreach(QLineEdit *temp, m_Edit)
	{
		temp->setEnabled(true);
	}
}

void QInsertFormula::SetOtherTrue()
{
	foreach(QLineEdit *temp, m_Edit)
	{
		if (temp == ui.lineEdit_1 || temp == ui.lineEdit_2)
		{
			temp->setEnabled(true);
		}
		else
		{
			temp->setEnabled(false);
		}
	}
}

QString&  QInsertFormula::GetRsult()
{
	QStringList m_data;
	foreach(QLineEdit *temp, m_Edit)
	{
		if (temp->isEnabled() && !(temp->text().isEmpty()))
		{
			m_data.append(temp->text());
		}
	}
	m_szResult = m_data.join(",");
	return m_szResult;
}

