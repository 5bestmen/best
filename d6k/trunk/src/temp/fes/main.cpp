

#include "server.h"

int main(int argc, char *argv[])
{
	CServer* pServer = new CServer;

	pServer->Initialize();
	pServer->Run();
	pServer->Shutdown();

	delete pServer;

	return 0;
}
