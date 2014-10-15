// ClientAssistantDlg.cpp : ʵ���ļ�
//

#include "vgStableHeaders.h"
#include "vgChanAssistClient/vgChanAssistClient.h"
#include "vgChanAssistClient/vgChanAssistClientDlg.h"
#include <WinSock2.h>
#include <TlHelp32.h>


static int		chanRecvPort = 8695;	//���ն˿ں�8695
static SOCKET	chanRecvSocket;
static SOCKET	chanSendSocket;
static SOCKADDR_IN addrSnd;

static DWORD	chanThreadID;
static HANDLE	chanReceiveHandle;

static std::string m_broadCastAddress;
static std::string m_ownAddeess;

static DWORD WINAPI chanReceiveThread(LPVOID lpParameter);//������Ϣ�߳�
static void reriveFromServerSocket(long dataLength);	  //		
static void reStartComputer(bool bclose);				  // �رջ����������

static void vgkillProgress(const char* szExeName);		  // �رս���

static std::string m_ProgerssName;	//	��������
static CClientAssistantDlg *pDlgthis;

#define PROP_HAS_BUTTON	0x0002		//  CFileItem ����

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


/////////////////////////////////////////////////////////////////////////////
// CFileItem Class

CFileItem::CFileItem(const CString& strFileName) :
CBCGPGridItem (_variant_t((LPCTSTR) strFileName))
{
	m_dwFlags = PROP_HAS_BUTTON;
}
//****************************************************************************************
void CFileItem::OnClickButton (CPoint /*point*/)
{
	CBCGPGridCtrl* pGridCtrl = GetOwnerList();

	m_bButtonIsDown = TRUE;
	Redraw ();

	CString strPath = (LPCTSTR)(_bstr_t)m_varValue;

	CFileDialog dlg (TRUE,  strPath);

	if (dlg.DoModal () == IDOK)
	{
		strPath = dlg.GetPathName ();
		m_varValue = (LPCTSTR) strPath;
	}
	else
	{
		return;
	}

	m_bButtonIsDown = FALSE;
	Redraw ();

	if (m_pWndInPlace != NULL)
	{
		m_pWndInPlace->SetWindowText (strPath);
		m_pWndInPlace->SetFocus ();
	}
	else
	{
		pGridCtrl->SetFocus ();
	}
}
//////////////////////////////////////////////////////////////////////////

// CClientAssistantDlg �Ի���




CClientAssistantDlg::CClientAssistantDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CClientAssistantDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	is_visible = false;
	_FullScreen = NULL;
	_startChannel = NULL;
	_softBlend = NULL;
	pDlgthis = this;
}

void CClientAssistantDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STC_TXT, m_DlgGridLocation);
}

BEGIN_MESSAGE_MAP(CClientAssistantDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_WINDOWPOSCHANGING()
	ON_BN_CLICKED(IDCANCEL, &CClientAssistantDlg::OnBnClickedCancel)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDOK, &CClientAssistantDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BTN_HIDE, &CClientAssistantDlg::OnBnClickedBtnHide)
	ON_BN_CLICKED(IDC_BTN_ADDROW, &CClientAssistantDlg::OnBnClickedBtnAddrow)
	ON_BN_CLICKED(IDC_BEN_DELETEROW, &CClientAssistantDlg::OnBnClickedBenDeleterow)
END_MESSAGE_MAP()


// CClientAssistantDlg ��Ϣ�������

BOOL CClientAssistantDlg::OnInitDialog()
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
	// --------------------------------------------------------------
	CRect rectGrid;
	m_DlgGridLocation.GetClientRect (&rectGrid);

	m_DlgGridLocation.MapWindowPoints (this, &rectGrid);

	m_wndGrid.Create (WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER, rectGrid, this, (UINT)-1);
	m_wndGrid.EnableHeader (TRUE, BCGP_GRID_HEADER_MOVE_ITEMS);

	//m_wndGrid.SetReadOnly ();
	m_wndGrid.SetWholeRowSel(FALSE);
	m_wndGrid.EnableMarkSortedColumn(FALSE);
	m_wndGrid.EnableHeader(TRUE, 0);

	m_wndGrid.InsertColumn(0, _T("��������"), 65);
	m_wndGrid.InsertColumn(1, _T("VRGIS����·��"), 130);
	m_wndGrid.InsertColumn(2, _T("VG�ļ�·��"), 130);
	m_wndGrid.InsertColumn(3, _T("ȫ��"), 40);
	m_wndGrid.InsertColumn(4, _T("������ͨ��"), 70);
	m_wndGrid.InsertColumn(5, _T("���ں�"), 65);
	m_wndGrid.InsertColumn(6, _T("�ͻ���/������"), 90);
		
	UpdateData(FALSE);
	// --------------------------------------------------------------

	write2Reg();	// д��ע���������
	
	// ע���ȼ� Ctrl + Alt + 9 
	//::RegisterHotKey(this->m_hWnd, m_hotKeyID ,MOD_CONTROL|MOD_ALT,'9');
	BOOL isRegister = ::RegisterHotKey(this->m_hWnd, 0Xa006 ,MOD_CONTROL|MOD_ALT,'9');
	
	if ( !isRegister )
	{
		AfxMessageBox("ע���ȼ�ʧ��!");
	}

	startConnect();			// ��������

	//readConfigFile_ini();	// ��ȡini�����ļ�
	readConfigFile_XML();	// ��ȡXML�����ļ�
						
	// --------------------------------------------------------------
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CClientAssistantDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CClientAssistantDlg::OnPaint()
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
HCURSOR CClientAssistantDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//-------------------------------------------------------------------

void CClientAssistantDlg::OnWindowPosChanging(WINDOWPOS* lpwndpos)
{
	// �˺����������ش��� -FengYK @2010-03-10 08:59
	if(!is_visible)   
		lpwndpos->flags &= ~SWP_SHOWWINDOW;   

	CDialog::OnWindowPosChanging(lpwndpos);

	// TODO: �ڴ˴������Ϣ����������
}
//-------------------------------------------------------------------

void CClientAssistantDlg::readConfigFile_ini()
{
	char szModule[MAX_PATH] ;      
	::GetModuleFileName (NULL, szModule, MAX_PATH);

	string srcFolder(szModule);

	string::size_type found = srcFolder.find_last_of( "\\/" );
	string srcFilePath = srcFolder.substr( 0 , found );
	//srcFilePath += "\\ClientAssistant.ini";

	string strName1 = srcFolder.substr(found + 1, srcFolder.length() - found - 5);

	srcFilePath += "\\";
	srcFilePath += strName1;
	srcFilePath += ".ini";

	FILE *fp;

	fopen_s(&fp, srcFilePath.c_str(), "r");	// ��,�ļ�������ڡ�

	if (fp == NULL)
	{
		CString strWarning;
		strWarning.Format("%s.exe �������ļ�ʧ��!\n��ȷ��Ӧ�ó���Ŀ¼�´���%s�ļ�!", strName1.c_str(), srcFilePath.c_str());
		AfxMessageBox(strWarning);
		//return;
		OnCancel();	//�˳�����
	}
	else
	{
		char str[256];
		int cnt = 0;

		if ( fgets(str, 256, fp) != NULL ) 
		{
			CString strInFile(str);

			if ( strInFile.Right(1) == "\n")
			{
				strInFile.TrimRight("\n");
			}

			if ( strInFile == "1")
			{
				is_visible = true;
			}
			else if ( strInFile == "0")
			{
				is_visible = false;
			}
		}


		while ( fgets(str, 256, fp) != NULL)
		{
			CString strInFile(str);

			//int m_length = strInFile.GetLength();

			if ( strInFile.Right(1) == "\n")
			{
				strInFile.TrimRight("\n");
			}

			vec_str_config.push_back(strInFile);

		}

		fclose(fp);
	}

	m_AddConfig();
}
//-------------------------------------------------------------------

