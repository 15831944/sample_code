/*
** Created by Lesky Zhou
** 
*/
#pragma once
#if 0
class CBCGPMenuBar;

typedef std::map<UINT, UINT>				CommandToMessage;


extern CWnd *								s_CommandWnd ;
extern CBCGPMenuBar*						s_pMenuBar;

extern CommandToMessage					s_CommandToMessage ;

// CCommandWnd ��Ӧ�����¼�
class CCommandWnd : public CWnd
{
	DECLARE_DYNAMIC(CCommandWnd)

public:
	CCommandWnd();
	virtual ~CCommandWnd();

protected:
	afx_msg LRESULT OnScriptRunFile( WPARAM wParam, LPARAM lParam ) ;
	afx_msg LRESULT OnScriptRunConsole( WPARAM wParam, LPARAM lParam ) ;
	afx_msg LRESULT OnScriptRunConfig( WPARAM wParam, LPARAM lParam ) ;

	afx_msg LRESULT OnScriptRefresh( WPARAM wParam, LPARAM lParam );

	DECLARE_MESSAGE_MAP()
};


#endif