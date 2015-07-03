#pragma once
#include"Conversation.h"

#include "afxcmn.h"

// CFileRecv �Ի���

class CFileRecv : public CDialogEx
{
	DECLARE_DYNAMIC(CFileRecv)

public:
	CFileRecv(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CFileRecv();

// �Ի�������
	enum { IDD = IDD_RECVDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedRecvfile();
	afx_msg void OnBnClickedNexttime();
	afx_msg void OnBnClickedCancel();
	long long m_fileLength;
	bool m_isChatFile;
	CString m_fileName;
	
	CString m_fromId;
};