void CClientAssistantDlg::writeConfigFile_ini()
{
	char szModule[MAX_PATH] ;      
	::GetModuleFileName (NULL, szModule, MAX_PATH);

	string srcFolder(szModule);

	string::size_type found = srcFolder.find_last_of( "\\/" );
	string srcFilePath = srcFolder.substr( 0 , found );
	//srcFilePath += "\\ClientAssistant.ini";
	string strName1 = srcFolder.substr(found + 1, srcFolder.length() - found - 5);

	srcFilePath += "\\";
	srcFilePath += strName1;
	srcFilePath += ".ini";

	FILE *fp;

	fopen_s(&fp, srcFilePath.c_str(), "w");	// д��

	if (fp == NULL)
	{
		CString strWarning;
		strWarning.Format("%s.exe д�������ļ�%sʧ��!", strName1.c_str(), srcFilePath.c_str());
		AfxMessageBox(strWarning);
		return;
	}
	else
	{
		fputs("1\n", fp);	// �Ƿ����ؽ��� 1:������; 0:����

		for (int i=0; i<m_wndGrid.GetRowCount(); i++)
		{
			CBCGPGridRow *pRow = m_wndGrid.GetRow(i);

			CString sceneName(pRow->GetItem(0)->GetValue().bstrVal);
			sceneName.Append("\n");
			fputs( sceneName, fp );
			
			CString vrgis_exePath(pRow->GetItem(1)->GetValue().bstrVal);
			vrgis_exePath.Append("\n");
			fputs( vrgis_exePath, fp );
			
			CString vg_path(pRow->GetItem(2)->GetValue().bstrVal);
			vg_path.Append(_T(" "));
			fputs(vg_path, fp);
			
			//int b_fullScreen = _FullScreen->GetValue();
			//int b_startChan = _startChannel->GetValue();

			int b_fullScreen = pRow->GetItem(3)->GetValue();
			int b_startChan = pRow->GetItem(4)->GetValue();
			int b_sofrBlend = pRow->GetItem(5)->GetValue();

			if ( b_fullScreen == 0 )		// �Ƿ�ѡ��ȫ��
			{
				fputs(_T("-nfs "), fp);
			}
			else if ( b_fullScreen == -1 )
			{
				fputs(_T("-fs "), fp);
			}

			if ( b_startChan == 0 )			// �Ƿ�ѡ������ͨ��
			{
				fputs(_T("-nsc "), fp);
			}
			else if ( b_startChan == -1 )
			{
				fputs(_T("-sc "), fp);
			}

			if ( b_sofrBlend == 0 )			// �Ƿ�ѡ�������ں�
			{
				fputs(_T("-nbaa "), fp);
			}
			else if ( b_sofrBlend == -1 )
			{
				fputs(_T("-baa "), fp);
			}

			CString server_Type(pRow->GetItem(6)->GetValue().bstrVal);
			if ( server_Type == "�ͻ���" )	// ��������ͻ���
			{
				fputs(_T("-1\n"), fp);
			}
			else if ( server_Type == "������" )
			{
				fputs(_T("-0\n"), fp);
			}

		}
		
		fclose(fp);
	}		

}
//-------------------------------------------------------------------

void CClientAssistantDlg::write2Reg()
{
	HKEY hKey; 

	LPCTSTR data_Set="Software\\Microsoft\\Windows\\CurrentVersion\\Run";//����ע�������ص�·��,����б�� 

	long ret0=(::RegOpenKeyEx(HKEY_LOCAL_MACHINE,data_Set,0,KEY_WRITE,&hKey));//��ע����е���Ӧ�� 

	if(ret0!=ERROR_SUCCESS) 
	{ 
		MessageBox("��ע���ʧ��"); 
		return ;
	} 

	int i=0;
	char szModule[MAX_PATH] ;      

	::GetModuleFileName (NULL, szModule, MAX_PATH); //��ó���·�������ļ���

	CString title;
	title.Format("vgChannelAssist_Client");				//д��ע���ı���

	//int length=szModule.GetLength()+1;			//+1����Ϊlpb�Ƿ����˿ռ��Ժ�����ַ�д�����ģ������Ƕ���һ��CString���͵�lpb��strcpy���������ģ������轫�ַ��������ַ�Ҳд��ȥ��
	int length = sizeof(szModule) + 1;
	int len = length - 1;

	LPBYTE lpb=new BYTE[length]; 
	int j=0;

	for(i = 0;i<len;i++) 
	{
		lpb[j] = szModule[i];
		j++;

	}

	lpb[j] = 0; //дlpb�����һ���ַ���Ҳ�����ַ��������ַ�����Ҳ�����泤��+1��ԭ��

	long ret1=(::RegSetValueEx(hKey,title,NULL,REG_SZ,lpb,length/*cbData*/));//����ص���Ϣд��ע���

	if(ret1!=ERROR_SUCCESS) //�ж�ϵͳ�����ע���Ƿ�ɹ���
	{ 
		CString strWaring;
		strWaring.Format("д��ע���ʧ��");
		MessageBox(strWaring); 
		delete lpb; 
		::RegCloseKey(hKey);//�ر�ע����е���Ӧ���� 
		return ;
	}
}
//-------------------------------------------------------------------

void CClientAssistantDlg::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnCancel();
	// ��Ϊ�������޷������˳��Լ������⣬�����ֶ������Լ� - FengYK
	char char_SelfName[MAX_PATH] ;      
	::GetModuleFileName (NULL, char_SelfName, MAX_PATH); //��ó���·�������ļ���
	string strName(char_SelfName);
	string strName1;
	string::size_type found = strName.find_last_of( "\\/" );
	strName1 = strName.substr(found + 1);

	vgkillProgress(strName1.c_str());

}
//-------------------------------------------------------------------

BOOL CClientAssistantDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	switch(pMsg->message)
	{
	case WM_KEYDOWN:
// 		if ( pMsg->wParam == 'Q')
// 		{
// 			AfxMessageBox("Q��������!");
// 		}
	case WM_SYSKEYDOWN:	// ��Ӧ��ݼ�
// 		iResult = TranslateAccelerator(m_hWnd,hAcc,pMsg);
// 		//�����ݼ��ɹ�������TRUE
// 		if(iResult)
// 			return TRUE;
	case WM_HOTKEY:		// ��Ӧ�ȼ�
		if(pMsg->wParam == 0Xa006)  
			OnShowDlg();
	}

	return CDialog::PreTranslateMessage(pMsg);
}
//-------------------------------------------------------------------

