/*! @file
<PRE>
********************************************************************************
ģ����       :  ��ʷ����
�ļ���       :  graphmodel.cpp
�ļ�ʵ�ֹ��� :  ��ʷ����
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
*  @date    2015.11.26
*/

#include "hissave.h"
#include "scadastudio/imainwindow.h"
#include "scadastudio/icore.h"
#include "scadastudiodefine.h"

#include <QObject>
#include <QModelIndex>

/*! \fn CHisSave::CHisSave(QObject *parent, CScadastudio *pUi, QModelIndex &index, int mouseClickType)
********************************************************************************************************* 
** \brief CHisSave::CHisSave 
** \details 
** \param parent 
** \param pUi 
** \param index 
** \param mouseClickType 
** \return  
** \author gw
** \date 2015��11��26�� 
** \note 
********************************************************************************************************/
CHisSave::CHisSave(IMainModuleInterface *pCore)
	: QObject(pCore->GetUIMgr()), m_pCore(nullptr)
{
	Q_ASSERT(pCore);
	if (!pCore)
	{
		return;
	}

	m_pCore = pCore;

	m_pUi = pCore->GetUIMgr();
	Q_ASSERT(m_pUi);
	if (!m_pUi)
	{
		return;
	}
	
	connect((QObject *)m_pUi->GetLeftTree(), SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(DoubleClicked(const QModelIndex &)));
}

/*! \fn CHisSave::~CHisSave()
********************************************************************************************************* 
** \brief CHisSave::~CHisSave 
** \details 
** \return  
** \author gw
** \date 2015��11��26�� 
** \note 
********************************************************************************************************/
CHisSave::~CHisSave()
{

}

void CHisSave::DoubleClicked(const QModelIndex &index)
{
	int nType = index.data(Qt::UserRole).toInt();

	if (FES_TYPE_HIS_AI_REPORT_ITEM == nType)
	{
		//ģ�����������ݱ���
		//Ԥ���ӿ�
	}
	else if (FES_TYPE_HIS_AI_CURVE_ITEM == nType)
	{
		//ģ�����������ݱ���
		//Ԥ���ӿ�
	}
	else if (FES_TYPE_HIS_AI_MAX_MIN_ITEM == nType)
	{
		//ģ������ֵͳ��
		//Ԥ���ӿ�
	}
	else if (FES_TYPE_HIS_AI_LIMIT_ITEM == nType)
	{
		//ģ����Խ��ֵͳ��
		//Ԥ���ӿ�
	}
	else if (FES_TYPE_HIS_AI_CUM_ITEM == nType)
	{
		//ģ�����ۼ�ֵͳ��
		//Ԥ���ӿ�
	}
	else if (FES_TYPE_HIS_DI_CUM_ITEM == nType)
	{
		//�������ۼ�ֵͳ��
		//Ԥ���ӿ�
	}
	else if (FES_TYPE_HIS_KWH_CUM_ITEM == nType)
	{
		//������ۼ�ֵͳ��
		//Ԥ���ӿ�
	}
}
