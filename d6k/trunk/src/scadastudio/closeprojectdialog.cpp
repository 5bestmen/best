/*! @file
<PRE>
********************************************************************************
ģ����       :  �����湤��
�ļ���       :  closeprojectdialog.cpp
�ļ�ʵ�ֹ��� :  �����湤��
����         :  gw
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         : <����˵��>
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

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
** \details �����湤��
** \return void 
** \author gw
** \date 2016��8��19�� 
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
** \details ȡ�����湤��
** \return void 
** \author gw
** \date 2016��8��19�� 
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
** \details ���湤��
** \return void 
** \author gw
** \date 2016��8��19�� 
** \note 
********************************************************************************************************/
void CCloseProjectDialog::Save()
{
	m_nType = SaveProject;

	this->accept();
}