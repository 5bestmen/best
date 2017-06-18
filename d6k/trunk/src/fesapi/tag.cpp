


#include "tag.h"

#include "fesapi/fesdatadef.h"
#include "fesapi/fescnst.h"
#include "log/log.h"  

#include "alarm.h"
#include <QDateTime>

void CFesTag::SetTimePack(TIMEPAK &dstTm, TIMEPAK *pTm)
{
	if (pTm)
	{
		memcpy(&dstTm, pTm, sizeof(TIMEPAK));
	}
	else
	{
		QDateTime dtNow = QDateTime::currentDateTime();
		dstTm.Year = dtNow.date().year();
		dstTm.Month = dtNow.date().month();
		dstTm.Day = dtNow.date().day();
		dstTm.DayOfWeek = dtNow.date().dayOfWeek();

		dstTm.Hour = dtNow.time().hour();
		dstTm.Minute = dtNow.time().minute();
		dstTm.Second = dtNow.time().second();
		dstTm.Milliseconds = dtNow.time().msec();
	}
}


CFesAin::CFesAin()
{
	m_nChannelMailBoxID = 0;
	m_pFB = nullptr;
	m_pAlarm = nullptr;
	m_pChannel = nullptr;
	m_pDevcie = nullptr;
}

CFesAin::CFesAin(AIN *pAin)
{
	m_nChannelMailBoxID = 0;
	m_pFB = nullptr;
	m_pFB = pAin;
	m_pAlarm = nullptr;
	m_pChannel = nullptr;
	m_pDevcie = nullptr;
}

CFesAin::~CFesAin()
{

}
/*! \fn bool CFesAin::CalcOutput(AIN *pFB,fp64 fpRaw, fp64 & fpOutput)
********************************************************************************************************* 
** \brief CFesAin::CalcOutput 
** \details  �����������
** \param pFB 
** \param fpRaw 
** \param fpOutput 
** \return bool 
** \author LiJin 
** \date 2016��10��12�� 
** \note 
********************************************************************************************************/
bool CFesAin::CalcOutput(AIN *pFB,fp64 fpRaw, fp64 & fpOutput)
{
	Q_ASSERT(pFB);
	if (pFB == nullptr)
		return false;

	Q_ASSERT(pFB->MaxRaw != pFB->MinRaw);
	if (pFB->MaxRaw == pFB->MinRaw)
	{
		return false;
	}

	fpOutput = (pFB->MaxScale - pFB->MinScale)*(fpRaw - pFB->MinRaw) / (pFB->MaxRaw - pFB->MinRaw) + pFB->MinScale;

	return true;
}
/*! \fn bool CFesAin::CalcNonLinearOutput(TRANSFORM_NONLINEAR *pNonLinear, fp64 fpRaw, fp64 & fpOutput)
********************************************************************************************************* 
** \brief CFesAin::CalcNonLinearOutput 
** \details ���������ת��
** \param pNonLinear 
** \param fpRaw 
** \param fpOutput 
** \return bool 
** \author LiJin 
** \date 2016��10��13�� 
** \note 
********************************************************************************************************/
bool CFesAin::CalcNonLinearOutput(TRANSFORM_NONLINEAR *pNonLinear, fp64 fpRaw, fp64 & fpOutput)
{
	Q_ASSERT(pNonLinear);
	if (pNonLinear == nullptr)
		return false;

	Q_ASSERT(pNonLinear->DataCount >= 2 && pNonLinear->DataCount <= MAX_NON_LINEAR_NUM);
	if (pNonLinear->DataCount < 2 || pNonLinear->DataCount > MAX_NON_LINEAR_NUM)
		return false;
	
	fp64 fpRet = 0.0f;

	int nCount = pNonLinear->DataCount;

	int nIndex = 0;

	//ע������ԭʼ���� ��������ʽ�������ڴ�ṹ��

	//�ڷ�Χ֮�� ������Ϊ���ݷǷ�
	if (pNonLinear->NonLinearData[0].RawValue > fpRaw || pNonLinear->NonLinearData[nCount].RawValue <fpRaw)
	{
		Q_ASSERT(false);
		return false;
	}

	//�������������飬���ϵ�һ������ >= fpRaw ���ҵ�����������ֵ
	for (int i = 0; i < nCount;++i)
	{
		if (pNonLinear->NonLinearData[i].RawValue == fpRaw)
		{
			return pNonLinear->NonLinearData[i].ScaleValue;
		}
		else if (pNonLinear->NonLinearData[i].RawValue > fpRaw)
		{
			nIndex = i;
			break;
		}
	}
	Q_ASSERT( nIndex > 0 && nIndex < nCount );

	//ǰһ����ֵ
	float preRawVal = pNonLinear->NonLinearData[nIndex - 1].RawValue;
	float preScaleVal = pNonLinear->NonLinearData[nIndex - 1].ScaleValue;
	//��һ����ֵ
	float nextRawVal = pNonLinear->NonLinearData[nIndex].RawValue;
	float nextScaleVal = pNonLinear->NonLinearData[nIndex].ScaleValue;

	Q_ASSERT( nextRawVal == preRawVal );

	//ֵ���ַǷ����� 
	if (nextRawVal == preRawVal)
	{
		return false;
	}

	fpRet = (nextScaleVal - preScaleVal)*(fpRaw - preRawVal) / (nextRawVal - preRawVal) + preScaleVal;
	
	fpOutput = fpRet;

	return true;
}
/*! \fn bool CFesAin::CalcLinearOutput(TRANSFORM_LINEAR *pLinear, fp64 fpRaw, fp64 & fpOutput)
********************************************************************************************************* 
** \brief CFesAin::CalcLinearOutput 
** \details �����������
** \param pLinear 
** \param fpRaw 
** \param fpOutput 
** \return bool 
** \author LiJin 
** \date 2016��10��12�� 
** \note 
********************************************************************************************************/
bool CFesAin::CalcLinearOutput(TRANSFORM_LINEAR *pLinear, fp64 fpRaw, fp64 & fpOutput)
{
	Q_ASSERT(pLinear);
	if (pLinear == nullptr)
		return false;
	Q_ASSERT(pLinear->MaxRaw != pLinear->MinRaw);
	if (pLinear->MaxRaw == pLinear->MinRaw)
		return false;

	fp64 fpRet = 0.0f;

	if (pLinear->EnableInverse == 0)
	{// û�в��� ��������ת��
		fpRet = (pLinear->MaxScale - pLinear->MinScale)*(fpRaw - pLinear->MinRaw) / (pLinear->MaxRaw-pLinear->MinRaw) + pLinear->MinScale;
	}
	else
	{// ����:������ת�����÷���ת��
		fpRet = (pLinear->MaxScale - pLinear->MinScale)*(fpRaw - pLinear->MinRaw) / (pLinear->MinRaw - pLinear->MaxRaw) + pLinear->MaxScale;
	}
	
	if (pLinear->EnableFactor != 0)
	{// ���ü���ϵ��		
		if (pLinear->InverseFactor)
		{// ���÷���ϵ��
			Q_ASSERT(pLinear->Gain != 0.0f);
			if (pLinear->Gain != 0.0f)
			{
				fpOutput = (fpRet - pLinear->Offset) / (pLinear->Gain);
				return true;
			}
			else
			{
				return false;
			}
		}
		else
		{
			fpOutput = fpRet * pLinear->Gain + pLinear->Offset;
			return true;
		}
	}
	else
	{
		fpOutput = fpRet;
		return true;
	}
}

