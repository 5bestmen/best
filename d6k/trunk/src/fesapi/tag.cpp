


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
** \details  计算线性输出
** \param pFB 
** \param fpRaw 
** \param fpOutput 
** \return bool 
** \author LiJin 
** \date 2016年10月12日 
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
** \details 计算非线性转换
** \param pNonLinear 
** \param fpRaw 
** \param fpOutput 
** \return bool 
** \author LiJin 
** \date 2016年10月13日 
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

	//注：遍历原始数据 按照升序方式放置在内存结构中

	//在范围之外 ，则认为数据非法
	if (pNonLinear->NonLinearData[0].RawValue > fpRaw || pNonLinear->NonLinearData[nCount].RawValue <fpRaw)
	{
		Q_ASSERT(false);
		return false;
	}

	//遍历非线性数组，符合第一个满足 >= fpRaw 则找到符合条件的值
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

	//前一个数值
	float preRawVal = pNonLinear->NonLinearData[nIndex - 1].RawValue;
	float preScaleVal = pNonLinear->NonLinearData[nIndex - 1].ScaleValue;
	//后一个数值
	float nextRawVal = pNonLinear->NonLinearData[nIndex].RawValue;
	float nextScaleVal = pNonLinear->NonLinearData[nIndex].ScaleValue;

	Q_ASSERT( nextRawVal == preRawVal );

	//值出现非法配置 
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
** \details 计算线性输出
** \param pLinear 
** \param fpRaw 
** \param fpOutput 
** \return bool 
** \author LiJin 
** \date 2016年10月12日 
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
	{// 没有采用 反比线性转换
		fpRet = (pLinear->MaxScale - pLinear->MinScale)*(fpRaw - pLinear->MinRaw) / (pLinear->MaxRaw-pLinear->MinRaw) + pLinear->MinScale;
	}
	else
	{// 反比:工程量转换采用反比转换
		fpRet = (pLinear->MaxScale - pLinear->MinScale)*(fpRaw - pLinear->MinRaw) / (pLinear->MinRaw - pLinear->MaxRaw) + pLinear->MaxScale;
	}
	
	if (pLinear->EnableFactor != 0)
	{// 启用计算系数		
		if (pLinear->InverseFactor)
		{// 启用反比系数
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
			// 清报警状态

			m_pFB->Init = INITED;
			return false;
		}
		else
		{
			// 获取 转换系数 计算
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
			{// 内置在结构中
				bRet = CalcOutput(m_pFB, fpValue, fpTemp);
				Q_ASSERT(bRet);
			}

			fp64 OutputL = m_pFB->RangeH + (m_pFB->RangeH - m_pFB->RangeL) * m_pFB->LowQua;
			fp64 OutputH = m_pFB->RangeL + (m_pFB->RangeH - m_pFB->RangeL) * m_pFB->HighQua;

			// 处理品质
			if (fpTemp > OutputH || fpTemp < OutputL)
			{// 超过范围
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
	{//测点退出
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

	// 获取 转换系数 计算
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
	{// 内置在结构中
		bRet = CalcOutput(m_pFB, fpValue, fpTemp);
		Q_ASSERT(bRet);
	}

	fp64 OutputL = m_pFB->RangeH + (m_pFB->RangeH - m_pFB->RangeL) * m_pFB->LowQua;
	fp64 OutputH = m_pFB->RangeL + (m_pFB->RangeH - m_pFB->RangeL) * m_pFB->HighQua;

	// 处理品质
	if (fpTemp > OutputH || fpTemp < OutputL)
	{// 超过范围
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
		// 清告警状态
		ClearAlarmState();
		return true;
	}

	// 获取告警相关配置
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
** \details IO驱动更新测值
** \param fpValue 
** \param nQua 
** \return bool 
** \author LiJin 
** \date 2016年10月12日 
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
** \details 加载配置，在初始化时调用
** \return void 
** \author LiJin 
** \date 2016年10月18日 
** \note 
********************************************************************************************************/
void  CFesAin::LoadConfig()
{
	Q_ASSERT(m_pFB);
	if (m_pFB == nullptr)
		return;

	// 告警相关，模拟量存在没有配置告警的可能
	if (m_pFB->AlarmOccNo != INVALID_OCCNO && m_pFB->AlarmOccNo <= MAX_OCCNO)
	{
		if (m_pAlarm == nullptr)
		{// 没有获取报警参数，则加载报警配置
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
	// 获取邮箱的ID
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
** \details IO通讯层修改测点品质
** \param nQua 
** \return void 
** \author LiJin 
** \date 2016年10月27日 
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

	// 告警相关
	if (m_pFB->AlarmOccNo != INVALID_OCCNO && m_pFB->AlarmOccNo < MAX_OCCNO)
	{
		if (m_pAlarm == nullptr)
		{// 没有获取报警参数，则加载报警配置
			m_pAlarm = m_fnGetAlarm(m_pFB->AlarmOccNo);
			Q_ASSERT(m_pAlarm);
			m_pAlarm->LoadConfig(m_pFB);
		}
	}
}
/*! \fn bool CFesDin::Update(int8u nValue, int8u nQua, bool bCpuEnable)
********************************************************************************************************* 
** \brief CFesDin::Update 
** \details 驱动更新测值
** \param nValue 
** \param nQua 
** \param bCpuEnable true: 由后台报警变位，false，告警信息由装置单独上送
** \return bool 
** \author LiJin 
** \date 2016年10月26日 
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

	// 如果定义了从遥信的号，说明该点是双位遥信，而且该遥信是主遥信
	if (m_pFB->SlaveOccNo != INVALID_OCCNO && m_pFB->SlaveOccNo != MAX_OCCNO)
	{// 处理双位遥信


		return false;
	}

	return NormalUpdate(nValue, nQua, bCpuEnable,pTm);
}
/*! \fn bool CFesDin::Update(int8u nValue, int8u nQua)
********************************************************************************************************* 
** \brief CFesDin::Update 
** \details 驱动更新测值
** \param nValue 
** \param nQua 
** \return bool 
** \author LiJin 
** \date 2016年10月20日 
** \note  注意双位遥信的处理
********************************************************************************************************/
bool CFesDin::Update(int8u nValue, int8u nQua)
{
	return Update(nValue, nQua, false);
}
/*! \fn bool CFesDin::NormalUpdate(int8u nValue, int8u nQua)
********************************************************************************************************* 
** \brief CFesDin::NormalUpdate 
** \details 普通遥信（双位遥信的从遥信）测值更新
** \param nValue 
** \param nQua 
** \return bool 
** \author LiJin 
** \date 2016年10月26日 
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
		{//退出状态
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
	{//测点退出
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
	// 常开节点，如果新的值与旧值不一致
	// 常闭节点，如果新的值与旧值一样，则需要告警，否则不需要告警
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
** \details  计算线性输出
** \param pFB
** \param fpRaw
** \param fpOutput
** \return bool
** \author LiJin
** \date 2016年10月12日
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
** \details 计算非线性转换
** \param pNonLinear
** \param fpRaw
** \param fpOutput
** \return bool
** \author LiJin
** \date 2016年10月13日
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

	//注：遍历原始数据 按照升序方式放置在内存结构中

	//在范围之外 ，则认为数据非法
	if (pNonLinear->NonLinearData[0].RawValue > fpRaw || pNonLinear->NonLinearData[nCount].RawValue < fpRaw)
	{
		Q_ASSERT(false);
		return false;
	}

	//遍历非线性数组，符合第一个满足 >= fpRaw 则找到符合条件的值
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

	//前一个数值
	float preRawVal = pNonLinear->NonLinearData[nIndex - 1].RawValue;
	float preScaleVal = pNonLinear->NonLinearData[nIndex - 1].ScaleValue;
	//后一个数值
	float nextRawVal = pNonLinear->NonLinearData[nIndex].RawValue;
	float nextScaleVal = pNonLinear->NonLinearData[nIndex].ScaleValue;

	Q_ASSERT(nextRawVal == preRawVal);

	//值出现非法配置 
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
** \details 计算线性输出
** \param pLinear
** \param fpRaw
** \param fpOutput
** \return bool
** \author LiJin
** \date 2016年10月12日
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
	{// 没有采用 反比线性转换
		fpRet = (pLinear->MaxScale - pLinear->MinScale)*(fpRaw - pLinear->MinRaw) / (pLinear->MaxRaw - pLinear->MinRaw) + pLinear->MinScale;
	}
	else
	{// 反比:工程量转换采用反比转换
		fpRet = (pLinear->MaxScale - pLinear->MinScale)*(fpRaw - pLinear->MinRaw) / (pLinear->MinRaw - pLinear->MaxRaw) + pLinear->MaxScale;
	}

	if (pLinear->EnableFactor != 0)
	{// 启用计算系数		
		if (pLinear->InverseFactor)
		{// 启用反比系数
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
	{	// 获取 转换系数 计算
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
		{// 内置在结构中
			bRet = CalcOutput(m_pFB, fpValue, fpTemp);
			Q_ASSERT(bRet);
		}

		fp64 OutputL = m_pFB->RangeH + (m_pFB->RangeH - m_pFB->RangeL) * m_pFB->LowQua;
		fp64 OutputH = m_pFB->RangeL + (m_pFB->RangeH - m_pFB->RangeL) * m_pFB->HighQua;

		// 处理品质
		if (fpTemp > OutputH || fpTemp < OutputL)
		{// 超过范围
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
	{//测点退出
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
	// 获取 转换系数 计算
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
	{// 内置在结构中
		bRet = CalcOutput(m_pFB, fpValue, fpTemp);
		Q_ASSERT(bRet);
	}

	fp64 OutputL = m_pFB->RangeH + (m_pFB->RangeH - m_pFB->RangeL) * m_pFB->LowQua;
	fp64 OutputH = m_pFB->RangeL + (m_pFB->RangeH - m_pFB->RangeL) * m_pFB->HighQua;

	// 处理品质
	if (fpTemp > OutputH || fpTemp < OutputL)
	{// 超过范围
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
** \details 驱动更新测值
** \param nValue
** \param nQua
** \return bool
** \author LiJin
** \date 2016年10月20日
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
	{//测点退出
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