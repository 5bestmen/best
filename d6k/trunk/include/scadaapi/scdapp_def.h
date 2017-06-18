/*! @file scdapp_def.h
<PRE>
********************************************************************************
ģ����       :
�ļ���       :  scdapp_def.h
�ļ�ʵ�ֹ��� :  SCADA Ӧ�õĶ���
����         :  LiJin
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         :  <����˵��>
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

********************************************************************************
</PRE>
*  @brief     SCADA Ӧ�õĶ���
*  @author    LiJin
*  @version   1.0
*  @date      2017.03.24
*******************************************************************************/
#ifndef SCD_APP_DEF_H
#define SCD_APP_DEF_H

#include "../datatypes.h"
#include "../variant.h"
#include "../public.h" 
#include "../fesapi/fescnst.h"
 
 

#pragma pack (1)
enum AppType 
{
	Inn_APP=0,    //����Ӧ��
	Ext_App=1,    //��չ����
};

enum LoadType
{
	Load_Dll=0,   //dll����
	Load_Exe=1,   //exe����
};
 
/*! \struct  SAPP
*   \brief  SCADA Ӧ�� */
struct SAPP
{
	int32u  OccNo;                //! �ؼ��� Ψһ Id ��1��ʼ
	int32u  NameOccNo;            //! ֻ���ַ������ڵ�ID���

	int8u   IsDefined;           /* YES : is defined, NO: spare */
	int8u   ScanEnable;          /* SCAN_OUT / SCAN_IN, DEFAULT_SCAN */
	int8u   Init;                /* UNINITED / INITED ,DEFAULT_INIT  */
	int8u   Quality;             /* QUALITY_OK /QUALITY_BAD, DEFAULT_QUA */
	
	int8u   Heartbeat;          /* ���� */
	int8u   PrintOn;            //���Կ��أ�1�������¼��0������� 

	int32u State;
	int8u  Prority;
	LoadType nLoadType;    //! �������ͣ�dll��ʽ ��exe��ʽ
	AppType  nAppType;	   //! Ӧ�����ͣ�����Ӧ�ã���չ����
	
//	char    ChannelName[MAX_NAME_LENGTH + STRING_PAD]; //ͨ������
	char    ProgramName[MAX_NAME_LENGTH + STRING_PAD]; //�� Ӧ�ó�������
	 
	int32s  MailBoxID;  //! ����ID
};

#pragma pack ()

#endif // SCD_APP_DEF_H


/** @}*/

