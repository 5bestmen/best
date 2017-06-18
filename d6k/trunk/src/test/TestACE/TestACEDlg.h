
// TestACEDlg.h : 头文件
//

#pragma once

#include "ace/Reactor.h"
#include "ace/TP_Reactor.h"

#include "connector.h"

#include "acereactor.h"
#include "handler.h"

#include "ClientUser.h"

// CTestACEDlg 对话框
class CTestACEDlg : public CDialogEx
{
// 构造
public:
	CTestACEDlg(CWnd* pParent = NULL);	// 标准构造函数

public:
	bool StartPeer(char* strServerIP, int nServerPort);//开启客户端
	void  StopPeer();


// 对话框数据
	enum { IDD = IDD_TESTACE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
//	ACE_Reactor* m_pReactor;
	//CConnector<CPeerHandler<ACE_SOCK_Stream>, ACE_SOCK_CONNECTOR> peer_connector;

	//CHandler* m_pHandler;

	//CAceReactor m_Reactor;

	CClientUser  m_client;
// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
};
