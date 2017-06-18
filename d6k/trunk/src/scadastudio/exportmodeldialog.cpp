/*! @file
<PRE>
********************************************************************************
模块名       : 
文件名       :  exportmodeldialog.cpp
文件实现功能 :  装置导出为模板
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
** \date 2016年8月30日 
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
** \date 2016年8月30日 
** \note 
********************************************************************************************************/
void CExportModelDialog::OnCancel()
{
	reject();

	this->close();
}
