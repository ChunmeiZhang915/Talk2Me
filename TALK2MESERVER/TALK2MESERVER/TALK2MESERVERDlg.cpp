#include "stdafx.h"
#include "TALK2MESERVER.h"
#include "TALK2MESERVERDlg.h"
#include "afxdialogex.h"
#include <WinSock2.h>
#include <stdlib.h>
#include <STDIO.H>
#include <windows.h>

#define DEBUG
#undef DEBUG


#include"mysql.h"
#pragma comment(lib,"Iphlpapi.lib")
#pragma comment(lib,"Ws2_32")
#pragma comment(lib, "libmysql.lib")
_ConnectionPtr m_pConnection;
 _RecordsetPtr m_pRecordset;
 _CommandPtr m_pCommand; 
 CString FILESAVEPATH;
void Receiver(LPVOID pParam);
MYSQL mysql;
class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CTALK2MESERVERDlg 对话框



CTALK2MESERVERDlg::CTALK2MESERVERDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTALK2MESERVERDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTALK2MESERVERDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	
}

BEGIN_MESSAGE_MAP(CTALK2MESERVERDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CLOSE(&CTALK2MESERVERDlg::OnClose)
	ON_BN_CLICKED(IDC_CONNECTBTN, &CTALK2MESERVERDlg::OnBnClickedConnectbtn)
	ON_BN_CLICKED(IDC_SETSERVERIPPORTBTN, &CTALK2MESERVERDlg::OnBnClickedSetserveripportbtn)
//ON_BN_CLICKED(IDC_DISCONNECTBTN, &CTALK2MESERVERDlg::OnBnClickedDisconnectbtn)
END_MESSAGE_MAP()


// CTALK2MESERVERDlg 消息处理程序

BOOL CTALK2MESERVERDlg::OnInitDialog()
{
	::CoInitialize(NULL);

	AfxEnableControlContainer();
	if(!AfxOleInit()){
		AfxMessageBox(_T("初始化OLE DLL失败"));
		return false;
	}
	if(!ConnectDB()){
		return false;	
	}

	CDialogEx::OnInitDialog();

	
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

	
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标
	

	m_IsConnect = false;
	(CButton*)GetDlgItem(IDC_SETSERVERIPPORTBTN)->EnableWindow(true);
	//(CButton*)GetDlgItem(IDC_DISCONNECTBTN)->EnableWindow(false);
	(CButton*)GetDlgItem(IDC_CONNECTBTN)->EnableWindow(false);
	ShowWindow(SW_NORMAL);


	//在当前目录下创建File文件夹;

	TCHAR Buffer[MAX_PATH];
	DWORD dwRet;
	dwRet = GetCurrentDirectory(MAX_PATH, Buffer);
	
	FILESAVEPATH = CString(Buffer);
	

	if(_access("File",0)){
		if(CreateDirectory(_T("File"),NULL) == NULL){
			ERRORMSG;
		}		
	}
	FILESAVEPATH += _T("\\File\\");
	return TRUE;
}
void CTALK2MESERVERDlg::OnClose(){//下线处理
	int m_rlp = AfxMessageBox(_T("确定要断开服务器吗？") , MB_OKCANCEL|MB_ICONQUESTION);		
	if(m_rlp == IDCANCEL){
		return;
	}else{
		if(!m_IsConnect){
			CDialogEx::OnClose();
		}else{
			m_IsConnect = false;
			CWnd::FromHandle(this->m_hWnd)->GetDlgItem(IDC_CONNECTBTN)->SetWindowTextW(_T("30s后自动断开"));
			Sleep(10000);

			m_pConnection->Close();
		
			closesocket(m_ServerSocket);
			WSACleanup();
			CDialogEx::OnClose();
		}
	}
	
	
}
BOOL CTALK2MESERVERDlg::ConnectDB(void)
{
	
	mysql_init(&mysql);//初始化
	bool isConnected=mysql_real_connect(&mysql,"sqld.duapp.com","AUl2V3dFITG9IDAXbg9Ae5PB","YH7bynOe681znjgOgqpO1ZYqyjfpqvsY","kBtgMQXJlivdOJTXyoRS",4050,0,0);//连
	if (isConnected)
	{
		MessageBox(_T("success connected!"));
		return true;
	}
	else
	{
		int i = mysql_errno(&mysql);//连接出错
		const char * s = mysql_error(&mysql);
		MessageBox(_T("failed to connect! Please check for your db service."));
		return false;
	}

	//HRESULT hr = NULL;
	//try{
	//	hr = m_pConnection.CreateInstance(_T("ADODB.Connection"));//创建 Connection 对象 
	//	m_pRecordset.CreateInstance(__uuidof(Recordset));
	//	m_pCommand.CreateInstance(_T("ADODB.Command")); 
	//	if(SUCCEEDED(hr))   
	//	{
	//		
	//		m_pConnection->ConnectionString = "driver={SQL Server};server=ZHANGCHUNMEI-PC;uid=Zhang;pwd=123456;database=Talk2Me";
	//		m_pConnection->Open("","","",adConnectUnspecified);
	//		m_pCommand->ActiveConnection = m_pConnection;
	//	}
	//	else
	//	{
	//		AfxMessageBox(_T("创建 Connection 对象失败"));//显示错误信息
	//		return FALSE;
	//	}
	//}catch(_com_error e){ 
	//	CString errormessage;  
 //       errormessage.Format( TEXT("连接数据库失败 !\r\n 错误信息 :%s(%ld)"),e.ErrorMessage(),e.Error() );   
 //       AfxMessageBox(errormessage);///显示错误信息    
	//	return FALSE;
	//}
	
}
void CTALK2MESERVERDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

void CTALK2MESERVERDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CTALK2MESERVERDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

  
 
 

void CTALK2MESERVERDlg::OnBnClickedConnectbtn()
{
	//(CButton*)GetDlgItem(IDC_DISCONNECTBTN)->EnableWindow(true);
	(CButton*)GetDlgItem(IDC_CONNECTBTN)->EnableWindow(false);
	
	m_IsConnect = true;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		return;
	}
	
	// 产生服务器端套接口
	m_ServerSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (m_ServerSocket == INVALID_SOCKET) {
		ERRORMSG;
		closesocket(m_ServerSocket);
		WSACleanup();
		return;
	}

	
	// 绑定服务端套接口
	m_serverAddr.sin_addr.S_un.S_addr = htonl(m_ServerSet.m_ServerIpAddr);
	m_serverAddr.sin_family = AF_INET;
	m_serverAddr.sin_port = htons(m_ServerSet.m_ServerPort);

	if(bind(m_ServerSocket, (struct sockaddr *)&m_serverAddr, sizeof(m_serverAddr)) == SOCKET_ERROR){
		ERRORMSG;
		closesocket(m_ServerSocket);
		WSACleanup();
		return;
	}
	listen(m_ServerSocket, SOMAXCONN);

	iLen = sizeof(m_clientAddrFrom);

	for(int i = 0;i < CO_THREAD_NUM;i++)
		_beginthread(Receiver,0,(LPVOID)this);

	
}
void CTALK2MESERVERDlg::OnBnClickedSetserveripportbtn()
	{
		INT_PTR nRes = m_ServerSet.DoModal();  // 弹出对话框 
		if(nRes == IDOK){
			(CButton*)GetDlgItem(IDC_SETSERVERIPPORTBTN)->EnableWindow(false);
			(CButton*)GetDlgItem(IDC_CONNECTBTN)->EnableWindow(true);
		}
	}


