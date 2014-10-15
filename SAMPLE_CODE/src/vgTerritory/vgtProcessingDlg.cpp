
#include <vgStableHeaders.h>
#include <vgTerritory/vgtProcessingDlg.h>


namespace vgTerritory
{
	// vgProcessingDlg �Ի���

	IMPLEMENT_DYNAMIC(vgtProcessingDlg, CDialog)

		vgtProcessingDlg::vgtProcessingDlg(CWnd* pParent /*=NULL*/)
		: CDialog(vgtProcessingDlg::IDD, pParent)
		, m_proccesingInfo(_T(""))
	{

	}

	vgtProcessingDlg::~vgtProcessingDlg()
	{
	}

	void vgtProcessingDlg::DoDataExchange(CDataExchange* pDX)
	{
		CDialog::DoDataExchange(pDX);
		DDX_Text(pDX, IDC_INFO, m_proccesingInfo);
		DDX_Control(pDX, IDC_PROGRESS_INDICATOR, m_progressBar);
	}


	BEGIN_MESSAGE_MAP(vgtProcessingDlg, CDialog)
	END_MESSAGE_MAP()


	// vgProcessingDlg ��Ϣ�������

	BOOL vgtProcessingDlg::OnInitDialog()
	{
		CDialog::OnInitDialog();

		m_proccesingInfo = "Processing ... ";
		
		m_progressBar.SetRange(0, 100);

		UpdateData(FALSE);

		// TODO:  �ڴ���Ӷ���ĳ�ʼ��

		return TRUE;  // return TRUE unless you set the focus to a control
		// �쳣: OCX ����ҳӦ���� FALSE
	}

	void vgtProcessingDlg::setInfo(std::string info)
	{
		m_proccesingInfo = info.c_str();

		//UpdateData(FALSE);
		UpdateData(false);
	}

	void vgtProcessingDlg::setProgress(int progress)
	{
		if (progress > 100 || progress < 0)
		{
			return ;
		}

		m_progressBar.SetPos(progress);

		UpdateData(FALSE);
	}

	void vgtProcessingDlg::OnOK()
	{
		// TODO: �ڴ����ר�ô����/����û���

		CDialog::OnOK();
	}

	void vgtProcessingDlg::OnCancel()
	{
		// TODO: �ڴ����ר�ô����/����û���

		CDialog::OnCancel();
	}

	BOOL vgtProcessingDlg::DestroyWindow()
	{
		// TODO: �ڴ����ר�ô����/����û���

		return CDialog::DestroyWindow();
	}

}
