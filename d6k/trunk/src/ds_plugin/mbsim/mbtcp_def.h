#ifndef MBTCP_DEF_H
#define MBTCP_DEF_H
  
/* ----------------------- MBAP Header --------------------------------------*/
/*
*
* <------------------------ MODBUS TCP/IP ADU(1) ------------------------->
*              <----------- MODBUS PDU (1') ---------------->
*  +-----------+---------------+------------------------------------------+
*  | TID | PID | Length | UID  |Code | Data                               |
*  +-----------+---------------+------------------------------------------+
*  |     |     |        |      |
* (2)   (3)   (4)      (5)    (6)
*
* (2)  ... MB_TCP_TID          = 0 (Transaction Identifier - 2 Byte)
* (3)  ... MB_TCP_PID          = 2 (Protocol Identifier - 2 Byte)
* (4)  ... MB_TCP_LEN          = 4 (Number of bytes - 2 Byte)
* (5)  ... MB_TCP_UID          = 6 (Unit Identifier - 1 Byte)
* (6)  ... MB_TCP_FUNC         = 7 (Modbus Function Code)
*
* (1)  ... Modbus TCP/IP Application Data Unit
* (1') ... Modbus Protocol Data Unit
*/

#define MB_TCP_TID          0
#define MB_TCP_PID          2
#define MB_TCP_LEN          4
#define MB_TCP_UID          6
#define MB_TCP_FUNC         7

#define MB_TCP_PROTOCOL_ID  0   /* 0 = Modbus Protocol */

#define MB_TCP_BUF_SIZE     ( 256 + 7 ) /* Must hold a complete Modbus TCP frame. */

#endif // MBTCP_DEF_H
