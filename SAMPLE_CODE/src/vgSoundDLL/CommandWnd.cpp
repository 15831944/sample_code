// CommandWnd.cpp : implementation file
/*
** Created by Lesky Zhou 
*/

#include <vgStableHeaders.h>

#if 0

#include "CommandWnd.h"

#include <vgKernel/vgkSystemUtility.h>

#include <vgSound/vgSoundManager.h>




// ���˵���ʾ��TEXT
std::string s_menuName = "��Ч(Sou&nd)";

// ÿһ���˵����¼�ID, ���ص�ʱ��̬�����¼�ID
const static UINT ID_3DSOUND_ADD		= ::RegisterWindowMessage( _T("ID_3DSOUND_ADD") )  ;
const static UINT ID_BACKSOUND_SET	    = ::RegisterWindowMessage( _T("ID_BACKSOUND_SET") )  ;



IMPLEMENT_DYNAMIC(CCommandWnd, CWnd)
CCommandWnd::CCommandWnd()
{
}

CCommandWnd::~CCommandWnd()
{
}

BEGIN_MESSAGE_MAP(CCommandWnd, CWnd)
	ON_REGISTERED_MESSAGE( ID_3DSOUND_ADD, On3dsoundAdd ) 
	ON_REGISTERED_MESSAGE( ID_BACKSOUND_SET, OnBacksoundSet ) 
END_MESSAGE_MAP()

// CCommandWnd message handlers
LRESULT CCommandWnd::On3dsoundAdd( WPARAM wParam, LPARAM lParam )
{
	vgKernel::StringVector strExts;
	strExts.push_back("wav");
	vgKernel::StringVector strVec = vgKernel::SystemUtility::getFilesOpenByDialog(strExts);

	vgSound::SoundManager::getSingleton().importFiles( strVec );

	return 0;
}

LRESULT CCommandWnd::OnBacksoundSet( WPARAM wParam, LPARAM lParam )
{
	vgKernel::StringVector strExts;
	strExts.push_back("mp3");
	vgKernel::StringVector strVec = vgKernel::SystemUtility::getFilesOpenByDialog(strExts);

	vgSound::SoundManager::getSingleton().importFiles( strVec );

	return 0;
}


// ���մ˺���ʵ���û��Լ��صĲ˵�.
bool SetUserMenu(CMenu* SubMenu, UINT * NextMenuID)
{
	// ������ʾ�Ĳ˵���
	SubMenu->AppendMenu( MF_STRING, (UINT_PTR)(*NextMenuID), _T("���������Ч(S&tereo Sound)") ) ;
	// �¼�ID,��Ӧ�������Ӧ����
	s_CommandToMessage[*NextMenuID] = ID_3DSOUND_ADD;
	(*NextMenuID)++;


	SubMenu->AppendMenu( MF_STRING, (UINT_PTR)(*NextMenuID), _T("��ӱ�����Ч(&Background Sound)") ) ;	
	s_CommandToMessage[*NextMenuID] = ID_BACKSOUND_SET;
	(*NextMenuID)++;	

	return true;
}

bool GetUserMenuCheck(UINT ID)
{
	// Ĭ��Check�ر�
	return false;
}

bool GetUserMenuEnable(UINT ID)
{

	// Ĭ�ϲ˵�����
	return true;
}

#endif
