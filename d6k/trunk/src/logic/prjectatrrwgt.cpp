/*! @file
<PRE>
********************************************************************************
ģ����       :
�ļ���       :  prjectatrrwgt.cpp
�ļ�ʵ�ֹ��� :  ��������widget
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
#include "prjectatrrwgt.h"

/*! \fn CPrjectAtrrWgt::CPrjectAtrrWgt(QWidget *parent)
*********************************************************************************************************
** \brief CPrjectAtrrWgt::CPrjectAtrrWgt(QWidget *parent)
** \details ���캯��
** \param  ����
** \return
** \author ww
** \date 2017��1��4��
** \note
********************************************************************************************************/
CPrjectAtrrWgt::CPrjectAtrrWgt(QWidget *parent)
    : QDialog(parent)
{
    ui.setupUi(this);
    connect(ui.pushButton,SIGNAL(clicked()),this,SLOT(Slot_Comform()));
    connect(ui.pushButton_2,SIGNAL(clicked()),this,SLOT(close()));
}

CPrjectAtrrWgt::~CPrjectAtrrWgt()
{

}

/*! \fn CPrjectAtrrWgt::SetprojectName(const QString &projectName)
*********************************************************************************************************
** \brief CPrjectAtrrWgt::SetprojectName(const QString &projectName)
** \details ���ù�������
** \param  
** \return
** \author ww
** \date 2017��1��4��
** \note
********************************************************************************************************/
void CPrjectAtrrWgt::SetprojectName(const QString &projectName)
{
    ui.lineEdit->setText(projectName);
}

const QString & CPrjectAtrrWgt::GetProjectName()
{
    return m_strProjectName;
}

const QString & CPrjectAtrrWgt::GetPassWord()
{
    return m_strPassWd;
}

const QString & CPrjectAtrrWgt::GetComment()
{
    return m_strComment;
}

/*! \fn CPrjectAtrrWgt::Slot_Comform()
*********************************************************************************************************
** \brief CPrjectAtrrWgt::Slot_Comform()
** \details ���ȷ�������ۺ���
** \param
** \return
** \author ww
** \date 2017��1��4��
** \note
********************************************************************************************************/
void CPrjectAtrrWgt::Slot_Comform()
{
    m_strProjectName = ui.lineEdit->text();
    //TODO
    m_strPassWd = "xxx";

    m_strComment = ui.textEdit->toPlainText();
    emit Signal_Conform();

    this->close();
}

/*! \fn CPrjectAtrrWgt::GetCheckFlag()
*********************************************************************************************************
** \brief CPrjectAtrrWgt::GetCheckFlag()
** \details chekc��ǩ
** \param
** \return
** \author ww
** \date 2017��1��4��
** \note
********************************************************************************************************/
bool CPrjectAtrrWgt::GetCheckFlag()
{
    if (ui.checkBox->checkState() == Qt::Checked)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void CPrjectAtrrWgt::SetComment(const QString &strComment)
{
    ui.textEdit->setText(strComment);
}

void CPrjectAtrrWgt::SetProtectFlag(bool bFlag)
{
    if (bFlag)
    {
        ui.checkBox->setCheckState(Qt::Checked);
    }
    else
    {
        ui.checkBox->setCheckState(Qt::Unchecked);
    }
    
}

