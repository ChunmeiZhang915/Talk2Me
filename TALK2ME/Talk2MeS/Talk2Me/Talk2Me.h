// Talk2Me.h
#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号


// CTalk2MeApp:


class CTalk2MeApp : public CWinApp
{
public:
	CTalk2MeApp();

// 重写
public:
	virtual BOOL InitInstance();

// 实现

	DECLARE_MESSAGE_MAP()
};

extern CTalk2MeApp theApp;