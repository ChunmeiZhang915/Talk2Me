#pragma once
#include<string.h>
#include"ChatRoom.h"
using namespace std;

// CChatroomIn �Ի���

class CChatroomIn : public CDialogEx
{
	DECLARE_DYNAMIC(CChatroomIn)

public:
	CChatroomIn(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CChatroomIn();
	int m_cId;
	int m_ownerId;//��ǰlistboxѡ�е�������id�ͽ�����id
// �Ի�������
	enum { IDD = IDD_CHATDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLbnDblclkAllchatrooms();
};
