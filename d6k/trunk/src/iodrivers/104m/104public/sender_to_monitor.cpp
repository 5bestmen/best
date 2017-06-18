
#include "sender.h"

#include <ace/Date_Time.h> 
#include <ace/OS_NS_sys_time.h>

#include "ihost.h"
#include "104impl.h"



//////////////////////////////////////////////////////////////////////////
//从 Control发送消息到 Monitor
//////////////////////////////////////////////////////////////////////////

bool CSender::OnSendGeneralCall(NBM_TELECTRL* pTelectrl)
{
	char buf[255];

	//组织ASDU100
	ASDU100* pAsdu100 = (ASDU100*)(buf + sizeof(APCI));

	pAsdu100->type = C_IC_NA_1;
	pAsdu100->vsq = 0x01;

	pAsdu100->cot.SetCot(COT_ACT);

	int nDeviceAddr=1;// = m_pHost->getKernel()->GetDeviceAddress(pTelectrl->m_nDeviceID);
	pAsdu100->asduAddr.SetAddr(nDeviceAddr);

	pAsdu100->m_infoaddr.SetAddr(0x00);

	pAsdu100->m_qoi = 0x14;

	int nResult = Send_I(buf, sizeof(ASDU100));

	if (nResult != SEND_OK)
	{
		char buf[128];
		sprintf(buf, "OnSendGeneralCall failed:%d  K:%d W:%d", nResult, m_pImpl->m_k, m_pImpl->m_w);
		m_pHost->LogCallback104(MCB_ERROR, TYPE_I, buf);
	}

	return (nResult == SEND_OK) ? true : false;
}

bool CSender::OnSendKwhGeneralCall(NBM_TELECTRL* pTelectrl)
{
	char buf[255];

	//组织ASDU101
	ASDU101* pAsdu101 = (ASDU101*)(buf + sizeof(APCI));

	pAsdu101->type = C_CI_NA_1;
	pAsdu101->vsq = 0x01;

	pAsdu101->cot.SetCot(COT_ACT);

	int nDeviceAddr=1;// = m_pHost->getKernel()->GetDeviceAddress(pTelectrl->m_nDeviceID);
	pAsdu101->asduAddr.SetAddr(nDeviceAddr);

	pAsdu101->m_infoaddr.SetAddr(0x00);

	pAsdu101->m_qcc = 0x45;

	int nResult = Send_I(buf, sizeof(ASDU101));

	if (nResult != SEND_OK)
	{
		char buf[128];
		sprintf(buf, "OnSendKwhGeneralCall failed:%d  K:%d W:%d", nResult, m_pImpl->m_k, m_pImpl->m_w);
		m_pHost->LogCallback104(MCB_ERROR, TYPE_I, buf);
	}

	return (nResult == SEND_OK) ? true : false;
}

bool CSender::OnSendSyncTime(NBM_TELECTRL* pTelectrl)
{
	char buf[255];

	//组织ASDU103
	ASDU103* pAsdu103 = (ASDU103*)(buf + sizeof(APCI));

	pAsdu103->type = C_CS_NA_1;
	pAsdu103->vsq = 0x01;

	pAsdu103->cot.SetCot(COT_ACT);

	int nDeviceAddr=1;// = m_pHost->getKernel()->GetDeviceAddress(pTelectrl->m_nDeviceID);
	pAsdu103->asduAddr.SetAddr(nDeviceAddr);

	pAsdu103->m_infoaddr.SetAddr(0x00);

	struct CP56Time2a* cp56time2a = (struct CP56Time2a*)(&(pAsdu103->m_time));

	ACE_Date_Time tvTime(ACE_OS::gettimeofday());
	cp56time2a->Initialize(tvTime.year(), tvTime.month(), tvTime.day(), tvTime.hour(), tvTime.minute(), tvTime.second(), tvTime.microsec());

	int nResult = Send_I(buf, sizeof(ASDU103));

	if (nResult != SEND_OK)
	{
		char buf[128];
		sprintf(buf, "OnSendSyncTime failed:%d  K:%d W:%d", nResult, m_pImpl->m_k, m_pImpl->m_w);
		m_pHost->LogCallback104(MCB_ERROR, TYPE_I, buf);
	}

	return (nResult == SEND_OK) ? true : false;
}

