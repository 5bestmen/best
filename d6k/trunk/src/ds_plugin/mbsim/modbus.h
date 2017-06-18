#ifndef MODBUS_H
#define MODBUS_H 

#include <functional>

#include "mbtcp_def.h"

/* ----------------------- Defines ------------------------------------------*/
#define MB_PDU_FUNC_READ_ADDR_OFF           ( MB_PDU_DATA_OFF )
#define MB_PDU_FUNC_READ_COILCNT_OFF        ( MB_PDU_DATA_OFF + 2 )
#define MB_PDU_FUNC_READ_SIZE               ( 4 )
#define MB_PDU_FUNC_READ_COILCNT_MAX        ( 0x07D0 )

#define MB_PDU_FUNC_WRITE_ADDR_OFF          ( MB_PDU_DATA_OFF )
#define MB_PDU_FUNC_WRITE_VALUE_OFF         ( MB_PDU_DATA_OFF + 2 )
#define MB_PDU_FUNC_WRITE_SIZE              ( 4 )

#define MB_PDU_FUNC_WRITE_MUL_ADDR_OFF      ( MB_PDU_DATA_OFF )
#define MB_PDU_FUNC_WRITE_MUL_COILCNT_OFF   ( MB_PDU_DATA_OFF + 2 )
#define MB_PDU_FUNC_WRITE_MUL_BYTECNT_OFF   ( MB_PDU_DATA_OFF + 4 )
#define MB_PDU_FUNC_WRITE_MUL_VALUES_OFF    ( MB_PDU_DATA_OFF + 5 )
#define MB_PDU_FUNC_WRITE_MUL_SIZE_MIN      ( 5 )
#define MB_PDU_FUNC_WRITE_MUL_COILCNT_MAX   ( 0x07B0 )

 
/* ----------------------- Defines ------------------------------------------*/
//#define MB_PDU_FUNC_READ_ADDR_OFF               ( MB_PDU_DATA_OFF + 0)
#define MB_PDU_FUNC_READ_REGCNT_OFF             ( MB_PDU_DATA_OFF + 2 )
#define MB_PDU_FUNC_READ_SIZE                   ( 4 )
#define MB_PDU_FUNC_READ_REGCNT_MAX             ( 0x007D )

//#define MB_PDU_FUNC_WRITE_ADDR_OFF              ( MB_PDU_DATA_OFF + 0)
#define MB_PDU_FUNC_WRITE_VALUE_OFF             ( MB_PDU_DATA_OFF + 2 )
#define MB_PDU_FUNC_WRITE_SIZE                  ( 4 )

//#define MB_PDU_FUNC_WRITE_MUL_ADDR_OFF          ( MB_PDU_DATA_OFF + 0 )
#define MB_PDU_FUNC_WRITE_MUL_REGCNT_OFF        ( MB_PDU_DATA_OFF + 2 )
#define MB_PDU_FUNC_WRITE_MUL_BYTECNT_OFF       ( MB_PDU_DATA_OFF + 4 )
#define MB_PDU_FUNC_WRITE_MUL_VALUES_OFF        ( MB_PDU_DATA_OFF + 5 )
#define MB_PDU_FUNC_WRITE_MUL_SIZE_MIN          ( 5 )
#define MB_PDU_FUNC_WRITE_MUL_REGCNT_MAX        ( 0x0078 )

#define MB_PDU_FUNC_READWRITE_READ_ADDR_OFF     ( MB_PDU_DATA_OFF + 0 )
#define MB_PDU_FUNC_READWRITE_READ_REGCNT_OFF   ( MB_PDU_DATA_OFF + 2 )
#define MB_PDU_FUNC_READWRITE_WRITE_ADDR_OFF    ( MB_PDU_DATA_OFF + 4 )
#define MB_PDU_FUNC_READWRITE_WRITE_REGCNT_OFF  ( MB_PDU_DATA_OFF + 6 )
#define MB_PDU_FUNC_READWRITE_BYTECNT_OFF       ( MB_PDU_DATA_OFF + 8 )
#define MB_PDU_FUNC_READWRITE_WRITE_VALUES_OFF  ( MB_PDU_DATA_OFF + 9 )
#define MB_PDU_FUNC_READWRITE_SIZE_MIN          ( 9 )


#define MB_PDU_FUNC_READ_DISCCNT_OFF        ( MB_PDU_DATA_OFF + 2 )
#define MB_PDU_FUNC_READ_SIZE               ( 4 )
#define MB_PDU_FUNC_READ_DISCCNT_MAX        ( 0x07D0 )


#define MB_PDU_FUNC_READ_REGCNT_OFF         ( MB_PDU_DATA_OFF + 2 )
#define MB_PDU_FUNC_READ_SIZE               ( 4 )
#define MB_PDU_FUNC_READ_REGCNT_MAX         ( 0x007D )

