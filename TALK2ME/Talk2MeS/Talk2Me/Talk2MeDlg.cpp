
// Talk2Medlg->cpp : 实现文件
//
#include "afxcmn.h"
#include "afxwin.h"
#include "stdafx.h"
#include "Talk2Me.h"
#include "Talk2Medlg.h"
#include "afxdialogex.h"
#include "IpNotFound.h"
#include<WinSock2.h>
#include<Windows.h>
#include <atlconv.h>
#include <afxpriv.h>


#pragma comment(lib,"Ws2_32")
#pragma comment(lib, "Iphlpapi.lib")
HANDLE r1;    //接收线程handler
HANDLE sendfile;//发送文件
DWORD receiverMark; //接收线程标识符
DWORD sendfileMark;//发送文件标示符
DWORD WINAPI Receiver(LPVOID pParam);
DWORD WINAPI SendFileThread(LPVOID pParam);

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
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


// CTalk2MeDlg 对话框

CTalk2MeDlg::CTalk2MeDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTalk2MeDlg::IDD, pParent)
	, m_myOwnId(-1)
	, m_wantToTalkId(0)
{
	memset(m_username, 0, DB_LENGTH);
	m_IsConnected = false;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTalk2MeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_ID_EDIT, m_wantToTalkId);
	DDX_Control(pDX, IDC_TAB1, m_tabCtrl);
	DDX_Control(pDX, IDC_TAB1, m_tabCtrl);
}

BEGIN_MESSAGE_MAP(CTalk2MeDlg, CDialogEx)
	ON_MESSAGE(WM_RecvMsg, OnRecvMsg)//用于弹出对话框
	ON_MESSAGE(WM_RecvChatMsg, OnRecvChatMsg)//用于弹出聊天室对话框
	ON_MESSAGE(WM_RecvFileSave, OnRecvFileSave)//用户弹出文件接收画面
	ON_REGISTERED_MESSAGE(WM_CloseMsg,OnCloseMsg)//关闭对话框下线消息
	ON_REGISTERED_MESSAGE(WM_SendMsg,OnSendMsg)//从子窗口中发送会话消息
	ON_REGISTERED_MESSAGE(WM_SendFile,OnSendFile)//从子窗口中发送文件
	ON_REGISTERED_MESSAGE(WM_Signup,OnSignUp)//注册
	ON_REGISTERED_MESSAGE(WM_AddOne,OnAddOne)//添加一个人到聊天室
	ON_REGISTERED_MESSAGE(WM_DelOne,OnDelOne)//从聊天室删除一个人
	ON_REGISTERED_MESSAGE(WM_SendChatMsg,OnSendChatMsg)//发送聊天室消息
	ON_REGISTERED_MESSAGE(WM_SendChatFile,OnSendChatFile)//发送聊天室文件
	ON_REGISTERED_MESSAGE(WM_RecvFile, OnRecvFile)//接收文件
	ON_REGISTERED_MESSAGE(WM_CancelFile, OnCancelFile)//接收文件
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_CLOSE(&CConversation::OnClose)
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_STARTTALK_BUTTON, &CTalk2MeDlg::OnBnClickedStarttalkButton)
	
	ON_BN_CLICKED(IDC_LOGINBTN, &CTalk2MeDlg::OnBnClickedLoginbtn)
	ON_BN_CLICKED(IDC_SETBTN, &CTalk2MeDlg::OnBnClickedSetbtn)
	
	ON_BN_CLICKED(IDC_SIGNUPBTN, &CTalk2MeDlg::OnBnClickedSignupbtn)
	ON_BN_CLICKED(IDC_CHATROOM_BTN, &CTalk2MeDlg::OnBnClickedChatroomBtn)
	ON_BN_CLICKED(IDC_LOGOUTBTN, &CTalk2MeDlg::OnBnClickedLogoutbtn)

	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CTalk2MeDlg::OnTcnSelchangeTab1)
END_MESSAGE_MAP()


// CTalk2MeDlg 消息处理程序
void CTalk2MeDlg::OnClose(){//下线处理
	if(!m_IsConnected){//本身没有登录
		CDialogEx::OnClose();
		return;
	}
	int m_rlp = AfxMessageBox(_T("确定要下线吗？") , MB_OKCANCEL|MB_ICONQUESTION);
	if(m_rlp == IDOK){
		OnBnClickedLogoutbtn();//先通知服务器
		CDialogEx::OnClose();//关闭窗口
	}
}
BOOL CTalk2MeDlg::OnInitDialog()
{
	
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。
	// IDM_ABOUTBOX 必须在系统命令范围内。
	
	

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
		
		}
	}

	
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标
	
//只用点击设置按钮后才可以使用																				
	(CButton*)GetDlgItem(IDC_SETBTN)->EnableWindow(true);
	(CButton*)GetDlgItem(IDC_LOGINBTN)->EnableWindow(false);
	(CButton*)GetDlgItem(IDC_SIGNUPBTN)->EnableWindow(false);
	(CButton*)GetDlgItem(IDC_LOGOUTBTN)->EnableWindow(false);
	(CButton*)GetDlgItem(IDC_CHATROOM_BTN)->EnableWindow(false);
	(CButton*)GetDlgItem(IDC_STARTTALK_BUTTON)->EnableWindow(false);
	(CTabCtrl*)GetDlgItem(IDC_TAB1)->EnableWindow(false);
	(*(CEdit*)GetDlgItem(IDC_ID_EDIT)).SetReadOnly(true);

	



	


	m_tabCtrl.InsertItem(0,_T("好友列表"));
	m_tabCtrl.InsertItem(1,_T("聊天室"));
	//设置IDC_TAB1为父窗口
	m_friendTab.Create(IDD_FRIENDDLG,GetDlgItem(IDC_TAB1));
	m_chatRoomTab.Create(IDD_CHATDLG,GetDlgItem(IDC_TAB1));
    
	//获得IDC_TABTEST客户区大小
	CRect rc;
	m_tabCtrl.GetClientRect(&rc);
    
	//调整子对话框在父窗口中的位置
	rc.top += 30;
	rc.bottom -= 8;
	rc.left += 8;
	rc.right -= 8;

	//设置子对话框尺寸并移动到指定位置
	m_friendTab.MoveWindow(&rc);
	m_chatRoomTab.MoveWindow(&rc);

	//分别设置隐藏和显示
	m_friendTab.ShowWindow(true);
	m_chatRoomTab.ShowWindow(false);

	//设置默认的选项卡
	m_tabCtrl.SetCurSel(0);

	m_myAddr.sin_addr.S_un.S_addr = INADDR_ANY;
	m_myAddr.sin_family = AF_INET;
	m_myAddr.sin_port = htons(m_serverSet.m_ClientOwnPort);
	
	

	
	

	
	return TRUE;
}

void CTalk2MeDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	
		CDialogEx::OnSysCommand(nID, lParam);
}


