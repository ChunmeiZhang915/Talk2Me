#pragma once


// CSetServerIpPort �Ի���

class CSetServerIpPort : public CDialogEx
{
	DECLARE_DYNAMIC(CSetServerIpPort)

public:
	CSetServerIpPort(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSetServerIpPort();

// �Ի�������
	enum { IDD = IDD_SETSERVERDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	WSADATA wsaData;
	DWORD m_ServerIpAddr;
	short m_ServerPort;
	afx_msg void OnBnClickedOk();
	BOOL IsPortUsed(unsigned long);
};
