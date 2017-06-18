/*! @file rtnet_msg_packer.h
<PRE>
********************************************************************************
ģ����       :
�ļ���       :  rtnet_msg_packer.h
�ļ�ʵ�ֹ��� :  ��������ģ��->���ݴ��������ʵʱ���ݿ��л�ȡ���ݲ���֯�ɱ���
����         :  LiJin
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         :  <����˵��>
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

********************************************************************************
</PRE>
*  @brief   ���ݴ����
*  @author  LiJin
*  @version 1.0
*  @date    2016.11.30
*******************************************************************************/
#ifndef RT_NET_MSG_PACKER_H
#define RT_NET_MSG_PACKER_H

#include "netmsg_packer.h"
#include "datatypes.h"

//������������
enum 
{
	INVALID_TYPE=0,
	AIN_TYPE=1,
	DIN_TYPE=2,
	MAX_DATA_TYPE=100,
};

//������ֽ���
enum 
{
	MAX_NETBUS_PAK_LENGTH = 398 * 1024,
};

//����ԭ��
enum 
{
	COT_NORMAL_ASYNC  =1,      //��������ͬ��
	COT_INCIDENT_ENENT=2,    //ͻ���¼�
};

class DATA_BASE
{
public:
	int m_nType;                      //!<������������
	int m_nCount;                     //!<����������Ŀ
	int m_nNodeOccNo;                 //!<�ڵ�OccNo
	int m_nStartOccNo;                //!<����������ʼ��ַ
	int m_nTransReason;               //!<���ݴ���ԭ��
	int m_nExtraLen;
};

const int32u MAX_AIN_COUNT = (MAX_NETBUS_PAK_LENGTH - sizeof DATA_BASE)/ (sizeof (FP64) );
const int32u MAX_DIN_COUNT = (MAX_NETBUS_PAK_LENGTH - sizeof DATA_BASE) / (sizeof(int8u));

class CAIN_DATA : public DATA_BASE
{
public:
	enum { MAX_AIN_COUNT = (MAX_NETBUS_PAK_LENGTH - sizeof DATA_BASE) / sizeof(FP64) };
	FP64 m_arrAins[MAX_AIN_COUNT];
public:
	void SetAinCount(int nCount)
	{
		Q_ASSERT(nCount > 0 && nCount<MAX_AIN_COUNT);
		if (nCount<=0 || nCount >MAX_AIN_COUNT)
		{
			return;
		}
		m_nCount = nCount;
	}

	int GetAinCount()
	{
		return m_nCount;
	}

	void SetDataType(int nType)
	{
		Q_ASSERT(nType!=INVALID_TYPE && nType <MAX_DATA_TYPE);
		if (nType ==INVALID_TYPE || nType >MAX_DATA_TYPE)
		{
			return;
		}
		m_nType = nType;
	}

	void SetStartOccNo(int32u nOccNo)
	{
		Q_ASSERT(nOccNo !=INVALID_OCCNO && nOccNo <= MAX_OCCNO);
		if (nOccNo== INVALID_OCCNO || nOccNo >MAX_OCCNO)
		{
			return;
		}
		m_nStartOccNo = nOccNo;
	}

	int GetPakLength()
	{
		return sizeof fp64 * m_nCount + sizeof DATA_BASE;
	}

	fp64 GetAinVal(int nIndex)
	{
		Q_ASSERT(nIndex >=0 && nIndex < MAX_AIN_COUNT);
		if (nIndex <0 || nIndex >MAX_AIN_COUNT)
		{
			return -1.0;
		}
		return m_arrAins[nIndex];
	}
};

