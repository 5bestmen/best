/*! @file 
<PRE>
********************************************************************************
模块名       :   
文件名       :   
文件实现功能 :    
作者         :  LiJin  
版本         :  V1.00  
--------------------------------------------------------------------------------
备注         : <其它说明>  
--------------------------------------------------------------------------------
修改记录 : 
日 期        版本     修改人              修改内容
 
********************************************************************************
</PRE>
*  @brief    
*  @author LiJin
*  @version 1.0
*  @date    2010.03.16
*******************************************************************************/

#ifndef __VARIANT_H
#define __VARIANT_H

#include <stdlib.h>
#include <string.h>

#include "datatypes.h"

/** @defgroup IO_VARIANT   复合值类型
 *  @ingroup IO_VARIANT
 *  @brief   复合值类型处理
 @{
 */
 

/*! \enum IO_VARENUM
\brief 复合值类型
*/
#if 0
enum IO_VARENUM
{
	IO_VT_EMPTY = 0,  //!< 空
	IO_VT_UINT8 = 1,
	IO_VT_UINT16 = 2,
	IO_VT_UINT32 = 3,
	IO_VT_INT32 = 4,

	IO_VT_UINT64 = 5,
	IO_VT_INT64 = 6,

	IO_VT_FLOAT = 7,
	IO_VT_DOUBLE = 8,
	IO_VT_STRING = 9,
	IO_VT_BLOCK = 10,
	IO_VT_MAX
};

#endif

enum DATA_TYPE
{
	DT_NULL = 0,
	DT_BOOLEAN = 1,	  //!< 布尔，8位，缺省值0
	DT_CHAR = 2,      //!< 有符号字节（8位）
	DT_BYTE = 3,      //!< 无符号字节（8位）
	DT_SHORT = 4,     //!< 无符号短整数 ，16位， 
	DT_WORD = 5,      //!< 无符短整数 ，16位
	DT_INT = 6,       //!< 有符号长整数 32位
	DT_DWORD = 7,      //!< 无符号长整数 32位
	DT_LLONG = 8,      //!< 有符号长整数  64位
	DT_LWORD = 9,      //!< 无符号长整数  64位
	DT_FLOAT = 10,
	DT_DOUBLE = 11,
	DT_STRING = 12,
	DT_BLOCK = 13,
	DT_MAX
};
 

/*! \struct IO_VARIANT
\brief 复合值结构定义
*/
struct IO_VARIANT
{
	INT32U Type;   //! 类型
	INT32U Length; //! if Type is IO_VT_STR, including \0, and IO_VT_BLOCK

	/*! \union UNION_VALUE
	\brief 值域
	*/
	union UNION_VALUE
	{
		FP64	dValue;         //! double
		FP32    fValue;         //! float
		INT64U  lValue;         //! unsigned long long 
		INT64S  slValue;        //! long long 
		INT32U	nValue;         //! unsigned int
		INT32S	iValue;         //! signed int
		INT16U	wValue;         //! unsigned short
		INT16S  sValue;         //! short
		INT8U	bValue;         //! unsigned char
		INT8S   cValue;         //! char
		INT8S*	pszTxtValue;    //! string
		INT8U*	pBlock;         //! buffer
	} Value;  //! 值域
};

/**
* \typedef struct   IO_VARIANT CIoVariant
* \brief 复合值结构
*/
typedef struct IO_VARIANT CIoVariant;

#define V_DT(X)         ((X).Type)

#define V_UNION(X, Y)   ((X).Value.Y)

#define V_BOOL(X)         V_UNION(X, bValue)

#define V_CHAR(X)         V_UNION(X, cValue)
#define V_BYTE(X)         V_UNION(X, bValue)
#define V_SHORT(X)        V_UNION(X, sValue)
#define V_WORD(X)         V_UNION(X, wValue) 

#define V_INT(X)          V_UNION(X, iValue)
#define V_DWORD(X)         V_UNION(X, nValue) 

#define V_LLONG(X)        V_UNION(X, slValue)
#define V_LWORD(X)        V_UNION(X, lValue)

#define V_FLOAT(X)        V_UNION(X, fValue) 
#define V_DOUBLE(X)       V_UNION(X, dValue) 
#define V_STR(X)          V_UNION(X, pszTxtValue)
#define V_BLOCK(X)        V_UNION(X, pBlock)


