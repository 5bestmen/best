

#include "data_def.h"
#include "kernel.h"

#include "mail/mail.h"

/*! \fn 
********************************************************************************************************* 
** \brief CKernel::CreateAINTable 
** \details 
** \param pParentElement 配置文件中AIN区根节点
** \return bool true:成功，false:失败
** \date 2015年9月15日 
** \note 创建AIN内存库
********************************************************************************************************/
bool CKernel::CreateAINTable(TiXmlElement* pParentElement)
{
	int nCount = GetElementChildCount(pParentElement,"ai");
	if(nCount <=0)
	{
		return false;
	}

	if(!m_poolAIN.CreateArray(SMKEY_MEM_AIN,nCount))
	{
		return false;
	}

	TiXmlNode *pChild;
	
	for (pChild = pParentElement->FirstChild(); pChild != 0; pChild = pChild->NextSibling()) 
	{
		std::string strChildName = pChild->Value();

		if(!Utils::IsEqualIgnoreCase(strChildName,"ai"))
			continue;

		//获取AIN配置信息
		TiXmlElement* pAINElement = pChild->ToElement();           

		//获取属性值
		int nOccNo = 0;
		int nNameOccNo = 0;
		int nNodeOccNo = 0;
		int nChannelOccNo = 0;
		int nDeviceOccNo = 0;
		int nBlockOccNo = 0;
		int nAddress = 0;
		fp64 fInitValue = 0;
		int nDataSource = 0;
		int nTransformType = 1;
		int nTransformOccNo = 0;
		fp64 fRangeL = 0;
		fp64 fRangeH = 0;
		fp64 fHighQua = 0;
		fp64 fLowQua = 0;

		int nAlarmOccNo = 0;
		int nExpressOccNo = 0;
		int nUnitNameOccNo = 0;
		int nPinLabelOccNo = 0;
		std::string strAddress;

		pAINElement->QueryIntAttribute("OccNo", &nOccNo);
		pAINElement->QueryIntAttribute("NameOccNo", &nNameOccNo);
		pAINElement->QueryIntAttribute("NodeOccNo", &nNodeOccNo);
		pAINElement->QueryIntAttribute("ChannelOccNo", &nChannelOccNo);
		pAINElement->QueryIntAttribute("DeviceOccNo", &nDeviceOccNo);
		pAINElement->QueryIntAttribute("BlockOccNo", &nBlockOccNo);

		pAINElement->QueryStringAttribute("Address", &strAddress);

		pAINElement->QueryDoubleAttribute("InitValue", &fInitValue);
		pAINElement->QueryIntAttribute("DataSource", &nDataSource);

		pAINElement->QueryIntAttribute("TransformType", &nTransformType);
		pAINElement->QueryIntAttribute("TransformOccNo", &nTransformOccNo);

		pAINElement->QueryDoubleAttribute("RangeL", &fRangeL);
		pAINElement->QueryDoubleAttribute("RangeH", &fRangeH);

		pAINElement->QueryDoubleAttribute("HighQua", &fHighQua);
		pAINElement->QueryDoubleAttribute("LowQua", &fLowQua);

		pAINElement->QueryIntAttribute("AlarmOccNo", &nAlarmOccNo);
		pAINElement->QueryIntAttribute("ExpressOccNo", &nExpressOccNo);
		pAINElement->QueryIntAttribute("UnitNameOccNo", &nUnitNameOccNo);
		pAINElement->QueryIntAttribute("PinLabelOccNo", &nPinLabelOccNo);

		AIN* pAIN = GetAIN(nOccNo);
		if(pAIN==NULL)
		{
			return false;
		}

		//初始化AIN对象
		pAIN->OccNo = nOccNo;
		pAIN->NameOccNo = nNameOccNo;
		pAIN->NodeOccNo = nNodeOccNo;
		pAIN->ChannelOccNo = nChannelOccNo;
		pAIN->DeviceOccNo = nDeviceOccNo;
		pAIN->BlockNo = nBlockOccNo;
		pAIN->InitValue = fInitValue;
		pAIN->DataSource = nDataSource;
		pAIN->TransformType = nTransformType;
		pAIN->TransformOccNo = nTransformOccNo;
		pAIN->RangeH = fRangeH;
		pAIN->RangeL = fRangeL;
		pAIN->HighQua = fHighQua;
		pAIN->LowQua = fLowQua;

		pAIN->AlarmOccNo = nAlarmOccNo;
		pAIN->ExpressOccNo = nExpressOccNo;
		pAIN->UnitNameOccNo = nUnitNameOccNo;
		pAIN->PinLabelOccNo = nPinLabelOccNo;

		if (strAddress.length() > MAX_ADDRESS_LENGTH)
			strAddress = strAddress.substr(0, MAX_ADDRESS_LENGTH);
		sprintf(pAIN->Address, "%s", strAddress.c_str());
	}

	return true;
}

/*! \fn 
********************************************************************************************************* 
** \brief CKernel::OpenAINTable 
** \details 
** \return bool true:成功，false:失败
** \date 2015年9月15日 
** \note 打开AIN内存库
********************************************************************************************************/
bool CKernel::OpenAINTable()
{
	if(!m_poolAIN.OpenArray(SMKEY_MEM_AIN))
		return false;

	return true;
}