class CDIN_DATA :public DATA_BASE
{
public:
	enum { MAX_DIN_COUNT  = ((MAX_NETBUS_PAK_LENGTH - sizeof DATA_BASE)) / (sizeof(int8u)) };
	int8u  m_arrDins[MAX_DIN_COUNT];
public:
	void SetDinCount(int nCount)
	{
		Q_ASSERT(nCount>0 && nCount <= MAX_DIN_COUNT);
		if (nCount<=0 || nCount > MAX_DIN_COUNT)
		{
			return;
		}
		m_nCount = nCount;
	}

	int GetDinCount()
	{
		return m_nCount;
	}

	void SetDataType(int nType)
	{
		Q_ASSERT(nType != INVALID_TYPE && nType < MAX_DATA_TYPE);
		if (nType == INVALID_TYPE || nType > MAX_DATA_TYPE)
		{
			return;
		}
		m_nType = nType;
	}

	void SetStartOccNo(int32u nOccNo)
	{
		Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
		if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
		{
			return;
		}
		m_nStartOccNo = nOccNo;
	}

	int GetPakLength()
	{
		return m_nCount + sizeof DATA_BASE;
	}

	int8u GetDinVal(int nIndex)
	{
		Q_ASSERT(nIndex >= 0 && nIndex < MAX_DIN_COUNT);
		if (nIndex <0 || nIndex >MAX_DIN_COUNT)
		{
			return 0;
		}
		return m_arrDins[nIndex];
	}
};


template<typename DBSvc>
class CRTNetMsgPacker :public CNetMsgPacker
{
public:
	CRTNetMsgPacker(std::shared_ptr<DBSvc> & pDB) : m_pDBSvc(pDB)
	{
		assert(pDB);
	}
	size_t PackageAllRTData(unsigned char *pInBuff, size_t nBuffLen)
	{
		size_t nDiCount = 0;
		int32u nNodeOccNO = m_pDBSvc->GetMyNodeOccNo();

		if (m_pDBSvc->IsDBAlive(0))
		{

		}

		return 0;
	}
	
	void GetAIPackageNum(std::vector<std::pair<int32u, int32u > >& arrNums)
	{
		assert(m_pDBSvc);
		if (m_pDBSvc == nullptr)
			return;

		size_t nEstimateCount = 0;

		if (m_pDBSvc->IsDBAlive(0))
		{
			int nCount = m_pDBSvc->GetDINCount();

			if (nCount > 0)
			{
				if ((nCount % MAX_AIN_COUNT))
				{
					nEstimateCount = nCount / MAX_AIN_COUNT + 1;
				}
				else
				{
					nEstimateCount = nCount / MAX_AIN_COUNT;
				}

				if (nEstimateCount == 1)
				{
					arrNums.push_back(std::make_pair(1, nCount));
				}
				else
				{
					for (int i = 0; i < nEstimateCount - 1; ++i)
					{
						arrNums.push_back(std::make_pair(1 + i*MAX_AIN_COUNT, MAX_AIN_COUNT));
					}
					//���һ֡
					arrNums.push_back(std::make_pair(1 + (nEstimateCount - 1)*MAX_AIN_COUNT, nCount%MAX_AIN_COUNT));
				}
			}
		}
	}

	void GetDIPackageNum(std::vector<std::pair<int32u, int32u > >& arrNums)
	{
		assert(m_pDBSvc);
		if (m_pDBSvc == nullptr)
			return ;

		size_t nEstimateCount = 0;

		if (m_pDBSvc->IsDBAlive(0))
		{
			int nCount = m_pDBSvc->GetDINCount();

			if (nCount>0)
			{
				if ((nCount % MAX_DIN_COUNT))
				{
					nEstimateCount = nCount / MAX_DIN_COUNT + 1;
				}
				else
				{
					nEstimateCount = nCount / MAX_DIN_COUNT;
				}

				if (nEstimateCount==1)
				{
					arrNums.push_back(std::make_pair(1,nCount));
				}
				else
				{
					for (int i = 0; i < nEstimateCount-1;++i)
					{
						arrNums.push_back(std::make_pair(1+i*MAX_DIN_COUNT,MAX_DIN_COUNT));
					}
					//���һ֡
					arrNums.push_back(std::make_pair(1 + (nEstimateCount - 1)*MAX_DIN_COUNT, nCount%MAX_DIN_COUNT));
				}
			}
		}		
	}


