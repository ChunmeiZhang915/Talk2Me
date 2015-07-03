// SetServerIpPort.cpp : 实现文件
//

#include "stdafx.h"
#include "TALK2MESERVER.h"
#include"TALK2MESERVERDlg.h"
#include "SetServerIpPort.h"
#include "afxdialogex.h"
#include<ws2def.h>
#include<ws2ipdef.h>
#include<IPHlpApi.h>


// CSetServerIpPort 对话框

IMPLEMENT_DYNAMIC(CSetServerIpPort, CDialogEx)

CSetServerIpPort::CSetServerIpPort(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSetServerIpPort::IDD, pParent)
	, m_ServerIpAddr(0)
	, m_ServerPort(5050)
{
	char hostname[256];  
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		return;
	}
    if (gethostname(hostname,sizeof(hostname)) == SOCKET_ERROR)  
    {  
        ERRORMSG;  
    }  
   
    HOSTENT* host=gethostbyname(hostname);  
    if (host==NULL)  
    {  
         ERRORMSG; 
    }  
  
	char ip[16];
    strcpy_s(ip,inet_ntoa(*(in_addr*)*host->h_addr_list)); 
	
	m_ServerIpAddr = ntohl(inet_addr(ip));//
	WSACleanup();
}

CSetServerIpPort::~CSetServerIpPort()
{
}

void CSetServerIpPort::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_IPAddress(pDX, IDC_SERVERIPADDR, m_ServerIpAddr);
	DDX_Text(pDX, IDC_SERVERPORTEDIT, m_ServerPort);
}


BEGIN_MESSAGE_MAP(CSetServerIpPort, CDialogEx)
	ON_BN_CLICKED(IDOK, &CSetServerIpPort::OnBnClickedOk)
END_MESSAGE_MAP()


// CSetServerIpPort 消息处理程序


void CSetServerIpPort::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码

	
	UpdateData(true);
	
	if(IsPortUsed(m_ServerPort)){
		AfxMessageBox(_T("端口被用，请重置！"));
		m_ServerPort = 0;
		UpdateData(false);
		return;
	}
	
	CDialogEx::OnOK();
	
}

BOOL CSetServerIpPort::IsPortUsed(unsigned long port){
	MIB_UDPTABLE UdpTable[100];
	DWORD nSize = sizeof(UdpTable);
	if(NO_ERROR == GetUdpTable(&UdpTable[0],&nSize,TRUE))
	{
		DWORD nCount = UdpTable[0].dwNumEntries;
		if (nCount > 0)
		{
			for(DWORD i=0;i<nCount;i++)
			{
				MIB_UDPROW UdpRow = UdpTable[0].table[i];
				DWORD temp1 = UdpRow.dwLocalPort;
				int temp2 = temp1 / 256 + (temp1 % 256) * 256;
				if(temp2 == port)
				{
					return TRUE;
				}
			}
		}
		return FALSE;
	}
	return FALSE;
}
