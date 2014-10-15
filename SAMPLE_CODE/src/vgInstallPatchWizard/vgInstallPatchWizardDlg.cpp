// vgTemplateSettingDlgDlg.cpp : ʵ���ļ�
//

#include "vgStableHeaders.h"
#include "vgInstallPatchWizard.h"
#include "vgInstallPatchWizardDlg.h"


#include "vgPatchUtility.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//��ʶ�����ļ�����ռ�������İٷֱ�
const int percentRate = 8;   /* 80% */

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


// CvgInstallPatchWizardDlg �Ի���




CvgInstallPatchWizardDlg::CvgInstallPatchWizardDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CvgInstallPatchWizardDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	_desDir = "";
	_numOfFiles = 0;
	_progressPos = 0;
}

void CvgInstallPatchWizardDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS1, m_progressCtrl);
}

BEGIN_MESSAGE_MAP(CvgInstallPatchWizardDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_INSTALL, &CvgInstallPatchWizardDlg::OnBnClickedInstall)
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CvgInstallPatchWizardDlg ��Ϣ�������

BOOL CvgInstallPatchWizardDlg::OnInitDialog()
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
	CStatic* pStatic = ((CStatic*)GetDlgItem(IDC_STATIC_TEXT));
	pStatic->SetWindowText( "��ӭʹ��VRGIS������װ����!" );
	
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CvgInstallPatchWizardDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CvgInstallPatchWizardDlg::OnPaint()
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
		//�Ի���������������ǰ����ʾ
		CRect rect;
		GetWindowRect( &rect );
		SetWindowPos(&(AfxGetApp()->m_pMainWnd->wndTopMost), rect.left, rect.top, 
			rect.Width(), rect.Height(), SWP_SHOWWINDOW );

		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù����ʾ��
//
HCURSOR CvgInstallPatchWizardDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CvgInstallPatchWizardDlg::OnBnClickedInstall()
{
	CStatic* pStatic = ((CStatic*)GetDlgItem( IDC_STATIC_TEXT ));
	pStatic->SetWindowText( "����������װĿ¼..." );

	//��ע����ȡ��װĿ¼
	if ( !this->getDesPathFormRegedit() )
	{
		return;
	}

	//_desDir = "E:\\��Ŀ����\\VRGISƽ̨\\VRGisSource\\temp\\test\\";    //���Դ���


	GroupNodeItemPtr pRootNode;
	pRootNode = _nodeManger.getVersionByVersionName( String("all") );
	assert( pRootNode != NULL );

	String srcDir = _nodeManger.getPathIn();
	assert( srcDir != "" );

	pStatic->SetWindowText( "�������ɸ�����Ϣ..." );

	//������״Ŀ¼
	this->getDirectorySystemIn( pRootNode, srcDir );

	//////////////////////////////////////////////////////////////////////////
	//���ý�������Χ
	m_progressCtrl.SetRange( 0, _numOfFiles*10 );

	_progressPos = _numOfFiles * ( 10 - percentRate );
	m_progressCtrl.SetPos( _progressPos );
	//////////////////////////////////////////////////////////////////////////

	//��ʼ�����ļ�
	String desDir = _desDir;
	this->copyPatchFiles( pRootNode, desDir );


	pStatic->SetWindowText( "������ɣ��밴��ȷ������Ŧ��������!" );
	CButton* pButton = ((CButton*)GetDlgItem( IDC_INSTALL ));
	pButton->EnableWindow( FALSE );

}

