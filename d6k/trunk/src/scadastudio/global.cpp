/*! @file
<PRE>
********************************************************************************
ģ����       :  ȫ�ֺ���
�ļ���       :  global.cpp
�ļ�ʵ�ֹ��� :   ȫ�ֺ���
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
** \details ���tagname�����Ƿ���Ч
** \param strTagName 
** \return bool 
** \author gw
** \date 2017��4��23�� 
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
** \details ��������һ���ַ�ռ�����ֽڣ�������ʽֻ�����������ַ����������Լ�һ���ж�
** \param strDesc 
** \return bool 
** \author gw
** \date 2017��4��23�� 
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