/*! \fn 
********************************************************************************************************* 
** \brief CKernel::DestroyAINTable 
** \details 
** \return void 
** \date 2015年9月15日 
** \note 关闭AIN内存库
********************************************************************************************************/
void CKernel::DestroyAINTable()
{
	m_poolAIN.CloseArray();
}

/*! \fn 
********************************************************************************************************* 
** \brief CKernel::GetAIN 
** \details 
** \param occNO 
** \return AIN* 
** \date 2015年9月15日 
** \note 根据AIN的OCCNO获取AIN数据指针
********************************************************************************************************/
AIN* CKernel::GetAIN(int32u occNO)
{
	int nCount = m_poolAIN.GetArraySize();
	occNO = occNO -1;//occNo从1开始
	if(occNO<0 || occNO>=nCount)
		return NULL;

	AIN* pAIN = m_poolAIN.GetObject(occNO);

	return pAIN;
}

/*! \fn bool CKernel::IoSetAINValue(int32u nOccNO,fp64 fValue,int32u nQuality)
********************************************************************************************************* 
** \brief CKernel::SetAINValue 
** \details 
** \param nOccNO 
** \param fValue 
** \param nQuality 
** \return bool 
** \date 2015年9月15日 
** \note 根据AIN的OCCNO设置其值和品质
********************************************************************************************************/
bool CKernel::IoSetAINValue(int32u nChannelOccNo, int32u nOccNO, fp64 fValue, int8u nQuality)
{
	AIN* pAIN = GetAIN(nOccNO);
	if (pAIN == NULL)
		return false;
	/*
	if (pAIN->IsDefined == IO_NO)
		return false;
	*/

	fp64 fTransValue = Transform(pAIN, fValue); 
	
	AINUpdate(pAIN, fValue, fTransValue, nQuality);

	AINAlarm(pAIN);

	AINEvent(nChannelOccNo,pAIN, false);

	/*
	fp64 DeadBand, OutputL, OutputH,tmp1;
	TRANSFORM *pTF = nullptr;

	_ASSERT(pFB->TransformOccNo);
	if (pFB->TransformOccNo)
	{
		pTF = GetAinTransForm(pFB->TransformOccNo);
	}

	// 没有初始化过
	if (pFB->Init != IO_INITED)
	{
		if (pFB->ScanEnable == IO_SCAN_OUT)
		{
			pFB->AlarmState = 0;
			pFB->Init = IO_INITED;
			return false;
		} 
		else
		{
			// 计算 转换关系
			// 获取转换关系，并计算出工程值
			if (pTF)
			{
			}


			tmp1 = 0.;

			// 计算有效值的范围
			OutputL = pFB->RangeH + (pFB->RangeH - pFB->RangeL) * pFB->LowQua;
			OutputH = pFB->RangeL + (pFB->RangeH - pFB->RangeL) * pFB->HighQua;

			// 如果测值不在有效范围内
			if (tmp1 > OutputH || tmp1 < OutputL)
			{ 
				pFB->Quality = IO_QUA_BAD;			 
			}
			else
			{
				pFB->Quality = nQuality ;
			}
			if (pFB->Quality == IO_QUA_GOOD)
			{
				pFB->Value = tmp1;
			}

			pFB->RawValue = fValue;
			pFB->AlarmState = 0;
			if (pFB->Quality == IO_QUA_GOOD && pFB->ScanEnable != IO_SCAN_OUT)
			{
#if 0
				if (pFB->Output >= pFB->HH && pFB->HH < 999999.0)
				{
					pFB->AlarmState |= AA_HH;
				}
				if ((pFB->Output >= pFB->H) && (pFB->H < 999999.0))
				{
					pFB->AlarmState |= AA_H;
				}
				if (pFB->Output <= pFB->L && pFB->L > -999999.0)
				{
					pFB->AlarmState |= AA_L;
				}
				if (pFB->Output <= pFB->LL && pFB->LL > -999999.0)
				{
					pFB->AlarmState |= AA_LL;
				}
#endif
			}
			pFB->Init = IO_INITED;
			return true;
		}
	}

	// 
	// if not in Scan, then return, we assume Output would be
	//set manually, so we do give NotOutput a value

	pFB->RawValue = fValue;
	pFB->Quality = nQuality;


	// 处理报警相关
	FE_MSG_NORMALEVENT msg;
	msg.Code = 1;
	// ....

	//SendMail();
	*/

	return true;
}

bool CKernel::AppSetAINValue(int32u nAppOccNo, int32u nOccNO, fp64 fValue, int8u nQuality)
{
	AIN* pAIN = GetAIN(nOccNO);
	if (pAIN == NULL)
		return false;
	if (pAIN->IsDefined == IO_NO)
		return false;

	fp64 fTransValue = Transform(pAIN, fValue);

	AINUpdate(pAIN, fValue, fTransValue, nQuality);

	AINAlarm(pAIN);

	AINEvent(nAppOccNo, pAIN, true);

	return true;
}

