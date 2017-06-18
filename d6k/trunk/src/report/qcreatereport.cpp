#include <QMessageBox>
#include "createreport.h"


QCreateReport::QCreateReport(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	setFixedSize(this->width(), this->height());
}

QCreateReport::~QCreateReport()
{

}
void QCreateReport::slot_on_ok()
{
	QString tableName = ui.nameLineEdit->text();

	if (tableName.isEmpty())
	{
		QMessageBox::warning(this, tr("表名为空"), tr("请输入表名！！！"));
	//  QMessageBox::warning(this,tr("TableName is missing"),tr("please input tableName！！！"));
		return;
	}
	else
	{
		tableName = tableName + ".xml";
	}

	qint32 typeIndex = ui.comboBox->currentIndex();
	if (ui.rowLineEdit->text().isEmpty() || ui.columnLineEdit->text().isEmpty())
	{
		QMessageBox::warning(this, tr("行或列为空"), tr("请重新输入！！！"));
		//  QMessageBox::warning(this,tr("row or column is emprt"),tr("please re input！！！"));
		return;
	}

	qint16 rowCount = ui.rowLineEdit->text().toInt();

	qint16 colCount = ui.columnLineEdit->text().toInt();


	m_TableInfo.tableName = tableName;
	m_TableInfo.tableType = typeIndex;
	m_TableInfo.tableRow = rowCount;
	m_TableInfo.tableColumn = colCount;

	QDialog::accept();
}

void QCreateReport::slot_on_cancel()
{
	QDialog::reject();
}

CreateTableInfo& QCreateReport::GetTableInfo()
{
	return m_TableInfo;
}
