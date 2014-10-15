// CommandWnd.cpp : implementation file
/*
** Created by Lesky Zhou 
*/

#include <vgStableHeaders.h>
#if 0
#include "CommandWnd.h"

#include <vgKernel/vgkCoordSystem.h>
#include <vgTerritory/vgtTerrainManager.h>
#include <vgTerritory/vgtElevationFilter.h>
#include <vgTerritory/vgtTerrainCenterManager.h>

// ���˵���ʾ��TEXT
std::string s_menuName = "���η���(&T)";
UINT s_menuPosition = -1;

// ÿһ���˵����¼�ID, ���ص�ʱ��̬�����¼�ID
const static UINT ID_VTR_DLL_LOAD		= ::RegisterWindowMessage( _T("ID_VTR_DLL_LOAD") )  ;
const static UINT ID_VTR_DLL_BLEND		= ::RegisterWindowMessage( _T("ID_VTR_DLL_BLEND") )  ;
const static UINT ID_VTR_DLL_COLLISION	= ::RegisterWindowMessage( _T("ID_VTR_DLL_COLLISION") )  ;
const static UINT ID_VTR_DLL_RENDER		= ::RegisterWindowMessage( _T("ID_VTR_DLL_RENDER") )  ;
const static UINT ID_VTR_DLL_DROPDATA	= ::RegisterWindowMessage( _T("ID_VTR_DLL_DROPDATA"));
const static UINT ID_VTR_DLL_CUT_AlA1	= ::RegisterWindowMessage( _T("ID_VTR_DLL_CUT_AlA1"));
const static UINT ID_VTR_DLL_CUT_AlA2	= ::RegisterWindowMessage( _T("ID_VTR_DLL_CUT_AlA2"));
const static UINT ID_VTR_DLL_LEVELING	= ::RegisterWindowMessage( _T("ID_VTR_DLL_LEVELING"));
const static UINT ID_VTR_DLL_ATTACH		= ::RegisterWindowMessage( _T("ID_VTR_DLL_ATTACH"));
const static UINT ID_VTR_DLL_MAPORIGIN	= ::RegisterWindowMessage( _T("ID_VTR_DLL_MAPORIGIN"));
const static UINT ID_VTR_DLL_RESTORE	= ::RegisterWindowMessage( _T("ID_VTR_DLL_RESTORE"));
const static UINT ID_VTR_DLL_SAVEAS		= ::RegisterWindowMessage( _T("ID_VTR_DLL_SAVEAS"));


IMPLEMENT_DYNAMIC(CCommandWnd, CWnd)
CCommandWnd::CCommandWnd()
{
}

CCommandWnd::~CCommandWnd()
{
}

BEGIN_MESSAGE_MAP(CCommandWnd, CWnd)
	ON_REGISTERED_MESSAGE( ID_VTR_DLL_LOAD, OnVtrLoad ) 
	ON_REGISTERED_MESSAGE( ID_VTR_DLL_BLEND, OnVtrBlend ) 
	ON_REGISTERED_MESSAGE( ID_VTR_DLL_COLLISION, OnVtrCollision ) 
	ON_REGISTERED_MESSAGE( ID_VTR_DLL_RENDER, OnVtrRender ) 
	ON_REGISTERED_MESSAGE( ID_VTR_DLL_DROPDATA, OnVtrDropData )
	ON_REGISTERED_MESSAGE( ID_VTR_DLL_CUT_AlA1, OnVtrCutAla1 )
	ON_REGISTERED_MESSAGE( ID_VTR_DLL_CUT_AlA2, OnVtrCutAla2 )
	ON_REGISTERED_MESSAGE( ID_VTR_DLL_LEVELING, OnVtrLeveling )
	ON_REGISTERED_MESSAGE( ID_VTR_DLL_ATTACH, OnVtrAttach )
	ON_REGISTERED_MESSAGE( ID_VTR_DLL_MAPORIGIN, OnVtrMapOrigin )
	ON_REGISTERED_MESSAGE( ID_VTR_DLL_RESTORE, OnVtrRestore )
	ON_REGISTERED_MESSAGE( ID_VTR_DLL_SAVEAS, OnVtrSaveas )
