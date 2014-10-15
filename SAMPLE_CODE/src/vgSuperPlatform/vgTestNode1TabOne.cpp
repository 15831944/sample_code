// TabOne.cpp : ʵ���ļ�
//

#include <vgStableHeaders.h>
#include "vgFrameWork.h"
#include "vgTestNode1TabOne.h"
#include "vgPropertiesViewBar.h"
#include "vgUIController.h"
// TabOne �Ի���

IMPLEMENT_DYNAMIC(TabOne, CDialog)

TabOne::TabOne(CWnd* pParent /*=NULL*/)
	: CDialog(TabOne::IDD, pParent)
{
	// ::MessageBox(NULL, "Construct tab one ", "INFO", MB_OK);
}

TabOne::TabOne(int *data):CDialog(TabOne::IDD, NULL)
{

}

TabOne::~TabOne()
{
	//::MessageBox(NULL, "Destory tab one ", "INFO", MB_OK);
	// //TRACE("Destory Tab One \n");
}

string TabOne::GetTitle()
{
	return "����ҳ��"; 
}

CWnd* TabOne::GetWnd()
{
	return this;
}

void TabOne::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(TabOne, CDialog)
	ON_BN_CLICKED(NIDC_BUTTON1, &TabOne::OnBnClickedButton1)
	ON_NOTIFY(TVN_SELCHANGED, NIDC_TREE1, &TabOne::OnTvnSelchangedTree1)
	ON_WM_DESTROY()
	ON_NOTIFY(TVN_BEGINDRAG, NIDC_TREE1, &TabOne::OnTvnBegindragTree1)
	ON_NOTIFY(NM_RCLICK, NIDC_TREE1, &TabOne::OnNMRclickTree1)
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()


// TabOne ��Ϣ�������

void TabOne::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	vgUIController::GetInstance()->RemoveAllPages();
}

void TabOne::OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
}

void TabOne::OnDestroy()
{
	__super::OnDestroy();
	
	// TODO: �ڴ˴������Ϣ����������
}

// ������DestoryWindow ����ÿ����ʾӦ����create����
//BOOL TabOne::DestroyWindow()
//{
//	::MessageBox(NULL, "Destory window here, so you should rebuild it", "Info " ,MB_OK);
//	return true;
//}
void TabOne::OnTvnBegindragTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
}

void TabOne::OnNMRclickTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;

	
}

void TabOne::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	__super::OnMouseMove(nFlags, point);
}
