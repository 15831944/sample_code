// vgReleaseAssistantDlg.cpp : ʵ���ļ�
//

#include "vgStableHeaders.h"
#include "vgReleaseAssistant.h"
#include "vgReleaseAssistantDlg.h"


#include <vgKernel/vgkSystemUtility.h>
#include <vgKernel/vgkStringUtility.h>
#include <vgKernel/vgkStringVector.h>


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


// CvgReleaseAssistantDlg �Ի���




CvgReleaseAssistantDlg::CvgReleaseAssistantDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CvgReleaseAssistantDlg::IDD, pParent)
	, m_folderName(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_strDirIn	= _versionsManager.getPathIn().c_str();

	m_strDirOut = _versionsManager.getPathOut().c_str();

	_oldTime = 0;

}

void CvgReleaseAssistantDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strDirIn);
	DDX_Text(pDX, IDC_EDIT2, m_strDirOut);
	DDX_Text(pDX, IDC_EDIT_FOLDERNAME, m_folderName);
}

BEGIN_MESSAGE_MAP(CvgReleaseAssistantDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, &CvgReleaseAssistantDlg::OnBnClickedButton1Executing)
	ON_BN_CLICKED(IDC_BUTTON2, &CvgReleaseAssistantDlg::OnBnClickedButton2OpenPathIn)
	ON_BN_CLICKED(IDC_BUTTON3, &CvgReleaseAssistantDlg::OnBnClickedButton3OpenPathOut)
	ON_BN_CLICKED(IDC_CHECK_COMMON, &CvgReleaseAssistantDlg::OnBnClickedCheckCommon)
	ON_BN_CLICKED(IDC_CHECK_DEBUG, &CvgReleaseAssistantDlg::OnBnClickedCheckDebug)
	ON_BN_CLICKED(IDC_CHECK_RELEASE, &CvgReleaseAssistantDlg::OnBnClickedCheckRelease)
	ON_BN_CLICKED(IDC_BUTTON_ADDFLOLDER, &CvgReleaseAssistantDlg::OnBnClickedButtonAddflolder)
	ON_EN_CHANGE(IDC_EDIT_FOLDERNAME, &CvgReleaseAssistantDlg::OnEnChangeEditFoldername)
	ON_BN_CLICKED(IDC_BUTTON_REMOVEFLOLDER, &CvgReleaseAssistantDlg::OnBnClickedButtonRemoveflolder)
END_MESSAGE_MAP()


// CvgReleaseAssistantDlg ��Ϣ�������

BOOL CvgReleaseAssistantDlg::OnInitDialog()
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

void CvgReleaseAssistantDlg::OnSysCommand(UINT nID, LPARAM lParam)
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


//----------------------------------------------------------------
void CvgReleaseAssistantDlg::OnPaint()
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



//----------------------------------------------------------------
HCURSOR CvgReleaseAssistantDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



//----------------------------------------------------------------
void CvgReleaseAssistantDlg::OnBnClickedButton1Executing()
{
	_oldTime = GetTickCount();
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData();

	if (m_strDirIn.IsEmpty() || m_strDirOut.IsEmpty())
	{
		AfxMessageBox("�����������");
		return;
	}
	distrubuteFiles();

	return;

}


//----------------------------------------------------------------
void CvgReleaseAssistantDlg::OnBnClickedButton2OpenPathIn()
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


//----------------------------------------------------------------
void CvgReleaseAssistantDlg::OnBnClickedButton3OpenPathOut()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
#if 0
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
#endif
	CString selectPath = BrowseFolder( this->m_hWnd,"ѡ��Ŀ¼" ); 
	m_strDirOut = selectPath + '\\';

	assert( m_strDirOut != "" );

	UpdateData(FALSE);

}


//----------------------------------------------------------------
bool CvgReleaseAssistantDlg::distrubuteFiles()
{

	GroupNodeItemPtr	pRootNode;
	vgKernel::StringVector filters;

	
	// Step 1: ��ʼ��Ŀ¼�ṹ
	pRootNode = _versionsManager.getVersionByVersionName( String("all") );
	getDirectorySystemIn( pRootNode,
		String(m_strDirIn) );
	
	pRootNode->getPath() = String(m_strDirIn);

	// Step 2: ����release��debug
	VersionMap* pVersions	= _versionsManager.getVersionsPtr();
	FiltersMap* pFilters	= _versionsManager.getFiltersPtr();
	VersionMapItr itrVer =  ++pVersions->begin();
	FiltersMapItr itrFil =  ++pFilters->begin();
	
	for ( ;
		itrVer != pVersions->end(), itrFil != pFilters->end();
		itrVer ++, itrFil ++)
	{
		filterVersion( &itrVer->second, itrFil->second);
	}


	// Step 3: �����ļ�
	copyReleaseFiles();


	// Step 4: ת�������ʾ
	outputResultMsg();

	pRootNode->deleteAllData();
	return true;

}


