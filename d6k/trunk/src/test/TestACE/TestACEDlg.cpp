
// TestACEDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "TestACE.h"
#include "TestACEDlg.h"
#include "afxdialogex.h"


#include <ace/Init_ACE.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

class CAceInit
{
public:
	CAceInit()
	{
		ACE::init();
	}
	~CAceInit()
	{
		ACE::fini();
	}
};

//static CAceInit s_PreInit;

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CTestACEDlg 对话框



CTestACEDlg::CTestACEDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTestACEDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTestACEDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CTestACEDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CTestACEDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CTestACEDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CTestACEDlg::OnBnClickedButton3)
END_MESSAGE_MAP()


// CTestACEDlg 消息处理程序

BOOL CTestACEDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CTestACEDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CTestACEDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CTestACEDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
 

void CTestACEDlg::OnBnClickedButton1()
{
	// TODO:  在此添加控件通知处理程序代码
	//ACE_TP_Reactor* tp_reactor = new ACE_TP_Reactor;	
 //	m_pReactor = new ACE_Reactor(tp_reactor, 1);
// 	ACE_WFMO_Reactor wfmo_reactor;
// 	ACE_TP_Reactor  tp_reactor;
//	ACE_Reactor reactor(&wfmo_reactor);
//	ACE_Reactor reactor(&tp_reactor);

//	ACE_Reactor::instance(&reactor);
//	peer_connector.init(0, 0);

}

bool CTestACEDlg::StartPeer(char* strServerIP, int nServerPort)
{ 

	//m_pHandler = ::new CHandler;
	//CHandler h;
	
//	return m_Reactor.StartReactor();
	return true;
}

void CTestACEDlg::StopPeer()
{ 
//	m_Reactor.StopReactor();
}

void CTestACEDlg::OnBnClickedButton2()
{
	// TODO:  在此添加控件通知处理程序代码

//	ACE_TCHAR chAddr[] = ("127.0.0.1:20002");
//	peer_connector.init(0, NULL);

	m_client.Start();

}


void CTestACEDlg::OnBnClickedButton3()
{
	// TODO:  在此添加控件通知处理程序代码
}
