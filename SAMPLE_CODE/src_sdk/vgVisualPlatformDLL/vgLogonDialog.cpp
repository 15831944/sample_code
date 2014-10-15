// D:\___visualVersion\from1297_trunkDLL-leshan\src_sdk\vgVisualPlatformDLL\vgLogonDialog.cpp : ʵ���ļ�
//

#include <vgStableHeaders.h>
#include <vgVisualPlatformDLL\vgLogonDialog.h>

#include <vgGlobal/vggSceneFileUtility.h>

// vgLogonDialog �Ի���

IMPLEMENT_DYNAMIC(vgLogonDialog, CDialog)

vgLogonDialog::vgLogonDialog(CWnd* pParent /*=NULL*/)
	: CDialog(vgLogonDialog::IDD, pParent)
	, m_user(_T(""))
	, m_passWord(_T(""))
	, m_userName(_T(""))
	, m_status(_T(""))
{
	m_bLoading = false;
}

vgLogonDialog::~vgLogonDialog()
{
}

void vgLogonDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SCENE_COMBO, m_sceneCombo);
	DDX_Text(pDX, IDC_CONNECT_PASSWORD, m_passWord);
	DDX_Text(pDX, IDC_CONNECT_USER, m_userName);
	DDX_Control(pDX, IDC_CONNET_IP, m_ipControl);
	DDX_Text(pDX, IDC_CONNECT_STATUES, m_status);
}


BEGIN_MESSAGE_MAP(vgLogonDialog, CDialog)
	ON_BN_CLICKED(IDC_CONNECT_SERVER, &vgLogonDialog::OnBnClickedConnectServer)
	ON_BN_CLICKED(IDOK, &vgLogonDialog::OnBnClickedOk)
	ON_WM_TIMER()
END_MESSAGE_MAP()

void vgLogonDialog::OnTimer(UINT_PTR nIDEvent)
{
	static int numOfDot;
	static bool enableUpdate = false;
	
	UpdateData(TRUE);

	if (m_bLoading)
	{
		numOfDot ++;

		if (numOfDot > 6)
		{
			numOfDot = 0;
		}
		
		string outString = "״̬ - ͨ����";
		
		for (int i=0; i<numOfDot; i++)
		{
			outString += ".";
		}

		m_status = outString.c_str();

		enableUpdate = true;
	}
	else
	{
		m_status = "״̬ - �ͻ��˿���";
	}

	//if (enableUpdate)
	{
		UpdateData(FALSE);
	}

	CDialog::OnTimer(nIDEvent);
}

 BOOL vgLogonDialog::OnInitDialog()
 {
	 __super::OnInitDialog();

 	// this->SetTimer(1, 300, NULL);

	 m_status = "״̬ - �ͻ��˿���";

	 UpdateData(FALSE);

 	return TRUE;
 }

void vgLogonDialog::OnBnClickedConnectServer()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString ip;

	m_ipControl.GetWindowText(ip);

	// AfxMessageBox(ip);

	m_scenePath = "\\\\";
	m_scenePath += ip.GetString();

	m_scenePath += "\\vg\\";

	string indexPath = m_scenePath + "scene.txt";



	FILE *fp = fopen(indexPath.c_str(), "r");

	if (fp == NULL)
	{
		AfxMessageBox("����Զ�̷�����ʧ�ܣ�������������.");
		return ;
	}

	vgKernel::StringVector mod;

	char buffer[256];

	m_sceneCombo.Clear();

	while (fgets(buffer, 256, fp) != NULL)
	{
		if (buffer[strlen(buffer)-1] == '\n')
			buffer[strlen(buffer)-1] = '\0';
		
		m_sceneCombo.AddString(buffer);
	}

	fclose(fp);

	m_sceneCombo.SetCurSel(0);

	UpdateData(FALSE);
}


void vgLogonDialog::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	
	CString scene;
	m_sceneCombo.GetWindowText(scene);
	
	string name = scene.GetString();

	if (name.length() == 0)
	{
		AfxMessageBox("δ���ӳ���.��ѡ�������.");
	}

	m_sceneVgPath = "";

	m_sceneVgPath = m_scenePath + "VRGIS_";
	m_sceneVgPath = m_sceneVgPath + name;
	m_sceneVgPath = m_sceneVgPath + "\\";
	m_sceneVgPath = m_sceneVgPath + name;
	m_sceneVgPath = m_sceneVgPath + ".vg";

	// AfxMessageBox(m_sceneVgPath.c_str());
	
	if (m_userName != "admin" || m_passWord != "vrgis")
	{
		AfxMessageBox("������û������룬����ϵ����Ա.");
		return ;
	}


	string vgPath = getVgSceneFullPath();

	FILE *fp = fopen(vgPath.c_str(), "r");

	if (fp == NULL)
	{
		AfxMessageBox("�������ڲ�����.����ϵ����Ա");

		return ;
	}

	fclose(fp);
	
	m_bLoading = true;

	m_status = "״̬ - ͨ����";

	UpdateData(FALSE);

	vgGlobal::SceneFileUtility::setUpdateTime( 1 );

	vgGlobal::SceneFileUtility::openScene(vgPath);

	m_bLoading = false;

	OnOK();
}
