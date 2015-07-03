#include "afxwin.h"
#include<map>
#include<vector>
#include<string>

#define BUFFER_LENGTH  1024
#define DB_LENGTH 50

#define MSG_LOGIN 0//登陆
#define MSG_LOGIN_RP 1//登陆回复
#define MSG_SEARCH 2//查找会话目标ID是否存在
#define MSG_SEARCH_RP 3
#define MSG_SEARCH_ERROR 4//无
#define MSG_ABROAD 5//无
#define MSG_ABROAD_RP //无
#define MSG_FILE 7//文件本体消息
#define MSG_FILETYPE 8//文件类型:名字,大小
#define MSG_FILEEND 9//文件结束
#define MSG_FILE_RP 10//无
#define MSG_LOGOUT 11//注销
#define MSG_LOGOUT_RP 12
#define MSG_BROADCAST 13//查找所有的注册用户
#define MSG_BROADCAST_RP 14
#define MSG_TEXT 15//一对一文本消息
#define MSG_ACK 100//////////////////无
#define MSG_SIGNUP 16//注册
#define MSG_SIGNUP_RP 17
#define MSG_CHATROOM 18//建立聊天室
#define MSG_CHATROOM_RP 19
#define MSG_ADDONECHAT 20//添加聊天室成员
#define MSG_ADDONECHAT_RP 21
#define MSG_DELETEONECHAT 22//删除聊天室成员
#define MSG_DELETEONECHAT_RP 23
#define MSG_CHATTXT 24//聊天文本消息
#define MSG_CHATFILETYPE 25//聊天室文件类型
#define MSG_CHATFILEEND 26//聊天室文件结束
#define MSG_CHATBROADCAST 27//查询当前参与的所有聊天室
#define MSG_CHATBROADCAST_RP 28
#define MSG_SEARCHCHAT 29
#define MSG_SEARCHCHAT_RP 30
#define MSG_RECVFILE 31
#define MSG_CANCELFILE 32
#define MSG_NEXTTIME 33
#define MSG_RECVCHATFILE 34
#define MSG_CANCELCHATFILE 35
#define TIMERBROAD (WM_USER+101)

#define WM_RecvMsg (WM_USER+102)//需要建立一对一聊天对话框
#define WM_RecvChatMsg (WM_USER+103)//需要建立聊天室对话框
#define WM_RecvFileSave (WM_USER+104)//需要建立文件接收对话框
static UINT WM_SendMsg = RegisterWindowMessage(_T("UserText"));//发送121文本消息
static UINT WM_SendFile = RegisterWindowMessage(_T("UserFile"));//发送121文件
static UINT WM_CloseMsg = RegisterWindowMessage(_T("UserClose"));//关闭对话框
static UINT WM_Signup = RegisterWindowMessage(_T("UserSignup"));//注册
static UINT WM_AddOne = RegisterWindowMessage(_T("AddOne"));//添加一个聊天室成员
static UINT WM_DelOne = RegisterWindowMessage(_T("DeleteOne"));//删除一个聊天室成员
static UINT WM_SendChatMsg = RegisterWindowMessage(_T("SendChatMsg"));//发送聊天室文本消息
static UINT WM_SendChatFile = RegisterWindowMessage(_T("SendChatFile"));//发送聊天室文件消息
static UINT WM_RecvFile = RegisterWindowMessage(_T("RecvFile"));//接收文件消息
static UINT WM_CancelFile = RegisterWindowMessage(_T("CancelFile"));//取消接收文件消息
static UINT WM_RecvChatFile = RegisterWindowMessage(_T("RecvChatFile"));//接收聊天室文件消息
static UINT WM_CancelChatFile = RegisterWindowMessage(_T("CancelChatFile"));//接收聊天室文件消息
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
using namespace std;
//无
struct IP_PORT{
	DWORD m_ip;
	USHORT m_port;
};
//服务器和客户端通信的主体结构
struct MSG0{
	unsigned short m_msgType;//消息类型
	char m_text[BUFFER_LENGTH];//消息内容
};
struct TMSG{//一对一文件/文本消息
	unsigned int m_idFrom;//一对一对话中的发送人Id
	char m_name[DB_LENGTH];//发送人的昵称
	unsigned int m_id;//消息要发送给的对方
	char m_text[BUFFER_LENGTH - DB_LENGTH - 2*sizeof(unsigned int)];//消息内容
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
struct CTMSG{//聊天室文本/文件消息
	unsigned int m_idFrom;//发送人Id
	unsigned int m_cId;//聊天室的ID
	unsigned int m_ownerId;//聊天室的建立者ID
	char m_name[DB_LENGTH];//发送人的昵称
	char m_text[BUFFER_LENGTH - DB_LENGTH - 3*sizeof( unsigned int)];//消息内容or文件名字
};
struct FMSG{//一对一文件消息	
	char m_filename[DB_LENGTH];//发送文件名称
	char m_text[BUFFER_LENGTH -DB_LENGTH ];//消息内容
};
//struct LF{
//	long long m_fileLength;//文件changdu
//	char m_filename[DB_LENGTH];//文件名字
//};
struct USER_INFO{//注册使用
	char m_name[DB_LENGTH];
	char m_pwd[DB_LENGTH];
};
struct USER_LOGIN{
	int m_id;//用户ID
	char m_pwd[DB_LENGTH];//用户的密码或者昵称
};
