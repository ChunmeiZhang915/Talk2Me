#pragma once
class CTALK2MESERVERDlg;

#define DEFAULT_PORT 5050		// 服务器端口号为5050
#define BUFFER_LENGTH 1024		// 接收数据缓冲区长度

// ServerSocket 命令目标

class ServerSocket : public CSocket
{
public:
	ServerSocket();
	virtual ~ServerSocket();
	int m_Status;
	void GetDlg(CTALK2MESERVERDlg* dlg);
	CTALK2MESERVERDlg* m_dlg;
	virtual void OnAccept(int nErrorCode);
	virtual void OnClose(int nErrorCode);
	virtual void OnConnect(int nErrorCode);
	virtual BOOL OnMessagePending();
	virtual void OnOutOfBandData(int nErrorCode);
	virtual void OnReceive(int nErrorCode);
	virtual void OnSend(int nErrorCode);
	virtual int Receive(void* lpBuf, int nBufLen, int nFlags = 0);
	virtual int Send(const void* lpBuf, int nBufLen, int nFlags = 0);
};