void CClientAssistantDlg::OnShowDlg()
{
	if ( is_visible )
	{
		is_visible = false;
		this->ShowWindow(SW_HIDE);
	}
	else
	{
		is_visible = true;
		this->ShowWindow(SW_SHOW);
	}

}
//-------------------------------------------------------------------

void CClientAssistantDlg::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: �ڴ˴������Ϣ����������
	UnregisterHotKey(this->m_hWnd, 0Xa006);

	WSACleanup();

	if(chanReceiveHandle != NULL)
	{
		WaitForSingleObject(chanReceiveHandle,INFINITE);
		chanReceiveHandle = NULL;
		chanThreadID = 0;
	}

	closesocket(chanRecvSocket);

	if ( pDlgthis != NULL )
	{
		delete pDlgthis;
		pDlgthis = NULL;
	}
	//delete this;
}
//-------------------------------------------------------------------

BOOL CClientAssistantDlg::startConnect()
{
	WSADATA wsd;

	int ret = WSAStartup(0x0202,&wsd);
	if(ret!=0)
	{
		WSACleanup();
	}

	//��ù㲥��IP----------------------------------------------------------
	char hostName[255];   
	PHOSTENT hostinfo;

	if(gethostname(hostName, sizeof(hostName))   ==   0)   
	{   
		if((hostinfo = gethostbyname(hostName)) != NULL)   
		{   
			m_broadCastAddress = inet_ntoa(*(struct in_addr *)*hostinfo->h_addr_list);   
			m_ownAddeess = m_broadCastAddress;
		}

		if (m_broadCastAddress[m_broadCastAddress.length()-2] == '.')//IP��ַ��������1λ
		{
			m_broadCastAddress.erase(m_broadCastAddress.length()-1);
			m_broadCastAddress.append("255",m_broadCastAddress.length()-1);;
		}
		else if (m_broadCastAddress[m_broadCastAddress.length()-3] == '.')//IP��ַ��������λ
		{
			m_broadCastAddress.erase(m_broadCastAddress.length()-2);
			m_broadCastAddress.append("255",m_broadCastAddress.length()-1);;
		}
		else
		{
			m_broadCastAddress[m_broadCastAddress.length()-1] = '5';
			m_broadCastAddress[m_broadCastAddress.length()-2] = '5';
			m_broadCastAddress[m_broadCastAddress.length()-3] = '2';
		}
	}
	//------------------------------------------------------------------------
	if(! createRecvSocket(chanRecvPort))
	{
		AfxMessageBox("�˿ڴ�������.");
		return FALSE;
	}

	chanReceiveHandle = 
		CreateThread(NULL, 0, chanReceiveThread, this, 0, &chanThreadID);

	SetThreadPriority( chanReceiveHandle, THREAD_PRIORITY_ABOVE_NORMAL);

	if( chanReceiveHandle == NULL )
	{
		return FALSE;
	}

	return TRUE;
}
//-------------------------------------------------------------------

BOOL CClientAssistantDlg::createRecvSocket(int RecvPort)
{
	int ret;

	// ����Server����Socket
	chanRecvSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP); 

	if(chanRecvSocket == INVALID_SOCKET)
	{
		return FALSE;
	}

	SOCKADDR_IN addr;
	ZeroMemory(&addr, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(RecvPort);

	// �󶨽���ʹ�õĶ˿ں͵�ַ
	ret = bind(chanRecvSocket, (struct sockaddr *)&addr, sizeof(addr));

	if(ret != 0)
	{
		return FALSE;
	}

	// ��������Socket
	chanSendSocket = socket(AF_INET,SOCK_DGRAM,IPPROTO_IP); 
	if(chanSendSocket == INVALID_SOCKET)
	{
		return 0;
	}

	//�󶨷���
	ZeroMemory(&addrSnd,sizeof(addrSnd));
	addrSnd.sin_family = AF_INET;
	addrSnd.sin_addr.s_addr = INADDR_ANY;

	ret = bind(chanSendSocket, (struct sockaddr *)&addrSnd, sizeof(addrSnd));

	addrSnd.sin_addr.s_addr = inet_addr(m_broadCastAddress.c_str());

	if(ret != 0)
	{
		return 0;
	}

	return TRUE;

}
//-------------------------------------------------------------------

DWORD WINAPI chanReceiveThread(LPVOID lpParameter)
{
	int ret;
	unsigned long lLength;

	while( TRUE )
	{
		ret = ioctlsocket(chanRecvSocket, FIONREAD, &lLength);
		if(ret != 0)
		{
			break;
		}
		else
		{
			if(lLength>0)
			{
				reriveFromServerSocket(lLength);
			}
		}

		Sleep(10);
	}

	return 0;

}
//-------------------------------------------------------------------

