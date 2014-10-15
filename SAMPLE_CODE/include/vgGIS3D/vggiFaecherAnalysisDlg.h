#pragma once
#include "afxwin.h"
#include <vgGIS/resource.h>
#include <vgKernel/vgkSelectManager.h>
#include <vgKernel/vgkRendererManager.h>
//#include <deque>


// FaecherAnalysisDlg �Ի���

class FaecherAnalysisDlg : public CDialog
{
	DECLARE_DYNAMIC(FaecherAnalysisDlg)

public:
	FaecherAnalysisDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~FaecherAnalysisDlg();

// �Ի�������
	enum { IDD = IDD_FAECHER_ANALYSIS_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
	CBCGPGridCtrl m_List;
	void ShowName(std::deque<String> NameQue, int columnNum);
	virtual BOOL OnInitDialog();
	std::deque<String> GetName(vgKernel::RendererQueue  que);
	CStatic m_static;
	CList <CBCGPGridItem*, CBCGPGridItem*> ItemList;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	vgKernel::RendererQueue m_IntersectQue;
	vgKernel::RendererQueue m_ContainQue;
	vgKernel::RendererQueue m_DisjointQue;
	virtual void OnOK();
	virtual void OnCancel();
};
