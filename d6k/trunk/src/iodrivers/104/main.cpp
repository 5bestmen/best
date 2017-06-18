
#include "fesapi\fesapi.h"

extern "C" __declspec(dllexport) int drivermain(int nParam)
{
	int nChannelID = nParam;
	OpenIO(nChannelID);

	return 0;
}
