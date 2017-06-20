/*! @file dync_event.cpp
<PRE>
********************************************************************************
模块名       :
文件名       :  dync_event.cpp
文件实现功能 :  命令事件定义
作者         :  LiJin
版本         :  V1.00
--------------------------------------------------------------------------------
备注         : <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief   基本的动态定义
*  @author  LiJin
*  @version 1.0
*  @date    2017.06.11
*******************************************************************************/


#include "base_widget.h"
#include "dync_event.h"
#include "base_dync_event_item.h"
#include "dync_file_op_event.h"
#include "dync_var_op_event.h"
#include "dync_sys_op_event.h"

#include "stl_util-inl.h"

#include "fesapi/fesdatadef.h"

#ifdef HMI_SDK_LIB
#include "log/log.h"
#include "scadaapi/scdsvcapi.h"
#else
#include "graph_module.h"
#endif

#include "../scdpub/variant_data.h"
 

CDyncEventData::CDyncEventData()
{
	m_bEnable = false;
	m_nDyncType = DYNC_EVENT;
}

CDyncEventData::~CDyncEventData()
{
	STLDeleteElements(&m_arrEvents);
}

const CDyncEventData& CDyncEventData::operator=(const CDyncEventData& src)
{
	if (this != &src)
	{
		m_bEnable = src.m_bEnable;
		m_nDyncType = src.m_nDyncType;
		m_szTagName = src.m_szTagName;

		STLDeleteElements(&m_arrEvents);

		auto it_op = src.m_arrEvents.begin();
		for (; it_op != src.m_arrEvents.end(); ++it_op)
		{
			CBaseDyncEventItem* pEvent =  new CBaseDyncEventItem(**it_op);
			m_arrEvents.push_back(pEvent);
		}
	}
	return *this;
}

bool CDyncEventData::operator==(const CDyncEventData& src)const
{
	if (this->m_bEnable != src.m_bEnable ||
		this->m_nDyncType != src.m_nDyncType ||
		this->m_szTagName != src.m_szTagName)
	{
		return false;
	}

	if (this->m_arrEvents.size() != src.m_arrEvents.size())
	{
		return false;
	}

	auto it_this = this->m_arrEvents.begin();
	auto it_src = src.m_arrEvents.begin();

	for (; it_this != this->m_arrEvents.end() && it_src != src.m_arrEvents.end(); ++it_src, ++it_this)
	{
		if ((*it_src)->operator==(**it_this) == false)
		{
			return false;
		}
	}
	return true;
}

void CDyncEventData::SaveXml(std::shared_ptr<QXmlStreamWriter>pXmlWriter) const
{
	Q_ASSERT(pXmlWriter);
	if (pXmlWriter == nullptr)
		return;

}

bool CDyncEventData::LoadXml(std::shared_ptr<QXmlStreamReader>pXmlReader)
{
	return true;
}

#if 0
/*! \fn void CDyncEventData::SaveXml(JWXml::CXmlNodePtr  pDyncPtr) const
*********************************************************************************************************
** \brief 保存
** \details 动态事件数据
** \param pDyncPtr
** \return void
** \author LiJin
** \date 2010年10月2日
** \note
********************************************************************************************************/
void CDyncEventData::SaveXml(JWXml::CXmlNodePtr  pDyncPtr)const
{
	ASSERT(!pDyncPtr->IsNull());
	if (pDyncPtr->IsNull())
		return;
	ASSERT(m_bEnable);
	if (m_bEnable == FALSE)
		return;
	// 	ASSERT(!m_szTagName.IsEmpty());
	// 	if (m_szTagName.IsEmpty())
	// 		return;
	using namespace JWXml;

	CXmlNodePtr pChildNode;
	// 名称及类型
	pDyncPtr->SetAttribute(_T("TYPE"), static_cast <int> (m_nDyncType));
	pDyncPtr->SetAttribute(_T("TAG"), m_szTagName);

	pChildNode = pDyncPtr->NewChild(_T("DYNC_DATA"));
	ASSERT(!pChildNode->IsNull());
	if (pChildNode->IsNull())
		return;

	auto iter = m_arrFileEvents.cbegin();

	CXmlNodePtr pSubNode;
	for (; iter != m_arrFileEvents.cend(); ++iter)
	{
		pSubNode = pChildNode->NewChild(_T("FILE_OP"));
		ASSERT(!pSubNode->IsNull());
		ASSERT((*iter)->m_bEnable);
		if (!pSubNode->IsNull() && (*iter)->m_bEnable)
		{
			(*iter)->SaveXml(pSubNode);
		}
	}
}

bool CDyncEventData::LoadXml(JWXml::CXmlNodePtr  pDyncPtr)
{
	m_arrFileEvents.clear();

	using namespace JWXml;

	m_bEnable = FALSE;
	int nVal = pDyncPtr->GetAttribute(_T("TYPE"), 0);
	ASSERT(nVal == m_nDyncType);
	//	if (nVal != m_nDyncType)
	//		return false;

	// 	m_szTagName = pDyncPtr->GetAttribute(_T("TAG"));
	// 	if (m_szTagName.IsEmpty())
	// 		return false;

	CXmlNodePtr pDyncData = pDyncPtr->GetChild(_T("DYNC_DATA"), FALSE);

	ASSERT(!pDyncData->IsNull());
	if (pDyncData->IsNull())
		return false;

	CXmlNodes child_nodes = pDyncData->GetChildren();
	CString szName;
	bool bRet = false;
	CDyncFileOpEventItem fileop_evt;
	for (long i = 0; i < child_nodes.GetCount(); i++)
	{
		szName = child_nodes[i]->GetName();
		if (szName == _T("FILE_OP"))
		{
			bRet = fileop_evt.LoadXml(child_nodes[i]);
			ASSERT(bRet);
			if (bRet)
			{
				std::shared_ptr <CDyncFileOpEventItem> ptr_fileop =
					std::make_shared<CDyncFileOpEventItem>(fileop_evt);
				m_arrFileEvents.push_back(ptr_fileop);
			}
		}
	}

	CheckDyncData();
	return true;
}
#endif

