/*! @file dync_file_op_event.cpp
<PRE>
********************************************************************************
模块名       :
文件名       :  dync_file_op_event.cpp
文件实现功能 :  文件操作命令
作者         :  LiJin
版本         :  V1.00
--------------------------------------------------------------------------------
备注         :  <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief   基本的命令事件定义
*  @author  LiJin
*  @version 1.0
*  @date    2017.06.11
*******************************************************************************/

#include "dync_file_op_event.h"

#include "fesapi/fesdatadef.h"

#ifdef HMI_SDK_LIB
#include "log/log.h"
#include "scadaapi/scdsvcapi.h"
#else
#include "graph_module.h"
#endif

#include "../scdpub/variant_data.h"
 


std::array<QString, CDyncFileOpEventItem::MAX_FILE_OP_NUM> CDyncFileOpEventItem::m_arrFileOpTypeTxt =
{
	("打开一个新文件，并关闭当前"),
	("关闭当前"),
	("以模式对话框打开一个新文件"),
	("以MDI方式打开一个新文件"),
	("打开下一个文件"),
	("打开上一个文件")
};

CDyncFileOpEventItem::CDyncFileOpEventItem() :CBaseDyncEventItem()
{
	m_FileOpType = FILE_OPEN_NORMAL;
	m_ActionType = ACTION_CLICK;

	m_nXPos = 0;
	m_nYPos = 0;
}

CDyncFileOpEventItem::CDyncFileOpEventItem(const CDyncFileOpEventItem *pSrc) :CBaseDyncEventItem()
{
	Q_ASSERT(pSrc);
	Q_ASSERT(pSrc != this);
	if (pSrc && pSrc != this)
	{
		Q_ASSERT(pSrc->m_OpEventType == DYNC_FILE_OP);
		m_bEnable = pSrc->m_bEnable;
		m_ActionType = pSrc->m_ActionType;
		m_OpEventType = pSrc->m_OpEventType;
		m_FileOpType = pSrc->m_FileOpType;
		m_szGraphFileName = pSrc->m_szGraphFileName;
		m_nXPos = pSrc->m_nXPos;
		m_nYPos = pSrc->m_nYPos;
	}
}

CDyncFileOpEventItem::~CDyncFileOpEventItem()
{

}

CDyncFileOpEventItem& CDyncFileOpEventItem::operator=(const CDyncFileOpEventItem& src)
{
	if (this != &src)
	{
		Q_ASSERT(src.m_OpEventType == DYNC_FILE_OP);

		this->m_bEnable = src.m_bEnable;
		this->m_ActionType = src.m_ActionType;
		this->m_OpEventType = src.m_OpEventType;
		this->m_FileOpType = src.m_FileOpType;

		m_szGraphFileName = src.m_szGraphFileName;
		m_nXPos = src.m_nXPos;
		m_nYPos = src.m_nYPos;
	}
	return *this;
}
bool CDyncFileOpEventItem::operator==(const CDyncFileOpEventItem& src) const
{
	if (this->m_bEnable == src.m_bEnable &&
		this->m_ActionType == src.m_ActionType &&
		this->m_OpEventType == src.m_OpEventType &&
		this->m_FileOpType == src.m_FileOpType &&
		m_szGraphFileName == src.m_szGraphFileName &&
		m_nXPos == src.m_nXPos &&
		m_nYPos == src.m_nYPos)
	{
		return true;
	}
	return false;
}

std::string CDyncFileOpEventItem::GetEventDescript() const
{
	std::string szRet;
	if (m_FileOpType == FILE_CLOSE_FILE || m_FileOpType == FILE_OPEN_NEXT_FILE || m_FileOpType == FILE_OPEN_PREV_FILE)
	{
		// 单击 关闭
	//	szRet.Format(("%s %s"), m_arrActionTypeTxt[m_ActionType], m_arrFileOpTypeTxt[m_FileOpType]);
	}
	else
	{
		// 单击 打开文件 ...
	//	szRet.Format(("%s %s 画面名称:%s"), m_arrActionTypeTxt[m_ActionType], m_arrFileOpTypeTxt[m_FileOpType], m_szGraphFileName);
	}
	return szRet;
}
#if 0
void CDyncFileOpEventItem::SaveXml(JWXml::CXmlNodePtr  pDyncPtr) const
{
	ASSERT(!pDyncPtr->IsNull());
	if (pDyncPtr->IsNull())
		return;
	// 	JWXml::CXmlNodePtr pChild = pDyncPtr->NewChild(_T("EVENT_TYPE"));
	// 	ASSERT(pChild);
	// 	if (pChild)
	// 	{
	// 		pChild->SetValue(static_cast<DWORD>(m_OpEventType) );
	// 	}
	pDyncPtr->SetAttribute(_T("EVENT_TYPE"), static_cast<DWORD>(m_OpEventType));

	JWXml::CXmlNodePtr pChild = pDyncPtr->NewChild(_T("ACTION_TYPE"));
	ASSERT(!pChild->IsNull());
	if (!pChild->IsNull())
	{
		pChild->SetValue(static_cast<DWORD>(m_ActionType));
	}
	pChild = pDyncPtr->NewChild(_T("FILE_NAME"));
	ASSERT(!pChild->IsNull());
	if (!pChild->IsNull())
	{
		pChild->SetValue(m_szGraphFileName);
	}
	pChild = pDyncPtr->NewChild(_T("GRAPH_POS"));
	ASSERT(!pChild->IsNull());
	if (!pChild->IsNull())
	{
		JWXml::CXmlNodePtr pPos = pChild->NewChild(_T("X_POS"));
		pPos->SetValue(static_cast<DWORD>(m_nXPos));
		pPos = pChild->NewChild(_T("Y_POS"));
		pPos->SetValue(static_cast<DWORD>(m_nYPos));
	}
}

