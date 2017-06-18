
// TestZmq2Dlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TestZmq2.h"
#include "TestZmq2Dlg.h"
#include "afxdialogex.h"


#pragma comment(lib,"libzmq.lib")
#include "zmq.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CTestZmq2Dlg �Ի���



CTestZmq2Dlg::CTestZmq2Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTestZmq2Dlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTestZmq2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CTestZmq2Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CTestZmq2Dlg::OnBnClickedButton1)

	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CTestZmq2Dlg ��Ϣ�������

BOOL CTestZmq2Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������

	m_pCtx = zmq_ctx_new();
	ASSERT(m_pCtx);

	m_pRecver = zmq_socket(m_pCtx, ZMQ_PULL);
	//int nRet = zmq_bind(m_pRecver, "tcp://*:10002");
	int nRet = zmq_connect(m_pRecver, "tcp://localhost:10002");
	ASSERT(nRet == 0);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CTestZmq2Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CTestZmq2Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CTestZmq2Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CTestZmq2Dlg::OnBnClickedButton1()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������

	zmq_msg_t msg;
	int nRet = 0;
	nRet = zmq_msg_init(&msg);
	//zmq_msg_init_size(&msg, 1024);
// 	char data[] = "1234556789";
// 	zmq_msg_init_data(&msg, data, sizeof data, nullptr, nullptr);
// 
// 	int nRet = zmq_msg_send(&msg, m_pSock, ZMQ_DONTWAIT);

	nRet = zmq_msg_recv(&msg,m_pRecver, ZMQ_DONTWAIT);

	if (nRet == -1)
	{
		return;
	}

	unsigned char data[20] = { 0 };
	memcpy(data, zmq_msg_data(&msg), zmq_msg_size(&msg));

	zmq_msg_close(&msg);

}


void CTestZmq2Dlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	zmq_close(m_pRecver);
	zmq_ctx_destroy(m_pCtx);
	// TODO:  �ڴ˴������Ϣ����������
}