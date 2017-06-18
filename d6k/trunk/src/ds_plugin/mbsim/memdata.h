#ifndef MEM_DATA_H
#define MEM_DATA_H 

#include <array>

#include <boost/thread/shared_mutex.hpp>

template<typename T>
class CMemDataItem
{
public:
	CMemDataItem()
	{
		m_nReadCount = 0;
		m_nWriteCount = 0;

		m_nValue = 0;
		m_nAddress = 0;
	}
	~CMemDataItem()
	{

	}
public:
	T GetValue()
	{
		m_nReadCount++;
		return m_nValue;
	}
		
	void SetValue(T nValue)
	{
		m_nWriteCount++;
		m_nValue = nValue;
	}

	void UISetValue(T nValue)
	{
		m_nValue = nValue;
	}

	unsigned int GetReadCount()
	{
		return m_nReadCount;
	}
	unsigned int GetWriteCount()
	{
		return m_nWriteCount;
	}
	
	void ResetCount()
	{
		m_nReadCount = 0;
		m_nWriteCount = 0;
	}
	void InitAddress(unsigned short nAddr)
	{
		m_nAddress = nAddr;
	}

	unsigned short GetAddress()
	{
		return m_nAddress;
	}

private:
	unsigned int m_nReadCount;
	unsigned int m_nWriteCount;

	T m_nValue;  //! ≤‚÷µ 
	unsigned short m_nAddress;//! µÿ÷∑
	unsigned int m_nIdx;
};

class CCoilOutput : public CMemDataItem<unsigned char>
{
public:
	CCoilOutput()
	{

	}
	~CCoilOutput()
	{

	}
private:

};

class CDigitalInput : public CMemDataItem<unsigned char>
{
public:
	CDigitalInput()
	{

	}
	~CDigitalInput()
	{

	}
private:

};


class CHoldingRegister : public CMemDataItem<unsigned short>
{
public:
	CHoldingRegister()
	{

	}
	~CHoldingRegister()
	{

	}
private:

};

class CAnalogueInput : public CMemDataItem<unsigned short>
{
public:
	CAnalogueInput()
	{

	}
	~CAnalogueInput()
	{

	}
private:

};
  

class CMemData
{
public:
	CMemData();
	~CMemData();
public:
	enum 
	{
		MAX_ITEM_NUM = 65535,
	};

public:
	unsigned char MBGetAddress()
	{
		return m_nMBAddress;
	}

	bool MBRegCoilsCB(unsigned char * pucRegBuffer, unsigned short usAddress, unsigned short usNCoils, bool bIsRead);
	bool MBRegHoldingCB(unsigned char * pucRegBuffer, unsigned short usAddress, unsigned short usNRegs, bool bIsRead);
	bool MBRegDiscreteCB(unsigned char * pucRegBuffer, unsigned short usAddress, unsigned short usNDiscrete);
	bool MBRegInputCB(unsigned char * pucRegBuffer, unsigned short usAddress, unsigned short usNRegs);

	unsigned char GetCoilValue(unsigned short nIdx);
	unsigned char GetDiscreteValue(unsigned short nIdx);

	unsigned short GetHoldingValue(unsigned short nIdx);
	unsigned short GetInputValue(unsigned short nIdx);

	void SetCoilValue(unsigned short nIdx, unsigned char nValue);
	void SetHoldingValue(unsigned short nIdx, unsigned short nValue);

	unsigned short GetCoilAddr(unsigned short nIdx);

protected:
	void InitMemData();
private:
	unsigned char  m_nMBAddress;

	std::array<CCoilOutput, MAX_ITEM_NUM> m_arrCoils;
	std::array<CDigitalInput, MAX_ITEM_NUM> m_arrDigitals;
	std::array<CHoldingRegister, MAX_ITEM_NUM> m_arrRegisters;
	std::array<CAnalogueInput, MAX_ITEM_NUM> m_arrAnalogues;

	boost::shared_mutex m_CoilMutex;
	boost::shared_mutex m_HoldingMutex;
// 	boost::shared_mutex m_DiscreteMutex;
// 	boost::shared_mutex m_InputMutex;
};

#endif // MEM_DATA_H