#define MB_PDU_FUNC_READ_RSP_BYTECNT_OFF    ( MB_PDU_DATA_OFF )

/*!
* Constants which defines the format of a modbus frame. The example is
* shown for a Modbus RTU/ASCII frame. Note that the Modbus PDU is not
* dependent on the underlying transport.
*
* <code>
* <------------------------ MODBUS SERIAL LINE PDU (1) ------------------->
*              <----------- MODBUS PDU (1') ---------------->
*  +-----------+---------------+----------------------------+-------------+
*  | Address   | Function Code | Data                       | CRC/LRC     |
*  +-----------+---------------+----------------------------+-------------+
*  |           |               |                                   |
* (2)        (3/2')           (3')                                (4)
*
* (1)  ... MB_SER_PDU_SIZE_MAX = 256
* (2)  ... MB_SER_PDU_ADDR_OFF = 0
* (3)  ... MB_SER_PDU_PDU_OFF  = 1
* (4)  ... MB_SER_PDU_SIZE_CRC = 2
*
* (1') ... MB_PDU_SIZE_MAX     = 253
* (2') ... MB_PDU_FUNC_OFF     = 0
* (3') ... MB_PDU_DATA_OFF     = 1
* </code>
*/

/* ----------------------- Defines ------------------------------------------*/
#define MB_PDU_SIZE_MAX     253 /*!< Maximum size of a PDU. */
#define MB_PDU_SIZE_MIN     1   /*!< Function Code */
#define MB_PDU_FUNC_OFF     0   /*!< Offset of function code in PDU. */
#define MB_PDU_DATA_OFF     1   /*!< Offset for response data in PDU. */

/* ----------------------- Defines ------------------------------------------*/
#define MB_ADDRESS_BROADCAST    ( 0 )   /*! Modbus broadcast address. */
#define MB_ADDRESS_MIN          ( 1 )   /*! Smallest possible slave address. */
#define MB_ADDRESS_MAX          ( 247 ) /*! Biggest possible slave address. */

#define MB_FUNC_NONE                          (  0 )
#define MB_FUNC_READ_COILS                    (  1 )
#define MB_FUNC_READ_DISCRETE_INPUTS          (  2 )
#define MB_FUNC_WRITE_SINGLE_COIL             (  5 )
#define MB_FUNC_WRITE_MULTIPLE_COILS          ( 15 )
#define MB_FUNC_READ_HOLDING_REGISTER         (  3 )
#define MB_FUNC_READ_INPUT_REGISTER           (  4 )
#define MB_FUNC_WRITE_REGISTER                (  6 )
#define MB_FUNC_WRITE_MULTIPLE_REGISTERS      ( 16 )
#define MB_FUNC_READWRITE_MULTIPLE_REGISTERS  ( 23 )
#define MB_FUNC_DIAG_READ_EXCEPTION           (  7 )
#define MB_FUNC_DIAG_DIAGNOSTIC               (  8 )
#define MB_FUNC_DIAG_GET_COM_EVENT_CNT        ( 11 )
#define MB_FUNC_DIAG_GET_COM_EVENT_LOG        ( 12 )
#define MB_FUNC_OTHER_REPORT_SLAVEID          ( 17 )
#define MB_FUNC_ERROR                         ( 128 )
 

class CModbusImpl
{
public:
	CModbusImpl();
	~CModbusImpl();
public:
	/*! \ingroup modbus
	* \brief Modbus serial transmission modes (RTU/ASCII).
	*
	* Modbus serial supports two transmission modes. Either ASCII or RTU. RTU
	* is faster but has more hardware requirements and requires a network with
	* a low jitter. ASCII is slower and more reliable on slower links (E.g. modems)
	*/
	enum MBMode
	{
		MB_RTU,                     /*!< RTU transmission mode. */
		MB_ASCII,                   /*!< ASCII transmission mode. */
		MB_TCP                      /*!< TCP mode. */
	};

	/*! \ingroup modbus
	* \brief If register should be written or read.
	*
	* This value is passed to the callback functions which support either
	* reading or writing register values. Writing means that the application
	* registers should be updated and reading means that the modbus protocol
	* stack needs to know the current register values.
	*
	* \see MBRegHoldingCB( ), MBRegCoilsCB( ), MBRegDiscreteCB( ) and
	*   MBRegInputCB( ).
	*/
	enum MBRegisterMode
	{
		MB_REG_READ,                /*!< Read register values and pass to protocol stack. */
		MB_REG_WRITE                /*!< Update register values. */
	};

