/*! @file
<PRE>
********************************************************************************
ģ����       :  ���ýṹ
�ļ���       :  base_data.cpp
�ļ�ʵ�ֹ��� :  �������ýṹ
����         :  gw
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         : <����˵��>
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

********************************************************************************
</PRE>
*  @brief
*  @author gw
*  @version 1.0
*  @date	2016.10.31
*/

#include "base_data.h"
#include "scadastudio/icore.h"
#include "scadastudiodefine.h"
#include "scadastudio/define.h"
#include "log/log2file.h"

#include <string>
#include <QString>
#include <QObject>

extern IMainModuleInterface *s_pGlobleCore;

namespace Config
{
	/*! \fn CBaseData::CBaseData()
	*********************************************************************************************************
	** \brief CBaseData::CBaseData
	** \details
	** \return
	** \author gw
	** \date 2016��1��4��
	** \note
	********************************************************************************************************/
	CBaseData::CBaseData()
		:m_bModifyFlag(false)
	{

	}

	/*! \fn CBaseData::~CBaseData()
	*********************************************************************************************************
	** \brief CBaseData::~CBaseData
	** \details
	** \return
	** \author gw
	** \date 2016��1��4��
	** \note
	********************************************************************************************************/
	CBaseData::~CBaseData()
	{

	}


	bool CBaseData::LogString(char *pszMoudleName, char *szLogTxt, int nLevel)
	{
		extern bool LogMsg(char *pszMoudleName, char *szLogTxt, int nLevel);

		return LogMsg(pszMoudleName, szLogTxt, nLevel);
	}


	/*! \fn bool CBaseData::CheckTagName(std::string & szTagName) const
	*********************************************************************************************************
	** \brief Config::CBaseData::CheckTagName
	** \details ����������ֽ���У����
	** \param szTagName
	** \return bool
	** \author LiJin
	** \date 2016��8��9��
	** \note
	********************************************************************************************************/

	bool CBaseData::CheckTagName(std::string & szTagName) const
	{
		if (szTagName.length() == 0 || szTagName.length() > MAX_NAME_LENGTH_SCADASTUDIO)
		{
			return false;
		}

		// ���ַ������е�����飬�����Ƿ��зǷ��ַ���
		std::string str("Test string");

		auto bFlag = true;

		QString strError;
		for (std::string::iterator it = str.begin(); it != str.end(); ++it)
		{
			auto &check = (*it);

			if (check == ' ' || check == '~' || check == '@' || check == '#' || check == '$' ||
				check == '%' || check == '^' || check == '&' || check == '*' || check == '(' ||
				check == ')' || check == '_' || check == '+' || check == '=' || check == '-' ||
				check == '.' || check == '<' || check == '>' || check == '?' || check == '/' ||
				check == '\\' || check == '|')
			{
				strError = QString("Tag name %1 contais invalid char %2").arg(szTagName.c_str()).arg(check);

				s_pGlobleCore->LogMsg(FES_DESC, (char *)strError.toStdString().c_str(), LEVEL_1);

				bFlag = false;
			}
		}

		return bFlag;
	}

	/*! \fn 
	********************************************************************************************************* 
	** \brief Config::CBaseData::LogToFile 
	** \details ��־log
	** \param pFilename 
	** \param pLog 
	** \return bool 
	** \author xingzhibing
	** \date 2017��6��23�� 
	** \note 
	********************************************************************************************************/
	bool CBaseData::LogToFile(const char* pFilename, const char* pLog)
	{
		MYLIB::Log2File(pFilename, pLog, true);

		return true;
	}

}


