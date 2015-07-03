#pragma once



#include"Conversation.h"
#include "afxdialogex.h"

#include "Talk2Me.h"
#include "afxcmn.h"
using namespace std;


// CChatRoom �Ի���

class CChatRoom : public CDialogEx
{
	DECLARE_DYNAMIC(CChatRoom)

public:
	CChatRoom(int id,int ownerId, CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CChatRoom();
	virtual void OnClose();
	int m_id;//roomId
	int m_owner;//owner id
	int nCurSel; //��ǰѡ�е��к�
	CString strText; //��ǰѡ���е��ַ���
// �Ի�������
	enum { IDD = IDD_CHATROOMDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedChatsendfilebtn();
	afx_msg void OnLbnSelchangeOulist();
	afx_msg void OnLbnSelchangeChatmemberlist();
	afx_msg void OnBnClickedChatsendmsgbtn();
	CString m_content;
	CString m_history;
	CString m_filePath;
	CString m_fileEx;
	CRichEditCtrl m_Scroll;
};
