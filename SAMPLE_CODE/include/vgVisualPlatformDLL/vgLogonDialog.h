#pragma once

#include "res.h"
#include "afxwin.h"
#include "afxcmn.h"


#include <string>

using std::string;

// vgLogonDialog �Ի���

class vgLogonDialog : public CDialog
{
	DECLARE_DYNAMIC(vgLogonDialog)

public:
	vgLogonDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~vgLogonDialog();

// �Ի�������
	enum { IDD = IDD_DIALOG_LOGON };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL OnInitDialog();

	string getVgSceneFullPath() { return m_sceneVgPath; }

public:
	afx_msg void OnBnClickedConnectServer();

protected:
	CComboBox m_sceneCombo;
	CString m_user;

public:
	CString m_passWord;
	CString m_userName;
	CIPAddressCtrl m_ipControl;

	string m_scenePath;

	string m_sceneVgPath;

	afx_msg void OnBnClickedOk();

	bool m_bLoading;

protected:
	CString m_status;
};
