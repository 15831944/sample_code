#pragma once

#include "resource.h"

// CIPDlg �Ի���

class  VGDEP_EXPORT CIPDlg : public CDialog
{
	DECLARE_DYNAMIC(CIPDlg)

public:
	CIPDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CIPDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_IP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	BYTE IP1;
	BYTE IP2;
	BYTE IP3;
	BYTE IP4;
	afx_msg void OnBnClickedOk();
};
