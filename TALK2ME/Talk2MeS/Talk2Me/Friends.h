#pragma once
#include<string.h>
#include"Conversation.h"
#include "afxdialogex.h"

#include "Talk2Me.h"
#include "afxcmn.h"
using namespace std;
// CFriends �Ի���

class CFriends : public CDialogEx
{
	DECLARE_DYNAMIC(CFriends)

public:
	CFriends(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CFriends();
	int m_id;
	char m_name[DB_LENGTH];//��ȡtab��ѡ���˵�id���ǳ�
// �Ի�������
	enum { IDD = IDD_FRIENDDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLbnDblclkLocalpartnerlist();
};
