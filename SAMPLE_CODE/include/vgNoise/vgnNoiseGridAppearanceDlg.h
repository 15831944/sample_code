#pragma once

#include <vgNoiseDLL/resource.h>
// NoiseGridAppearance �Ի���
namespace vgNoise
{
	class NoiseGridAppearanceDlg : public CDialog
	{
		DECLARE_DYNAMIC(NoiseGridAppearanceDlg)

	public:
		NoiseGridAppearanceDlg(CWnd* pParent = NULL);   // ��׼���캯��
		virtual ~NoiseGridAppearanceDlg();
		int m_MapShow;
		int m_GridShow;
		BOOL m_GridShowFlag;
	// �Ի�������
		enum { IDD = IDD_GRID_APPEARANCE };

	protected:
		virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

		DECLARE_MESSAGE_MAP()
	public:
		afx_msg void OnBnClickedAppearanceok();
	};
}

