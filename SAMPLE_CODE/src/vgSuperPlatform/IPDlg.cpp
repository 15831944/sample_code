// IPDlg.cpp : ʵ���ļ�
//

#include "vgStableHeaders.h"
#include "IPDlg.h"


// CIPDlg �Ի���

IMPLEMENT_DYNAMIC(CIPDlg, CDialog)

CIPDlg::CIPDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CIPDlg::IDD, pParent)
	, IP1(211)
	, IP2(64)
	, IP3(136)
	, IP4(19)
{

}

CIPDlg::~CIPDlg()
{
}

void CIPDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_IP1, IP1);
	DDX_Text(pDX, IDC_IP2, IP2);
	DDX_Text(pDX, IDC_IP3, IP3);
	DDX_Text(pDX, IDC_IP4, IP4);
}


BEGIN_MESSAGE_MAP(CIPDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CIPDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CIPDlg ��Ϣ�������

void CIPDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData();
	OnOK();
}
