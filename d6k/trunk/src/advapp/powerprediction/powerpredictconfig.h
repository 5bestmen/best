#ifndef POWER_PREDICT_CONFIG_H
#define POWER_PREDICT_CONFIG_H


#include <vector>
#include <QMap>
#include <QString>
#include <unordered_map>
#include <QVector>

#include "define.h"
#include "../../scadastudio/base_data.h"
#include "powerpredictdefine.h"
#include "scadastudio/icore.h"
#include "datatypes.h"


namespace PPConfig
{
	class CPowerPredictData;
	class CDevice;
	class CPointInfo;

	class CPowerPredictGroup
	{
	public:
		CPowerPredictGroup();
		~CPowerPredictGroup();

		bool CheckModifyGroupNameExist(CPowerPredictGroup *pPowerPredictGroup, QString strGroupName);

		//创建组
		CPowerPredictGroup* CreateGroup();

		//修改组
		bool ModifyGroup(const QString &strTagName, const QString & strLastTagName);

		//删除组
		bool DeleteGroup(CPowerPredictGroup *pPowerPredictGroup, const QString &strTagGroup);

		bool DeleteItem(CPowerPredictData *pPowerPredict, const QString &strTagGroup);
		
		//组名
		QString m_strGroup;

		std::unordered_map<std::string, CPowerPredictGroup * > m_arrPowerPredictGroup;
		std::unordered_map<std::string, CPowerPredictData * > m_arrPowerPredictItem;
		
	};


	class CBaseData
	{
	public:
		CBaseData();
		virtual ~CBaseData();
	public:
		virtual void Save() = 0;

		virtual void SetModifyFlag(bool bFlag) = 0;

		virtual bool GetModifyFlag() const = 0;


	protected:
		bool m_bModifyFlag;
	};


	class CPowerPredictData : public CBaseData
	{
	public:
		CPowerPredictData(IMainModuleInterface *pCore);
		~CPowerPredictData();

	public:

		bool PushPowerPredictTagNameHashMap(int nIDD, const QString &strTagName, CBaseData *pData);

	public:

		virtual void Save();
		virtual void SetModifyFlag(bool bFlag)
		{
			 m_bModifyFlag = bFlag;
		}

		virtual bool GetModifyFlag() const
		{
			return m_bModifyFlag;
		}

	public:


		using TagNameMap = std::unordered_map<std::string, CBaseData *>;

		TagNameMap m_arrMaps[POWERPREDICTHASHTYPENUM::MAX_IDD_NUM];

		uint m_nID;

		QString m_strDescription;

		QString m_strGroupName;

		char m_szTagName[MAX_NAME_LENGTH + STRING_PAD];

		//装置组
		CDevice *m_pDeviceGroup;

	private:

		IMainModuleInterface *m_pCore;

		bool  m_bModifyFlag;
	};


	/*class CPowerPredictConfigData
	{
	public:
		CPowerPredictConfigData();
		~CPowerPredictConfigData();


	public:
		bool CheckModifyPowerPredictTagNameExist(const QString &strTagName);

		CPowerPredictData *AddNewPowerPredictData(CPowerPredictGroup *pPowerPredictGroup);

		std::unordered_map<std::string, CPowerPredictData * > &GetPowerPredictArr()
		{
			return m_arrHashTagNameMap;
		}

		bool DeleteHashPowerPredictItem(std::string &szPowerPredictTag);


	public:


		std::vector<CPowerPredictData *> m_arrPowerPredictDatas;

		CPowerPredictGroup *m_pPowerPredictGroup;

		std::unordered_map<std::string, CPowerPredictData *> m_arrHashTagNameMap;

	};*/


	/*class CDevice
	{
	public:
		CDevice();
		~CDevice();

	};*/


/*
	class CPowerPredictGroupMgr
	{
	public:
		CPowerPredictGroupMgr()
		{
			m_pMgrGrps = nullptr;
		}

		~CPowerPredictGroupMgr()
		{

		}

	public:
		QString m_strGroupName;          //组名

		CPowerPredictGroup *m_pMgrGrps;  //预测组信息
		
	};
*/


};


/*
class CAiInput
{
public:

	CAiInput();
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
	int32u m_nID;
	QString m_strDesc;
	QString m_szTagName;
	int32u m_nOccNo;

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
	int32u m_nID;
	QString m_strDesc;
	QString m_szTagName;
	int32u m_nOccNo;

private:

};



class CPredictDevice
{
	
public:
	CPredictDevice(IMainModuleInterface* pCore);
	~CPredictDevice();

public:
	char m_szTagName[512];
	QString m_strDesc;
	int32u m_nID;


public:

	std::vector <CAiInput *> m_vecAis;
	std::vector <CDiInput *> m_vecDis;


};*/

#endif  //POWER_PREDICT_CONFIG_H