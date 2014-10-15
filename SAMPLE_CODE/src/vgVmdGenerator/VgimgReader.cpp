// VgimgReader.cpp : ����Ӧ�ó��������Ϊ��
//

#include <vgStableHeaders.h>
#include "VgimgReader.h"
#include "MainFrm.h"


#include "LeftView.h"

#include "SaveVgimgDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CVgimgReaderApp

BEGIN_MESSAGE_MAP(CVgimgReaderApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CVgimgReaderApp::OnAppAbout)
	// �����ļ��ı�׼�ĵ�����
	ON_COMMAND(ID_FILE_NEW, &CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinApp::OnFileOpen)
END_MESSAGE_MAP()


// CVgimgReaderApp ����

CVgimgReaderApp::CVgimgReaderApp()
{
}
CVgimgReaderApp::~CVgimgReaderApp()
{

}



CVgimgReaderApp theApp;


// CVgimgReaderApp ��ʼ��

BOOL CVgimgReaderApp::InitInstance()
{

	CSaveVgimgDlg dlg;
	dlg.DoModal();

	return true;

}



// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// �������жԻ����Ӧ�ó�������
void CVgimgReaderApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}


// CVgimgReaderApp ��Ϣ�������

