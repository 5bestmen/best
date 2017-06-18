/*! @file node.h
<PRE>
********************************************************************************
ģ����       :
�ļ���       :  node.h
�ļ�ʵ�ֹ��� :  ϵͳ�Ľڵ㶨��
����         :  LiJin
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         :   
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

********************************************************************************
</PRE>
*  @brief   ϵͳ�Ľڵ㶨��
*  @author  LiJin
*  @version 1.0
*  @date    2016.09.16
*******************************************************************************/
#ifndef FES_DEVICE_H
#define FES_DEVICE_H

#include "datatypes.h"
#include <vector>
#include <functional>
 
struct DEVICE;

class CFesDevice
{
public:
	explicit CFesDevice(DEVICE *pDevice);
	~CFesDevice();

public:
	size_t LoadFromMem(unsigned char *pMem);

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

	size_t GetAinOccNos(std::vector<int32u> & arrOccNos);
	size_t GetDinOccNos( std::vector<int32u> & arrOccNos);
	size_t GetAoutOccNos( std::vector<int32u> & arrOccNos);
	size_t GetDoutOccNos( std::vector<int32u> & arrOccNos);

	void SetHeartBeat();

	bool  SetQuality(int8u nQuality);

	std::function< bool(int32u nOccNo, int8u nQua) > m_fnSetAinQua;
	std::function< bool(int32u nOccNo, int8u nQua) > m_fnSetDinQua;
	std::function< bool(int32u nOccNo, int8u nQua) > m_fnSetAoutQua;
	std::function< bool(int32u nOccNo, int8u nQua) > m_fnSetDoutQua;
protected:
	CFesDevice();

private:
	int32u m_nOccNo;

	std::vector<int32u> m_arrAins;
	std::vector<int32u> m_arrDins;
	std::vector<int32u> m_arrAouts;
	std::vector<int32u> m_arrDouts;
	DEVICE *m_pFB;
};

#endif // NODE_H


/** @}*/

