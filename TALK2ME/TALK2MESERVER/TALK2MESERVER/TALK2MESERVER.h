#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������

class CTALK2MESERVERApp : public CWinApp
{
public:
	CTALK2MESERVERApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��
	DECLARE_MESSAGE_MAP()
};

extern CTALK2MESERVERApp theApp;