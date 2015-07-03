// ChatRoom.cpp : 实现文件
//

#include "stdafx.h"
#include"ChatRoom.h"


// CChatRoom 对话框

IMPLEMENT_DYNAMIC(CChatRoom, CDialogEx)

CChatRoom::CChatRoom(int id, int ownerId, CWnd* pParent /*=NULL*/)
	: CDialogEx(CChatRoom::IDD, pParent)
	, m_id(id)
	, m_owner(ownerId)
	, m_content(_T(""))
	, m_history(_T(""))
{
	
}

CChatRoom::~CChatRoom()
{
}

void CChatRoom::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_CONTENTRE, m_content);
	DDX_Text(pDX, IDC_CHATROOMRE, m_history);
	DDX_Control(pDX, IDC_CHATROOMRE, m_Scroll);
}


BEGIN_MESSAGE_MAP(CChatRoom, CDialogEx)
	ON_WM_CLOSE(&CChatRoom::OnClose)
	ON_BN_CLICKED(IDC_CHATSENDFILEBTN, &CChatRoom::OnBnClickedChatsendfilebtn)
	ON_LBN_SELCHANGE(IDC_OULIST, &CChatRoom::OnLbnSelchangeOulist)
	ON_LBN_SELCHANGE(IDC_CHATMEMBERLIST, &CChatRoom::OnLbnSelchangeChatmemberlist)
	ON_BN_CLICKED(IDC_CHATSENDMSGBTN, &CChatRoom::OnBnClickedChatsendmsgbtn)
END_MESSAGE_MAP()


// CChatRoom 消息处理程序

void CChatRoom::OnClose(){

	int m_rlp = AfxMessageBox(_T("确定要关闭会话？") , MB_OKCANCEL|MB_ICONQUESTION);

	if(m_rlp == IDOK){
		CDialogEx::OnClose();
		GetParent()->PostMessage(WM_CloseMsg,m_id, false);//true表示关闭的是一对一会话
		
	}
}
void CChatRoom::OnBnClickedChatsendfilebtn()
{
	CFileDialog m_fileDlg(TRUE, _T("(*.*)|*.*"),_T("*.*") ,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT ,_T("所有文件(*.*)|*.*||"));
	TCHAR m_buffer[100];
	GetCurrentDirectory(100, m_buffer);
	m_fileDlg.m_ofn.lpstrInitialDir = m_buffer;
	if (m_fileDlg.DoModal() == IDOK)
	{
		
		m_filePath = m_fileDlg.GetPathName();
		m_fileEx  = m_fileDlg.GetFileName();
		
		GetParent()->SendMessage(WM_SendChatFile,m_id);
	
	}

}


void CChatRoom::OnLbnSelchangeOulist()
{
	CListBox* m_listBox = (CListBox*)(CWnd::FromHandle(this->m_hWnd)->GetDlgItem(IDC_OULIST));
	
	int nCurSel = (*m_listBox).GetCurSel();
	(*m_listBox).GetText(nCurSel, strText);
	USES_CONVERSION;
	char* temp = W2A(strText);
	int i = 0, j = 0;
	for(; temp[i]!= ' '; ++i){
		if(temp[i] == '0')
			continue;
		else{
			j = i;
		}
	}
	string str = string(temp);
	str.substr(j,i - j);
	if(atoi(str.c_str()) == m_owner){
		return;
	}
	
	GetParent()->SendMessage(WM_AddOne,atoi(str.c_str()),m_id);

}


void CChatRoom::OnLbnSelchangeChatmemberlist()
{
	CListBox* m_listBox = (CListBox*)(CWnd::FromHandle(this->m_hWnd)->GetDlgItem(IDC_CHATMEMBERLIST));
	CString strText; 
	nCurSel = (*m_listBox).GetCurSel();
	(*m_listBox).GetText(nCurSel, strText);
	
	USES_CONVERSION;
	char* temp = W2A(strText);
	int i = 0, j = 0;
	for(; temp[i]!= ' '; ++i){
		if(temp[i] == '0')
			continue;
		else{
			j = i;
		}
	}
	string str = string(temp);
	str.substr(j,i - j);
	
	
	GetParent()->SendMessage(WM_DelOne,atoi(str.c_str()),m_id);
}


void CChatRoom::OnBnClickedChatsendmsgbtn()
{
	UpdateData(true);
	if(m_content == _T("")){
		AfxMessageBox(_T("不能发送空消息"));
		return;
	}
	m_Scroll.LineScroll(m_Scroll.GetLineCount());
	GetParent()->SendMessage(WM_SendChatMsg,m_id);
//	GotoDlgCtrl(GetDlgItem(IDC_CONTENTRICHEDIT));
}
