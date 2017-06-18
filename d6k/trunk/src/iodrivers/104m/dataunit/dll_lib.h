
#ifndef DLL_LIB_H
#define DLL_LIB_H


#include "datatypes.h"

#include <vector>
#include <memory>

class CIoChannel;
class C104Main;

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

	std::vector < std::shared_ptr<C104Main>> m_arr104Channels;

};


#endif // FESLIB_H

