#pragma once
#include "resource.h"
#include <vgAutoBuild/vgAutoBuildNode.h>

// vgDlgAutoBuild �Ի���

class  VGDEP_EXPORT vgDlgAutoBuild : public CDialog
{
	DECLARE_DYNAMIC(vgDlgAutoBuild)

public:
	vgDlgAutoBuild(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~vgDlgAutoBuild();

// �Ի�������
	enum { IDD = IDD_AUTOBUILD_DLG };

public:
	float GetUnit()
	{
		return m_unit;
	}

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
	float m_unit;
};
