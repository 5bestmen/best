#include <QMessageBox>
#include "dgetselection.h"


DGetSelection::DGetSelection(QWidget *parent): QDialog(parent)
{
    setupUi(this);
	m_pVal = new CDataUint;
  
    setFixedSize(this->width(), this->height());  
    if (!mostValcheckBox->isChecked())
    {
        SetMostValue(false);
    }
    if (!overValcheckBox->isChecked())
    {
        SetOverValue(false);
    }
    TimeComboBox->setCurrentIndex(1);
}

DGetSelection::~DGetSelection()
{

}
void DGetSelection::slot_on_ok()
{
    m_pVal->m_szTimeIncrease=TimeComboBox->currentText();
    m_pVal->index=TimeComboBox->currentIndex();
    if (m_pVal->index==0)
    {
    //    QMessageBox::information(this,qApp->applicationName(),tr("请选择时间间隔!!!"));
      //  return;
    }
//值
    if (curValcheckBox->isChecked())
    {
        //desList.append("dp");
        m_pVal->m_val=1;
    }
//最值
    if (mostValcheckBox->isChecked())
    {
        if (maxValcheckBox->isChecked())//最大值
        {
            //desList.append("mx");
            m_pVal->m_mVal->maxVal=1;
            
        }
        if (maxValTimecheckBox->isChecked())//最大值发生时间
        {
            //desList.append("");
            m_pVal->m_mVal->maxValTime=1;
        }
        if (minValcheckBox->isChecked())//最小值
        {
            m_pVal->m_mVal->minVal=1;
        }
        if (minValTimecheckBox->isChecked())//最小值发生时间
        {
            m_pVal->m_mVal->minValTime=1;
        }
    }
//平均值
    if (avgValcheckBox->isChecked())
    {
        m_pVal->m_avgVal=1;
    }
//越限值
    if (overValcheckBox->isChecked())
    {
        if (overUpCountcheckBox->isChecked())//越上限次数
        {
            m_pVal->m_oVal->upLimitCount=1;
        }
        if(overUpTimecheckBox->isChecked())//越上限时间
        {
            m_pVal->m_oVal->upLimitTime=1;
        }
        if (overDownDownCountRatecheckBox->isChecked())//越下下限次数
        {
            m_pVal->m_oVal->ddLimitCount=1;
        }
        if (overDownCountcheckBox->isChecked())//越下限次数
        {
            m_pVal->m_oVal->downLimitCount=1;
        }
        if (overDownTimecheckBox->isChecked())//越下限时间
        {
            m_pVal->m_oVal->downLimitTIme=1;
        }
        if (overDownDownTimeRatecheckBox->isChecked())//越下下限时间
        {
            m_pVal->m_oVal->ddLimitTime=1;
        }
        if (OverUpUpCountcheckBox->isChecked())//越上上限次数
        {
            m_pVal->m_oVal->uuLimitCount=1;
        }
        if (overUpUpTimecheckBox->isChecked())//越上上限时间
        {
            m_pVal->m_oVal->uuLimitTime=1;
        }
    }
//累计值，暂时不用
    if (sumValcheckBox->isChecked())
    {
        m_pVal->m_sumVal=1;
    }
   

    QDialog::accept();
}
void DGetSelection::slot_on_cancel()
{
    QDialog::reject();
}

void DGetSelection::slot_on_mostValChange(bool index)
{
  //  if (maxValcheckBox->isChecked())
    {
        SetMostValue(index);
    }
  //  else
    {
  //      setMostValue(false);
    }
}
void DGetSelection::slot_on_overValChange(bool index)
{
    if (overValcheckBox->isChecked())
    {
        SetOverValue(true);
    }
    else
    {
        SetOverValue(false);
    }
}

void DGetSelection::SetMostValue(const bool& m_val)
{
    maxValTimecheckBox->setEnabled(m_val);
    maxValcheckBox->setEnabled(m_val);
    minValTimecheckBox->setEnabled(m_val);
    minValcheckBox->setEnabled(m_val);
}
void DGetSelection::SetOverValue(const bool& m_val)
{
    overDownDownCountRatecheckBox->setEnabled(m_val);
    overUpCountcheckBox->setEnabled(m_val);
    overUpTimecheckBox->setEnabled(m_val);
    overDownCountcheckBox->setEnabled(m_val);
    overDownDownTimeRatecheckBox->setEnabled(m_val);
    overDownTimecheckBox->setEnabled(m_val);
    OverUpUpCountcheckBox->setEnabled(m_val);
    overUpUpTimecheckBox->setEnabled(m_val);
}

void DGetSelection::InitMonth()
{
    TimeComboBox->clear();
	TimeComboBox->addItem(tr("1天"));
	TimeComboBox->addItem(tr("2天"));
	TimeComboBox->addItem(tr("3天"));
	//TimeComboBox->addItem(tr("1 day"));
	//TimeComboBox->addItem(tr("2 days"));
	//TimeComboBox->addItem(tr("3 days"));
}

void DGetSelection::InitYear()
{
    TimeComboBox->clear();
	TimeComboBox->addItem(tr("1月"));
	TimeComboBox->addItem(tr("2月"));
 /*   TimeComboBox->addItem(tr("1 month"));
    TimeComboBox->addItem(tr("2 months"));*/
}