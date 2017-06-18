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
	QString m_strDesc;       //描述
	char m_szTagName[MAX_NAME_LENGTH];
	//QString m_szTagName;     //TagName
	int32u  m_nBlockOccNo;   //内部编号
	int32u  m_nOccNo;        //大排行号

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
	QString m_strDesc;       //描述
	char m_szTagName[MAX_NAME_LENGTH];
	//QString m_szTagName;     //TagName
	int32u  m_nOccNo;        //大排行号
	int32u  m_nBlockOccNo;   //内部编号

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
	QString m_strDesc;        //描述
	char m_szTagName[MAX_NAME_LENGTH];
	//QString m_szTagName;      //TagName
	int32u  m_nOccNo;         //大排行号
	double  m_dValue;
	int32u  m_nBlockOccNo;    //内部编号

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

	std::vector <CAiInput* >    m_vecAis;                //遥测Ai vec
	std::vector <CDiInput* >    m_vecDis;                //遥信Di vec
	std::vector <CParamInput* > m_vecParams;             //参数Param vec

	//QMap<QString, CAiInput* >   m_mapAiInfo;
	//QMap<QString, CDiInput* >   m_mapDiInfo;
	//QMap<QString, CPredictDevice* > m_mapDeviceInfo;


public:
	int32u  m_nID;                          //ID
	QString m_strDesc;                      //装置Describe
	int8u   m_nDeviceType;                  //装置Type
	QString m_strDeviceAddr;                //装置Address
	//int     m_nDeviceModelId;               //装置模板ID
	char    m_szTagName[MAX_NAME_LENGTH];   //TagName

private:
	int32u m_nOccNo;                       //大排行号OccNo
	IMainModuleInterface *m_pCore;         //模块指针
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
	QString  m_strDesc;                       //描述
	QString  m_strItemName;
	char     m_strTagName[MAX_NAME_LENGTH];
	

	//厂站下所有装置指针向量
	QVector<CPredictDevice* > m_pPredictDevices;

	QVector<CItemDataInfo> m_vecTableInfo;

	//厂站下,所有装置名、装置指针map
	QMap<QString, CPredictDevice*> m_mapDevice;

	//std::map<QString, CPredictDevice*>m_mapDevice;

	//QMap<QString, CPredictDevice*> m_mapDeviceInfo;

	//
	QMap<QString, CPredictItem*> m_mapItem;

private:

	int32u m_nOccNo;       //大排行号

};


class CPredictGroup
{
public:
	CPredictGroup();

	~CPredictGroup();

public:

	//创建组,分配内存
	CPredictItem* CreateItem();

	//删除item,释放内存
	bool DeleteItem(const QString& strTagGroup);

	//修改item
	bool ModifyItem(const QString& strTagName, const QString& strLastItemName);

	//获取item名
	QString GetItemName(const QString& strItemName);

	//创建组,分配内存
	CPredictGroup* CreateGroup();

	//删除组,释放内存
	bool DeleteGroup(CPredictGroup* pPrdtGroup, const QString& strLastTagName);

	//修改组
	bool ModifyGroup(const QString& strTagName, const QString& strLastName);

	//获取组名
	QString GetGroupName(const QString& strGroupName);

	bool IsPredictGroup(const QString& strGroupNameKey, CPredictGroup* pPredictItem);

	bool IsPredictItem(const QString& strGroupNameKey, const CPredictItem* pItem);


public:

	//组名
	QString m_strGroupName;

	QMap<QString, CPredictItem*>  m_mapItemInfo;
	
	//主组下建子组,存放新建子组,根节点下新建组,即存入该map
	QMap<QString, CPredictGroup*>  m_mapGroups;

};


class CPredictRootItem
{

public:
	CPredictRootItem();

	~CPredictRootItem();

public:

	//预测保存类型
	enum PREDICT_SAVE_TYPE
	{
		//本地文件
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
	
	//新建预测项
	CPredictItem* AddNewPredictItem(CPredictGroup* pPredictGroup);

public:
	//预测根节点名
	QString m_strRootItem;

	//根节点本身为组,组号为0
	CPredictGroup* m_pPredictGroup;

	//预测项vector
	QVector<CPredictItem*> m_vecPredictItemDatas;

	//所有预测项map,不论在根组或子组下添加item都存入该map
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