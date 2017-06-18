#include "server_netbus.h"
#include <QtWidgets/QApplication>

#ifndef int32u
typedef unsigned long  int32u;

#endif
const int32u MAX_EMSG_L = (399 * 1024);

enum EMSG_PATH
{
	TO_SERVER = 1,      //!< ���͵�������
	TO_FES = 2,         //!< ���͵�ǰ��
	TO_MASTER_FES = 3,
	TO_CLIENT = 4,
	TO_PEER = 5,        //!< ���͵���飬�Բ�����ڵ�
	TO_SELF = 6,
};


class CMsgBody
{
private:
	char* m_pMsg;

public:
	CMsgBody()
	{
		m_pMsg = NULL;
	}

	CMsgBody(unsigned long len)
	{
		m_pMsg = NULL;
		//Init(len);
	}

	~CMsgBody()
	{
	}

	/*void Init(unsigned long len)
	{
		m_pMsg = new char[len + 1];
	}

	void Fini()
	{
		if (m_pMsg != NULL)
		{
			delete[] m_pMsg;
			m_pMsg = NULL;
		}
	}

	char* GetMsg()
	{
		return m_pMsg;
	}

	CMsgHead* GetHead()
	{
		if (m_pMsg == NULL)
		{
			return NULL;
		}

		return (CMsgHead*)m_pMsg;
	}

	char* GetData()
	{
		if (m_pMsg == NULL)
		{
			return NULL;
		}

		return m_pMsg + sizeof(CMsgHead);
	}

	int32u GetLen()
	{
		return GetHead()->MsgDataSize;
	}*/

private:

};

/*! \struct  EMSG_BUF
*   \brief ��̫������ */
class CMsgHead
{
public:
	int32u  MsgPath;        //! ��Ϣ·��

	int32u  MsgType;        //! ��Ϣ����
	int32u  SrcOccNo;       //! Դ�ڵ��OccNo

	int32u  MsgDataSize;    //! MsgData �е���Ч����
	int32u  FuncCode;
	int32u  SeqNum;         //! ��ţ������ۼ�

	void* data;

	CMsgBody msg;
	//unsigned char MsgData[MAX_EMSG_L];
};
#include <string>
int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	//int m = sizeof(CMsgBody);
	//int l = sizeof(int32u);
	//int len = sizeof(CMsgHead);

	char buf[10] = {0};
	buf[0] = '1';
	buf[1] = '2';
	buf[2] = '2';

	char* p = "123";
	char* temp = p;
	p = new char[5+1];
	p[0] = '0';
	p[1] = '0';
	strcpy(p+2, temp);

	server_netbus w;
	w.show();
	return a.exec();
}

