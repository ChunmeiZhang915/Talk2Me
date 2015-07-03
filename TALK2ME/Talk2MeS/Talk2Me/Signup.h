#pragma once
#include "afxdialogex.h"

#include "Talk2Me.h"
#include "afxcmn.h"

// CSignup 对话框

class CSignup : public CDialogEx
{
	DECLARE_DYNAMIC(CSignup)

public:
	CSignup(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSignup();

// 对话框数据
	enum { IDD = IDD_SIGNUP_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	bool IsValidInput(CString);
	bool m_test;
	CString m_username;
	CString m_pwd1;
	CString m_pwd;
};
