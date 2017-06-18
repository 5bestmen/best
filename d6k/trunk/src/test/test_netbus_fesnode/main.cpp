
#include "netbus/nbapi.h"
#include "fesapi/fescnst.h"

#include <string>
using std::string;
#include <iostream>
using namespace std;

#include <QLibrary>
#include <QString>

#include <windows.h>


typedef void(*StartUp)(NET_CONFIG * pConf, const char *pszProjPath, const char * pszPredicateName, int32u nNodeType, int32u nStartMode);

void ModifyTime()
{
	SYSTEMTIME systime;
	systime.wYear = 0;
	systime.wMonth = 0;
	systime.wDay = 0;
	systime.wHour = 0;
	systime.wMinute = 0;
	systime.wSecond = 0;
	systime.wMilliseconds = 0;

	unsigned short year = 2016;
	unsigned short month = 1;
	unsigned short day = 1;
	unsigned short hour = 1;
	unsigned short minute = 1;
	unsigned short second = 1;

	memcpy(&systime.wYear, &year, 2);
	memcpy(&systime.wMonth, &month, 1);
	memcpy(&systime.wDay, &day, 1);
	memcpy(&systime.wHour, &hour, 1);
	memcpy(&systime.wMinute, &minute, 1);
	memcpy(&systime.wSecond, &second, 1);

	SetLocalTime(&systime);
}

int main()
{
	//ModifyTime();

	string strTest = "1.2.3.4";
	size_t pos = strTest.find_last_of('.');

	char buf[64] = { 0 };
	strncpy(buf, strTest.c_str(), pos + 1);
	strncpy(buf + pos + 1, "255", 3);

	NET_CONFIG* pTest = new NET_CONFIG();
	memset(pTest, 0, sizeof(NET_CONFIG));
	pTest->MyNodeOccNo = 1;
	pTest->IsDoubleNet = false;
	pTest->NodeCount = 2;
	pTest->pNodeConfig = new CNodeConfig[pTest->NodeCount];

	for (int i = 0; i < pTest->NodeCount; ++i)
	{
		memset(&pTest->pNodeConfig[i], 0, sizeof(CNodeConfig));
	}

	pTest->pNodeConfig[0].OccNo = 1;
	pTest->pNodeConfig[0].NodeType = NODE_FES;
	pTest->pNodeConfig[0].HostSlave = MAIN;
	pTest->bCheckTime = true;
	pTest->CheckTimeNodeOccNo = 1;
	strncpy(pTest->pNodeConfig[0].chIP[0], "172.16.38.2", strlen("172.16.38.2"));
	strncpy(pTest->pNodeConfig[0].HostName, "zyj-PC", strlen("zyj-PC"));

	pTest->pNodeConfig[1].OccNo = 2;
	pTest->pNodeConfig[1].NodeType = NODE_SVR;
	//strncpy(pTest->pNodeConfig[1].chIP[0], "172.16.38.131", strlen("172.16.38.131"));
	strncpy(pTest->pNodeConfig[1].chIP[0], "172.16.38.2", strlen("172.16.38.2"));
	strncpy(pTest->pNodeConfig[1].HostName, "test-PC", strlen("test-PC"));


	//StartNetBus(pTest, "", "", 1, 1);

	QLibrary* pNetbus = nullptr;

#ifdef _DEBUG
	pNetbus = new QLibrary("netbus");
#else
	pNetbus = new QLibrary("netbus");
#endif

	if (!pNetbus->load())
	{
		cout << "Load fail !!!" << endl;
		return -1;
	}

	StartUp startup = (StartUp)pNetbus->resolve("StartNetBus");
	if (startup == nullptr)
	{
		cout << "Resolve fail !!!" << endl;
		return -1;
	}

	startup(pTest, "111", "222", NODE_FES, 1);

	while (1)
	{
		::Sleep(10);
	}

	return 0;
}