	size_t PackageAllDI(EMSG_BUF *pInBuff, std::pair<int32u, int32u> data, int32u& nSize)
	{
		assert(m_pDBSvc);
		if (m_pDBSvc == nullptr)
		{
			return 0;
		}

		Q_ASSERT(pInBuff );
		if (pInBuff == nullptr )
		{
			return 0;
		}

		int32u nOccNo = data.first;
		Q_ASSERT(nOccNo!=INVALID_OCCNO && nOccNo <= MAX_OCCNO);

		if (nOccNo ==INVALID_OCCNO || nOccNo >MAX_OCCNO)
		{
			return 0;
		}

		int32u nNumCount = data.second;
		Q_ASSERT(nNumCount <= MAX_DIN_COUNT);
		if (nNumCount >MAX_DIN_COUNT)
		{
			return 0;
		}

		int32u nNodeOccNO = m_pDBSvc->GetMyNodeOccNo();

		Q_ASSERT(nNodeOccNO != INVALID_OCCNO && nNodeOccNO < MAX_NODE_OCCNO);

		if (nNodeOccNO == INVALID_OCCNO || nNodeOccNO > MAX_NODE_OCCNO)
		{
			return 0;
		}

		if (m_pDBSvc->IsDBAlive(0))
		{
			pInBuff->MsgDataSize = sizeof EMSG_BUF_HEAD  + sizeof DATA_BASE + nNumCount * sizeof int8u;
			memset(pInBuff->BuffData, 0, sizeof DATA_BASE + nNumCount * sizeof int8u);
			
			DATA_BASE dataBase;
			dataBase.m_nCount = nNumCount;
			dataBase.m_nNodeOccNo = nNodeOccNO;
			dataBase.m_nStartOccNo = nOccNo;
			dataBase.m_nTransReason = COT_NORMAL_ASYNC;
			dataBase.m_nType = DIN_TYPE;
			dataBase.m_nExtraLen = nNumCount * sizeof int8u;

			memcpy(pInBuff->BuffData,&dataBase,sizeof DATA_BASE);
			
			std::vector<int8u> arrValues;

			for (int32u i = nOccNo; i < nOccNo + nNumCount; i++)
			{
				DIN* pFB = m_pDBSvc->GetDinByIndex(i-1);
				assert(pFB);

				if (!pFB)
				{
					continue;
				}
				arrValues.push_back(pFB->Value);
			}
			memcpy(&pInBuff->BuffData[sizeof DATA_BASE], arrValues.data(), sizeof int8u * arrValues.size());
			nSize = sizeof EMSG_BUF_HEAD + sizeof DATA_BASE + nNumCount * sizeof int8u;
		}
		
		return 0;
	}

	size_t PackageAllAI(EMSG_BUF *pInBuff, std::pair<int32u, int32u> data, int32u& nSize)
	{
		assert(m_pDBSvc);
		if (m_pDBSvc == nullptr)
		{
			return 0;
		}

		Q_ASSERT(pInBuff);
		if (pInBuff == nullptr)
		{
			return 0;
		}

		int32u nOccNo = data.first;
		Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);

		if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
		{
			return 0;
		}

		int32u nNumCount = data.second;
		Q_ASSERT(nNumCount <= MAX_AIN_COUNT);
		if (nNumCount > MAX_AIN_COUNT)
		{
			return 0;
		}

		int32u nNodeOccNO = m_pDBSvc->GetMyNodeOccNo();

		Q_ASSERT(nNodeOccNO != INVALID_OCCNO && nNodeOccNO < MAX_NODE_OCCNO);

