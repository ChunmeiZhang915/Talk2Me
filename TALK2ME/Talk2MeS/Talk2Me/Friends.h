#pragma once
#include<string.h>
#include"Conversation.h"
#include "afxdialogex.h"

#include "Talk2Me.h"
#include "afxcmn.h"
using namespace std;
// CFriends 对话框

class CFriends : public CDialogEx
{
	DECLARE_DYNAMIC(CFriends)

public:
	CFriends(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CFriends();
	int m_id;
	char m_name[DB_LENGTH];//存取tab中选中人的id和昵称
// 对话框数据
	enum { IDD = IDD_FRIENDDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLbnDblclkLocalpartnerlist();
};
