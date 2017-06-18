#include "public_define.h"

namespace ParaseDefines
{
	QString GetDefineDes(int index)
	{
		QString sqlStart;

		switch (index)
		{
		case HISDATA_REAL://当前值，工程值
		{
			sqlStart = "dp";
		}
		break;
		//  case HISDATA_AVERAGE://平均值
		case HISDATA_DAYS_AVERAGE:
		{
			sqlStart = "da";
		}
		break;
		case HISDATA_DAYKWHTTL://待定，电度累计值
		{
			sqlStart = "sum";
		}
		break;
		//   case HISDATA_REALMAX://实时最大值
		case HISDATA_DAYS_MAX:
		{
			sqlStart = "mx";
		}
		break;
		// case HISDATA_MAXTIME://最大实时值发生时间
		case HISDATA_DAYS_MAXTIME:
		{
			sqlStart = "mn";
		}
		break;
		// case HISDATA_REALMIN://实时最小值
		case HISDATA_DAYS_MIN:
		{
			sqlStart = "mt";
		}
		break;
		//case HISDATA_MINTIME://最小值发生时间
		case HISDATA_DAYS_MINTIME:
		{
			sqlStart = "nt";
		}
		break;
		case HISDATA_OVHCOUNT://越上限次数
		{
			sqlStart = "e1";
		}
		break;
		case HISDATA_OVHTIME:  //越上限时间
		{
			sqlStart = "e10";
		}
		break;
		case HISDATA_OVHHCOUNT://越上上线次数
		{
			sqlStart = "e3";
		}
		break;
		case HISDATA_OVHHTIME://越上上限时间
		{
			sqlStart = "e4";
		}
		break;
		case HISDATA_DOWNLCOUNT://越下限次数
		{
			sqlStart = "e5";
		}
		break;
		case HISDATA_DOWNLTIME://越下限时间
		{
			sqlStart = "e6";
		}
		break;

		case HISDATA_DOWNLLCOUNT://越下下限次数
		{
			sqlStart = "e7";
		}
		break;
		case HISDATA_DOWNLLTIME://越下下限时间
		{
			sqlStart = "e8";
		}
		break;
		case HISDATA_REALHGL://待用，合格率
		{
			sqlStart = "e9";
		}
		break;
		case  KWH_DATA:
		{
			sqlStart = "dd";
		}
		break;
		default:
			break;;
		}

		return sqlStart;
	}

	int GetDefineInt(const QString& des)
	{
		if (des == "dp")
		{
			return HISDATA_REAL;
		}
		if (des == "dd")
		{
			return KWH_DATA;
		}
		if (des == "da")
		{
			return HISDATA_DAYS_AVERAGE;
		}
		if (des == "sum")
		{
			return HISDATA_DAYKWHTTL;
		}
		if (des == "mx")
		{
			//return HISDATA_REALMAX;
			return HISDATA_DAYS_MAX;
		}
		if (des == "mn")
		{
			// return HISDATA_MAXTIME;
			return HISDATA_DAYS_MAXTIME;
		}
		if (des == "mt")
		{
			// return HISDATA_REALMIN;
			return HISDATA_DAYS_MIN;

		}
		if (des == "nt")
		{
			//   return HISDATA_MINTIME;
			return HISDATA_DAYS_MINTIME;
		}
		if (des == "e1")
		{
			return HISDATA_OVHCOUNT;
		}
		if (des == "e10")
		{
			return HISDATA_OVHTIME;
		}
		if (des == "e3")
		{
			return HISDATA_OVHHCOUNT;
		}
		if (des == "e4")
		{
			return HISDATA_OVHHTIME;
		}
		if (des == "e5")
		{
			return HISDATA_DOWNLCOUNT;
		}
		if (des == "e6")
		{
			return HISDATA_DOWNLTIME;
		}
		if (des == "e7")
		{
			return HISDATA_DOWNLLCOUNT;
		}
		if (des == "e8")
		{
			return HISDATA_DOWNLLTIME;
		}

		if (des == "e9")
		{
			return HISDATA_REALHGL;
		}

		return 0;
	}
}

