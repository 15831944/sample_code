#pragma once
#include <resource.h>

// vgVcrHeightDlg �Ի���

class  VGDEP_EXPORT vgVcrHeightDlg : public CDialog
{
	DECLARE_DYNAMIC(vgVcrHeightDlg)

public:
	vgVcrHeightDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~vgVcrHeightDlg();

// �Ի�������
	enum { IDD = IDD_CAMRECORD_HEIGHT };

public:
	float getTransHeight()
	{
		return m_translateHeight;
	}
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	float m_translateHeight;
};