bool CSender::OnSendSetBinarySPRequest(NBM_TELECTRL* pTelectrl)
{
	char buf[255];

	//组织ASDU45
	ASDU45* pAsdu45 = (ASDU45*)(buf + sizeof(APCI));

	pAsdu45->type = C_SC_NA_1;
	pAsdu45->vsq = 0x01;

	int nDeviceAddr=1;// = m_pHost->getKernel()->GetDeviceAddress(pTelectrl->m_nDeviceID);
	pAsdu45->asduAddr.SetAddr(nDeviceAddr);

	int nBinaryTelectrlStartAddr = m_pImpl->GetParam()->m_nBinaryTelectrlStartAddr;

	pAsdu45->m_infoaddr.SetAddr(pTelectrl->m_nDataID + nBinaryTelectrlStartAddr);

	pAsdu45->cot.SetCot((pTelectrl->m_nCtrlType == TELECTRL_REQUEST_UNSELECT) ? COT_DEACT : COT_ACT);
	pAsdu45->m_sco.SelectOrExcute = (pTelectrl->m_nCtrlType == TELECTRL_REQUEST_SELECT) ? 1 : 0;
	pAsdu45->m_sco.QU = 0;
	pAsdu45->m_sco.RES = 0;
	pAsdu45->m_sco.SCS = pTelectrl->m_fValue;

	int nResult = Send_I(buf, sizeof(ASDU45));

	if (nResult != SEND_OK)
	{
		char buf[128];
		sprintf(buf, "OnSendSetBinarySPRequest failed:%d  K:%d W:%d", nResult, m_pImpl->m_k, m_pImpl->m_w);
		m_pHost->LogCallback104(MCB_ERROR, TYPE_I, buf);
	}

	string strMsg = pTelectrl->Dump();
	strMsg += (nResult == SEND_OK) ? "[OK]" : "[FAILED]";
	// m_pHost->getKernel()->Log2File("log/telectrl_driver_send_to_device_binary.log",(char*)strMsg.c_str(),true);

	return (nResult == SEND_OK) ? true : false;
}

bool CSender::OnSendSetBinaryDPRequest(NBM_TELECTRL* pTelectrl)
{
	char buf[255];

	//组织ASDU46
	ASDU46* pAsdu46 = (ASDU46*)(buf + sizeof(APCI));

	pAsdu46->type = C_DC_NA_1;
	pAsdu46->vsq = 0x01;

	int nDeviceAddr=1;// = m_pHost->getKernel()->GetDeviceAddress(pTelectrl->m_nDeviceID);
	pAsdu46->asduAddr.SetAddr(nDeviceAddr);

	int nBinaryTelectrlStartAddr = m_pImpl->GetParam()->m_nBinaryTelectrlStartAddr;
	pAsdu46->m_infoaddr.SetAddr(pTelectrl->m_nDataID + nBinaryTelectrlStartAddr);

	pAsdu46->cot.SetCot((pTelectrl->m_nCtrlType == TELECTRL_REQUEST_UNSELECT) ? COT_DEACT : COT_ACT);
	pAsdu46->m_dco.SelectOrExcute = (pTelectrl->m_nCtrlType == TELECTRL_REQUEST_SELECT) ? 1 : 0;
	pAsdu46->m_dco.QU = 0;
	pAsdu46->m_dco.DCS = pTelectrl->m_fValue + 1;//双点01分，10合

	int nResult = Send_I(buf, sizeof(ASDU46));

	if (nResult != SEND_OK)
	{
		char buf[128];
		sprintf(buf, "OnSendSetBinaryDPRequest failed:%d  K:%d W:%d", nResult, m_pImpl->m_k, m_pImpl->m_w);
		m_pHost->LogCallback104(MCB_ERROR, TYPE_I, buf);
	}

	return (nResult == SEND_OK) ? true : false;
}