/*! \fn 
********************************************************************************************************* 
** \brief CKernel::GetAINValue 
** \details 
** \param occNO 
** \return fp32 
** \date 2015年9月15日 
** \note 根据AIN的OCCNO获取其值
********************************************************************************************************/
fp64 CKernel::GetAINValue(int32u occNO)
{
	AIN* pAIN = GetAIN(occNO);
	if(pAIN == NULL)
		return 0;

	return pAIN->RawValue;
}

fp64 CKernel::Transform(AIN* pAIN, fp64 fValue)
{
	if (pAIN->TransformType == TRANSFORM_TYPE_LINEAR)
		return LinearTransform(pAIN->TransformOccNo, fValue);
	else if (pAIN->TransformType == TRANSFORM_TYPE_NONLINEAR)
		return NonLinearTransform(pAIN->TransformOccNo, fValue);
	else
		return fValue;
}

void CKernel::AINUpdate(AIN* pAIN, fp64 fValue, fp64 fTransValue, int8u nQuality)
{
	//设值
	pAIN->RawValue = fValue;
	pAIN->OldValue = pAIN->Value;
	pAIN->Value = fTransValue;

	//3、设品质
	fp64 OutputL = pAIN->RangeH + (pAIN->RangeH - pAIN->RangeL) * pAIN->LowQua;
	fp64 OutputH = pAIN->RangeL + (pAIN->RangeH - pAIN->RangeL) * pAIN->HighQua;
	if (fTransValue > OutputH || fTransValue < OutputL)
	{
		nQuality = IO_QUA_BAD;
	}
	pAIN->Quality = nQuality;
}

bool CKernel::AINAlarm(AIN* pAIN)
{
	AIN_ALARM* pAINAlarm = GetAinAlarm(pAIN->AlarmOccNo);
	if (pAINAlarm == NULL)
		return false;

	int8u nOldAlarmState = pAIN->AlarmState;
	int8u nNewAlarmState = AIN_NORMAL;

	if (pAIN->Value >= pAINAlarm->HH && pAINAlarm->HH < 999999.0)
	{
		nNewAlarmState = AIN_OVER_HH;
	}
	else if ((pAIN->Value >= pAINAlarm->H) && (pAINAlarm->H < 999999.0))
	{
		nNewAlarmState = AIN_OVER_H;
	}
	if (pAIN->Value <= pAINAlarm->L && pAINAlarm->L > -999999.0)
	{
		nNewAlarmState = AIN_OVER_L;
	}
	if (pAIN->Value <= pAINAlarm->LL && pAINAlarm->LL > -999999.0)
	{
		nNewAlarmState = AIN_OVER_LL;
	}

	return true;
}

bool CKernel::AINEvent(int32u nChannelOccNo, AIN* pAIN, bool IsManSet)
{
	/*
	if (IsManSet)
	{
		MSG_SETVAL msg;
		msg.Type = MT_SETVALUE;
		msg.Len = sizeof(MSG_SETVAL)-sizeof(MSG_BASE);

		return Mail::SendMail(SMKEY_MAIL_CORE + nChannelOccNo, SMKEY_MAIL_CORE, (char*)(&msg), sizeof(MSG_EVENT));
	}
	else
	{
		MSG_EVENT msg;
		msg.Type = MT_NORMAL_EVENT;
		msg.Len = sizeof(MSG_EVENT)-sizeof(MSG_BASE);
		msg.Occno = pAIN->OccNo;
		

		return Mail::SendMail(SMKEY_MAIL_CORE + nChannelOccNo, SMKEY_MAIL_CORE, (char*)(&msg), sizeof(MSG_EVENT));
	}
	*/
	return true;
}


int32u CKernel::GetAinCount(int32u nChannelOccNo, int32u nDeviceOccNo)
{
	int nAinCount = 0;
	int nCount = m_poolAIN.GetArraySize();
	
	for (int i = 0; i < nCount; i++)
	{
		AIN* pAIN = m_poolAIN.GetObject(i);
		if (pAIN==NULL)
			continue;

		if ((pAIN->ChannelOccNo == nChannelOccNo) && (pAIN->DeviceOccNo == nDeviceOccNo))
			nAinCount++;
	}

	return nAinCount;
}

void CKernel::GetAinOccNos(int32u nChannelOccNo, int32u nDeviceOccNo,std::vector<int32u>& vecAinOccNos)
{
	int32u nCount = m_poolAIN.GetArraySize();
	for (int i = 0; i < nCount; i++)
	{
		AIN* pAIN = m_poolAIN.GetObject(i);
		if (pAIN == NULL)
			continue;

		if ((pAIN->ChannelOccNo == nChannelOccNo) && (pAIN->DeviceOccNo == nDeviceOccNo) )
		{
			vecAinOccNos.push_back(pAIN->OccNo);
		}
	}
}

int32u CKernel::GetAinAddress(int32u nAinOccNo)
{
	AIN* pAIN = GetAIN(nAinOccNo);
	if (pAIN == NULL)
		return 0;

	std::string strAddress = pAIN->Address;

	//return atoi((char*)strAddress.c_str());
	return Utils::HexToInt((char*)strAddress.c_str());
}