void CTalk2MeDlg::OnPaint()
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
void CTalk2MeDlg::GetMembersFromTheChatRoom(int cid){
	
	memset(&m_sendMsg, 0 , sizeof(m_sendMsg));
	m_sendMsg.m_msgType = MSG_SEARCHCHAT;
	
	memcpy(m_sendMsg.m_text, &cid, sizeof(cid));
	
	iSend = send(m_ClientSocket, (char*)&m_sendMsg, sizeof(m_sendMsg), 0);
	if (iSend == SOCKET_ERROR) {
		ERRORMSG;
		return;
	}
	else if (iSend == 0)
		return;
	else{
#ifdef DEBUG
		AfxMessageBox(_T("查找聊天室ID发送成功\n"));
#endif
	}
}
HCURSOR CTalk2MeDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
LRESULT CTalk2MeDlg::OnRecvFile(WPARAM lparam, LPARAM isChat){
	CString* filename = (CString*)lparam;
	CString tempNameEx;
	tempNameEx += filename->Mid(filename->ReverseFind(_T('.'))+1);
	CFileDialog m_fileDlg(false, tempNameEx, *filename ,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT ,tempNameEx + _T("|*") + tempNameEx + _T("|文档(*.doc)|*.doc|所有文件(*.*)|*.*||"));
							
							
	TCHAR m_buffer[100];

	GetCurrentDirectory(100, m_buffer);
	m_fileDlg.m_ofn.lpstrInitialDir = m_buffer;
	CString strPath;  // 文件路径
	if(m_fileDlg.DoModal() == IDOK){
		strPath = m_fileDlg.GetFileExt();
		if(strPath != _T("")){
			strPath = m_fileDlg.GetPathName();

			strPath.Replace(strPath.Mid(strPath.ReverseFind(_T('.'))+1),tempNameEx);
		}else{
			strPath = m_fileDlg.GetPathName();
			strPath += tempNameEx;
		}
		m_File2FileName.insert(std::pair<CString, CString>(*filename, strPath));
		CString m_s;
		m_s  = _T("正在下载文件:");
		m_s += *filename;
		CProgress* m_cpb = new CProgress(m_file2SaveDlg.at(*filename)->m_fileLength, m_s);

		m_File2ProgressBar.insert(std::pair<CString,CProgress*>(*filename,m_cpb));
		m_cpb->Create(CProgress::IDD,  CWnd::FromHandle(this->m_hWnd));//创建非模式式对话框 
	
					
		m_cpb->ShowWindow(SW_SHOW);
		m_cpb->UpdateData(false);
		m_cpb->UpdateWindow();
		m_cpb->SetActiveWindow();
		m_cpb->m_ProCtrl.SetRange32(0,(int)(m_file2SaveDlg.at(*filename)->m_fileLength));
	}else{
		int m_rlp = AfxMessageBox(_T("确定要取消接收文件吗？") , MB_OKCANCEL|MB_ICONQUESTION);
		if(m_rlp == IDOK){
			OnCancelFile(lparam, isChat);
		}
	}
	
	memset(&m_sendMsg, 0, sizeof(m_sendMsg));
	if(isChat){
		m_sendMsg.m_msgType = MSG_RECVCHATFILE;
	}else{
		m_sendMsg.m_msgType = MSG_RECVFILE;
	}
	USES_CONVERSION;
	char* temp = W2A(*filename);

	USER_LOGIN m_user;
	memset(&m_user, 0 ,sizeof(USER_LOGIN));
	m_user.m_id = m_myOwnId;
	strcpy_s(m_user.m_pwd, temp);
	memcpy(m_sendMsg.m_text, &m_user,sizeof(USER_LOGIN));
	//// 向服务器发送数据
	iSend = send(m_ClientSocket, (char*)&m_sendMsg, sizeof(m_sendMsg), 0);
	if (iSend == SOCKET_ERROR) {
		ERRORMSG;
		 
	}
	else if (iSend == 0)
		return 0;
	else{
		
	}
	return 0;
}
LRESULT CTalk2MeDlg::OnCancelFile(WPARAM lparam, LPARAM isChat){
	CString* filename = (CString*)lparam;
	memset(&m_sendMsg, 0, sizeof(m_sendMsg));
	if(isChat){
		m_sendMsg.m_msgType = MSG_CANCELCHATFILE;
	}else{
		m_sendMsg.m_msgType = MSG_CANCELFILE;
	}
	m_file2SaveDlg.erase(m_file2SaveDlg.find(*filename));
	USES_CONVERSION;
	char* temp = W2A(*filename);
	strcpy_s(m_sendMsg.m_text, temp);

	USER_LOGIN m_user;
	memset(&m_user, 0 ,sizeof(USER_LOGIN));
	m_user.m_id = m_myOwnId;
	strcpy_s(m_user.m_pwd, temp);
	memcpy(m_sendMsg.m_text, &m_user,sizeof(USER_LOGIN));
	//// 向服务器发送数据
	iSend = send(m_ClientSocket, (char*)&m_sendMsg, sizeof(m_sendMsg), 0);
	if (iSend == SOCKET_ERROR) {
		ERRORMSG;
		 
	}
	else if (iSend == 0)
		return 0;
	else{
		if(isChat){
			CString csTemp;//分离文件名字中的id
			CTMSG * m_ct = new CTMSG;
			AfxExtractSubString(csTemp, (LPCTSTR)*filename, 0, '_');
			m_ct->m_idFrom = _ttoi(csTemp);
			AfxExtractSubString(csTemp, (LPCTSTR)*filename, 1, '_');
			m_ct->m_cId = _ttoi(csTemp);
			AfxExtractSubString(csTemp, (LPCTSTR)*filename, 2, '_');
			m_ct->m_ownerId = _ttoi(csTemp);
			AfxExtractSubString(csTemp, (LPCTSTR)*filename, 3, '_');
			USES_CONVERSION;
			char* temp = W2A(csTemp);
			strcpy_s(m_ct->m_name, temp);
			AfxExtractSubString(csTemp, (LPCTSTR)*filename, 4, '_');
						
			temp = W2A(csTemp);
			strcpy_s(m_ct->m_text, temp);

			if(m_id2Chatroom.find(m_ct->m_cId) == m_id2Chatroom.end()){	//聊天室本来不存在
				CChatRoom* newConversation = new CChatRoom(m_ct->m_cId, m_ct->m_ownerId);//内存清除
						
					
				m_id2Chatroom.insert(std::pair<int, CChatRoom*>(m_ct->m_cId, newConversation));
				CWnd::FromHandle(this->m_hWnd)->SendMessage(WM_RecvChatMsg,m_ct->m_cId);
				newConversation->m_content = CString(_T("取消接收文件:"));
				newConversation->m_content += CString(m_ct->m_text);
				CWnd::FromHandle(this->m_hWnd)->SendMessage(WM_SendChatMsg,m_ct->m_cId);
			}else{
				CChatRoom* newConversation = (m_id2Chatroom).at(m_ct->m_cId);
				newConversation->m_content = CString(_T("取消接收文件:"));
				newConversation->m_content += CString(m_ct->m_text);
				CWnd::FromHandle(this->m_hWnd)->SendMessage(WM_SendChatMsg,m_ct->m_cId);
				newConversation->m_Scroll.LineScroll(newConversation->m_Scroll.GetLineCount());	
			}
		
			delete m_ct;
		}else{
			TMSG* m_t = new TMSG;
			CString csTemp;//分离文件名字中的id
			AfxExtractSubString(csTemp, (LPCTSTR)*filename, 0, '_');
			m_t->m_idFrom = _ttoi(csTemp);
			AfxExtractSubString(csTemp, (LPCTSTR)*filename, 1, '_');
			m_t->m_id = _ttoi(csTemp);
			AfxExtractSubString(csTemp, (LPCTSTR)*filename, 2, '_');
			USES_CONVERSION;
			char* temp = W2A(csTemp);
			strcpy_s(m_t->m_name, temp);
			AfxExtractSubString(csTemp, (LPCTSTR)*filename, 3, '_');
						
			temp = W2A(csTemp);
			strcpy_s(m_t->m_text, temp);
			if(m_id2Conversation.find(m_t->m_idFrom) == m_id2Conversation.end()){	//对话本来不存在
						
				CConversation* newConversation = new CConversation(m_t->m_idFrom,CString(m_t->m_name));//内存清除
						
						
				m_id2Conversation.insert(std::pair<int, CConversation*>(m_t->m_idFrom, newConversation));
				CWnd::FromHandle(this->m_hWnd)->SendMessage(WM_RecvMsg,m_t->m_idFrom);
				newConversation->m_content = CString(_T("取消接收文件:"));
				newConversation->m_content += CString(m_t->m_text);
							
				CWnd::FromHandle(this->m_hWnd)->SendMessage(WM_SendMsg,m_t->m_idFrom);
			}else{
				CConversation* newConversation = (this->m_id2Conversation).at(m_t->m_idFrom);
				newConversation->m_content = CString(_T("取消接收文件:"));
				newConversation->m_content += CString(m_t->m_text);
				CWnd::FromHandle(this->m_hWnd)->SendMessage(WM_SendMsg,m_t->m_idFrom);
				newConversation->m_Scroll.LineScroll(newConversation->m_Scroll.GetLineCount());	
			}
			delete m_t;
		}
		
	}
	return 0;
}
LRESULT CTalk2MeDlg::OnSendChatMsg(WPARAM id, LPARAM lparam)
{
	
	m_sendMsg.m_msgType = MSG_CHATTXT;
	CTMSG m_ct;
	m_ct.m_idFrom = m_myOwnId;
	m_ct.m_ownerId = m_id2Chatroom.at((int)id)->m_owner;
	memcpy(m_ct.m_name, m_username, DB_LENGTH);
	m_ct.m_cId = id;//聊天室号
	USES_CONVERSION;
	char* temp = W2A(m_id2Chatroom.at((int)id)->m_content);
	strcpy_s(m_ct.m_text, temp);
	memcpy(m_sendMsg.m_text, &m_ct, sizeof(CTMSG));
	
	//// 向服务器发送数据
	iSend = send(m_ClientSocket, (char*)&m_sendMsg, sizeof(m_sendMsg), 0);
	if (iSend == SOCKET_ERROR) {
		ERRORMSG;
		 
	}
	else if (iSend == 0)
		return 0;
	else{
		CChatRoom* newC = m_id2Chatroom.at((int)id);
		newC->m_content = "";
		CWnd::FromHandle(newC->m_hWnd)->GetDlgItem(IDC_CONTENTRE)->SetWindowTextW(newC->m_content);
	}
	return 0;
}
LRESULT CTalk2MeDlg::OnAddOne(WPARAM pId, LPARAM cId){
	if(m_myOwnId != m_id2Chatroom.at(cId)->m_owner){//只有创建者才可以添加人
		return 0;
	}
	if(m_myOwnId == pId)
		return 0;
	CString str;
	for(int i = 0; i < (*(CListBox*)m_id2Chatroom.at(cId)->GetDlgItem(IDC_CHATMEMBERLIST)).GetCount(); ++i){

		(*(CListBox*)m_id2Chatroom.at(cId)->GetDlgItem(IDC_CHATMEMBERLIST)).GetText(i, str);
		if(!str.Compare(m_id2Chatroom.at(cId)->strText))
			return 0;
	}
	(*(CListBox*)m_id2Chatroom.at(cId)->GetDlgItem(IDC_CHATMEMBERLIST)).AddString(m_id2Chatroom.at(cId)->strText);
	memset(&m_sendMsg, 0 , sizeof(m_sendMsg));
	m_sendMsg.m_msgType = MSG_ADDONECHAT;
	int m_addOne[] = {pId, cId};//添加人的id和聊天室id
	memcpy(m_sendMsg.m_text, m_addOne, sizeof(m_addOne));
	
	iSend = send(m_ClientSocket, (char*)&m_sendMsg, sizeof(m_sendMsg), 0);
	if (iSend == SOCKET_ERROR) {
		ERRORMSG;
		return 0;
	}
	else if (iSend == 0)
		return 0;
	else{
#ifdef DEBUG
		AfxMessageBox(_T("添加一员发送成功\n"));
#endif
	}
	return 0;
}
LRESULT CTalk2MeDlg::OnDelOne(WPARAM pId, LPARAM cId){
	if(m_myOwnId != m_id2Chatroom.at(cId)->m_owner){//只有创建者才可以删除人
		return 0;
	}
	if(m_myOwnId == pId)
		return 0;
	(*(CListBox*)m_id2Chatroom.at(cId)->GetDlgItem(IDC_CHATMEMBERLIST)).DeleteString(m_id2Chatroom.at(cId)->nCurSel);
	memset(&m_sendMsg, 0 , sizeof(m_sendMsg));
	m_sendMsg.m_msgType = MSG_DELETEONECHAT;
	int m_addOne[] = {pId, cId};
	memcpy(m_sendMsg.m_text, m_addOne, sizeof(m_addOne));
	
	iSend = send(m_ClientSocket, (char*)&m_sendMsg, sizeof(m_sendMsg), 0);
	if (iSend == SOCKET_ERROR) {
		ERRORMSG;
		return 0;
	}
	else if (iSend == 0)
		return 0;
	else{
#ifdef DEBUG
		AfxMessageBox(_T("添加一员发送成功\n"));
#endif
	}
	return 0;
}
LRESULT CTalk2MeDlg::OnSignUp(WPARAM ip, LPARAM lparam){

	return 0;
}
LRESULT CTalk2MeDlg::OnCloseMsg(WPARAM id, LPARAM notChatRoom){//关闭子对话框
	
	if(notChatRoom){
		//delete m_id2Conversation.find((DWORD)id)->second;
		m_id2Conversation.erase(m_id2Conversation.find((DWORD)id));
	}
	else{
		//delete m_id2Chatroom.find((DWORD)id)->second;
		m_id2Chatroom.erase(m_id2Chatroom.find((DWORD)id));
	}
	
	return 0;		 
}
LRESULT CTalk2MeDlg::OnSendFile(WPARAM id, LPARAM lparam){//发送文件相关信息，文件类型和名字
	m_fileConversationId = id;
	sendfile = CreateThread(NULL,0,SendFileThread,this,0,&sendfileMark);
	return 0;
}
DWORD WINAPI SendFileThread(LPVOID lp){
	
	CTalk2MeDlg* dlg = (CTalk2MeDlg*) lp;
	int id = dlg->m_fileConversationId;
	CString m_fileName = dlg->m_id2Conversation.at(id)->m_filePath;//获得文件路径
	CFile m_cFile;
	if(m_cFile.Open(m_fileName , CFile::modeRead | CFile::typeBinary) == NULL){
			ERRORMSG;
			return 0;
	}
	
	CString m_s;
	m_s  = _T("正在上传文件:");
	m_s += dlg->m_id2Conversation.at(id)->m_fileEx;
	CProgress* m_cpb = new CProgress(m_cFile.GetLength(), m_s);

	//m_File2ProgressBar.insert(std::pair<CString,CProgress*>(*filename,m_cpb));
	m_cpb->Create(CProgress::IDD,  CWnd::FromHandle(dlg->m_hWnd));//创建非模式式对话框 				
	m_cpb->ShowWindow(SW_SHOW);
	m_cpb->UpdateData(false);
	m_cpb->UpdateWindow();
	m_cpb->SetActiveWindow();
	m_cpb->m_ProCtrl.SetRange32(0,(int)(m_cFile.GetLength()));
	memset(&dlg->m_sendMsg, 0 , sizeof(dlg->m_sendMsg));
	dlg->m_sendMsg.m_msgType = MSG_FILETYPE;
	TMSG m_t;
	m_t.m_idFrom = dlg->m_myOwnId;
	memcpy(m_t.m_name, dlg->m_username, DB_LENGTH);
	m_t.m_id = id;
	CString m_fn;//文件名字
	m_fn.Format(_T("%d_%d_"),m_t.m_idFrom,m_t.m_id);
	m_fn += CString(dlg->m_username);
	m_fn += CString(_T("_"));
	m_fn += dlg->m_id2Conversation.at(id)->m_fileEx;
	if(m_fn.GetLength() >= 50){
		AfxMessageBox(_T("文件名太长,不能多于50个字符!"));
		return 0;
	}
	USES_CONVERSION;
	char* temp = W2A(m_fn);
	strcpy_s(m_t.m_text, temp);
	
	memcpy(dlg->m_sendMsg.m_text, &m_t, sizeof(TMSG));
	//发送文件信息 文件名字
	dlg->iSend = send(dlg->m_ClientSocket, (char*)&dlg->m_sendMsg, sizeof(dlg->m_sendMsg), 0);
	if (dlg->iSend == SOCKET_ERROR) {
		ERRORMSG;
		 
	}
	else if (dlg->iSend == 0)
		return 0;
	else{
			
		memset(&dlg->m_sendMsg, 0 , sizeof(dlg->m_sendMsg));
		
		dlg->m_sendMsg.m_msgType = MSG_FILE;
		
		FMSG m_fmsg;
		strcpy_s(m_fmsg.m_filename, temp);

		int ch = 0;
		memset(m_fmsg.m_text, 0, sizeof(m_fmsg.m_text));
		while( (ch = m_cFile.Read(m_fmsg.m_text,sizeof(m_fmsg.m_text))) != 0){
			
			m_cpb->SetProgress(ch);
			memset(dlg->m_sendMsg.m_text, 0, sizeof(dlg->m_sendMsg.m_text));
			memcpy(dlg->m_sendMsg.m_text, &m_fmsg, sizeof(FMSG));
			memset(m_fmsg.m_text, 0, sizeof(m_fmsg.m_text));
			//Sleep(100);
			dlg->iSend = send(dlg->m_ClientSocket, (char*)&dlg->m_sendMsg, sizeof(dlg->m_sendMsg), 0);
			if (dlg->iSend == SOCKET_ERROR) {
				ERRORMSG;
		 
			}
			else if (dlg->iSend == 0)
				return 0;
			else{
			
			}
		}
	}	
	m_cFile.Close();
	
	::PostMessage(m_cpb->m_hWnd,WM_CLOSE,0,0);
	//文章结束
	memset(&dlg->m_sendMsg, 0 , sizeof(dlg->m_sendMsg));
	dlg->m_sendMsg.m_msgType = MSG_FILEEND;
	strcpy_s(dlg->m_sendMsg.m_text, temp);
	dlg->iSend = send(dlg->m_ClientSocket, (char*)&dlg->m_sendMsg, sizeof(dlg->m_sendMsg), 0);
	if (dlg->iSend == SOCKET_ERROR) {
		ERRORMSG;
		 
	}
	else if (dlg->iSend == 0)
		return 0;
	else{
		dlg->m_id2Conversation.at(id)->m_history += CString(_T("\n文件已经成功上传!"));
		CWnd::FromHandle(dlg->m_id2Conversation.at(id)->m_hWnd)->GetDlgItem(IDC_HISTORYRICHEDIT)->SetWindowTextW(dlg->m_id2Conversation.at(id)->m_history);
	}
	return 0;
}