void reriveFromServerSocket(long dataLength)
{
	char *pbuf;
	int addr_len, iRecLen;;
	struct sockaddr_in addr;

	pbuf = new char[dataLength + 1];
	memset(pbuf, 0, dataLength + 1);

	addr_len = sizeof(addr);

	iRecLen = recvfrom(chanRecvSocket, pbuf, dataLength, 0,(struct sockaddr *)&addr, &addr_len);

	CString recvStr(pbuf);	//���յ�����Ϣ

	if( iRecLen <= 0 )
	{
		return;
	}
	else if ( recvStr.Left(17) == "VRGISstartChannel" )
	{

		SHELLEXECUTEINFO  ShExecInfo = {0};
		string strExeFilePath;
		string strvgPath;
		CString tempStr;

		if ( recvStr.GetLength() > 17)
		{
			tempStr = recvStr.Mid(17);
		}

		//////////////////////////////////////////////////////////////////////////
		// ��Ϊ��ʼ��static �ĺ����в��ܵ���CClientAssistantDlg::m_wndGrid 
		// ����û��ʹ�����ְ취;ʹ�õĶ�ȡ�����ļ��ķ���
		// ����ʹ��ָ��ķ�����;
		// �ڿ�ͷ������һ�� static CClientAssistantDlg *pDlg;
		// ��CClientAssistantDlg �Ĺ��캯���ж�pDlg ���и�ֵ��
		// ��CClientAssistantDlg ��Destory �����н�������

		for ( int i = 0; i < pDlgthis->m_wndGrid.GetRowCount(); i++ )
		{
			CBCGPGridRow *pRow = pDlgthis->m_wndGrid.GetRow(i);

			if ( pRow->GetItem(0)->GetValue().bstrVal == tempStr )
			{
				strExeFilePath = (_bstr_t)(pRow->GetItem(1)->GetValue().bstrVal);
				strvgPath = (_bstr_t)(pRow->GetItem(2)->GetValue().bstrVal);

				int b_fullScreen = pRow->GetItem(3)->GetValue();
				int b_startChan = pRow->GetItem(4)->GetValue();
				int b_softBlend = pRow->GetItem(5)->GetValue();

				CString strFullScreen;
				CString strStartChan;
				CString strSoftBlend;			// �������ں�

				if ( b_fullScreen == 0 )		// �Ƿ�ѡ��ȫ��
				{
					strFullScreen.Format("%s", _T(" -nfs"));
				}
				else if ( b_fullScreen == -1 )
				{
					strFullScreen.Format("%s", _T(" -fs"));
				}

				if ( b_startChan == 0 )			// �Ƿ�ѡ������ͨ��
				{
					strStartChan.Format("%s", _T(" -nsc"));
				}
				else if ( b_startChan == -1 )
				{
					strStartChan.Format("%s", _T(" -sc"));
				}

				if ( b_softBlend == 0 )			// �Ƿ�ѡ�������ں�
				{
					strSoftBlend.Format("%s", _T(" -nbaa"));
				}
				else if ( b_softBlend == -1 )
				{
					strSoftBlend.Format("%s", _T(" -baa"));
				}

				strvgPath += strFullScreen.GetBuffer();
				strFullScreen.ReleaseBuffer();

				strvgPath += strStartChan.GetBuffer();
				strStartChan.ReleaseBuffer();

				strvgPath += strSoftBlend.GetBuffer();
				strSoftBlend.ReleaseBuffer();

				CString strServType(pRow->GetItem(6)->GetValue().bstrVal);

				if ( strServType == "�ͻ���" )	// ��������ͻ���
				{
					strvgPath += " -1";
				}
				else if ( strServType == "������" )
				{
					strvgPath += " -0";;
				}
				
				m_ProgerssName = strExeFilePath;
			}
		}

		if ( strExeFilePath == "" || strvgPath == "" )
		{
			return;
		}
		//////////////////////////////////////////////////////////////////////////

#if 0
		// ---------------------------------------------------------------
		// �յ���Ϣ���ȡ XML �����ļ���ʽ
		char szModule[MAX_PATH] ;      
		::GetModuleFileName (NULL, szModule, MAX_PATH);

		string srcFolder(szModule);

		string::size_type found = srcFolder.find_last_of( "\\/" );
		string srcFilePath = srcFolder.substr( 0 , found );
		//srcFilePath += "\\ClientAssistant.ini";

		string strName1 = srcFolder.substr(found + 1, srcFolder.length() - found - 5);

		CString configFilePath;

		configFilePath.Format( "%s\\%s.xml", srcFilePath.c_str(), strName1.c_str() );

		XMLConfigManager *pXMLConfig = new XMLConfigManager();

		pXMLConfig->LoadXmlDocument( configFilePath );

		// ������Ʋ���
		if ( pXMLConfig->FindSection( tempStr ) )
		{
			SectionData sceneInfo = pXMLConfig->GetCurrentValue( tempStr );

			CString vrgis_exePath = sceneInfo._Currentkeyvalue[0];
			CString vg_path = sceneInfo._Currentkeyvalue[1];
			CString strFullScreen = sceneInfo._Currentkeyvalue[2];
			CString strStartChan = sceneInfo._Currentkeyvalue[3];
			CString strSoftBlend = sceneInfo._Currentkeyvalue[4];
			CString strServType = sceneInfo._Currentkeyvalue[5];

			strExeFilePath = vrgis_exePath.GetBuffer();
			vrgis_exePath.ReleaseBuffer();
			m_ProgerssName = strExeFilePath;
			
			strvgPath = vg_path.GetBuffer();
			vg_path.ReleaseBuffer();

			strvgPath += _T(" ");
			strvgPath += strFullScreen.GetBuffer();
			strFullScreen.ReleaseBuffer();
			
			strvgPath += _T(" ");
			strvgPath += strStartChan.GetBuffer();
			strStartChan.ReleaseBuffer();

			strvgPath += _T(" ");
			strvgPath += strSoftBlend.GetBuffer();
			strSoftBlend.ReleaseBuffer();

			strvgPath += _T(" ");

			if ( strServType == "�ͻ���" )
			{
				strvgPath += _T("-1");
			}
			else if ( strServType == "������" )
			{
				strvgPath += _T("-0");
			}

		}
		else
		{
			CString s_sceneName;
			s_sceneName.Format(_T("û���ҵ�����%s,��鿴����!"), tempStr );
			AfxMessageBox(s_sceneName);
		}

		// ---------------------------------------------------------------
#endif
	
#if 0
		// ---------------------------------------------------------------
		// �յ���Ϣ���ȡ ini �����ļ���ʽ

		char szModule[MAX_PATH] ;      
		::GetModuleFileName (NULL, szModule, MAX_PATH);

		string srcFolder(szModule);

		string::size_type found = srcFolder.find_last_of( "\\/" );
		string srcFilePath = srcFolder.substr( 0 , found );
		//srcFilePath += "\\ClientAssistant.ini";

		string strName1 = srcFolder.substr(found + 1, srcFolder.length() - found - 5);

		srcFilePath += "\\";
		srcFilePath += strName1;
		srcFilePath += ".ini";

		FILE *fp;

		fopen_s(&fp, srcFilePath.c_str(), "r");	// ��,�ļ�������ڡ�

		if (fp == NULL)
		{
			CString strWarning;
			strWarning.Format("%s.exe ��ȡ�����ļ�ʧ��!\n��ȷ��Ӧ�ó���Ŀ¼�´���%s�ļ�!", strName1.c_str(), srcFilePath.c_str());
			AfxMessageBox(strWarning);
			return;
		}
		else
		{
			char str[256];
			int cnt = 0;

			fgets(str, 256, fp);

				while ( fgets(str, 256, fp) != NULL )
				{
					CString strInFile(str);

					if ( strInFile.Right(1) == "\n")
					{
						strInFile.TrimRight("\n");
					}

					if ( strInFile == tempStr )
					{
						if ( fgets(str, 256, fp) != NULL )
						{
							CString exePath(str);

							if ( exePath.Right(1) == "\n")
							{
								exePath.TrimRight("\n");
							}

							strExeFilePath = exePath.GetBuffer();
							exePath.ReleaseBuffer();

							m_ProgerssName = strExeFilePath;
						}

						if ( fgets(str, 256, fp) != NULL )
						{
							CString vgPath(str);

							if ( vgPath.Right(1) == "\n")
							{
								vgPath.TrimRight("\n");
							}

							strvgPath = vgPath.GetBuffer();
							vgPath.ReleaseBuffer();
						}

						//return;
					}

				}

				fclose(fp);
		}
		// ---------------------------------------------------------------
#endif


#if 0
		// ---------------------------------------------------------------
		// ԭ��д�������ļ��ķ�ʽ
		char szModule[MAX_PATH] ;      
		::GetModuleFileName (NULL, szModule, MAX_PATH);

		string srcFolder(szModule);



		string::size_type found = srcFolder.find_last_of( "\\/" );
		string srcFilePath = srcFolder.substr( 0 , found );
		//srcFilePath += "\\vgChannelAssistClient.ini";
		if ( recvStr.GetLength() > 17)
		{
			CString tempStr = recvStr.Mid(17);
			srcFilePath += "\\vgChannelAssistClient";
			srcFilePath += tempStr.GetBuffer();
			tempStr.ReleaseBuffer();
			srcFilePath += ".ini";
		}
		else
		{
			srcFilePath += "\\vgChannelAssistClient.ini";
		}

		FILE *fp;

		//fp = fopen(srcFilePath.c_str(), "r");	//ʹ��fopen����ʱ���治��ȫ
		fopen_s(&fp, srcFilePath.c_str(), "r");

		if (fp == NULL)
		{
			CString strWarning;
			strWarning.Format("ChannelAssistClient.exe �������ļ�ʧ�ܣ�\n��ȷ��%s �ļ�����", srcFilePath.c_str());
			AfxMessageBox(strWarning);
			return;
		}
		else
		{
			fseek(fp, 0, SEEK_END);
			int iEnd = ftell(fp);
			fseek(fp, 0, SEEK_SET);
			int iStart = ftell(fp);

			int m_uiFileLen = iEnd - iStart;

			char * ucpBuffer = NULL;
			ucpBuffer = new char[m_uiFileLen];

			size_t numofRead = fread(ucpBuffer, 1, m_uiFileLen, fp);

			string strInFile(ucpBuffer);

			size_t charcaterNum = strInFile.find_first_of("\n", 1);

			strExeFilePath = strInFile.substr(0,charcaterNum);

			m_ProgerssName = strExeFilePath;

			strvgPath = strInFile;

			strvgPath.erase( 0, charcaterNum+1 );

			if (strvgPath.length() > (numofRead - charcaterNum))
			{
				strvgPath.erase(numofRead - charcaterNum);
			}

			delete ucpBuffer;
			ucpBuffer = NULL;

			fclose(fp);
		}

		// ---------------------------------------------------------------------
#endif
		

		ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
		ShExecInfo.fMask     = NULL;
		ShExecInfo.hwnd      = NULL;
		ShExecInfo.lpVerb    = NULL;
		//ShExecInfo.lpVerb	 = _T("open");

		//�������ļ��ж�ȡִ���ļ��ľ���·��,������ .exeҲ����ʹ .vg 
		ShExecInfo.lpFile	 = strExeFilePath.c_str();		
		ShExecInfo.lpParameters =  strvgPath.c_str();	//��VRGIS���ݵĲ���(vg��·�����Ƿ����ͨ����ȫ�������Զ�ȡ���ļ�)

		ShExecInfo.lpDirectory  = NULL;

		ShExecInfo.nShow        = SW_SHOWMAXIMIZED;  // �����ʾ�������
		ShExecInfo.hInstApp = NULL;
		ShExecInfo.hProcess = NULL;

		//string::size_type found2 = strParamaInFile.find_last_of( "\\/" );
		//strvgConfigPath = strParamaInFile.substr( 0 , found2 );
		//strvgConfigPath += "\\data.ini";

		//fstream Write_to_File(strvgConfigPath.c_str(), ios_base::out);	//����fstream ������ȷ��ȡ����·���µ��ļ�
		//Write_to_File << strvgPath;
		//Write_to_File.close();

		//��������Ѿ��򿪾��˳�
		if( FindWindow(NULL,"VR-GIS��ҵ�� Platform 2.0")) return;

		ShellExecuteEx(&ShExecInfo);

		DWORD waitStatus = WaitForSingleObject(ShExecInfo.hProcess,INFINITE);

		if (WAIT_ABANDONED)
		{
			CString strSend;
			strSend.Format("IP��ַΪ:%s�Ŀͻ����յ�������Ϣ", m_ownAddeess.c_str());

			addrSnd.sin_port = htons(8694);

			sendto(chanRecvSocket,strSend,strSend.GetLength()+1,0,
				(SOCKADDR*)&addrSnd,sizeof(SOCKADDR));
		}
		//fstream Write_File_Null(strvgConfigPath.c_str(), ios_base::out);
		//Write_File_Null << endl;
		//Write_File_Null.close();

	}
	else if (recvStr == "VRGISendChannel")
	{
		//FindWindow�������������԰����㶨λһ������.��̬���
		HWND hWnd = ::FindWindow(NULL, _T("VR-GIS��ҵ�� Platform 2.0"));
		if (NULL != hWnd) 
		{
			::SendMessage(hWnd, WM_CLOSE, 0, 0);
		}

		hWnd = ::FindWindow(NULL, _T("VR-GIS Platform 2.0"));
		if ( NULL != hWnd)
		{
			::SendMessage(hWnd, WM_CLOSE, 0, 0);
		}

		if ( !m_ProgerssName.empty())
		{
			string::size_type found = m_ProgerssName.find_last_of( "\\/" );
			string srcFilePath = m_ProgerssName.substr(found + 1);

			vgkillProgress(srcFilePath.c_str());	//��������
		}

	}
	else if ( recvStr == "vrgisReStartComputer" )	
	{
		reStartComputer(false);
	}
	else if (recvStr == "vrgisEndComputer")
	{
		reStartComputer(true);
	}

	delete [] pbuf;
	pbuf = NULL;
}
//-------------------------------------------------------------------