/*! \fn void SetVariantValue(CIoVariant*pVT, INT16U nType, void *pValue, int length)
*********************************************************************************************************
** \brief   复合变量设值
** \details
** \param     pVT
** \param     nType
** \param     pValue
** \return    void
** \author   LiJin
** \date   2010年4月11日
** \note   注意，如果是字符串，使用后，必须CleanUpVariant，回收资源
********************************************************************************************************/
__inline void SetVariantValue(CIoVariant*pVT, INT16U nType, void *pValue, INT16U length)
{
	//IO_ASSERT(pVT && pValue);
	if (pVT == NULL || pValue == NULL)
		return;
	//IO_ASSERT(nType > IO_VT_EMPTY && nType < IO_VT_MAX);

	pVT->Type = nType;
	pVT->Length = length;
	switch (nType)
	{
	case DT_BOOLEAN:
	case DT_BYTE:
		pVT->Value.bValue = *((INT8U *)pValue);
		break;
	case DT_CHAR:
		pVT->Value.cValue = *((INT8S *)pValue);
		break;
	case  DT_WORD:
		pVT->Value.wValue = *((INT16U *)pValue);
		break;
	case  DT_SHORT:
		pVT->Value.sValue = *((INT16S *)pValue);
		break;
	case  DT_INT:
		pVT->Value.iValue = *((INT32S *)pValue);
		break;
	case  DT_DWORD:
		pVT->Value.nValue = *((INT32U *)pValue);
		break;
	case  DT_LLONG:
		pVT->Value.slValue = *((INT64S *)pValue);
		break;
	case  DT_LWORD:
		pVT->Value.lValue = *((INT64U *)pValue);
		break; 
	case  DT_FLOAT:
		pVT->Value.fValue = *((FP32 *)pValue);
		break;
	case  DT_DOUBLE:
		pVT->Value.dValue = *((FP64 *)pValue);
		break;
	case  DT_STRING:
		{
			char *pszTxt = (char *)pValue;

			if (pVT->Value.pszTxtValue)
			{
				free(pVT->Value.pszTxtValue);
			}

			pVT->Length = static_cast<INT32U> (strlen(pszTxt));
			//assert(nLen <= 128);
		//	if (pVT->Length <= 1024)
			if (pVT->Length > 0)
			{
				pVT->Value.pszTxtValue = (char*)calloc(pVT->Length + 1, sizeof(char));
				strncpy(pVT->Value.pszTxtValue, pszTxt, pVT->Length);
			}
		}
		break;
	case  DT_BLOCK:
		{
			INT8U *pBlock = (INT8U*)pValue;

			if (pVT->Value.pBlock)
			{
				free(pVT->Value.pBlock);
			} 
			pVT->Value.pBlock = (INT8U*)calloc(length, sizeof(char));
			memcpy(pVT->Value.pBlock, pBlock, length);
		}
		break;
	default:
		break;
	}
}
/*! \fn  __inline void CleanUpVariant( CIoVariant*pVT )
*********************************************************************************************************
** \brief   回收相应的资源
** \details
** \param     pVT
** \return    void
** \author   LiJin
** \date   2010年4月20日
** \note
********************************************************************************************************/
__inline void CleanUpVariant(CIoVariant*pVT)
{
	//IO_ASSERT(pVT);
	if (pVT == NULL)
		return;

	if (pVT->Type == DT_STRING)
	{
		if (pVT->Value.pszTxtValue)
		{
			free(pVT->Value.pszTxtValue);
			pVT->Value.pszTxtValue = NULL;
			pVT->Length = 0;
		}
	}
	else if (pVT->Type == DT_BLOCK)
	{
		if (pVT->Value.pBlock)
		{
			free(pVT->Value.pBlock);
			pVT->Value.pBlock = NULL;
			pVT->Length = 0;
		}
	}
}


#define S_BOOL(X, VAL)		 SetVariantValue((X), DT_BOOLEAN,reinterpret_cast<void*>(VAL), sizeof(INT8U))
#define S_CHAR(X, VAL)		 SetVariantValue((X), DT_CHAR, reinterpret_cast<void*>(VAL), sizeof(INT8S))
#define S_BYTE(X, VAL)		 SetVariantValue((X), DT_BYTE, reinterpret_cast<void*>(VAL), sizeof(INT8U))

#define S_SHORT(X,VAL)       SetVariantValue((X), DT_SHORT, reinterpret_cast<void*>(VAL), sizeof(INT16S))
#define S_WORD(X,VAL)        SetVariantValue((X), DT_WORD, reinterpret_cast<void*>(VAL), sizeof(INT16U))

#define S_INT(X,VAL)         SetVariantValue((X), DT_INT, reinterpret_cast<void*>(VAL), sizeof(INT32S))
#define S_DWORD(X,VAL)       SetVariantValue((X), DT_DWORD, reinterpret_cast<void*>(VAL), sizeof(INT32U))

#define S_LWORD(X,VAL)       SetVariantValue((X), DT_LWORD, reinterpret_cast<void*>(VAL), sizeof(INT64U))
#define S_LLONG(X,VAL)       SetVariantValue((X), DT_LLONG, reinterpret_cast<void*>(VAL), sizeof(INT64S))

#define S_FLOAT(X,VAL)       SetVariantValue((X), DT_FLOAT, reinterpret_cast<void*>(VAL), sizeof(FP32))
#define S_DOUBLE(X,VAL)      SetVariantValue((X), DT_DOUBLE, reinterpret_cast<void*>(VAL), sizeof(FP64))
#define S_STR(X,VAL)         SetVariantValue((X), DT_STRING, reinterpret_cast<void*>(VAL), 0)
#define S_BLOCK(X, VAL,LEN)  SetVariantValue((X), DT_BLOCK,reinterpret_cast<void*>(VAL), (LEN))

template<typename T>
T ConvertVariant(const IO_VARIANT & var)
{
	T value;
	memset(&value, 0, sizeof(T));
	switch (var.Type)
	{
	case  DT_BOOLEAN:
		value = static_cast<T> (V_BOOL(var));
		break;
	case  DT_CHAR:
		value = static_cast<T> (V_CHAR(var));
		break;
	case  DT_BYTE:
		value = static_cast<T> (V_BYTE(var));
		break;
	case  DT_SHORT:
		value = static_cast<T> (V_SHORT(var));
		break;
	case  DT_WORD:
		value = static_cast<T> (V_WORD(var));
		break;
	case  DT_INT:
		value = static_cast<T> (V_INT(var));
		break;
	case  DT_DWORD:
		value = static_cast<T> (V_DWORD(var));
		break;
	case  DT_LLONG:
		value = static_cast<T> (V_LLONG(var));
		break;
	case  DT_LWORD:
		value = static_cast<T> (V_LWORD(var));
		break;
	case  DT_FLOAT:
		value = static_cast<T> (V_FLOAT(var));
		break;
	case  DT_DOUBLE:
		value = static_cast<T> (V_DOUBLE(var));
		break;
	default:
		break;
	}

	return value;
}



#endif // __VARIANT_H

/** @}*/  

