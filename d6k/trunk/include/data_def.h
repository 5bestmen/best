/*! @file
<PRE>
********************************************************************************
模块名       :
文件名       :
文件实现功能 :   
作者         :  LiJin
版本         :  V1.00
--------------------------------------------------------------------------------
备注         : <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief   
*  @author LiJin
*  @version 1.0
*  @date    2007.09.11
*/


#if !defined(DATA_DEF_H)
#define DATA_DEF_H

//#pragma once
#if 0
#define IO_YES            1
#define IO_NO             0

#define IO_INITED         1
#define IO_UNINITED       0   
#define IO_DEFAULT_INIT   IO_UNINITED

#define IO_SCAN_IN        1   //测点投入
#define IO_SCAN_OUT       0   //测点退出
#define DEFAULT_SCAN      IO_SCAN_IN
#define DEFAULT_SCANTIME 10
#endif



//! 定义过程值或者事件的品质 分为3类，good, uncertain和bad
// Values for QUALITY_MASK bit field
#define    IO_QUA_BAD             0x00
#define    IO_QUA_UNCERTAIN       0x40  //! 没有指定原因说明值为什么不确定
#define    IO_QUA_GOOD            0xC0


// STATUS_MASK Values for Quality = BAD
//
#define    IO_QUA_CONFIG_ERROR    0x04
#define    IO_QUA_NOT_CONNECTED   0x08
#define    IO_QUA_DEVICE_FAILURE  0x0c
#define    IO_QUA_SENSOR_FAILURE  0x10
#define    IO_QUA_LAST_KNOWN      0x14	//! 通讯失败。最后的值是可用的。
#define    IO_QUA_COMM_FAILURE    0x18
#define    IO_QUA_OUT_OF_SERVICE  0x1C

// STATUS_MASK Values for Quality = UNCERTAIN
//
#define    IO_QUA_LAST_USABLE     0x44  //! 最后的可用值。
#define    IO_QUA_SENSOR_CAL      0x50  //! 传感器达到了它的一个限值或者超过了它的量程
#define    IO_QUA_EGU_EXCEEDED    0x54  //! 返回值越限
#define    IO_QUA_SUB_NORMAL      0x58  //! 值有几个源，并且可用的源少于规定的品质好的源。

// STATUS_MASK Values for Quality = GOOD
// 值被覆盖。典型意思为输入失去连接和手动被强制。
#define    IO_QUA_LOCAL_OVERRIDE  0xD8



/** @}*/

#endif
