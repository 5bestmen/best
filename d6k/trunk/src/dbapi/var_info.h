#pragma once

#ifndef _VAR_INFO_H_
#define _VAR_INFO_H_

#include <string>

struct columnsinfo
{
	int  dbtype;		//database type
	int  nullok;		//whether is nullable or not
	int  dbsize;
	int  scale;			//小数点后的数字位
	int  prec;			//多少位有效数字
	std::string name;	//column
	columnsinfo() :dbtype(0), nullok(1), dbsize(0), scale(0), prec(0) {};
};

class CVarinfo :public columnsinfo
{
public:
	int pos;
	int dbpendtype;		// database dependent, column datatype code.

public:
	CVarinfo() :pos(0), dbpendtype(0)
	{

	}

	bool SortByType(const CVarinfo& rhs) const
	{
		return dbtype < rhs.dbtype;
	}

	bool SortByPos(const CVarinfo &rhs)	const
	{
		return pos < rhs.pos;
	}

};
#endif
