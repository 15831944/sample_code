// D:\___NewVersion\vrgis\src\vgSuperPlatform\DlgPlanParam.cpp : ʵ���ļ�
//

#include <vgStableHeaders.h>
#include "DlgPlanParam.h"
#include <vgPlan/vgPlanmanager.h>

// DlgPlanParam �Ի���

IMPLEMENT_DYNAMIC(DlgPlanParam, CDialog)

DlgPlanParam::DlgPlanParam(CWnd* pParent /*=NULL*/)
	: CDialog(DlgPlanParam::IDD, pParent)
	, m_gridX(1)
	, m_gridY(1)
{

}

DlgPlanParam::~DlgPlanParam()
{
}

void DlgPlanParam::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_GRID_REV_X, m_gridX);
	//DDV_MinMaxInt(pDX, m_gridX, 1, 10000);
	DDX_Text(pDX, IDC_GRID_REV_Y, m_gridY);
	//DDV_MinMaxInt(pDX, m_gridY, 1, 10000);
}


BEGIN_MESSAGE_MAP(DlgPlanParam, CDialog)
	ON_CBN_SELCHANGE(IDC_SCHEM_COMBO, &DlgPlanParam::OnCbnSelchangeSchemCombo)
	ON_BN_CLICKED(IDC_GENERATE_GRID, &DlgPlanParam::OnBnClickedGenerateGrid)
END_MESSAGE_MAP()

BOOL DlgPlanParam::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_SCHEM_COMBO);

	if (pCombo == NULL)
	{
		return FALSE;
	}

	pCombo->Clear();

	stringstream outSting;
	int nameCnt = vgPlan::vgPlanManager::getSingleton().getNumOfScene();

	for (int i=0; i<nameCnt; i++)
	{
		outSting.str("");
		outSting << vgPlan::vgPlanManager::getSingleton().getSceneNameById(i);

		pCombo->AddString(outSting.str().c_str());
	}

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

// DlgPlanParam ��Ϣ�������

void DlgPlanParam::OnCbnSelchangeSchemCombo()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_SCHEM_COMBO);
	
	if (pCombo == NULL)
	{
		return ;
	}

	int index = pCombo->GetCurSel();
	CString comboString;
	pCombo->GetLBText(index, comboString);

	// vgPlan::vgPlanManager::getSingleton().setActiveSceneById(index);
	
	vgPlan::vgPlanManager::getSingleton().setActiveSceneByName(comboString.GetString());

	m_gridX = vgPlan::vgPlanManager::getSingleton().getActiveScene()->getGridResolveX();
	m_gridY = vgPlan::vgPlanManager::getSingleton().getActiveScene()->getGridResolveY();

	UpdateData(TRUE);
}


void DlgPlanParam::OnBnClickedGenerateGrid()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	stringstream outString;

	UpdateData(TRUE);
	
	if (m_gridX <= 0 || m_gridY <= 0)
	{
		AfxMessageBox("դ��ֱ��ʲ���ȷ.");

		return ;
	}

	vgPlan::vgPlanScene *pScene = vgPlan::vgPlanManager::getSingleton().getActiveScene();

	if (pScene != NULL)
	{
		outString << "������"<<pScene->getName() << "����" << m_gridX << "," << m_gridY <<"դ�� \n";

		VGK_SHOW(outString.str());

		pScene->caculateGrid(m_gridX, m_gridY);
	}
	
	AfxMessageBox("�������");
}