void Receiver(LPVOID pParam )
{
	CTALK2MESERVERDlg* dlg = (CTALK2MESERVERDlg*) pParam;
	fd_set readSet;
	int ret;
	timeval tv;
	while(dlg->m_IsConnect)
    {
		
		// 从客户端接收数据
		SOCKET m_socketConnected  = accept(dlg->m_ServerSocket,(SOCKADDR *)&(dlg->m_clientAddrFrom),&(dlg->iLen));//接受连接请求
		
		if (m_socketConnected == SOCKET_ERROR){
			ERRORMSG;
			continue;
		}

		

		while(1){
			FD_ZERO(&readSet);//初始化readSet
			FD_SET(m_socketConnected,&readSet);//将套接字sockSvr加入readSet中
			tv.tv_sec = 250;//250秒
			tv.tv_usec =0;//0毫秒

			//调用select,等待有数据可读或等待超时
			ret = select(0,&readSet,NULL,NULL,&tv);
			//如果调用select或等待超时
			if(ret == SOCKET_ERROR)
			{
				//断开连接,有的话
				for(map<int,SOCKET>::iterator first = dlg->m_OnlineClients.begin(); first != dlg->m_OnlineClients.end(); ++first){
					if(first->second == m_socketConnected){
						closesocket(m_socketConnected);
						dlg->m_OnlineClients.erase(first);
						break;
					}

				}
				ERRORMSG;
				break;
			}
			if(ret == 0){
				//AfxMessageBox(_T("Time out!"));
				//断开连接,如果有的话.
				for(map<int,SOCKET>::iterator first = dlg->m_OnlineClients.begin(); first != dlg->m_OnlineClients.end(); ++first){
					if(first->second == m_socketConnected){
						closesocket(m_socketConnected);
						dlg->m_OnlineClients.erase(first);
						break;
					}

				}
				break;
			}
			if(FD_ISSET(m_socketConnected,&readSet)){
				memset(&(dlg->m_recvMsg), 0, sizeof(dlg->m_recvMsg));
				memset(&(dlg->m_sendMsg), 0, sizeof(dlg->m_sendMsg));
				dlg->iRecv = recv(m_socketConnected,(char*)&(dlg->m_recvMsg),sizeof(dlg->m_recvMsg),0);
				if(dlg->iRecv == SOCKET_ERROR){
					//断开连接,如果有的话,
					for(map<int,SOCKET>::iterator first = dlg->m_OnlineClients.begin(); first != dlg->m_OnlineClients.end(); ++first){
						if(first->second == m_socketConnected){
							dlg->m_OnlineClients.erase(first);
							break;
						}

					}
					ERRORMSG;
					break;
				}else{
					if(dlg->iRecv == 0){
						continue;
					}else{
						TCHAR m_operation[MAX_PATH * 3];  
						USER_INFO* m_user;//新注册用户
						USER_LOGIN* m_loginUser;
						TMSG* m_t;
						FMSG* m_f;
						CTMSG* m_ct;
						LTMSG* m_lt;//121文件头信息
						LCTMSG* m_lct;//用户发送群发文件头信息
						
						_variant_t var;
						CString m_cstr;//文件名字
						int id;
						int* m_2int;//存储聊天室的不在线成员id
						CFile m_cFile;//客户端上传的文件
						switch(dlg->m_recvMsg.m_msgType){
						case MSG_SIGNUP:
							m_user = new USER_INFO;//表示新注册用户,存储昵称和密码
							memcpy( m_user, dlg->m_recvMsg.m_text, sizeof(USER_INFO));
							_tcscpy_s(m_operation,_T("INSERT INTO [Talk2Me].[dbo].[User]([Username],[Password],[OnlineState])VALUES('"));
							_tcscat_s(m_operation, CString(m_user->m_name));
							_tcscat_s(m_operation,_T("','"));
							_tcscat_s(m_operation,CString(m_user->m_pwd));
							_tcscat_s(m_operation,_T("',0)"));
							try{
								m_pRecordset->Open(_variant_t(m_operation),  _variant_t  ((IDispatch*) m_pConnection,true), adOpenStatic, adLockOptimistic, adCmdText); 
								if(m_pRecordset != NULL && m_pRecordset->GetState() == adStateOpen){
									m_pRecordset->Close();
								}
								memset(m_operation,0,sizeof(m_operation));
								_tcscpy_s(m_operation,_T("select MAX(ID) from [Talk2Me].[dbo].[User]"));//获取系统分配的id
								m_pRecordset->Open(_variant_t(m_operation),  _variant_t  ((IDispatch*) m_pConnection,true), adOpenStatic, adLockOptimistic, adCmdText); 
								var = m_pRecordset->GetCollect(_variant_t((long)0));
								dlg->m_sendMsg.m_msgType = MSG_SIGNUP_RP;
								memcpy(dlg->m_sendMsg.m_text,&(var.lVal),sizeof(var.lVal));
								//注册成功,给注册用户发送新注册的id
								dlg->iSend = send(m_socketConnected, (char*)&(dlg->m_sendMsg), sizeof(dlg->m_sendMsg), 0);
								if (dlg->iSend == SOCKET_ERROR) {
									ERRORMSG;
									return;
								}
								else if (dlg->iSend == 0)
									continue;
								else{
	#ifdef DEBUG
		AfxMessageBox(_T("注册成功,已回复\n"));
	#endif
								}
							}
							catch(_com_error e)
							{
								CString errormessage;  
								errormessage.Format( _T("insert失败 !\r\n 错误信息 :%s(%ld)"),e.ErrorMessage(),e.Error() );   
								AfxMessageBox(errormessage);///显示错误信息 
							}
							if(m_pRecordset != NULL && m_pRecordset->GetState() == adStateOpen){
								m_pRecordset->Close();
							}
							delete m_user;
							break;
						case MSG_LOGOUT:
							m_loginUser = new USER_LOGIN;//存储注销人的Id
								
							memcpy( m_loginUser, dlg->m_recvMsg.m_text, sizeof(USER_LOGIN));
							m_cstr.Format(_T("%d"),m_loginUser->m_id);
							memset(m_operation, 0, sizeof(m_operation));
							_tcscpy_s(m_operation,_T("UPDATE [Talk2Me].[dbo].[User] SET OnlineState = 0 WHERE ID = "));
							_tcscat_s(m_operation, m_cstr);
							try{
								m_pRecordset->Open(_variant_t(m_operation),  _variant_t  ((IDispatch*) m_pConnection,true), adOpenStatic, adLockOptimistic, adCmdText); 
							}catch(_com_error e){
								CString errormessage;  
								errormessage.Format( _T("注销失败 !\r\n 错误信息 :%s(%ld)"),e.ErrorMessage(),e.Error() );   
								AfxMessageBox(errormessage);///显示错误信息 
							}	
							if(m_pRecordset != NULL && m_pRecordset->GetState() == adStateOpen){
								m_pRecordset->Close();
							}
							for(map<int,SOCKET>::iterator first = dlg->m_OnlineClients.begin(); first != dlg->m_OnlineClients.end(); ++first){
								if(first->second == m_socketConnected){
									dlg->m_OnlineClients.erase(first);
									break;
								}

							}
							FD_CLR(m_socketConnected,&readSet);
							closesocket(m_socketConnected);
							delete m_loginUser;
							break;
						case MSG_LOGIN:
							m_loginUser = new USER_LOGIN;//存储登陆人的Id和密码
							m_user = new USER_INFO;//用户返回登录人的姓名和登陆反馈信息
							memcpy( m_loginUser, dlg->m_recvMsg.m_text, sizeof(USER_LOGIN));
							//_tcscpy_s(m_operation,_T("select * from [Talk2Me].[dbo].[User]"));
							m_cstr.Format(_T("%d"),m_loginUser->m_id);
							_tcscpy_s(m_operation,_T("SELECT * FROM [Talk2Me].[dbo].[User] where [Talk2Me].[dbo].[User].ID = "));
							_tcscat_s(m_operation, m_cstr);
							_tcscat_s(m_operation,_T(" and Password = '"));
							_tcscat_s(m_operation,CString(m_loginUser->m_pwd));
							_tcscat_s(m_operation,_T("'"));
							try{
								m_pRecordset->CursorLocation = adUseClient;
								m_pRecordset->Open(_variant_t(m_operation),  _variant_t  ((IDispatch*) m_pConnection,true), adOpenDynamic, adLockOptimistic, adCmdText); 
								dlg->m_sendMsg.m_msgType = MSG_LOGIN_RP;
								if(m_pRecordset->GetRecordCount() == 1){
									strcpy_s(m_user->m_name,_com_util::ConvertBSTRToString((m_pRecordset->GetCollect(_variant_t(_T("Username")))).bstrVal));
										
									var = m_pRecordset->GetCollect(_variant_t(_T("OnlineState")));
									if(var.boolVal == -1){
										memcpy(m_user->m_pwd,"已经登陆!",sizeof("已经登陆!"));
										memcpy(dlg->m_sendMsg.m_text,m_user,sizeof(USER_INFO));
									}else{
										dlg->m_OnlineClients.insert(std::pair<int, SOCKET>(m_loginUser->m_id, m_socketConnected));
										memcpy(m_user->m_pwd,"登陆成功!",sizeof("登陆成功!"));
										memcpy(dlg->m_sendMsg.m_text,m_user,sizeof(USER_INFO));
										var = m_pRecordset->GetCollect(_variant_t((long)0));
										m_cstr.Format(_T("%ld"),var.lVal);
										if(m_pRecordset != NULL && m_pRecordset->GetState() == adStateOpen){
											m_pRecordset->Close();
										}
										if(m_pRecordset != NULL && m_pRecordset->GetState() == adStateOpen){
											m_pRecordset->Close();
										}
										memset(m_operation, 0, sizeof(m_operation));
										_tcscpy_s(m_operation,_T("UPDATE [Talk2Me].[dbo].[User] SET OnlineState = 1 WHERE ID = "));
										_tcscat_s(m_operation, m_cstr);
										m_pRecordset->Open(_variant_t(m_operation),  _variant_t  ((IDispatch*) m_pConnection,true), adOpenStatic, adLockOptimistic, adCmdText); 
											
									}
									dlg->m_OnlineClients.insert(std::pair<int, SOCKET>(m_loginUser->m_id, m_socketConnected));
								}else{
									memcpy(m_user->m_pwd,"登陆失败!",sizeof("登陆失败!"));
									memcpy(dlg->m_sendMsg.m_text,m_user,sizeof(USER_INFO));
								}
								dlg->iSend = send(m_socketConnected, (char*)&(dlg->m_sendMsg), sizeof(dlg->m_sendMsg), 0);
								if (dlg->iSend == SOCKET_ERROR) {
									ERRORMSG;
									return;
								}
								else if (dlg->iSend == 0)
									continue;
								else{
	#ifdef DEBUG
		AfxMessageBox(_T("登陆,已回复\n"));
	#endif
								}
								//判断是否有离线聊天室消息
								if(CString(m_user->m_pwd) == _T("登陆成功!")){
									if(m_pRecordset != NULL && m_pRecordset->GetState() == adStateOpen){
										m_pRecordset->Close();
									}
									memset(m_operation, 0, sizeof(m_operation));
									m_cstr.Format(_T("%d"),m_loginUser->m_id);
									m_pRecordset->CursorLocation = adUseClient;
									_tcscpy_s(m_operation,_T("SELECT U.Username, M.[IDFrom],[IDTo],[Message],[Time],[IsFile],[IsChatRoom],C.[ChatRoomNo],C.Founder FROM [Talk2Me].[dbo].[ChatRoom] C,[Talk2Me].[dbo].[Msg] M,[Talk2Me].[dbo].[User]  U WHERE M.IDFrom = U.ID AND M.IsChatRoom = 1 AND M.ChatRoomNo = C.ChatRoomNo AND M.IDTo = "));
									_tcscat_s(m_operation, m_cstr);
									m_pRecordset->CursorLocation = adUseClient;
									m_pRecordset->Open(_variant_t(m_operation),  _variant_t  ((IDispatch*) m_pConnection,true), adOpenDynamic, adLockOptimistic, adCmdText); 
									//聊天室消息
									while(!m_pRecordset->adoEOF){
										memset(&(dlg->m_sendMsg), 0, sizeof(dlg->m_sendMsg));
										//判断是文件还是文本消息
										if((m_pRecordset->GetCollect(_variant_t(_T("IsFile")))).boolVal == -1){//是文件消息
											m_cstr.Empty();
											m_cstr = CString(FILESAVEPATH);
											m_cstr +=  CString(_com_util::ConvertBSTRToString((m_pRecordset->GetCollect(_variant_t(_T("Message")))).bstrVal));//获得已经传完的文件名字
								
								
											if(m_cFile.Open(m_cstr, CFile::modeRead | CFile::typeBinary) == NULL){
												ERRORMSG;
												break;
											}else{
												
												memset(&dlg->m_sendMsg, 0 , sizeof(dlg->m_sendMsg));
												dlg->m_sendMsg.m_msgType = MSG_CHATFILETYPE;
												m_lct = new LCTMSG;
												m_cstr.Empty();
								
												m_cstr.Empty();
												m_cstr = CString(_com_util::ConvertBSTRToString((m_pRecordset->GetCollect(_variant_t(_T("Message")))).bstrVal));//获得已经传完的文件名字
							
												CString csTemp;
												AfxExtractSubString(csTemp, (LPCTSTR)m_cstr, 0, '_');
												m_lct->m_idFrom = _ttoi(csTemp);
												AfxExtractSubString(csTemp, (LPCTSTR)m_cstr, 1, '_');
												m_lct->m_cId = _ttoi(csTemp);
												AfxExtractSubString(csTemp, (LPCTSTR)m_cstr, 2, '_');
												m_lct->m_ownerId = _ttoi(csTemp);
												AfxExtractSubString(csTemp, (LPCTSTR)m_cstr, 3, '_');
												USES_CONVERSION;
												char* temp = W2A(csTemp);
												strcpy_s(m_lct->m_name, temp);
												AfxExtractSubString(csTemp, (LPCTSTR)m_cstr, 4, '_');
												
												temp = W2A(csTemp);
												strcpy_s(m_lct->m_text, temp);
												m_lct->m_fileLength = m_cFile.GetLength();
												memcpy(dlg->m_sendMsg.m_text, m_lct, sizeof(LCTMSG));
												memset(m_operation, 0, sizeof(m_operation));

												m_cFile.Close();
												dlg->iSend = send(m_socketConnected, (char*)&(dlg->m_sendMsg), sizeof(dlg->m_sendMsg), 0);
												if (dlg->iSend == SOCKET_ERROR) {
													ERRORMSG;
													return;
												}
												else if (dlg->iSend == 0)
													break;
												else{
			#ifdef DEBUG
				AfxMessageBox(_T("对方在线，群发文件消息已转发\n"));
			#endif
												
												}
												break;
												
											}
											
											delete m_lct;
										}else{//是文本消息
											dlg->m_sendMsg.m_msgType = MSG_CHATTXT;
											m_ct = new CTMSG;
											strcpy_s(m_ct->m_name,_com_util::ConvertBSTRToString((m_pRecordset->GetCollect(_variant_t(_T("Username")))).bstrVal));
											m_ct->m_ownerId = (m_pRecordset->GetCollect(_variant_t(_T("Founder")))).lVal;
											m_ct->m_idFrom = (m_pRecordset->GetCollect(_variant_t(_T("IDFrom")))).lVal;
											m_ct->m_cId = (m_pRecordset->GetCollect(_variant_t(_T("ChatRoomNo")))).lVal;
											strcpy_s(m_ct->m_text,_com_util::ConvertBSTRToString((m_pRecordset->GetCollect(_variant_t(_T("Message")))).bstrVal));
											memcpy(dlg->m_sendMsg.m_text, m_ct, sizeof(CTMSG));
											delete m_ct;
												
											dlg->iSend = send(m_socketConnected, (char*)&(dlg->m_sendMsg), sizeof(dlg->m_sendMsg), 0);
											if (dlg->iSend == SOCKET_ERROR) {
												ERRORMSG;
												return;
											}
											else if (dlg->iSend == 0)
												continue;
											else{
				#ifdef DEBUG
					AfxMessageBox(_T("离线聊天室文本消息,已回复\n"));
				#endif
											}
										}
										m_pRecordset->MoveNext();
									}
									if(m_pRecordset != NULL && m_pRecordset->GetState() == adStateOpen){
										m_pRecordset->Close();
									}
									memset(m_operation, 0, sizeof(m_operation));
									m_cstr.Format(_T("%d"),m_loginUser->m_id);
									m_pRecordset->CursorLocation = adUseClient;
									_tcscpy_s(m_operation,_T("SELECT U.Username, M.[IDFrom],[IDTo],[Message],[Time],[IsFile],[IsChatRoom] FROM [Talk2Me].[dbo].[Msg] M,[Talk2Me].[dbo].[User]  U WHERE M.IDFrom = U.ID AND M.IsChatRoom = 0 AND M.IDTo = "));
									_tcscat_s(m_operation, m_cstr);
									m_pRecordset->CursorLocation = adUseClient;
									m_pRecordset->Open(_variant_t(m_operation),  _variant_t  ((IDispatch*) m_pConnection,true), adOpenDynamic, adLockOptimistic, adCmdText); 
									//121对话消息
									while(!m_pRecordset->adoEOF){
										memset(&(dlg->m_sendMsg), 0, sizeof(dlg->m_sendMsg));
										//判断是文件还是文本消息，还是聊天室消息
										if((m_pRecordset->GetCollect(_variant_t(_T("IsFile")))).boolVal == -1){//是文件消息
											
											
											m_cstr.Empty();
											m_cstr = CString(FILESAVEPATH);
											m_cstr +=  CString(_com_util::ConvertBSTRToString((m_pRecordset->GetCollect(_variant_t(_T("Message")))).bstrVal));//获得已经传完的文件名字
								
											if(m_cFile.Open(m_cstr, CFile::modeRead | CFile::typeBinary) == NULL){
												ERRORMSG;
												break;
											}else{
												
												memset(&dlg->m_sendMsg, 0 , sizeof(dlg->m_sendMsg));
												dlg->m_sendMsg.m_msgType = MSG_FILETYPE;
												m_lt = new LTMSG;
												memset(m_lt, 0, sizeof(LTMSG));
												CString csTemp;
												m_cstr.Empty();
												m_cstr = CString(_com_util::ConvertBSTRToString((m_pRecordset->GetCollect(_variant_t(_T("Message")))).bstrVal));//获得已经传完的文件名字
							
												AfxExtractSubString(csTemp, (LPCTSTR)m_cstr, 0, '_');
												m_lt->m_idFrom = _ttoi(csTemp);
												AfxExtractSubString(csTemp, (LPCTSTR)m_cstr, 1, '_');
												m_lt->m_id = _ttoi(csTemp);
												AfxExtractSubString(csTemp, (LPCTSTR)m_cstr, 2, '_');
												USES_CONVERSION;
												char* temp = W2A(csTemp);
												strcpy_s(m_lt->m_name, temp);
												AfxExtractSubString(csTemp, (LPCTSTR)m_cstr, 3, '_');
												
												temp = W2A(csTemp);
												strcpy_s(m_lt->m_text, temp);
												m_lt->m_fileLength = m_cFile.GetLength();
												
												memcpy(dlg->m_sendMsg.m_text, m_lt, sizeof(LTMSG));
												m_cFile.Close();
												dlg->iSend = send(m_socketConnected, (char*)&(dlg->m_sendMsg), sizeof(dlg->m_sendMsg), 0);
												if (dlg->iSend == SOCKET_ERROR) {
													ERRORMSG;
													return;
												}
												else if (dlg->iSend == 0)
													break;
												else{
			#ifdef DEBUG
				AfxMessageBox(_T("对方在线，文件头消息已转发\n"));
			#endif
												
												}
											}
											
											delete m_lt;
											
												
										}else{//是文本消息
											//一对一文本消息
											dlg->m_sendMsg.m_msgType = MSG_TEXT;
											m_t = new TMSG;
											strcpy_s(m_t->m_name,_com_util::ConvertBSTRToString((m_pRecordset->GetCollect(_variant_t(_T("Username")))).bstrVal));
											m_t->m_id = m_loginUser->m_id;
											m_t->m_idFrom = (m_pRecordset->GetCollect(_variant_t(_T("IDFrom")))).lVal;
											strcpy_s(m_t->m_text,_com_util::ConvertBSTRToString((m_pRecordset->GetCollect(_variant_t(_T("Message")))).bstrVal));
											memcpy(dlg->m_sendMsg.m_text, m_t, sizeof(TMSG));
											delete m_t;
												
											dlg->iSend = send(m_socketConnected, (char*)&(dlg->m_sendMsg), sizeof(dlg->m_sendMsg), 0);
											if (dlg->iSend == SOCKET_ERROR) {
												ERRORMSG;
												return;
											}
											else if (dlg->iSend == 0)
												continue;
											else{
				#ifdef DEBUG
					AfxMessageBox(_T("离线文本消息,已回复\n"));
				#endif
											}
										}
										m_pRecordset->MoveNext();
									}
									if(m_pRecordset != NULL && m_pRecordset->GetState() == adStateOpen){
										m_pRecordset->Close();
									}
									memset(m_operation, 0, sizeof(m_operation));
									m_cstr.Format(_T("%d"),m_loginUser->m_id);
									m_pRecordset->CursorLocation = adUseClient;
									_tcscpy_s(m_operation,_T("DELETE FROM [Talk2Me].[dbo].[Msg] where IsFile = 0 AND [IDTo] = "));
									_tcscat_s(m_operation, m_cstr);
									m_pRecordset->CursorLocation = adUseClient;
									m_pRecordset->Open(_variant_t(m_operation),  _variant_t  ((IDispatch*) m_pConnection,true), adOpenStatic, adLockOptimistic, adCmdText); 
									
								
								}
							}catch(_com_error e){
								CString errormessage;  
								errormessage.Format( _T("select失败 !\r\n 错误信息 :%s(%ld)"),e.ErrorMessage(),e.Error() );   
								AfxMessageBox(errormessage);///显示错误信息 
							}
							if(m_pRecordset != NULL && m_pRecordset->GetState() == adStateOpen){
								m_pRecordset->Close();
							}
							delete m_user;
							delete m_loginUser;
							break;
						case MSG_SEARCH:
							m_loginUser = new USER_LOGIN;
							memcpy( &id, dlg->m_recvMsg.m_text, sizeof(id));
							m_cstr.Format(_T("%d"),id);
							m_loginUser->m_id = id;
							_tcscpy_s(m_operation,_T("SELECT * FROM [Talk2Me].[dbo].[User] where [Talk2Me].[dbo].[User].ID = "));
							_tcscat_s(m_operation, m_cstr);
							try{
								m_pRecordset->CursorLocation = adUseClient;
								m_pRecordset->Open(_variant_t(m_operation),  _variant_t  ((IDispatch*) m_pConnection,true), adOpenDynamic, adLockOptimistic, adCmdText); 
								dlg->m_sendMsg.m_msgType = MSG_SEARCH_RP;
								if(m_pRecordset->GetRecordCount() == 1){
									strcpy_s(m_loginUser->m_pwd,_com_util::ConvertBSTRToString((m_pRecordset->GetCollect(_variant_t(_T("Username")))).bstrVal));
									memcpy(dlg->m_sendMsg.m_text,m_loginUser,sizeof(USER_LOGIN));

								}else{
									memcpy(dlg->m_sendMsg.m_text,"查找失败!",sizeof("查找失败!"));
								}
								dlg->iSend = send(m_socketConnected, (char*)&(dlg->m_sendMsg), sizeof(dlg->m_sendMsg), 0);
								if (dlg->iSend == SOCKET_ERROR) {
									ERRORMSG;
									return;
								}
								else if (dlg->iSend == 0)
									continue;
								else{
	#ifdef DEBUG
		AfxMessageBox(_T("查找,已回复\n"));
	#endif
								}
							}
							catch(_com_error e)
							{
								CString errormessage;  
								errormessage.Format( _T("selectID失败 !\r\n 错误信息 :%s(%ld)"),e.ErrorMessage(),e.Error() );   
								AfxMessageBox(errormessage);///显示错误信息 
							}
							if(m_pRecordset != NULL && m_pRecordset->GetState() == adStateOpen){
								m_pRecordset->Close();
							}
							delete m_loginUser;
							break;
						case MSG_TEXT:
							m_t = new TMSG;
							memset(m_t, 0, sizeof(TMSG));
							memcpy(m_t, dlg->m_recvMsg.m_text, sizeof(TMSG));
							m_cstr.Format(_T("%d"),m_t->m_id);
							_tcscpy_s(m_operation,_T("SELECT * FROM [Talk2Me].[dbo].[User] where [Talk2Me].[dbo].[User].ID = "));
							_tcscat_s(m_operation, m_cstr);
							try{
								m_pRecordset->CursorLocation = adUseClient;
								m_pRecordset->Open(_variant_t(m_operation),  _variant_t  ((IDispatch*) m_pConnection,true), adOpenDynamic, adLockOptimistic, adCmdText); 
								//
								if(m_pRecordset->GetRecordCount() == 1){
									var = m_pRecordset->GetCollect(_variant_t(_T("OnlineState")));
									//m_cstr.Format(_T("%ld"),var.lVal);
									if(var.boolVal == -1){//在线
										dlg->m_sendMsg.m_msgType = MSG_TEXT;
										for(map<int,SOCKET>::iterator first = dlg->m_OnlineClients.begin(); first != dlg->m_OnlineClients.end(); ++first){
											if(first->first == m_t->m_id){
												memcpy(dlg->m_sendMsg.m_text,dlg->m_recvMsg.m_text,sizeof(dlg->m_recvMsg.m_text));
												dlg->iSend = send(first->second, (char*)&(dlg->m_sendMsg), sizeof(dlg->m_sendMsg), 0);
												if (dlg->iSend == SOCKET_ERROR) {
													ERRORMSG;
													return;
												}
												else if (dlg->iSend == 0)
													continue;
												else{
			#ifdef DEBUG
				AfxMessageBox(_T("对方在线，消息已转发\n"));
			#endif
												}
												break;
											}

										}//
											
									}else{
										memset(m_operation, 0, sizeof(m_operation));
										_tcscpy_s(m_operation,_T("INSERT INTO [Talk2Me].[dbo].[Msg]([IDFrom],[IDTo],[Message],[IsFile],[IsChatRoom],[ChatRoomNo])VALUES("));
										m_cstr.Format(_T("%d"),m_t->m_idFrom);
										_tcscat_s(m_operation, m_cstr);
										_tcscat_s(m_operation,_T(","));
										m_cstr.Format(_T("%d"),m_t->m_id);
										_tcscat_s(m_operation,m_cstr);
										_tcscat_s(m_operation,_T(",'"));
									
										_tcscat_s(m_operation,CString(m_t->m_text));
										_tcscat_s(m_operation,_T("',0,0,-1)"));
										if(m_pRecordset != NULL && m_pRecordset->GetState() == adStateOpen){
											m_pRecordset->Close();
										}
										m_pRecordset->Open(_variant_t(m_operation),  _variant_t  ((IDispatch*) m_pConnection,true), adOpenStatic, adLockOptimistic, adCmdText); 
									}
								}else{//对方不存在，不太可能
									//memcpy(dlg->m_sendMsg.m_text,"登陆失败!",sizeof("登陆失败!"));
								}
							}catch(_com_error e){
								CString errormessage;  
								errormessage.Format( _T("select失败 !\r\n 错误信息 :%s(%ld)"),e.ErrorMessage(),e.Error() );   
								AfxMessageBox(errormessage);///显示错误信息 
							}
							if(m_pRecordset != NULL && m_pRecordset->GetState() == adStateOpen){
								m_pRecordset->Close();
							}
							delete m_t;
							break;
						case MSG_FILETYPE:
							m_t = new TMSG;
							memcpy(m_t, dlg->m_recvMsg.m_text, sizeof(TMSG));
							m_cstr.Format(_T("%d"),m_t->m_id);
							_tcscpy_s(m_operation,_T("SELECT * FROM [Talk2Me].[dbo].[User] where [Talk2Me].[dbo].[User].ID = "));
							_tcscat_s(m_operation, m_cstr);

							try{
								m_pRecordset->CursorLocation = adUseClient;
								m_pRecordset->Open(_variant_t(m_operation),  _variant_t  ((IDispatch*) m_pConnection,true), adOpenDynamic, adLockOptimistic, adCmdText); 
								//
								if(m_pRecordset->GetRecordCount() == 1){//用户存在

									//保存文件
									m_cstr.Empty();
									m_cstr = CString(FILESAVEPATH);
									m_cstr += CString(m_t->m_text);
									if(m_cFile.Open(m_cstr , CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite | CFile::typeBinary) == NULL){
											ERRORMSG;
											break;
									}else{
										m_cFile.Close();
									}
										
									memset(m_operation, 0, sizeof(m_operation));
									_tcscpy_s(m_operation,_T("INSERT INTO [Talk2Me].[dbo].[Msg]([IDFrom],[IDTo],[Message],[IsFile],[IsChatRoom],[ChatRoomNo])VALUES("));
									m_cstr.Format(_T("%d"),m_t->m_idFrom);
									_tcscat_s(m_operation, m_cstr);
									_tcscat_s(m_operation,_T(","));
									m_cstr.Format(_T("%d"),m_t->m_id);
									_tcscat_s(m_operation,m_cstr);
									_tcscat_s(m_operation,_T(",'"));
									
									_tcscat_s(m_operation,CString(m_t->m_text));//文件名字;idfrom_idto_name_名字.
									_tcscat_s(m_operation,_T("',1,0,-1)"));


									if(m_pRecordset != NULL && m_pRecordset->GetState() == adStateOpen){
										m_pRecordset->Close();
									}
									m_pRecordset->Open(_variant_t(m_operation),  _variant_t  ((IDispatch*) m_pConnection,true), adOpenStatic, adLockOptimistic, adCmdText); 
									
								}else{//对方不存在，不太可能
									//memcpy(dlg->m_sendMsg.m_text,"登陆失败!",sizeof("登陆失败!"));
								}
							}catch(_com_error e){
								CString errormessage;  
								errormessage.Format( _T("select失败 !\r\n 错误信息 :%s(%ld)"),e.ErrorMessage(),e.Error() );   
								AfxMessageBox(errormessage);///显示错误信息 
							}
							if(m_pRecordset != NULL && m_pRecordset->GetState() == adStateOpen){
								m_pRecordset->Close();
							}
							delete m_t;
							break;
						case MSG_CHATFILETYPE:
							m_2int = new int[100];
							id = 0;
							m_ct = new CTMSG;
							memcpy(m_ct, dlg->m_recvMsg.m_text, sizeof(CTMSG));
							m_cstr.Empty();
							m_cstr = CString(FILESAVEPATH);
							m_cstr += CString(m_ct->m_text);
							if(m_cFile.Open(m_cstr , CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite | CFile::typeBinary) == NULL){
									ERRORMSG;
									break;
							}else{
								m_cFile.Close();
							}
							m_cstr.Format(_T("%d"),m_ct->m_cId);
							_tcscpy_s(m_operation,_T("SELECT * FROM [Talk2Me].[dbo].[Chat] where ChatRoomNo = "));
							_tcscat_s(m_operation, m_cstr);
							try{
								m_pRecordset->CursorLocation = adUseClient;
								m_pRecordset->Open(_variant_t(m_operation),  _variant_t  ((IDispatch*) m_pConnection,true), adOpenDynamic, adLockOptimistic, adCmdText); 
								while(!m_pRecordset->adoEOF){
									m_2int[id] = (m_pRecordset->GetCollect(_variant_t(_T("ID")))).lVal;
									++id;
									m_pRecordset->MoveNext();
								}
								for(int i = 0; i < id; ++i){
									if(m_2int[i] == m_ct->m_idFrom)
										continue;
									memset(m_operation, 0, sizeof(m_operation));
									_tcscpy_s(m_operation,_T("INSERT INTO [Talk2Me].[dbo].[Msg]([IDFrom],[IDTo],[Message],[IsFile],[IsChatRoom],[ChatRoomNo])VALUES("));
									m_cstr.Format(_T("%d"),m_ct->m_idFrom);
									_tcscat_s(m_operation, m_cstr);
									_tcscat_s(m_operation,_T(","));
									m_cstr.Format(_T("%ld"),m_2int[i]);
									_tcscat_s(m_operation,m_cstr);
									_tcscat_s(m_operation,_T(",'"));
									
									_tcscat_s(m_operation,CString(m_ct->m_text));
									_tcscat_s(m_operation,_T("',1,1,"));
									m_cstr.Format(_T("%d"),m_ct->m_cId);
									_tcscat_s(m_operation,m_cstr);
									_tcscat_s(m_operation,_T(")"));
									if(m_pRecordset != NULL && m_pRecordset->GetState() == adStateOpen){
										m_pRecordset->Close();
									}
									m_pRecordset->Open(_variant_t(m_operation),  _variant_t  ((IDispatch*) m_pConnection,true), adOpenStatic, adLockOptimistic, adCmdText);								
										
								}
							}catch(_com_error e){
								CString errormessage;  
								errormessage.Format( _T("select chatroom失败 !\r\n 错误信息 :%s(%ld)"),e.ErrorMessage(),e.Error() );   
								AfxMessageBox(errormessage);///显示错误信息 
							}
							if(m_pRecordset != NULL && m_pRecordset->GetState() == adStateOpen){
								m_pRecordset->Close();
							}
							delete m_ct;
							delete[] m_2int;
							break;
						case MSG_FILE:
							m_f = new FMSG;
							memset(m_f, 0, sizeof(FMSG));
							memcpy(m_f, dlg->m_recvMsg.m_text, sizeof(FMSG));
							m_cstr.Empty();
							m_cstr = CString(FILESAVEPATH);
							m_cstr += CString(m_f->m_filename);
							if(m_cFile.Open(m_cstr , CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite | CFile::typeBinary) == NULL){
								ERRORMSG;
								break;
							}else{
								m_cFile.SeekToEnd();
								m_cFile.Write(m_f->m_text, sizeof(m_f->m_text));
								m_cFile.Close();
							}
							delete m_f;
							break;
						case MSG_FILEEND://一对一文件结束
							m_cstr.Empty();
							m_cstr = CString(FILESAVEPATH);
							m_cstr +=  CString(dlg->m_recvMsg.m_text);//获得已经传完的文件名字
								
								
							if(m_cFile.Open(m_cstr, CFile::modeRead | CFile::typeBinary) == NULL){
								ERRORMSG;
								break;
							}else{
								
								memset(&dlg->m_sendMsg, 0 , sizeof(dlg->m_sendMsg));
								dlg->m_sendMsg.m_msgType = MSG_FILETYPE;
								m_lt = new LTMSG;
								memset(m_lt, 0, sizeof(LTMSG));
								CString csTemp;
								m_cstr.Empty();
								m_cstr =  CString(dlg->m_recvMsg.m_text);//获得已经传完的文件名字
								AfxExtractSubString(csTemp, (LPCTSTR)m_cstr, 0, '_');
								m_lt->m_idFrom = _ttoi(csTemp);
								AfxExtractSubString(csTemp, (LPCTSTR)m_cstr, 1, '_');
								m_lt->m_id = _ttoi(csTemp);
								AfxExtractSubString(csTemp, (LPCTSTR)m_cstr, 2, '_');
								USES_CONVERSION;
								char* temp = W2A(csTemp);
								strcpy_s(m_lt->m_name, temp);
								AfxExtractSubString(csTemp, (LPCTSTR)m_cstr, 3, '_');
								
								temp = W2A(csTemp);
								strcpy_s(m_lt->m_text, temp);
								m_lt->m_fileLength = m_cFile.GetLength();
								memcpy(dlg->m_sendMsg.m_text, m_lt, sizeof(LTMSG));
								m_cFile.Close();	
								for(map<int,SOCKET>::iterator first = dlg->m_OnlineClients.begin(); first != dlg->m_OnlineClients.end(); ++first){
									if(first->first == m_lt->m_id){
										//发送文件信息到对方客户端,
										dlg->iSend = send(first->second, (char*)&(dlg->m_sendMsg), sizeof(dlg->m_sendMsg), 0);
										if (dlg->iSend == SOCKET_ERROR) {
											ERRORMSG;
											return;
										}
										else if (dlg->iSend == 0)
											break;
										else{
	#ifdef DEBUG
		AfxMessageBox(_T("对方在线，文件头消息已转发\n"));
	#endif
												
										}
										break;
									}
								}
							}
							delete m_lt;
							
							break;
						case MSG_RECVFILE:
							m_f = new FMSG;
							m_loginUser = new USER_LOGIN;//用户的获得接收文件的人的id
							memset(m_f, 0, sizeof(FMSG));
							memset(m_loginUser, 0, sizeof(USER_LOGIN));
							memcpy(m_loginUser,dlg->m_recvMsg.m_text, sizeof(USER_LOGIN));
							memcpy(m_f->m_filename, m_loginUser->m_pwd, sizeof(m_f->m_filename));
							m_cstr.Empty();
							m_cstr =  CString(FILESAVEPATH);
							m_cstr +=  CString(m_loginUser->m_pwd);//获得接收的文件名字,文件命名规则:发送者id_接收者id_发送者姓名_文件名字
							
							if(m_cFile.Open(m_cstr, CFile::modeRead | CFile::typeBinary) == NULL){
								ERRORMSG;
								break;
							}else{
								

								memset(&dlg->m_sendMsg, 0 , sizeof(dlg->m_sendMsg));
								dlg->m_sendMsg.m_msgType = MSG_FILE;
								while( m_cFile.Read(m_f->m_text,sizeof(m_f->m_text)) != 0){
									memset(dlg->m_sendMsg.m_text, 0, sizeof(dlg->m_sendMsg.m_text));
									memcpy(dlg->m_sendMsg.m_text, m_f, sizeof(FMSG));
									memset(m_f->m_text, 0, sizeof(m_f->m_text));
									dlg->iSend = send(m_socketConnected, (char*)&dlg->m_sendMsg, sizeof(dlg->m_sendMsg), 0);
									if (dlg->iSend == SOCKET_ERROR) {
										ERRORMSG;
		 
									}
									else if (dlg->iSend == 0)
										break;
									else{
			
									}
									memset(dlg->m_sendMsg.m_text, 0 , sizeof(dlg->m_sendMsg.m_text));
								}
								m_cFile.Close();
								DeleteFile(m_cstr);
								memset(&dlg->m_sendMsg, 0 , sizeof(dlg->m_sendMsg));
								dlg->m_sendMsg.m_msgType = MSG_FILEEND;
								memcpy(dlg->m_sendMsg.m_text,m_loginUser->m_pwd, sizeof(m_loginUser->m_pwd));
												
								dlg->iSend = send(m_socketConnected, (char*)&dlg->m_sendMsg, sizeof(dlg->m_sendMsg), 0);
								if (dlg->iSend == SOCKET_ERROR) {
									ERRORMSG;
		 
								}
								else if (dlg->iSend == 0)
									break;
								else{}
								//从数据库中删除文件
								//从文件中删除
								
								CString csTemp;
								
								memset(m_operation, 0, sizeof(m_operation));
								_tcscpy_s(m_operation,_T("DELETE FROM [Talk2Me].[dbo].[Msg] WHERE [Message] = '"));
								_tcscat_s(m_operation,CString(m_loginUser->m_pwd));
								_tcscat_s(m_operation,_T("'"));
								
								m_pRecordset->Open(_variant_t(m_operation),  _variant_t  ((IDispatch*) m_pConnection,true), adOpenStatic, adLockOptimistic, adCmdText); 
								
							}
							
							if(m_pRecordset != NULL && m_pRecordset->GetState() == adStateOpen){
								m_pRecordset->Close();
							}
							
							delete m_f;
							break;
						case MSG_NEXTTIME:
							//什么都不做
							break;
						case MSG_CANCELFILE:
							//文件取消接收,直接删除
							m_loginUser = new USER_LOGIN;//用户的获得取消接收文件的人的id
							
							memset(m_loginUser, 0, sizeof(USER_LOGIN));
							memcpy(m_loginUser,dlg->m_recvMsg.m_text, sizeof(USER_LOGIN));
							m_cstr.Empty();
							m_cstr =  CString(FILESAVEPATH);
							m_cstr +=  CString(m_loginUser->m_pwd);//获得取消接收的文件名字,文件命名规则:发送者id_接收者id_发送者姓名_文件名字

							if(m_cFile.Open(m_cstr, CFile::modeRead | CFile::typeBinary) == NULL){
								ERRORMSG;
								break;
							}else{
								//从文件中删除
								m_cFile.Close();
								DeleteFile(m_cstr);
								memset(m_operation, 0, sizeof(m_operation));
								_tcscpy_s(m_operation,_T("DELETE FROM [Talk2Me].[dbo].[Msg] WHERE [Message] = '"));
								_tcscat_s(m_operation,CString(dlg->m_recvMsg.m_text));
								_tcscat_s(m_operation,_T("'"));
								
								m_pRecordset->Open(_variant_t(m_operation),  _variant_t  ((IDispatch*) m_pConnection,true), adOpenStatic, adLockOptimistic, adCmdText); 
							}
							if(m_pRecordset != NULL && m_pRecordset->GetState() == adStateOpen){
								m_pRecordset->Close();
							}		
							break;
						case MSG_CHATFILEEND:
							
							m_cstr.Empty();
							m_cstr = CString(FILESAVEPATH);
							m_cstr +=  CString(dlg->m_recvMsg.m_text);//获得已经传完的文件名字
								
								
							if(m_cFile.Open(m_cstr, CFile::modeRead | CFile::typeBinary) == NULL){
								ERRORMSG;
								break;
							}else{
								
								memset(&dlg->m_sendMsg, 0 , sizeof(dlg->m_sendMsg));
								dlg->m_sendMsg.m_msgType = MSG_CHATFILETYPE;
								m_lct = new LCTMSG;
								m_cstr.Empty();
								
								m_cstr =  CString(dlg->m_recvMsg.m_text);//获得已经传完的文件名字
								CString csTemp;
								AfxExtractSubString(csTemp, (LPCTSTR)m_cstr, 0, '_');
								m_lct->m_idFrom = _ttoi(csTemp);
								AfxExtractSubString(csTemp, (LPCTSTR)m_cstr, 1, '_');
								m_lct->m_cId = _ttoi(csTemp);
								AfxExtractSubString(csTemp, (LPCTSTR)m_cstr, 2, '_');
								m_lct->m_ownerId = _ttoi(csTemp);
								AfxExtractSubString(csTemp, (LPCTSTR)m_cstr, 3, '_');
								USES_CONVERSION;
								char* temp = W2A(csTemp);
								strcpy_s(m_lct->m_name, temp);
								AfxExtractSubString(csTemp, (LPCTSTR)m_cstr, 4, '_');
								
								temp = W2A(csTemp);
								strcpy_s(m_lct->m_text, temp);
								m_lct->m_fileLength = m_cFile.GetLength();
								memcpy(dlg->m_sendMsg.m_text, m_lct, sizeof(LCTMSG));
								memset(m_operation, 0, sizeof(m_operation));
								m_cFile.Close();
								m_cstr.Format(_T("%d"),m_lct->m_cId);
								_tcscpy_s(m_operation,_T("SELECT * FROM [Talk2Me].[dbo].[Chat] where ChatRoomNo = "));
								_tcscat_s(m_operation, m_cstr);
								try{
									m_pRecordset->CursorLocation = adUseClient;
									m_pRecordset->Open(_variant_t(m_operation),  _variant_t  ((IDispatch*) m_pConnection,true), adOpenDynamic, adLockOptimistic, adCmdText); 
									while(!m_pRecordset->adoEOF){
										if((m_pRecordset->GetCollect(_variant_t(_T("ID")))).lVal == m_lct->m_idFrom){
											m_pRecordset->MoveNext();
											continue;
										}
										for(map<int,SOCKET>::iterator first = dlg->m_OnlineClients.begin(); first != dlg->m_OnlineClients.end(); ++first){
											if(first->first == (m_pRecordset->GetCollect(_variant_t(_T("ID")))).lVal){
												dlg->iSend = send(first->second, (char*)&(dlg->m_sendMsg), sizeof(dlg->m_sendMsg), 0);
												if (dlg->iSend == SOCKET_ERROR) {
													ERRORMSG;
													return;
												}
												else if (dlg->iSend == 0)
													break;
												else{
			#ifdef DEBUG
				AfxMessageBox(_T("对方在线，群发文件消息已转发\n"));
			#endif
												
												}
												break;
											}
										}
										m_pRecordset->MoveNext();
									}
								}catch(_com_error e){
									CString errormessage;  
									errormessage.Format( _T("select chatroom失败 !\r\n 错误信息 :%s(%ld)"),e.ErrorMessage(),e.Error() );   
									AfxMessageBox(errormessage);///显示错误信息 
								}

							}
							
							delete m_lct;
							if(m_pRecordset != NULL && m_pRecordset->GetState() == adStateOpen){
								m_pRecordset->Close();
							}
							break;
						case MSG_RECVCHATFILE:
							m_f = new FMSG;
							m_loginUser = new USER_LOGIN;//用户的获得接收文件的人的id
							memset(m_f, 0, sizeof(FMSG));
							memset(m_loginUser, 0, sizeof(USER_LOGIN));
							memcpy(m_loginUser,dlg->m_recvMsg.m_text, sizeof(USER_LOGIN));
							memcpy(m_f->m_filename, m_loginUser->m_pwd, sizeof(m_f->m_filename));
							m_cstr.Empty();
							m_cstr =  CString(FILESAVEPATH);
							m_cstr +=  CString(m_loginUser->m_pwd);//获得接收的文件名字,文件命名规则:发送者id_接收者id_发送者姓名_文件名字
							if(m_cFile.Open(m_cstr, CFile::modeRead | CFile::typeBinary) == NULL){
								ERRORMSG;
								break;
							}else{
								
								memset(&dlg->m_sendMsg, 0 , sizeof(dlg->m_sendMsg));
								dlg->m_sendMsg.m_msgType = MSG_FILE;
								while(m_cFile.Read(m_f->m_text,sizeof(m_f->m_text)) != 0){
									memset(dlg->m_sendMsg.m_text, 0, sizeof(dlg->m_sendMsg.m_text));
									memcpy(dlg->m_sendMsg.m_text, m_f, sizeof(FMSG));
									memset(m_f->m_text, 0, sizeof(m_f->m_text));
									dlg->iSend = send(m_socketConnected, (char*)&dlg->m_sendMsg, sizeof(dlg->m_sendMsg), 0);
									if (dlg->iSend == SOCKET_ERROR) {
										ERRORMSG;
		 
									}
									else if (dlg->iSend == 0)
										break;
									else{
			
									}
									memset(dlg->m_sendMsg.m_text, 0 , sizeof(dlg->m_sendMsg.m_text));
								}
								m_cFile.Close();
								memset(&dlg->m_sendMsg, 0 , sizeof(dlg->m_sendMsg));
								dlg->m_sendMsg.m_msgType = MSG_CHATFILEEND;
								memcpy(dlg->m_sendMsg.m_text,m_loginUser->m_pwd, sizeof(m_loginUser->m_pwd));
												
								dlg->iSend = send(m_socketConnected, (char*)&dlg->m_sendMsg, sizeof(dlg->m_sendMsg), 0);
								if (dlg->iSend == SOCKET_ERROR) {
									ERRORMSG;
		 
								}
								else if (dlg->iSend == 0)
									break;
								else{
									
								//从文件中删除
								
								DeleteFile(m_cstr);
								memset(m_operation, 0, sizeof(m_operation));
								_tcscpy_s(m_operation,_T("DELETE FROM [Talk2Me].[dbo].[Msg] WHERE [Message] = '"));
								_tcscat_s(m_operation,CString(m_f->m_filename));
								_tcscat_s(m_operation,_T("' AND [IDTo] = "));

								m_cstr.Empty();
								m_cstr.Format(_T("%d"),m_loginUser->m_id);
								_tcscat_s(m_operation,m_cstr);
								if(m_pRecordset != NULL && m_pRecordset->GetState() == adStateOpen){
									m_pRecordset->Close();
								}
								m_pRecordset->Open(_variant_t(m_operation),  _variant_t  ((IDispatch*) m_pConnection,true), adOpenStatic, adLockOptimistic, adCmdText); 
						

							}
							if(m_pRecordset != NULL && m_pRecordset->GetState() == adStateOpen){
								m_pRecordset->Close();
							}
							delete m_f;						
							delete m_loginUser;					
							break;
						case MSG_CANCELCHATFILE:
							m_loginUser = new USER_LOGIN;//用户的获得取消接收文件的人的id
							
							memset(m_loginUser, 0, sizeof(USER_LOGIN));
							memcpy(m_loginUser,dlg->m_recvMsg.m_text, sizeof(USER_LOGIN));
							m_cstr.Empty();
							m_cstr =  CString(FILESAVEPATH);
							m_cstr +=  CString(m_loginUser->m_pwd);//获得quxiao接收的文件名字,文件命名规则:发送者id_聊天室id_聊天室创建者_发送者姓名_文件名字
							if(m_cFile.Open(m_cstr, CFile::modeRead | CFile::typeBinary) == NULL){
								ERRORMSG;
								break;
							}else{
								//向发送者说文件已被取消接收
								m_cFile.Close();
						
								
								//从文件中删除
								
								
								memset(m_operation, 0, sizeof(m_operation));
								_tcscpy_s(m_operation,_T("DELETE FROM [Talk2Me].[dbo].[Msg] WHERE [Message] = '"));
								_tcscat_s(m_operation,CString(m_loginUser->m_pwd));
								_tcscat_s(m_operation,_T("' AND [IDTo] = "));
								m_cstr.Empty();
								m_cstr.Format(_T("%d"),m_loginUser->m_id);
								_tcscat_s(m_operation,m_cstr);
								if(m_pRecordset != NULL && m_pRecordset->GetState() == adStateOpen){
									m_pRecordset->Close();
								}
								m_pRecordset->Open(_variant_t(m_operation),  _variant_t  ((IDispatch*) m_pConnection,true), adOpenStatic, adLockOptimistic, adCmdText); 
						

							}
							if(m_pRecordset != NULL && m_pRecordset->GetState() == adStateOpen){
								m_pRecordset->Close();
							}
											
							delete m_loginUser;	
							break;
						case MSG_CHATTXT://聊天室消息
							m_2int = new int[100];
							id = 0;
							m_ct = new CTMSG;
							memcpy(m_ct, dlg->m_recvMsg.m_text, sizeof(CTMSG));
							m_cstr.Format(_T("%d"),m_ct->m_cId);
							_tcscpy_s(m_operation,_T("SELECT * FROM [Talk2Me].[dbo].[Chat] where ChatRoomNo = "));
							_tcscat_s(m_operation, m_cstr);
							try{
								m_pRecordset->CursorLocation = adUseClient;
								m_pRecordset->Open(_variant_t(m_operation),  _variant_t  ((IDispatch*) m_pConnection,true), adOpenDynamic, adLockOptimistic, adCmdText); 
								while(!m_pRecordset->adoEOF){
									map<int,SOCKET>::iterator first;
									for( first = dlg->m_OnlineClients.begin(); first != dlg->m_OnlineClients.end(); ++first){
										if(first->first == (m_pRecordset->GetCollect(_variant_t(_T("ID")))).lVal){
											memset(&(dlg->m_sendMsg), 0, sizeof(dlg->m_sendMsg));
											dlg->m_sendMsg.m_msgType = MSG_CHATTXT;
											memcpy(dlg->m_sendMsg.m_text,dlg->m_recvMsg.m_text,sizeof(dlg->m_recvMsg.m_text));
											dlg->iSend = send(first->second, (char*)&(dlg->m_sendMsg), sizeof(dlg->m_sendMsg), 0);
											if (dlg->iSend == SOCKET_ERROR) {
												ERRORMSG;
												return;
											}
											else if (dlg->iSend == 0)
												continue;
											else{
		#ifdef DEBUG
			AfxMessageBox(_T("对方在线，消息已转发\n"));
		#endif
											}
											break;
										}

									}
									if(first == dlg->m_OnlineClients.end()){
										m_2int[id] = (m_pRecordset->GetCollect(_variant_t(_T("ID")))).lVal;
										++id;
									}
										
									m_pRecordset->MoveNext();
								}
								for(int i = 0; i < id; ++i){//当前不在线的用户,消息存入数据库
									memset(m_operation, 0, sizeof(m_operation));
									_tcscpy_s(m_operation,_T("INSERT INTO [Talk2Me].[dbo].[Msg]([IDFrom],[IDTo],[Message],[IsFile],[IsChatRoom],[ChatRoomNo])VALUES("));
									m_cstr.Format(_T("%d"),m_ct->m_idFrom);
									_tcscat_s(m_operation, m_cstr);
									_tcscat_s(m_operation,_T(","));
									m_cstr.Format(_T("%ld"),m_2int[i]);
									_tcscat_s(m_operation,m_cstr);
									_tcscat_s(m_operation,_T(",'"));
									
									_tcscat_s(m_operation,CString(m_ct->m_text));
									_tcscat_s(m_operation,_T("',0,1,"));
									m_cstr.Format(_T("%d"),m_ct->m_cId);
									_tcscat_s(m_operation,m_cstr);
									_tcscat_s(m_operation,_T(")"));
									if(m_pRecordset != NULL && m_pRecordset->GetState() == adStateOpen){
										m_pRecordset->Close();
									}
									m_pRecordset->Open(_variant_t(m_operation),  _variant_t  ((IDispatch*) m_pConnection,true), adOpenStatic, adLockOptimistic, adCmdText);								
										
								}
							}catch(_com_error e){
								CString errormessage;  
								errormessage.Format( _T("select chatroom失败 !\r\n 错误信息 :%s(%ld)"),e.ErrorMessage(),e.Error() );   
								AfxMessageBox(errormessage);///显示错误信息 
							}
							if(m_pRecordset != NULL && m_pRecordset->GetState() == adStateOpen){
								m_pRecordset->Close();
							}
							delete m_ct;
							delete[] m_2int;
							break;
						case MSG_BROADCAST://查询所有注册用户
							m_loginUser = new USER_LOGIN[10];//10*100 = 1000 包含id和密码,每次发十条
							_tcscpy_s(m_operation,_T("SELECT * FROM [Talk2Me].[dbo].[User]"));
						
							try{
								m_pRecordset->CursorLocation = adUseClient;
								m_pRecordset->Open(_variant_t(m_operation),  _variant_t  ((IDispatch*) m_pConnection,true), adOpenDynamic, adLockOptimistic, adCmdText); 
								dlg->m_sendMsg.m_msgType = MSG_BROADCAST_RP;
								while(!m_pRecordset->adoEOF){
									memset(&(dlg->m_sendMsg.m_text), 0, sizeof(dlg->m_sendMsg.m_text));
									memset(m_loginUser,0, 10 *sizeof(USER_LOGIN));
								
									for(int i = 0; i < 10; ++i){
									
										strcpy_s(m_loginUser[i].m_pwd,_com_util::ConvertBSTRToString((m_pRecordset->GetCollect(_variant_t(_T("Username")))).bstrVal));
										m_loginUser[i].m_id =  m_pRecordset->GetCollect(_variant_t(_T("ID"))).lVal;
										if( m_pRecordset->GetCollect(_variant_t(_T("OnlineState"))).boolVal == 0){
											m_loginUser[i].m_id *= -1;//负值的id表示不在线用户
										}
										m_pRecordset->MoveNext();
										if(m_pRecordset->adoEOF){
											break;
										}
									}
									memcpy(dlg->m_sendMsg.m_text, m_loginUser, 10 *sizeof(USER_LOGIN));
									dlg->iSend = send(m_socketConnected, (char*)&(dlg->m_sendMsg), sizeof(dlg->m_sendMsg), 0);
									if (dlg->iSend == SOCKET_ERROR) {
										ERRORMSG;
										return;
									}
									else if (dlg->iSend == 0)
										continue;
									else{
		#ifdef DEBUG
			AfxMessageBox(_T("广播,已回复\n"));
		#endif
									}
								}
							
							}
							catch(_com_error e)
							{
								CString errormessage;  
								errormessage.Format( _T("broadcast失败 !\r\n 错误信息 :%s(%ld)"),e.ErrorMessage(),e.Error() );   
								AfxMessageBox(errormessage);///显示错误信息 
							}
							if(m_pRecordset != NULL && m_pRecordset->GetState() == adStateOpen){
								m_pRecordset->Close();
							}
							delete[] m_loginUser;
							break;
						case MSG_CHATBROADCAST://查询某个用户参与的所有聊天室
							m_2int = new int[126];//126*8 = 1008,可以存放63个聊天室信息：偶数表示聊天室id，奇数表示建立者
							memset(m_2int, -1, 126*sizeof(int));
							id = 0;
							memcpy(&id, dlg->m_recvMsg.m_text,sizeof(id));
							m_cstr.Format(_T("%d"),id);
							_tcscpy_s(m_operation,_T("SELECT C.ChatRoomNo , C.ID,  U.Founder FROM [Talk2Me].[dbo].[Chat] C ,[Talk2Me].[dbo].ChatRoom U WHERE C.ChatRoomNo = U.ChatRoomNo AND C.ID ="));
							_tcscat_s(m_operation,m_cstr);
							try{
								m_pRecordset->CursorLocation = adUseClient;
								m_pRecordset->Open(_variant_t(m_operation),  _variant_t  ((IDispatch*) m_pConnection,true), adOpenDynamic, adLockOptimistic, adCmdText); 
								dlg->m_sendMsg.m_msgType = MSG_CHATBROADCAST_RP;
								while(!m_pRecordset->adoEOF){
									memset(&(dlg->m_sendMsg.m_text), 0, sizeof(dlg->m_sendMsg.m_text));
										
								
									for(int i = 0; i < 126; ++i){
									
										m_2int[i] =  m_pRecordset->GetCollect(_variant_t(_T("ChatRoomNo"))).lVal;
										m_2int[++i] =  m_pRecordset->GetCollect(_variant_t(_T("Founder"))).lVal;
										m_pRecordset->MoveNext();
										if(m_pRecordset->adoEOF){
											break;
										}
									}
									memcpy(dlg->m_sendMsg.m_text, m_2int, 126 *sizeof(int));
									dlg->iSend = send(m_socketConnected, (char*)&(dlg->m_sendMsg), sizeof(dlg->m_sendMsg), 0);
									if (dlg->iSend == SOCKET_ERROR) {
										ERRORMSG;
										return;
									}
									else if (dlg->iSend == 0)
										continue;
									else{
		#ifdef DEBUG
			AfxMessageBox(_T("聊天室广播,已回复\n"));
		#endif
									}
								}
							
							}
							catch(_com_error e)
							{
								CString errormessage;  
								errormessage.Format( _T("broadcast失败 !\r\n 错误信息 :%s(%ld)"),e.ErrorMessage(),e.Error() );   
								AfxMessageBox(errormessage);///显示错误信息 
							}
							if(m_pRecordset != NULL && m_pRecordset->GetState() == adStateOpen){
								m_pRecordset->Close();
							}
							delete[] m_2int;
							break;
						case MSG_SEARCHCHAT://查询当前聊天室包含的所有人
							m_loginUser = new USER_LOGIN[10];//10*100 = 1000
							id = 0;
							memcpy(&id, dlg->m_recvMsg.m_text,sizeof(id));
							m_cstr.Format(_T("%d"),id);
							_tcscpy_s(m_operation,_T("SELECT [ChatRoomNo],U.ID,U.OnlineState,U.Username  FROM [Talk2Me].[dbo].[Chat]C, [Talk2Me].[dbo] .[User]  U WHERE C.ID = U.ID AND C.ChatRoomNo = "));
							_tcscat_s(m_operation,m_cstr);
							try{
								m_pRecordset->CursorLocation = adUseClient;
								m_pRecordset->Open(_variant_t(m_operation),  _variant_t  ((IDispatch*) m_pConnection,true), adOpenDynamic, adLockOptimistic, adCmdText); 
								dlg->m_sendMsg.m_msgType = MSG_SEARCHCHAT_RP;
								while(!m_pRecordset->adoEOF){
									memset(&(dlg->m_sendMsg.m_text), 0, sizeof(dlg->m_sendMsg.m_text));
									memset(m_loginUser,0, 10 *sizeof(USER_LOGIN));
									m_loginUser[0].m_id = id;//所在聊天室的号
									for(int i = 1; i < 10; ++i){
									
										strcpy_s(m_loginUser[i].m_pwd,_com_util::ConvertBSTRToString((m_pRecordset->GetCollect(_variant_t(_T("Username")))).bstrVal));
										m_loginUser[i].m_id =  m_pRecordset->GetCollect(_variant_t(_T("ID"))).lVal;
										if( m_pRecordset->GetCollect(_variant_t(_T("OnlineState"))).boolVal == 0){
											m_loginUser[i].m_id *= -1;
										}
										m_pRecordset->MoveNext();
										if(m_pRecordset->adoEOF){
											break;
										}
									}
									memcpy(dlg->m_sendMsg.m_text, m_loginUser, 10 *sizeof(USER_LOGIN));
									dlg->iSend = send(m_socketConnected, (char*)&(dlg->m_sendMsg), sizeof(dlg->m_sendMsg), 0);
									if (dlg->iSend == SOCKET_ERROR) {
										ERRORMSG;
										return;
									}
									else if (dlg->iSend == 0)
										continue;
									else{
		#ifdef DEBUG
			AfxMessageBox(_T("返回该聊天室的所有成员,已回复\n"));
		#endif
									}
								}
							
							}
							catch(_com_error e)
							{
								CString errormessage;  
								errormessage.Format( _T("broadcast失败 !\r\n 错误信息 :%s(%ld)"),e.ErrorMessage(),e.Error() );   
								AfxMessageBox(errormessage);///显示错误信息 
							}
							if(m_pRecordset != NULL && m_pRecordset->GetState() == adStateOpen){
								m_pRecordset->Close();
							}
							delete[] m_loginUser;
							break;
						case MSG_CHATROOM://新建的聊天室
							m_2int = new int[2];
							memcpy( &id, dlg->m_recvMsg.m_text, sizeof(id));
							m_cstr.Format(_T("%d"),id);	
							_tcscpy_s(m_operation,_T("INSERT INTO [Talk2Me].[dbo].[ChatRoom]([Founder])VALUES("));
							_tcscat_s(m_operation, m_cstr);
							_tcscat_s(m_operation,_T(")"));
							try{
								m_pRecordset->Open(_variant_t(m_operation),  _variant_t  ((IDispatch*) m_pConnection,true), adOpenStatic, adLockOptimistic, adCmdText); 
								if(m_pRecordset != NULL && m_pRecordset->GetState() == adStateOpen){
									m_pRecordset->Close();
								}
								memset(m_operation,0,MAX_PATH);
								_tcscpy_s(m_operation,_T("select MAX(ChatRoomNo) from [Talk2Me].[dbo].[ChatRoom]"));
								m_pRecordset->Open(_variant_t(m_operation),  _variant_t  ((IDispatch*) m_pConnection,true), adOpenStatic, adLockOptimistic, adCmdText); 
								var = m_pRecordset->GetCollect(_variant_t((long)0));
								dlg->m_sendMsg.m_msgType = MSG_CHATROOM_RP;
								m_2int[0] = (int)var.lVal;//聊天室id
								m_2int[1] = id;//founder id
								memcpy(dlg->m_sendMsg.m_text,m_2int,sizeof(int)*2);//返回聊天室id
							
								dlg->iSend = send(m_socketConnected, (char*)&(dlg->m_sendMsg), sizeof(dlg->m_sendMsg), 0);
								if (dlg->iSend == SOCKET_ERROR) {
									ERRORMSG;
									return;
								}
								else if (dlg->iSend == 0)
									continue;
								else{
	#ifdef DEBUG
		AfxMessageBox(_T("聊天室创建成功,已回复\n"));
	#endif
								}
								if(m_pRecordset != NULL && m_pRecordset->GetState() == adStateOpen){
									m_pRecordset->Close();
								}
								memset(m_operation,0,sizeof(m_operation));
								_tcscpy_s(m_operation,_T("INSERT INTO [Talk2Me].[dbo].[Chat]([ChatRoomNo],[ID])VALUES("));
								m_cstr.Format(_T("%ld"),var.lVal);	
								_tcscat_s(m_operation, m_cstr);
								_tcscat_s(m_operation,_T(","));
								m_cstr.Format(_T("%d"),id);	
								_tcscat_s(m_operation, m_cstr);
								_tcscat_s(m_operation,_T(")"));
								m_pRecordset->Open(_variant_t(m_operation),  _variant_t  ((IDispatch*) m_pConnection,true), adOpenStatic, adLockOptimistic, adCmdText); 
							
							}
							catch(_com_error e)
							{
								CString errormessage;  
								errormessage.Format( _T("insert聊天室失败 !\r\n 错误信息 :%s(%ld)"),e.ErrorMessage(),e.Error() );   
								AfxMessageBox(errormessage);///显示错误信息 
							}
							if(m_pRecordset != NULL && m_pRecordset->GetState() == adStateOpen){
								m_pRecordset->Close();
							}

							delete[] m_2int;
							break;
						case MSG_ADDONECHAT://增加一员到聊天室
							m_2int = new int[2];
							memcpy( m_2int, dlg->m_recvMsg.m_text, sizeof(int)*2);
						
							_tcscpy_s(m_operation,_T("INSERT INTO [Talk2Me].[dbo].[Chat]([ChatRoomNo],[ID])VALUES("));
							m_cstr.Format(_T("%d"),m_2int[1]);
							_tcscat_s(m_operation, m_cstr);
							_tcscat_s(m_operation,_T(","));
							m_cstr.Format(_T("%d"),m_2int[0]);	
							_tcscat_s(m_operation, m_cstr);
							_tcscat_s(m_operation,_T(")"));
							try{
								m_pRecordset->Open(_variant_t(m_operation),  _variant_t  ((IDispatch*) m_pConnection,true), adOpenStatic, adLockOptimistic, adCmdText); 
							
							}
							catch(_com_error e)
							{
								CString errormessage;  
								errormessage.Format( _T("insertone聊天室失败 !\r\n 错误信息 :%s(%ld)"),e.ErrorMessage(),e.Error() );   
								AfxMessageBox(errormessage);///显示错误信息 
							}
							if(m_pRecordset != NULL && m_pRecordset->GetState() == adStateOpen){
								m_pRecordset->Close();
							}
							delete[] m_2int;
							break;
						case MSG_DELETEONECHAT:
							m_2int = new int[2];

							memcpy( m_2int, dlg->m_recvMsg.m_text, sizeof(int)*2);
						
							_tcscpy_s(m_operation,_T("DELETE FROM [Talk2Me].[dbo].[Chat] WHERE ID = "));
							m_cstr.Format(_T("%d"),m_2int[0]);
							_tcscat_s(m_operation, m_cstr);
							_tcscat_s(m_operation,_T(" AND ChatRoomNo = "));
							m_cstr.Format(_T("%d"),m_2int[1]);	
							_tcscat_s(m_operation, m_cstr);
							try{
								m_pRecordset->Open(_variant_t(m_operation),  _variant_t  ((IDispatch*) m_pConnection,true), adOpenStatic, adLockOptimistic, adCmdText); 
							
							}
							catch(_com_error e)
							{
								CString errormessage;  
								errormessage.Format( _T("deleteone聊天室失败 !\r\n 错误信息 :%s(%ld)"),e.ErrorMessage(),e.Error() );   
								AfxMessageBox(errormessage);///显示错误信息 
							}
							if(m_pRecordset != NULL && m_pRecordset->GetState() == adStateOpen){
								m_pRecordset->Close();
							}
							delete[] m_2int;
							break;
					
				
						}
						
					}
				}
					
			}
			if(!FD_ISSET(m_socketConnected,&readSet))
				break;
		}
		
	//	closesocket(m_socketConnected);

		}
		
	}
	_endthread();

	return;
}