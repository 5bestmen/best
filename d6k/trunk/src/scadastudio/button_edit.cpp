/*! @file
<PRE>
********************************************************************************
ģ����       :	��ť�༭�ؼ�
�ļ���       :  buttoneidt.cpp
�ļ�ʵ�ֹ��� :  ����model
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
** \date 2016��10��13�� 
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
** \details ��ť�����ۺ���
** \return void 
** \author gw
** \date 2016��10��13�� 
** \note 
********************************************************************************************************/
void CButtonEdit::buttonClicked()
{
	CChannelParaConfigDialog *pDialog = new CChannelParaConfigDialog(nullptr);
	pDialog->setWindowModality(Qt::ApplicationModal);
	pDialog->show();
}
