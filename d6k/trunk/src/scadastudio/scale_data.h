#ifndef SCALE_CONFIG_DATA_H
#define SCALE_CONFIG_DATA_H

#include "datatypes.h"
#include "scadastudio/define.h"
#include "scadastudiodefine.h"
#include "base_data.h"

#include <unordered_map>



namespace Config
{ 
	struct NONLINEAR_SCALE
	{
		fp64 RawValue;
		fp64 ScaleValue;
	};


	class CLinearTransform :public CBaseData
	{
	public:
		CLinearTransform();
		~CLinearTransform();

		virtual void Save();

		virtual void SetModifyFlag(bool bFlag)
		{
			m_bModifyFlag = bFlag;
		}

		virtual bool GetModifyFlag() const
		{
			return m_bModifyFlag;
		}

		int32u GetOccNo() const
		{
			return m_nOccNo;
		}

		void SetOccNo(int32u nOccNo)
		{
			m_nOccNo = nOccNo;
		}

	public:
		//uint m_nOccNo;				//大排行号
		char m_szTagName[MAX_TAGNAME_LEN_SCADASTUDIO + STRING_PAD];		//主键
		QString m_strDescription;	//描述

		uint m_nID;

		QString m_strGroupName;		//分组名称
		QString m_strExtention;		//扩展参数

		bool m_bEnableInverse;	//是否采用反比
		fp64 m_dbMaxRaw;			//最大原始值
		fp64 m_dbMinRaw;			//最小原始值
		fp64 m_dbMaxScale;			//最大值
		fp64 m_dbMinScale;			//最小值
		//fp64 m_dbInverse;
		bool m_bEnableFactor;		//启用因子
		bool m_bInverseFactor;		//取反因子



		fp64 m_dbGain;				//增益	
		fp64 m_dbOffset;			//偏置

		int m_nType;		//类型
	private:
		int32u m_nOccNo;				//大排行号
	};

	class CNonlinearTransform :public CBaseData
	{
	public:
		CNonlinearTransform();
		~CNonlinearTransform();

		virtual void Save();

		virtual void SetModifyFlag(bool bFlag)
		{
			m_bModifyFlag = bFlag;
		}

		virtual bool GetModifyFlag() const
		{
			return m_bModifyFlag;
		}

		int32u GetOccNo() const
		{
			return m_nOccNo;
		}

		void SetOccNo(int32u nOccNo)
		{
			m_nOccNo = nOccNo;
		}
	public:
		//uint m_nOccNo;				//大排行号
		char m_szTagName[MAX_TAGNAME_LEN_SCADASTUDIO + STRING_PAD];		//主键
		QString m_strDescription;	//描述

		uint m_nID;

		QString m_strGroupName;		//分组名称
		QString m_strExtention;		//扩展参数

		int m_nType;		//类型

		std::vector<NONLINEAR_SCALE *> m_arrPNonliear;

	private:
		void Clear();
	private:
		int32u m_nOccNo;				//大排行号
	};

	//转换
	class CTransform :public CBaseData
	{
	public:
		CTransform()
			:m_pLinearScale(NULL), m_pNonLinearScale(NULL)
		{
			memset(m_szTagName, 0, sizeof(m_szTagName));

			m_nOccNo = 0;
		}
		~CTransform()
		{
			if (m_pLinearScale)
			{
				delete m_pLinearScale;
				m_pLinearScale = nullptr;
			}

			if (m_pNonLinearScale)
			{
				delete m_pNonLinearScale;
				m_pNonLinearScale = nullptr;
			}
		}

		virtual void Save();

		virtual void SetModifyFlag(bool bFlag)
		{
			m_bModifyFlag = bFlag;
		}

		virtual bool GetModifyFlag() const
		{
			return m_bModifyFlag;
		}

		CLinearTransform *GetLinear()
		{
			if (!m_pLinearScale)
			{
				m_pLinearScale = new CLinearTransform;
			}

			Q_ASSERT(m_pLinearScale);

			return m_pLinearScale;
		}

		void SetLinear(CLinearTransform *pLinear)
		{
			m_pLinearScale = pLinear;

			Q_ASSERT(pLinear);
		}

		Config::CNonlinearTransform *GetNonLinear()
		{
			if (!m_pNonLinearScale)
			{
				m_pNonLinearScale = new CNonlinearTransform;
			}

			Q_ASSERT(m_pNonLinearScale);

			return m_pNonLinearScale;
		}

