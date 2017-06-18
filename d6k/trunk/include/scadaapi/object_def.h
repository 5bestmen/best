/*! @file object_def.h
<PRE>
********************************************************************************
模块名       :
文件名       :  object_def.h
文件实现功能 :  对象数据定义
作者         :  LiJin
版本         :  V1.00
--------------------------------------------------------------------------------
备注         :  <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief     对象数据定义
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
*   \brief  对象的属性定义
\
*/
struct OBJ_PROPERTY
{
	int32u  OccNo;
	int32u  NameOccNo;     //! 只读字符串池内的ID序号
	int32u  BlockNo;

	int8u   Type;         //! 静态属性？还是动态属性
	int8u   Pad1;
	int16u  Pad2;

	int32u  LinkedNodeOccNo;// 关联的变量或者测点信息
	int32u  LinkedType;   //!  属于哪一种类型的变量（测点）
	int32u  LinkedOccNo;
	int32u  LinkedAtt;    //!  变量的何种属性


};

/*! \struct  OBJECT
*   \brief  对象定义 
    \  
*/
struct OBJECT
{
	int32u  OccNo;
	int32u  NameOccNo;     //! 只读字符串池内的ID序号
 	
	int32u  BlockNo; 

	struct OBJ_GUID ModelId;  //! 模型的ID


	int32u  StaticPropertyCount;
	int32u  DynamicPropertyCount;
	int32u  SubObjectCount;


};

 
 
 
  

#pragma pack ()

#endif // FES_DATA_DEF_H


/** @}*/

