/*! @file
<PRE>
********************************************************************************
ģ����       :
�ļ���       :
�ļ�ʵ�ֹ��� :   
����         :  LiJin
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         : <����˵��>
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

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

#define IO_SCAN_IN        1   //���Ͷ��
#define IO_SCAN_OUT       0   //����˳�
#define DEFAULT_SCAN      IO_SCAN_IN
#define DEFAULT_SCANTIME 10
#endif



//! �������ֵ�����¼���Ʒ�� ��Ϊ3�࣬good, uncertain��bad
// Values for QUALITY_MASK bit field
#define    IO_QUA_BAD             0x00
#define    IO_QUA_UNCERTAIN       0x40  //! û��ָ��ԭ��˵��ֵΪʲô��ȷ��
#define    IO_QUA_GOOD            0xC0


// STATUS_MASK Values for Quality = BAD
//
#define    IO_QUA_CONFIG_ERROR    0x04
#define    IO_QUA_NOT_CONNECTED   0x08
#define    IO_QUA_DEVICE_FAILURE  0x0c
#define    IO_QUA_SENSOR_FAILURE  0x10
#define    IO_QUA_LAST_KNOWN      0x14	//! ͨѶʧ�ܡ�����ֵ�ǿ��õġ�
#define    IO_QUA_COMM_FAILURE    0x18
#define    IO_QUA_OUT_OF_SERVICE  0x1C

// STATUS_MASK Values for Quality = UNCERTAIN
//
#define    IO_QUA_LAST_USABLE     0x44  //! ���Ŀ���ֵ��
#define    IO_QUA_SENSOR_CAL      0x50  //! �������ﵽ������һ����ֵ���߳�������������
#define    IO_QUA_EGU_EXCEEDED    0x54  //! ����ֵԽ��
#define    IO_QUA_SUB_NORMAL      0x58  //! ֵ�м���Դ�����ҿ��õ�Դ���ڹ涨��Ʒ�ʺõ�Դ��

// STATUS_MASK Values for Quality = GOOD
// ֵ�����ǡ�������˼Ϊ����ʧȥ���Ӻ��ֶ���ǿ�ơ�
#define    IO_QUA_LOCAL_OVERRIDE  0xD8



/** @}*/

#endif
