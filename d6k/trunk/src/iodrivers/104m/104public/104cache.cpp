#include "104cache.h"
#include <qglobal.h>
/*! \fn char* C104Cache::GetDataBuff()
********************************************************************************************************* 
** \brief C104Cache::GetDataBuff 
** \details 获取数据区数据
** \return char* 
** \author xingzhibing
** \date 2017年2月17日 
** \note 
********************************************************************************************************/
char* C104Cache::GetDataBuff()
{
	return m_DataBuf;
}
/*! \fn int C104Cache::GetDataLen()
********************************************************************************************************* 
** \brief C104Cache::GetDataLen 
** \details 获取数据区长度
** \return int 
** \author xingzhibing
** \date 2017年2月17日 
** \note 
********************************************************************************************************/
int C104Cache::GetDataLen()
{
	unsigned char lengthByte = m_DataBuf[1];
	return lengthByte + 2;
}

/*! \fn int C104Cache::GetCurByteLength()
********************************************************************************************************* 
** \brief C104Cache::GetCurByteLength 
** \details 获取数据区 实际字节长度
** \return int 
** \author xingzhibing
** \date 2017年2月17日 
** \note 
********************************************************************************************************/
int C104Cache::GetCurByteLength()
{
	return m_nCurPos;
}
/*! \fn int C104Cache::GetNeedByteLength()
********************************************************************************************************* 
** \brief C104Cache::GetNeedByteLength 
** \details 获取实际需要的字节长度
** \return int 
** \author xingzhibing
** \date 2017年2月17日 
** \note 
********************************************************************************************************/
int C104Cache::GetNeedByteLength()
{
	int nDataLen = GetDataLen();

	if (nDataLen <= 0)
		return 0;

	if (nDataLen > MAX_APDU_BUFFER_LEN)
		nDataLen = MAX_APDU_BUFFER_LEN;

	return nDataLen - m_nCurPos;
}

/*! \fn void C104Cache::CopyData(char* pBuff, int nLen)
********************************************************************************************************* 
** \brief C104Cache::CopyData 
** \details 拷贝数据到当前区域
** \param pBuff 
** \param nLen 
** \return void 
** \author xingzhibing
** \date 2017年2月17日 
** \note 
********************************************************************************************************/
void C104Cache::CopyData(char* pBuff, int nLen)
{
	Q_ASSERT(pBuff && nLen);

	char* pFrom = m_DataBuf + m_nCurPos;

	memcpy(pFrom, pBuff, nLen);

	m_nCurPos = m_nCurPos + nLen;
}
bool C104Cache::IsEmpty()
{
	if (m_nCurPos == 0)
		return true;
	return false;
}
void C104Cache::Clear()
{
	m_nCurPos = 0;
}

