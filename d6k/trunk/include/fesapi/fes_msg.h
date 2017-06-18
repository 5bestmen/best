/*! @file mem_db.h
<PRE>
********************************************************************************
ģ����       :
�ļ���       :  mem_db.h
�ļ�ʵ�ֹ��� :  ʵʱ���ݿ�ģ��
����         :  LiJin
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         :  ��Ҫע���Ƿ���һ�����̿ռ���
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

********************************************************************************
</PRE>
*  @brief   ʵʱ���ݿ�ģ��
*  @author  LiJin
*  @version 1.0
*  @date    2016.09.16
*******************************************************************************/
#ifndef FES_MSG_DEF_H
#define FES_MSG_DEF_H

#include "fesdatadef.h"

enum MSG_TYPES
{
	MT_AIN,
	MT_DIN,
	MT_SOE,
	MT_NET,
	MT_SYS,
	MT_CTRL,
	MT_MAX,
};

struct MSG_BASE
{
	int16u Type;
	int16u Len;
	TIMEPAK Time;
};


//ģ����Խ�ޡ����ޡ��ݶ���
struct MSG_LIMIT : public MSG_BASE
{
	int32u FesOccNo; //ǰ��OccNo
	int32u AinOccNo; //���OccNo
	int32u LimitType;//Խ������
	
	fp64 AinValue;   //��ʱ��ֵ
};

//��������λ
struct MSG_BREAK : public MSG_BASE
{
	int32u FesOccNo; //ǰ��OccNo
	int32u DinOccNo; //���OccNo

	int8u DinValue;   //��ʱ��ֵ
};

//SOE
struct MSG_SOE : public MSG_BASE
{
	int32u FesOccNo; //ǰ��OccNo
	int32u DinOccNo; //���OccNo

	int8u DinValue;   //��ʱ��ֵ
};


//ң������
struct MSG_CTRL : public MSG_BASE
{
public:
	int32u OccNo;//ң�ص�OccNo

public:
	int32u UserOccNo;

public:
	int32u CommandType;//ң����������,45,46,48,49,50?

public:
	int32u DataType;//ģ����?������?
	fp64   Value;    //��ֵ

public:
	int  CtrlType; // ң������(���٣�����ȷ�ϣ���ʱ,��ʱȷ�ϣ�Ԥ�ã�Ԥ��ȷ�ϣ�ִ�У�ִ��ȷ�ϣ�����������ȷ��....)
};





struct MSG_EVENT : public MSG_BASE
{
	int32u	EvtCode;    //! ������Ϣ����
	int32u	Occno;

	int16u	AlarmFlag;         //���λΪ1��������,Ϊ0������
	int16u  AlarmState;        //�Կ�����������;��ģ����,
	IO_VARIANT	Value;
};

struct MSG_SETVAL : public MSG_BASE
{
	int8u	Att;
	int16u  Pad1;

	int32u	NodeOccNo;
	int32u	Occno;

	int8u	Datatype;
	int8u  Pad2;     /*MSG_SETVLUDONE 0:ѡ������ɹ� 1:ѡ�����ʧ�� */
	int8u  Source1; /*** see lcucnst.h ***/
	int8u  Source2;
	IO_VARIANT	Value[2];/*CTRL: 1ִ�У�2ѡ��3ѡ���ִ�У�4����*/
};


enum FE_MCODE
{
	MT_SETVALUE = 0X03,       //!< ��ֵ
	MT_SETVALUE_DONE = 0X04,  //!< ��ֵ���
	MT_NORMAL_EVENT = 0X50,   //!< ��ͨ�¼�
	MT_OPER  = 0X5A,    //!< ���Ʋ�������
	MT_DIAG  = 0X5B,    //!< �������Ϣ
	MT_RELAY = 0X5D,    //!< �����¼�
	MT_RELAY_EXT= 0X5E, //!< ������չ�¼� 
	MT_RELAY_DIAG = 0X5F, //!< ���������
};

struct FES_CMD
{

};

#endif // FES_MSG_DEF_H


/** @}*/

