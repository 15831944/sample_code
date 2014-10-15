#pragma once

#include <vgGis/Resource.h>
#include <vgGIS/vggisDefinition.h>
// vggiDlgControlHeight �Ի���
namespace vgGIS3D
{
	class VGG_EXPORT vggiDlgControlHeight : public CDialog
	{
		DECLARE_DYNAMIC(vggiDlgControlHeight)

	public:
		vggiDlgControlHeight(CWnd* pParent = NULL);   // ��׼���캯��
		virtual ~vggiDlgControlHeight();

		// �Ի�������
		enum { IDD = IDD_DLG_CONTROL_HEIGHT };

	protected:
		virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

		DECLARE_MESSAGE_MAP()
	public:
		float fControlHeight;
		float getControlHeight();
		void  setControlHeight( float fht );
		virtual BOOL OnInitDialog();
	};
}