		if (nNodeOccNO == INVALID_OCCNO || nNodeOccNO > MAX_NODE_OCCNO)
		{
			return 0;
		}

		if (m_pDBSvc->IsDBAlive(0))
		{
			pInBuff->MsgDataSize = sizeof EMSG_BUF_HEAD + sizeof DATA_BASE + nNumCount * sizeof FP64;
			memset(pInBuff->BuffData, 0, sizeof DATA_BASE + nNumCount * sizeof FP64);

			DATA_BASE dataBase;
			dataBase.m_nCount = nNumCount;
			dataBase.m_nNodeOccNo = nNodeOccNO;
			dataBase.m_nStartOccNo = nOccNo;
			dataBase.m_nTransReason = COT_NORMAL_ASYNC;
			dataBase.m_nType = AIN_TYPE;
			dataBase.m_nExtraLen = nNumCount * sizeof FP64;

			memcpy(pInBuff->BuffData, &dataBase, sizeof DATA_BASE);

			std::vector<fp64> arrValues;

			for (int32u i = nOccNo; i < nOccNo + nNumCount; i++)
			{
				AIN* pFB = m_pDBSvc->GetAinByIndex(i - 1);
				assert(pFB);

				if (!pFB)
				{
					continue;
				}
				arrValues.push_back(pFB->Value);
			}
			memcpy(&pInBuff->BuffData[sizeof DATA_BASE], arrValues.data(), sizeof fp64 * arrValues.size());
			nSize = sizeof EMSG_BUF_HEAD + sizeof DATA_BASE + nNumCount * sizeof FP64;
		}