LRESULT CTalk2MeDlg::OnSendChatFile(WPARAM id, LPARAM lparam)
{
	
	CString m_fileName = m_id2Chatroom.at(id)->m_filePath;
	CFile m_cFile;
	if(m_cFile.Open(m_fileName , CFile::modeRead | CFile::typeBinary) == NULL){
			ERRORMSG;
			return 0;
	}
	memset(&m_sendMsg, 0 , sizeof(m_sendMsg));
	m_sendMsg.m_msgType = MSG_CHATFILETYPE;
	CTMSG m_t;
	m_t.m_idFrom = m_myOwnId;
	memcpy(m_t.m_name, m_username, DB_LENGTH);
	m_t.m_cId = id;
	m_t.m_ownerId =  m_id2Chatroom.at(id)->m_owner;
	CString m_fn;
	m_fn.Format(_T("%d_%d_%d_"),m_t.m_idFrom,m_t.m_cId,m_t.m_ownerId);
	m_fn += CString(m_username);
	m_fn += CString(_T("_"));
	m_fn += m_id2Chatroom.at(id)->m_fileEx;
	USES_CONVERSION;
	char* temp = W2A(m_fn);
	strcpy_s(m_t.m_text, temp);
	
	memcpy(m_sendMsg.m_text, &m_t, sizeof(CTMSG));
	//发送文件信息 文件名字
	iSend = send(m_ClientSocket, (char*)&m_sendMsg, sizeof(m_sendMsg), 0);
	if (iSend == SOCKET_ERROR) {
		ERRORMSG;
		 
	}
	else if (iSend == 0)
		return 0;
	else{
			
		memset(&m_sendMsg, 0 , sizeof(m_sendMsg));
		
		m_sendMsg.m_msgType = MSG_FILE;
		
		FMSG m_fmsg;
		strcpy_s(m_fmsg.m_filename, temp);

		memset(m_fmsg.m_text, 0, sizeof(m_fmsg.m_text));
		while( m_cFile.Read(m_fmsg.m_text,sizeof(m_fmsg.m_text)) != 0){
			memset(m_sendMsg.m_text, 0, sizeof(m_sendMsg.m_text));
			memcpy(m_sendMsg.m_text, &m_fmsg, sizeof(FMSG));
			memset(m_fmsg.m_text, 0, sizeof(m_fmsg.m_text));
			iSend = send(m_ClientSocket, (char*)&m_sendMsg, sizeof(m_sendMsg), 0);
			if (iSend == SOCKET_ERROR) {
				ERRORMSG;
		 
			}
			else if (iSend == 0)
				return 0;
			else{
			
			}
			memset(m_sendMsg.m_text, 0 , sizeof(m_sendMsg.m_text));
		}
	}	
	m_cFile.Close();
	
	//文章结束
	memset(&m_sendMsg, 0 , sizeof(m_sendMsg));
	m_sendMsg.m_msgType = MSG_CHATFILEEND;
	strcpy_s(m_sendMsg.m_text, temp);
	iSend = send(m_ClientSocket, (char*)&m_sendMsg, sizeof(m_sendMsg), 0);
	if (iSend == SOCKET_ERROR) {
		ERRORMSG;
		 
	}
	else if (iSend == 0)
		return 0;
	else{
		m_id2Chatroom.at(id)->m_history += CString(_T("\n文件已经成功上传!"));
		CWnd::FromHandle(m_id2Chatroom.at(id)->m_hWnd)->UpdateData(false);	
	}
	return 0;
}
LRESULT CTalk2MeDlg::OnRecvChatMsg(WPARAM id, LPARAM lparam)//收到聊天室消息建立对话框,被动通信早已插入map
{
	if(m_id2Chatroom.find(id) == m_id2Chatroom.end()){//主要是处理来自点击tab中的聊天室,并弹出聊天画面
		m_id2Chatroom.insert(std::pair<int, CChatRoom*>(id, new CChatRoom(id, m_chatRoomTab.m_ownerId)));
	}
	CChatRoom* newChatRoom = m_id2Chatroom.at(id);
	if(newChatRoom->m_hWnd != NULL){
		return 0;
	}
	CWnd* pWnd = CWnd::FromHandle(this->m_hWnd);
	newChatRoom->Create(CChatRoom::IDD,  pWnd);//创建非模式式对话框 
	CString m_c;
	m_c.Format(_T("聊天室%d号"),id);
	(CWnd::FromHandle(newChatRoom->m_hWnd))->SetWindowTextW(m_c);
	
	
	newChatRoom->ShowWindow(SW_SHOW);
	newChatRoom->UpdateData(false);
	newChatRoom->UpdateWindow();
	newChatRoom->SetActiveWindow();
	for(int i = 0; i < (*(CListBox*)m_friendTab.GetDlgItem(IDC_LOCALPARTNERLIST)).GetCount(); ++i){
		m_c.Empty();
		(*(CListBox*)m_friendTab.GetDlgItem(IDC_LOCALPARTNERLIST)).GetText(i,m_c);
		(*(CListBox*)(CWnd::FromHandle(newChatRoom->m_hWnd)->GetDlgItem(IDC_OULIST))).AddString(m_c);
	}
	
	
	newChatRoom->UpdateWindow();
	GetMembersFromTheChatRoom(id);
	return 0;
}
LRESULT CTalk2MeDlg::OnRecvFileSave(WPARAM wparam, LPARAM isChat)//收到消息建立对话框
{
	CString* fileName= (CString*)wparam;
	CFileRecv* m_file2Recv = m_file2SaveDlg.at(*fileName);
					
	m_file2Recv->Create(CFileRecv::IDD,  CWnd::FromHandle(this->m_hWnd));//创建非模式式对话框 
	
					
	m_file2Recv->ShowWindow(SW_SHOW);
	m_file2Recv->UpdateData(false);
	m_file2Recv->UpdateWindow();
	m_file2Recv->SetActiveWindow();	
	return 0;
}
LRESULT CTalk2MeDlg::OnRecvMsg(WPARAM id, LPARAM lparam)//收到消息建立对话框
{
	if(m_id2Conversation.find(id) == m_id2Conversation.end()){//主要是处理来自点击tab中的好友,并弹出聊天画面,其他被动通信早已插入map
		m_id2Conversation.insert(std::pair<int, CConversation*>(id, new CConversation(id, CString(m_friendTab.m_name))));
	}
	CConversation* newConversation = m_id2Conversation.at(id);
	if(newConversation->m_hWnd != NULL){
		return 0;
	}
	CWnd* pWnd = CWnd::FromHandle(this->m_hWnd);
	newConversation->Create(CConversation::IDD,  pWnd);//创建非模式式对话框 
	
	(CWnd::FromHandle(newConversation->m_hWnd))->SetWindowTextW(newConversation->m_title);
	newConversation->ShowWindow(SW_SHOW);
	newConversation->UpdateData(false);
	newConversation->UpdateWindow();
	newConversation->SetActiveWindow();	
	return 0;
}
LRESULT CTalk2MeDlg::OnSendMsg(WPARAM id, LPARAM lparam)
{
	
	m_sendMsg.m_msgType = MSG_TEXT;
	TMSG m_t;
	memset(&m_t, 0, sizeof(TMSG));
	m_t.m_idFrom = m_myOwnId;
	memcpy(m_t.m_name, m_username, DB_LENGTH);
	m_t.m_id = id;
	USES_CONVERSION;
	char* temp = W2A(m_id2Conversation.at((int)id)->m_content);
	strcpy_s(m_t.m_text, temp);
	memcpy(m_sendMsg.m_text, &m_t, sizeof(TMSG));
	
	//// 向服务器发送数据
	iSend = send(m_ClientSocket, (char*)&m_sendMsg, sizeof(m_sendMsg), 0);
	if (iSend == SOCKET_ERROR) {
		ERRORMSG;
		 
	}
	else if (iSend == 0)
		return 0;
	else{
		CConversation* newC = m_id2Conversation.at((int)id);
		newC->m_history += "\n\n"; 
		newC->m_history += _T("我说:");									
		newC->m_history  += newC->m_content;
		newC->m_content = "";
		CWnd::FromHandle(newC->m_hWnd)->GetDlgItem(IDC_HISTORYRICHEDIT)->SetWindowTextW(newC->m_history);
		//滚动条加在这里
		CWnd::FromHandle(newC->m_hWnd)->GetDlgItem(IDC_CONTENTRICHEDIT)->SetWindowTextW(newC->m_content);
	}
	return 0;
}
void CTalk2MeDlg::OnBnClickedStarttalkButton()//主动开始与别人的会话
{
	UpdateData(true);
	
	if(m_wantToTalkId == 0){
		AfxMessageBox(_T("请输入对方ID号！"));
		return;
	}
	memset(&m_sendMsg, 0 , sizeof(m_sendMsg));
	m_sendMsg.m_msgType = MSG_SEARCH;
	
	memcpy(m_sendMsg.m_text, &m_wantToTalkId, sizeof(m_wantToTalkId));
	
	iSend = send(m_ClientSocket, (char*)&m_sendMsg, sizeof(m_sendMsg), 0);
	if (iSend == SOCKET_ERROR) {
		ERRORMSG;
		return;
	}
	else if (iSend == 0)
		return;
	else{
#ifdef DEBUG
		AfxMessageBox(_T("查找ID发送成功\n"));
#endif
	}
}




