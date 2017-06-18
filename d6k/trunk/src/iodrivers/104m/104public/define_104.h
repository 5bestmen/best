
#ifndef _DEFINE_104_H
#define _DEFINE_104_H

#include "ace/Basic_Types.h"

#include "asdu_104.h"


//////////////////////////////////////////////////////////////////////////
class APCI
{
public:
	unsigned char  head;        //�����ַ�ͷ
	unsigned char  len;           //APDU����
	unsigned char  control0;   //������
	unsigned char  control1;   //������
	unsigned char  control2;   //������
	unsigned char  control3;   //������
public:
	int GetType()
	{
		//��λλ��1�����λΪ1
		if (control0 & 0x01)
		{       //�����ڶ�λΪ1
			if ((control0 >> 1) & 0x01)
				return TYPE_U;
			return TYPE_S;
		}
		return TYPE_I;
	}
	unsigned short GetSendNum()
	{
		return MYMAKE16(control0, control1) >> 1;
	}
	unsigned short GetRecvNum()
	{
		return MYMAKE16(control2, control3) >> 1;
	}
	void SetSendNum(unsigned short nSendNum)
	{
		unsigned short send_num = (nSendNum << 1) & 0xfffe;
		control0 = MYLO8(send_num);
		control1 = MYHI8(send_num);
	}
	void SetRecvNum(unsigned short nRecvNum)
	{
		unsigned short recv_num = (nRecvNum << 1) & 0xfffe;
		control2 = MYLO8(recv_num);
		control3 = MYHI8(recv_num);
	}
	int GetUType()
	{
		return control0;
	}
	int GetApduLength()
	{
		return len;
	}
};
int operator >> (ACE_InputCDR &cdr, APCI &apci);
int operator<< (ACE_OutputCDR &cdr, const APCI &apci);
//////////////////////////////////////////////////////////////////////////


#endif//_DEFINE_104_H
