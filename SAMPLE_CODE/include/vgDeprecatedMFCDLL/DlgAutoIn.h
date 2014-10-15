#pragma once

#include "resource.h"
// DlgAutoIn �Ի���

class  VGDEP_EXPORT DlgAutoIn : public CDialog
{
	DECLARE_DYNAMIC(DlgAutoIn)

public:
	DlgAutoIn(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~DlgAutoIn();

// �Ի�������
	enum { IDD = IDD_AUTOBUILD_IN };

	int getYear()
	{
		return m_years;
	}

	CString getSchem()
	{
		return m_schem;
	}

	int getSchemId()
	{
		if (m_schem == "GS")
		{
			return 0;
		}
		else if (m_schem == "LS")
		{
			return 1;
		}
		else if (m_schem == "NE")
		{
			return 2;
		}
		else
		{
			return 3;
		}
	}
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

	CString m_schem;
	int m_years;
};
