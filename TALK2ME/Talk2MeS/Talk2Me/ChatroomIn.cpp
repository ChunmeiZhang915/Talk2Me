// ChatroomIn.cpp : 实现文件
//
#include "stdafx.h"
#include"ChatroomIn.h"



// CChatroomIn 对话框

IMPLEMENT_DYNAMIC(CChatroomIn, CDialogEx)

CChatroomIn::CChatroomIn(CWnd* pParent /*=NULL*/)
	: CDialogEx(CChatroomIn::IDD, pParent)
{
	m_cId = 0;
	m_ownerId = 0;
}

CChatroomIn::~CChatroomIn()
{
}

void CChatroomIn::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CChatroomIn, CDialogEx)
	ON_LBN_DBLCLK(IDC_ALLCHATROOMS, &CChatroomIn::OnLbnDblclkAllchatrooms)
END_MESSAGE_MAP()


// CChatroomIn 消息处理程序


void CChatroomIn::OnLbnDblclkAllchatrooms()
{
	CListBox* m_listBox = (CListBox*)(CWnd::FromHandle(this->m_hWnd)->GetDlgItem(IDC_ALLCHATROOMS));
	CString strText; 
	int nCurSel = (*m_listBox).GetCurSel();
	(*m_listBox).GetText(nCurSel, strText);
	USES_CONVERSION;
	char* temp = W2A(strText);
	int i = 0, j = 0;
	for(; temp[i]!= ' '; ++i){
		if(temp[i] == '0')
			continue;
		else{
			if(j == 0){
				j = i;
			}
		}
	}
	string str =  (string(temp)).substr(j,i - j);
	m_cId = atoi(str.c_str());

	i += 11;
	j = 0;
	for(; temp[i]!= ' '; ++i){
		if(temp[i] == '0')
			continue;
		else{
			if(j == 0){
				j = i;
			}
		}
	}
	string str1 = (string(temp)).substr(j,i - j);
	m_ownerId = atoi(str1.c_str());
	 
	CWnd::FromHandle(GetParent()->GetParent()->m_hWnd)->SendMessage(WM_RecvChatMsg,m_cId);
}
