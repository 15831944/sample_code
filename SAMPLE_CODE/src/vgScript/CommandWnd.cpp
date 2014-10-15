// CommandWnd.cpp : implementation file
/*
** Created by Lesky Zhou 
*/

#include <vgStableHeaders.h>
#if 0
#include "vgScript/CommandWnd.h"

#include <vgScript/vgscScriptManager.h>
#include <vgKernel/vgkSelectManager.h>




// ���˵���ʾ��TEXT
std::string s_menuName = "�ű�ϵͳ(&T)";

// ÿһ���˵����¼�ID, ���ص�ʱ��̬�����¼�ID
const static UINT ID_SCRIPT_RUN_FILE		= ::RegisterWindowMessage( _T("ID_SCRIPT_RUN_FILE") )  ;
const static UINT ID_SCRIPT_RUN_CONSOLE	= ::RegisterWindowMessage( _T("ID_SCRIPT_RUN_CONSOLE") )  ;
const static UINT ID_SCRIPT_RUN_CONFIG		= ::RegisterWindowMessage( _T("ID_SCRIPT_RUN_CONFIG") )  ;

const static UINT ID_SCRIPT_REFRESH		= ::RegisterWindowMessage( _T("ID_SCRIPT_REFRESH") )  ;


IMPLEMENT_DYNAMIC(CCommandWnd, CWnd)
CCommandWnd::CCommandWnd()
{
}

CCommandWnd::~CCommandWnd()
{
}

BEGIN_MESSAGE_MAP(CCommandWnd, CWnd)
	ON_REGISTERED_MESSAGE( ID_SCRIPT_RUN_FILE, OnScriptRunFile ) 
	ON_REGISTERED_MESSAGE( ID_SCRIPT_RUN_CONSOLE, OnScriptRunConsole ) 
	ON_REGISTERED_MESSAGE( ID_SCRIPT_RUN_CONFIG, OnScriptRunConfig ) 
	ON_REGISTERED_MESSAGE( ID_SCRIPT_REFRESH, OnScriptRefresh ) 
END_MESSAGE_MAP()

LRESULT CCommandWnd::OnScriptRunFile( WPARAM wParam, LPARAM lParam )
{
	vgScript::ScriptManager::getSingleton().createMultiSelectDailogAndRunScript();
return 0;
}

LRESULT CCommandWnd::OnScriptRunConsole( WPARAM wParam, LPARAM lParam )
{
	vgScript::ScriptManager::getSingleton().runScriptConsole();

	return 0;
}

LRESULT CCommandWnd::OnScriptRunConfig( WPARAM wParam, LPARAM lParam )
{
	vgScript::ScriptManager::getSingleton().runUserScriptConfig();
	return 0;
}

LRESULT CCommandWnd::OnScriptRefresh( WPARAM wParam, LPARAM lParam )
{
	using namespace vgScript;


	ScriptManager::getSingleton().setUserDefinedScripts( 
		ScriptManager::getSingleton().getUserDefinedScripts() );

	return 0;
}


// ���մ˺���ʵ���û��Լ��صĲ˵�.
bool SetUserMenu(CMenu* SubMenu, UINT * NextMenuID)
{
	// ������ʾ�Ĳ˵���
	SubMenu->AppendMenu( MF_STRING, (UINT_PTR)(*NextMenuID), _T("���нű�(Run Script Fi&le)") ) ;
	// �¼�ID,��Ӧ�������Ӧ����
	s_CommandToMessage[*NextMenuID] = ID_SCRIPT_RUN_FILE;
	(*NextMenuID)++;


	SubMenu->AppendMenu( MF_STRING, (UINT_PTR)(*NextMenuID), _T("���п���̨(Run Script Consol&e)") ) ;
	s_CommandToMessage[*NextMenuID] = ID_SCRIPT_RUN_CONSOLE;
	(*NextMenuID)++;

	SubMenu->AppendMenu( MF_STRING, (UINT_PTR)(*NextMenuID), _T("�û��ű�����(User Script Confi&g)") ) ;
	s_CommandToMessage[*NextMenuID] = ID_SCRIPT_RUN_CONFIG;
	(*NextMenuID)++;

	SubMenu->AppendMenu( MF_STRING, (UINT_PTR)(*NextMenuID), _T("ˢ�½ű�(Re&fresh Script)") ) ;
	s_CommandToMessage[*NextMenuID] = ID_SCRIPT_REFRESH;
	(*NextMenuID)++;



	return true;
}

bool GetUserMenuCheck(UINT ID)
{
	if (ID == ID_SCRIPT_RUN_FILE)
	{
		//return (vgSolar::SolarManager::);
	}

	if (ID == ID_SCRIPT_RUN_CONSOLE)
	{
		//return (vgGIS3nalysisType());
	}

	if (ID == ID_SCRIPT_RUN_CONFIG)
	{
		//return (vgGIS3D::GisArea == vgGIS3D::GisManager::getSingleton().getAnalysisType());
	}
	


	// Ĭ��Check�ر�
	return false;
}

bool GetUserMenuEnable(UINT ID)
{
	if (ID == ID_SCRIPT_RUN_FILE)
	{
		//return (vgSolar::SolarManager::);
	}

	if (ID == ID_SCRIPT_RUN_CONSOLE)
	{
		//return (vgGIS3nalysisType());
	}

	if (ID == ID_SCRIPT_RUN_CONFIG)
	{
		//return (vgGIS3D::GisArea == vgGIS3D::GisManager::getSingleton().getAnalysisType());
	}



	// Ĭ�ϲ˵�����
	return true;
}
#endif