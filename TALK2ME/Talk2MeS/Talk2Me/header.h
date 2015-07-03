#include "afxwin.h"
#include<map>
#include<vector>
#include<string>

#define BUFFER_LENGTH  1024
#define DB_LENGTH 50

#define MSG_LOGIN 0//��½
#define MSG_LOGIN_RP 1//��½�ظ�
#define MSG_SEARCH 2//���һỰĿ��ID�Ƿ����
#define MSG_SEARCH_RP 3
#define MSG_SEARCH_ERROR 4//��
#define MSG_ABROAD 5//��
#define MSG_ABROAD_RP //��
#define MSG_FILE 7//�ļ�������Ϣ
#define MSG_FILETYPE 8//�ļ�����:����,��С
#define MSG_FILEEND 9//�ļ�����
#define MSG_FILE_RP 10//��
#define MSG_LOGOUT 11//ע��
#define MSG_LOGOUT_RP 12
#define MSG_BROADCAST 13//�������е�ע���û�
#define MSG_BROADCAST_RP 14
#define MSG_TEXT 15//һ��һ�ı���Ϣ
#define MSG_ACK 100//////////////////��
#define MSG_SIGNUP 16//ע��
#define MSG_SIGNUP_RP 17
#define MSG_CHATROOM 18//����������
#define MSG_CHATROOM_RP 19
#define MSG_ADDONECHAT 20//��������ҳ�Ա
#define MSG_ADDONECHAT_RP 21
#define MSG_DELETEONECHAT 22//ɾ�������ҳ�Ա
#define MSG_DELETEONECHAT_RP 23
#define MSG_CHATTXT 24//�����ı���Ϣ
#define MSG_CHATFILETYPE 25//�������ļ�����
#define MSG_CHATFILEEND 26//�������ļ�����
#define MSG_CHATBROADCAST 27//��ѯ��ǰ���������������
#define MSG_CHATBROADCAST_RP 28
#define MSG_SEARCHCHAT 29
#define MSG_SEARCHCHAT_RP 30
#define MSG_RECVFILE 31
#define MSG_CANCELFILE 32
#define MSG_NEXTTIME 33
#define MSG_RECVCHATFILE 34
#define MSG_CANCELCHATFILE 35
#define TIMERBROAD (WM_USER+101)

#define WM_RecvMsg (WM_USER+102)//��Ҫ����һ��һ����Ի���
#define WM_RecvChatMsg (WM_USER+103)//��Ҫ���������ҶԻ���
#define WM_RecvFileSave (WM_USER+104)//��Ҫ�����ļ����նԻ���
static UINT WM_SendMsg = RegisterWindowMessage(_T("UserText"));//����121�ı���Ϣ
static UINT WM_SendFile = RegisterWindowMessage(_T("UserFile"));//����121�ļ�
static UINT WM_CloseMsg = RegisterWindowMessage(_T("UserClose"));//�رնԻ���
static UINT WM_Signup = RegisterWindowMessage(_T("UserSignup"));//ע��
static UINT WM_AddOne = RegisterWindowMessage(_T("AddOne"));//���һ�������ҳ�Ա
static UINT WM_DelOne = RegisterWindowMessage(_T("DeleteOne"));//ɾ��һ�������ҳ�Ա
static UINT WM_SendChatMsg = RegisterWindowMessage(_T("SendChatMsg"));//�����������ı���Ϣ
static UINT WM_SendChatFile = RegisterWindowMessage(_T("SendChatFile"));//�����������ļ���Ϣ
static UINT WM_RecvFile = RegisterWindowMessage(_T("RecvFile"));//�����ļ���Ϣ
static UINT WM_CancelFile = RegisterWindowMessage(_T("CancelFile"));//ȡ�������ļ���Ϣ
static UINT WM_RecvChatFile = RegisterWindowMessage(_T("RecvChatFile"));//�����������ļ���Ϣ
static UINT WM_CancelChatFile = RegisterWindowMessage(_T("CancelChatFile"));//�����������ļ���Ϣ
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
//��
struct IP_PORT{
	DWORD m_ip;
	USHORT m_port;
};
//�������Ϳͻ���ͨ�ŵ�����ṹ
struct MSG0{
	unsigned short m_msgType;//��Ϣ����
	char m_text[BUFFER_LENGTH];//��Ϣ����
};
struct TMSG{//һ��һ�ļ�/�ı���Ϣ
	unsigned int m_idFrom;//һ��һ�Ի��еķ�����Id
	char m_name[DB_LENGTH];//�����˵��ǳ�
	unsigned int m_id;//��ϢҪ���͸��ĶԷ�
	char m_text[BUFFER_LENGTH - DB_LENGTH - 2*sizeof(unsigned int)];//��Ϣ����
};
struct LTMSG{//һ��һ�ļ�������Ϣ
	unsigned int m_idFrom;//һ��һ�Ի��еķ�����Id
	char m_name[DB_LENGTH];//�����˵��ǳ�
	unsigned int m_id;//��ϢҪ���͸��ĶԷ�
	long long m_fileLength;//�ļ���С
	char m_text[BUFFER_LENGTH - DB_LENGTH - 2*sizeof(unsigned int) - sizeof(long long)];//�ļ�����
};
struct LCTMSG{//�������ļ�������Ϣ
	unsigned int m_idFrom;//������Id
	unsigned int m_cId;//�����ҵ�ID
	unsigned int m_ownerId;//�����ҵĽ�����ID
	char m_name[DB_LENGTH];//�����˵��ǳ�
	long long m_fileLength;//�ļ���С
	char m_text[BUFFER_LENGTH - DB_LENGTH - 3*sizeof( unsigned int) - sizeof(long long)];//�ļ�����
};
struct CTMSG{//�������ı�/�ļ���Ϣ
	unsigned int m_idFrom;//������Id
	unsigned int m_cId;//�����ҵ�ID
	unsigned int m_ownerId;//�����ҵĽ�����ID
	char m_name[DB_LENGTH];//�����˵��ǳ�
	char m_text[BUFFER_LENGTH - DB_LENGTH - 3*sizeof( unsigned int)];//��Ϣ����or�ļ�����
};
struct FMSG{//һ��һ�ļ���Ϣ	
	char m_filename[DB_LENGTH];//�����ļ�����
	char m_text[BUFFER_LENGTH -DB_LENGTH ];//��Ϣ����
};
//struct LF{
//	long long m_fileLength;//�ļ�changdu
//	char m_filename[DB_LENGTH];//�ļ�����
//};
struct USER_INFO{//ע��ʹ��
	char m_name[DB_LENGTH];
	char m_pwd[DB_LENGTH];
};
struct USER_LOGIN{
	int m_id;//�û�ID
	char m_pwd[DB_LENGTH];//�û�����������ǳ�
};
