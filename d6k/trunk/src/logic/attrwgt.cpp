/*! @file
<PRE>
********************************************************************************
模块名       :  属性  fast  mast  aux  右击属性widget
文件名       :  attrwgt.cpp
文件实现功能 :  显示属性
作者         :  ww
版本         :  V1.00
--------------------------------------------------------------------------------
备注         : <其它说明> 逻辑编辑
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

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
** \details 构造函数
** \param parent
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
CAttrWgt::CAttrWgt(QWidget *parent)
    : QDialog(parent)
{
    ui.setupUi(this);

    m_nRepertType = 0;
    //间隔
    m_nMsec = 0;
    //注释
    m_strComment = "";

    //默认为period模式
    ui.radioButton->setChecked(true);
    //确认
    connect(ui.pushButton,SIGNAL(clicked()),this,SLOT(Slot_Conform()));
    //关闭
    connect(ui.pushButton_2, SIGNAL(clicked()), this, SLOT(close()));
}

CAttrWgt::~CAttrWgt()
{

}

/*! \fn CAttrWgt::InitMast()
*********************************************************************************************************
** \brief CAttrWgt::InitMast
** \details 当前的属性类型  mast fast aux
** \param 
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
void CAttrWgt::InitMast()
{
    //确定mast 属性,并且不能编辑
    ui.comboBox->setCurrentIndex(MAST_ATTR);
    ui.comboBox->setEnabled(false);
}

/*! \fn CAttrWgt::InitFast()
*********************************************************************************************************
** \brief CAttrWgt::InitFast
** \details 当前的属性类型  mast fast aux
** \param 
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
void CAttrWgt::InitFast()
{
    //确定fast 属性,并且不能编辑
    ui.comboBox->setCurrentIndex(FAST_ATTR);
    ui.comboBox->setEnabled(false);
}

/*! \fn CAttrWgt::InitAux()
*********************************************************************************************************
** \brief CAttrWgt::InitAux
** \details 当前的属性类型  mast fast aux
** \param 
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
void CAttrWgt::InitAux()
{
    //确定aux 属性,并且不能编辑
    ui.comboBox->setCurrentIndex(AUX_ATTR);
    ui.comboBox->setEnabled(false);
}

/*! \fn CAttrWgt::Slot_Conform()
*********************************************************************************************************
** \brief CAttrWgt::Slot_Conform
** \details 点击确定触发的槽函数
** \param
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
void CAttrWgt::Slot_Conform()
{
    if (ui.radioButton->isChecked())
    {
        //循环类型   1:period   2:cycle
        m_nRepertType = PERIOD_ITEM;
        m_nMsec = ui.spinBox->value();
    }
    else if(ui.radioButton_2->isChecked())
    {
        //循环类型   1:period   2:cycle
        m_nRepertType = CYCLE_ITEM;
        m_nMsec = ui.spinBox_2->value();
    }
    else
    {
        //循环类型   1:period   2:cycle
        m_nRepertType = 0;
        m_nMsec = 0;
    }

    m_strComment = ui.textEdit->toPlainText();
    this->close();

    //发送确认信号,触发更新数据模型操作
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
** \details 根据模型数据,更新界面循环类型
** \param  循环类型   1:period   2:cycle
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
void CAttrWgt::SetCycleType(int nType)
{
    //循环类型   1:period   2:cycle
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
** \details 设置combobox当前值  只做显示用
** \param  
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
void CAttrWgt::SetComboName(const QString &strName)
{
    ui.comboBox->addItem(strName);
    ui.comboBox->setCurrentText(strName);
    ui.comboBox->setEnabled(false);

}
