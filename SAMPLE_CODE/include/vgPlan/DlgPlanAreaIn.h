#pragma once

#include "resource.h"
// DlgPlanAreaIn �Ի���

namespace vgPlan
{

	class DlgPlanAreaIn : public CDialog
	{
		DECLARE_DYNAMIC(DlgPlanAreaIn)

	public:
		DlgPlanAreaIn(CWnd* pParent = NULL);   // ��׼���캯��
		virtual ~DlgPlanAreaIn();

		// �Ի�������
		enum { IDD = IDD_PLAN_AREAIN };

	public:
		string getSchemName()
		{
			return m_schemName.GetString();
		}
	protected:
		virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

		DECLARE_MESSAGE_MAP()
	private:
		CString m_schemName;
	};

}