//----------------------------------------------------------------
int CvgReleaseAssistantDlg::getDirectorySystemIn( GroupNodeItemPtr pGroupNode, String filePath,
												 int* nCountFile/* = NULL*/, int* nCountDir/* = NULL*/  )
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
			pTempParentNode->appendSubItem( String(wfd.cFileName) );			
		} 
	} while (FindNextFile(hFind, &wfd)); 

	FindClose(hFind); // �رղ��Ҿ�� 

	return true;
}

//----------------------------------------------------------------
bool CvgReleaseAssistantDlg::filterVersion( GroupNodeItemPtr pGroupNode,
												vgKernel::StringVector filters)
{
	// pRootNodeIn ���˵� pRootNodeOutD��pRootNodeOutR����Debug��ReleaseĿ¼
	
	GroupNodeItemPtr	pRootNodeIn	= _versionsManager.getVersionByVersionName( String("all") );
	GroupNodeItemPtr	pRootNodeOut= pGroupNode;

	*pRootNodeOut = *pRootNodeIn;

	GroupNodeManager::filterVersion( pRootNodeOut, filters, _folderNameMap );

	return true;
}

//----------------------------------------------------------------
bool CvgReleaseAssistantDlg::copyReleaseFiles()
{

	VersionMap* pVersions	= _versionsManager.getVersionsPtr();
	VersionMapItr itrVer =  ++pVersions->begin();
	for ( ;
		itrVer != pVersions->end();
		itrVer ++)
	{
		String outDir = String(m_strDirOut) + itrVer->first + '\\';

		vgKernel::SystemUtility::createDirectory( outDir );

		copyReleaseFiles( &itrVer->second, outDir);
	}


	return true;
}

//----------------------------------------------------------------
bool CvgReleaseAssistantDlg::copyReleaseFiles( GroupNodeItemPtr pGroupNode, String dir )
{
	String dirParent = dir;

	GroupNodeItemVec*	pSubItems = pGroupNode->getSubItems();
	GroupNodeItemVec::iterator itr = pSubItems->begin();
	for ( ; itr != pSubItems->end(); itr ++)
	{
		//����Ƿ�֧������ļ����Ƿ���Ҫ����,Ŀǰֻ���bin �µ����ļ���
		if ( itr->getType() == GROUP_NODE_BRANCH && itr->getParentNode()->getParentNode() == NULL )
		{
			if ( !_versionsManager.checkIsNeedFilter( &(*itr), _folderNameMap ) )
			{
				String dirSrc = itr->getParentNode()->getPath();
				dirSrc += itr->getName();

				String dirDes = dirParent + itr->getName();
				vgKernel::SystemUtility::copyDirectory( dirDes, dirSrc );

				continue;
			}

		}

		if (  itr->isLeafItem() )
		{
			String fileSrc = itr->getParentNode()->getPath();
			fileSrc += itr->getName();

			String fileDes = dirParent + itr->getName();

			vgKernel::SystemUtility::copyFile(fileSrc, fileDes);
		}
		else //if( itr->hasSubItem() ) 
		{
			String dirTemp = dirParent + itr->getName() + '\\';
			vgKernel::SystemUtility::createDirectory( dirTemp );
			copyReleaseFiles(&(*itr), dirTemp );
		}
	}

	return true;
}

//----------------------------------------------------------------
void CvgReleaseAssistantDlg::outputResultMsg()
{
	DWORD elpseTime = GetTickCount() - _oldTime;
	CString str;
	str.Format( "%u", elpseTime );

	std::ostringstream ostr;
	ostr << "�汾��������ɣ��ܹ����ɣ�";
	ostr << _versionsManager.getVersionsPtr()->size() - 1;
	ostr << "���汾��";
/*
	ostr << "\n";
	ostr << "��ʱ:";
	ostr << str;
	ostr << "����";*/

	AfxMessageBox( ostr.str().c_str() );

	
	
	
}

//----------------------------------------------------------------
bool CvgReleaseAssistantDlg::AddVersionAndFilter( string versionName, 
												 vgKernel::StringVector filterString )
{
	//����VersionMap
	VersionMap* pVersions = _versionsManager.getVersionsPtr();

	assert( pVersions != NULL );
	if ( !pVersions )
	{
		return false;
	}

	VersionMapItr itrVer =  pVersions->begin();
	pVersions->insert( VersionPair( versionName, itrVer->second ) );


	//����FiltersMap
	FiltersMap* pFilters = _versionsManager.getFiltersPtr();

	assert( pFilters != NULL );
	if ( !pFilters )
	{
		return false;
	}

	pFilters->insert( FiltersPair( versionName,	filterString ) );

	return true;
}

