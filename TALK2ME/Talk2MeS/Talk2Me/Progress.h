#pragma once
#include "afxcmn.h"


// CProgress 对话框

class CProgress : public CDialogEx
{
	DECLARE_DYNAMIC(CProgress)

public:
	CProgress(long long range,CString tip,CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CProgress();
	void SetProgress(short step);//filename含有上传和下载字符
// 对话框数据
	enum { IDD = IDD_PROGRESS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CProgressCtrl m_ProCtrl;
	CString m_filename;
};
