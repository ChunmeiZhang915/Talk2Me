// Conversation.cpp : ʵ���ļ�
//
#include "stdafx.h"
#include "Conversation.h"



// CConversation �Ի���

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


// CConversation ��Ϣ�������

void CConversation::OnClose(){

	int m_rlp = AfxMessageBox(_T("ȷ��Ҫ�رջỰ��") , MB_OKCANCEL|MB_ICONQUESTION);

	if(m_rlp == IDOK){
		GetParent()->SendMessage(WM_CloseMsg,m_fId, true);//true��ʾ�رյ���һ��һ�Ự
		CDialogEx::OnClose();
	}
}
void CConversation::OnBnClickedSendfilebtn()//��������ļ���ť�������Ի����ͷ����ļ�����Ϣ
{
	
	CFileDialog m_fileDlg(TRUE, _T("(*.*)|*.*"),_T("*.*") ,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT ,_T("�����ļ�(*.*)|*.*||"));
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


void CConversation::OnBnClickedSenfcontentbtn()//�����ı���Ϣ
{
	
	UpdateData(true);
	if(m_content == _T("")){
		AfxMessageBox(_T("���ܷ��Ϳ���Ϣ"));
		return;
	}
	///
	m_Scroll.LineScroll(m_Scroll.GetLineCount());
	GetParent()->SendMessage(WM_SendMsg,m_fId);
//	GotoDlgCtrl(GetDlgItem(IDC_CONTENTRICHEDIT));
}