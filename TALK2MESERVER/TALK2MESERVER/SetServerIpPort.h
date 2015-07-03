#pragma once


// CSetServerIpPort 对话框

class CSetServerIpPort : public CDialogEx
{
	DECLARE_DYNAMIC(CSetServerIpPort)

public:
	CSetServerIpPort(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSetServerIpPort();

// 对话框数据
	enum { IDD = IDD_SETSERVERDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	WSADATA wsaData;
	DWORD m_ServerIpAddr;
	short m_ServerPort;
	afx_msg void OnBnClickedOk();
	BOOL IsPortUsed(unsigned long);
};
