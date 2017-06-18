
// TestACEDlg.h : ͷ�ļ�
//

#pragma once

#include "ace/Reactor.h"
#include "ace/TP_Reactor.h"

#include "connector.h"

#include "acereactor.h"
#include "handler.h"

#include "ClientUser.h"

// CTestACEDlg �Ի���
class CTestACEDlg : public CDialogEx
{
// ����
public:
	CTestACEDlg(CWnd* pParent = NULL);	// ��׼���캯��

public:
	bool StartPeer(char* strServerIP, int nServerPort);//�����ͻ���
	void  StopPeer();


// �Ի�������
	enum { IDD = IDD_TESTACE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��
//	ACE_Reactor* m_pReactor;
	//CConnector<CPeerHandler<ACE_SOCK_Stream>, ACE_SOCK_CONNECTOR> peer_connector;

	//CHandler* m_pHandler;

	//CAceReactor m_Reactor;

	CClientUser  m_client;
// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
