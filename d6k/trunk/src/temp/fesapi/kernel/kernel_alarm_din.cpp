

#include "data_def.h"
#include "kernel.h"

/*! \fn
*********************************************************************************************************
** \brief CKernel::CreateDinAlarmTable
** \details
** \param pParentElement 配置文件中DIN ALARM区根节点
** \return bool true:成功，false:失败
** \date 2015年9月15日
** \note 创建DIN ALARM内存库
********************************************************************************************************/
bool CKernel::CreateDinAlarmTable(TiXmlElement* pParentElement)
{
	int nCount = GetElementChildCount(pParentElement, "da");
	if (nCount <= 0)
	{
		return false;
	}

	if (!m_poolDinAlarm.CreateArray(SMKEY_MEM_ALARM_DIN, nCount))
	{
		return false;
	}

	TiXmlNode *pChild;

	for (pChild = pParentElement->FirstChild(); pChild != 0; pChild = pChild->NextSibling())
	{
		std::string strChildName = pChild->Value();

		if (!Utils::IsEqualIgnoreCase(strChildName, "da"))
			continue;

		//获取DIN_ALARM配置信息
		TiXmlElement* pDINAlarmElement = pChild->ToElement();

		//获取属性值
		int nOccNO = 0;
		int nChannelNo = 0;
		int nDevNO = 0;
		int nBlockNO = 0;

		pDINAlarmElement->QueryIntAttribute("OccNo", &nOccNO);
		
		DIN_ALARM* pDINAlarm = GetDinAlarm(nOccNO);

		if (pDINAlarm == NULL)
		{
			return false;
		}

		//初始化DIN_ALARM对象
		pDINAlarm->OccNo = nOccNO;
	}

	return true;
}

/*! \fn
*********************************************************************************************************
** \brief CKernel::OpenDinAlarmTable
** \details
** \return bool true:成功，false:失败
** \date 2015年9月15日
** \note 打开Din ALARM内存库
********************************************************************************************************/
bool CKernel::OpenDinAlarmTable()
{
	if (!m_poolDinAlarm.OpenArray(SMKEY_MEM_ALARM_DIN))
		return false;

	return true;
}

/*! \fn
*********************************************************************************************************
** \brief CKernel::DestroyDinAlarmTable
** \details
** \return void
** \date 2015年9月15日
** \note 关闭DIN ALARM内存库
********************************************************************************************************/
void CKernel::DestroyDinAlarmTable()
{
	m_poolDinAlarm.CloseArray();
}

/*! \fn
*********************************************************************************************************
** \brief CKernel::GetDinAlarm
** \details
** \param occNO
** \return DIN_ALARM*
** \date 2015年9月15日
** \note 根据AlarmOccNo获取DIN_ALARM数据指针
********************************************************************************************************/
DIN_ALARM* CKernel::GetDinAlarm(int32u occNO)
{
	int nCount = m_poolAIN.GetArraySize();
	occNO = occNO - 1;//occNo从1开始
	if (occNO < 0 || occNO >= nCount)
		return NULL;

	DIN_ALARM* pDINAlarm = m_poolDinAlarm.GetObject(occNO);

	return pDINAlarm;
}