bool  CFesAin::Update(fp64 fpValue, int8u nQua, TIMEPAK * pTm)
{
	Q_ASSERT(m_pFB);
	if (m_pFB == nullptr)
		return false;

	if (m_pFB->IsDefined != FES_YES)
		return false;

	fp64 fpTemp = fpValue;
	bool bRet = false;
	/******** if not initilized, then initilizing, no event is triggered ***********/
	if (m_pFB->Init != INITED)
	{
		if (m_pFB->ScanEnable == SCAN_OUT)
		{
			// �屨��״̬

			m_pFB->Init = INITED;
			return false;
		}
		else
		{
			// ��ȡ ת��ϵ�� ����
			if (m_pFB->TransformOccNo != INVALID_OCCNO && m_pFB->TransformOccNo <= MAX_OCCNO)
			{
				if (m_pFB->TransformType == TRANSFORM_TYPE_LINEAR)
				{
					Q_ASSERT(m_fnGetTransformLinear);
					if (m_pLinear == nullptr && m_fnGetTransformLinear != nullptr)
					{
						m_pLinear = m_fnGetTransformLinear(m_pFB->TransformOccNo);
					}
					if (m_pLinear)
					{
						bRet = CalcLinearOutput(m_pLinear, fpValue, fpTemp);
						Q_ASSERT(bRet);
					}
				}
				else if (m_pFB->TransformType == TRANSFORM_TYPE_NONLINEAR)
				{
					Q_ASSERT(m_fnGetTransformNonLinear);
					if (m_pNonLinear == nullptr && m_fnGetTransformNonLinear != nullptr)
					{
						m_pNonLinear = m_fnGetTransformNonLinear(m_pFB->TransformOccNo);
						bRet = CalcNonLinearOutput(m_pNonLinear, fpValue, fpTemp);
						Q_ASSERT(bRet);
					}
				}
				else
				{
					Q_ASSERT(false);
				}
			}
			else
			{// �����ڽṹ��
				bRet = CalcOutput(m_pFB, fpValue, fpTemp);
				Q_ASSERT(bRet);
			}

			fp64 OutputL = m_pFB->RangeH + (m_pFB->RangeH - m_pFB->RangeL) * m_pFB->LowQua;
			fp64 OutputH = m_pFB->RangeL + (m_pFB->RangeH - m_pFB->RangeL) * m_pFB->HighQua;

			// ����Ʒ��
			if (fpTemp > OutputH || fpTemp < OutputL)
			{// ������Χ
				m_pFB->Quality = QUALITY_BAD;
			}
			else
			{
				m_pFB->Quality = nQua;
			}

			m_pFB->Value = fpTemp;
			m_pFB->Init = INITED;
			this->SetTimePack(m_pFB->LastUpdateTime, pTm);
			return true;
		}
	}
	/* if not in Scan, then return, we assume Output would be set manually, so we do give NotOutput a value */
	if (m_pFB->ScanEnable == SCAN_OUT)
	{//����˳�
		return false;
	}
	if (m_pDevcie && m_pDevcie->ScanEnable == SCAN_OUT)
	{
		return false;
	}
	if (m_pChannel && m_pChannel->ScanEnable == SCAN_OUT)
	{
		return false;
	}

	// ��ȡ ת��ϵ�� ����
	if (m_pFB->TransformOccNo != INVALID_OCCNO && m_pFB->TransformOccNo <= MAX_OCCNO)
	{
		if (m_pFB->TransformType == TRANSFORM_TYPE_LINEAR)
		{
			Q_ASSERT(m_fnGetTransformLinear);
			if (m_pLinear == nullptr && m_fnGetTransformLinear != nullptr)
			{
				m_pLinear = m_fnGetTransformLinear(m_pFB->TransformOccNo);
			}
			if (m_pLinear)
			{
				bRet = CalcLinearOutput(m_pLinear, fpValue, fpTemp);
				Q_ASSERT(bRet);
			}
		}
		else if (m_pFB->TransformType == TRANSFORM_TYPE_NONLINEAR)
		{
			Q_ASSERT(m_fnGetTransformNonLinear);
			if (m_pNonLinear == nullptr && m_fnGetTransformNonLinear != nullptr)
			{
				m_pNonLinear = m_fnGetTransformNonLinear(m_pFB->TransformOccNo);
				bRet = CalcNonLinearOutput(m_pNonLinear, fpValue, fpTemp);
				Q_ASSERT(bRet);
			}
		}
		else
		{
			Q_ASSERT(false);
		}
	}
	else
	{// �����ڽṹ��
		bRet = CalcOutput(m_pFB, fpValue, fpTemp);
		Q_ASSERT(bRet);
	}

	fp64 OutputL = m_pFB->RangeH + (m_pFB->RangeH - m_pFB->RangeL) * m_pFB->LowQua;
	fp64 OutputH = m_pFB->RangeL + (m_pFB->RangeH - m_pFB->RangeL) * m_pFB->HighQua;

	// ����Ʒ��
	if (fpTemp > OutputH || fpTemp < OutputL)
	{// ������Χ
		m_pFB->Quality = QUALITY_BAD;
	}
	else
	{
		m_pFB->Quality = nQua;
	}
	this->SetTimePack(m_pFB->LastUpdateTime, pTm);
	m_pFB->Value = fpTemp;

	if (m_pFB->Quality == QUALITY_BAD)
	{
		// ��澯״̬
		ClearAlarmState();
		return true;
	}

	// ��ȡ�澯�������
	if (m_pFB->AlarmOccNo != INVALID_OCCNO && m_pFB->AlarmOccNo <= MAX_OCCNO)
	{
		if (m_pAlarm == nullptr)
		{
			m_pAlarm = m_fnGetAlarm(m_pFB->AlarmOccNo);
			Q_ASSERT(m_pAlarm);
		}
	}

	if (m_pAlarm)
	{
		m_pAlarm->Update(m_pFB,m_pFB->Value, m_pFB->Quality,pTm);
	}
	return true;
}
/*! \fn bool CFesAin::Update(fp64 fpValue, int8u nQua)
********************************************************************************************************* 
** \brief CFesAin::Update 
** \details IO�������²�ֵ
** \param fpValue 
** \param nQua 
** \return bool 
** \author LiJin 
** \date 2016��10��12�� 
** \note 
********************************************************************************************************/
bool CFesAin::Update(fp64 fpValue, int8u nQua)
{
	return this->Update(fpValue, nQua, nullptr);
}

