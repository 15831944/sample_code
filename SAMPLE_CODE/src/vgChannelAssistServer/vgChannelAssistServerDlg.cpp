// channelAssistantDlg.cpp : ʵ���ļ�
//

#include "vgStableHeaders.h"
#include "vgChannelAssistServer/vgChannelAssistServer.h"
#include "vgChannelAssistServer/vgChannelAssistServerDlg.h"

#include <WinSock2.h>
#include <string>
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


static SOCKET	assitSendSocket;
static SOCKET	assitRecvSocket;
static SOCKADDR_IN addrSnd;

static DWORD	chanThreadID;
static HANDLE	assitReceiveHandle;

static DWORD WINAPI assitReceiveThread(LPVOID lpParameter);
static void assitReceive(long dataLength);
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


// CchannelAssistantDlg �Ի���




CchannelAssistantDlg::CchannelAssistantDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CchannelAssistantDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CchannelAssistantDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_VGNAME, m_vgNameCombo);
}

BEGIN_MESSAGE_MAP(CchannelAssistantDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_START_CHANNEL, &CchannelAssistantDlg::OnBnClickedBtnStartChannel)
	ON_BN_CLICKED(IDC_BTN_END_CHANNEL, &CchannelAssistantDlg::OnBnClickedBtnEndChannel)
	ON_BN_CLICKED(IDC_BTN_REBOOT, &CchannelAssistantDlg::OnBnClickedBtnReboot)
	ON_BN_CLICKED(IDC_BTN_SHUTDOWN, &CchannelAssistantDlg::OnBnClickedBtnShutdown)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CchannelAssistantDlg ��Ϣ�������

BOOL CchannelAssistantDlg::OnInitDialog()
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
	//////////////////////////////////////////////////////////////////////////
	WSADATA wsd;

	int ret = WSAStartup(0x0202,&wsd);
	if(ret!=0)
	{
		WSACleanup();
	}
	//��������Socket
	assitRecvSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP); 

	if(assitRecvSocket == INVALID_SOCKET)
	{
		return false;
	}

	SOCKADDR_IN addr;
	ZeroMemory(&addr, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(8694);
	// �󶨽���ʹ�õĶ˿ں͵�ַ
	ret = bind(assitRecvSocket, (struct sockaddr *)&addr, sizeof(addr));


	// ����Server����Socket
	assitSendSocket = socket(AF_INET,SOCK_DGRAM,IPPROTO_IP); 
	if(assitSendSocket == INVALID_SOCKET)
	{
		//return 0;
		AfxMessageBox("channelAssistant��������Socketʧ��");
	}

	
	//�󶨷���
	ZeroMemory(&addrSnd,sizeof(addrSnd));
	addrSnd.sin_family = AF_INET;
	addrSnd.sin_addr.S_un.S_addr=htonl(INADDR_ANY);
	//��ù㲥��IP
	char hostName[255];   
	PHOSTENT hostinfo;
	std::string m_broadCastAddress;

	if(gethostname(hostName, sizeof(hostName))   ==   0)   
	{   
		if((hostinfo = gethostbyname(hostName)) != NULL)   
		{   
			m_broadCastAddress = inet_ntoa(*(struct in_addr *)*hostinfo->h_addr_list);   
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
		else		//IP��ַ�����3λ
		{
			m_broadCastAddress[m_broadCastAddress.length()-1] = '5';
			m_broadCastAddress[m_broadCastAddress.length()-2] = '5';
			m_broadCastAddress[m_broadCastAddress.length()-3] = '2';
		}

	} 

	addrSnd.sin_addr.s_addr = INADDR_ANY;
	ret = bind(assitSendSocket, (struct sockaddr *)&addrSnd, sizeof(addrSnd));
	addrSnd.sin_addr.s_addr = inet_addr(m_broadCastAddress.c_str());
	//addrSnd.sin_addr.s_addr = inet_addr("222.195.148.255");
	
	if(ret != 0)
	{
		//return 0;
		AfxMessageBox("channelAssistant�󶨷���Socketʧ��");
	}

	assitReceiveHandle = 
		CreateThread(NULL, 0, assitReceiveThread, this, 0, &chanThreadID);

	SetThreadPriority( assitReceiveHandle, THREAD_PRIORITY_ABOVE_NORMAL);

	if( assitReceiveHandle == NULL )
	{
		return false;
	}
	//////////////////////////////////////////////////////////////////////////
	readvgName();
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CchannelAssistantDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CchannelAssistantDlg::OnPaint()
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
HCURSOR CchannelAssistantDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CchannelAssistantDlg::OnBnClickedBtnStartChannel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	CString wndStr;

	int nSelIndex = 0;
	nSelIndex = m_vgNameCombo.GetCurSel();

	if ( -1 == nSelIndex )
	{
		AfxMessageBox("û��ѡ��vg�ļ�����������ѡ��");
		return;
	}

	wndStr.GetBuffer(m_vgNameCombo.GetLBTextLen( nSelIndex ));
	m_vgNameCombo.GetLBText(m_vgNameCombo.GetCurSel(), wndStr);

	CString strSend;
	
	strSend.Format("VRGISstartChannel%s", wndStr.GetString());
	
	addrSnd.sin_port=htons(8695);
	sendto(assitSendSocket,strSend,strSend.GetLength()+1,0,
		(SOCKADDR*)&addrSnd,sizeof(SOCKADDR));

}

void CchannelAssistantDlg::OnBnClickedBtnEndChannel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	CString strSend = "VRGISendChannel";
	addrSnd.sin_port=htons(8695);
	sendto(assitSendSocket,strSend,strSend.GetLength()+1,0,
		(SOCKADDR*)&addrSnd,sizeof(SOCKADDR));
}

