// vgRootTabPage.cpp : ʵ���ļ�
//

#include <vgStableHeaders.h>
#include "vgFrameWork.h"
#include "vgRootTabPage.h"


// vgRootTabPage �Ի���

IMPLEMENT_DYNAMIC(vgRootTabPage, CDialog)

vgRootTabPage::vgRootTabPage(CWnd* pParent /*=NULL*/)
	: CDialog(vgRootTabPage::IDD, pParent)
{

}

vgRootTabPage::~vgRootTabPage()
{
}

void vgRootTabPage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

string vgRootTabPage::GetTitle()
{
	return "VRGIS";
}

CWnd* vgRootTabPage::GetWnd()
{
	return this;
}

BEGIN_MESSAGE_MAP(vgRootTabPage, CDialog)
END_MESSAGE_MAP()


// vgRootTabPage ��Ϣ�������
