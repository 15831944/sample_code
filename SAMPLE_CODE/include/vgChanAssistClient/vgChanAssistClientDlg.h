// ClientAssistantDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"
#include "BCGPGridCtrl.h"
#include <string>
#include <vector>
#include "afxwin.h"
using namespace std;
#include <vgConfig/vgcXMLConfigManager.h>
#include <vgKernel/vgkTrace.h>
#include "vgChanAssistClient/vgChanresource.h"

//#include "XMLConfigManager.h"

// CClientAssistantDlg �Ի���
class CClientAssistantDlg : public CDialog
{
// ����
public:
	CClientAssistantDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_CLIENTASSISTANT_DIALOG };

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
	bool is_visible;
	string m_broadCastAddress;
	UINT m_hotKeyID;
	CBCGPGridCheckItem* _FullScreen;
	CBCGPGridCheckItem* _startChannel;
	CBCGPGridCheckItem* _softBlend;		// ���ں�
	std::vector<CString> vec_str_config;

	CBCGPGridCtrl m_wndGrid;
	//vgConfig::XMLConfigManager *pxmlManager;

private:
	

public:
	afx_msg void readConfigFile_ini();	// Ŀǰʹ�õ��Ƕ�ȡXML �����ļ��ķ�ʽ,
	afx_msg void writeConfigFile_ini();	// ��ȡini �����ļ��ķ�ʽ��֮ǰд��,�����ܻ�����,û��ɾ��

	afx_msg void readConfigFile_XML();
	afx_msg void writeConfigFile_XML();

	afx_msg void write2Reg();

	BOOL startConnect();
	BOOL createRecvSocket(int RecvPort);

	afx_msg void OnShowDlg();
	afx_msg void m_AddRow();
	afx_msg void m_AddConfig();			// ԭ��д�Ķ�ȡini �����ļ���ʱ��Ҫ�õ��ģ�����û��ʹ��


	afx_msg void OnWindowPosChanging(WINDOWPOS* lpwndpos);
	afx_msg void OnBnClickedCancel();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedOk();
	CStatic m_DlgGridLocation;
	afx_msg void OnBnClickedBtnHide();
	afx_msg void OnBnClickedBtnAddrow();
	afx_msg void OnBnClickedBenDeleterow();
};

//////////////////////////////////////////////////////////////////////
// CFileItem Class

class  CFileItem : public CBCGPGridItem
{
	// Construction
public:
	CFileItem(const CString& strFileName);

	// Overrides
	virtual void OnClickButton (CPoint point);
};

//////////////////////////////////////////////////////////////////////////