/*! \fn  bool CBaseDyncData::IsDyncTypeEqual(CBaseDyncData *pData) const
*********************************************************************************************************
** \brief   CBaseDyncData::IsDyncTypeEqual
** \details 比较两个动态连接是否是一样的类型
** \param   pData
** \return bool
** \author LiJin
** \date  2010年12月20日
** \note
********************************************************************************************************/
bool CDyncEventData::IsDyncTypeEqual(CBaseDyncData *pData) const
{
	Q_ASSERT(pData);
	if (pData == nullptr)
		return false;

	if (pData->m_nDyncType == this->m_nDyncType)
	{
		return true;
	}
	return false;
}

bool CDyncEventData::CheckDyncData()
{
	// 	if(m_szTagName.IsEmpty())
	// 		return false;
	// 	return true;

	if (m_arrEvents.empty())
	{
		m_bEnable = false;
	}
	else
	{
		m_bEnable = true;
	}
	return true;
}

CBaseDyncData *CDyncEventData::CloneDyncData()
{
	CDyncEventData *pNewDync = new CDyncEventData;
	*pNewDync = *this;
	return dynamic_cast<CDyncEventData*> (pNewDync);
}

void  CDyncEventData::PrepareWidgetDync(CBaseWidget *pWidget)
{
	Q_ASSERT(pWidget);
	if (pWidget == nullptr)
		return;
}

bool CDyncEventData::ProcessWidgetDync(CBaseWidget *pWidget)
{
	Q_ASSERT(pWidget);
	if (pWidget == nullptr)
		return false;

	return false;
}

bool  CDyncEventData::ProcessAction(CBaseWidget *pWidget, CEventIntent &intent)
{
	Q_ASSERT(pWidget);
	if (pWidget == nullptr)
		return false;

	auto it_file = m_arrEvents.begin();
	for (; it_file != m_arrEvents.end(); ++it_file)
	{
		(*it_file)->DoAction(intent);
	}

	return false;
}

// std::vector <std::shared_ptr <CDyncFileOpEventItem>> & CDyncEventData::GetFileOpEvents()
// {
// 	return m_arrEvents;
// }

// CDyncFileOpEventItem* CDyncEventData::CreateFileOpEvent()
// {
// 	std::shared_ptr <CDyncFileOpEventItem> pEvent = std::make_shared<CDyncFileOpEventItem>();
// 	m_arrFileEvents.push_back(pEvent);
// 	pEvent->m_bEnable = true;
// 	return pEvent.get();
// }

#if 0

/*! \fn void CDyncEventData::DeleteEvent(CBaseDyncEventItem *pEvent)
*********************************************************************************************************
** \brief CDyncEventData::DeleteEvent
** \details 删除一个事件
** \param pEvent
** \return void
** \author LiJin
** \date 2012年1月19日
** \note
********************************************************************************************************/
void CDyncEventData::DeleteEvent(CBaseDyncEventItem *pEvent)
{
	Q_ASSERT(pEvent);
	if (pEvent == nullptr)
		return;
	auto iter = m_arrFileEvents.begin();
	for (; iter != m_arrFileEvents.end(); ++iter)
	{
		if ((*iter).get() == pEvent)
		{
			m_arrFileEvents.erase(iter);
			return;
		}
	}
	Q_ASSERT(false);
}

void CDyncEventData::TrimData()
{
	auto iter = m_arrFileEvents.begin();
	for (; iter != m_arrFileEvents.end(); )
	{
		if ((*iter)->m_bEnable == false)
		{
			m_arrFileEvents.erase(iter);
		}
		else
		{
			++iter;
		}
	}
}
#endif

CBaseDyncEventItem * CDyncEventData::CreateEventItem(int nEventType)
{
	CBaseDyncEventItem *newItem = nullptr;
	
	switch (nEventType)
	{
	case CBaseDyncEventItem::DYNC_FILE_OP:
	{
		//文件操作
		newItem = new CDyncFileOpEventItem;
		break;
	}
	case CBaseDyncEventItem::DYNC_VAR_OP:
	{
		//变量操作
		newItem = new CDyncFileOpEventItem;
		break;
	}
	case CBaseDyncEventItem::DYNC_SYS_OP:
	{
		//系统类操作
		newItem = new CDyncSysOpEventItem;
		break;
	}
	case CBaseDyncEventItem::DYNC_SCRIPT_OP:
	{
		//脚本类操作
		break;
	}
	case CBaseDyncEventItem::DYNC_USER_OP:
	{
		//用户操作
		break;
	}
	
	default:
		break;
	}
	
	return newItem;
}




 

/** @}*/