void reStartComputer(bool bclose)	// �رջ����������
{
	OSVERSIONINFO OsVerInfo;		//����ϵͳ�汾��Ϣ�����ݽṹ 
	OsVerInfo.dwOSVersionInfoSize=sizeof(OSVERSIONINFO); 
	GetVersionEx(&OsVerInfo);		//ȡ��ϵͳ�İ汾��Ϣ 
	if(OsVerInfo.dwPlatformId == VER_PLATFORM_WIN32_NT) 
	{ 
		TOKEN_PRIVILEGES tkp; 
		HANDLE hToken; 

		if (!OpenProcessToken(GetCurrentProcess(),TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))  
		{ 
			AfxMessageBox("�򿪽��̱�ʶʧ�ܣ�");
		} 

		LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME,&tkp.Privileges[0].Luid); //��ñ��ػ�Ψһ�ı�ʶ 
		tkp.PrivilegeCount = 1;   
		tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;  
		AdjustTokenPrivileges(hToken, FALSE, &tkp, 0,(PTOKEN_PRIVILEGES) NULL, 0); //������õ�Ȩ�� 

		if (GetLastError() != ERROR_SUCCESS)  
		{ 
			AfxMessageBox("�򿪽��̱�ʶʧ�ܣ�");
		} 
		BOOL fResult = FALSE;
		if (bclose)									//�ػ�
		{
			fResult =InitiateSystemShutdown(  
				NULL,								// Ҫ�صļ�����û���,���ھ��������йص��Է��Ļ�����NULL��ʾ�ر��� 
				"WINDOWS��������ʱ���ڹرգ������ñ��湤��!",  // ��ʾ����Ϣ 
				60,									// �ػ������ʱ�� 
				TRUE,                                  
				FALSE);								//��ΪTRUEΪ������ΪFALSEΪ�ػ� 

		}
		else										//����
		{
			fResult =InitiateSystemShutdown(  
				NULL,
				"WINDOWS��������ʱ�������������������ñ��湤��!",
				60,
				TRUE,
				TRUE);								
		}

		if(!fResult)  
		{  
			AfxMessageBox("�򿪽��̱�ʶʧ�ܣ�");
		}  

		tkp.Privileges[0].Attributes = 0;  
		AdjustTokenPrivileges(hToken, FALSE, &tkp, 0,(PTOKEN_PRIVILEGES) NULL, 0);  

		if (GetLastError() != ERROR_SUCCESS)  
		{ 
			AfxMessageBox("�ػ�Ȩ�޻�ȡʧ��!");
		} 

		if (bclose)
		{
			ExitWindowsEx(EWX_SHUTDOWN, 0);
		}
		else
		{
			ExitWindowsEx(EWX_REBOOT, 0); 
		}
		//��ʼ����������ڱ����������У�ֻ����һ�����Ϳ���ʵ�ּ��������������ʹ��Debug��Release�ļ����е�exeȴ�����������������
		//��Debug��Release�ļ����е�exeҪ����������Ļ���Ҫif�е����д��롣��ΪVC++����������ߵ��߳�ӵ�йػ�Ȩ�ޣ���������ΪVC�������ߵ�
		//�ӽ��̼̳��˹ػ�Ȩ�ޣ����Կ���ʵ�ֹػ�������������������Debug��Release�ļ����е�exe������Ϊ�������̣߳�û�йػ�Ȩ�ޣ�������Ҫ��if���Ĵ�������ùػ�Ȩ�ޡ�
	} 
}
//-------------------------------------------------------------------

