// MyThread.cpp : ʵ���ļ�
//

#include <vgStableHeaders.h>
#include "vgAutoLoadThread.h"
// UILoadThread


IMPLEMENT_DYNCREATE(CUILoadThread, CWinThread)

CUILoadThread::CUILoadThread()
{
}

CUILoadThread::~CUILoadThread()
{
}

BOOL CUILoadThread::InitInstance()
{
	// TODO: �ڴ�ִ���������̳߳�ʼ��
	dlg.DoModal();

	return TRUE;
}

int CUILoadThread::ExitInstance()
{
	// TODO: �ڴ�ִ���������߳�����

	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CUILoadThread, CWinThread)
	ON_THREAD_MESSAGE( WM_DESTROY, OnDestroy)
END_MESSAGE_MAP()


// MyThread ��Ϣ�������
void CUILoadThread::OnDestroy(WPARAM wParam, LPARAM lParam)
{
	HWND hWnd = dlg.GetSafeHwnd();
	SendMessage(hWnd,WM_CLOSE,0,0);
}