		void SetNonLinear(CNonlinearTransform *pNonLinear)
		{
			m_pNonLinearScale = pNonLinear;

			Q_ASSERT(pNonLinear);
		}

		void GetType(int &nType)
		{
			nType = m_nType;
		}

		bool CopyLinearToNon()
		{
			//线性转换为非线性
			if (m_pLinearScale == NULL && m_pNonLinearScale == NULL)
			{
				return false;
			}

			if (m_pLinearScale == NULL)
			{
				m_pNonLinearScale = new CNonlinearTransform;
			}

			memset(m_pNonLinearScale->m_szTagName, 0, sizeof(m_pNonLinearScale->m_szTagName));
			int nSize = sizeof(m_pLinearScale->m_szTagName);
			strncpy(m_pNonLinearScale->m_szTagName, m_pLinearScale->m_szTagName, qMin(MAX_NAME_LENGTH_SCADASTUDIO, nSize));

			m_pNonLinearScale->m_strDescription = m_pLinearScale->m_strDescription;
			m_pNonLinearScale->m_strGroupName = m_pNonLinearScale->m_strGroupName;

			return true;
		}

		bool CopyNonToLinear()
		{
			//非线性转换为线性
			if (m_pLinearScale == NULL && m_pNonLinearScale == NULL)
			{
				return false;
			}

			if (m_pLinearScale == NULL)
			{
				m_pLinearScale = new CLinearTransform;
			}

			memset(m_pLinearScale->m_szTagName, 0, sizeof(m_pLinearScale->m_szTagName));
			strncpy(m_pLinearScale->m_szTagName, m_pLinearScale->m_szTagName, qMin(MAX_NAME_LENGTH_SCADASTUDIO, (int)sizeof(m_pLinearScale->m_szTagName)));
			m_pLinearScale->m_strDescription = m_pNonLinearScale->m_strDescription;
			m_pNonLinearScale->m_strGroupName = m_pNonLinearScale->m_strGroupName;

			return true;
		}

		int32u GetOccNo() const
		{
			return m_nOccNo;
		}

		void SetOccNo(int32u nOccNo)
		{
			m_nOccNo = nOccNo;
		}

	public:
		//int32u m_nOccNo;				//大排行号
		char m_szTagName[MAX_TAGNAME_LEN_SCADASTUDIO + STRING_PAD];		//主键
		//QString m_strScaleName;	//描述

		//uint m_nID;

		//QString m_strGroupName;		//分组名称
		//QString m_strExtention;		//扩展参数

		int m_nType;		//类型

		//LINEAR_SCALE m_LinearScale;

		//std::vector<NONLINEAR_SCALE *> m_arrPNonliear;

		CLinearTransform *m_pLinearScale;			//线性

		CNonlinearTransform *m_pNonLinearScale;		//非线性

	private:
		int32u m_nOccNo;				//大排行号
	};


	class CTransformGroup : public CBaseData
	{
	public:
		CTransformGroup();
		~CTransformGroup();

		virtual void Save()
		{

		}

		virtual void SetModifyFlag(bool bFlag)
		{
			Q_UNUSED(bFlag);
		}

		virtual bool GetModifyFlag() const
		{
			return true;
		}

		virtual int32u GetOccNo() const
		{
			return 0;
		}


		CTransformGroup *Find(QString &strName);

		CTransformGroup *CreateGroup();

		bool AddTransformGroup(CTransformGroup *pGroup);
		bool AddTransformItem(CTransform *pItem);

		bool SetGroupName(QString &strGroupName)
		{
			Q_ASSERT(!strGroupName.isEmpty());
			if (strGroupName.isEmpty())
			{
				return false;
			}

			m_strGroupName = strGroupName;

			return true;
		}

		QString &GetGroupName()
		{
			return m_strGroupName;
		}

		std::vector<CTransformGroup *> &GetGroup()
		{
			return m_arrTransformGroup;
		}

		std::vector<CTransform *> &GetItem()
		{
			return m_arrLinearTransformItem;
		}


		int32u GetItemCount();

		bool CheckModifyGroupNameExist(CTransformGroup *pGroup, QString strGroupName);
		bool DeletGroup(CTransformGroup *pChildGroup);
		bool DeleteItem(CTransform *pChildItem);
	private:
		//组包含的组
		std::vector<CTransformGroup *> m_arrTransformGroup;
		std::vector<CTransform *> m_arrLinearTransformItem;

		QString m_strGroupName;
	};
}




#endif