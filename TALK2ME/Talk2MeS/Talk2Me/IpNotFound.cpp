// IPNotFound.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Talk2Me.h"
#include "IPNotFound.h"
#include "afxdialogex.h"


// CIPNotFound �Ի���

IMPLEMENT_DYNAMIC(CIPNotFound, CDialogEx)

CIPNotFound::CIPNotFound(CWnd* pParent /*=NULL*/)
	: CDialogEx(CIPNotFound::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	
}

CIPNotFound::~CIPNotFound()
{
}

void CIPNotFound::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CIPNotFound, CDialogEx)
//	ON_STN_CLICKED(IDC_IPNOTFOUNDTIP_STATIC, &CIPNotFound::OnStnClickedIpnotfoundtipStatic)
END_MESSAGE_MAP()


BOOL CIPNotFound::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	/*m_ipOfPartner = ntohl(inet_addr("0.0.0.1"));
	UpdateData(false);*/
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}