void  CFesAin::ClearAlarmState()
{
	Q_ASSERT(m_pFB);
	if (m_pFB == nullptr)
		return  ;

	std::memset(m_pFB->AlarmState, 0, sizeof(int32u)*MAX_AIN_ALARMLIMIT_BITCOUNT);
}

void  CFesAin::GetValue(fp64 &fpValue, int8u &nQua)const
{
	Q_ASSERT(m_pFB);
	if (m_pFB == nullptr)
		return ;

	fpValue = m_pFB->Value;
	nQua = m_pFB->Quality;
}
/*! \fn void  CFesAin::LoadConfig()
********************************************************************************************************* 
** \brief CFesAin::LoadConfig 
** \details �������ã��ڳ�ʼ��ʱ����
** \return void 
** \author LiJin 
** \date 2016��10��18�� 
** \note 
********************************************************************************************************/
void  CFesAin::LoadConfig()
{
	Q_ASSERT(m_pFB);
	if (m_pFB == nullptr)
		return;

	// �澯��أ�ģ��������û�����ø澯�Ŀ���
	if (m_pFB->AlarmOccNo != INVALID_OCCNO && m_pFB->AlarmOccNo <= MAX_OCCNO)
	{
		if (m_pAlarm == nullptr)
		{// û�л�ȡ��������������ر�������
			Q_ASSERT(m_fnGetAlarm);
			if (m_fnGetAlarm)
			{
				m_pAlarm = m_fnGetAlarm(m_pFB->AlarmOccNo);
				Q_ASSERT(m_pAlarm);
				if (m_pAlarm)
				{
					m_pAlarm->LoadConfig(m_pFB);
				}
			}			
		}
	}
	// ��ȡ�����ID
	Q_ASSERT(m_pFB->ChannelOccNo != INVALID_OCCNO && m_pFB->ChannelOccNo <= MAX_OCCNO);
	if (m_pFB->ChannelOccNo != INVALID_OCCNO && m_pFB->ChannelOccNo <= MAX_OCCNO)
	{
		Q_ASSERT(m_fnGetChannel);
		if (m_fnGetChannel)
		{
		//	CHANNEL chan;
			bool bRet = m_fnGetChannel(m_pFB->ChannelOccNo, &m_pChannel);
			Q_ASSERT(bRet && m_pChannel);
			if (bRet)
			{
				m_nChannelMailBoxID = m_pChannel->MailBoxID;
			}
		}
	}
}
/*! \fn void  CFesAin::SetQuality(int8u nQua)
********************************************************************************************************* 
** \brief CFesAin::SetQuality 
** \details IOͨѶ���޸Ĳ��Ʒ��
** \param nQua 
** \return void 
** \author LiJin 
** \date 2016��10��27�� 
** \note 
********************************************************************************************************/
void  CFesAin::SetQuality(int8u nQua)
{
	Q_ASSERT(m_pFB);
	if (m_pFB == nullptr)
		return;

	m_pFB->Quality = nQua;
}

