#ifndef VARIABLE_DATA_H
#define VARIABLE_DATA_H

#include <QObject>

#include "datatypes.h"
#include "scadastudio/define.h"
#include "scadastudiodefine.h"
#include "base_data.h"

namespace Config
{
	class CSystemVariable : public CBaseData
	{
	public:
		explicit CSystemVariable();
		~CSystemVariable();

		virtual void Save()
		{

		}

		virtual void SetModifyFlag(bool bFlag)
		{
			m_bModifyFlag = bFlag;
		}

		virtual bool GetModifyFlag()const
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
		char m_szTagName[MAX_TAGNAME_LEN_SCADASTUDIO + STRING_PAD];
		QString m_strDescription;	//����
		int32u m_nID;
		int32u m_nType;			//��������

	private:
		int32u m_nOccNo;
	};


	class CUserVariable : public CBaseData
	{
	public:
		CUserVariable();

		~CUserVariable()
		{

		}

		virtual void Save()
		{

		}

		virtual void SetModifyFlag(bool bFlag)
		{
			m_bModifyFlag = bFlag;
		}

		virtual bool GetModifyFlag()const
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
		char m_szTagName[MAX_TAGNAME_LEN_SCADASTUDIO + STRING_PAD];
		QString m_strDescription;	//����
		int32u m_nID;
		int32u m_nType;			//��������

		fp64 m_dbInitValue;
		int8u m_nInitialQua;

		QString m_strAddress;
		QString m_strPinLabel;

		QString m_strSourceTagName;
		QString m_strSourceOccNo;
		//ǰ�á�������� ��������ģ��ʹ�� ��ģ���ж�ʹ��
		QString m_strFesTagName;
		QString m_strDataTagName;
		QString m_strAttrTagName;

		//����ת��
		QString m_strScaleTagName;
		//QString m_strScaleDesc;
		int8u m_nScaleType;
		int32u m_nScaleOccNo;
		
		//�����澯
		QString m_strAlarmTagName;
		//QString m_strAlarmDesc;
		int8u m_nAlarmType;
		int32u m_nAlarmOccNo;
		
		bool m_bNewScalePoint;
	private:
		int32u m_nOccNo;
	};


	class CUserVariableGroup : public CBaseData
	{
	public:
		CUserVariableGroup();
		~CUserVariableGroup();

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

		void SetGroupName(QString &strGroupName)
		{
			m_strGroupName = strGroupName;
		}

		bool AddUserVariableGroup(CUserVariableGroup *pGroup);
		bool AddUserVariableItem(CUserVariable *pItem);

		std::vector<CUserVariableGroup *> &GetGroup()
		{
			return m_arrGroup;
		}

		QString GetGroupName()
		{
			return m_strGroupName;
		}

		virtual int32u GetOccNo() const
		{
			return 0;
		}

		std::vector<CUserVariable *> &GetItem()
		{
			return m_arrItem;
		}

		bool CheckModifyGroupNameExist(CUserVariableGroup *pGroup, QString &strTagName);

		CUserVariableGroup *CreateGroup();

		bool CheckGroupNameExist(QString &strTagName);
		bool DeleteGroup(CUserVariableGroup *pChildGroup);
		bool DeleteItem(CUserVariable *pItem);
	public:
		//���������
		std::vector<CUserVariableGroup *> m_arrGroup;
		std::vector<CUserVariable *> m_arrItem;

		QString m_strGroupName;
	};
}

#endif // VARIABLE_DATA_H