void CchannelAssistantDlg::OnBnClickedBtnReboot()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	CString strSend = "vrgisReStartComputer";
	addrSnd.sin_port=htons(8695);
	sendto(assitSendSocket,strSend,strSend.GetLength()+1,0,
		(SOCKADDR*)&addrSnd,sizeof(SOCKADDR));

}

void CchannelAssistantDlg::OnBnClickedBtnShutdown()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	CString strSend = "vrgisEndComputer";
	addrSnd.sin_port=htons(8695);
	sendto(assitSendSocket,strSend,strSend.GetLength()+1,0,
		(SOCKADDR*)&addrSnd,sizeof(SOCKADDR));

}

void CchannelAssistantDlg::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: �ڴ˴������Ϣ����������
	WSACleanup();
	closesocket(assitSendSocket);

}

DWORD WINAPI assitReceiveThread(LPVOID lpParameter)
{
	int ret;
	unsigned long lLength;

	while( TRUE )
	{
		ret = ioctlsocket(assitRecvSocket, FIONREAD, &lLength);
		if(ret != 0)
		{
			break;
		}
		else
		{
			if(lLength>0)
			{
				assitReceive(lLength);
			}
		}

		Sleep(10);
	}

	return 0;
}

void assitReceive(long dataLength)
{
	char *pbuf;
	int addr_len, iRecLen;;
	struct sockaddr_in addr;

	pbuf = new char[dataLength + 1];
	memset(pbuf, 0, dataLength + 1);

	addr_len = sizeof(addr);

	iRecLen = recvfrom(assitRecvSocket, pbuf, dataLength, 0,(struct sockaddr *)&addr, &addr_len);

	CString recvStr(pbuf);	//���յ�����Ϣ

	if( iRecLen <= 0 )
	{
		return;
	}
	else
	{
		AfxMessageBox(recvStr);
	}

}

void CchannelAssistantDlg::readvgName()
{
	char szModule[MAX_PATH] ;      
	::GetModuleFileName (NULL, szModule, MAX_PATH);

	string srcFolder(szModule);

	string::size_type found = srcFolder.find_last_of( "\\/" );
	string srcFilePath = srcFolder.substr( 0 , found );
	srcFilePath += "\\vgChannelAssistServer.ini";
	
	FILE *fp;

	fopen_s(&fp, srcFilePath.c_str(), "r");

	if (fp == NULL)
	{
		CString strWarning;
		strWarning.Format("ChannelAssistServer.exe �������ļ�ʧ�ܣ�\n��ȷ��Ӧ�ó���Ŀ¼�´���%s�ļ�", srcFilePath.c_str());
		AfxMessageBox(strWarning);
		return;
	}
	else
	{
		char str[256];
		int cnt = 0;

		while ( fgets(str, 256, fp) != NULL)
		{
			CString strInFile(str);

			int m_length = strInFile.GetLength();

			if ( strInFile.Right(1) == "\n")
			{
				strInFile.TrimRight("\n");
			}

			m_vgNameCombo.AddString(strInFile);
		}

		if ( m_vgNameCombo.GetCount() > 0)
		{
			m_vgNameCombo.SetCurSel(0);
		}

 		UpdateData(FALSE);

		fclose(fp);
	}
}