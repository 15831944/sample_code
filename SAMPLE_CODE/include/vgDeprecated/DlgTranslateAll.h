#pragma once

#include "Resource.h"
// CDlgTranslateAll �Ի���

class  VGDEP_EXPORT CDlgTranslateAll : public CDialog
{
	DECLARE_DYNAMIC(CDlgTranslateAll)

public:
	CDlgTranslateAll(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgTranslateAll();

// �Ի�������
	enum { IDD = IDD_TRANSALL };


	vgKernel::Vec3 getTranslateVec() 
	{
		return vgKernel::Vec3( m_fltTransX , m_fltTransY , m_fltTransZ );
	}

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
private:
	float m_fltTransX;
	float m_fltTransY;
	float m_fltTransZ;
	afx_msg void OnBnClickedOk();
public:
	float m_tranX;
};
