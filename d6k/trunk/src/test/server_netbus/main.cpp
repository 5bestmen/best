#include "server_netbus.h"
#include <QtWidgets/QApplication>

#ifndef int32u
typedef unsigned long  int32u;

#endif
const int32u MAX_EMSG_L = (399 * 1024);

enum EMSG_PATH
{
	TO_SERVER = 1,      //!< 发送到服务器
	TO_FES = 2,         //!< 发送到前置
	TO_MASTER_FES = 3,
	TO_CLIENT = 4,
	TO_PEER = 5,        //!< 发送到伙伴，对侧冗余节点
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
*   \brief 以太网报文 */
class CMsgHead
{
public:
	int32u  MsgPath;        //! 消息路线

	int32u  MsgType;        //! 消息类型
	int32u  SrcOccNo;       //! 源节点的OccNo

	int32u  MsgDataSize;    //! MsgData 中的有效长度
	int32u  FuncCode;
	int32u  SeqNum;         //! 序号，依次累加

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

