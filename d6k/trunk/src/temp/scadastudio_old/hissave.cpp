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
CHisSave::CHisSave(QObject *parent, CScadastudio *pUi, QModelIndex &index, int mouseClickType)
	: QObject(parent), m_index(index)
{

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
