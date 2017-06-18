 

#ifndef MB_TASK_H
#define MB_TASK_H

#include <memory>


 

class CTcpClient;

class CMbeTask
{
public:
	CMbeTask(void);
	~CMbeTask(void);
public:
	
	 
public: 

private:
	std::shared_ptr<CTcpClient> m_pTcpClient;
 
};


#endif //MB_TASK_H

