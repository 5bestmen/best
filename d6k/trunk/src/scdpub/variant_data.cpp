/*! @file nb_svc.cpp
<PRE>
********************************************************************************
模块名       :
文件名       :  nb_svc.cpp
文件实现功能 :  网络接口模块
作者         :  LiJin
版本         :  V1.00
--------------------------------------------------------------------------------
备注         :  <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief   网络接口模块
*  @author  LiJin
*  @version 1.0
*  @date    2016.09.01
*******************************************************************************/
#include "variant_data.h"
#include <QDebug>


CVariant::CVariant()
{
	Init();
}

CVariant::~CVariant()
{
	Clear();
}

void CVariant::Init()
{
	this->Type = DT_NULL;
	this->Length = 0;
	memset(& this->Value, 0, sizeof(this->Value));
}

void CVariant::Clear()
{
	if (this->Type == DT_STRING)
	{
		if (this->Value.pszTxtValue)
		{
			free(this->Value.pszTxtValue);
			this->Value.pszTxtValue = nullptr;
			this->Length = 0;
		}
	}
	else if (this->Type == DT_BLOCK)
	{
		if (this->Value.pBlock)
		{
			free(this->Value.pBlock);
			this->Value.pBlock = nullptr;
			this->Length = 0;
		}
	}
}

CVariant::CVariant(const IO_VARIANT& varSrc)
{
	this->Type = varSrc.Type;
	this->Length = varSrc.Length;

	this->Value = varSrc.Value;
}

CVariant::CVariant(const IO_VARIANT* pSrc)
{
	Q_ASSERT(pSrc);
	if (pSrc)
	{
		this->Type = pSrc->Type;
		this->Length = pSrc->Length;

		this->Value = pSrc->Value;
	}
}

CVariant::CVariant(const CVariant& varSrc)
{
	this->Type = varSrc.Type;
	this->Length = varSrc.Length;

	this->Value = varSrc.Value;
}
 
CVariant::CVariant(INT8U ucSrc, DATA_TYPE vtSrc )    // Creates a DT_BYTE 
{
	if ((vtSrc != DT_BOOLEAN) && (vtSrc != DT_BYTE) && (vtSrc != DT_CHAR))
	{
		return;
	}

	if (vtSrc == DT_BOOLEAN)
	{
		V_DT(*this) = DT_BOOLEAN;
		V_BOOL(*this) = (ucSrc ? 1 : 0);
		this->Length = 0;
	}
	else if (vtSrc == DT_CHAR)
	{
		V_DT(*this) = DT_CHAR;
		V_CHAR(*this) = ucSrc;
		this->Length = 0;
	}
	else
	{
		V_DT(*this) = DT_BYTE;
		V_BYTE(*this) = ucSrc;
		this->Length = 0;
	}
}

CVariant::CVariant(INT16S sSrc, DATA_TYPE vtSrc)
{
	if ((vtSrc != DT_WORD) && (vtSrc != DT_SHORT))
	{ 
		return;
	}

	if (vtSrc == DT_WORD)
	{
		V_DT(*this) = DT_WORD;
		V_SHORT(*this) = sSrc; 
		this->Length = 0;
	}
	else 
	{
		V_DT(*this) = DT_SHORT;
		V_SHORT(*this) = sSrc;
		this->Length = 0;
	}
}

CVariant::CVariant(INT32S lSrc, DATA_TYPE vtSrc)     // Creates a VT_I4, a VT_ERROR, or a VT_BOOL
{
	if ((vtSrc != DT_INT) && (vtSrc != DT_DWORD))
	{
		return;
	}

	if (vtSrc == DT_DWORD)
	{
		V_DT(*this) = DT_DWORD;
		V_DWORD(*this) = lSrc;
		this->Length = 0;
	}
	else
	{
		V_DT(*this) = DT_INT;
		V_INT(*this) = lSrc;
		this->Length = 0;
	}
}

CVariant::CVariant(FP32 fltSrc)
{
	V_DT(*this) = DT_FLOAT;
	V_FLOAT(*this) = fltSrc;
}

CVariant::CVariant(FP64 dblSrc, DATA_TYPE vtSrc )
{
	if (vtSrc != DT_DOUBLE)
	{
		Q_ASSERT(false);
		return;
	}

	this->Type = DT_DOUBLE;
	this->Length = 0;
	this->Value.dValue = dblSrc;
}

CVariant& CVariant::operator=(const IO_VARIANT& varSrc)
{
	this->Type = varSrc.Type;
	this->Length = varSrc.Length;

	this->Value = varSrc.Value;

	return *this;
}

