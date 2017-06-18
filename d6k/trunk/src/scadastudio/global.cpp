/*! @file
<PRE>
********************************************************************************
模块名       :  全局函数
文件名       :  global.cpp
文件实现功能 :   全局函数
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
*  @date    2017.4.21
*/

#include "scadastudiodefine.h"
#include "scadastudio/define.h"
#include "scadastudio/icore.h"
#include "global.h"

#include <QObject>


/*! \fn bool CheckTagNameIsValid(const QString &strTagName)
********************************************************************************************************* 
** \brief CheckTagNameIsValid 
** \details 检查tagname长度是否有效
** \param strTagName 
** \return bool 
** \author gw
** \date 2017年4月23日 
** \note 
********************************************************************************************************/
bool CheckTagNameIsValid(const QString &strTagName, const char *szModuleName)
{
	bool bFlag = strTagName.size() > MAX_TAGNAME_LEN_SCADASTUDIO || strTagName.size() == 0;
	Q_ASSERT(!bFlag);
	if (bFlag)
	{
		auto strError = QObject::tr("TagName %1 is invalid!!!").arg(strTagName);
		s_pGlobleCore->LogMsg(szModuleName, strError.toStdString().c_str(), LEVEL_1);

		return false;
	}

	return true;
}

/*! \fn bool CheckDescIsValid(const QString &strDesc)
********************************************************************************************************* 
** \brief CheckDescIsValid 
** \details 由于中文一个字符占两个字节，正则表达式只是限制输入字符个数，所以加一个判断
** \param strDesc 
** \return bool 
** \author gw
** \date 2017年4月23日 
** \note 
********************************************************************************************************/
bool CheckDescIsValid(const QString &strDesc, const char *szModuleName, int nCounts)
{
	bool bFlag = strDesc.toLocal8Bit().size() > nCounts;
	Q_ASSERT(!bFlag);
	if (bFlag)
	{
		auto strError = QObject::tr("String %1 length is invalid!!!").arg(strDesc);
		s_pGlobleCore->LogMsg(szModuleName, strError.toStdString().c_str(), LEVEL_1);
		
		return false;
	}

	return true;
}