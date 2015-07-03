#pragma once
#include<string.h>
#include"ChatRoom.h"
using namespace std;

// CChatroomIn 对话框

class CChatroomIn : public CDialogEx
{
	DECLARE_DYNAMIC(CChatroomIn)

public:
	CChatroomIn(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CChatroomIn();
	int m_cId;
	int m_ownerId;//当前listbox选中的聊天室id和建立者id
// 对话框数据
	enum { IDD = IDD_CHATDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLbnDblclkAllchatrooms();
};
