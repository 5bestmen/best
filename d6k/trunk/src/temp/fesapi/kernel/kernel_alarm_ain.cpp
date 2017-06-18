

#include "data_def.h"
#include "kernel.h"

/*! \fn 
********************************************************************************************************* 
** \brief CKernel::CreateAinAlarmTable 
** \details 
** \param pParentElement �����ļ���AIN ALARM�����ڵ�
** \return bool true:�ɹ���false:ʧ��
** \date 2015��9��15�� 
** \note ����AIN ALARM�ڴ��
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

		if(!Utils::IsEqualIgnoreCase(strChildName,"aa"))//����a(������ע�͵�)
			continue;

		//��ȡAIN_ALARM������Ϣ
		TiXmlElement* pAINAlarmElement = pChild->ToElement();           

		//��ȡ����ֵ
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

		//��ʼ��AINAlarm����
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
** \return bool true:�ɹ���false:ʧ��
** \date 2015��9��15�� 
** \note ��Ain ALARM�ڴ��
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
** \date 2015��9��15�� 
** \note �ر�AIN ALARM�ڴ��
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
** \date 2015��9��15�� 
** \note ����AlarmOccNo��ȡAIN_ALARM����ָ��
********************************************************************************************************/
AIN_ALARM* CKernel::GetAinAlarm(int32u occNO)
{
	int nCount = m_poolAIN.GetArraySize();
	occNO = occNO -1;//occNo��1��ʼ
	if(occNO<0 || occNO>=nCount)
		return NULL;

	AIN_ALARM* pAINAlarm = m_poolAinAlarm.GetObject(occNO);

	return pAINAlarm;
}

