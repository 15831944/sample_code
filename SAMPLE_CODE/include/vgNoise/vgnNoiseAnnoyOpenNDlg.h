#pragma once
#include <vgNoiseDLL/resource.h>


// vgnNoiseAnnoyOpenNDlg �Ի���
namespace vgNoise
{
		class vgnNoiseAnnoyOpenNDlg : public CDialog
		{
			DECLARE_DYNAMIC(vgnNoiseAnnoyOpenNDlg)

		public:
			vgnNoiseAnnoyOpenNDlg(CWnd* pParent = NULL);   // ��׼���캯��
			virtual ~vgnNoiseAnnoyOpenNDlg();
			BOOL m_AnnoyShowFlag;

		// �Ի�������
			enum { IDD = IDD_DIALOG_ANNOPEN };

		protected:
			virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

			DECLARE_MESSAGE_MAP()
		public:
			afx_msg void OnBnClickedButtonAnnoyopenok();
		};
}
