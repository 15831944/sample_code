#pragma once

#include "resource.h"
#include "afxwin.h"
#include <vgPlan/vgPlanscene.h>

// DlgPlanOutput �Ի���

class DlgPlanOutput : public CDialog
{
	DECLARE_DYNAMIC(DlgPlanOutput)

public:
	DlgPlanOutput(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~DlgPlanOutput();

// �Ի�������
	enum { IDD = IDD_PLAN_OUTPUT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

public:
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
private:
	CComboBox m_floorComboBox;

	vgPlan::BlockParam m_blockParam;

	int m_minFloor;
	int m_maxFloor;
public:
	afx_msg void OnCbnSelchangeFloor();
protected:
	virtual void OnCancel();
public:
	afx_msg void OnEnChangeEdit7();
private:
	int m_blockX;
	int m_blockY;
	int m_blockCnt;
	float m_floorAreaRadio;
public:
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio3();
	afx_msg void OnBnClickedRadio2();
private:
	CString m_schemName;
public:
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnBnClickedExpShp();
	afx_msg void OnEnChangeEdit1();
private:
	float m_red;
	float m_green;
public:
	float m_blue;
private:
	float m_alpha;
public:
	afx_msg void OnEnChangePlanColorr();
	afx_msg void OnEnChangePlanColorg();
	afx_msg void OnEnChangePlanColorb();
	afx_msg void OnEnChangePlanColoralpha();
};
