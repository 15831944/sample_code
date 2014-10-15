// CommandWnd.cpp : implementation file
/*
** Created by Lesky Zhou 
*/

#include <vgStableHeaders.h>
#include "vgDatabase/CommandWnd.h"
#if 0

#include <vgDatabase/vgdDatabaseManager.h>
#include <vgDatabase/vgDlgDBConnect.h>


// ���˵���ʾ��TEXT
std::string s_menuName = "���ݿ�(&Database)";
UINT s_menuPosition = -1;

// ÿһ���˵����¼�ID, ���ص�ʱ��̬�����¼�ID
const static UINT ID_DB_DLL_CONNECT		= ::RegisterWindowMessage( _T("ID_DB_DLL_CONNECT") );
const static UINT ID_DB_DLL_DISCONNECT  = ::RegisterWindowMessage( _T("ID_DB_DLL_DISCONNECT") );
const static UINT ID_DB_DLL_CONDITION   = ::RegisterWindowMessage( _T("ID_DB_DLL_CONDITION") );

IMPLEMENT_DYNAMIC(CCommandWnd, CWnd)
CCommandWnd::CCommandWnd()
{
	m_dbCondition = NULL;
}

CCommandWnd::~CCommandWnd()
{
}

BEGIN_MESSAGE_MAP(CCommandWnd, CWnd)
	ON_REGISTERED_MESSAGE( ID_DB_DLL_CONNECT, OnDbConnect ) 
	ON_REGISTERED_MESSAGE( ID_DB_DLL_DISCONNECT, OnDbDisconnect ) 
	ON_REGISTERED_MESSAGE( ID_DB_DLL_CONDITION, OnDbCondition ) 

 	ON_REGISTERED_MESSAGE( WM_DELETE_DLG, OnDelDlgMsg )
END_MESSAGE_MAP()

// CCommandWnd message handlers
LRESULT CCommandWnd::OnDbConnect( WPARAM wParam, LPARAM lParam )
{
	DBConnectDlg dialog;

	dialog.DoModal();

	return 0;
}

LRESULT CCommandWnd::OnDbDisconnect( WPARAM wParam, LPARAM lParam )
{
	if (! vgDatabase::DatabaseManager::getSingleton().CloseDatabase())
	{
		AfxMessageBox("���ݿ�δ���ӻ�ر�ʧ��!");
	}
	else
	{
		AfxMessageBox("�ѹر����ݿ�����!");
	}

	return 0;
}

LRESULT CCommandWnd::OnDbCondition( WPARAM wParam, LPARAM lParam )
{
	if (! vgDatabase::DatabaseManager::getSingleton().IsOpen())
	{
		DBConnectDlg dialog;

		dialog.DoModal();

		if (! vgDatabase::DatabaseManager::getSingleton().IsOpen())
		{
			return 0;
		}
	}

	if (!vgDatabase::ProfileManager::getSingleton().IsProfileLoaded())
	{
		AfxMessageBox("δ����profile�ļ������ȼ��ض�̬���ٴ򿪳���vg�ļ���");
		return 0;
	}

	if(m_dbCondition)
	{
		m_dbCondition->SetForegroundWindow();
	}
	else
	{
		m_dbCondition = new DBCondition(this);
		m_dbCondition->Create(DBCondition::IDD, GetDesktopWindow());
		m_dbCondition->ShowWindow(SW_SHOW);
	}

	return 0;
}

LRESULT  CCommandWnd::OnDelDlgMsg( WPARAM wParam, LPARAM lParam )
{
	delete m_dbCondition;
	m_dbCondition = NULL;

	return 0;
}


// ���մ˺���ʵ���û��Լ��صĲ˵�.
bool SetUserMenu(CMenu* SubMenu, UINT * NextMenuID)
{
	// ������ʾ�Ĳ˵���
	SubMenu->AppendMenu( MF_STRING, (UINT_PTR)(*NextMenuID), _T("�������ݿ�(&Link Database)") ) ;
	// �¼�ID,��Ӧ�������Ӧ����
	s_CommandToMessage[*NextMenuID] = ID_DB_DLL_CONNECT;
	(*NextMenuID)++;

	SubMenu->AppendMenu( MF_STRING, (UINT_PTR)(*NextMenuID), _T("�Ͽ����ݿ�(&Dislink Database)") ) ;
	s_CommandToMessage[*NextMenuID] = ID_DB_DLL_DISCONNECT;
	(*NextMenuID)++;

	SubMenu->AppendMenu( MF_STRING, (UINT_PTR)(*NextMenuID), _T("�Զ����ѯ(&Custom Query)") ) ;
	s_CommandToMessage[*NextMenuID] = ID_DB_DLL_CONDITION;
	(*NextMenuID)++;


	return true;
}

bool GetUserMenuCheck(UINT ID)
{
	if (ID == ID_DB_DLL_CONNECT)
	{
	}

	if (ID == ID_DB_DLL_DISCONNECT)
	{
	}

	if (ID == ID_DB_DLL_CONDITION)
	{
	}

	// Ĭ��Check�ر�
	return false;
}

bool GetUserMenuEnable(UINT ID)
{
	if (ID == ID_DB_DLL_CONNECT)
	{
	}

	if (ID == ID_DB_DLL_DISCONNECT)
	{
	}

	if (ID == ID_DB_DLL_CONDITION)
	{
	}

	// Ĭ�ϲ˵�����
	return true;
}
#endif