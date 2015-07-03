#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CSetServerIpPort �Ի���

class CSetServerIpPort : public CDialogEx
{
	DECLARE_DYNAMIC(CSetServerIpPort)

public:
	CSetServerIpPort(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSetServerIpPort();

// �Ի�������
	enum { IDD = IDD_SETSERVERIP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnIpnFieldchangedServeripaddress(NMHDR *pNMHDR, LRESULT *pResult);
	WSADATA wsaData;
	DWORD m_ServerIpAddr;
	short m_ServerPort;
	afx_msg void OnBnClickedOk();
	short m_ClientOwnPort;
	BOOL IsPortUsed(unsigned long);
};
