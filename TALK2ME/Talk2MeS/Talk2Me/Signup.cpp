// Signup.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Talk2Me.h"
#include "Signup.h"
#include "afxdialogex.h"


// CSignup �Ի���

IMPLEMENT_DYNAMIC(CSignup, CDialogEx)

CSignup::CSignup(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSignup::IDD, pParent)
	, m_username(_T(""))
	, m_pwd1(_T(""))
	, m_pwd(_T(""))
{
	m_test = false;
}

CSignup::~CSignup()
{
}

void CSignup::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_USERNAME_EDIT, m_username);
	DDX_Text(pDX, IDC_PWD1_EDIT, m_pwd1);
	DDX_Text(pDX, IDC_PWD2_EDIT, m_pwd);
}


BEGIN_MESSAGE_MAP(CSignup, CDialogEx)
	ON_BN_CLICKED(IDOK, &CSignup::OnBnClickedOk)
END_MESSAGE_MAP()


// CSignup ��Ϣ�������
bool CSignup::IsValidInput(CString m_str){
	USES_CONVERSION;
	char* temp = W2A(m_str);
	for(int i = 0;i < m_str.GetLength(); ++i)
	{
		
		if((temp[i] < '0') ||(temp[i] > '9' && temp[i] < 'A')|| (temp[i] > 'Z' && temp[i] < 'a') || temp[i] > 'z')
			return false;
	}
	return true;
}

void CSignup::OnBnClickedOk()
{
	UpdateData(true);
	if(IsValidInput(m_username) && IsValidInput(m_pwd) && !(m_pwd.Compare(m_pwd1))){
		//GetParent()->SendMessage(WM_Signup);
		CDialogEx::OnOK();
	}else{
		if(!IsValidInput(m_username)){
			AfxMessageBox(_T("�û��ǳ����벻�Ϸ���ֻ�ܰ������ֺ���ĸ"));
		}
		if(!IsValidInput(m_pwd)){
			AfxMessageBox(_T("�û��������벻�Ϸ���ֻ�ܰ������ֺ���ĸ"));
		}
		if(m_pwd.Compare(m_pwd1)){
			AfxMessageBox(_T("�����������벻һ�£�"));
		}
	}
	
}
