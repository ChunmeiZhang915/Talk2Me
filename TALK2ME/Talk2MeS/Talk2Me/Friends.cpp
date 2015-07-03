// Friends.cpp : 实现文件
//

#include "stdafx.h"
#include "Talk2Me.h"
#include "Friends.h"
#include "afxdialogex.h"


// CFriends 对话框

IMPLEMENT_DYNAMIC(CFriends, CDialogEx)

CFriends::CFriends(CWnd* pParent /*=NULL*/)
	: CDialogEx(CFriends::IDD, pParent)
{
	m_id = 0;
	memset(m_name,0,DB_LENGTH);
}

CFriends::~CFriends()
{
}

void CFriends::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CFriends, CDialogEx)
	ON_LBN_DBLCLK(IDC_LOCALPARTNERLIST, &CFriends::OnLbnDblclkLocalpartnerlist)
END_MESSAGE_MAP()


// CFriends 消息处理程序


void CFriends::OnLbnDblclkLocalpartnerlist()
{
	CListBox* m_listBox = (CListBox*)(CWnd::FromHandle(this->m_hWnd)->GetDlgItem(IDC_LOCALPARTNERLIST));
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
			j = i;
		}
	}
	string str = string(temp);
	int n = 0;
	memset(m_name, 0, DB_LENGTH);
	for(int m = str.find_last_of(' ') + 1; m < str.length(); ++m, ++n){
		m_name[n] = str[m];
	}
	str.substr(j,i - j);
	m_id = atoi(str.c_str());
	
	CWnd::FromHandle(GetParent()->GetParent()->m_hWnd)->SendMessage(WM_RecvMsg,atoi(str.c_str()));
}
