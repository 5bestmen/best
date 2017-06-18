/*! @file object_def.h
<PRE>
********************************************************************************
ģ����       :
�ļ���       :  object_def.h
�ļ�ʵ�ֹ��� :  �������ݶ���
����         :  LiJin
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         :  <����˵��>
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

********************************************************************************
</PRE>
*  @brief     �������ݶ���
*  @author    LiJin
*  @version   1.0
*  @date      2016.12.09
*******************************************************************************/
#ifndef OBJECT_DATA_DEF_H
#define OBJECT_DATA_DEF_H

#include "../datatypes.h"
#include "../variant.h"
#include "../public.h"
 

#ifndef NULL
#ifdef __cplusplus
#define NULL    0
#else
#define NULL    ((void *)0)
#endif
#endif
 

#pragma pack (1)


struct OBJ_GUID
{
	unsigned long   Data1;
	unsigned short  Data2;
	unsigned short  Data3;
	unsigned char   Data4[8];
};

/*! \struct  OBJ_PROPERTY
*   \brief  ��������Զ���
\
*/
struct OBJ_PROPERTY
{
	int32u  OccNo;
	int32u  NameOccNo;     //! ֻ���ַ������ڵ�ID���
	int32u  BlockNo;

	int8u   Type;         //! ��̬���ԣ����Ƕ�̬����
	int8u   Pad1;
	int16u  Pad2;

	int32u  LinkedNodeOccNo;// �����ı������߲����Ϣ
	int32u  LinkedType;   //!  ������һ�����͵ı�������㣩
	int32u  LinkedOccNo;
	int32u  LinkedAtt;    //!  �����ĺ�������


};

/*! \struct  OBJECT
*   \brief  ������ 
    \  
*/
struct OBJECT
{
	int32u  OccNo;
	int32u  NameOccNo;     //! ֻ���ַ������ڵ�ID���
 	
	int32u  BlockNo; 

	struct OBJ_GUID ModelId;  //! ģ�͵�ID


	int32u  StaticPropertyCount;
	int32u  DynamicPropertyCount;
	int32u  SubObjectCount;


};

 
 
 
  

#pragma pack ()

#endif // FES_DATA_DEF_H


/** @}*/