void CTalk2MeDlg::OnBnClickedRefreshButton()//获得当前注册用户
{	
	m_onlineFriends.clear();
	((CListBox*)m_friendTab.GetDlgItem(IDC_LOCALPARTNERLIST))->ResetContent();//清空当前存储的在线用户
	for(map<int,CChatRoom*>::iterator first = m_id2Chatroom.begin(); first != m_id2Chatroom.end();++first){
		if(first->second->m_hWnd == NULL)
			continue;
		((CListBox*)first->second->GetDlgItem(IDC_OULIST ))->ResetContent();//清空当前存储的在线用户					
						
	}
	memset(&m_sendMsg, 0, sizeof(m_sendMsg));
	m_sendMsg.m_msgType = MSG_BROADCAST;	
		// 向服务器发送数据
	iSend = send(m_ClientSocket, (char*)&m_sendMsg, sizeof(m_sendMsg), 0);
	if (iSend == SOCKET_ERROR) {
		ERRORMSG;
		return;
	}
	else if (iSend == 0)
		return;
	else{
#ifdef DEBUG
		AfxMessageBox(_T("查询在线用户消息发送成功\n"));
#endif
	}
	
}

BOOL CTalk2MeDlg::PreTranslateMessage(MSG* pMsg)
{
   
    return CDialog::PreTranslateMessage(pMsg);
}