//----------------------------------------------------------------
bool CvgInstallPatchWizardDlg::getDesPathFormRegedit()
{
	//��ע����л�ȡ��װĿ¼
	bool result = false;

	HKEY      hKEY;       
	LPCTSTR   data_Set = ".vg\\shell\\open\\command\\";

	long  ret0 = ( ::RegOpenKeyEx( HKEY_CLASSES_ROOT, data_Set, 0, KEY_READ, &hKEY ) );   //��ע�����  
	if( ret0 != ERROR_SUCCESS )   
	{
		MessageBox( "����:�޷����йص�hKEY!�������Ƿ��Ѿ���װ��VRGISƽ̨��" );     
		return false;
	}     

	LPBYTE   path_Get = new BYTE[256];    
	DWORD    type = REG_SZ;
	DWORD    cbData = 256;  

	long ret1 = ::RegQueryValueEx( hKEY, NULL, NULL, &type, path_Get, &cbData );    //��ѯ�йص����� 
	if( ret1 != ERROR_SUCCESS )     
	{     
		MessageBox("����:�޷���ѯ�й�ע�����Ϣ!�������Ƿ��Ѿ���װ��VRGISƽ̨��" );     
		return false;     
	}     

	::RegCloseKey( hKEY );     //�������ǰҪ�ر��Ѿ��򿪵�hKEY 

	CString exeAbsPath = (CString)path_Get;
	delete[] path_Get;
	path_Get = NULL;

	_desDir = vgUtility::PatchUtility::getPathFromAbsoluteFilename( (String)exeAbsPath );
	result = true;

	if ( result == false )
	{
		string msg = "����:�޷���ѯ�й�ע�����Ϣ!";
		::MessageBox( AfxGetApp()->m_pMainWnd->m_hWnd, msg.c_str(), 
			"����", MB_OK | MB_ICONASTERISK );  
		return false;     
	}  

	return result;
}

//----------------------------------------------------------------
int CvgInstallPatchWizardDlg::getDirectorySystemIn( GroupNodeItemPtr pGroupNode, String filePath,
												 int* nCountFile/* = NULL*/, int* nCountDir/*NULL*/  )
{
	int nCountLeaf(0), nCountBranch(0);

	using namespace vgGroup;

	GroupNodeItemPtr pTempParentNode = pGroupNode;

	TCHAR szFind[MAX_PATH]; 
	String src_path(filePath);
	String src_subpath;

	lstrcpy(szFind, src_path.c_str() ); 
	lstrcat(szFind, "\\"); 
	lstrcat(szFind, "*.*"); // �������ļ� 

	WIN32_FIND_DATA wfd; 

	HANDLE hFind = FindFirstFile(szFind, &wfd); 

	if (hFind == INVALID_HANDLE_VALUE) // ���û���ҵ������ʧ�� 
		return false; 	
	do 
	{ 
		if (wfd.cFileName[0] == '.') 
			continue; // ����������Ŀ¼ 

		if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) 
		{ 
			src_subpath = src_path + "\\" + wfd.cFileName;

			GroupNodeItem	subGroupNode( String(wfd.cFileName), pTempParentNode, 0, GROUP_NODE_BRANCH);

			getDirectorySystemIn( &subGroupNode, src_subpath, nCountFile, nCountDir );	

			pTempParentNode->appendSubItem( subGroupNode );

		} 
		else 
		{ 
			_numOfFiles ++;  //�ļ������ۼ�
			pTempParentNode->appendSubItem( String(wfd.cFileName) );
		} 

	} while (FindNextFile(hFind, &wfd)); 

	FindClose(hFind); // �رղ��Ҿ�� 

	return true;
}

//----------------------------------------------------------------
bool CvgInstallPatchWizardDlg::copyPatchFiles( GroupNodeItemPtr pGroupNode, String dir )
{
	String dirParent = dir;

	GroupNodeItemVec*	pSubItems = pGroupNode->getSubItems();
	GroupNodeItemVec::iterator itr = pSubItems->begin();
	for ( ; itr != pSubItems->end(); itr ++)
	{
		if (  itr->isLeafItem() )
		{
			String fileSrc = itr->getParentNode()->getPath();
			fileSrc += itr->getName();

			String fileDes = dirParent + itr->getName();

			//������ڸ��µ��ļ���Ϣ
			CStatic* pStatic = ((CStatic*)GetDlgItem( IDC_STATIC_TEXT ));
			String msg = "���ڸ����ļ� " + itr->getName(); 
			pStatic->SetWindowText( msg.c_str() );

			//�����ļ�
			vgUtility::PatchUtility::copyFile(fileSrc, fileDes);

			//���ý�����
			_progressPos += percentRate;
			m_progressCtrl.SetPos( _progressPos );
		}
		else 
		{
			String dirTemp = dirParent + itr->getName() + '\\';
			vgUtility::PatchUtility::createDirectory( dirTemp );
			copyPatchFiles(&(*itr), dirTemp );//�ݹ���� 
		}
	}

	return true;
}



