#pragma once
#include "afxdialogex.h"

#include "Talk2Me.h"
#include "afxcmn.h"

// CSignup �Ի���

class CSignup : public CDialogEx
{
	DECLARE_DYNAMIC(CSignup)

public:
	CSignup(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSignup();

// �Ի�������
	enum { IDD = IDD_SIGNUP_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	bool IsValidInput(CString);
	bool m_test;
	CString m_username;
	CString m_pwd1;
	CString m_pwd;
};
