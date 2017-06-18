
#ifndef DLL_LIB_H
#define DLL_LIB_H


#include "datatypes.h"

#include <vector>
#include <memory>
  
class CIoChannel;

class CIoDriverLib
{
public:
	CIoDriverLib(void);
	~CIoDriverLib(void);
public:
	//! ע��ͨ��
	bool RegisterChannel(int32u nOccNo);
	//! ����ͨ��
	bool Start(int32u nOccNo);
	//! ֹͣͨ��
	bool Stop(int32u nOccNo); 
protected:
 
private:
	 
	//! ��������ͨ��
	std::vector<std::shared_ptr<CIoChannel>>  m_arrChannels;
 
};


#endif // FESLIB_H