END_MESSAGE_MAP()

// CCommandWnd message handlers
LRESULT CCommandWnd::OnVtrLoad( WPARAM wParam, LPARAM lParam )
{
	char szFilters[]=
		"vtr files(*.vtr)|*.vtr|";

	CFileDialog dlg (TRUE, "vtr", NULL,
		OFN_FILEMUSTEXIST| OFN_HIDEREADONLY, szFilters, this);


	std::string fileName;
	if( dlg.DoModal ()==IDOK )
	{
		 fileName = dlg.GetPathName();
		 vgTerritory::TerrainManager::getSingleton().openVtrFile(fileName);
	}

	return 0 ; 
}

LRESULT CCommandWnd::OnVtrBlend( WPARAM wParam, LPARAM lParam ) 
{
	vgTerritory::TerrainManager::getSingleton().setBlendFlag(
		! vgTerritory::TerrainManager::getSingleton().getBlendFlag());

	return 0 ; 
}

LRESULT CCommandWnd::OnVtrCollision( WPARAM wParam, LPARAM lParam ) 
{
	vgTerritory::TerrainManager::getSingleton().setCollisionDetectedFlag(
		! vgTerritory::TerrainManager::getSingleton().getCollisionDetectedFlag());

	return 0 ; 
}

LRESULT CCommandWnd::OnVtrRender( WPARAM wParam, LPARAM lParam ) 
{
	vgTerritory::TerrainManager::getSingleton().setRenderFlag(
		! vgTerritory::TerrainManager::getSingleton().getRenderFlag());

	return 0 ; 
}

LRESULT CCommandWnd::OnVtrDropData(WPARAM wParam, LPARAM lParam)
{
	vgTerritory::TerrainManager::getSingleton().dropTerrianData();

	return 0;
}

LRESULT CCommandWnd::OnVtrCutAla1(WPARAM wParam, LPARAM lParam)
{
	vgTerritory::TerrainManager::getSingleton().setCutSurfaceAnalysisFlag(
		! vgTerritory::TerrainManager::getSingleton().getCutSurfaceAnalysisFlag());
	
	return 0;
}

LRESULT CCommandWnd::OnVtrCutAla2(WPARAM wParam, LPARAM lParam)
{
	vgTerritory::TerrainManager::getSingleton().setCutSurfaceAnalysisFlag2(
		! vgTerritory::TerrainManager::getSingleton().getCutSurfaceAnalysisFlag2());

	return 0;
}

LRESULT CCommandWnd::OnVtrLeveling(WPARAM wParam, LPARAM lParam)
{

	vgTerritory::TerrainManager::getSingleton().setCutSurfaceLevellingFlag(
		! vgTerritory::TerrainManager::getSingleton().getCutSurfaceLevellingFlag());

	return 0;
}

LRESULT CCommandWnd::OnVtrAttach(WPARAM wParam, LPARAM lParam)
{
	return 0;
}

LRESULT CCommandWnd::OnVtrMapOrigin(WPARAM wParam, LPARAM lParam)
{
	vgTerritory::VtrHeaderPtr header = 
		vgTerritory::ElevationManager::getSingleton().getMainFileHandleVtr()->getHeader();

	if ( header.isNull() == true )
	{
		return NULL;
	}

	vgKernel::Vec3 coo( header->originX , header->originY , -header->originZ );

	vgKernel::CoordSystem::getSingleton().setProjectionCoord( coo );

	return 0;
}

LRESULT CCommandWnd::OnVtrRestore(WPARAM wParam, LPARAM lParam)
{
	vgTerritory::ElevationFilter::getSingleton().clearPolygons();

	vgTerritory::TerrainCenterManager::getSingleton().reloadAllCenters();

	return 0;
}