//----------------------------------------------------------------
bool CvgReleaseAssistantDlg::RemoveVersionAndFilter( string versionName )
{
	//�Ƴ�VersionMap
	VersionMap* pVersions = _versionsManager.getVersionsPtr();

	assert( pVersions != NULL );
	if ( !pVersions )
	{
		return false;
	}

	VersionMapItr itrVer = pVersions->find( versionName );
	if ( itrVer != pVersions->end() )
	{
		pVersions->erase( itrVer );
	}

	//�Ƴ�FiltersMap
	FiltersMap* pFilters = _versionsManager.getFiltersPtr();

	assert( pFilters != NULL );
	if ( !pFilters )
	{
		return false;
	}

	FiltersMapItr itrFil = pFilters->find( versionName );
	if ( itrFil != pFilters->end() )
	{
		pFilters->erase( itrFil );
	}
	
	return true;
}

//----------------------------------------------------------------
void CvgReleaseAssistantDlg::OnBnClickedCheckCommon()
{
	// TODO: Add your control notification handler code here
	static bool bCheck = false;
	bCheck = !bCheck;

	if ( bCheck )
	{
		vgKernel::StringVector filterString;
		filterString.clear();
		filterString.push_back( "debug" );
		filterString.push_back( "release" );

		this->AddVersionAndFilter( "common", filterString );
	}
	else
	{
		this->RemoveVersionAndFilter( "common" );
	}
}

//----------------------------------------------------------------
void CvgReleaseAssistantDlg::OnBnClickedCheckDebug()
{
	// TODO: Add your control notification handler code here
	static bool bCheck = false;
	bCheck = !bCheck;

	if ( bCheck )
	{
		vgKernel::StringVector filterString;
		filterString.clear();
		filterString.push_back( "release" );

		this->AddVersionAndFilter( "debug", filterString );
	}
	else
	{
		this->RemoveVersionAndFilter( "debug" );
	}
}

//----------------------------------------------------------------
void CvgReleaseAssistantDlg::OnBnClickedCheckRelease()
{
	// TODO: Add your control notification handler code here
	static bool bCheck = false;
	bCheck = !bCheck;

	if ( bCheck )
	{
		vgKernel::StringVector filterString;
		filterString.clear();
		filterString.push_back( "debug" );

		this->AddVersionAndFilter( "release", filterString );
	}
	else
	{
		this->RemoveVersionAndFilter( "release" );
	}
}

//----------------------------------------------------------------
void CvgReleaseAssistantDlg::OnBnClickedButtonAddflolder()
{
	// TODO: Add your control notification handler code here
	if ( m_folderName == "" )
	{
		return;
	}

	//���б���м�������
	CListBox* pListBox = ( ( CListBox* )GetDlgItem( IDC_LIST_SHOWFOLDER ) );
	pListBox->AddString( m_folderName );

	int index = pListBox->GetCount() - 1;

	_folderNameMap.insert( FolderNamePair( index, (String)m_folderName ) );

	//��ձ༭������
	SetDlgItemText( IDC_EDIT_FOLDERNAME, "" );


}

//----------------------------------------------------------------
void CvgReleaseAssistantDlg::OnEnChangeEditFoldername()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	UpdateData( true );
}

//----------------------------------------------------------------
void CvgReleaseAssistantDlg::OnBnClickedButtonRemoveflolder()
{
	// TODO: Add your control notification handler code here
	int index = ( ( CListBox* )GetDlgItem( IDC_LIST_SHOWFOLDER ) )->GetCurSel();
	if ( index < 0 )
	{
		return;
	}

	( ( CListBox* )GetDlgItem( IDC_LIST_SHOWFOLDER ) )->DeleteString( index );

	FolderNameMapItr iter = _folderNameMap.find( index );
	if ( iter != _folderNameMap.end() )
	{
		_folderNameMap.erase( iter );
	}

}

//----------------------------------------------------------------
CString CvgReleaseAssistantDlg::BrowseFolder( HWND hWnd, LPCTSTR lpTitle )
{

	char szPath[MAX_PATH]={0};
	BROWSEINFO m_bi;

	m_bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_STATUSTEXT;
	m_bi.hwndOwner = hWnd;
	m_bi.pidlRoot = NULL;
	m_bi.lpszTitle = lpTitle;
	m_bi.lpfn = NULL;
	m_bi.lParam = NULL;
	m_bi.pszDisplayName = szPath;

	LPITEMIDLIST pidl = ::SHBrowseForFolder( &m_bi );
	if ( pidl )
	{
		if( !::SHGetPathFromIDList ( pidl, szPath ) ) szPath[0]=0;

		IMalloc * pMalloc = NULL;
		if ( SUCCEEDED ( ::SHGetMalloc( &pMalloc ) ) ) // ȡ��IMalloc�������ӿ�
		{
			pMalloc->Free( pidl ); // �ͷ��ڴ�
			pMalloc->Release(); // �ͷŽӿ�
		}
	}
	return szPath;
}