#ifndef  PREDICT_DATA_INFO_H
#define  PREDICT_DATA_INFO_H

#include <QString>
#include <QVector>
#include <QMap>
#include <vector>
#include <QDomDocument>
#include <QDomElement>
#include <unordered_map>
#include <QXmlStreamWriter>
#include <QFile>
#include <QXmlStreamReader>

#include "datatypes.h"
#include "powerpredictdefine.h"
#include "scadastudio/imodule.h"
#include "define.h"

class IMainModuleInterface;
class CPredictItem;

class CItemDataInfo
{
public:
	CItemDataInfo();

	~CItemDataInfo();

public:
	int32u  m_ID;          
	QString m_strTagName;
	QString m_strDesc;
	int32u  m_OccNo;

};

class CAiInput
{
public:

	CAiInput();

	explicit CAiInput(int32u nOccNo)
	{
		memset(m_szTagName, 0, MAX_NAME_LENGTH);
		Q_ASSERT(nOccNo >= MIN_OCCNO_PREDICTION && nOccNo <= MAX_OCCNO_PREDICTION);
		m_nOccNo = nOccNo;
	}

	~CAiInput();

	int32u GetOccNo() const
	{
		return m_nOccNo;
	}

	void SetOccNo(int32u nOccNo)
	{
		m_nOccNo = nOccNo;
	}

public:
	int32u  m_nID;           //ID
	QString m_strDesc;       //����
	char m_szTagName[MAX_NAME_LENGTH];
	//QString m_szTagName;     //TagName
	int32u  m_nBlockOccNo;   //�ڲ����
	int32u  m_nOccNo;        //�����к�

private:
	
};

class CDiInput
{
public:
	CDiInput();

	~CDiInput();

	int32u GetOccNo() const
	{
		return m_nOccNo;
	}

	void SetOccNo(int32u nOccNo)
	{
		m_nOccNo = nOccNo;
	}

public:
	int32u  m_nID;           //ID
	QString m_strDesc;       //����
	char m_szTagName[MAX_NAME_LENGTH];
	//QString m_szTagName;     //TagName
	int32u  m_nOccNo;        //�����к�
	int32u  m_nBlockOccNo;   //�ڲ����

private:

};

class CParamInput
{
public:
	CParamInput();

	~CParamInput();

	int32u GetOccNo() const
	{
		return m_nOccNo;
	}

	void SetOccNo(int32u nOccNo)
	{
		m_nOccNo = nOccNo;
	}

public:
	int32u  m_nID;            //ID
	QString m_strDesc;        //����
	char m_szTagName[MAX_NAME_LENGTH];
	//QString m_szTagName;      //TagName
	int32u  m_nOccNo;         //�����к�
	double  m_dValue;
	int32u  m_nBlockOccNo;    //�ڲ����

private:


};


class CPredictDevice
{
public:
	CPredictDevice(IMainModuleInterface* pCore);
	
	~CPredictDevice();

	int32u GetOccNo() const
	{
		return m_nOccNo;
	}

	void SetOccNo(int32u nOccNo)
	{
		m_nOccNo = nOccNo;
	}

public:
	void AddAiInput(CAiInput* pAnalogInput);

	void AddDiInput(CDiInput* pDigitalInput);

	void AddParamInput(CParamInput* pParamInput);

	bool ReadAINode(QXmlStreamReader& reader, CPredictItem* pPrdtItem);

	bool ReadDINode(QXmlStreamReader& reader, CPredictItem* pPrdtItem);

	bool ReadParamNode(QXmlStreamReader& reader, CPredictItem* pPrdtItem);

	bool ReadAiValue(QXmlStreamReader& reader, CPredictItem* pPrdtItem);

	bool ReadDiValue(QXmlStreamReader& reader, CPredictItem* pPrdtItem);

	bool ReadParamValue(QXmlStreamReader& reader, CPredictItem* pPrdtItem);

	bool LoadData(QXmlStreamReader& reader, CPredictItem* pPrdtItem);

	bool SavePredictAI(QXmlStreamWriter& writer, int &nOccNoDevice, int &nAIOccNo);

	bool SavePredictDI(QXmlStreamWriter& writer, int &nOccNoDevice, int &nDIOccNo);

	bool SavePredictParam(QXmlStreamWriter& writer, int &nOccNoDevice, int &nParamOccNo);

	bool SavePredictDevice(QXmlStreamWriter& writer, int &nOccNoDevice, int &nAIOccNo, int &nDIOccNo, int &nParamOccNo);
	
public:

	std::vector <CAiInput* >    m_vecAis;                //ң��Ai vec
	std::vector <CDiInput* >    m_vecDis;                //ң��Di vec
	std::vector <CParamInput* > m_vecParams;             //����Param vec

	//QMap<QString, CAiInput* >   m_mapAiInfo;
	//QMap<QString, CDiInput* >   m_mapDiInfo;
	//QMap<QString, CPredictDevice* > m_mapDeviceInfo;


public:
	int32u  m_nID;                          //ID
	QString m_strDesc;                      //װ��Describe
	int8u   m_nDeviceType;                  //װ��Type
	QString m_strDeviceAddr;                //װ��Address
	//int     m_nDeviceModelId;               //װ��ģ��ID
	char    m_szTagName[MAX_NAME_LENGTH];   //TagName

private:
	int32u m_nOccNo;                       //�����к�OccNo
	IMainModuleInterface *m_pCore;         //ģ��ָ��
};


class CPredictItem
{
public:

	CPredictItem();

	~CPredictItem();

public:

	QString GetDeviceName(const QString& strDeviceName, CPredictDevice* pPrdtDevice);
	
