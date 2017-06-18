
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

int main()
{
	
	NET_CONFIG* pTest = new NET_CONFIG();
	pTest->MyNodeOccNo = 1;
	pTest->IsDoubleNet = false;
	pTest->NodeCount = 2;
	pTest->pNodeConfig = new CNodeConfig[pTest->NodeCount];

	pTest->pNodeConfig[0].OccNo = 1;
	pTest->pNodeConfig[0].NodeType = NODE_SVR;
	strncpy(pTest->pNodeConfig[0].chIP[0], "172.16.38.2", strlen("172.16.38.2"));
	strncpy(pTest->pNodeConfig[0].HostName, "zyj-PC", strlen("zyj-PC"));

	pTest->pNodeConfig[1].OccNo = 2;
	pTest->pNodeConfig[1].NodeType = NODE_FES;
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

	startup(pTest, "111", "222", NODE_SVR, 1);

	while (1)
	{
		::Sleep(10);
	}

	return 0;
}