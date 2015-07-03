// ServerSocket.cpp : 实现文件
//

#include "stdafx.h"
#include "TALK2MESERVER.h"
#include "ServerSocket.h"
#include"TALK2MESERVERDlg.h"

// ServerSocket

ServerSocket::ServerSocket()
{
}

ServerSocket::~ServerSocket()
{
}

void ServerSocket::OnAccept(int nErrorCode){
	/*if (m_dlg->m_ClientSocket==NULL)
		m_dlg->OnAccept();*/
	CSocket::OnAccept(nErrorCode);

}
void ServerSocket::OnClose(int nErrorCode){
	m_dlg->OnClose();
	CSocket::OnClose(nErrorCode);
}
void ServerSocket::OnConnect(int nErrorCode){

}
BOOL ServerSocket::OnMessagePending(){
	return false;
}
void ServerSocket::OnOutOfBandData(int nErrorCode){

}
void ServerSocket::OnReceive(int nErrorCode){
	m_dlg->OnReceive();
	CSocket::OnReceive(nErrorCode);
}
void ServerSocket::OnSend(int nErrorCode){

}
int ServerSocket::Receive(void* lpBuf, int nBufLen, int nFlags){
	return 0;

}
int ServerSocket::Send(const void* lpBuf, int nBufLen, int nFlags){
	return 0;
}
void ServerSocket::GetDlg(CTALK2MESERVERDlg* dlg){
	m_dlg=dlg;
}

// ServerSocket 成员函数
