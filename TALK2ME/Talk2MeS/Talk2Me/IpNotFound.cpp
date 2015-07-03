// IPNotFound.cpp : 实现文件
//

#include "stdafx.h"
#include "Talk2Me.h"
#include "IPNotFound.h"
#include "afxdialogex.h"


// CIPNotFound 对话框

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

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	/*m_ipOfPartner = ntohl(inet_addr("0.0.0.1"));
	UpdateData(false);*/
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}