void vgkillProgress(const char* szExeName)		// ��������
{
	if (szExeName == NULL) 
		return; 

	int lsr = lstrlen(szExeName); 

	if (lsr == 0) 
		return; 

	PROCESSENTRY32 m_Progress; 

	HANDLE m_handle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0); 

	if (((int)m_handle) != -1) 
	{ 
		m_Progress.dwSize = sizeof(m_Progress); 

		if (Process32First(m_handle, &m_Progress)) 
		{ 
			do 
			{ 
				if (lstrcmpi(szExeName, m_Progress.szExeFile + lstrlen(m_Progress.szExeFile) - lsr) == 0) 
				{ 
					HANDLE hProcessHandle; 
					hProcessHandle = OpenProcess(0x1F0FFF, true, m_Progress.th32ProcessID); 

					if (hProcessHandle != NULL) 
					{ 
						if (TerminateProcess(hProcessHandle, 0) != NULL) 
						{ 
							CloseHandle(hProcessHandle); 
						} 
					} 
					//CloseHandle(m_handle); 
					break; 
				} 

			}while ( Process32Next(m_handle, &m_Progress) ); 
		} 

		CloseHandle(m_handle); 
	} 

}
//-------------------------------------------------------------------

void CClientAssistantDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//writeConfigFile_ini();
	writeConfigFile_XML();
	//OnOK();
	// Edit By FengYK @2010-04-03 14:32 ��������ʱ�������ش���
	//is_visible = false;
	//this->ShowWindow(SW_HIDE);
}
//-------------------------------------------------------------------

void CClientAssistantDlg::OnBnClickedBtnHide()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	is_visible = false;
	this->ShowWindow(SW_HIDE);
}
//-------------------------------------------------------------------

