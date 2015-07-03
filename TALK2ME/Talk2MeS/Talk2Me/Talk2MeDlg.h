
// Talk2MeDlg.h 
//

#pragma once



#include"SetServerIpPort.h"
#include"Signup.h"
#include"Login.h"
#include"ChatRoom.h"
#include "afxcmn.h"
#include"Friends.h"
#include"ChatroomIn.h"
#include"FileRecv.h"
#include"Progress.h"
#define DEBUG
#undef DEBUG



// CTalk2MeDlg 对话框
class CTalk2MeDlg : public CDialogEx
{

public:
	CTalk2MeDlg(CWnd* pParent = NULL);	// 标准构造函数
	map<int, CConversation*> m_id2Conversation;//存储121对话
	map<int, CChatRoom*> m_id2Chatroom;//存储聊天室
	vector<USER_LOGIN> m_onlineFriends;//注册用户
	map <CString, CString> m_File2FileName; //原始名字对现在的名字
	map<CString, CProgress*> m_File2ProgressBar;//文件名对进度条
	map<CString, CFileRecv*> m_file2SaveDlg;//文件名对保存对话框
	
	CSetServerIpPort m_serverSet;//设置服务器对话框
	CSignup m_signup;//注册对话框
	CLogin m_login;//登陆对话框
	CFriends m_friendTab;//注册好友tab
	CChatroomIn m_chatRoomTab;//聊天室tab
// 对话框数据
	enum { IDD = IDD_TALK2ME_DIALOG };
	SOCKET m_ClientSocket;
	
	int m_fileConversationId;//121文件传输,用于文件线程的发送,作为参数传到
	int m_fileChatRoomId;//聊天室文件传输
	WSADATA wsaData;
	int iLen;			// 客户地址长度
	int iSend;			// 发送数据的长度
	int iRecv;			// 接收数据的长度
	struct sockaddr_in m_myAddr,m_serverAddr;				// 本地地址和客户地址
	MSG0 m_sendMsg , m_recvMsg;
	int m_wantToTalkId;
	int m_myOwnId;//当前用户的id
	char m_username[DB_LENGTH];
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
	

// 实现
protected:
	HICON m_hIcon;
	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	virtual void OnClose();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	BOOL m_IsConnected;
	
	void GetMembersFromTheChatRoom(int cid);//获得当前用户参与的聊天室的所有成员情况
	afx_msg void OnBnClickedStarttalkButton();
	afx_msg void OnBnClickedLoginbtn();
	afx_msg void OnBnClickedSetbtn();
	afx_msg LRESULT OnRecvFileSave(WPARAM wParam, LPARAM lParam);//接收文件提示框
	afx_msg LRESULT OnSendFile(WPARAM wParam, LPARAM lParam);//发送121文件
	afx_msg LRESULT OnSendMsg(WPARAM wParam, LPARAM lParam);//发送121消息
	afx_msg LRESULT OnRecvChatMsg(WPARAM wParam, LPARAM lParam);//接收聊天室消息
	afx_msg LRESULT OnRecvMsg(WPARAM wParam, LPARAM lParam);//接收121消息
	afx_msg LRESULT OnCloseMsg(WPARAM wParam, LPARAM lParam);//关闭对话框
	afx_msg LRESULT OnSignUp(WPARAM wParam, LPARAM lParam);//注册
	afx_msg LRESULT OnAddOne(WPARAM wParam, LPARAM lParam);//增加聊天室成员
	afx_msg LRESULT OnDelOne(WPARAM wParam, LPARAM lParam);//删除聊天室成员
	afx_msg LRESULT OnSendChatMsg(WPARAM wParam, LPARAM lParam);//发送聊天室消息
	afx_msg LRESULT OnSendChatFile(WPARAM wParam, LPARAM lParam);//发送群发文件
	afx_msg LRESULT OnRecvFile(WPARAM wParam, LPARAM lParam);//接收文件
	afx_msg LRESULT OnCancelFile(WPARAM wParam, LPARAM lParam);//取消文件
	
	afx_msg void OnBnClickedSignupbtn();
	
	afx_msg void OnBnClickedChatroomBtn();
	afx_msg void OnBnClickedLogoutbtn();
	void OnBnClickedGetroombtn();//获得用户参与的所有的聊天室
	void OnBnClickedRefreshButton();//获得好友列表
	
	CTabCtrl m_tabCtrl;
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
};