bool CSender::OnSendSetAnalogNormalizeRequest(NBM_TELECTRL* pTelectrl)//归一化设点
{
	char buf[255];

	//组织ASDU48
	ASDU48* pAsdu48 = (ASDU48*)(buf + sizeof(APCI));

	pAsdu48->type = C_SE_NA_1;
	pAsdu48->vsq = 0x01;


	int nDeviceAddr=1;// = m_pHost->getKernel()->GetDeviceAddress(pTelectrl->m_nDeviceID);
	pAsdu48->asduAddr.SetAddr(nDeviceAddr);

	int nAnalogTelectrlStartAddr = m_pImpl->GetParam()->m_nAnalogTelectrlStartAddr;

	pAsdu48->m_infoaddr.SetAddr(pTelectrl->m_nDataID + nAnalogTelectrlStartAddr);

	pAsdu48->SetValue(pTelectrl->m_fValue);

	pAsdu48->cot.SetCot((pTelectrl->m_nCtrlType == TELECTRL_REQUEST_UNSELECT) ? COT_DEACT : COT_ACT);
	pAsdu48->m_qos.SelectOrExcute = (pTelectrl->m_nCtrlType == TELECTRL_REQUEST_SELECT) ? 1 : 0;
	pAsdu48->m_qos.QL = 0;


	int nResult = Send_I(buf, sizeof(ASDU48));

	if (nResult != SEND_OK)
	{
		char buf[128];
		sprintf(buf, "OnSendSetAnalogNormalizeRequest failed:%d  K:%d W:%d", nResult, m_pImpl->m_k, m_pImpl->m_w);
		m_pHost->LogCallback104(MCB_ERROR, TYPE_I, buf);
	}


	string strMsg = pTelectrl->Dump();
	strMsg += (nResult == SEND_OK) ? "[OK]" : "[FAILED]";
	//   m_pHost->getKernel()->Log2File("log/telectrl_driver_send_to_device_analog.log",(char*)strMsg.c_str(),true);

	return (nResult == SEND_OK) ? true : false;
}


bool CSender::OnSendSetAnalogScaleRequest(NBM_TELECTRL* pTelectrl)//标度化设点
{
	char buf[255];

	//组织ASDU49
	ASDU49* pAsdu49 = (ASDU49*)(buf + sizeof(APCI));

	pAsdu49->type = C_SE_NB_1;
	pAsdu49->vsq = 0x01;


	int nDeviceAddr=1;// = m_pHost->getKernel()->GetDeviceAddress(pTelectrl->m_nDeviceID);
	pAsdu49->asduAddr.SetAddr(nDeviceAddr);

	int nAnalogTelectrlStartAddr = m_pImpl->GetParam()->m_nAnalogTelectrlStartAddr;
	pAsdu49->m_infoaddr.SetAddr(pTelectrl->m_nDataID + nAnalogTelectrlStartAddr);

	pAsdu49->SetValue(pTelectrl->m_fValue);

	pAsdu49->cot.SetCot((pTelectrl->m_nCtrlType == TELECTRL_REQUEST_UNSELECT) ? COT_DEACT : COT_ACT);
	pAsdu49->m_qos.SelectOrExcute = (pTelectrl->m_nCtrlType == TELECTRL_REQUEST_SELECT) ? 1 : 0;
	pAsdu49->m_qos.QL = 0;


	int nResult = Send_I(buf, sizeof(ASDU49));

	if (nResult != SEND_OK)
	{
		char buf[128];
		sprintf(buf, "OnSendSetAnalogScaleRequest failed:%d  K:%d W:%d", nResult, m_pImpl->m_k, m_pImpl->m_w);
		m_pHost->LogCallback104(MCB_ERROR, TYPE_I, buf);
	}

	return (nResult == SEND_OK) ? true : false;
}


bool CSender::OnSendSetAnalogShortFloatRequest(NBM_TELECTRL* pTelectrl)//短浮点数设点
{
	char buf[255];

	//组织ASDU50
	ASDU50* pAsdu50 = (ASDU50*)(buf + sizeof(APCI));

	pAsdu50->type = C_SE_NC_1;
	pAsdu50->vsq = 0x01;


	int nDeviceAddr=1;// = m_pHost->getKernel()->GetDeviceAddress(pTelectrl->m_nDeviceID);
	pAsdu50->asduAddr.SetAddr(nDeviceAddr);

	int nAnalogTelectrlStartAddr = m_pImpl->GetParam()->m_nAnalogTelectrlStartAddr;
	pAsdu50->m_infoaddr.SetAddr(pTelectrl->m_nDataID + nAnalogTelectrlStartAddr);

	pAsdu50->SetValue(pTelectrl->m_fValue);

	pAsdu50->cot.SetCot((pTelectrl->m_nCtrlType == TELECTRL_REQUEST_UNSELECT) ? COT_DEACT : COT_ACT);
	pAsdu50->m_qos.SelectOrExcute = (pTelectrl->m_nCtrlType == TELECTRL_REQUEST_SELECT) ? 1 : 0;
	pAsdu50->m_qos.QL = 0;


	int nResult = Send_I(buf, sizeof(ASDU50));

	if (nResult != SEND_OK)
	{
		char buf[128];
		sprintf(buf, "OnSendSetAnalogShortFloatRequest failed:%d  K:%d W:%d", nResult, m_pImpl->m_k, m_pImpl->m_w);
		m_pHost->LogCallback104(MCB_ERROR, TYPE_I, buf);
	}

	return (nResult == SEND_OK) ? true : false;
}


