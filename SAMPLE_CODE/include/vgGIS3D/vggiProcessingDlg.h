#pragma once

#include <vgGIS/resource.h>
#include <vgGIS/vggisDefinition.h>

// vgProcessingDlg �Ի���
namespace vgGIS3D
{
	class VGG_EXPORT vgProcessingDlg : public CDialog
	{
		DECLARE_DYNAMIC(vgProcessingDlg)

	public:
		vgProcessingDlg(CWnd* pParent = NULL);   // ��׼���캯��
		virtual ~vgProcessingDlg();

		// �Ի�������
		enum { IDD = IDD_PROCESSING };

	public:
		void setProgress(int progress);

		void setInfo(string info);

	protected:
		virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

		DECLARE_MESSAGE_MAP()
		CString m_proccesingInfo;
		CProgressCtrl m_progressBar;

	public:
		virtual BOOL OnInitDialog();


	protected:
		virtual void OnOK();
		virtual void OnCancel();
	public:
		virtual BOOL DestroyWindow();
	};

}
