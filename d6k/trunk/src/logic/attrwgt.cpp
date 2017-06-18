/*! @file
<PRE>
********************************************************************************
ģ����       :  ����  fast  mast  aux  �һ�����widget
�ļ���       :  attrwgt.cpp
�ļ�ʵ�ֹ��� :  ��ʾ����
����         :  ww
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         : <����˵��> �߼��༭
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

********************************************************************************
</PRE>
*  @brief
*  @author ww
*  @version 1.0
*  @date    2017.1.4
*/
#include "attrwgt.h"

/*! \fn CAttrWgt::CAttrWgt(QWidget *parent)
*********************************************************************************************************
** \brief CAttrWgt::CAttrWgt
** \details ���캯��
** \param parent
** \return
** \author ww
** \date 2017��1��4��
** \note
********************************************************************************************************/
CAttrWgt::CAttrWgt(QWidget *parent)
    : QDialog(parent)
{
    ui.setupUi(this);

    m_nRepertType = 0;
    //���
    m_nMsec = 0;
    //ע��
    m_strComment = "";

    //Ĭ��Ϊperiodģʽ
    ui.radioButton->setChecked(true);
    //ȷ��
    connect(ui.pushButton,SIGNAL(clicked()),this,SLOT(Slot_Conform()));
    //�ر�
    connect(ui.pushButton_2, SIGNAL(clicked()), this, SLOT(close()));
}

CAttrWgt::~CAttrWgt()
{

}

/*! \fn CAttrWgt::InitMast()
*********************************************************************************************************
** \brief CAttrWgt::InitMast
** \details ��ǰ����������  mast fast aux
** \param 
** \return
** \author ww
** \date 2017��1��4��
** \note
********************************************************************************************************/
void CAttrWgt::InitMast()
{
    //ȷ��mast ����,���Ҳ��ܱ༭
    ui.comboBox->setCurrentIndex(MAST_ATTR);
    ui.comboBox->setEnabled(false);
}

/*! \fn CAttrWgt::InitFast()
*********************************************************************************************************
** \brief CAttrWgt::InitFast
** \details ��ǰ����������  mast fast aux
** \param 
** \return
** \author ww
** \date 2017��1��4��
** \note
********************************************************************************************************/
void CAttrWgt::InitFast()
{
    //ȷ��fast ����,���Ҳ��ܱ༭
    ui.comboBox->setCurrentIndex(FAST_ATTR);
    ui.comboBox->setEnabled(false);
}

/*! \fn CAttrWgt::InitAux()
*********************************************************************************************************
** \brief CAttrWgt::InitAux
** \details ��ǰ����������  mast fast aux
** \param 
** \return
** \author ww
** \date 2017��1��4��
** \note
********************************************************************************************************/
void CAttrWgt::InitAux()
{
    //ȷ��aux ����,���Ҳ��ܱ༭
    ui.comboBox->setCurrentIndex(AUX_ATTR);
    ui.comboBox->setEnabled(false);
}

/*! \fn CAttrWgt::Slot_Conform()
*********************************************************************************************************
** \brief CAttrWgt::Slot_Conform
** \details ���ȷ�������Ĳۺ���
** \param
** \return
** \author ww
** \date 2017��1��4��
** \note
********************************************************************************************************/
void CAttrWgt::Slot_Conform()
{
    if (ui.radioButton->isChecked())
    {
        //ѭ������   1:period   2:cycle
        m_nRepertType = PERIOD_ITEM;
        m_nMsec = ui.spinBox->value();
    }
    else if(ui.radioButton_2->isChecked())
    {
        //ѭ������   1:period   2:cycle
        m_nRepertType = CYCLE_ITEM;
        m_nMsec = ui.spinBox_2->value();
    }
    else
    {
        //ѭ������   1:period   2:cycle
        m_nRepertType = 0;
        m_nMsec = 0;
    }

    m_strComment = ui.textEdit->toPlainText();
    this->close();

    //����ȷ���ź�,������������ģ�Ͳ���
    emit Signal_Conform();
}

int CAttrWgt::GetRepeatType() const
{
    return m_nRepertType;
}

int CAttrWgt::GetMsec() const
{
    return m_nMsec;
}

const QString& CAttrWgt::GetComment()
{
    return m_strComment;
}

/*! \fn CAttrWgt::SetCycleType(int nType)
*********************************************************************************************************
** \brief CAttrWgt::SetCycleType(int nType)
** \details ����ģ������,���½���ѭ������
** \param  ѭ������   1:period   2:cycle
** \return
** \author ww
** \date 2017��1��4��
** \note
********************************************************************************************************/
void CAttrWgt::SetCycleType(int nType)
{
    //ѭ������   1:period   2:cycle
    if (nType == PERIOD_ITEM)
    {
        ui.radioButton->setChecked(true);
    }
    else if (nType == CYCLE_ITEM)
    {
        ui.radioButton_2->setChecked(true);
    }
}

void CAttrWgt::SetComment(const QString &strComment)
{
    ui.textEdit->setText(strComment);
}

void CAttrWgt::SetMsec(int nMsec)
{
    ui.spinBox->setValue(nMsec);
    ui.spinBox_2->setValue(nMsec);
}

/*! \fn CAttrWgt::SetComboName(const QString &strName)
*********************************************************************************************************
** \brief CAttrWgt::SetComboName(const QString &strName)
** \details ����combobox��ǰֵ  ֻ����ʾ��
** \param  
** \return
** \author ww
** \date 2017��1��4��
** \note
********************************************************************************************************/
void CAttrWgt::SetComboName(const QString &strName)
{
    ui.comboBox->addItem(strName);
    ui.comboBox->setCurrentText(strName);
    ui.comboBox->setEnabled(false);

}
