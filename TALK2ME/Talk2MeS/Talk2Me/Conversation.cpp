// Conversation.cpp : 实现文件
//
#include "stdafx.h"
#include "Conversation.h"



// CConversation 对话框

IMPLEMENT_DYNAMIC(CConversation, CDialogEx)

CConversation::CConversation(int id,CString title, CWnd* pParent /*=NULL*/)
	: CDialogEx(CConversation::IDD, pParent)
	, m_title(title)
	, m_history(_T(""))
	, m_content(_T(""))
	, m_fId(id)
{
	
}

CConversation::~CConversation()
{
}

void CConversation::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_HISTORYRICHEDIT, m_history);
	DDX_Text(pDX, IDC_CONTENTRICHEDIT, m_content);
	DDX_Control(pDX, IDC_HISTORYRICHEDIT, m_Scroll);
	DDX_Text(pDX, IDC_CONID_EDIT, m_fId);
}


BEGIN_MESSAGE_MAP(CConversation, CDialogEx)
	ON_WM_CLOSE(&CConversation::OnClose)
	ON_BN_CLICKED(IDC_SENDFILEBTN, &CConversation::OnBnClickedSendfilebtn)
	ON_BN_CLICKED(IDC_SENFCONTENTBTN, &CConversation::OnBnClickedSenfcontentbtn)
END_MESSAGE_MAP()


// CConversation 消息处理程序

void CConversation::OnClose(){

	int m_rlp = AfxMessageBox(_T("确定要关闭会话？") , MB_OKCANCEL|MB_ICONQUESTION);

	if(m_rlp == IDOK){
		GetParent()->SendMessage(WM_CloseMsg,m_fId, true);//true表示关闭的是一对一会话
		CDialogEx::OnClose();
	}
}
void CConversation::OnBnClickedSendfilebtn()//点击发送文件按钮，向主对话框传送发送文件的消息
{
	
	CFileDialog m_fileDlg(TRUE, _T("(*.*)|*.*"),_T("*.*") ,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT ,_T("所有文件(*.*)|*.*||"));
	TCHAR m_buffer[100];
	GetCurrentDirectory(100, m_buffer);
	m_fileDlg.m_ofn.lpstrInitialDir = m_buffer;
	if (m_fileDlg.DoModal() == IDOK)
	{
		
		m_filePath = m_fileDlg.GetPathName();
		m_fileEx  = m_fileDlg.GetFileName();
		
		GetParent()->PostMessage(WM_SendFile,m_fId);
	
	}
}


void CConversation::OnBnClickedSenfcontentbtn()//发送文本消息
{
	
	UpdateData(true);
	if(m_content == _T("")){
		AfxMessageBox(_T("不能发送空消息"));
		return;
	}
	///
	m_Scroll.LineScroll(m_Scroll.GetLineCount());
	GetParent()->SendMessage(WM_SendMsg,m_fId);
//	GotoDlgCtrl(GetDlgItem(IDC_CONTENTRICHEDIT));
}