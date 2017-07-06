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
** \date 2015年11月26日 
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
** \date 2015年11月26日 
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
		//模拟量报表数据保存
		//预留接口
	}
	else if (FES_TYPE_HIS_AI_CURVE_ITEM == nType)
	{
		//模拟量曲线数据保存
		//预留接口
	}
	else if (FES_TYPE_HIS_AI_MAX_MIN_ITEM == nType)
	{
		//模拟量最值统计
		//预留接口
	}
	else if (FES_TYPE_HIS_AI_LIMIT_ITEM == nType)
	{
		//模拟量越限值统计
		//预留接口
	}
	else if (FES_TYPE_HIS_AI_CUM_ITEM == nType)
	{
		//模拟量累计值统计
		//预留接口
	}
	else if (FES_TYPE_HIS_DI_CUM_ITEM == nType)
	{
		//开关量累计值统计
		//预留接口
	}
	else if (FES_TYPE_HIS_KWH_CUM_ITEM == nType)
	{
		//点度量累计值统计
		//预留接口
	}
}