CFesDin::CFesDin()
{
	m_nChannelMailBoxID = 0;
	m_pFB = nullptr;

	m_pChannel = nullptr;
	m_pDevcie = nullptr;
}

CFesDin::CFesDin(DIN *pDin)
{
	m_pFB = pDin;
	m_nChannelMailBoxID = 0;

	m_pChannel = nullptr;
	m_pDevcie = nullptr;
}

CFesDin::~CFesDin()
{

}

void  CFesDin::LoadConfig()
{
	Q_ASSERT(m_pFB);
	if (m_pFB == nullptr)
		return;

	// �澯���
	if (m_pFB->AlarmOccNo != INVALID_OCCNO && m_pFB->AlarmOccNo < MAX_OCCNO)
	{
		if (m_pAlarm == nullptr)
		{// û�л�ȡ��������������ر�������
			m_pAlarm = m_fnGetAlarm(m_pFB->AlarmOccNo);
			Q_ASSERT(m_pAlarm);
			m_pAlarm->LoadConfig(m_pFB);
		}
	}
}
/*! \fn bool CFesDin::Update(int8u nValue, int8u nQua, bool bCpuEnable)
********************************************************************************************************* 
** \brief CFesDin::Update 
** \details �������²�ֵ
** \param nValue 
** \param nQua 
** \param bCpuEnable true: �ɺ�̨������λ��false���澯��Ϣ��װ�õ�������
** \return bool 
** \author LiJin 
** \date 2016��10��26�� 
** \note 
********************************************************************************************************/
bool CFesDin::Update(int8u nValue, int8u nQua, bool bCpuEnable)
{
	return this->Update(nValue, nQua, bCpuEnable, nullptr);
}

