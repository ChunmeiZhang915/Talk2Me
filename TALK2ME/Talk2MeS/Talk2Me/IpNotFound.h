#pragma once


// CIPNotFound 对话框

class CIPNotFound : public CDialogEx
{
	DECLARE_DYNAMIC(CIPNotFound)

public:
	CIPNotFound(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CIPNotFound();



protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	HICON m_hIcon;
	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnStnClickedIpnotfoundtipStatic();
};