void CClientAssistantDlg::m_AddRow()
{
	CBCGPGridRow* pRow = m_wndGrid.CreateRow(m_wndGrid.GetColumnCount ());

	for (int i = 0; i < m_wndGrid.GetColumnCount();i++)
	{
		pRow->GetItem (i)->AllowEdit (TRUE);
	}

	pRow->GetItem (0)->SetValue (_T("��������:"));

	pRow->ReplaceItem (1, new CFileItem (_T("VRGIS Platform·��:")));
	pRow->ReplaceItem (2, new CFileItem (_T("VG�ļ�·��:")));

	_FullScreen = new CBCGPGridCheckItem (TRUE);
	_startChannel = new CBCGPGridCheckItem (TRUE);
	_softBlend = new CBCGPGridCheckItem (TRUE);		// �Ƿ������ں� 
	pRow->ReplaceItem (3, _FullScreen);
	pRow->ReplaceItem (4, _startChannel);
	pRow->ReplaceItem(5, _softBlend);

	CBCGPGridItem* pItem;
	pItem = new CBCGPGridItem (_T("�ͻ���"));
	pItem->AddOption (_T("�ͻ���"), TRUE);
	pItem->AddOption(_T("������"), TRUE);
	pRow->ReplaceItem (6, pItem);

	m_wndGrid.AddRow(pRow, TRUE);
	UpdateData(FALSE);

}
void CClientAssistantDlg::OnBnClickedBtnAddrow()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_AddRow();
}
//-------------------------------------------------------------------
// ԭ��д�Ķ�ȡini �����ļ���ʱ��Ҫ�õ��ģ�����û��ʹ��
void CClientAssistantDlg::m_AddConfig()
{
	if ( !vec_str_config.empty())
	{
		vector<CString>::iterator iterBegin = vec_str_config.begin();
		vector<CString>::iterator iterEnd = vec_str_config.end();

		for ( iterBegin; iterBegin != iterEnd; iterBegin++ )
		{
			if ( iterBegin == iterEnd - 2)
			{
				return;
			}

			CString sceneName = (*iterBegin);
			CString exePath = (*(iterBegin + 1));
			CString str_cmd = (*(iterBegin + 2 ));
			
			CBCGPGridRow* pRow = m_wndGrid.CreateRow(m_wndGrid.GetColumnCount ());

			for (int i = 0; i < m_wndGrid.GetColumnCount();i++)
			{
				pRow->GetItem (i)->AllowEdit (TRUE);
			}
			
			// ��������
			pRow->GetItem(0)->SetValue(sceneName.AllocSysString(), TRUE);
			
			//	VRGISPlatform ·��
			pRow->ReplaceItem (1, new CFileItem (exePath));

			//	VG·��
			int firstNull = str_cmd.Find( _T(" "), 0);
			CString vgPath = str_cmd.Left( firstNull );
			pRow->ReplaceItem (2, new CFileItem (vgPath));

			//	ȫ��
			int secdNull = str_cmd.Find( _T(" "), firstNull+1);
			CString str_fullscreen = str_cmd.Mid( firstNull+1, secdNull - firstNull - 1 );
			if ( str_fullscreen == "-fs" )
			{
				_FullScreen = new CBCGPGridCheckItem (TRUE);
				pRow->ReplaceItem (3, _FullScreen);
			}
			else if ( str_fullscreen == "-nfs" )
			{
				_FullScreen = new CBCGPGridCheckItem (FALSE);
				pRow->ReplaceItem (3, _FullScreen);
			}

			// ������ͨ��
			int thirdNull = str_cmd.Find( _T(" "), secdNull+1);
			CString str_startchan = str_cmd.Mid( secdNull+1, thirdNull - secdNull - 1 );
			if ( str_startchan == "-sc" )
			{
				_startChannel = new CBCGPGridCheckItem (TRUE);
				pRow->ReplaceItem (4, _startChannel);
			}
			else if ( str_startchan == "-nsc" )
			{
				_startChannel = new CBCGPGridCheckItem (FALSE);
				pRow->ReplaceItem (4, _startChannel);
			}

			// �������ں�(��Ե�ںϺͼ���У��)blend and adjust
			int fourthNull = str_cmd.Find( _T(" "), thirdNull+1);
			CString str_softBlend = str_cmd.Mid( thirdNull+1, fourthNull - thirdNull - 1 );
			if ( str_softBlend == "-baa" )	// blend and adjust
			{
				_softBlend = new CBCGPGridCheckItem (TRUE);
				pRow->ReplaceItem (5, _softBlend);
			}
			else if ( str_startchan == "-nbaa" )
			{
				_softBlend = new CBCGPGridCheckItem (FALSE);
				pRow->ReplaceItem (5, _softBlend);
			}

			// �ͻ��˻������
			CString str_servType = str_cmd.Mid( thirdNull+1, str_cmd.GetLength() - thirdNull );
			if ( str_servType == "-1")
			{
				CBCGPGridItem* pItem;
				pItem = new CBCGPGridItem (_T("�ͻ���"));
				pItem->AddOption (_T("�ͻ���"), TRUE);
				pItem->AddOption(_T("������"), TRUE);
				pRow->ReplaceItem (6, pItem);
			}
			else if ( str_servType == "-0")
			{
				CBCGPGridItem* pItem;
				pItem = new CBCGPGridItem (_T("������"));
				pItem->AddOption (_T("�ͻ���"), TRUE);
				pItem->AddOption(_T("������"), TRUE);
				pRow->ReplaceItem (6, pItem);
			}
			
			m_wndGrid.AddRow(pRow, TRUE);

			UpdateData( FALSE );

			iterBegin++;
			iterBegin++;
		}

	}
}
//-------------------------------------------------------------------
void CClientAssistantDlg::readConfigFile_XML()
{
	char szModule[MAX_PATH] ;      
	::GetModuleFileName (NULL, szModule, MAX_PATH);

	string srcFolder(szModule);

	string::size_type found = srcFolder.find_last_of( "\\/" );
	string srcFilePath = srcFolder.substr( 0 , found );
	string strName1 = srcFolder.substr(found + 1, srcFolder.length() - found - 5);

	CString configFilePath;
	int sceneCount = 0;

	configFilePath.Format( "%s\\%s.xml", srcFilePath.c_str(), strName1.c_str() );

	bool isreadok = vgConfig::XMLConfigManager::getSingleton().LoadXmlDocument( configFilePath );
	
	if ( !isreadok )
	{
		CString warning;
		warning.Format("��ȡ�����ļ�%sʧ��!\n�����ļ��Ƿ����!", configFilePath);
		AfxMessageBox(warning);
		return;
	}

	// ������Ʋ���
	if ( vgConfig::XMLConfigManager::getSingleton().FindSection( _T("Interface")) )
	{
		vgConfig::SectionData secInface = vgConfig::XMLConfigManager::getSingleton().GetCurrentValue(_T("Interface"));

		int show_hide = vgConfig::XMLConfigManager::getSingleton().GetIntValue( _T("Interface"), _T("Interface") );
		
		if ( show_hide == 1 )
		{
			is_visible = true;
		}
		else if ( show_hide == 0 )
		{
			is_visible = false;
		}
	}

	// �볡���йصĲ���
	if ( vgConfig::XMLConfigManager::getSingleton().FindSection( _T("SceneInfo") ) )
	{
		vgConfig::SectionData scene = vgConfig::XMLConfigManager::getSingleton().GetCurrentValue(_T("SceneInfo"));

		sceneCount = vgConfig::XMLConfigManager::getSingleton().GetIntValue( _T("SceneInfo"), _T("SceneCount") );
		
		for ( int i = 0; i < sceneCount; i++ )
		{
			CString str_sceneName;

			vgConfig::SectionData tempSecData = vgConfig::XMLConfigManager::getSingleton().GetCurrentValue( _T("SceneInfo") );
			
			str_sceneName = tempSecData._Currentkeyvalue[i+1];

			if ( vgConfig::XMLConfigManager::getSingleton().FindSection( str_sceneName ) )
			{
				vgConfig::SectionData sceneInfo = vgConfig::XMLConfigManager::getSingleton().GetCurrentValue( str_sceneName );

				CString vrgis_exePath = sceneInfo._Currentkeyvalue[0];
				CString vg_path = sceneInfo._Currentkeyvalue[1];
				CString strFullScreen = sceneInfo._Currentkeyvalue[2];
				CString strStartChan = sceneInfo._Currentkeyvalue[3];
				CString strSoftBlend = sceneInfo._Currentkeyvalue[4];
				CString strServType = sceneInfo._Currentkeyvalue[5];

				CBCGPGridRow* pRow = m_wndGrid.CreateRow(m_wndGrid.GetColumnCount ());

				for (int i = 0; i < m_wndGrid.GetColumnCount();i++)
				{
					pRow->GetItem (i)->AllowEdit (TRUE);
				}
				
				// ��������
				pRow->GetItem(0)->SetValue(str_sceneName.AllocSysString(), TRUE);

				//	VRGISPlatform ·��
				pRow->ReplaceItem (1, new CFileItem (vrgis_exePath));

				//	VG·��			
				pRow->ReplaceItem (2, new CFileItem (vg_path));

				//	ȫ��
				if ( strFullScreen == "-fs" )
				{
					_FullScreen = new CBCGPGridCheckItem (TRUE);
					pRow->ReplaceItem (3, _FullScreen);
				}
				else if ( strFullScreen == "-nfs" )
				{
					_FullScreen = new CBCGPGridCheckItem (FALSE);
					pRow->ReplaceItem (3, _FullScreen);
				}

				// ������ͨ��
				if ( strStartChan == "-sc" )
				{
					_startChannel = new CBCGPGridCheckItem (TRUE);
					pRow->ReplaceItem (4, _startChannel);
				}
				else if ( strStartChan == "-nsc" )
				{
					_startChannel = new CBCGPGridCheckItem (FALSE);
					pRow->ReplaceItem (4, _startChannel);
				}

				//	������Ե�ںϺͼ���У��
				if ( strSoftBlend == "-baa" )
				{
					_softBlend = new CBCGPGridCheckItem (TRUE);
					pRow->ReplaceItem (5, _softBlend);
				}
				else if ( strSoftBlend == "-nbaa" )
				{
					_softBlend = new CBCGPGridCheckItem (FALSE);
					pRow->ReplaceItem (5, _softBlend);
				}

				// �ͻ��˻������
				if ( strServType == _T("�ͻ���") )
				{
					CBCGPGridItem* pItem;
					pItem = new CBCGPGridItem (_T("�ͻ���"));
					pItem->AddOption (_T("�ͻ���"), TRUE);
					pItem->AddOption(_T("������"), TRUE);
					pRow->ReplaceItem (6, pItem);
				}
				else if ( strServType == _T("������") )
				{
					CBCGPGridItem* pItem;
					pItem = new CBCGPGridItem (_T("������"));
					pItem->AddOption (_T("�ͻ���"), TRUE);
					pItem->AddOption(_T("������"), TRUE);
					pRow->ReplaceItem (6, pItem);
				}

				m_wndGrid.AddRow(pRow, TRUE);

				UpdateData( FALSE );

			}
		}
	}
}
//-------------------------------------------------------------------
void CClientAssistantDlg::writeConfigFile_XML()
{
	char szModule[MAX_PATH] ;      
	::GetModuleFileName (NULL, szModule, MAX_PATH);

	string srcFolder(szModule);

	string::size_type found = srcFolder.find_last_of( "\\/" );
	string srcFilePath = srcFolder.substr( 0 , found );
	string strName1 = srcFolder.substr(found + 1, srcFolder.length() - found - 5);

	CString configFilePath;

	configFilePath.Format( "%s\\%s.xml", srcFilePath.c_str(), strName1.c_str() );

	vgConfig::XMLConfigManager::getSingleton().LoadXmlDocument(configFilePath);

	for ( int i = 0; i < m_wndGrid.GetRowCount(); i++ )
	{
		CBCGPGridRow *pRow = m_wndGrid.GetRow(i);
		CString str_sceneName(pRow->GetItem(0)->GetValue().bstrVal);
		bool isHave = true;

		if ( !vec_str_config.empty())
		{
			vector<CString>::iterator iterBegin = vec_str_config.begin();
			vector<CString>::iterator iterEnd = vec_str_config.end();

			for ( iterBegin; iterBegin != iterEnd; iterBegin++ )
			{
				if ( (*iterBegin) == str_sceneName )
				{
					isHave = false;
				}
			}

		}

		if ( isHave )
		{
			vec_str_config.push_back(str_sceneName);
		}

	}

	CString strRowCount;
	strRowCount.Format("%d", vec_str_config.size());
	// CString strRowCount;		// ����ʹ��RowCount, ��ΪXML д�ļ�ʱ��
	// Section �� Key ����ͬ��
	// strRowCount.Format("%d", m_wndGrid.GetRowCount());

	vgConfig::XMLConfigManager::getSingleton().AddKeys(_T("Interface"),		// ���ƶԻ������ʾ/����
		_T("Interface"), _T("1"));

	vgConfig::XMLConfigManager::getSingleton().AddKeys(_T("SceneInfo"), 
		_T("SceneCount"), strRowCount);

	for ( int i = 0; i < m_wndGrid.GetRowCount(); i++ )
	{
		CBCGPGridRow *pRow = m_wndGrid.GetRow(i);
		CString str_sceneName(pRow->GetItem(0)->GetValue().bstrVal);

		vgConfig::XMLConfigManager::getSingleton().AddKeys(_T("SceneInfo"), 
			str_sceneName, str_sceneName);
	}

	for (int i=0; i<m_wndGrid.GetRowCount(); i++)
	{
		CBCGPGridRow *pRow = m_wndGrid.GetRow(i);

		CString sceneName(pRow->GetItem(0)->GetValue().bstrVal);
		CString vrgis_exePath(pRow->GetItem(1)->GetValue().bstrVal);
		CString vg_path(pRow->GetItem(2)->GetValue().bstrVal);

		int b_fullScreen = pRow->GetItem(3)->GetValue();
		int b_startChan = pRow->GetItem(4)->GetValue();
		int b_softBlend = pRow->GetItem(5)->GetValue();

		CString strFullScreen;
		CString strStartChan;
		CString strSoftBlend;

		if ( b_fullScreen == 0 )		// �Ƿ�ѡ��ȫ��
		{
			strFullScreen.Format("%s", _T("-nfs"));
		}
		else if ( b_fullScreen == -1 )
		{
			strFullScreen.Format("%s", _T("-fs"));
		}

		if ( b_startChan == 0 )			// �Ƿ�ѡ������ͨ��
		{
			strStartChan.Format("%s", _T("-nsc"));
		}
		else if ( b_startChan == -1 )
		{
			strStartChan.Format("%s", _T("-sc"));
		}

		if ( b_softBlend == 0 )			// �Ƿ�ѡ�������ں�
		{
			strSoftBlend.Format("%s", _T("-nbaa"));
		}
		else if ( b_softBlend == -1 )
		{
			strSoftBlend.Format("%s", _T("-baa"));
		}

		CString strServType(pRow->GetItem(6)->GetValue().bstrVal);

		vgConfig::XMLConfigManager::getSingleton().AddKeys(sceneName, 
			_T("vrgis_exePath"), vrgis_exePath);
		vgConfig::XMLConfigManager::getSingleton().AddKeys(sceneName, 
			_T("vg_path"), vg_path);
		vgConfig::XMLConfigManager::getSingleton().AddKeys(sceneName, 
			_T("FullScreen"), strFullScreen);
		vgConfig::XMLConfigManager::getSingleton().AddKeys(sceneName, 
			_T("StartChannel"), strStartChan);
		vgConfig::XMLConfigManager::getSingleton().AddKeys(sceneName, 
			_T("SoftBlend"), strSoftBlend);
		vgConfig::XMLConfigManager::getSingleton().AddKeys(sceneName, 
			_T("ServerType"), strServType);		
	}

	vgConfig::XMLConfigManager::getSingleton().SaveXML(configFilePath);

}
//-------------------------------------------------------------------
void CClientAssistantDlg::OnBnClickedBenDeleterow()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CBCGPGridRow *pNowRow = m_wndGrid.GetCurSel();

	if ( pNowRow != NULL )
	{
		CString sceneName(pNowRow->GetItem(0)->GetValue().bstrVal);
		
		char szModule[MAX_PATH] ;      
		::GetModuleFileName (NULL, szModule, MAX_PATH);

		string srcFolder(szModule);

		string::size_type found = srcFolder.find_last_of( "\\/" );
		string srcFilePath = srcFolder.substr( 0 , found );
		string strName1 = srcFolder.substr(found + 1, srcFolder.length() - found - 5);

		CString configFilePath;
		int sceneCount = 0;

		configFilePath.Format( "%s\\%s.xml", srcFilePath.c_str(), strName1.c_str() );
		
		bool isreadok = vgConfig::XMLConfigManager::getSingleton().LoadXmlDocument( configFilePath );

		if ( !isreadok )
		{
			//CString warning;
			//warning.Format("��ȡ�����ļ�%sʧ��!\n�����ļ��Ƿ����!", configFilePath);
			//AfxMessageBox(warning);
			return;
		}

		// ɾ�������ļ��е������Ϣ
		if ( vgConfig::XMLConfigManager::getSingleton().FindSection( sceneName ) )
		{
			vgConfig::XMLConfigManager::getSingleton().RemoveKeys(_T("SceneInfo"), sceneName);
			vgConfig::XMLConfigManager::getSingleton().RemoveSection( sceneName );
			CString strRowCount;
			strRowCount.Format("%d", m_wndGrid.GetRowCount() - 1);

			vgConfig::XMLConfigManager::getSingleton().AddKeys(_T("SceneInfo"), 
				_T("SceneCount"), strRowCount);
			vgConfig::XMLConfigManager::getSingleton().SaveXML(configFilePath);
		}

		m_wndGrid.RemoveRow( pNowRow->GetRowId(), TRUE );

	}
}
//-------------------------------------------------------------------
