
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
	//! 注册通道
	bool RegisterChannel(int32u nOccNo);
	//! 启动通信
	bool Start(int32u nOccNo);
	//! 停止通信
	bool Stop(int32u nOccNo); 
protected:
 
private:
	 
	//! 本驱动的通道
	std::vector<std::shared_ptr<CIoChannel>>  m_arrChannels;
 
};


#endif // FESLIB_H