void CTalk2MeDlg::OnBnClickedLoginbtn()
{
	m_serverAddr.sin_addr.S_un.S_addr = htonl(m_serverSet.m_ServerIpAddr);
	m_serverAddr.sin_family = AF_INET;
	m_serverAddr.sin_port = htons(m_serverSet.m_ServerPort);
	if(!m_IsConnected){//如果因为注销活着刚刚启动程序,需要重新连接
		if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
			ERRORMSG;
			return;
		}

		// 建立客户端数据报套接口
		m_ClientSocket = socket(AF_INET, SOCK_STREAM, 0);
		if (m_ClientSocket == INVALID_SOCKET) {
			ERRORMSG;
			return;
		}

		if(connect(m_ClientSocket,(SOCKADDR*)&(m_serverAddr),sizeof(m_serverAddr)) == SOCKET_ERROR){
			ERRORMSG;
			closesocket(m_ClientSocket);
			WSACleanup();
			return;
		}
		m_IsConnected = true;
		//listen(m_ClientSocket, SOMAXCONN);
		r1 = CreateThread(NULL,0,Receiver,this,0,&receiverMark);
	
	
	}
	INT_PTR nRes = m_login.DoModal();
	if(nRes == IDOK){
		USER_LOGIN m_user;
		USES_CONVERSION;
		char* temp = W2A(m_login.m_pwd);
		strcpy_s(m_user.m_pwd, temp);
		memcpy(&m_user.m_id, &m_login.m_userid, sizeof(m_login.m_userid));
		
		memset(&m_sendMsg, 0, sizeof(m_sendMsg));
		m_sendMsg.m_msgType = MSG_LOGIN;
		memcpy(m_sendMsg.m_text, &m_user, sizeof(m_user));
		
			// 向服务器发送数据
		iSend = send(m_ClientSocket, (char*)&m_sendMsg, sizeof(m_sendMsg), 0);
		if (iSend == SOCKET_ERROR) {
			ERRORMSG;
			return;
		}
		else if (iSend == 0)
			return;
		else{
			m_myOwnId =  m_login.m_userid;
#ifdef DEBUG
			AfxMessageBox(_T("登陆消息发送成功\n"));
#endif
		}
		((CButton*)GetDlgItem(IDC_LOGINBTN))->SetWindowTextW(_T("登陆中..."));
		((CButton*)GetDlgItem(IDC_LOGINBTN))->EnableWindow(false);
	}
	
}




void CTalk2MeDlg::OnBnClickedSetbtn()
{
	INT_PTR nRes = m_serverSet.DoModal();
	if(nRes == IDOK){
		(CButton*)GetDlgItem(IDC_SETBTN)->EnableWindow(false);
		(CButton*)GetDlgItem(IDC_LOGINBTN)->EnableWindow(true);
		(CButton*)GetDlgItem(IDC_SIGNUPBTN)->EnableWindow(true);
		
	}
												
}



void CTalk2MeDlg::OnBnClickedSignupbtn()
{
	m_serverAddr.sin_addr.S_un.S_addr = htonl(m_serverSet.m_ServerIpAddr);
	m_serverAddr.sin_family = AF_INET;
	m_serverAddr.sin_port = htons(m_serverSet.m_ServerPort);

	if(!m_IsConnected){//如果因为注销活着刚刚启动程序,需要重新连接
		if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
			ERRORMSG;
			return;
		}

		// 建立客户端数据报套接口
		m_ClientSocket = socket(AF_INET, SOCK_STREAM, 0);
		if (m_ClientSocket == INVALID_SOCKET) {
			ERRORMSG;
			return;
		}

		if(connect(m_ClientSocket,(SOCKADDR*)&(m_serverAddr),sizeof(m_serverAddr)) == SOCKET_ERROR){
			ERRORMSG;
			closesocket(m_ClientSocket);
			WSACleanup();
			return;
		}
		m_IsConnected = true;
		r1 = CreateThread(NULL,0,Receiver,this,0,&receiverMark);
	
	}
	INT_PTR nRes = m_signup.DoModal();
	if(nRes == IDOK){
		USER_INFO m_user;
		USES_CONVERSION;
		char* temp = W2A(m_signup.m_username);
		strcpy_s(m_user.m_name, temp);
		temp = W2A(m_signup.m_pwd);
		strcpy_s(m_user.m_pwd, temp);
		memset(&m_sendMsg, 0, sizeof(m_sendMsg));
		m_sendMsg.m_msgType = MSG_SIGNUP;
		memcpy(m_sendMsg.m_text, &m_user, sizeof(m_user));
		// 向服务器发送数据
		iSend = send(m_ClientSocket, (char*)&m_sendMsg, sizeof(m_sendMsg), 0);
		if (iSend == SOCKET_ERROR) {
			ERRORMSG;
			return;
		}
		else if (iSend == 0)
			return;
		else{
#ifdef DEBUG
			AfxMessageBox(_T("注册消息发送成功\n"));
#endif
		}
		
	}
}

