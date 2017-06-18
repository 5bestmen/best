#include "dataproperty.h"
#include "public_define.h"

QDataTypeAI::QDataTypeAI(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	InitDialog();

}
void QDataTypeAI::InitDialog()
{
	setMostFalse();
	setOverFalse();

	m_AIData.nAIType = -1;
	m_AIData.m_StartTime = QString::null;
	m_AIData.timeIncrease = QString::null;

	ui.dateEdit->setEnabled(false);
	ui.endDateEdit->setEnabled(false);

	//ui.TimeComboBox->setCurrentIndex(0);
	ui.TimeComboBox->setEnabled(false);
	ui.setPushButton->setEnabled(false);
}

QDataTypeAI::~QDataTypeAI()
{

}
void QDataTypeAI::slot_on_ok()
{
	QString time = ui.dateEdit->text();

	QString timeIncrease = ui.TimeComboBox->currentText();

	if (ui.curValradioButton->isChecked())
	{
		AIDataFlag = HISDATA_REAL;
	}
	if (ui.avgValradioButton->isChecked())
	{
		AIDataFlag = HISDATA_AVERAGE;
	}
	if (ui.sumValradioButton->isChecked())
	{
		AIDataFlag = HISDATA_DAYKWHTTL;
	}


	QDialog::accept();
}
void QDataTypeAI::slot_on_cancel()
{
	QDialog::reject();
}
void QDataTypeAI::slot_on_settime()
{
	ui.dateEdit->setEnabled(true);

	ui.endDateEdit->setEnabled(true);

	m_AIData.m_StartTime = ui.dateEdit->text();

	m_AIData.m_EndTime = ui.endDateEdit->text();

}
void QDataTypeAI::slot_on_setDataType()
{
	if (ui.mostValradioButton->isChecked())
	{
		setMostTrue();
	}
	else
	{
		setMostFalse();
	}

	if (ui.overValradioButton->isChecked())
	{
		setOverTrue();
	}
	else
	{
		setOverFalse();
	}
}
void QDataTypeAI::slot_on_setMostValue()
{
	if (ui.maxValradioButton->isChecked())
	{
		AIDataFlag = HISDATA_REALMAX;
	}
	if (ui.maxValTimeradioButton->isChecked())
	{
		AIDataFlag = HISDATA_MAXTIME;
	}
	if (ui.minValradioButton->isChecked())
	{
		AIDataFlag = HISDATA_REALMIN;
	}
	if (ui.minValTimeradioButton->isChecked())
	{
		AIDataFlag = HISDATA_MINTIME;
	}
}
void QDataTypeAI::slot_on_setOverVal()
{
	if (ui.overUpCountradioButton->isChecked())
	{
		AIDataFlag = HISDATA_OVHCOUNT;
	}
	if (ui.overUpTimeradioButton->isChecked())
	{
		AIDataFlag = HISDATA_OVHTIME;
	}
	if (ui.OverUpUpCountradioButton->isChecked())
	{
		AIDataFlag = HISDATA_OVHHCOUNT;
	}

	if (ui.overDownCountradioButton->isChecked())
	{
		AIDataFlag = HISDATA_DOWNLCOUNT;
	}
	if (ui.overDownTimeradioButton->isChecked())
	{
		AIDataFlag = HISDATA_DOWNLTIME;
	}
	if (ui.overUpUpTimeradioButton->isChecked())
	{
		AIDataFlag = HISDATA_OVHHTIME;
	}
	if (ui.overDownDownCountRateradioButton->isChecked())
	{
		AIDataFlag = HISDATA_DOWNLLCOUNT;
	}
	if (ui.overDownDownTimeRateradioButton->isChecked())
	{
		AIDataFlag = HISDATA_DOWNLLTIME;
	}
	if (ui.normalRateradioButton->isChecked())
	{
		AIDataFlag = HISDATA_REALHGL;
	}
}
void QDataTypeAI::slot_on_timeChanged(int)
{
	m_AIData.m_nIndex = ui.TimeComboBox->currentIndex();
	m_AIData.timeIncrease = ui.TimeComboBox->currentText();
}

void QDataTypeAI::setMostFalse()
{
	ui.maxValradioButton->setEnabled(false);
	ui.maxValTimeradioButton->setEnabled(false);
	ui.minValTimeradioButton->setEnabled(false);
	ui.minValradioButton->setEnabled(false);
}
void QDataTypeAI::setMostTrue()
{
	ui.maxValradioButton->setEnabled(true);
	ui.maxValTimeradioButton->setEnabled(true);
	ui.minValTimeradioButton->setEnabled(true);
	ui.minValradioButton->setEnabled(true);
}
void QDataTypeAI::setOverFalse()
{
	ui.overUpCountradioButton->setEnabled(false);
	ui.overDownDownCountRateradioButton->setEnabled(false);
	ui.overUpTimeradioButton->setEnabled(false);

	ui.overDownTimeradioButton->setEnabled(false);
	ui.overDownCountradioButton->setEnabled(false);
	ui.overDownDownTimeRateradioButton->setEnabled(false);

	ui.OverUpUpCountradioButton->setEnabled(false);
	ui.normalRateradioButton->setEnabled(false);
	ui.overUpUpTimeradioButton->setEnabled(false);
}
void QDataTypeAI::setOverTrue()
{
	ui.overUpCountradioButton->setEnabled(true);
	ui.overDownDownCountRateradioButton->setEnabled(true);
	ui.overUpTimeradioButton->setEnabled(true);

	ui.overDownTimeradioButton->setEnabled(true);
	ui.overDownCountradioButton->setEnabled(true);
	ui.overDownDownTimeRateradioButton->setEnabled(true);

	ui.OverUpUpCountradioButton->setEnabled(true);
	ui.normalRateradioButton->setEnabled(true);
	ui.overUpUpTimeradioButton->setEnabled(true);
}
MAIData QDataTypeAI::getDataType()
{
	m_AIData.nAIType = AIDataFlag;
	return  m_AIData;
}


void QDataTypeAI::setTimeFalse()
{
	ui.TimeComboBox->setEditable(false);
	ui.setPushButton->setEnabled(false);
}
void QDataTypeAI::setTimeTrue()
{
	ui.setPushButton->setEnabled(true);
	ui.TimeComboBox->setEditable(true);
}

void QDataTypeAI::InitMonth()
{
	ui.TimeComboBox->clear();
	ui.TimeComboBox->addItem(tr("1天"));
	ui.TimeComboBox->addItem(tr("2天"));
	ui.TimeComboBox->addItem(tr("3天"));
	//ui.TimeComboBox->addItem(tr("1 day"));
	//ui.TimeComboBox->addItem(tr("2 days"));
	//ui.TimeComboBox->addItem(tr("3 days"));
}
void QDataTypeAI::InitYear()
{
	ui.TimeComboBox->clear();
	ui.TimeComboBox->addItem(tr("1月"));
	ui.TimeComboBox->addItem(tr("2月"));
	//ui.TimeComboBox->addItem(tr("1 month"));
	//ui.TimeComboBox->addItem(tr("2 months"));
}