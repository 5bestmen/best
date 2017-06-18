#ifndef _SHELL_H
#define _SHELL_H
#include "../../include/datatypes.h"

#include <string>

#include <QLibrary>


class CShell
{
public:
	CShell();
	~CShell();

public:
	void SetDllName(const std::string& dllName)
	{
		m_strDriverName = dllName;
	}

	void SetProjName(const std::string& projName)
	{
		m_szProjName = projName;
	}

	void SetNodeOccNo(const int32u& nodeOccNo)
	{
		m_nNodeOccNo = nodeOccNo;
	}
	void SetOccNo(const int32u& OccNo)
	{
		m_nOccNo = OccNo;
	}
public:
	bool Open() ;
	bool Run();
	void Close();	
private:
	std::string   m_strDriverName;
	std::string   m_szProjName;
	int32u m_nNodeOccNo;
	int32u m_nOccNo;
};

#endif // _SHELL_H
