#pragma once
#include"header.h"
#include "afxdialogex.h"

#include "Talk2Me.h"
#include "afxcmn.h"

using namespace std;
// CConversation �Ի���
class CConversation : public CDialogEx
{
	DECLARE_DYNAMIC(CConversation)

public:
	CConversation(int id, CString title, CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CConversation();

// �Ի�������
	enum { IDD = IDD_COVERSATION };
	

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual void OnClose();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedSendfilebtn();//��������ļ�
	afx_msg void OnBnClickedSenfcontentbtn();//���������Ϣ
	
	CString m_history;//��ʷ��¼
	CString m_content;//��Ϣ����
	CString m_filePath;//�ļ�·��
	CString m_fileEx;//�ļ���
	
	CRichEditCtrl m_Scroll;
	CString m_title;//���������ǳƣ���ʾ�ڶԻ���title����
	int m_fId;//��������ID
	
};
