
// TestZmq1Dlg.h : ͷ�ļ�
//

#pragma once


// CTestZmq1Dlg �Ի���
class CTestZmq1Dlg : public CDialogEx
{
// ����
public:
	CTestZmq1Dlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_TESTZMQ1_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��
protected:
	void * m_pCtx;
	void * m_pSender;


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
	afx_msg void OnClose();
	afx_msg void OnDestroy();
};
