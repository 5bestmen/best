

#include "data_def.h"
#include "kernel.h"

/*! \fn
*********************************************************************************************************
** \brief CKernel::CreateDinAlarmTable
** \details
** \param pParentElement �����ļ���DIN ALARM�����ڵ�
** \return bool true:�ɹ���false:ʧ��
** \date 2015��9��15��
** \note ����DIN ALARM�ڴ��
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

		//��ȡDIN_ALARM������Ϣ
		TiXmlElement* pDINAlarmElement = pChild->ToElement();

		//��ȡ����ֵ
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

		//��ʼ��DIN_ALARM����
		pDINAlarm->OccNo = nOccNO;
	}

	return true;
}

/*! \fn
*********************************************************************************************************
** \brief CKernel::OpenDinAlarmTable
** \details
** \return bool true:�ɹ���false:ʧ��
** \date 2015��9��15��
** \note ��Din ALARM�ڴ��
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
** \date 2015��9��15��
** \note �ر�DIN ALARM�ڴ��
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
** \date 2015��9��15��
** \note ����AlarmOccNo��ȡDIN_ALARM����ָ��
********************************************************************************************************/
DIN_ALARM* CKernel::GetDinAlarm(int32u occNO)
{
	int nCount = m_poolAIN.GetArraySize();
	occNO = occNO - 1;//occNo��1��ʼ
	if (occNO < 0 || occNO >= nCount)
		return NULL;

	DIN_ALARM* pDINAlarm = m_poolDinAlarm.GetObject(occNO);

	return pDINAlarm;
}

