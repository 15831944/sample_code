// D:\___visualVersion\trunkDLL\src\vgCam\vgcaDlgAviProgress.cpp : ʵ���ļ�
//

#include <vgStableHeaders.h>
#include "vgCam/vgcaDlgAviProgress.h"
#include <vgCam/vgcaCamManager.h>
#include <vgLibraryMFCDLL/vgLibraryResource.h>
namespace vgCam
{
	// vgcaDlgAviProgress �Ի���

	IMPLEMENT_DYNAMIC(vgcaDlgAviProgress, CDialog)

		vgcaDlgAviProgress::vgcaDlgAviProgress(CWnd* pParent /*=NULL*/)
		: CDialog(vgcaDlgAviProgress::IDD, pParent)
	{

	}

	vgcaDlgAviProgress::~vgcaDlgAviProgress()
	{
	}

	void vgcaDlgAviProgress::DoDataExchange(CDataExchange* pDX)
	{
		CDialog::DoDataExchange(pDX);
	}


	BEGIN_MESSAGE_MAP(vgcaDlgAviProgress, CDialog)
	END_MESSAGE_MAP()


	// vgcaDlgAviProgress ��Ϣ�������

	BOOL vgcaDlgAviProgress::DestroyWindow()
	{
		// TODO: �ڴ����ר�ô����/����û���

		return CDialog::DestroyWindow();
	}

	void vgcaDlgAviProgress::OnOK()
	{
		// TODO: �ڴ����ר�ô����/����û���

		// 	CDialog::OnOK();
		// 	DestroyWindow();
	}

	void vgcaDlgAviProgress::OnCancel()
	{
		// TODO: �ڴ����ר�ô����/����û���

		vgCam::CamManager::getSingleton().setAviRecordCnt(0);
	}

	BOOL vgcaDlgAviProgress::OnInitDialog()
	{
		CDialog::OnInitDialog();

		//m_progressBar.SetRange(0, 100);
		//setProgress(0);
 		CProgressCtrl *pProgressCtrl = (CProgressCtrl*)GetDlgItem(IDC_AVI_PROGRESS);
 		pProgressCtrl->SetRange(0, 100);
// 		
		setProgress(0);

		return TRUE;  // return TRUE unless you set the focus to a control
		// �쳣: OCX ����ҳӦ���� FALSE
	}

	void vgcaDlgAviProgress::setProgress(int progress)
	{
		CProgressCtrl *pProgressCtrl = (CProgressCtrl*)GetDlgItem(IDC_AVI_PROGRESS);
		pProgressCtrl->SetPos(progress);	
	}

}
