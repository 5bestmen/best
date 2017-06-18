/*! @file channel.h
<PRE>
********************************************************************************
ģ����       :
�ļ���       :  channel.h
�ļ�ʵ�ֹ��� :  ϵͳ��ͨ�������װ
����         :  LiJin
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         :   
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

********************************************************************************
</PRE>
*  @brief   ϵͳ��ͨ�������װ
*  @author  LiJin
*  @version 1.0
*  @date    2016.09.18
*******************************************************************************/
#ifndef FES_CHANNEL_H
#define FES_CHANNEL_H

#include "datatypes.h"

#include <vector>
#include <functional>

struct CHANNEL;

class CFesChannel
{
public:
	explicit CFesChannel(CHANNEL *pChannel);
	~CFesChannel();
public:
	size_t LoadFromMem(unsigned char *pMem);
	size_t GetDeviceCount()
	{
		return m_arrDevices.size();
	}
	size_t GetAinCount()
	{
		return m_arrAins.size();
	}
	size_t GetDinCount()
	{
		return m_arrDins.size();
	}
	size_t GetAoutCount()
	{
		return m_arrAouts.size();
	}
	size_t GetDoutCount()
	{
		return m_arrDouts.size();
	}
	size_t GetDeviceOccNos(std::vector<int32u> & arrOccNos);

	size_t GetAinOccNos(std::vector<int32u> & arrOccNos);
	size_t GetDinOccNos(std::vector<int32u> & arrOccNos);
	size_t GetAoutOccNos(std::vector<int32u> & arrOccNos);
	size_t GetDoutOccNos(std::vector<int32u> & arrOccNos);
	
	void SetHeartBeat();

	bool SetQuality(int8u nQuality);

	std::function< bool(int32u nOccNo, int8u nQua) > m_fnSetDeviceQua;

private:
	CFesChannel();
	
	int32u m_nOccNo;

	std::vector<int32u> m_arrDevices;

	std::vector<int32u> m_arrAins;
	std::vector<int32u> m_arrDins;
	std::vector<int32u> m_arrAouts;
	std::vector<int32u> m_arrDouts;
	CHANNEL *m_pFB;
};

#endif // NODE_H


/** @}*/

