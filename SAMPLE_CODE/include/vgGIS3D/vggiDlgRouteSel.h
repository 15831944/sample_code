#pragma once

#include <vgGISDLL/vgGisResource.h>
#include <vgGIS/vggisDefinition.h>

namespace vgGIS3D
{

	// vgDlgRouteSel �Ի���

	class VGG_EXPORT vgDlgRouteSel : public CDialog
	{
		DECLARE_DYNAMIC(vgDlgRouteSel)

	public:
		vgDlgRouteSel(CWnd* pParent = NULL);   // ��׼���캯��
		virtual ~vgDlgRouteSel();

		// �Ի�������
		enum { IDD = IDD_DLG_ROUTE_SEL };

	protected:
		virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

		DECLARE_MESSAGE_MAP()
	public:
		virtual BOOL OnInitDialog();
	};

}