	/*! \ingroup modbus
	* \brief Errorcodes used by all function in the protocol stack.
	*/
	enum MBErrorCode
	{
		MB_ENOERR,                  /*!< no error. */
		MB_ENOREG,                  /*!< illegal register address. */
		MB_EINVAL,                  /*!< illegal argument. */
		MB_EPORTERR,                /*!< porting layer error. */
		MB_ENORES,                  /*!< insufficient resources. */
		MB_EIO,                     /*!< I/O error. */
		MB_EILLSTATE,               /*!< protocol stack in illegal state. */
		MB_ETIMEDOUT                /*!< timeout error occurred. */
	};

	enum MBException
	{
		MB_EX_NONE = 0x00,
		MB_EX_ILLEGAL_FUNCTION = 0x01,
		MB_EX_ILLEGAL_DATA_ADDRESS = 0x02,
		MB_EX_ILLEGAL_DATA_VALUE = 0x03,
		MB_EX_SLAVE_DEVICE_FAILURE = 0x04,
		MB_EX_ACKNOWLEDGE = 0x05,
		MB_EX_SLAVE_BUSY = 0x06,
		MB_EX_MEMORY_PARITY_ERROR = 0x08,
		MB_EX_GATEWAY_PATH_FAILED = 0x0A,
		MB_EX_GATEWAY_TGT_FAILED = 0x0B
	};
public:
	// 回调
	std::function <	bool(unsigned char * pucRegBuffer, unsigned short usAddress, unsigned short usNCoils, bool bIsRead)> m_fnRegCoils;
	std::function <	bool(unsigned char * pucRegBuffer, unsigned short usAddress, unsigned short usNRegs, bool bIsRead)> m_fnRegHolding;
	std::function <	bool(unsigned char * pucRegBuffer, unsigned short usAddress, unsigned short usNDiscrete)> m_fnRegDiscrete;
	std::function <	bool(unsigned char * pucRegBuffer, unsigned short usAddress, unsigned short usNRegs)> m_fnRegInput;

	std::function<unsigned char()> m_fnGetAddress;
	std::function<std::string ()> m_fnGetRemoteEndPoint;
public:
	bool MBPoll(unsigned char * pucFrame, unsigned short * usLen);

protected:
	// 线圈
	MBException MBFuncReadCoils(unsigned char * pucFrame, unsigned short * usLen);
	MBException MBFuncWriteCoil(unsigned char * pucFrame, unsigned short * usLen);
	MBException MBFuncWriteMultipleCoils(unsigned char * pucFrame, unsigned short * usLen);
	// 离散输入
	MBException MBFuncReadDiscreteInputs(unsigned char * pucFrame, unsigned short * usLen);
	// 保持型寄存器
	MBException MBFuncReadHoldingRegister(unsigned char * pucFrame, unsigned short * usLen);

	MBException MBFuncWriteHoldingRegister(unsigned char * pucFrame, unsigned short * usLen);
	MBException MBFuncWriteMultipleHoldingRegister(unsigned char * pucFrame, unsigned short * usLen);
	MBException MBFuncReadWriteMultipleHoldingRegister(unsigned char * pucFrame, unsigned short * usLen);
	// 输入寄存器
	MBException MBFuncReadInputRegister(unsigned char * pucFrame, unsigned short * usLen);

	bool MBRegCoilsCallBack(unsigned char * pucRegBuffer, unsigned short usAddress, unsigned short usNCoils, bool bIsRead);
	bool MBRegHoldingCallBack(unsigned char * pucRegBuffer, unsigned short usAddress, unsigned short usNRegs, bool bIsRead);

	bool MBRegInputCallBack(unsigned char * pucRegBuffer, unsigned short usAddress, unsigned short usNRegs);
	bool MBRegDiscreteCallBack(unsigned char * pucRegBuffer, unsigned short usAddress, unsigned short usNDiscrete);

	MBErrorCode  MBRegCoilsCB(unsigned char  * pucRegBuffer, unsigned short usAddress,unsigned short usNCoils, MBRegisterMode eMode);
	MBErrorCode  MBRegHoldingCB(unsigned char * pucRegBuffer, unsigned short usAddress,	unsigned short usNRegs, MBRegisterMode eMode);

	MBErrorCode  MBRegInputCB(unsigned char * pucRegBuffer, unsigned short usAddress,	unsigned short usNRegs);
	MBErrorCode  MBRegDiscreteCB(unsigned char * pucRegBuffer, unsigned short usAddress,	unsigned short usNDiscrete);

	std::string GetClient()
	{
		if (m_fnGetRemoteEndPoint)
		{
			return  m_fnGetRemoteEndPoint();
		}
		return "";
	}
	void MBLogString(const char *szLogTxt, int nLevel);
private:
	MBException MBError2Exception(MBErrorCode eErrorCode);
};


#endif // MODBUS_H
