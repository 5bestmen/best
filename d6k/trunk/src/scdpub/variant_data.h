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

#ifndef VARIANT_EX_H
#define VARIANT_EX_H

#include "variant.h"

class CVariant : public IO_VARIANT
{
public:
	CVariant();
	~CVariant();

//	CVariant() throw();

	CVariant(const IO_VARIANT& varSrc);
	CVariant(const IO_VARIANT* pSrc);
	CVariant(const CVariant& varSrc);

	CVariant(INT8U ucSrc, DATA_TYPE vtSrc = DT_BYTE);    // Creates a DT_BYTE 
	CVariant(INT16S sSrc, DATA_TYPE vtSrc = DT_SHORT);    // Creates a DT_SHORT 
 
	CVariant(INT32S lSrc, DATA_TYPE vtSrc = DT_INT);     // Creates a VT_I4, a VT_ERROR, or a VT_BOOL
	CVariant(FP32 fltSrc);                               // Creates a VT_R4
	CVariant(FP64 dblSrc, DATA_TYPE vtSrc = DT_DOUBLE); // Creates a VT_R8, or a VT_DATE

	//Extractors
	operator IO_VARIANT() const throw();

	operator double() const;
	operator unsigned char() const;
public:
	CVariant& operator=(const IO_VARIANT& varSrc);
	CVariant& operator=(const IO_VARIANT* pSrc);
	CVariant& operator=(const CVariant& varSrc);

	CVariant& operator=(INT8S cSrc);
	CVariant& operator=(INT8U bSrc);

	CVariant& operator=(INT16S sSrc);
	CVariant& operator=(INT16U wSrc);

	CVariant& operator=(INT32S iSrc);              
	CVariant& operator=(INT32U nSrc);              

	CVariant& operator=(INT64S slSrc);
	CVariant& operator=(INT64U lSrc);

	CVariant& operator=(FP32 fltSrc);
	CVariant& operator=(FP64 dblSrc);            

	bool operator==(const IO_VARIANT& varSrc) const throw();
	bool operator==(const IO_VARIANT* pSrc) const throw();

	bool operator!=(const IO_VARIANT& varSrc) const throw();
	bool operator!=(const IO_VARIANT* pSrc) const throw();


	IO_VARIANT& GetVariant() throw();
private:
	void ChangeType(DATA_TYPE vartype, const CVariant* pSrc);

	void Clear();
	void Init();
};
 

#endif // VARIANT_EX_H

/** @}*/  

