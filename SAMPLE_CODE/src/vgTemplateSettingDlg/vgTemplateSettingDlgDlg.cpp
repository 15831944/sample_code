// vgTemplateSettingDlgDlg.cpp : ʵ���ļ�
//

#include "vgStableHeaders.h"
#include "vgTemplateSettingDlg.h"
#include "vgTemplateSettingDlgDlg.h"


#include <vgKernel/vgkSystemUtility.h>
#include <vgKernel/vgkStringUtility.h>
#include <vgKernel/vgkStringVector.h>

#include "vgGroup/vgGroupNodeManager.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


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


// CvgTemplateSettingDlgDlg �Ի���




CvgTemplateSettingDlgDlg::CvgTemplateSettingDlgDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CvgTemplateSettingDlgDlg::IDD, pParent)
	, m_strDirIn(_T(""))
	, m_strDirOut(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CvgTemplateSettingDlgDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strDirIn);
	DDX_Text(pDX, IDC_EDIT2, m_strDirOut);
}

BEGIN_MESSAGE_MAP(CvgTemplateSettingDlgDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, &CvgTemplateSettingDlgDlg::OnBnClickedButton1Executing)
	ON_BN_CLICKED(IDC_BUTTON2, &CvgTemplateSettingDlgDlg::OnBnClickedButton2OpenPathIn)
	ON_BN_CLICKED(IDC_BUTTON3, &CvgTemplateSettingDlgDlg::OnBnClickedButton3OpenPathOut)
END_MESSAGE_MAP()


// CvgTemplateSettingDlgDlg ��Ϣ�������

BOOL CvgTemplateSettingDlgDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CvgTemplateSettingDlgDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CvgTemplateSettingDlgDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ��������о���
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
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù����ʾ��
//
HCURSOR CvgTemplateSettingDlgDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CvgTemplateSettingDlgDlg::OnBnClickedButton1Executing()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData();

	using namespace vgGroup;
#if 0
	GroupNodeManager directoryIn, directoryOutDebug, directoryOutRelease;
	GroupNodeItemPtr	pRootNodeIn, pRootNodeOutD, pRootNodeOutR;
	
	pRootNodeIn		= directoryIn.getRootNode();
	pRootNodeOutD	= directoryOutDebug.getRootNode();
	pRootNodeOutR	= directoryOutRelease.getRootNode();

	pRootNodeIn->getName()		= m_strDirIn;
	pRootNodeOutD->getName()	= m_strDirOut;
	pRootNodeOutR->getName()	= m_strDirOut;
#endif
	return;

}

void CvgTemplateSettingDlgDlg::OnBnClickedButton2OpenPathIn()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	vgKernel::StringVector	fileExt;
	fileExt.push_back("exe");

	vgKernel::StringVector	filePaths = vgKernel::SystemUtility::
		getFilesOpenByDialog(fileExt);

	if (filePaths.empty())
	{
		return;
	}

	String filePath = vgKernel::StringUtility::
		getPathFromAbsoluteFilename(filePaths[0].c_str());

	m_strDirIn = filePath.c_str();

	UpdateData(FALSE);
}

void CvgTemplateSettingDlgDlg::OnBnClickedButton3OpenPathOut()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	vgKernel::StringVector	fileExt;
	fileExt.push_back("exe");

	String	filePath = vgKernel::SystemUtility::
		getFileSaveByDialog(fileExt);

	if (filePath.empty())
	{
		return;
	}

	filePath = vgKernel::StringUtility::
		getPathFromAbsoluteFilename(filePath);

	m_strDirOut = filePath.c_str();

	UpdateData(FALSE);

}
