#pragma once


// CdlgCutSuface �Ի���
#include "vgTerritoryResource.h"
#include "vgTerritory/CutSurfView.h"

class CdlgCutSuface : public CDialog
{
	DECLARE_DYNAMIC(CdlgCutSuface)

public:
	CdlgCutSuface(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CdlgCutSuface();

// �Ի�������
	enum { IDD = IDD_DLG_CUTSURFACE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	
	DECLARE_MESSAGE_MAP()
public:
	
	virtual BOOL OnInitDialog();
public:
	CutSurfView *pView;
	bool  m_bClosed;

	void setClosed( bool bClosed ) { m_bClosed = bClosed; }
	bool getClosed() { return m_bClosed; }

public:

	afx_msg void OnDestroy();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	CString sHeightValue;
	afx_msg void OnBnClickedOk();
};
