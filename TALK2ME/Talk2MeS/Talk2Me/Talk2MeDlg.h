
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



// CTalk2MeDlg �Ի���
class CTalk2MeDlg : public CDialogEx
{

public:
	CTalk2MeDlg(CWnd* pParent = NULL);	// ��׼���캯��
	map<int, CConversation*> m_id2Conversation;//�洢121�Ի�
	map<int, CChatRoom*> m_id2Chatroom;//�洢������
	vector<USER_LOGIN> m_onlineFriends;//ע���û�
	map <CString, CString> m_File2FileName; //ԭʼ���ֶ����ڵ�����
	map<CString, CProgress*> m_File2ProgressBar;//�ļ����Խ�����
	map<CString, CFileRecv*> m_file2SaveDlg;//�ļ����Ա���Ի���
	
	CSetServerIpPort m_serverSet;//���÷������Ի���
	CSignup m_signup;//ע��Ի���
	CLogin m_login;//��½�Ի���
	CFriends m_friendTab;//ע�����tab
	CChatroomIn m_chatRoomTab;//������tab
// �Ի�������
	enum { IDD = IDD_TALK2ME_DIALOG };
	SOCKET m_ClientSocket;
	
	int m_fileConversationId;//121�ļ�����,�����ļ��̵߳ķ���,��Ϊ��������
	int m_fileChatRoomId;//�������ļ�����
	WSADATA wsaData;
	int iLen;			// �ͻ���ַ����
	int iSend;			// �������ݵĳ���
	int iRecv;			// �������ݵĳ���
	struct sockaddr_in m_myAddr,m_serverAddr;				// ���ص�ַ�Ϳͻ���ַ
	MSG0 m_sendMsg , m_recvMsg;
	int m_wantToTalkId;
	int m_myOwnId;//��ǰ�û���id
	char m_username[DB_LENGTH];
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��
	

// ʵ��
protected:
	HICON m_hIcon;
	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	virtual void OnClose();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	BOOL m_IsConnected;
	
	void GetMembersFromTheChatRoom(int cid);//��õ�ǰ�û�����������ҵ����г�Ա���
	afx_msg void OnBnClickedStarttalkButton();
	afx_msg void OnBnClickedLoginbtn();
	afx_msg void OnBnClickedSetbtn();
	afx_msg LRESULT OnRecvFileSave(WPARAM wParam, LPARAM lParam);//�����ļ���ʾ��
	afx_msg LRESULT OnSendFile(WPARAM wParam, LPARAM lParam);//����121�ļ�
	afx_msg LRESULT OnSendMsg(WPARAM wParam, LPARAM lParam);//����121��Ϣ
	afx_msg LRESULT OnRecvChatMsg(WPARAM wParam, LPARAM lParam);//������������Ϣ
	afx_msg LRESULT OnRecvMsg(WPARAM wParam, LPARAM lParam);//����121��Ϣ
	afx_msg LRESULT OnCloseMsg(WPARAM wParam, LPARAM lParam);//�رնԻ���
	afx_msg LRESULT OnSignUp(WPARAM wParam, LPARAM lParam);//ע��
	afx_msg LRESULT OnAddOne(WPARAM wParam, LPARAM lParam);//���������ҳ�Ա
	afx_msg LRESULT OnDelOne(WPARAM wParam, LPARAM lParam);//ɾ�������ҳ�Ա
	afx_msg LRESULT OnSendChatMsg(WPARAM wParam, LPARAM lParam);//������������Ϣ
	afx_msg LRESULT OnSendChatFile(WPARAM wParam, LPARAM lParam);//����Ⱥ���ļ�
	afx_msg LRESULT OnRecvFile(WPARAM wParam, LPARAM lParam);//�����ļ�
	afx_msg LRESULT OnCancelFile(WPARAM wParam, LPARAM lParam);//ȡ���ļ�
	
	afx_msg void OnBnClickedSignupbtn();
	
	afx_msg void OnBnClickedChatroomBtn();
	afx_msg void OnBnClickedLogoutbtn();
	void OnBnClickedGetroombtn();//����û���������е�������
	void OnBnClickedRefreshButton();//��ú����б�
	
	CTabCtrl m_tabCtrl;
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
};

