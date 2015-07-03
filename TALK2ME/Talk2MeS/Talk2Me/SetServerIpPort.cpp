// SetServerIpPort.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Talk2Me.h"
#include"Talk2MeDlg.h"
#include "SetServerIpPort.h"
#include "afxdialogex.h"
#include<ws2def.h>
#include<ws2ipdef.h>
#include<IPHlpApi.h>

// CSetServerIpPort �Ի���

IMPLEMENT_DYNAMIC(CSetServerIpPort, CDialogEx)

CSetServerIpPort::CSetServerIpPort(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSetServerIpPort::IDD, pParent)
	, m_ServerIpAddr(0)
	, m_ServerPort(0)
	, m_ClientOwnPort(0)
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
	
	m_ServerPort = 5050;
	m_ClientOwnPort = 3050;
}

CSetServerIpPort::~CSetServerIpPort()
{
}

void CSetServerIpPort::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_IPAddress(pDX, IDC_SERVERIPADDRESS, m_ServerIpAddr);
	DDX_Text(pDX, IDC_PORTEDIT, m_ServerPort);
	DDX_Text(pDX, IDC_OWNPORTEDIT, m_ClientOwnPort);
}


BEGIN_MESSAGE_MAP(CSetServerIpPort, CDialogEx)
	ON_NOTIFY(IPN_FIELDCHANGED, IDC_SERVERIPADDRESS, &CSetServerIpPort::OnIpnFieldchangedServeripaddress)
	ON_BN_CLICKED(IDOK, &CSetServerIpPort::OnBnClickedOk)
END_MESSAGE_MAP()


// CSetServerIpPort ��Ϣ�������


void CSetServerIpPort::OnIpnFieldchangedServeripaddress(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMIPADDRESS pIPAddr = reinterpret_cast<LPNMIPADDRESS>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
}


void CSetServerIpPort::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(true);
	if(IsPortUsed(m_ClientOwnPort)){
		AfxMessageBox(_T("�ͻ��˶˿ڱ��ã������ã�"));
		m_ClientOwnPort = 0;
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