bool CFesDin::Update(int8u nValue, int8u nQua, bool bCpuEnable, TIMEPAK * pTm)
{
	Q_ASSERT(m_pFB);
	if (m_pFB == nullptr)
		return false;

	if (m_pFB->IsDefined != FES_YES)
		return false;

	// ��������˴�ң�ŵĺţ�˵���õ���˫λң�ţ����Ҹ�ң������ң��
	if (m_pFB->SlaveOccNo != INVALID_OCCNO && m_pFB->SlaveOccNo != MAX_OCCNO)
	{// ����˫λң��


		return false;
	}

	return NormalUpdate(nValue, nQua, bCpuEnable,pTm);
}
/*! \fn bool CFesDin::Update(int8u nValue, int8u nQua)
********************************************************************************************************* 
** \brief CFesDin::Update 
** \details �������²�ֵ
** \param nValue 
** \param nQua 
** \return bool 
** \author LiJin 
** \date 2016��10��20�� 
** \note  ע��˫λң�ŵĴ���
********************************************************************************************************/
bool CFesDin::Update(int8u nValue, int8u nQua)
{
	return Update(nValue, nQua, false);
}
/*! \fn bool CFesDin::NormalUpdate(int8u nValue, int8u nQua)
********************************************************************************************************* 
** \brief CFesDin::NormalUpdate 
** \details ��ͨң�ţ�˫λң�ŵĴ�ң�ţ���ֵ����
** \param nValue 
** \param nQua 
** \return bool 
** \author LiJin 
** \date 2016��10��26�� 
** \note 
********************************************************************************************************/
bool CFesDin::NormalUpdate(int8u nValue, int8u nQua, bool bCpuEnable, TIMEPAK * pTm)
{
	Q_ASSERT(m_pFB);
	if (m_pFB == nullptr)
		return false;

	if (m_pFB->IsDefined != FES_YES)
		return false;

	if (nValue != 0)
	{
		nValue = 1;
	}

	if (m_pFB->Init != INITED)
	{
		if (m_pFB->ScanEnable == SCAN_OUT )
		{//�˳�״̬
			m_pFB->Init = INITED;
			m_pFB->NegValue = (int8u)!(m_pFB->Value);
			return true;
		}
		else
		{//
			m_pFB->RawValue = nValue;
			m_pFB->Value = ((m_pFB->SignalType == NORMAL_OPEN)? m_pFB->RawValue : m_pFB->NegValue);
		}
		m_pFB->NegValue = (int8u)(!m_pFB->Value);
		m_pFB->Quality = nQua;
		m_pFB->Init = INITED;
		this->SetTimePack(m_pFB->LastUpdateTime, nullptr);

		return true;
	}

/* if not in Scan, then return, we assume Output would be set manually, so we do give NotOutput a value */
	if (m_pFB->ScanEnable == SCAN_OUT)
	{//����˳�
		m_pFB->NegValue = (int8u)!m_pFB->Value;
		return false;
	}
	if (m_pDevcie && m_pDevcie->ScanEnable == SCAN_OUT)
	{
		m_pFB->NegValue = (int8u)!m_pFB->Value;
		return false;
	}
	if (m_pChannel && m_pChannel->ScanEnable == SCAN_OUT)
	{
		m_pFB->NegValue = (int8u)!m_pFB->Value;
		return false;
	}
	m_pFB->RawValue = nValue;
	m_pFB->Quality = nQua;

	/* if point is in bad quality, then only it's rawdata is recorded **/
	if (m_pFB->Quality == QUALITY_BAD)
		return false;
	// �����ڵ㣬����µ�ֵ���ֵ��һ��
	// ���սڵ㣬����µ�ֵ���ֵһ��������Ҫ�澯��������Ҫ�澯
	bool bFlag = false;
	if ((m_pFB->RawValue != m_pFB->Value && m_pFB->SignalType == NORMAL_OPEN)
		|| (m_pFB->RawValue == m_pFB->Value && m_pFB->SignalType == NORMAL_CLOSE))
		bFlag = true;
	else
		bFlag = false;
	
	m_pFB->Value = (m_pFB->SignalType == NORMAL_OPEN) ? m_pFB->RawValue : (!m_pFB->RawValue);
	m_pFB->NegValue = (int8u)!m_pFB->Value;

	/****** if status changed , then trigger event ********/
	/****** so for virtual din, CPUEna must be CPU_ENABLE *****/
	if (bFlag == true && bCpuEnable == true)
	{
		if (m_pAlarm)
		{
			m_pAlarm->Update(m_pFB,nValue, m_pFB->Quality, pTm);
		}
	}
	
	return true;
}

bool CFesDin::Alarm(int8u nValue, int8u nQua, TIMEPAK * pTm)
{

	return true;
}

void CFesDin::GetValue(int8u &nValue, int8u &nQua)const
{
	Q_ASSERT(m_pFB);
	if (m_pFB == nullptr)
		return;

	nValue = m_pFB->Value;
	nQua = m_pFB->Quality;
}

