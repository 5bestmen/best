/*! @file
<PRE>
********************************************************************************
模块名       :	按钮编辑控件
文件名       :  buttoneidt.cpp
文件实现功能 :  排序model
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
*  @date    2016.10.13
*/

#include "button_edit.h"
#include "channelparaconfigdialog.h"

#include <QHBoxLayout>
#include <QLineEdit>
#include <QToolButton>
#include <QPushButton>


/*! \fn CButtonEdit::CButtonEdit(QWidget *parent)
********************************************************************************************************* 
** \brief CButtonEdit::CButtonEdit 
** \details 
** \param parent 
** \return  
** \author gw
** \date 2016年10月13日 
** \note 
********************************************************************************************************/
CButtonEdit::CButtonEdit(QWidget *parent)
	: QWidget(parent)
{
	QHBoxLayout *layout = new QHBoxLayout(this);
	layout->setMargin(0);
	layout->setSpacing(0);
	auto pLineEdit = new QLineEdit(this);

	auto *button = new QPushButton(this);
	button->setText("test");
	button->setSizePolicy(QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred));
	button->setText(("..."));
	layout->addWidget(pLineEdit);
	layout->addWidget(button);


	connect(button, SIGNAL(clicked()),
		this, SLOT(buttonClicked()));
}

CButtonEdit::~CButtonEdit()
{

}

/*! \fn void CButtonEdit::buttonClicked()
********************************************************************************************************* 
** \brief CButtonEdit::buttonClicked 
** \details 按钮单击槽函数
** \return void 
** \author gw
** \date 2016年10月13日 
** \note 
********************************************************************************************************/
void CButtonEdit::buttonClicked()
{
	CChannelParaConfigDialog *pDialog = new CChannelParaConfigDialog(nullptr);
	pDialog->setWindowModality(Qt::ApplicationModal);
	pDialog->show();
}