DWORD WINAPI Receiver(LPVOID pParam )
{
	CTalk2MeDlg* dlg = (CTalk2MeDlg*) pParam;
	
	while(dlg->m_IsConnected)
    {
		memset(&(dlg->m_recvMsg), 0, sizeof(dlg->m_recvMsg));
		memset(&(dlg->m_sendMsg), 0, sizeof(dlg->m_sendMsg));
		// 从客户端接收数据
		
		dlg->iLen = sizeof(dlg->m_serverAddr);
		dlg->iRecv = recv(dlg->m_ClientSocket,(char*)&(dlg->m_recvMsg),sizeof(dlg->m_recvMsg),0);
			
		if(dlg->iRecv == SOCKET_ERROR){
			ERRORMSG;
			break;
		}else{
			if(dlg->iRecv == 0){
				continue;
			}else{
				//TCHAR m_operation[MAX_PATH];  
				USER_LOGIN* m_userLogin;//
				USER_INFO* m_user;//登录人的姓名和密码or登录人的姓名和服务返回的登录状态
				_variant_t var;
				long m_rp;
				CString m_str;
				int* m_2int;
				TMSG* m_t;
				FMSG* m_f;
				CTMSG* m_ct;//聊天室消息
				CFile m_cFile;
				LTMSG* m_lt;
				LCTMSG* m_lct;
				
				switch(dlg->m_recvMsg.m_msgType){
				case MSG_SIGNUP_RP:
					m_rp = 0;
					m_str.Empty();
					memcpy( &m_rp, dlg->m_recvMsg.m_text, sizeof(long));
					m_str.Format(_T("注册成功！请记住自己的ID：%ld"),m_rp);
					AfxMessageBox(m_str);
					break;
				case MSG_LOGIN_RP:
					m_user = new USER_INFO;
					memset(m_user, 0 , sizeof(USER_INFO));
					memcpy(m_user, dlg->m_recvMsg.m_text, sizeof(USER_INFO));
					memcpy(dlg->m_username,m_user->m_name, DB_LENGTH);
					m_str.Empty();
					m_str = CString(m_user->m_pwd);
					if(m_str == _T("登陆成功!")){
						dlg->m_myOwnId = dlg->m_login.m_userid;
						m_str.Empty();
						m_str.Format(_T("%d_"),dlg->m_myOwnId);
						m_str += CString(m_user->m_name);
						(CWnd::FromHandle(dlg->m_hWnd))->SetWindowTextW(m_str);
						
						(CButton*)dlg->GetDlgItem(IDC_LOGOUTBTN)->EnableWindow(true);
						(CButton*)dlg->GetDlgItem(IDC_CHATROOM_BTN)->EnableWindow(true);
						(CButton*)dlg->GetDlgItem(IDC_STARTTALK_BUTTON)->EnableWindow(true);
						(CTabCtrl*)dlg->GetDlgItem(IDC_TAB1)->EnableWindow(true);
						(*(CEdit*)dlg->GetDlgItem(IDC_ID_EDIT)).SetReadOnly(false);
						((CButton*)dlg->GetDlgItem(IDC_LOGINBTN))->SetWindowTextW(_T("登陆成功"));
						((CButton*)dlg->GetDlgItem(IDC_LOGINBTN))->EnableWindow(false);
						dlg->OnBnClickedRefreshButton();
					}else{
						if(m_str == _T("登陆失败!")){
							dlg->m_IsConnected = false;
							dlg->m_myOwnId = -1;
							AfxMessageBox(_T("密码或者是ID错误,请重试!"));
						}else{
							AfxMessageBox(_T("您已经在其他终端登陆,请注意隐私安全!"));
						}
						(CButton*)dlg->GetDlgItem(IDC_LOGOUTBTN)->EnableWindow(false);
						(CButton*)dlg->GetDlgItem(IDC_CHATROOM_BTN)->EnableWindow(false);
						(CButton*)dlg->GetDlgItem(IDC_STARTTALK_BUTTON)->EnableWindow(false);
						(CTabCtrl*)dlg->GetDlgItem(IDC_TAB1)->EnableWindow(false);
						(*(CEdit*)dlg->GetDlgItem(IDC_ID_EDIT)).SetReadOnly(true);
						((CButton*)dlg->GetDlgItem(IDC_LOGINBTN))->SetWindowTextW(_T("请登陆"));
						((CButton*)dlg->GetDlgItem(IDC_LOGINBTN))->EnableWindow(true);
						
					}
					
					delete m_user;
					break;
				case MSG_SEARCH_RP:
					m_userLogin = new USER_LOGIN;
					m_str.Empty();
					m_str = CString(dlg->m_recvMsg.m_text);
					if(m_str == _T("查找失败!")){
					//button set
						AfxMessageBox(_T("用户不存在"));
					}else{
						memcpy( m_userLogin, dlg->m_recvMsg.m_text, sizeof(USER_LOGIN));
						if(dlg->m_id2Conversation.find(m_userLogin->m_id) == dlg->m_id2Conversation.end())				
							dlg->m_id2Conversation.insert(std::pair<int, CConversation*>(m_userLogin->m_id, (new CConversation(m_userLogin->m_id,CString(m_userLogin->m_pwd)))));
						CWnd::FromHandle(dlg->m_hWnd)->SendMessage(WM_RecvMsg,m_userLogin->m_id);
					}
					delete m_userLogin;
					break;
				case MSG_SEARCHCHAT_RP:
					m_str.Empty();
					m_userLogin = new USER_LOGIN[10];
					memset( m_userLogin, 0, 10 *sizeof(USER_LOGIN));
					memcpy( m_userLogin, dlg->m_recvMsg.m_text, 10 *sizeof(USER_LOGIN));
					for(int i = 1; i < 10; ++i){
						if(m_userLogin[i].m_id == 0)
							break;
						if(m_userLogin[i].m_id < 0){
							m_userLogin[i].m_id *= -1;
							m_str.Format(_T("%.5d 【离线】 "),m_userLogin[i].m_id);
						}else{
							m_str.Format(_T("%.5d 【在线】 "),m_userLogin[i].m_id);
						}
						
						
						m_str += m_userLogin[i].m_pwd;
						
						for(map<int,CChatRoom*>::iterator first = dlg->m_id2Chatroom.begin(); first != dlg->m_id2Chatroom.end();++first){
							if(first->second->m_hWnd == NULL)
								continue;
							if(first->first == m_userLogin[0].m_id){
								((CListBox*)first->second->GetDlgItem(IDC_CHATMEMBERLIST ))->AddString(m_str);
								break;
							}
						}
					}
					delete[] m_userLogin;
					break;
				case MSG_TEXT://一对一文本消息
					m_t = new TMSG;
					memset(m_t, 0, sizeof(TMSG));
					memcpy(m_t, dlg->m_recvMsg.m_text, sizeof(TMSG));
					if(dlg->m_id2Conversation.find(m_t->m_idFrom) == dlg->m_id2Conversation.end()){	//对话本来不存在
						
						CConversation* newConversation = new CConversation(m_t->m_idFrom,CString(m_t->m_name));//内存清除
						newConversation->m_history.Format(_T("\n\n%d号"),m_t->m_idFrom);
						newConversation->m_history += CString(m_t->m_name);
						newConversation->m_history += CString(_T("说："));
						newConversation->m_history += CString(m_t->m_text);
					
						dlg->m_id2Conversation.insert(std::pair<int, CConversation*>(m_t->m_idFrom, newConversation));
						CWnd::FromHandle(dlg->m_hWnd)->SendMessage(WM_RecvMsg,m_t->m_idFrom);
					}else{
						CConversation* newConversation = (dlg->m_id2Conversation).at(m_t->m_idFrom);
						m_str.Format(_T("\n\n%d号"),m_t->m_idFrom); 
						newConversation->m_history += m_str;
						
						newConversation->m_history += CString(m_t->m_name);
						newConversation->m_history += CString(_T("说："));
						newConversation->m_history += CString(m_t->m_text);				
						CWnd::FromHandle(newConversation->m_hWnd)->GetDlgItem(IDC_HISTORYRICHEDIT)->SetWindowTextW(newConversation->m_history);
						newConversation->m_Scroll.LineScroll(newConversation->m_Scroll.GetLineCount());	
					}
					delete m_t;
					break;
				case MSG_FILETYPE:
					m_lt = new LTMSG;

					memset(m_lt, 0, sizeof(LTMSG));
					memcpy(m_lt, dlg->m_recvMsg.m_text, sizeof(LTMSG));
					//显示有未下载的文件,点击接收,取消或者下次接收
					if(true){
						CFileRecv* m_file2Recv = new CFileRecv;
						m_file2Recv->m_fileName.Format(_T("%d_%d_"),m_lt->m_idFrom,m_lt->m_id);
						m_file2Recv->m_fileName += CString(m_lt->m_name);
						m_file2Recv->m_fileName += CString(_T("_"));
						m_file2Recv->m_fileName += CString(m_lt->m_text);
						m_file2Recv->m_fromId.Format(_T("%d"),m_lt->m_idFrom);
						m_file2Recv->m_isChatFile = false;
						m_file2Recv->m_fileLength = m_lt->m_fileLength;
						dlg->m_file2SaveDlg.insert(std::pair<CString, CFileRecv*>(m_file2Recv->m_fileName, m_file2Recv));
						CWnd::FromHandle(dlg->m_hWnd)->SendMessage(WM_RecvFileSave,(WPARAM)&m_file2Recv->m_fileName,false);//121
					}
					
					delete m_lt;
					break;
				case MSG_CHATFILETYPE://收到群发文件
					m_lct = new LCTMSG;
					memset(m_lct, 0, sizeof(LCTMSG));
					memcpy(m_lct, dlg->m_recvMsg.m_text, sizeof(LCTMSG));
					//显示有未下载的文件,点击接收,取消或者下次接收
					if(true){
						CFileRecv* m_file2Recv = new CFileRecv;
						m_file2Recv->m_fileName.Format(_T("%d_%d_%d_"),m_lct->m_idFrom,m_lct->m_cId,m_lct->m_ownerId);
						m_file2Recv->m_fileName += CString(m_lct->m_name);
						m_file2Recv->m_fileName += CString(_T("_"));
						m_file2Recv->m_fileName += CString(m_lct->m_text);
						m_file2Recv->m_fromId.Format(_T("%d"),m_lct->m_idFrom);
						m_file2Recv->m_isChatFile = true;
						m_file2Recv->m_fileLength = m_lct->m_fileLength;
						dlg->m_file2SaveDlg.insert(std::pair<CString, CFileRecv*>(m_file2Recv->m_fileName, m_file2Recv));
						CWnd::FromHandle(dlg->m_hWnd)->SendMessage(WM_RecvFileSave,(WPARAM)&m_file2Recv->m_fileName,true);//聊天室
					}
					
					delete m_lct;
					break;
				case MSG_FILEEND:
					m_str.Empty();
					m_str = CString(dlg->m_recvMsg.m_text);//文件名字
					//delete dlg->m_File2FileName.find(m_str)->second;
					dlg->m_File2FileName.erase(dlg->m_File2FileName.find(m_str));
					//delete dlg->m_file2SaveDlg.find(m_str)->second;
					dlg->m_file2SaveDlg.erase(dlg->m_file2SaveDlg.find(m_str));
					::SendMessage(dlg->m_File2ProgressBar.at(m_str)->m_hWnd,WM_CLOSE,0,0);
					//delete dlg->m_File2ProgressBar.find(m_str)->second;
					dlg->m_File2ProgressBar.erase(dlg->m_File2ProgressBar.find(m_str));
					m_t = new TMSG;
					memset(m_t, 0, sizeof(TMSG));
					
					if(true){	
						CString csTemp;//分离文件名字中的id
						AfxExtractSubString(csTemp, (LPCTSTR)m_str, 0, '_');
						m_t->m_idFrom = _ttoi(csTemp);
						AfxExtractSubString(csTemp, (LPCTSTR)m_str, 1, '_');
						m_t->m_id = _ttoi(csTemp);
						AfxExtractSubString(csTemp, (LPCTSTR)m_str, 2, '_');
						USES_CONVERSION;
						char* temp = W2A(csTemp);
						strcpy_s(m_t->m_name, temp);
						AfxExtractSubString(csTemp, (LPCTSTR)m_str, 3, '_');
						
						temp = W2A(csTemp);
						strcpy_s(m_t->m_text, temp);
						//delete dlg->m_file2SaveDlg.find(csTemp)->second;
						
						if(dlg->m_id2Conversation.find(m_t->m_idFrom) == dlg->m_id2Conversation.end()){	//对话本来不存在
						
							CConversation* newConversation = new CConversation(m_t->m_idFrom,CString(m_t->m_name));//内存清除
						
						
							dlg->m_id2Conversation.insert(std::pair<int, CConversation*>(m_t->m_idFrom, newConversation));
							CWnd::FromHandle(dlg->m_hWnd)->SendMessage(WM_RecvMsg,m_t->m_idFrom);
							newConversation->m_content = CString(_T("成功接收文件:"));
							newConversation->m_content += CString(m_t->m_text);
							
							CWnd::FromHandle(dlg->m_hWnd)->SendMessage(WM_SendMsg,m_t->m_idFrom);
						}else{
							CConversation* newConversation = (dlg->m_id2Conversation).at(m_t->m_idFrom);
							newConversation->m_content = CString(_T("成功接收文件:"));
							newConversation->m_content += CString(m_t->m_text);
							CWnd::FromHandle(dlg->m_hWnd)->SendMessage(WM_SendMsg,m_t->m_idFrom);
							newConversation->m_Scroll.LineScroll(newConversation->m_Scroll.GetLineCount());	
						}
						
					}
					delete m_t;			
					
					break;
				case MSG_CHATFILEEND:
					m_str.Empty();
					
					m_str = CString(dlg->m_recvMsg.m_text);//文件名字
					//delete dlg->m_File2FileName.find(m_str)->second;
					dlg->m_File2FileName.erase(dlg->m_File2FileName.find(m_str));
					//delete dlg->m_file2SaveDlg.find(m_str)->second;
					dlg->m_file2SaveDlg.erase(dlg->m_file2SaveDlg.find(m_str));
					::SendMessage(dlg->m_File2ProgressBar.at(m_str)->m_hWnd,WM_CLOSE,0,0);
					//delete dlg->m_File2ProgressBar.find(m_str)->second;
					dlg->m_File2ProgressBar.erase(dlg->m_File2ProgressBar.find(m_str));
					m_ct = new CTMSG;
					memset(m_ct, 0, sizeof(CTMSG));
					if(true){
						CString csTemp;//分离文件名字中的id
						AfxExtractSubString(csTemp, (LPCTSTR)m_str, 0, '_');
						m_ct->m_idFrom = _ttoi(csTemp);
						AfxExtractSubString(csTemp, (LPCTSTR)m_str, 1, '_');
						m_ct->m_cId = _ttoi(csTemp);
						AfxExtractSubString(csTemp, (LPCTSTR)m_str, 2, '_');
						m_ct->m_ownerId = _ttoi(csTemp);
						AfxExtractSubString(csTemp, (LPCTSTR)m_str, 3, '_');
						USES_CONVERSION;
						char* temp = W2A(csTemp);
						strcpy_s(m_ct->m_name, temp);
						AfxExtractSubString(csTemp, (LPCTSTR)m_str, 4, '_');
						
						temp = W2A(csTemp);
						strcpy_s(m_ct->m_text, temp);

						if(dlg->m_id2Chatroom.find(m_ct->m_cId) == dlg->m_id2Chatroom.end()){	//聊天室本来不存在
							CChatRoom* newConversation = new CChatRoom(m_ct->m_cId, m_ct->m_ownerId);//内存清除
						
					
							dlg->m_id2Chatroom.insert(std::pair<int, CChatRoom*>(m_ct->m_cId, newConversation));
							CWnd::FromHandle(dlg->m_hWnd)->SendMessage(WM_RecvChatMsg,m_ct->m_cId);
							newConversation->m_content = CString(_T("成功接收文件:"));
							newConversation->m_content += CString(m_ct->m_text);
							CWnd::FromHandle(dlg->m_hWnd)->SendMessage(WM_SendChatMsg,m_ct->m_cId);
						}else{
							CChatRoom* newConversation = (dlg->m_id2Chatroom).at(m_ct->m_cId);
							newConversation->m_content = CString(_T("成功接收文件:"));
							newConversation->m_content += CString(m_ct->m_text);
							CWnd::FromHandle(dlg->m_hWnd)->SendMessage(WM_SendChatMsg,m_ct->m_cId);
							newConversation->m_Scroll.LineScroll(newConversation->m_Scroll.GetLineCount());	
						}
					}
					delete m_ct;
					break;

				case MSG_FILE:
					m_f = new FMSG;
					memcpy(m_f, dlg->m_recvMsg.m_text, sizeof(FMSG));
					if(m_cFile.Open(dlg->m_File2FileName.at(CString(m_f->m_filename)) , CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite | CFile::typeBinary) == NULL){
						ERRORMSG;
						break;
					}else{
						dlg->m_File2ProgressBar.at(CString(m_f->m_filename))->SetProgress(sizeof(m_f->m_text));
						m_cFile.SeekToEnd();
						m_cFile.Write(m_f->m_text, sizeof(m_f->m_text));
						m_cFile.Close();
					}
					delete m_f;
					break;
				
				case MSG_BROADCAST_RP://当前注册用户的信息
					m_str.Empty();
					m_userLogin = new USER_LOGIN[10];
					memset( m_userLogin, 0, 10 *sizeof(USER_LOGIN));
					memcpy( m_userLogin, dlg->m_recvMsg.m_text, 10 *sizeof(USER_LOGIN));
					for(int i = 0; i < 10; ++i){
						if(m_userLogin[i].m_id == 0)
							break;
						if(m_userLogin[i].m_id == dlg->m_myOwnId)//不显示当前用户
							continue;
						if(m_userLogin[i].m_id < 0){
							m_userLogin[i].m_id *= -1;
							m_str.Format(_T("%.5d 【离线】 "),m_userLogin[i].m_id);
						}else{
							m_str.Format(_T("%.5d 【在线】 "),m_userLogin[i].m_id);
						}
						dlg->m_onlineFriends.push_back(m_userLogin[i]);
						
						m_str += m_userLogin[i].m_pwd;
						(*(CListBox*)dlg->m_friendTab.GetDlgItem(IDC_LOCALPARTNERLIST)).AddString(m_str);
						for(map<int,CChatRoom*>::iterator first = dlg->m_id2Chatroom.begin(); first != dlg->m_id2Chatroom.end();++first){
							if(first->second->m_hWnd == NULL)
								continue;
							((CListBox*)first->second->GetDlgItem(IDC_OULIST ))->AddString(m_str);
						}
					}
					delete[] m_userLogin;
					break;
				case MSG_CHATBROADCAST_RP://查询参与的聊天室消息
					m_2int = new int[126];
					memset(m_2int, -1, 126*sizeof(int));
					m_str.Empty();
					
					memcpy( m_2int, dlg->m_recvMsg.m_text, 126 *sizeof(int));
					for(int i = 0; i < 126; i+=2){
						if(m_2int[i] == -1)
							break;
						m_str.Format(_T("%.5d 号聊天室【%.5d 】"),m_2int[i],m_2int[i+1]);
						/*if(dlg->m_id2Chatroom.find(m_2int[i]) == dlg->m_id2Chatroom.end()){
							dlg->m_id2Chatroom.insert(std::pair<int,CChatRoom*>(m_2int[i], new CChatRoom(m_2int[i], m_2int[i+1])));
						}*/
						
						(*(CListBox*)dlg->m_chatRoomTab.GetDlgItem(IDC_ALLCHATROOMS)).AddString(m_str);
						
					}
					delete[] m_2int;
					break;
				case MSG_CHATROOM_RP:
					m_str.Empty();
					m_2int = new int[2];//0表示聊天室id,1表示founder
					memcpy( m_2int, dlg->m_recvMsg.m_text, 2 * sizeof(int));
					m_str.Format(_T("聊天室创建成功！ID：%ld"),m_2int[0]);
					if(dlg->m_id2Chatroom.find((int)m_2int[0]) == dlg->m_id2Chatroom.end())				
						dlg->m_id2Chatroom.insert(std::pair<int, CChatRoom*>(m_2int[0], (new CChatRoom(m_2int[0], m_2int[1]))));
					CWnd::FromHandle(dlg->m_hWnd)->SendMessage(WM_RecvChatMsg,m_2int[0]);
					AfxMessageBox(m_str);
					delete[] m_2int;
					break;
				case MSG_CHATTXT:
					m_ct = new CTMSG;
					memset(m_ct, 0, sizeof(CTMSG));
					memcpy(m_ct, dlg->m_recvMsg.m_text, sizeof(CTMSG));
					if(dlg->m_id2Chatroom.find(m_ct->m_cId) == dlg->m_id2Chatroom.end()){	//聊天室本来不存在
						CChatRoom* newConversation = new CChatRoom(m_ct->m_cId, m_ct->m_ownerId);//内存清除
						newConversation->m_history.Format(_T("\n\n%d号"),m_ct->m_idFrom);
						newConversation->m_history += CString(m_ct->m_name);
						newConversation->m_history += CString(_T("说："));
						newConversation->m_history += CString(m_ct->m_text);
					
						dlg->m_id2Chatroom.insert(std::pair<int, CChatRoom*>(m_ct->m_cId, newConversation));
						CWnd::FromHandle(dlg->m_hWnd)->SendMessage(WM_RecvChatMsg,m_ct->m_cId);
					}else{
						CChatRoom* newConversation = (dlg->m_id2Chatroom).at(m_ct->m_cId);
						m_str.Format(_T("\n\n%d号"),m_ct->m_idFrom); 
						newConversation->m_history += m_str;
						newConversation->m_history += CString(m_ct->m_name);
						newConversation->m_history += CString(_T("说："));
						newConversation->m_history += CString(m_ct->m_text);					
						CWnd::FromHandle(newConversation->m_hWnd)->GetDlgItem(IDC_CHATROOMRE)->SetWindowTextW(newConversation->m_history);
						newConversation->m_Scroll.LineScroll(newConversation->m_Scroll.GetLineCount());	
					}
					delete m_ct;
					break;
				
				}
						
			}		
		}
		
	}
	return 0;
}

