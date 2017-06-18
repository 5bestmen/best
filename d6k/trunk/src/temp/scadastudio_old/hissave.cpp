/*! @file
<PRE>
********************************************************************************
模块名       :  历史保存
文件名       :  graphmodel.cpp
文件实现功能 :  历史保存
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
** \date 2015年11月26日 
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
** \date 2015年11月26日 
** \note 
********************************************************************************************************/
CHisSave::~CHisSave()
{

}
