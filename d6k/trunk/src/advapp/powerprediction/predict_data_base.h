#ifndef PREDICT_BASE_DATA_H
#define PREDICT_BASE_DATA_H

#include "datatypes.h"

#include <vector>

class CPredictBaseData
{
public:
	CPredictBaseData();
	virtual ~CPredictBaseData();
public:
	virtual void Save() = 0;

	virtual void SetModifyFlag(bool bFlag) = 0;

	virtual bool GetModifyFlag() const = 0;
	
	bool CheckTagName(std::string & szTagName) const;

	virtual int32u GetOccNo() const = 0;
public:

protected:
	bool LogString(char *pszMoudleName, char *szLogTxt, int nLevel);
protected:
	bool m_bModifyFlag;
};

#endif  //PREDICT_BASE_DATA_H