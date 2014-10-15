// D:\___NewVersion\vrgis1010\src\vgSuperPlatform\vgDlgEdgeBlend.cpp : ʵ���ļ�
//

#include <vgStableHeaders.h> 
#include "vgChannel/vgDlgEdgeBlend.h"
#include <vgChannel/vgcChannelManager.h>

// vgDlgEdgeBlend �Ի���

IMPLEMENT_DYNAMIC(vgDlgEdgeBlend, CDialog)

vgDlgEdgeBlend::vgDlgEdgeBlend(CWnd* pParent /*=NULL*/)
	: CDialog(vgDlgEdgeBlend::IDD, pParent)
	, m_leftEdge(0)
	, m_rightEdge(0)
	, m_red(0)
	, m_green(0)
	, m_blue(0)
	, m_leftGamma(0)
	, m_rightGamma(0)
	, m_fullGamma(0)
{

}

vgDlgEdgeBlend::~vgDlgEdgeBlend()
{
}

void vgDlgEdgeBlend::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_LEFTEDGE, m_leftEdge);
	DDX_Text(pDX, IDC_RIGHTEDGE, m_rightEdge);
	DDX_Text(pDX, IDC_RED, m_red);
	DDX_Text(pDX, IDC_GREEN, m_green);
	DDX_Text(pDX, IDC_BLUE, m_blue);
	DDX_Text(pDX, IDC_LEFT_GAMMA, m_leftGamma);
	DDX_Text(pDX, IDC_RIGHT_GAMMA, m_rightGamma);
	DDX_Text(pDX, IDC_FGAMMA, m_fullGamma);
}


BEGIN_MESSAGE_MAP(vgDlgEdgeBlend, CDialog)
	ON_EN_CHANGE(IDC_RIGHTEDGE, &vgDlgEdgeBlend::OnEnChangeRightedge)
	ON_EN_CHANGE(IDC_RED, &vgDlgEdgeBlend::OnEnChangeRed)
	ON_EN_CHANGE(IDC_GREEN, &vgDlgEdgeBlend::OnEnChangeGreen)
	ON_EN_CHANGE(IDC_BLUE, &vgDlgEdgeBlend::OnEnChangeBlue)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDOK, &vgDlgEdgeBlend::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &vgDlgEdgeBlend::OnBnClickedCancel)
	ON_EN_CHANGE(IDC_LEFTEDGE, &vgDlgEdgeBlend::OnEnChangeLeftedge) 
	ON_EN_CHANGE(IDC_LEFT_GAMMA, &vgDlgEdgeBlend::OnEnChangeLeftGamma)
	ON_EN_CHANGE(IDC_RIGHT_GAMMA, &vgDlgEdgeBlend::OnEnChangeRightGamma)
	ON_EN_CHANGE(IDC_FGAMMA, &vgDlgEdgeBlend::OnEnChangeFgamma)
END_MESSAGE_MAP()


// vgDlgEdgeBlend ��Ϣ�������

void vgDlgEdgeBlend::OnEnChangeRightedge()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ�������������
	// ���͸�֪ͨ��������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������

	UpdateData(TRUE);

	m_pBlendPixelImpl->setRightEdge(m_rightEdge);
}

// void vgDlgEdgeBlend::OnEnChangeGamma()
// {
// 	// TODO:  ����ÿؼ��� RICHEDIT �ؼ�������������
// 	// ���͸�֪ͨ��������д CDialog::OnInitDialog()
// 	// ���������� CRichEditCtrl().SetEventMask()��
// 	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�
// 
// 	// TODO:  �ڴ���ӿؼ�֪ͨ����������
// 
// 	UpdateData(TRUE);
// 
// 	m_pBlendPixelImpl->setGamma(m_gamma);
// }

void vgDlgEdgeBlend::OnEnChangeRed()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ�������������
	// ���͸�֪ͨ��������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������

	UpdateData(TRUE);

	m_pBlendPixelImpl->setRedGamma(m_red);
}

void vgDlgEdgeBlend::OnEnChangeGreen()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ�������������
	// ���͸�֪ͨ��������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������

	UpdateData(TRUE);

	m_pBlendPixelImpl->setGreenGamma(m_green);
}

void vgDlgEdgeBlend::OnEnChangeBlue()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ�������������
	// ���͸�֪ͨ��������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������

	UpdateData();

	m_pBlendPixelImpl->setBlueGamma(m_blue);
}

BOOL vgDlgEdgeBlend::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	m_pBlendPixelImpl = vgChannel::ChannelManager::getSingleton().getBlenderPtr();

	m_leftEdge = m_pBlendPixelImpl->getLeftEdge();
	m_rightEdge = m_pBlendPixelImpl->getRightEdge();

	m_leftGamma = m_pBlendPixelImpl->getLeftGama();
	m_rightGamma = m_pBlendPixelImpl->getRightGama();

	m_red = m_pBlendPixelImpl->getRedGamma();
	m_green = m_pBlendPixelImpl->getGreenGama();
	m_blue = m_pBlendPixelImpl->getBlueGamma();
	
	m_fullGamma = m_pBlendPixelImpl->getFullScreenGamma();

	UpdateData(FALSE);

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void vgDlgEdgeBlend::OnDestroy()
{
	CDialog::OnDestroy();
	
	delete this;
	// TODO: �ڴ˴������Ϣ����������
}

void vgDlgEdgeBlend::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_pBlendPixelImpl->SaveToXml();	//@FengYK 2009/10/31 11:05
	OnOK();
	DestroyWindow();
}

void vgDlgEdgeBlend::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString strExtension = _T(".xml");

	char exebuf[512];
	DWORD getret = GetModuleFileName( NULL , exebuf , 512 );

	assert( getret < 512 );
	assert( getret > 0 );

	String exestr( exebuf );
	String exepath = exestr.substr( 0 , 
		exestr.find_last_of("\\/")  + 1);

	CString path;
	path.Format("%s", exepath.c_str());

	Xmlfilename =  _T("Channel");		//@FengYK	2009/10/31 10:21
	CString sFileName = path + Xmlfilename + strExtension ;
	Xmlsectionname = _T("Edge blending correction");
	if (vgConfig::XMLConfigManager::getSingleton().LoadXmlDocument(sFileName)&&
		vgConfig::XMLConfigManager::getSingleton().FindSection(Xmlsectionname))
	{
		m_pBlendPixelImpl->initBlendParam();
	}
	OnCancel();
	DestroyWindow();
}

void vgDlgEdgeBlend::OnEnChangeLeftedge()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ�������������
	// ���͸�֪ͨ��������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);

	m_pBlendPixelImpl->setLeftEdge(m_leftEdge);
}

void vgDlgEdgeBlend::OnEnChangeLeftGamma()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ�������������
	// ���͸�֪ͨ��������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);

	m_pBlendPixelImpl->setLeftGama(m_leftGamma);
}

void vgDlgEdgeBlend::OnEnChangeRightGamma()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ�������������
	// ���͸�֪ͨ��������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������

	UpdateData(TRUE);

	m_pBlendPixelImpl->setRightGama(m_rightGamma);
}

void vgDlgEdgeBlend::OnEnChangeFgamma()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ�������������
	// ���͸�֪ͨ��������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);

	m_pBlendPixelImpl->setFullScreenGamma(m_fullGamma);
}
