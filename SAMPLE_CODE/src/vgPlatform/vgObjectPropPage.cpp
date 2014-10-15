// vgObjectPropPage.cpp : ʵ���ļ�
//

#include <vgStableHeaders.h>
#include "vgFrameWork.h"
#include "vgObjectPropPage.h"
#include "vgObjectNode.h"

// vgObjectPropPage �Ի���

IMPLEMENT_DYNAMIC(vgObjectPropPage, CDialog)

vgObjectPropPage::vgObjectPropPage(CWnd* pParent /*=NULL*/)
	: CDialog(vgObjectPropPage::IDD, pParent)
{
}

vgObjectPropPage::~vgObjectPropPage()
{
}

void vgObjectPropPage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

string vgObjectPropPage::GetTitle()
{
	return "����";
}

CWnd* vgObjectPropPage::GetWnd()
{
	return this;
}


BEGIN_MESSAGE_MAP(vgObjectPropPage, CDialog)
	ON_REGISTERED_MESSAGE(BCGM_PROPERTY_CHANGED, OnPropertyChanged)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()

void vgObjectPropPage::AdjustLayout()
{
	CRect rectClient,rectCombo;
	GetClientRect (rectClient);
	m_wndPropList.SetWindowPos (NULL,
		rectClient.left, 
		rectClient.top, 
		rectClient.Width(),
		rectClient.Height(),
		SWP_NOACTIVATE | SWP_NOZORDER);
}

void vgObjectPropPage::InitPropList()
{
	// Ϊ�趨��ʼֵ����ת��
	vgObjectNode* tnode = dynamic_cast<vgObjectNode*>(m_pNode);

	m_wndPropList.EnableHeaderCtrl (FALSE);
	m_wndPropList.EnableDesciptionArea ();
	m_wndPropList.SetVSDotNetLook ();
	m_wndPropList.MarkModifiedProperties ();
	
	CBCGPProp* pGroup0 = new CBCGPProp (_T("��������"));
	pGroup0->AddSubItem(new CBCGPProp(_T("X ����"), (_variant_t)(float)tnode->m_posX, _T("ָ��X ����")));
	pGroup0->AddSubItem(new CBCGPProp(_T("Y ����"), (_variant_t)(float)tnode->m_posY, _T("ָ��Y ����")));
	pGroup0->AddSubItem(new CBCGPProp(_T("Z ����"), (_variant_t)(float)tnode->m_posZ, _T("ָ��Z ����")));	

	m_wndPropList.AddProperty(pGroup0);
	
	CBCGPProp* pGroup1 = new CBCGPProp (_T("LOD"));
	pGroup1->AddSubItem(new CBCGPProp(_T("NEAR"), (_variant_t)(long)tnode->m_near, _T("NEAR")));
	pGroup1->AddSubItem(new CBCGPProp(_T("FAR"), (_variant_t)(long)tnode->m_far, _T("FAR")));
	m_wndPropList.AddProperty (pGroup1);
		

	CBCGPProp* pGroup2 = new CBCGPProp (_T("��ת"));

	pGroup2->AddSubItem (new CBCGPProp (_T("������ת"), (_variant_t) false,
		_T("ָ�������Ƿ���Ա���ת")));

	pGroup2->AddSubItem(new CBCGPProp(_T("X �ỡ��"), tnode->m_rotX, _T("ָ��X ����ת�Ƕ�")));
	pGroup2->AddSubItem(new CBCGPProp(_T("Y �ỡ��"), tnode->m_rotY, _T("ָ��Y ����ת�Ƕ�")));
	pGroup2->AddSubItem(new CBCGPProp(_T("Z �ỡ��"), tnode->m_rotZ, _T("ָ��Z ����ת�Ƕ�")));	

	m_wndPropList.AddProperty (pGroup2);

	CBCGPProp* pGroup3 = new CBCGPProp (_T("����"));

	pGroup3->AddSubItem(new CBCGPProp(_T("X ��"), (_variant_t)(float)tnode->m_scaleX, _T("ָ��X �����ű���")));
	pGroup3->AddSubItem(new CBCGPProp(_T("Y ��"), (_variant_t)(float)tnode->m_scaley, _T("ָ��Y �����ű���")));
	pGroup3->AddSubItem(new CBCGPProp(_T("Z ��"), (_variant_t)(float)tnode->m_scaleZ, _T("ָ��Z �����ű���")));	

	m_wndPropList.AddProperty (pGroup3);

}
// vgObjectPropPage ��Ϣ�������

LRESULT vgObjectPropPage::OnPropertyChanged(WPARAM w, LPARAM l)
{
	CBCGPProp* pProp = (CBCGPProp*)l;
	vgObjectNode* tnode = dynamic_cast<vgObjectNode*>(m_pNode);
	_variant_t var = pProp->GetValue(); 
	string name(pProp->GetName());

	if (name == "X ����")
	{
		tnode->m_posX = var.fltVal;	
	} 
	else if (name == "Y ����")
	{
		tnode->m_posY = var.fltVal;
	}
	else if (name == "Z ����")
	{
		tnode->m_posZ = var.fltVal;
	}
	else if (name == "NEAR")
	{
		tnode->m_near = var.intVal;
	}
	else if (name == "FAR")
	{
		tnode->m_far = var.intVal;
	}
	else if (name == "X �ỡ��")
	{
		tnode->m_rotX = var.fltVal;
	}
	else if (name == "Y �ỡ��")
	{
		tnode->m_rotY= var.fltVal;
	}	
	else if (name == "Z �ỡ��")
	{
		tnode->m_rotZ = var.fltVal;
	}
	else if (name == "X ��")
	{
		tnode->m_scaleX = var.intVal;
	}
	else if (name == "Y ��")
	{
		tnode->m_scaley = var.intVal;
	}
	else if (name == "Z ��")
	{
		tnode->m_scaleZ = var.intVal;
	}
	return NULL;
}

int vgObjectPropPage::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  �ڴ������ר�õĴ�������
	CRect rectDummy;
	rectDummy.SetRectEmpty ();

	if (!m_wndPropList.Create (WS_VISIBLE | WS_CHILD, rectDummy, this, 2))
	{
		TRACE0("Failed to create Properies Grid \n");
		return -1;      // fail to create
	}

	InitPropList();
	AdjustLayout ();

	return 0;
}

void vgObjectPropPage::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType, cx, cy);

	AdjustLayout();
	// TODO: �ڴ˴������Ϣ����������
}

BOOL vgObjectPropPage::DestroyWindow()
{
	// TODO: �ڴ����ר�ô����/����û���
	// ::MessageBox(NULL , "Window Destory here","info" ,MB_OK);
	return __super::DestroyWindow();
}