LRESULT CCommandWnd::OnVtrSaveas(WPARAM wParam, LPARAM lParam)
{
	vgTerritory::ElevationFilter::getSingleton().doFilterAndSave();

	return 0;
}


// ���մ˺���ʵ���û��Լ��صĲ˵�.
bool SetUserMenu(CMenu* SubMenu, UINT * NextMenuID)
{
	// ������ʾ�Ĳ˵���
	SubMenu->AppendMenu( MF_STRING, (UINT_PTR)(*NextMenuID), _T("�򿪵���(Open &Terrain)") ) ;
	// �¼�ID,��Ӧ�������Ӧ����
	s_CommandToMessage[*NextMenuID] = ID_VTR_DLL_LOAD;
	(*NextMenuID)++;

	SubMenu->AppendMenu( MF_SEPARATOR, 0, _T("") ) ;

	SubMenu->AppendMenu( MF_STRING, (UINT_PTR)(*NextMenuID), _T("��������͸��(Enable &Alpha Rendering)") ) ;
	s_CommandToMessage[*NextMenuID] = ID_VTR_DLL_BLEND;
	(*NextMenuID)++;

	SubMenu->AppendMenu( MF_STRING, (UINT_PTR)(*NextMenuID), _T("����������ײ(Collision Detec&t)") ) ;
	s_CommandToMessage[*NextMenuID] = ID_VTR_DLL_COLLISION;
	(*NextMenuID)++;

	SubMenu->AppendMenu( MF_STRING, (UINT_PTR)(*NextMenuID), _T("����������Ⱦ(Turn on R&endering)") ) ;
	s_CommandToMessage[*NextMenuID] = ID_VTR_DLL_RENDER;
	(*NextMenuID)++;


	SubMenu->AppendMenu( MF_STRING, (UINT_PTR)(*NextMenuID), _T("ж�ص�������(&Drop Terrain Data)") ) ;
	s_CommandToMessage[*NextMenuID] = ID_VTR_DLL_DROPDATA;
	(*NextMenuID)++;

	SubMenu->AppendMenu( MF_SEPARATOR, 0, _T("") ) ;

	SubMenu->AppendMenu( MF_STRING, (UINT_PTR)(*NextMenuID), _T("�������(�߶�����ģʽ)(&Cutting Surface Analysis)") ) ;
	s_CommandToMessage[*NextMenuID] = ID_VTR_DLL_CUT_AlA1;
	(*NextMenuID)++;

	SubMenu->AppendMenu( MF_STRING, (UINT_PTR)(*NextMenuID), _T("�������(�ռ����ģʽ)(C&utting Surface Analysis)") ) ;
	s_CommandToMessage[*NextMenuID] = ID_VTR_DLL_CUT_AlA2;
	(*NextMenuID)++;
	
	SubMenu->AppendMenu( MF_STRING, (UINT_PTR)(*NextMenuID), _T("������������(Terrain Leveling)") ) ;
	s_CommandToMessage[*NextMenuID] = ID_VTR_DLL_LEVELING;
	(*NextMenuID)++;

	SubMenu->AppendMenu( MF_SEPARATOR, 0, _T("") ) ;

	SubMenu->AppendMenu( MF_STRING, (UINT_PTR)(*NextMenuID), _T("ģ������(&Attach Objects)") ) ;
	s_CommandToMessage[*NextMenuID] = ID_VTR_DLL_ATTACH;
	(*NextMenuID)++;

	SubMenu->AppendMenu( MF_STRING, (UINT_PTR)(*NextMenuID), _T("����������ӳ�䵽ԭ��(&Mapping Coordinates)") ) ;
	s_CommandToMessage[*NextMenuID] = ID_VTR_DLL_MAPORIGIN;
	(*NextMenuID)++;

	SubMenu->AppendMenu( MF_SEPARATOR, 0, _T("") ) ;

	SubMenu->AppendMenu( MF_STRING, (UINT_PTR)(*NextMenuID), _T("�����޸Ļ�ԭ(Terrain Recovery)") ) ;
	s_CommandToMessage[*NextMenuID] = ID_VTR_DLL_RESTORE;
	(*NextMenuID)++;

	SubMenu->AppendMenu( MF_STRING, (UINT_PTR)(*NextMenuID), _T("�����޸ı���(Terrain &Save)") ) ;
	s_CommandToMessage[*NextMenuID] = ID_VTR_DLL_SAVEAS;
	(*NextMenuID)++;

	return true;
}

