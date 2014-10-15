
#ifndef _UILoadThread_H
#define _UILoadThread_H

#include <vgDeprecatedMFCDLL/AutoLoadDialog.h>

// UILoadThread

class CUILoadThread : public CWinThread
{
	DECLARE_DYNCREATE(CUILoadThread)

protected:
	CUILoadThread();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CUILoadThread();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	afx_msg void OnDestroy(WPARAM wParam, LPARAM lParam);
protected:
	DECLARE_MESSAGE_MAP()

	CAutoLoadDialog dlg; 

};

#endif
