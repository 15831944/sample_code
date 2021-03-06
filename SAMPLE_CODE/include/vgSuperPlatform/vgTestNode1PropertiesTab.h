#pragma once

#include "vgBasedTabPage.h"
#include "vgBaseEntry.h"


#if 0


// PropertiesTab 对话框
// 非自动化PropertiesTab继承自vgBasedTabPage
class PropertiesTab : public CDialog, public vgBasedTabPage
{
	DECLARE_DYNAMIC(PropertiesTab)

public:
	PropertiesTab(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~PropertiesTab();

	string GetTitle();
	CWnd* GetWnd();

	void AdjustLayout();
	void ConnectNode(vgBaseEntry* node) { m_pNode = node; }

	virtual LRESULT OnPropertyChanged(WPARAM w, LPARAM l);

	
// 对话框数据
	enum { IDD = NIDD_PROP };

protected:
	void InitPropList();

	CBCGPPropList		m_wndPropList;

	vgBaseEntry*		m_pNode;
	
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};


#endif