bool GetUserMenuCheck(UINT ID)
{
	if (ID == ID_VTR_DLL_RENDER)
	{
		return (vgTerritory::TerrainManager::getSingleton().getRenderFlag());
	}

	if (ID == ID_VTR_DLL_BLEND)
	{
		return (vgTerritory::TerrainManager::getSingleton().getBlendFlag());
	}

	if (ID == ID_VTR_DLL_COLLISION)
	{
		return (vgTerritory::TerrainManager::getSingleton().getCollisionDetectedFlag());
	}
	
	if (ID == ID_VTR_DLL_LEVELING)
	{
		return (vgTerritory::TerrainManager::getSingleton().getCutSurfaceLevellingFlag());
	}

	if (ID == ID_VTR_DLL_CUT_AlA1)
	{
		return (vgTerritory::TerrainManager::getSingleton().getCutSurfaceAnalysisFlag());
	}

	if (ID == ID_VTR_DLL_CUT_AlA2)
	{
		return (vgTerritory::TerrainManager::getSingleton().getCutSurfaceAnalysisFlag2());
	}
	
	if (ID == ID_VTR_DLL_LEVELING)
	{
		return (vgTerritory::TerrainManager::getSingleton().getCutSurfaceLevellingFlag());
	}

	// Ĭ��Check�ر�
	return false;
}

bool GetUserMenuEnable(UINT ID)
{
	if (ID == ID_VTR_DLL_RENDER)
	{
		return (vgTerritory::TerrainManager::getSingleton().hasTerrainData());
	}
	
	if (ID == ID_VTR_DLL_COLLISION)
	{
		return (vgTerritory::TerrainManager::getSingleton().hasTerrainData());
	}

	if (ID == ID_VTR_DLL_ATTACH)
	{
		return (vgTerritory::TerrainManager::getSingleton().hasTerrainData());
	}

	if (ID == ID_VTR_DLL_BLEND)
	{
		return (vgTerritory::TerrainManager::getSingleton().hasTerrainData());
	}

	if (ID == ID_VTR_DLL_LEVELING )
	{
		return (vgTerritory::TerrainManager::getSingleton().hasTerrainData());
	}

	if (ID == ID_VTR_DLL_CUT_AlA1)
	{
		return (vgTerritory::TerrainManager::getSingleton().hasTerrainData());
	}

	if (ID == ID_VTR_DLL_CUT_AlA2)
	{
		return (vgTerritory::TerrainManager::getSingleton().hasTerrainData());
	}

	if (ID == ID_VTR_DLL_RESTORE)
	{
		return (vgTerritory::TerrainManager::getSingleton().hasTerrainData());
	}

	if (ID == ID_VTR_DLL_SAVEAS)
	{
		return (vgTerritory::TerrainManager::getSingleton().hasTerrainData());
	}

	if (ID == ID_VTR_DLL_DROPDATA)
	{
		return (vgTerritory::TerrainManager::getSingleton().hasTerrainData());
	}

	if (ID == ID_VTR_DLL_MAPORIGIN)
	{
		return (vgTerritory::TerrainManager::getSingleton().hasTerrainData());
	}

	// Ĭ�ϲ˵�����
	return true;
}
#endif