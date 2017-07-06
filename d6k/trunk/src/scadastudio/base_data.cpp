/*! @file
<PRE>
********************************************************************************
模块名       :  配置结构
文件名       :  base_data.cpp
文件实现功能 :  基础配置结构
作者         :  gw
版本         :  V1.00
--------------------------------------------------------------------------------
备注         : <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

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
	** \date 2016年1月4日
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
	** \date 2016年1月4日
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
	** \details 对输入的名字进行校验检查
	** \param szTagName
	** \return bool
	** \author LiJin
	** \date 2016年8月9日
	** \note
	********************************************************************************************************/

	bool CBaseData::CheckTagName(std::string & szTagName) const
	{
		if (szTagName.length() == 0 || szTagName.length() > MAX_NAME_LENGTH_SCADASTUDIO)
		{
			return false;
		}

		// 对字符串进行迭代检查，看看是否有非法字符？
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
	** \details 日志log
	** \param pFilename 
	** \param pLog 
	** \return bool 
	** \author xingzhibing
	** \date 2017年6月23日 
	** \note 
	********************************************************************************************************/
	bool CBaseData::LogToFile(const char* pFilename, const char* pLog)
	{
		MYLIB::Log2File(pFilename, pLog, true);

		return true;
	}

}


