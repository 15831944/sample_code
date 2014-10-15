#pragma once

#include "vgBasedTabPage.h"
#include "vgBasedNode.h"

// PropertiesTab �Ի���
// ���Զ���PropertiesTab�̳���vgBasedTabPage
class PropertiesTab : public CDialog, public vgBasedTabPage
{
	DECLARE_DYNAMIC(PropertiesTab)

public:
	PropertiesTab(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~PropertiesTab();

	string GetTitle();
	CWnd* GetWnd();

	void AdjustLayout();
	void ConnectNode(vgBasedNode* node) { m_pNode = node; }

	virtual LRESULT OnPropertyChanged(WPARAM w, LPARAM l);

	
// �Ի�������
	enum { IDD = IDD_PROP };

protected:
	void InitPropList();

	CBCGPPropList		m_wndPropList;

	vgBasedNode*		m_pNode;
	
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
