/*! @file
<PRE>
********************************************************************************
ģ����       :  �ڲ��������߲����ӿڶ���
�ļ���       :
�ļ�ʵ�ֹ��� :
����         :  LiJin
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         : <����˵��>
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

********************************************************************************
</PRE>
*  @brief
*  @author  LiJin
*  @version 1.0
*  @date    2015.09.1
*/
#ifndef NETBUS_API_H
#define NETBUS_API_H
 

#include <QtCore/QtGlobal>

#ifdef NBAPI_LIB
# define NBAPI Q_DECL_EXPORT
#else
# define NBAPI Q_DECL_IMPORT
#endif

#include <netbus/nbdef.h> 
#include "../datatypes.h"

extern "C"
{
	/*! \fn NBAPI bool StartNetBus(NET_CONFIG * pConf, const char * pLogStationName, int32u nNodeType, int32u nStartMode);
	********************************************************************************************************* 
	** \brief StartNetBus 
	** \details ������������
	** \param pConf ϵͳ�ڵ�������Ϣ
	** \param pszPredicateName ���������ƣ�Ҳ��LOG��¼ǰ׺���˴�һ�����룺"FES"����"SCADA"����
	** \param pszProjPath  ��Ŀ·�� 
	** \param nNodeType : NODE_SVR    NODE_FES  	NODE_CLIENT 
	** \param nStartMode  ����ģʽ
	** \return NBAPI bool 
	** \author LiJin 
	** \date 2016��10��22�� 
	** \note 
	********************************************************************************************************/
	NBAPI bool StartNetBus(NET_CONFIG * pConf, const char *pszProjPath, const char * pszPredicateName, int32u nNodeType, int32u nStartMode);
	NBAPI void StopNetBus(const char * pszPredicateName);

	// ��̬����ϵͳ�ͻ��˽ڵ�
	NBAPI void AddNode(NET_CONFIG * pConf);
	   
	// ���ͽӿ�
	NBAPI int NBSendData(const char * pszPredicateName, const int8u * pMsg, int32u nMsgLen);

	// ���սӿڣ�
	NBAPI bool NBRecvData(const char * pszPredicateName, const int8u * pMsg, int32u nMsgLen, int32u *pCopySize, int32u ulTimeout);
	// �ֶ��л�
	NBAPI bool ManualSwitchNode(char *pszPredicateName);
}



/** @}*/
#endif // NETBUS_API_H
