
// TestZmq2.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CTestZmq2App: 
// �йش����ʵ�֣������ TestZmq2.cpp
//

class CTestZmq2App : public CWinApp
{
public:
	CTestZmq2App();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CTestZmq2App theApp;