void  CFesDin::SetQuality(int8u nQua)
{
	Q_ASSERT(m_pFB);
	if (m_pFB == nullptr)
		return;

	m_pFB->Quality = nQua;
}

CFesAout::CFesAout()
{
	m_nChannelMailBoxID = 0; 

	m_pChannel = nullptr;
	m_pDevcie = nullptr;
} 

CFesAout::~CFesAout()
{

}

CFesAout::CFesAout(AOUT *pAout) :m_pFB(pAout)
{
	Q_ASSERT(pAout);
	m_nChannelMailBoxID = 0;

	m_pChannel = nullptr;
	m_pDevcie = nullptr;
}

void  CFesAout::SetQuality(int8u nQua)
{
	Q_ASSERT(m_pFB);
	if (m_pFB == nullptr)
		return;

	m_pFB->Quality = nQua;
} 
/*! \fn bool CFesAout::CalcOutput(AOUT *pFB,fp64 fpRaw, fp64 & fpOutput)
*********************************************************************************************************
** \brief CFesAout::CalcOutput
** \details  �����������
** \param pFB
** \param fpRaw
** \param fpOutput
** \return bool
** \author LiJin
** \date 2016��10��12��
** \note
********************************************************************************************************/
bool CFesAout::CalcOutput(AOUT *pFB, fp64 fpRaw, fp64 & fpOutput)
{
	Q_ASSERT(pFB);
	if (pFB == nullptr)
		return false;

	Q_ASSERT(pFB->MaxRaw != pFB->MinRaw);
	if (pFB->MaxRaw == pFB->MinRaw)
	{
		return false;
	}

	fpOutput = (pFB->MaxScale - pFB->MinScale)*(fpRaw - pFB->MinRaw) / (pFB->MaxRaw - pFB->MinRaw) + pFB->MinScale;

	return true;
}
/*! \fn bool CFesAout::CalcNonLinearOutput(TRANSFORM_NONLINEAR *pNonLinear, fp64 fpRaw, fp64 & fpOutput)
*********************************************************************************************************
** \brief CFesAout::CalcNonLinearOutput
** \details ���������ת��
** \param pNonLinear
** \param fpRaw
** \param fpOutput
** \return bool
** \author LiJin
** \date 2016��10��13��
** \note
********************************************************************************************************/
bool CFesAout::CalcNonLinearOutput(TRANSFORM_NONLINEAR *pNonLinear, fp64 fpRaw, fp64 & fpOutput)
{
	Q_ASSERT(pNonLinear);
	if (pNonLinear == nullptr)
		return false;

	Q_ASSERT(pNonLinear->DataCount >= 2 && pNonLinear->DataCount <= MAX_NON_LINEAR_NUM);
	if (pNonLinear->DataCount < 2 || pNonLinear->DataCount > MAX_NON_LINEAR_NUM)
		return false;

	fp64 fpRet = 0.0f;

	int nCount = pNonLinear->DataCount;

	int nIndex = 0;

	//ע������ԭʼ���� ��������ʽ�������ڴ�ṹ��

	//�ڷ�Χ֮�� ������Ϊ���ݷǷ�
	if (pNonLinear->NonLinearData[0].RawValue > fpRaw || pNonLinear->NonLinearData[nCount].RawValue < fpRaw)
	{
		Q_ASSERT(false);
		return false;
	}

	//�������������飬���ϵ�һ������ >= fpRaw ���ҵ�����������ֵ
	for (int i = 0; i < nCount; ++i)
	{
		if (pNonLinear->NonLinearData[i].RawValue == fpRaw)
		{
			return pNonLinear->NonLinearData[i].ScaleValue;
		}
		else if (pNonLinear->NonLinearData[i].RawValue > fpRaw)
		{
			nIndex = i;
			break;
		}
	}
	Q_ASSERT(nIndex > 0 && nIndex < nCount);

	//ǰһ����ֵ
	float preRawVal = pNonLinear->NonLinearData[nIndex - 1].RawValue;
	float preScaleVal = pNonLinear->NonLinearData[nIndex - 1].ScaleValue;
	//��һ����ֵ
	float nextRawVal = pNonLinear->NonLinearData[nIndex].RawValue;
	float nextScaleVal = pNonLinear->NonLinearData[nIndex].ScaleValue;

	Q_ASSERT(nextRawVal == preRawVal);

	//ֵ���ַǷ����� 
	if (nextRawVal == preRawVal)
	{
		return false;
	}

	fpRet = (nextScaleVal - preScaleVal)*(fpRaw - preRawVal) / (nextRawVal - preRawVal) + preScaleVal;

	fpOutput = fpRet;

	return true;
}
/*! \fn bool CFesAout::CalcLinearOutput(TRANSFORM_LINEAR *pLinear, fp64 fpRaw, fp64 & fpOutput)
*********************************************************************************************************
** \brief CFesAout::CalcLinearOutput
** \details �����������
** \param pLinear
** \param fpRaw
** \param fpOutput
** \return bool
** \author LiJin
** \date 2016��10��12��
** \note
********************************************************************************************************/
bool CFesAout::CalcLinearOutput(TRANSFORM_LINEAR *pLinear, fp64 fpRaw, fp64 & fpOutput)
{
	Q_ASSERT(pLinear);
	if (pLinear == nullptr)
		return false;
	Q_ASSERT(pLinear->MaxRaw != pLinear->MinRaw);
	if (pLinear->MaxRaw == pLinear->MinRaw)
		return false;

	fp64 fpRet = 0.0f;

	if (pLinear->EnableInverse == 0)
	{// û�в��� ��������ת��
		fpRet = (pLinear->MaxScale - pLinear->MinScale)*(fpRaw - pLinear->MinRaw) / (pLinear->MaxRaw - pLinear->MinRaw) + pLinear->MinScale;
	}
	else
	{// ����:������ת�����÷���ת��
		fpRet = (pLinear->MaxScale - pLinear->MinScale)*(fpRaw - pLinear->MinRaw) / (pLinear->MinRaw - pLinear->MaxRaw) + pLinear->MaxScale;
	}

	if (pLinear->EnableFactor != 0)
	{// ���ü���ϵ��		
		if (pLinear->InverseFactor)
		{// ���÷���ϵ��
			Q_ASSERT(pLinear->Gain != 0.0f);
			if (pLinear->Gain != 0.0f)
			{
				fpOutput = (fpRet - pLinear->Offset) / (pLinear->Gain);
				return true;
			}
			else
			{
				return false;
			}
		}
		else
		{
			fpOutput = fpRet * pLinear->Gain + pLinear->Offset;
			return true;
		}
	}
	else
	{
		fpOutput = fpRet;
		return true;
	}
}

