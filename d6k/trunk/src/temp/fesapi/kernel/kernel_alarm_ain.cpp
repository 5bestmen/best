

#include "data_def.h"
#include "kernel.h"

/*! \fn 
********************************************************************************************************* 
** \brief CKernel::CreateAinAlarmTable 
** \details 
** \param pParentElement 配置文件中AIN ALARM区根节点
** \return bool true:成功，false:失败
** \date 2015年9月15日 
** \note 创建AIN ALARM内存库
********************************************************************************************************/
bool CKernel::CreateAinAlarmTable(TiXmlElement* pParentElement)
{
	int nCount = GetElementChildCount(pParentElement,"aa");
	if(nCount <=0)
	{
		return false;
	}

	if (!m_poolAinAlarm.CreateArray(SMKEY_MEM_ALARM_AIN, nCount))
	{
		return false;
	}

	TiXmlNode *pChild;
	
	for (pChild = pParentElement->FirstChild(); pChild != 0; pChild = pChild->NextSibling()) 
	{
		std::string strChildName = pChild->Value();

		if(!Utils::IsEqualIgnoreCase(strChildName,"aa"))//不是a(比如是注释等)
			continue;

		//获取AIN_ALARM配置信息
		TiXmlElement* pAINAlarmElement = pChild->ToElement();           

		//获取属性值
		int nOccNO = 0;
		fp64 fHH = 0;
		fp64 fH = 0;
		fp64 fL = 0;
		fp64 fLL = 0;

		fp64 fGStart = 0;
		fp64 fGLimit = 0;

		int nDeadBandType = AIN_ALARM::DEADBAND_RELATIVE;
		fp64 fDeadBand = 0;

		pAINAlarmElement->QueryIntAttribute("OccNo", &nOccNO);
		pAINAlarmElement->QueryDoubleAttribute("HH", &fHH);
		pAINAlarmElement->QueryDoubleAttribute("H", &fH);
		pAINAlarmElement->QueryDoubleAttribute("LL", &fLL);
		pAINAlarmElement->QueryDoubleAttribute("L", &fL);
		pAINAlarmElement->QueryDoubleAttribute("GStart", &fGStart);
		pAINAlarmElement->QueryDoubleAttribute("GLimit", &fGLimit);
		pAINAlarmElement->QueryIntAttribute("DeadBandType", &nDeadBandType);
		pAINAlarmElement->QueryDoubleAttribute("DeadBand", &fDeadBand);

		AIN_ALARM* pAINAlarm = GetAinAlarm(nOccNO);
		if (pAINAlarm == NULL)
		{
			return false;
		}

		//初始化AINAlarm对象
		pAINAlarm->OccNo = nOccNO;
		pAINAlarm->HH = fHH;
		pAINAlarm->H = fH;
		pAINAlarm->L = fL;
		pAINAlarm->LL = fLL;
		pAINAlarm->GStart = fGStart;
		pAINAlarm->GLimit = fGLimit;
		pAINAlarm->DeadBandType = nDeadBandType;
		pAINAlarm->DeadBand = fDeadBand;
	}

	return true;
}

/*! \fn 
********************************************************************************************************* 
** \brief CKernel::OpenAinAlarmTable 
** \details 
** \return bool true:成功，false:失败
** \date 2015年9月15日 
** \note 打开Ain ALARM内存库
********************************************************************************************************/
bool CKernel::OpenAinAlarmTable()
{
	if (!m_poolAinAlarm.OpenArray(SMKEY_MEM_ALARM_AIN))
		return false;

	return true;
}

/*! \fn 
********************************************************************************************************* 
** \brief CKernel::DestroyAinAlarmTable 
** \details 
** \return void 
** \date 2015年9月15日 
** \note 关闭AIN ALARM内存库
********************************************************************************************************/
void CKernel::DestroyAinAlarmTable()
{
	m_poolAinAlarm.CloseArray();
}

/*! \fn 
********************************************************************************************************* 
** \brief CKernel::GetAinAlarm 
** \details 
** \param occNO 
** \return AIN_ALARM* 
** \date 2015年9月15日 
** \note 根据AlarmOccNo获取AIN_ALARM数据指针
********************************************************************************************************/
AIN_ALARM* CKernel::GetAinAlarm(int32u occNO)
{
	int nCount = m_poolAIN.GetArraySize();
	occNO = occNO -1;//occNo从1开始
	if(occNO<0 || occNO>=nCount)
		return NULL;

	AIN_ALARM* pAINAlarm = m_poolAinAlarm.GetObject(occNO);

	return pAINAlarm;
}