		return 0;
	}


	size_t PackageAllAI(unsigned char *pInBuff, size_t nBuffLen)
	{
		assert(m_pDBSvc);
		if (m_pDBSvc == nullptr)
			return 0;

		size_t nAiCount = 0;

		int32u nNodeOccNO = m_pDBSvc->GetMyNodeOccNo();

		if (m_pDBSvc->IsDBAlive(0))
		{
			CAIN_DATA nAinData;

			size_t nCount = m_pDBSvc->GetAINCount();

			int i = 0;
			AIN *pFB = nullptr;

			int nOccNo = 1;

			for (i = 0; i < nCount; i++)
			{
				pFB = m_pDBSvc->GetAinByIndex(i);
				assert(pFB);

				if (!pFB)
				{
					continue;
				}

				//���ݰ�δ��,�������
				if (nAiCount >= 0 && nAiCount < CAIN_DATA::MAX_AIN_COUNT)
				{
					nAinData.m_arrAins[i] = pFB->Value;
					nAiCount++;
				}

				//���ݰ����򷢳�
				if (nAiCount >= CDIN_DATA::MAX_DIN_COUNT)
				{
					nAinData.SetAinCount(nCount);
					nAinData.SetDataType(DIN_TYPE);
					nAinData.SetStartOccNo(nOccNo);
					int nLen = NBSendData("FES", (int8u*)&nAinData, sizeof nAinData);
					nOccNo += nCount - 1;
					nCount = 0;
				}
			}

			if (nCount > 0)
			{
				nAinData.SetAinCount(nCount);
				nAinData.SetDataType(DIN_TYPE);
				nAinData.SetStartOccNo(nOccNo);
				int nLen = NBSendData("FES", (int8u*)&nAinData, sizeof nAinData);
			}
		}

		return 0;
	}


	size_t PackageAllDI(std::vector<CDIN_DATA*  >& arrDins)
	{
		assert(m_pDBSvc);
		if (m_pDBSvc == nullptr)
			return 0;

		size_t nDiCount = 1;

		int32u nNodeOccNO = m_pDBSvc->GetMyNodeOccNo();

		Q_ASSERT(nNodeOccNO != INVALID_OCCNO && nNodeOccNO < MAX_NODE_OCCNO);

		if (nNodeOccNO == INVALID_OCCNO || nNodeOccNO > MAX_NODE_OCCNO)
		{
			return 0;
		}
		CDIN_DATA* pDinData = nullptr ;

		if (m_pDBSvc->IsDBAlive(0))
		{
			size_t nCount = m_pDBSvc->GetDINCount();

			int i = 0;
			DIN *pFB = nullptr;

			int nOccNo = 1;

			pDinData = new CDIN_DATA;

			for (i = 0; i < nCount; i++)
			{
				pFB = m_pDBSvc->GetDinByIndex(i);
				assert(pFB);

				if (!pFB)
				{
					continue;
				}
				
				//���ݰ�δ��,�������
				if (nDiCount >= 0 && nDiCount < CDIN_DATA::MAX_DIN_COUNT)
				{
					pDinData->m_arrDins[i] = pFB->Value;
					nDiCount++;
				}

				//���ݰ����򷢳�
				if (nDiCount >= CDIN_DATA::MAX_DIN_COUNT)
				{
					pDinData->SetDinCount(nDiCount);
					pDinData->SetDataType(DIN_TYPE);
					pDinData->SetStartOccNo(nOccNo);
					
					arrDins.push_back(pDinData);

					nOccNo += nDiCount;
					nDiCount = 0;
					//һ�����˾��ڿ�һ��
					//nDinData = std::make_shared<CDIN_DATA>();
					pDinData = new CDIN_DATA;
				}
			}

			if (nCount > 0)
			{
				pDinData->SetDinCount(nDiCount);
				pDinData->SetDataType(DIN_TYPE);
				pDinData->SetStartOccNo(nOccNo);

				arrDins.push_back(pDinData);
			}
		}

		return 0;
	}

	size_t PackageAllAI(std::vector<CAIN_DATA*>& arrAins)
	{
		assert(m_pDBSvc);
		if (m_pDBSvc == nullptr)
			return 0;

		size_t nAiCount = 1;

		int32u nNodeOccNO = m_pDBSvc->GetMyNodeOccNo();

		Q_ASSERT(nNodeOccNO != INVALID_OCCNO && nNodeOccNO < MAX_NODE_OCCNO);

		if (nNodeOccNO == INVALID_OCCNO || nNodeOccNO > MAX_NODE_OCCNO)
		{
			return 0;
		}

		CAIN_DATA* pAinData = nullptr;

		if (m_pDBSvc->IsDBAlive(0))
		{
	
			pAinData = new CAIN_DATA;

			size_t nCount = m_pDBSvc->GetAINCount();

			int i = 0;
			AIN *pFB = nullptr;

			int nOccNo = 1;

			for (i = 0; i < nCount; i++)
			{
				pFB = m_pDBSvc->GetAinByIndex(i);
				assert(pFB);

				if (!pFB)
				{
					continue;
				}

				//���ݰ�δ��,�������
				if (nAiCount >= 0 && nAiCount < CAIN_DATA::MAX_AIN_COUNT)
				{
					pAinData->m_arrAins[i] = pFB->Value;
					nAiCount++;
				}

				//���ݰ����򷢳�
				if (nAiCount >= CDIN_DATA::MAX_DIN_COUNT)
				{
					pAinData->SetAinCount(nAiCount);
					pAinData->SetDataType(AIN_TYPE);
					pAinData->SetStartOccNo(nOccNo);
					
					arrAins.push_back(pAinData);

					nOccNo += nCount ;
					nCount = 0;

					pAinData = new CAIN_DATA;
				}
			}

			if (nCount > 0)
			{
				pAinData->SetAinCount(nCount);
				pAinData->SetDataType(AIN_TYPE);
				pAinData->SetStartOccNo(nOccNo);
				arrAins.push_back(pAinData);
			}
		}
		return 0;
	}
private:
	std::shared_ptr<DBSvc> &   m_pDBSvc;

};


#endif // RT_NET_MSG_PACKER_H

/** @}*/

