/*! @file base_dync_event_item.h
<PRE>
********************************************************************************
ģ����       :
�ļ���       :  base_dync_event_item.h
�ļ�ʵ�ֹ��� :  �����������¼�����
����         :  LiJin
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         :  <����˵��>
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

********************************************************************************
</PRE>
*  @brief   �����������¼�����
*  @author  LiJin
*  @version 1.0
*  @date    2017.06.11
*******************************************************************************/
#ifndef BASE_DYNC_EVENT_ITEM_H
#define BASE_DYNC_EVENT_ITEM_H
 
#include <string>
#include <memory>
#include "dync_event.h"

class QXmlStreamWriter;
class QXmlStreamReader;

struct TAGITEM;

class CVariant;

class CBaseWidget;

/*! \class CBaseDyncEventItem base_dync_event.h  "base_dync_event.h"
*  \brief ��̬�����¼�
*  \details
*/
class CBaseDyncEventItem
{
public:
	CBaseDyncEventItem();
	virtual ~CBaseDyncEventItem();
	CBaseDyncEventItem(const CBaseDyncEventItem&);
	CBaseDyncEventItem& operator=(const CBaseDyncEventItem& src);
	bool operator==(const CBaseDyncEventItem& src) const;
public:
	enum EVENT_TYPE
	{
		DYNC_FILE_OP = 0, //!< �ļ�����
		DYNC_VAR_OP,      //!< ��������
		DYNC_SYS_OP,      //!< ϵͳ�����
		DYNC_SCRIPT_OP,   //!< �ű������
		DYNC_USER_OP,     //!< �û�����
	};
	// ��������
	enum ACTION_TYPE
	{
		ACTION_CLICK = 0, //!< ����
		ACTION_PRESSED,   //!< ����
		ACTION_RELEASE,   //!< �ͷ�
		ACTION_FOCUSED,   //!< ����
		ACTION_UNFOCUSED,
		MAX_ACTION_NUM
	};
public:
// 	virtual void SaveXml(JWXml::CXmlNodePtr  pDyncPtr)const;
// 	virtual bool LoadXml(JWXml::CXmlNodePtr  pDyncPtr);

	virtual void DoAction(CEventIntent &intent);
	virtual std::string GetEventDescript()const;

	EVENT_TYPE GetEventType();
	// ���ö�������
	void SetActionType(const std::string & szTxt);
	std::string GetActionTypeTxt();
public:
	bool m_bEnable;
	//! ����
 	static std::array<std::string, MAX_ACTION_NUM> m_arrActionTypeTxt;
protected:
	//! ����
	EVENT_TYPE  m_OpEventType;
	//! ��궯������
	ACTION_TYPE   m_ActionType;
};


#endif // BASE_DYNC_EVENT_ITEM_H


/** @}*/
