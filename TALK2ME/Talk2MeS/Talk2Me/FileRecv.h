#pragma once
#include"Conversation.h"

#include "afxcmn.h"

// CFileRecv 对话框

class CFileRecv : public CDialogEx
{
	DECLARE_DYNAMIC(CFileRecv)

public:
	CFileRecv(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CFileRecv();

// 对话框数据
	enum { IDD = IDD_RECVDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

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
