/*! @file
<PRE>
********************************************************************************
ģ����       : 
�ļ���       :  exportmodeldialog.cpp
�ļ�ʵ�ֹ��� :  װ�õ���Ϊģ��
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
*  @date	2016.8.30
*/

#include "exportmodeldialog.h"

CExportModelDialog::CExportModelDialog(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	connect(ui.pOk, SIGNAL(clicked()), this, SLOT(OnOk()));
	connect(ui.pCancel, SIGNAL(clicked()), this, SLOT(OnCancel()));
}

CExportModelDialog::~CExportModelDialog()
{

}

/*! \fn void CExportModelDialog::OnOk()
********************************************************************************************************* 
** \brief CExportModelDialog::OnOk 
** \details 
** \return void 
** \author gw
** \date 2016��8��30�� 
** \note 
********************************************************************************************************/
void CExportModelDialog::OnOk()
{
	accept();

	m_strFactory = ui.pFactory->text();
	m_strModelDesc = ui.pModelDesc->text();
	m_strModelPath = ui.pModelPath->text();

	this->close();
}

/*! \fn void CExportModelDialog::OnCancel()
********************************************************************************************************* 
** \brief CExportModelDialog::OnCancel 
** \details 
** \return void 
** \author gw
** \date 2016��8��30�� 
** \note 
********************************************************************************************************/
void CExportModelDialog::OnCancel()
{
	reject();

	this->close();
}