bool CDyncFileOpEventItem::LoadXml(JWXml::CXmlNodePtr  pDyncPtr)
{
	ASSERT(!pDyncPtr->IsNull());
	if (pDyncPtr->IsNull())
		return false;

	using namespace JWXml;

	DWORD dwRet = pDyncPtr->GetAttribute(_T("EVENT_TYPE"), 0);
	ASSERT(dwRet <= DYNC_USER_OP);

	CXmlNodes child_nodes = pDyncPtr->GetChildren();
	CString szName;

	for (long i = 0; i < child_nodes.GetCount(); i++)
	{
		szName = child_nodes[i]->GetName();
		if (szName == _T("ACTION_TYPE"))
		{
			dwRet = child_nodes[i]->GetValue(0);
			ASSERT(dwRet < MAX_ACTION_NUM);
			if (dwRet >= MAX_ACTION_NUM)
				dwRet = 0;
			m_ActionType = static_cast<ACTION_TYPE>(dwRet);
		}
		else if (szName == _T("FILE_NAME"))
		{
			m_szGraphFileName = child_nodes[i]->GetValue(_T(""));
		}
		else if (szName == _T("GRAPH_POS"))
		{
			CXmlNodePtr  pSub = child_nodes[i]->GetChild(_T("X_POS"), FALSE);
			ASSERT(!pSub->IsNull());
			if (!pSub->IsNull())
			{
				dwRet = pSub->GetValue(0);
				m_nXPos = dwRet;
			}
			pSub = child_nodes[i]->GetChild(_T("Y_POS"), FALSE);
			ASSERT(!pSub->IsNull());
			if (!pSub->IsNull())
			{
				dwRet = pSub->GetValue(0);
				m_nYPos = dwRet;
			}
		}
	}
	m_bEnable = true;
	return true;
}
#endif

const std::string & CDyncFileOpEventItem::GetGraphFileName() const
{
	return m_szGraphFileName;
}

void CDyncFileOpEventItem::SetFileOpType(const QString & szTxt)
{
	unsigned int i = 0;
	for (i = 0; i < MAX_FILE_OP_NUM; i++)
	{
		if (szTxt == m_arrFileOpTypeTxt[i])
		{
			m_FileOpType = static_cast<FILE_OP_TYPE>(i);
			return;
		}
	}
	Q_ASSERT(false);
}

QString  CDyncFileOpEventItem::GetFileOpTypeTxt() const
{
	if (m_FileOpType < MAX_FILE_OP_NUM)
	{
		return m_arrFileOpTypeTxt[m_FileOpType];
	}
	else
	{
		Q_ASSERT(false);
		return ("错误");
	}
}

void CDyncFileOpEventItem::SetGraphPos(const std::string & szX, const std::string & szY)
{
	int nVal = 0; // _ttoi(szX);
	if (nVal < 0)
	{
		nVal = 0;
	}
	m_nXPos = static_cast<unsigned int>(nVal);

	nVal = 0; // _ttoi(szY);
	if (nVal < 0)
	{
		nVal = 0;
	}
	m_nYPos = static_cast<unsigned int>(nVal);
}

void CDyncFileOpEventItem::SetGraphXPos(const std::string & szX)
{
	int nVal = 0;// _ttoi(szX);
	if (nVal < 0)
	{
		nVal = 0;
	}
	m_nXPos = static_cast<unsigned int>(nVal);
}
void CDyncFileOpEventItem::SetGraphYPos(const std::string & szY)
{
	int nVal = 0;// _ttoi(szY);
	if (nVal < 0)
	{
		nVal = 0;
	}
	m_nYPos = static_cast<unsigned int>(nVal);
}

void CDyncFileOpEventItem::SetFileName(const std::string & szName)
{
	//	ASSERT(!szName.IsEmpty());
	m_szGraphFileName = szName;
}

void CDyncFileOpEventItem::DoAction(CEventIntent &intent)
{
	switch (m_FileOpType)
	{
	case FILE_OPEN_NORMAL:
		if (!m_szGraphFileName.empty())
		{
// 			if (intent.m_fnOpenNewGraph)
// 			{
// 				intent.m_fnOpenNewGraph(m_szGraphFileName);
// 			}
		}
		break;
	case FILE_CLOSE_FILE:
		break;
	}
}

/** @}*/