	QString GetItemName(const QString& strItemName);
	
	bool IsPredictItem(const QString& strItemNameKey, const CPredictItem* pItem);
	
	bool IsPredictDevice(const QString& strDeviceNameKey, const CPredictDevice* pItem);

	bool SavePredictItemData(const QString& szRoot, int nOccNo);
	
	bool SaveChildItemData(QXmlStreamWriter& writer, QVector<CPredictDevice* > pPredictDevice);

	bool LoadPredictItemData(QXmlStreamReader &reader);
	
	void AddPredictDevice(CPredictDevice* pDevice);

	bool DeletePredictDevice(CPredictItem* pPrdtItem, const QString& strLastTagName);

	bool LoadDeviceData(QXmlStreamReader& reader, CPredictItem* pPrdtItem);

public:

	uint     m_nID;
	QString  m_strDesc;                       //����
	QString  m_strItemName;
	char     m_strTagName[MAX_NAME_LENGTH];
	

	//��վ������װ��ָ������
	QVector<CPredictDevice* > m_pPredictDevices;

	QVector<CItemDataInfo> m_vecTableInfo;

	//��վ��,����װ������װ��ָ��map
	QMap<QString, CPredictDevice*> m_mapDevice;

	//std::map<QString, CPredictDevice*>m_mapDevice;

	//QMap<QString, CPredictDevice*> m_mapDeviceInfo;

	//
	QMap<QString, CPredictItem*> m_mapItem;

private:

	int32u m_nOccNo;       //�����к�

};


class CPredictGroup
{
public:
	CPredictGroup();

	~CPredictGroup();

public:

	//������,�����ڴ�
	CPredictItem* CreateItem();

	//ɾ��item,�ͷ��ڴ�
	bool DeleteItem(const QString& strTagGroup);

	//�޸�item
	bool ModifyItem(const QString& strTagName, const QString& strLastItemName);

	//��ȡitem��
	QString GetItemName(const QString& strItemName);

	//������,�����ڴ�
	CPredictGroup* CreateGroup();

	//ɾ����,�ͷ��ڴ�
	bool DeleteGroup(CPredictGroup* pPrdtGroup, const QString& strLastTagName);

	//�޸���
	bool ModifyGroup(const QString& strTagName, const QString& strLastName);

	//��ȡ����
	QString GetGroupName(const QString& strGroupName);

	bool IsPredictGroup(const QString& strGroupNameKey, CPredictGroup* pPredictItem);

	bool IsPredictItem(const QString& strGroupNameKey, const CPredictItem* pItem);


public:

	//����
	QString m_strGroupName;

	QMap<QString, CPredictItem*>  m_mapItemInfo;
	
	//�����½�����,����½�����,���ڵ����½���,�������map
	QMap<QString, CPredictGroup*>  m_mapGroups;

};


class CPredictRootItem
{

public:
	CPredictRootItem();

	~CPredictRootItem();

public:

	//Ԥ�Ᵽ������
	enum PREDICT_SAVE_TYPE
	{
		//�����ļ�
		PREDICT_LOCAL_FILE, 
		//Mysql
		PREDITC_MYSQL,
		 
	};

	QString GetRootItemName(const QString& strRootItemName);
	
	bool IsPredictItem(const QString& strItemName, const CPredictItem* pItem);

	bool SaveProjectPredictNode(QDomDocument* pXml, QDomElement& pElement, const QString& szRoot);

	bool SaveChildPredictNode(QDomDocument* pXml, QDomElement& pElement, CPredictGroup* pPredictGroup);

	bool SaveChildItem(CPredictGroup* pPredictGroup, const QString& szRoot, int nOccNo);

	bool LoadProjectPredictNode(QDomElement& pElement, const QString& szRoot);

	bool LoadPredictChildGroup(CPredictGroup* pPredictGrp, QDomElement& pElement, const QString& szRoot);

	bool LoadPredictChildItem(CPredictItem* pPredictItem, const QString& szRoot);

	bool LoadPredictFile(CPredictItem* pPredictItem, const QString& filePath);

	void Clear();

	void Create();

	bool DeleteHashPredictItem(QString& strPredictTag);

	bool DeleteItem();
	
	//�½�Ԥ����
	CPredictItem* AddNewPredictItem(CPredictGroup* pPredictGroup);

public:
	//Ԥ����ڵ���
	QString m_strRootItem;

	//���ڵ㱾��Ϊ��,���Ϊ0
	CPredictGroup* m_pPredictGroup;

	//Ԥ����vector
	QVector<CPredictItem*> m_vecPredictItemDatas;

	//����Ԥ����map,�����ڸ�������������item�������map
	QMap<QString, CPredictItem*> m_mapPredictItem;

	QMap<QString, CPredictGroup*> m_mapPredictGroup;

	QMap<QString, CPredictRootItem*> m_mapRootItemInfo;

	std::vector<CPredictGroup* > m_arrPredictGroup;
};


class CPredictMgr
{
public:
	CPredictMgr()
	{
		m_pMgrGroups = nullptr;
		m_pMgrRootItem = nullptr;
	}
	~CPredictMgr() 
	{
		if (m_pMgrGroups != nullptr)
		{
			delete m_pMgrGroups;
			m_pMgrGroups = nullptr;
		}

		if (m_pMgrRootItem != nullptr)
		{
			delete m_pMgrRootItem;
			m_pMgrRootItem = nullptr;
		}
	}

public:
	QString m_strGroupName;
	CPredictGroup* m_pMgrGroups;
	CPredictRootItem* m_pMgrRootItem;
};

#endif  //PREDICT_DATA_INFO_H