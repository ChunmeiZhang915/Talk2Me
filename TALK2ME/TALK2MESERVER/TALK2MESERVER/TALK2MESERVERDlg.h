#pragma once

#include<map>
#include "afxwin.h"
#include "SetServerIpPort.h"
#include <direct.h>
#include <io.h>
using namespace std;
#define BUFFER_LENGTH 1024		// 接收数据缓冲区长度
#define DB_LENGTH 50 //数据库存储字符串的长度


#define MSG_LOGIN 0
#define MSG_LOGIN_RP 1
#define MSG_SEARCH 2
#define MSG_SEARCH_RP 3
#define MSG_SEARCH_ERROR 4
#define MSG_ABROAD 5
#define MSG_ABROAD_RP 6
#define MSG_FILE 7
#define MSG_FILETYPE 8
#define MSG_FILEEND 9
#define MSG_FILE_RP 10
#define MSG_LOGOUT 11
#define MSG_LOGOUT_RP 12
#define MSG_BROADCAST 13
#define MSG_BROADCAST_RP 14
#define MSG_TEXT 15
#define MSG_SIGNUP 16
#define MSG_SIGNUP_RP 17
#define MSG_CHATROOM 18
#define MSG_CHATROOM_RP 19
#define MSG_ADDONECHAT 20
#define MSG_ADDONECHAT_RP 21
#define MSG_DELETEONECHAT 22
#define MSG_DELETEONECHAT_RP 23
#define MSG_CHATTXT 24
#define MSG_CHATFILETYPE 25
#define MSG_CHATFILEEND 26
#define MSG_CHATBROADCAST 27
#define MSG_CHATBROADCAST_RP 28
#define MSG_SEARCHCHAT 29
#define MSG_SEARCHCHAT_RP 30
#define MSG_RECVFILE 31
#define MSG_CANCELFILE 32
#define MSG_NEXTTIME 33
#define MSG_RECVCHATFILE 34
#define MSG_CANCELCHATFILE 35
#define CO_THREAD_NUM 4

#define DEBUG
#undef DEBUG


#define ERRORMSG {LPVOID lpMsgBuf;\
		FormatMessage(\
			FORMAT_MESSAGE_ALLOCATE_BUFFER |\
			FORMAT_MESSAGE_FROM_SYSTEM |\
			FORMAT_MESSAGE_IGNORE_INSERTS,\
			NULL,\
			GetLastError(),\
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), \
			(LPTSTR) &lpMsgBuf,\
			0,\
			NULL\
		);\
		AfxMessageBox((LPCTSTR)lpMsgBuf, MB_OK|MB_ICONEXCLAMATION);\
		LocalFree( lpMsgBuf );}
struct LF{
	long long m_fileLength;//文件changdu
	char m_filename[DB_LENGTH];//文件名字
};
struct IP_PORT{
	DWORD m_ip;
	USHORT m_port;
};
struct USER_LOGIN{
	int m_id;
	char m_pwd[DB_LENGTH];
};
struct USER_INFO{//昵称和密码；昵称和登陆状态
	char m_name[DB_LENGTH];
	char m_pwd[DB_LENGTH];
};
struct MSG0{
	unsigned short m_msgType;
	char m_text[BUFFER_LENGTH];
};
struct TMSG{
	unsigned int m_idFrom;//一对一对话中的发送人Id
	char m_name[DB_LENGTH];//发送人的昵称
	unsigned int m_id;//消息要发送给的对方
	char m_text[BUFFER_LENGTH - DB_LENGTH - 2*sizeof(int)];//消息内容
};
struct FMSG{//一对一文件消息	
	char m_filename[DB_LENGTH];//发送文件名称
	char m_text[BUFFER_LENGTH -DB_LENGTH ];//消息内容
};
struct CTMSG{
	unsigned int m_idFrom;//发送人Id
	unsigned int m_cId;//聊天室的ID
	unsigned int m_ownerId;//聊天室的建立者ID
	char m_name[DB_LENGTH];//发送人的昵称
	char m_text[BUFFER_LENGTH - DB_LENGTH - 3*sizeof(int)];//消息内容
};
struct LTMSG{//一对一文件类型消息
	unsigned int m_idFrom;//一对一对话中的发送人Id
	char m_name[DB_LENGTH];//发送人的昵称
	unsigned int m_id;//消息要发送给的对方
	long long m_fileLength;//文件大小
	char m_text[BUFFER_LENGTH - DB_LENGTH - 2*sizeof(unsigned int) - sizeof(long long)];//文件名字
};
struct LCTMSG{//聊天室文件类型消息
	unsigned int m_idFrom;//发送人Id
	unsigned int m_cId;//聊天室的ID
	unsigned int m_ownerId;//聊天室的建立者ID
	char m_name[DB_LENGTH];//发送人的昵称
	long long m_fileLength;//文件大小
	char m_text[BUFFER_LENGTH - DB_LENGTH - 3*sizeof( unsigned int) - sizeof(long long)];//文件名字
};
// CTALK2MESERVERDlg 对话框
class CTALK2MESERVERDlg : public CDialogEx
{
// 构造
public:
	CTALK2MESERVERDlg(CWnd* pParent = NULL);	// 标准构造函数
	SOCKET m_ServerSocket;
	CSetServerIpPort m_ServerSet;
	map <int ,SOCKET> m_OnlineClients;
	WSADATA wsaData;
	int iLen;			// 客户地址长度
	int iSend;			// 发送数据的长度
	int iRecv;			// 接收数据的长度
	
	struct sockaddr_in m_serverAddr, m_clientAddrTo, m_clientAddrFrom;				// 本地地址和客户地址
	MSG0 m_sendMsg , m_recvMsg;
	
	enum { IDD = IDD_TALK2MESERVER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedConnectbtn();
	BOOL m_IsConnect;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedSetserveripportbtn();
	afx_msg void OnClose();
	
	BOOL ConnectDB(void);
};

