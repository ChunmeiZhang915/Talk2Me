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
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CTALK2MESERVERDlg �Ի���



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


// CTALK2MESERVERDlg ��Ϣ�������

BOOL CTALK2MESERVERDlg::OnInitDialog()
{
	::CoInitialize(NULL);

	AfxEnableControlContainer();
	if(!AfxOleInit()){
		AfxMessageBox(_T("��ʼ��OLE DLLʧ��"));
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

	
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��
	

	m_IsConnect = false;
	(CButton*)GetDlgItem(IDC_SETSERVERIPPORTBTN)->EnableWindow(true);
	//(CButton*)GetDlgItem(IDC_DISCONNECTBTN)->EnableWindow(false);
	(CButton*)GetDlgItem(IDC_CONNECTBTN)->EnableWindow(false);
	ShowWindow(SW_NORMAL);


	//�ڵ�ǰĿ¼�´���File�ļ���;

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
void CTALK2MESERVERDlg::OnClose(){//���ߴ���
	int m_rlp = AfxMessageBox(_T("ȷ��Ҫ�Ͽ���������") , MB_OKCANCEL|MB_ICONQUESTION);		
	if(m_rlp == IDCANCEL){
		return;
	}else{
		if(!m_IsConnect){
			CDialogEx::OnClose();
		}else{
			m_IsConnect = false;
			CWnd::FromHandle(this->m_hWnd)->GetDlgItem(IDC_CONNECTBTN)->SetWindowTextW(_T("30s���Զ��Ͽ�"));
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
	
	mysql_init(&mysql);//��ʼ��
	bool isConnected=mysql_real_connect(&mysql,"sqld.duapp.com","AUl2V3dFITG9IDAXbg9Ae5PB","YH7bynOe681znjgOgqpO1ZYqyjfpqvsY","kBtgMQXJlivdOJTXyoRS",4050,0,0);//��
	if (isConnected)
	{
		MessageBox(_T("success connected!"));
		return true;
	}
	else
	{
		int i = mysql_errno(&mysql);//���ӳ���
		const char * s = mysql_error(&mysql);
		MessageBox(_T("failed to connect! Please check for your db service."));
		return false;
	}

	//HRESULT hr = NULL;
	//try{
	//	hr = m_pConnection.CreateInstance(_T("ADODB.Connection"));//���� Connection ���� 
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
	//		AfxMessageBox(_T("���� Connection ����ʧ��"));//��ʾ������Ϣ
	//		return FALSE;
	//	}
	//}catch(_com_error e){ 
	//	CString errormessage;  
 //       errormessage.Format( TEXT("�������ݿ�ʧ�� !\r\n ������Ϣ :%s(%ld)"),e.ErrorMessage(),e.Error() );   
 //       AfxMessageBox(errormessage);///��ʾ������Ϣ    
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
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
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
	
	// �������������׽ӿ�
	m_ServerSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (m_ServerSocket == INVALID_SOCKET) {
		ERRORMSG;
		closesocket(m_ServerSocket);
		WSACleanup();
		return;
	}

	
	// �󶨷�����׽ӿ�
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
		INT_PTR nRes = m_ServerSet.DoModal();  // �����Ի��� 
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
		
		// �ӿͻ��˽�������
		SOCKET m_socketConnected  = accept(dlg->m_ServerSocket,(SOCKADDR *)&(dlg->m_clientAddrFrom),&(dlg->iLen));//������������
		
		if (m_socketConnected == SOCKET_ERROR){
			ERRORMSG;
			continue;
		}

		

		while(1){
			FD_ZERO(&readSet);//��ʼ��readSet
			FD_SET(m_socketConnected,&readSet);//���׽���sockSvr����readSet��
			tv.tv_sec = 250;//250��
			tv.tv_usec =0;//0����

			//����select,�ȴ������ݿɶ���ȴ���ʱ
			ret = select(0,&readSet,NULL,NULL,&tv);
			//�������select��ȴ���ʱ
			if(ret == SOCKET_ERROR)
			{
				//�Ͽ�����,�еĻ�
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
				//�Ͽ�����,����еĻ�.
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
					//�Ͽ�����,����еĻ�,
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
						USER_INFO* m_user;//��ע���û�
						USER_LOGIN* m_loginUser;
						TMSG* m_t;
						FMSG* m_f;
						CTMSG* m_ct;
						LTMSG* m_lt;//121�ļ�ͷ��Ϣ
						LCTMSG* m_lct;//�û�����Ⱥ���ļ�ͷ��Ϣ
						
						_variant_t var;
						CString m_cstr;//�ļ�����
						int id;
						int* m_2int;//�洢�����ҵĲ����߳�Աid
						CFile m_cFile;//�ͻ����ϴ����ļ�
						switch(dlg->m_recvMsg.m_msgType){
						case MSG_SIGNUP:
							m_user = new USER_INFO;//��ʾ��ע���û�,�洢�ǳƺ�����
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
								_tcscpy_s(m_operation,_T("select MAX(ID) from [Talk2Me].[dbo].[User]"));//��ȡϵͳ�����id
								m_pRecordset->Open(_variant_t(m_operation),  _variant_t  ((IDispatch*) m_pConnection,true), adOpenStatic, adLockOptimistic, adCmdText); 
								var = m_pRecordset->GetCollect(_variant_t((long)0));
								dlg->m_sendMsg.m_msgType = MSG_SIGNUP_RP;
								memcpy(dlg->m_sendMsg.m_text,&(var.lVal),sizeof(var.lVal));
								//ע��ɹ�,��ע���û�������ע���id
								dlg->iSend = send(m_socketConnected, (char*)&(dlg->m_sendMsg), sizeof(dlg->m_sendMsg), 0);
								if (dlg->iSend == SOCKET_ERROR) {
									ERRORMSG;
									return;
								}
								else if (dlg->iSend == 0)
									continue;
								else{
	#ifdef DEBUG
		AfxMessageBox(_T("ע��ɹ�,�ѻظ�\n"));
	#endif
								}
							}
							catch(_com_error e)
							{
								CString errormessage;  
								errormessage.Format( _T("insertʧ�� !\r\n ������Ϣ :%s(%ld)"),e.ErrorMessage(),e.Error() );   
								AfxMessageBox(errormessage);///��ʾ������Ϣ 
							}
							if(m_pRecordset != NULL && m_pRecordset->GetState() == adStateOpen){
								m_pRecordset->Close();
							}
							delete m_user;
							break;
						case MSG_LOGOUT:
							m_loginUser = new USER_LOGIN;//�洢ע���˵�Id
								
							memcpy( m_loginUser, dlg->m_recvMsg.m_text, sizeof(USER_LOGIN));
							m_cstr.Format(_T("%d"),m_loginUser->m_id);
							memset(m_operation, 0, sizeof(m_operation));
							_tcscpy_s(m_operation,_T("UPDATE [Talk2Me].[dbo].[User] SET OnlineState = 0 WHERE ID = "));
							_tcscat_s(m_operation, m_cstr);
							try{
								m_pRecordset->Open(_variant_t(m_operation),  _variant_t  ((IDispatch*) m_pConnection,true), adOpenStatic, adLockOptimistic, adCmdText); 
							}catch(_com_error e){
								CString errormessage;  
								errormessage.Format( _T("ע��ʧ�� !\r\n ������Ϣ :%s(%ld)"),e.ErrorMessage(),e.Error() );   
								AfxMessageBox(errormessage);///��ʾ������Ϣ 
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
							m_loginUser = new USER_LOGIN;//�洢��½�˵�Id������
							m_user = new USER_INFO;//�û����ص�¼�˵������͵�½������Ϣ
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
										memcpy(m_user->m_pwd,"�Ѿ���½!",sizeof("�Ѿ���½!"));
										memcpy(dlg->m_sendMsg.m_text,m_user,sizeof(USER_INFO));
									}else{
										dlg->m_OnlineClients.insert(std::pair<int, SOCKET>(m_loginUser->m_id, m_socketConnected));
										memcpy(m_user->m_pwd,"��½�ɹ�!",sizeof("��½�ɹ�!"));
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
									memcpy(m_user->m_pwd,"��½ʧ��!",sizeof("��½ʧ��!"));
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
		AfxMessageBox(_T("��½,�ѻظ�\n"));
	#endif
								}
								//�ж��Ƿ���������������Ϣ
								if(CString(m_user->m_pwd) == _T("��½�ɹ�!")){
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
									//��������Ϣ
									while(!m_pRecordset->adoEOF){
										memset(&(dlg->m_sendMsg), 0, sizeof(dlg->m_sendMsg));
										//�ж����ļ������ı���Ϣ
										if((m_pRecordset->GetCollect(_variant_t(_T("IsFile")))).boolVal == -1){//���ļ���Ϣ
											m_cstr.Empty();
											m_cstr = CString(FILESAVEPATH);
											m_cstr +=  CString(_com_util::ConvertBSTRToString((m_pRecordset->GetCollect(_variant_t(_T("Message")))).bstrVal));//����Ѿ�������ļ�����
								
								
											if(m_cFile.Open(m_cstr, CFile::modeRead | CFile::typeBinary) == NULL){
												ERRORMSG;
												break;
											}else{
												
												memset(&dlg->m_sendMsg, 0 , sizeof(dlg->m_sendMsg));
												dlg->m_sendMsg.m_msgType = MSG_CHATFILETYPE;
												m_lct = new LCTMSG;
												m_cstr.Empty();
								
												m_cstr.Empty();
												m_cstr = CString(_com_util::ConvertBSTRToString((m_pRecordset->GetCollect(_variant_t(_T("Message")))).bstrVal));//����Ѿ�������ļ�����
							
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
				AfxMessageBox(_T("�Է����ߣ�Ⱥ���ļ���Ϣ��ת��\n"));
			#endif
												
												}
												break;
												
											}
											
											delete m_lct;
										}else{//���ı���Ϣ
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
					AfxMessageBox(_T("�����������ı���Ϣ,�ѻظ�\n"));
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
									//121�Ի���Ϣ
									while(!m_pRecordset->adoEOF){
										memset(&(dlg->m_sendMsg), 0, sizeof(dlg->m_sendMsg));
										//�ж����ļ������ı���Ϣ��������������Ϣ
										if((m_pRecordset->GetCollect(_variant_t(_T("IsFile")))).boolVal == -1){//���ļ���Ϣ
											
											
											m_cstr.Empty();
											m_cstr = CString(FILESAVEPATH);
											m_cstr +=  CString(_com_util::ConvertBSTRToString((m_pRecordset->GetCollect(_variant_t(_T("Message")))).bstrVal));//����Ѿ�������ļ�����
								
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
												m_cstr = CString(_com_util::ConvertBSTRToString((m_pRecordset->GetCollect(_variant_t(_T("Message")))).bstrVal));//����Ѿ�������ļ�����
							
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
				AfxMessageBox(_T("�Է����ߣ��ļ�ͷ��Ϣ��ת��\n"));
			#endif
												
												}
											}
											
											delete m_lt;
											
												
										}else{//���ı���Ϣ
											//һ��һ�ı���Ϣ
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
					AfxMessageBox(_T("�����ı���Ϣ,�ѻظ�\n"));
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
								errormessage.Format( _T("selectʧ�� !\r\n ������Ϣ :%s(%ld)"),e.ErrorMessage(),e.Error() );   
								AfxMessageBox(errormessage);///��ʾ������Ϣ 
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
									memcpy(dlg->m_sendMsg.m_text,"����ʧ��!",sizeof("����ʧ��!"));
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
		AfxMessageBox(_T("����,�ѻظ�\n"));
	#endif
								}
							}
							catch(_com_error e)
							{
								CString errormessage;  
								errormessage.Format( _T("selectIDʧ�� !\r\n ������Ϣ :%s(%ld)"),e.ErrorMessage(),e.Error() );   
								AfxMessageBox(errormessage);///��ʾ������Ϣ 
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
									if(var.boolVal == -1){//����
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
				AfxMessageBox(_T("�Է����ߣ���Ϣ��ת��\n"));
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
								}else{//�Է������ڣ���̫����
									//memcpy(dlg->m_sendMsg.m_text,"��½ʧ��!",sizeof("��½ʧ��!"));
								}
							}catch(_com_error e){
								CString errormessage;  
								errormessage.Format( _T("selectʧ�� !\r\n ������Ϣ :%s(%ld)"),e.ErrorMessage(),e.Error() );   
								AfxMessageBox(errormessage);///��ʾ������Ϣ 
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
								if(m_pRecordset->GetRecordCount() == 1){//�û�����

									//�����ļ�
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
									
									_tcscat_s(m_operation,CString(m_t->m_text));//�ļ�����;idfrom_idto_name_����.
									_tcscat_s(m_operation,_T("',1,0,-1)"));


									if(m_pRecordset != NULL && m_pRecordset->GetState() == adStateOpen){
										m_pRecordset->Close();
									}
									m_pRecordset->Open(_variant_t(m_operation),  _variant_t  ((IDispatch*) m_pConnection,true), adOpenStatic, adLockOptimistic, adCmdText); 
									
								}else{//�Է������ڣ���̫����
									//memcpy(dlg->m_sendMsg.m_text,"��½ʧ��!",sizeof("��½ʧ��!"));
								}
							}catch(_com_error e){
								CString errormessage;  
								errormessage.Format( _T("selectʧ�� !\r\n ������Ϣ :%s(%ld)"),e.ErrorMessage(),e.Error() );   
								AfxMessageBox(errormessage);///��ʾ������Ϣ 
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
								errormessage.Format( _T("select chatroomʧ�� !\r\n ������Ϣ :%s(%ld)"),e.ErrorMessage(),e.Error() );   
								AfxMessageBox(errormessage);///��ʾ������Ϣ 
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
						case MSG_FILEEND://һ��һ�ļ�����
							m_cstr.Empty();
							m_cstr = CString(FILESAVEPATH);
							m_cstr +=  CString(dlg->m_recvMsg.m_text);//����Ѿ�������ļ�����
								
								
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
								m_cstr =  CString(dlg->m_recvMsg.m_text);//����Ѿ�������ļ�����
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
										//�����ļ���Ϣ���Է��ͻ���,
										dlg->iSend = send(first->second, (char*)&(dlg->m_sendMsg), sizeof(dlg->m_sendMsg), 0);
										if (dlg->iSend == SOCKET_ERROR) {
											ERRORMSG;
											return;
										}
										else if (dlg->iSend == 0)
											break;
										else{
	#ifdef DEBUG
		AfxMessageBox(_T("�Է����ߣ��ļ�ͷ��Ϣ��ת��\n"));
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
							m_loginUser = new USER_LOGIN;//�û��Ļ�ý����ļ����˵�id
							memset(m_f, 0, sizeof(FMSG));
							memset(m_loginUser, 0, sizeof(USER_LOGIN));
							memcpy(m_loginUser,dlg->m_recvMsg.m_text, sizeof(USER_LOGIN));
							memcpy(m_f->m_filename, m_loginUser->m_pwd, sizeof(m_f->m_filename));
							m_cstr.Empty();
							m_cstr =  CString(FILESAVEPATH);
							m_cstr +=  CString(m_loginUser->m_pwd);//��ý��յ��ļ�����,�ļ���������:������id_������id_����������_�ļ�����
							
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
								//�����ݿ���ɾ���ļ�
								//���ļ���ɾ��
								
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
							//ʲô������
							break;
						case MSG_CANCELFILE:
							//�ļ�ȡ������,ֱ��ɾ��
							m_loginUser = new USER_LOGIN;//�û��Ļ��ȡ�������ļ����˵�id
							
							memset(m_loginUser, 0, sizeof(USER_LOGIN));
							memcpy(m_loginUser,dlg->m_recvMsg.m_text, sizeof(USER_LOGIN));
							m_cstr.Empty();
							m_cstr =  CString(FILESAVEPATH);
							m_cstr +=  CString(m_loginUser->m_pwd);//���ȡ�����յ��ļ�����,�ļ���������:������id_������id_����������_�ļ�����

							if(m_cFile.Open(m_cstr, CFile::modeRead | CFile::typeBinary) == NULL){
								ERRORMSG;
								break;
							}else{
								//���ļ���ɾ��
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
							m_cstr +=  CString(dlg->m_recvMsg.m_text);//����Ѿ�������ļ�����
								
								
							if(m_cFile.Open(m_cstr, CFile::modeRead | CFile::typeBinary) == NULL){
								ERRORMSG;
								break;
							}else{
								
								memset(&dlg->m_sendMsg, 0 , sizeof(dlg->m_sendMsg));
								dlg->m_sendMsg.m_msgType = MSG_CHATFILETYPE;
								m_lct = new LCTMSG;
								m_cstr.Empty();
								
								m_cstr =  CString(dlg->m_recvMsg.m_text);//����Ѿ�������ļ�����
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
				AfxMessageBox(_T("�Է����ߣ�Ⱥ���ļ���Ϣ��ת��\n"));
			#endif
												
												}
												break;
											}
										}
										m_pRecordset->MoveNext();
									}
								}catch(_com_error e){
									CString errormessage;  
									errormessage.Format( _T("select chatroomʧ�� !\r\n ������Ϣ :%s(%ld)"),e.ErrorMessage(),e.Error() );   
									AfxMessageBox(errormessage);///��ʾ������Ϣ 
								}

							}
							
							delete m_lct;
							if(m_pRecordset != NULL && m_pRecordset->GetState() == adStateOpen){
								m_pRecordset->Close();
							}
							break;
						case MSG_RECVCHATFILE:
							m_f = new FMSG;
							m_loginUser = new USER_LOGIN;//�û��Ļ�ý����ļ����˵�id
							memset(m_f, 0, sizeof(FMSG));
							memset(m_loginUser, 0, sizeof(USER_LOGIN));
							memcpy(m_loginUser,dlg->m_recvMsg.m_text, sizeof(USER_LOGIN));
							memcpy(m_f->m_filename, m_loginUser->m_pwd, sizeof(m_f->m_filename));
							m_cstr.Empty();
							m_cstr =  CString(FILESAVEPATH);
							m_cstr +=  CString(m_loginUser->m_pwd);//��ý��յ��ļ�����,�ļ���������:������id_������id_����������_�ļ�����
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
									
								//���ļ���ɾ��
								
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
							m_loginUser = new USER_LOGIN;//�û��Ļ��ȡ�������ļ����˵�id
							
							memset(m_loginUser, 0, sizeof(USER_LOGIN));
							memcpy(m_loginUser,dlg->m_recvMsg.m_text, sizeof(USER_LOGIN));
							m_cstr.Empty();
							m_cstr =  CString(FILESAVEPATH);
							m_cstr +=  CString(m_loginUser->m_pwd);//���quxiao���յ��ļ�����,�ļ���������:������id_������id_�����Ҵ�����_����������_�ļ�����
							if(m_cFile.Open(m_cstr, CFile::modeRead | CFile::typeBinary) == NULL){
								ERRORMSG;
								break;
							}else{
								//������˵�ļ��ѱ�ȡ������
								m_cFile.Close();
						
								
								//���ļ���ɾ��
								
								
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
						case MSG_CHATTXT://��������Ϣ
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
			AfxMessageBox(_T("�Է����ߣ���Ϣ��ת��\n"));
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
								for(int i = 0; i < id; ++i){//��ǰ�����ߵ��û�,��Ϣ�������ݿ�
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
								errormessage.Format( _T("select chatroomʧ�� !\r\n ������Ϣ :%s(%ld)"),e.ErrorMessage(),e.Error() );   
								AfxMessageBox(errormessage);///��ʾ������Ϣ 
							}
							if(m_pRecordset != NULL && m_pRecordset->GetState() == adStateOpen){
								m_pRecordset->Close();
							}
							delete m_ct;
							delete[] m_2int;
							break;
						case MSG_BROADCAST://��ѯ����ע���û�
							m_loginUser = new USER_LOGIN[10];//10*100 = 1000 ����id������,ÿ�η�ʮ��
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
											m_loginUser[i].m_id *= -1;//��ֵ��id��ʾ�������û�
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
			AfxMessageBox(_T("�㲥,�ѻظ�\n"));
		#endif
									}
								}
							
							}
							catch(_com_error e)
							{
								CString errormessage;  
								errormessage.Format( _T("broadcastʧ�� !\r\n ������Ϣ :%s(%ld)"),e.ErrorMessage(),e.Error() );   
								AfxMessageBox(errormessage);///��ʾ������Ϣ 
							}
							if(m_pRecordset != NULL && m_pRecordset->GetState() == adStateOpen){
								m_pRecordset->Close();
							}
							delete[] m_loginUser;
							break;
						case MSG_CHATBROADCAST://��ѯĳ���û����������������
							m_2int = new int[126];//126*8 = 1008,���Դ��63����������Ϣ��ż����ʾ������id��������ʾ������
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
			AfxMessageBox(_T("�����ҹ㲥,�ѻظ�\n"));
		#endif
									}
								}
							
							}
							catch(_com_error e)
							{
								CString errormessage;  
								errormessage.Format( _T("broadcastʧ�� !\r\n ������Ϣ :%s(%ld)"),e.ErrorMessage(),e.Error() );   
								AfxMessageBox(errormessage);///��ʾ������Ϣ 
							}
							if(m_pRecordset != NULL && m_pRecordset->GetState() == adStateOpen){
								m_pRecordset->Close();
							}
							delete[] m_2int;
							break;
						case MSG_SEARCHCHAT://��ѯ��ǰ�����Ұ�����������
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
									m_loginUser[0].m_id = id;//���������ҵĺ�
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
			AfxMessageBox(_T("���ظ������ҵ����г�Ա,�ѻظ�\n"));
		#endif
									}
								}
							
							}
							catch(_com_error e)
							{
								CString errormessage;  
								errormessage.Format( _T("broadcastʧ�� !\r\n ������Ϣ :%s(%ld)"),e.ErrorMessage(),e.Error() );   
								AfxMessageBox(errormessage);///��ʾ������Ϣ 
							}
							if(m_pRecordset != NULL && m_pRecordset->GetState() == adStateOpen){
								m_pRecordset->Close();
							}
							delete[] m_loginUser;
							break;
						case MSG_CHATROOM://�½���������
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
								m_2int[0] = (int)var.lVal;//������id
								m_2int[1] = id;//founder id
								memcpy(dlg->m_sendMsg.m_text,m_2int,sizeof(int)*2);//����������id
							
								dlg->iSend = send(m_socketConnected, (char*)&(dlg->m_sendMsg), sizeof(dlg->m_sendMsg), 0);
								if (dlg->iSend == SOCKET_ERROR) {
									ERRORMSG;
									return;
								}
								else if (dlg->iSend == 0)
									continue;
								else{
	#ifdef DEBUG
		AfxMessageBox(_T("�����Ҵ����ɹ�,�ѻظ�\n"));
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
								errormessage.Format( _T("insert������ʧ�� !\r\n ������Ϣ :%s(%ld)"),e.ErrorMessage(),e.Error() );   
								AfxMessageBox(errormessage);///��ʾ������Ϣ 
							}
							if(m_pRecordset != NULL && m_pRecordset->GetState() == adStateOpen){
								m_pRecordset->Close();
							}

							delete[] m_2int;
							break;
						case MSG_ADDONECHAT://����һԱ��������
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
								errormessage.Format( _T("insertone������ʧ�� !\r\n ������Ϣ :%s(%ld)"),e.ErrorMessage(),e.Error() );   
								AfxMessageBox(errormessage);///��ʾ������Ϣ 
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
								errormessage.Format( _T("deleteone������ʧ�� !\r\n ������Ϣ :%s(%ld)"),e.ErrorMessage(),e.Error() );   
								AfxMessageBox(errormessage);///��ʾ������Ϣ 
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