void CTalk2MeDlg::OnBnClickedChatroomBtn()
{	
	memset(&m_sendMsg, 0, sizeof(m_sendMsg));
	m_sendMsg.m_msgType = MSG_CHATROOM;
	memcpy(m_sendMsg.m_text, &m_myOwnId, sizeof(m_myOwnId));
		
		// 向服务器发送数据
	iSend = send(m_ClientSocket, (char*)&m_sendMsg, sizeof(m_sendMsg), 0);
	if (iSend == SOCKET_ERROR) {
		ERRORMSG;
		return;
	}
	else if (iSend == 0)
		return;
	else{
#ifdef DEBUG
		AfxMessageBox(_T("新建聊天室消息发送成功\n"));
#endif
	}
}


void CTalk2MeDlg::OnBnClickedLogoutbtn()//只是断开连接,注销,但不退出程序
{
	(CButton*)GetDlgItem(IDC_LOGINBTN)->EnableWindow(true);
	(CButton*)GetDlgItem(IDC_SIGNUPBTN)->EnableWindow(true);
	(CButton*)GetDlgItem(IDC_LOGOUTBTN)->EnableWindow(false);
	(CButton*)GetDlgItem(IDC_CHATROOM_BTN)->EnableWindow(false);
	(CButton*)GetDlgItem(IDC_STARTTALK_BUTTON)->EnableWindow(false);
	(CTabCtrl*)GetDlgItem(IDC_TAB1)->EnableWindow(false);
	(*(CEdit*)GetDlgItem(IDC_ID_EDIT)).SetReadOnly(true);
	((CButton*)GetDlgItem(IDC_LOGINBTN))->SetWindowTextW(_T("请登陆"));
	
	
	USER_LOGIN m_user;
	m_user.m_id = m_myOwnId;
		
	memset(&m_sendMsg, 0, sizeof(m_sendMsg));
	m_sendMsg.m_msgType = MSG_LOGOUT;
	memcpy(m_sendMsg.m_text, &m_user, sizeof(USER_LOGIN));
		
		// 向服务器发送数据
	iSend = send(m_ClientSocket, (char*)&m_sendMsg, sizeof(m_sendMsg), 0);
	if (iSend == SOCKET_ERROR){
		ERRORMSG;
		return;
	}else if (iSend == 0)
		return;
	else{
		m_myOwnId =  m_login.m_userid;
#ifdef DEBUG
		AfxMessageBox(_T("注销消息发送成功\n"));
#endif
	}
	m_IsConnected = false;
		
}


