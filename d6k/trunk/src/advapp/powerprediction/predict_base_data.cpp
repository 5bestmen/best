#include <string>
#include <QString>
#include <QObject>



#include "predict_data_base.h"
#include "define.h"


CPredictBaseData::CPredictBaseData()
	:m_bModifyFlag(false)
{


}

CPredictBaseData::~CPredictBaseData()
{

}

bool CPredictBaseData::CheckTagName(std::string & szTagName) const
{
	if (szTagName.length() == 0 || szTagName.length() > MAX_NAME_LENGTH)
	{
		return false;
	}

	//对字符串进行迭代检查，查看是否有非法字符 
	std::string str("Test string");

	auto bFlag = true;

	QString strError;
	for (std::string::iterator it = str.begin(); it != str.end(); ++it)
	{
		auto &check = (*it);

		if (check == ' ' || check == '~' || check == '@' || check == '#' || check == '$' ||
			check == '%' || check == '^' || check == '&' || check == '*' || check == '(' ||
			check == ')' || check == '_' || check == '+' || check == '=' || check == '-' ||
			check == '.' || check == '<' || check == '>' || check == '?' || check == '/' ||
			check == '\\' || check == '|')
		{
			strError = QString("Tag name %1 contais invalid char %2").arg(szTagName.c_str()).arg(check);

			bFlag = false;
		}
	}

	return bFlag;
}


bool CPredictBaseData::LogString(char* pszModuleName, char* szLogTxt, int nLevel)
{
	return true;
}