#pragma once

#include "datatypes.h"
#include <vector>
#include <memory>
#include <string>

class C104Master;
class C104Channel;

class C104Main
{
public:
	C104Main(int32u nOccNo);
	~C104Main();
public:
	bool Start();
	void Run();
	void Stop();
public:
	int32u  GetOccNo()
	{
		return m_nOccNo;
	}
private:
	int32u m_nOccNo;
	std::shared_ptr <C104Master>  m_p104;
	std::shared_ptr <C104Channel> m_pDataUnit;
};

