
// TestZmq1.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CTestZmq1App: 
// �йش����ʵ�֣������ TestZmq1.cpp
//

class CTestZmq1App : public CWinApp
{
public:
	CTestZmq1App();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CTestZmq1App theApp;