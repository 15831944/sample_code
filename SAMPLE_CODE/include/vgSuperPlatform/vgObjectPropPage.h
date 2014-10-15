#pragma once
#include "vgBasedTabPage.h"
#include "vgBaseEntry.h"
#include "Resource.h"

// vgObjectPropPage �Ի���

class vgObjectPropPage : public CDialog, public vgBasedTabPage
{
	DECLARE_DYNAMIC(vgObjectPropPage)

public:
	vgObjectPropPage(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~vgObjectPropPage();

	// extends from vgBasedTabPage
	string GetTitle();
	CWnd* GetWnd();
	void ConnectNode(vgBaseEntry* node){ m_pNode = node; };
		 
	void AdjustLayout();
	virtual LRESULT OnPropertyChanged(WPARAM w, LPARAM l);

// �Ի�������
	enum { IDD = NIDD_OBJECT_PROP };

protected:
	void InitPropList();

	CBCGPPropList		m_wndPropList;

	vgBaseEntry*		m_pNode;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL DestroyWindow();
};