bool CFesAout::Update(fp64 fpValue, int8u nQua)
{
	Q_ASSERT(m_pFB);
	if (m_pFB == nullptr)
		return false;

	if (m_pFB->IsDefined != FES_YES)
		return false;

	fp64 fpTemp = fpValue;
	bool bRet = false;
	/******** if not initilized, then initilizing, no event is triggered ***********/
	if (m_pFB->Init != INITED)
	{	// ��ȡ ת��ϵ�� ����
		if (m_pFB->TransformOccNo != INVALID_OCCNO && m_pFB->TransformOccNo <= MAX_OCCNO)
		{
			if (m_pFB->TransformType == TRANSFORM_TYPE_LINEAR)
			{
				Q_ASSERT(m_fnGetTransformLinear);
				if (m_pLinear == nullptr && m_fnGetTransformLinear != nullptr)
				{
					m_pLinear = m_fnGetTransformLinear(m_pFB->TransformOccNo);
				}
				if (m_pLinear)
				{
					bRet = CalcLinearOutput(m_pLinear, fpValue, fpTemp);
					Q_ASSERT(bRet);
				}
			}
			else if (m_pFB->TransformType == TRANSFORM_TYPE_NONLINEAR)
			{
				Q_ASSERT(m_fnGetTransformNonLinear);
				if (m_pNonLinear == nullptr && m_fnGetTransformNonLinear != nullptr)
				{
					m_pNonLinear = m_fnGetTransformNonLinear(m_pFB->TransformOccNo);
					bRet = CalcNonLinearOutput(m_pNonLinear, fpValue, fpTemp);
					Q_ASSERT(bRet);
				}
			}
			else
			{
				Q_ASSERT(false);
			}
		}
		else
		{// �����ڽṹ��
			bRet = CalcOutput(m_pFB, fpValue, fpTemp);
			Q_ASSERT(bRet);
		}

		fp64 OutputL = m_pFB->RangeH + (m_pFB->RangeH - m_pFB->RangeL) * m_pFB->LowQua;
		fp64 OutputH = m_pFB->RangeL + (m_pFB->RangeH - m_pFB->RangeL) * m_pFB->HighQua;

		// ����Ʒ��
		if (fpTemp > OutputH || fpTemp < OutputL)
		{// ������Χ
			m_pFB->Quality = QUALITY_BAD;
		}
		else
		{
			m_pFB->Quality = nQua;
		}

		m_pFB->RawData = fpTemp;
		m_pFB->Init = INITED;
		return true;
	}
	/* if not in Scan, then return, we assume Output would be set manually, so we do give NotOutput a value */
	if (m_pFB->ScanEnable == SCAN_OUT)
	{//����˳�
		return false;
	}
	if (m_pDevcie && m_pDevcie->ScanEnable == SCAN_OUT)
	{
		return false;
	}
	if (m_pChannel && m_pChannel->ScanEnable == SCAN_OUT)
	{
		return false;
	}
	// ��ȡ ת��ϵ�� ����
	if (m_pFB->TransformOccNo != INVALID_OCCNO && m_pFB->TransformOccNo <= MAX_OCCNO)
	{
		if (m_pFB->TransformType == TRANSFORM_TYPE_LINEAR)
		{
			Q_ASSERT(m_fnGetTransformLinear);
			if (m_pLinear == nullptr && m_fnGetTransformLinear != nullptr)
			{
				m_pLinear = m_fnGetTransformLinear(m_pFB->TransformOccNo);
			}
			if (m_pLinear)
			{
				bRet = CalcLinearOutput(m_pLinear, fpValue, fpTemp);
				Q_ASSERT(bRet);
			}
		}
		else if (m_pFB->TransformType == TRANSFORM_TYPE_NONLINEAR)
		{
			Q_ASSERT(m_fnGetTransformNonLinear);
			if (m_pNonLinear == nullptr && m_fnGetTransformNonLinear != nullptr)
			{
				m_pNonLinear = m_fnGetTransformNonLinear(m_pFB->TransformOccNo);
				bRet = CalcNonLinearOutput(m_pNonLinear, fpValue, fpTemp);
				Q_ASSERT(bRet);
			}
		}
		else
		{
			Q_ASSERT(false);
		}
	}
	else
	{// �����ڽṹ��
		bRet = CalcOutput(m_pFB, fpValue, fpTemp);
		Q_ASSERT(bRet);
	}

	fp64 OutputL = m_pFB->RangeH + (m_pFB->RangeH - m_pFB->RangeL) * m_pFB->LowQua;
	fp64 OutputH = m_pFB->RangeL + (m_pFB->RangeH - m_pFB->RangeL) * m_pFB->HighQua;

	// ����Ʒ��
	if (fpTemp > OutputH || fpTemp < OutputL)
	{// ������Χ
		m_pFB->Quality = QUALITY_BAD;
	}
	else
	{
		m_pFB->Quality = nQua;
	}

	m_pFB->RawData = fpTemp;

	if (m_pFB->Quality == QUALITY_BAD)
	{
		return false;
	}
	/** To match Output and RawData, appliction that writes Output should
	give Filter a value (such as 5) when triggerring a aout cmd***********/



	return true;
}

