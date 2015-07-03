// FileRecv.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Talk2Me.h"
#include "FileRecv.h"
#include "afxdialogex.h"


// CFileRecv �Ի���

IMPLEMENT_DYNAMIC(CFileRecv, CDialogEx)

CFileRecv::CFileRecv(CWnd* pParent /*=NULL*/)
	: CDialogEx(CFileRecv::IDD, pParent)
	, m_fileName(_T(""))
	, m_fromId(_T(""))
{
	m_fileLength = 0;
}

CFileRecv::~CFileRecv()
{
}

void CFileRecv::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_FILENAME_STC, m_fileName);
	DDX_Text(pDX, IDC_FROMID_STC, m_fromId);
}


BEGIN_MESSAGE_MAP(CFileRecv, CDialogEx)
	ON_BN_CLICKED(ID_RECVFILE, &CFileRecv::OnBnClickedRecvfile)
	ON_BN_CLICKED(IDC_NEXTTIME, &CFileRecv::OnBnClickedNexttime)
	ON_BN_CLICKED(ID_CANCEL, &CFileRecv::OnBnClickedCancel)
END_MESSAGE_MAP()


// CFileRecv ��Ϣ�������


void CFileRecv::OnBnClickedRecvfile()
{
	CDialogEx::OnOK();
	GetParent()->SendMessage(WM_RecvFile,(WPARAM)&m_fileName,m_isChatFile);
	
}


void CFileRecv::OnBnClickedNexttime()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnOK();
}


void CFileRecv::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnOK();
	GetParent()->SendMessage(WM_CancelFile,(WPARAM)&m_fileName,m_isChatFile);
	
}
