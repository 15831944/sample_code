#pragma once

#include "Resource.h"

// CDlgTriggerConfig �Ի���

class  VGDEP_EXPORT CDlgTriggerConfig : public CDialog
{
	DECLARE_DYNAMIC(CDlgTriggerConfig)

public:
	CDlgTriggerConfig(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgTriggerConfig();

	// �Ի�������
	enum { IDD = IDD_DLG_TRIGGER_CONFIG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
	float m_fltTransX;
	float m_fltTransY;
public:
	afx_msg void OnBnClickedOk();
protected:
	float m_fontSize;
};

#if 0
// CDlgTriggerConfig �Ի���

class CDlgTriggerConfig : public CDialog
{
	DECLARE_DYNAMIC(CDlgTriggerConfig)

public:
	CDlgTriggerConfig(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgTriggerConfig();

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
	afx_msg void OnBnClickedOk();
public:
	float m_tranX;
};
#endif