CVariant& CVariant::operator=(FP64 dblSrc)
{
	if (V_DT(*this) != DT_DOUBLE )
	{
		Clear();
		V_DT(*this) = DT_DOUBLE;
	}

	V_DOUBLE(*this) = dblSrc;
	return *this;
}

CVariant& CVariant::operator=(FP32 fltSrc)
{
	if (V_DT(*this) != DT_FLOAT)
	{
		Clear();
		V_DT(*this) = DT_FLOAT;
	}

	V_FLOAT(*this) = fltSrc;
	return *this;
}

CVariant& CVariant::operator=(INT8U bSrc)
{
	if (V_DT(*this) != DT_BOOLEAN || V_DT(*this) != DT_BYTE)
	{
		Clear();
		V_DT(*this) = DT_BOOLEAN;
	}

	V_BOOL(*this) = bSrc;
	return *this;
}

CVariant& CVariant::operator=(INT8S cSrc)
{
	if (V_DT(*this) != DT_CHAR)
	{
		Clear();
		V_DT(*this) = DT_CHAR;
	}

	V_CHAR(*this) = cSrc;
	return *this;
}

// Compare a _variant_t against a const VARIANT& for equality

inline bool CVariant::operator==(const IO_VARIANT& varSrc) const throw()
{
	return *this == &varSrc;
}

inline bool CVariant::operator==(const IO_VARIANT* pSrc) const throw()
{
	if (pSrc == nullptr) 
	{
		return false;
	}

	if (this == pSrc)
	{
		return true;
	}

	// Variants not equal if types don't match
	if (V_DT(*this) != V_DT(*pSrc))
	{
		return false;
	}

	switch (this->Type)
	{
	case DT_NULL:
		return true;

	case DT_BOOLEAN:
		return V_BOOL(*this) == V_BOOL(*pSrc);
	case DT_CHAR:
		return V_CHAR(*this) == V_CHAR(*pSrc);
	case DT_BYTE:
		return V_BYTE(*this) == V_BYTE(*pSrc);
	case DT_SHORT:
		return V_SHORT(*this) == V_SHORT(*pSrc);
	case DT_WORD:
		return V_WORD(*this) == V_WORD(*pSrc);
	case DT_INT:
		return V_INT(*this) == V_INT(*pSrc);
	case DT_DWORD:
		return V_DWORD(*this) == V_DWORD(*pSrc);
	case DT_LLONG:
		return V_LLONG(*this) == V_LLONG(*pSrc);
	case DT_LWORD:
		return V_LWORD(*this) == V_LWORD(*pSrc);
	case DT_FLOAT:
		return V_FLOAT(*this) == V_FLOAT(*pSrc);
	case DT_DOUBLE:
		return V_DOUBLE(*this) == V_DOUBLE(*pSrc);
	case DT_BLOCK:
		return ((this->Length == pSrc->Length) && std::memcmp(V_BLOCK(*this), V_BLOCK(*pSrc), pSrc->Length) == 0);
	case DT_STRING:
		return (strlen(V_STR(*this)) == strlen(V_STR(*pSrc)) && strcmp(V_STR(*this), V_STR(*pSrc)));
	default:
		Q_ASSERT(false);
		break;
	}

	return false;
}

CVariant::operator IO_VARIANT() const throw()
{
	return *(IO_VARIANT*)(this);
}

CVariant::operator double() const
{
	if (V_DT(*this) == DT_DOUBLE)
	{
		return V_DOUBLE(*this);
	}

	CVariant varDest;
	varDest.ChangeType(DT_DOUBLE, this);

 	return V_DOUBLE(varDest);
}

CVariant::operator unsigned char() const
{
	if (V_DT(*this) == DT_BYTE)
	{
		return V_BYTE(*this);
	}

	CVariant varDest;
	varDest.ChangeType(DT_BYTE, this);

	return V_BYTE(varDest);
}


IO_VARIANT& CVariant::GetVariant() throw()
{
	return *(IO_VARIANT*) this;
}

void CVariant::ChangeType(DATA_TYPE vartype, const CVariant* pSrc)
{
	if (pSrc == nullptr)
	{
		pSrc = this;
	}

	if (this!=pSrc || vartype != V_DT(*this))
	{
		if (V_DT(*this) != DT_BLOCK || V_DT(*this) != DT_STRING)
		{
			Clear();
		}
		this->Type = vartype;
		// 暂时不考虑转换成字符串
		if ( V_DT(*pSrc) != DT_BLOCK && V_DT(*pSrc) != DT_STRING )
		{
			this->Value = pSrc->Value;
		}
		else
		{

		}
	}
}

/** @}*/