CFesDout::CFesDout()
{
	m_nChannelMailBoxID = 0;

	m_pChannel = nullptr;
	m_pDevcie = nullptr;
} 

CFesDout::~CFesDout()
{

}

CFesDout::CFesDout(DOUT *pDout) :m_pFB(pDout)
{
	Q_ASSERT(pDout);
	m_nChannelMailBoxID = 0; 

	m_pChannel = nullptr;
	m_pDevcie = nullptr;
}

void  CFesDout::SetQuality(int8u nQua)
{
	Q_ASSERT(m_pFB);
	if (m_pFB == nullptr)
		return;

	m_pFB->Quality = nQua;
}
/*! \fn bool CFesDout::Update(int8u nValue, int8u nQua)
*********************************************************************************************************
** \brief CFesDout::Update
** \details �������²�ֵ
** \param nValue
** \param nQua
** \return bool
** \author LiJin
** \date 2016��10��20��
** \note   
********************************************************************************************************/
bool CFesDout::Update(int8u nValue, int8u nQua)
{
	Q_ASSERT(m_pFB);
	if (m_pFB == nullptr)
		return false;

	if (m_pFB->IsDefined != FES_YES)
		return false;
	
	if (nValue != 0)
	{
		nValue = 1;
	}

	if (m_pFB->Init != INITED)
	{
		m_pFB->RelayState = nValue;
		m_pFB->Output = m_pFB->RelayState;
		m_pFB->Quality = nQua;
		m_pFB->Init = INITED;
		return true;
	}

	/* if not in Scan, then return, we assume Output would be set manually, so we do give NotOutput a value */
	if (m_pFB->ScanEnable == SCAN_OUT)
	{//����˳�
		return false;
	}
	if (m_pDevcie && m_pDevcie->ScanEnable == SCAN_OUT)
	{
		return false;
	}
	if (m_pChannel && m_pChannel->ScanEnable == SCAN_OUT)
	{
		return false;
	}

	m_pFB->RelayState = nValue;
	m_pFB->Quality = nQua;

	/* if point is in bad quality, then only it's RelayState is recorded **/
	if (m_pFB->Quality == QUALITY_BAD)
		return false;
	/** To match Output and RelayState, appliction that writes Output should
	give Filter a value (such as 5) when triggerring a dout cmd***********/



	return true;
}