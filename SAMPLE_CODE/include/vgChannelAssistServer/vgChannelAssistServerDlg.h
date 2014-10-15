// channelAssistantDlg.h : ͷ�ļ�
//

#pragma once


// CchannelAssistantDlg �Ի���
class CchannelAssistantDlg : public CDialog
{
// ����
public:
	CchannelAssistantDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_CHANNELASSISTANT_DIALOG };

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
	afx_msg void OnBnClickedBtnStartChannel();
	afx_msg void OnBnClickedBtnEndChannel();
	afx_msg void OnBnClickedBtnReboot();
	afx_msg void OnBnClickedBtnShutdown();
	afx_msg void OnDestroy();
	CComboBox m_vgNameCombo;

public:
	void readvgName();
};
