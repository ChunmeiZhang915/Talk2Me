#pragma once


// CIPNotFound �Ի���

class CIPNotFound : public CDialogEx
{
	DECLARE_DYNAMIC(CIPNotFound)

public:
	CIPNotFound(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CIPNotFound();



protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	HICON m_hIcon;
	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnStnClickedIpnotfoundtipStatic();
};
