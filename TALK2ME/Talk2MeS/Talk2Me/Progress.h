#pragma once
#include "afxcmn.h"


// CProgress �Ի���

class CProgress : public CDialogEx
{
	DECLARE_DYNAMIC(CProgress)

public:
	CProgress(long long range,CString tip,CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CProgress();
	void SetProgress(short step);//filename�����ϴ��������ַ�
// �Ի�������
	enum { IDD = IDD_PROGRESS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CProgressCtrl m_ProCtrl;
	CString m_filename;
};
