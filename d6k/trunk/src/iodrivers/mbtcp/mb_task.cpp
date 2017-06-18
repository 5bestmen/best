

#include "mb_task.h"
#include "tcpclient.h"


CMbeTask::CMbeTask()
{
	m_pTcpClient = std::make_shared<CTcpClient>();
}

CMbeTask::~CMbeTask(void)
{

}
 