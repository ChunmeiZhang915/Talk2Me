#pragma once
#include"header.h"
#include "afxdialogex.h"

#include "Talk2Me.h"
#include "afxcmn.h"

using namespace std;
// CConversation 对话框
class CConversation : public CDialogEx
{
	DECLARE_DYNAMIC(CConversation)

public:
	CConversation(int id, CString title, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CConversation();

// 对话框数据
	enum { IDD = IDD_COVERSATION };
	

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual void OnClose();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedSendfilebtn();//点击发送文件
	afx_msg void OnBnClickedSenfcontentbtn();//点击发送消息
	
	CString m_history;//历史记录
	CString m_content;//消息内容
	CString m_filePath;//文件路径
	CString m_fileEx;//文件名
	
	CRichEditCtrl m_Scroll;
	CString m_title;//聊天对象的昵称，显示在对话框title部分
	int m_fId;//聊天对象的ID
	
};
