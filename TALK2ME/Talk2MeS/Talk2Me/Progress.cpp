// Progress.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Talk2Me.h"
#include "Progress.h"
#include "afxdialogex.h"


// CProgress �Ի���

IMPLEMENT_DYNAMIC(CProgress, CDialogEx)

CProgress::CProgress(long long range,CString tip, CWnd* pParent /*=NULL*/)
	: CDialogEx(CProgress::IDD, pParent)
	, m_filename(tip)
{
	//m_ProCtrl.SetRange32(0,(int)range);

}

CProgress::~CProgress()
{
}

void CProgress::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS1, m_ProCtrl);
	DDX_Text(pDX, IDC_FILENAMESTC, m_filename);
}


BEGIN_MESSAGE_MAP(CProgress, CDialogEx)
END_MESSAGE_MAP()


// CProgress ��Ϣ�������
void CProgress::SetProgress(short step){

	m_ProCtrl.OffsetPos(step); //���ý�������λ��


}