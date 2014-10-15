// vgTemplateSettingDlgDlg.h : ͷ�ļ�
//

#pragma once
#include "resource.h"

// CvgTemplateSettingDlgDlg �Ի���
class CvgTemplateSettingDlgDlg : public CDialog
{
// ����
public:
	CvgTemplateSettingDlgDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_TEMPLATESETTING_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	// ������ļ�Ŀ¼
	CString m_strDirIn;
	// ������ļ�Ŀ¼
	CString m_strDirOut;
	afx_msg void OnBnClickedButton1Executing();
	afx_msg void OnBnClickedButton2OpenPathIn();
	afx_msg void OnBnClickedButton3OpenPathOut();
};
