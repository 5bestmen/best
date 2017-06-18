/*! @file
<PRE>
********************************************************************************
模块名       :  不保存工程
文件名       :  closeprojectdialog.cpp
文件实现功能 :  不保存工程
作者         :  gw
版本         :  V1.00
--------------------------------------------------------------------------------
备注         : <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief
*  @author gw
*  @version 1.0
*  @date	2016.8.19
*/

#include "closeprojectdialog.h"

CCloseProjectDialog::CCloseProjectDialog(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	Qt::WindowFlags flags = Qt::Dialog;
	flags |= Qt::WindowCloseButtonHint;
	setWindowFlags(flags);

	connect(ui.pushButtonNotSave, SIGNAL(clicked()), this, SLOT(Notsave()));
	connect(ui.pushButtonSave, SIGNAL(clicked()), this, SLOT(Save()));
	connect(ui.pushButtonCancel, SIGNAL(clicked()), this, SLOT(Cancel()));
}

CCloseProjectDialog::~CCloseProjectDialog()
{

}

/*! \fn void CCloseProjectDialog::NotSave()
********************************************************************************************************* 
** \brief CCloseProjectDialog::NotSave 
** \details 不保存工程
** \return void 
** \author gw
** \date 2016年8月19日 
** \note 
********************************************************************************************************/
void CCloseProjectDialog::Notsave()
{
	m_nType = NotsaveProject;

	this->accept();
}

/*! \fn void CCloseProjectDialog::Cancel()
********************************************************************************************************* 
** \brief CCloseProjectDialog::Cancel 
** \details 取消保存工程
** \return void 
** \author gw
** \date 2016年8月19日 
** \note 
********************************************************************************************************/
void CCloseProjectDialog::Cancel()
{
	m_nType = CancelProject;

	this->accept();
}

/*! \fn void CCloseProjectDialog::Save()
********************************************************************************************************* 
** \brief CCloseProjectDialog::Save 
** \details 保存工程
** \return void 
** \author gw
** \date 2016年8月19日 
** \note 
********************************************************************************************************/
void CCloseProjectDialog::Save()
{
	m_nType = SaveProject;

	this->accept();
}