void CTalk2MeDlg::OnBnClickedGetroombtn()
{
	
	((CListBox*)m_chatRoomTab.GetDlgItem(IDC_ALLCHATROOMS))->ResetContent();//清空当前存储的所有参与的聊天室
	
	memset(&m_sendMsg, 0, sizeof(m_sendMsg));
	m_sendMsg.m_msgType = MSG_CHATBROADCAST;
	memcpy(m_sendMsg.m_text, &m_myOwnId, sizeof(m_myOwnId));
		// 向服务器发送数据
	iSend = send(m_ClientSocket, (char*)&m_sendMsg, sizeof(m_sendMsg), 0);
	if (iSend == SOCKET_ERROR) {
		ERRORMSG;
		return;
	}
	else if (iSend == 0)
		return;
	else{
#ifdef DEBUG
		AfxMessageBox(_T("查询参与的聊天室消息发送成功\n"));
#endif
	}
}






void CTalk2MeDlg::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	int CurSel = m_tabCtrl.GetCurSel();

    switch(CurSel)
    {
    case 0:
		m_friendTab.ShowWindow(true);
		m_chatRoomTab.ShowWindow(false);
		OnBnClickedRefreshButton();//调用函数向服务器查询所有注册的用户
        break;
    case 1:
        m_friendTab.ShowWindow(false);
        m_chatRoomTab.ShowWindow(true);
		OnBnClickedGetroombtn();//调用函数向服务器发送查询参与的聊天室的消息
        break;
    }
    *pResult = 0;
	
}
