#include <vgStableHeaders.h>
#include "vgUIController.h"
#include "vgObjectPropPage.h"
#include "vgTestNode1TabOne.h"
#include "vgObjectPropPage.h"
#include "vgTestNode1PropertiesTab.h"
#include "vgRootTabPage.h"

#include <vgMesh/vgmMeshManager.h>
#include <vgKernel/vgkInputCodeDef.h>


#include "vgFogEntry.h"
#include "vgRootEntry.h"
#include "vgCamMgrEntry.h"
#include "vgSoundBackgroundEntry.h"

#include <vgOcc/vgoOcclusion.h>
#include "vgFrustrumEntry.h"
#include "vgTerrainEntry.h"
#include "vgBackgroundEntry.h"

#include <vgQuadtreeSceneManager.h>
#include <vgSound/vgSoundManager.h>


vgUIController* vgUIController::s_pInstance = NULL;

vgUIController* vgUIController::GetInstance()
{
	if (s_pInstance == NULL)
		s_pInstance = new vgUIController;

	return s_pInstance;
}

void vgUIController::DestoryInstance()
{
	delete s_pInstance;

	s_pInstance = NULL;
}

vgUIController::vgUIController(void)
{	
	m_pTabView = new vgPropertiesViewBar();

	m_pWorkSpaceBar = new vgWorkSpaceBar();

	m_pOutputBar = new vgOutputBar();

	m_wndStatusBar = new CBCGPStatusBar;

	//	m_listTabPage.removeAll(); 

	// �ڴ˹���������Ҫʹ�õ�Tabҳ
	// �û������µ�Tabҳ����Ҫ�ڴ���ӳ�ʼ��
	m_listTabPage[TAB_ROOT_TAB] = new vgRootTabPage();
	//m_listTabPage[TAB_TESTNODE_PROP] = new PropertiesTab();
	//m_listTabPage[TAB_TESTNODE_CTRL] = new TabOne();
	//m_listTabPage[TAB_OBJECT_PROP] = new vgObjectPropPage();
	//m_listTabPage[TAB_PROPERTIES_PAGE] = new vgPropertyPage();

	m_propPage = new vgPropertyPage();
	
	this->SetHandleStatus(VG_INPUTMODE_UI);
	vgKernel::InputSystem::getSingleton().registerHandle(this);
}

vgUIController::~vgUIController(void)
{
	//m_pTabView->RemoveAllTab();
	
	delete m_propPage;

	for (int i=0; i<TAB_ARRAY_SIZ; i++)
	{
		delete m_listTabPage[i];
	}

	// double delete test
	if (m_pTabView != NULL)
	{
		delete(m_pTabView);
		m_pTabView = NULL;
	}

	if (m_pWorkSpaceBar != NULL)
	{
		delete m_pWorkSpaceBar;
		m_pWorkSpaceBar = NULL;
	}

	if (m_wndStatusBar != NULL)
	{
		delete m_wndStatusBar;
		m_wndStatusBar = NULL;
	}

	vgKernel::InputSystem::getSingleton().removeHandle(this);
}

vgBasedTabPage* vgUIController::GetTabPage(long ID)
{
	return m_listTabPage[ID];
}

vgPropertyPage* vgUIController::GetPropPage()
{
	return m_propPage;
}

vgPropertiesViewBar* vgUIController::GetPropertiesViewBar()
{
	return m_pTabView;
}

vgWorkSpaceBar* vgUIController::GetWorkSpaceBar()
{
	return m_pWorkSpaceBar;
}

void vgUIController::AddTabPage(string pageName, CWnd* page)
{
	m_pTabView->AddTab(pageName, page);

	return ;
}

void vgUIController::RemoveAllPages()
{
	m_pTabView->RemoveAllTab();
}

void vgUIController::OnChar(int keyCode)
{
	//if (keyCode == vgKernel::InputCode::DEL)
	//{
	//	//TRACE("Delete pressed \n");
	//	m_pWorkSpaceBar->DeleteSelectedNode();
	//}
	switch(keyCode)
	{
	case vgKernel::InputCode::DEL:
		m_pWorkSpaceBar->DeleteSelectedNode(); 
		break;

	case vgKernel::InputCode::TAB:
		vgCore::QuadtreeSceneManager::getSingleton().vgCurrentMode = 1;
		break;
	default:
		break; 
	}
}


// ��ʼ��������
bool vgUIController::InitUIControl()
{
	// ��0��
#if VGK_LANG_CHS
	vgBaseEntry*	node = new vgRootEntry("����");
#else
	vgBaseEntry*	node = new vgRootEntry("Scene");
#endif
	m_hRootNode = vgUIController::GetInstance()->AddNode(node);

	node = new vgCoordinateEntry();//2008.9.11
	vgUIController::GetInstance()->AddNode( m_hRootNode, node );

	//zhu
	node = new vgFrustrumEntry();//2008.9.27
	vgUIController::GetInstance()->AddNode( m_hRootNode, node );

	// ��һ��
#if VGK_LANG_CHS
	node = new vgRootEntry("ģ��");
	m_hRootModel = vgUIController::GetInstance()->AddNode(node);

	node = new vgRootEntry("���");
	m_hRootCamera = vgUIController::GetInstance()->AddNode(m_hRootNode, node);

	node = new vgRootEntry("��Ч");
	m_hRootSpecial = vgUIController::GetInstance()->AddNode(node);

	node = new vgRootEntry("����");
	m_hRootMoving = vgUIController::GetInstance()->AddNode(node);

	node = new vgRootEntry("��Ч");
	m_hRootSound = vgUIController::GetInstance()->AddNode(m_hRootNode, node);

	node = new vgRootEntry("ר�����");
	m_hRootTopic = vgUIController::GetInstance()->AddNode(m_hRootNode, node);
#else
	node = new vgRootEntry("Model");
	m_hRootModel = vgUIController::GetInstance()->AddNode(node);

	node = new vgRootEntry("Camera");
	m_hRootCamera = vgUIController::GetInstance()->AddNode(m_hRootNode, node);

	node = new vgRootEntry("Effects");
	m_hRootSpecial = vgUIController::GetInstance()->AddNode(node);

	node = new vgRootEntry("Animation");
	m_hRootMoving = vgUIController::GetInstance()->AddNode(node);

	node = new vgRootEntry("Sound");
	m_hRootSound = vgUIController::GetInstance()->AddNode(m_hRootNode, node);

	node = new vgRootEntry("Subject");
	m_hRootTopic = vgUIController::GetInstance()->AddNode(m_hRootNode, node);

#endif

	// �ڶ���
	/// ���
	node = new vgCamMgrEntry();
	HTREEITEM hCam = vgUIController::GetInstance()->AddNode(m_hRootCamera, node);
	if (hCam != NULL)
	{
		GetWorkSpaceBar()->Expand(hCam );
		GetWorkSpaceBar()->EnsureVisible(hCam);
	}	
	
#if VGK_LANG_CHS
	node = new vgRootEntry("�������");
	m_hSubStaticCamera = vgUIController::GetInstance()->AddNode(m_hRootCamera, node);
#else
	node = new vgRootEntry("Static Camera");
	m_hSubStaticCamera = vgUIController::GetInstance()->AddNode(m_hRootCamera, node);

#endif

	/// ��Ч
	node = new vgSoundBackgroundEntry(&vgCore::SoundManager::getSingleton().getCurrentBackgroundMusic() );
	HTREEITEM hSnd = vgUIController::GetInstance()->AddNode(m_hRootSound, node);
	if (hSnd != NULL)
	{
		GetWorkSpaceBar()->Expand(hSnd );
		GetWorkSpaceBar()->EnsureVisible(hSnd);
	}	

#if VGK_LANG_CHS
	node = new vgRootEntry("������Դ");
	m_hSub3DSound = vgUIController::GetInstance()->AddNode(m_hRootSound, node);
#else
	node = new vgRootEntry("Stereo Sound");
	m_hSub3DSound = vgUIController::GetInstance()->AddNode(m_hRootSound, node);

#endif

	node = new vgTerrainEntry();
	m_hTerainNode = vgUIController::GetInstance()->AddNode(node);

	node = new vgBackgroundEntry();
	m_hTerainNode = vgUIController::GetInstance()->AddNode(node);

	node = new vgFogEntry();
	m_hTerainNode = vgUIController::GetInstance()->AddNode(node);

	/// ��Ч
#if VGK_LANG_CHS
	node = new vgRootEntry("ֲ��");
	m_hSubBillboard = vgUIController::GetInstance()->AddNode(m_hRootSpecial, node);

	node = new vgRootEntry("��Ȫ");
	m_hSubFountain = vgUIController::GetInstance()->AddNode(m_hRootSpecial, node);

	node = new vgRootEntry("��");
	m_hSubFire = vgUIController::GetInstance()->AddNode(m_hRootSpecial, node);

	node = new vgRootEntry("��");
	m_hSubSmoke = vgUIController::GetInstance()->AddNode(m_hRootSpecial, node);

	node = new vgRootEntry("����");
	m_hSubFlag = vgUIController::GetInstance()->AddNode(m_hRootSpecial, node);

	node = new vgRootEntry("ˮ");
	m_hSubWater = vgUIController::GetInstance()->AddNode(m_hRootSpecial, node);

	node = new vgRootEntry("����");
	m_hSubGlass = vgUIController::GetInstance()->AddNode(m_hRootSpecial, node);

	node = new vgRootEntry("����");
	m_hSubMirror = vgUIController::GetInstance()->AddNode(m_hRootSpecial, node);

	node = new vgRootEntry("����");
	m_hSubMetal = vgUIController::GetInstance()->AddNode(m_hRootSpecial, node);

	node = new vgRootEntry("��������");
	m_hSubFloor = vgUIController::GetInstance()->AddNode(m_hRootSpecial, node);

	node = new vgRootEntry("AVI��Ƶ");
	m_hSubVideotexture = vgUIController::GetInstance()->AddNode(m_hRootSpecial, node);
	
	node = new vgRootEntry("��պ�");
	m_hSubMoveskybox = vgUIController::GetInstance()->AddNode(m_hRootSpecial, node);

	node = new vgRootEntry("�Զ���ģ");
	m_hSubAutobuild = vgUIController::GetInstance()->AddNode(m_hRootSpecial, node);
	
	node = new vgRootEntry("������");
	m_hSubTrigger = vgUIController::GetInstance()->AddNode(m_hRootSpecial, node);

	node = new vgRootEntry("��̬����");
	m_hSubDynamictex = vgUIController::GetInstance()->AddNode(m_hRootSpecial, node);

	/// ����
	node = new vgRootEntry("�ؼ�֡");
	m_hSubKeyframe = vgUIController::GetInstance()->AddNode(m_hRootMoving, node);

	node = new vgRootEntry("����");
	m_hSubSkeleton = vgUIController::GetInstance()->AddNode(m_hRootMoving, node);

	node = new vgRootEntry("����");
	m_hSubAnimation = vgUIController::GetInstance()->AddNode(m_hRootMoving, node);

	node = new vgRootEntry("VGMģ��");
	m_hSubVGM = vgUIController::GetInstance()->AddNode(m_hRootModel, node);

	node = new vgRootEntry("MODģ��");
	m_hSubMOD = vgUIController::GetInstance()->AddNode(m_hRootModel, node);
	
	
	/// ר��
// 	node = new vgRootEntry("�滮����");
// 	m_hCityplan = vgUIController::GetInstance()->AddNode( m_hRootTopic, node );

	node = new vgRootEntry("��·ƥ��");
	m_hSubROAD = vgUIController::GetInstance()->AddNode(m_hRootTopic, node);//----- [9/23/2008 YX]

	node = new vgRootEntry("ʸ������");
	m_hSubVecString = vgUIController::GetInstance()->AddNode(m_hRootNode, node);
#else
	node = new vgRootEntry("Grass");
	m_hSubBillboard = vgUIController::GetInstance()->AddNode(m_hRootSpecial, node);

	node = new vgRootEntry("Fountain");
	m_hSubFountain = vgUIController::GetInstance()->AddNode(m_hRootSpecial, node);

	node = new vgRootEntry("Fire");
	m_hSubFire = vgUIController::GetInstance()->AddNode(m_hRootSpecial, node);

	node = new vgRootEntry("Fireworks");
	m_hSubSmoke = vgUIController::GetInstance()->AddNode(m_hRootSpecial, node);

	node = new vgRootEntry("Flag");
	m_hSubFlag = vgUIController::GetInstance()->AddNode(m_hRootSpecial, node);

	node = new vgRootEntry("Water");
	m_hSubWater = vgUIController::GetInstance()->AddNode(m_hRootSpecial, node);

	node = new vgRootEntry("Glass");
	m_hSubGlass = vgUIController::GetInstance()->AddNode(m_hRootSpecial, node);

	node = new vgRootEntry("Mirror");
	m_hSubMirror = vgUIController::GetInstance()->AddNode(m_hRootSpecial, node);

	node = new vgRootEntry("Metal");
	m_hSubMetal = vgUIController::GetInstance()->AddNode(m_hRootSpecial, node);

	node = new vgRootEntry("Bright floor");
	m_hSubFloor = vgUIController::GetInstance()->AddNode(m_hRootSpecial, node);

	node = new vgRootEntry("AVI");
	m_hSubVideotexture = vgUIController::GetInstance()->AddNode(m_hRootSpecial, node);

	node = new vgRootEntry("Skybox");
	m_hSubMoveskybox = vgUIController::GetInstance()->AddNode(m_hRootSpecial, node);

	node = new vgRootEntry("Automate model");
	m_hSubAutobuild = vgUIController::GetInstance()->AddNode(m_hRootSpecial, node);
	
	node = new vgRootEntry("Trigger");
	m_hSubTrigger = vgUIController::GetInstance()->AddNode(m_hRootSpecial, node);
	
	
	/// ����
	node = new vgRootEntry("Keyframe");
	m_hSubKeyframe = vgUIController::GetInstance()->AddNode(m_hRootMoving, node);

	node = new vgRootEntry("Skeleton");
	m_hSubSkeleton = vgUIController::GetInstance()->AddNode(m_hRootMoving, node);

	node = new vgRootEntry("Roam");
	m_hSubAnimation = vgUIController::GetInstance()->AddNode(m_hRootMoving, node);

	node = new vgRootEntry("VGM");
	m_hSubVGM = vgUIController::GetInstance()->AddNode(m_hRootModel, node);

	node = new vgRootEntry("MOD");
	m_hSubMOD = vgUIController::GetInstance()->AddNode(m_hRootModel, node);


	/// ר��
	node = new vgRootEntry("Subject");
	m_hCityplan = vgUIController::GetInstance()->AddNode( m_hRootTopic, node );

	node = new vgRootEntry("Road");
	m_hSubROAD = vgUIController::GetInstance()->AddNode(m_hRootTopic, node);//----- [9/23/2008 YX]

	node = new vgRootEntry("Vector");
	m_hSubVecString = vgUIController::GetInstance()->AddNode(m_hRootNode, node);

#endif


#if VGK_LANG_CHS
	// ʸ��ͼ�νڵ��0��------------------------------------------------------------
	node = new vgRootEntry("ʸ��ͼ��");
	m_hRootVector = vgUIController::GetInstance()->AddNode(m_hRootNode, node);//AddNode(node, 2);

	/// ʸ��ͼ�νڵ��1��
	node = new vgRootEntry("ShapeFile");
	m_hSubShape = vgUIController::GetInstance()->AddNode(m_hRootVector, node);

	node = new vgRootEntry("����");
	m_hSubShapePoint = vgUIController::GetInstance()->AddNode(m_hSubShape, node);

	node = new vgRootEntry("����");
	m_hSubShapeLine = vgUIController::GetInstance()->AddNode(m_hSubShape, node);

	node = new vgRootEntry("����");
	m_hSubShapePolygon = vgUIController::GetInstance()->AddNode(m_hSubShape, node);

	node = new vgRootEntry("����");
	m_hSubShapeUnknow = vgUIController::GetInstance()->AddNode(m_hSubShape, node);
#else
	// ʸ��ͼ�νڵ��0��------------------------------------------------------------
	node = new vgRootEntry("Vector");
	m_hRootVector = vgUIController::GetInstance()->AddNode(m_hRootNode, node);//AddNode(node, 2);

	/// ʸ��ͼ�νڵ��1��
	node = new vgRootEntry("ShapeFile");
	m_hSubShape = vgUIController::GetInstance()->AddNode(m_hRootVector, node);

	node = new vgRootEntry("Point");
	m_hSubShapePoint = vgUIController::GetInstance()->AddNode(m_hSubShape, node);

	node = new vgRootEntry("Line");
	m_hSubShapeLine = vgUIController::GetInstance()->AddNode(m_hSubShape, node);

	node = new vgRootEntry("Block");
	m_hSubShapePolygon = vgUIController::GetInstance()->AddNode(m_hSubShape, node);

	node = new vgRootEntry("Other");
	m_hSubShapeUnknow = vgUIController::GetInstance()->AddNode(m_hSubShape, node);

#endif

	// �Ĳ�����ͼ��0��
	//node = new vgRootEntry("�Ĳ�����ͼ");
	//m_hRootQuadtree = vgUIController::GetInstance()->AddNode(node, 2);
	return true;
}

//new Status Bar ===============================================================

void vgUIController::EnableProgressBar()
{
	m_wndStatusBar->EnablePaneProgressBar(nStatusProgress, 100);
}

void vgUIController::DisableProgressBar()
{
	m_wndStatusBar->EnablePaneProgressBar(nStatusProgress, -1);
}

void vgUIController::SetProgress(UINT cnt)
{
	m_wndStatusBar->SetPaneProgress(nStatusProgress, cnt);
}

void vgUIController::SetFrameSpeed(UINT speed)
{
	char strSpeed[24];

	sprintf_s(strSpeed, 24, "%d", speed);

	m_wndStatusBar->SetPaneText(vgUIController::nStatusFrame, strSpeed);

	if (speed > 30)
	{
		m_wndStatusBar->SetPaneTextColor(nStatusFrame, GREEN);
	}
	else if (speed > 20)
	{
		m_wndStatusBar->SetPaneTextColor(nStatusFrame, LIGHTBLUE);
	}
	else
	{
		m_wndStatusBar->SetPaneTextColor(nStatusFrame, RED);
	}
}

void vgUIController::SetNewOccCnt(UINT tcnt,UINT rcnt,UINT cnt)
{

	//return ;
	char strCnt[24];

	if (vgOcclusion::vgOcclusionManager::getSingleton().GetOccState())
		//sprintf_s(strCnt, 24, "%u-%u=%u",tcnt,cnt,rcnt );
		sprintf_s(strCnt, 24, "Occ ON");
	else
		sprintf_s(strCnt, 24, "Occ OFF");

	m_wndStatusBar->SetPaneText(vgUIController::nStatusOcc, strCnt);

	if (cnt > 30)
	{
		m_wndStatusBar->SetPaneTextColor(nStatusOcc, GREEN);
	}
	else if (cnt > 20)
	{
		m_wndStatusBar->SetPaneTextColor(nStatusOcc, LIGHTBLUE);
	}
	else
	{
		m_wndStatusBar->SetPaneTextColor(nStatusOcc, RED);
	}
}

HTREEITEM& vgUIController::GetRootGpu( vgCore::IMAGETYPE EFFECT_IMAGE_TYPE )
{
	switch( EFFECT_IMAGE_TYPE )
	{
	case vgCore::IMG_WATER:
		return m_hSubWater;
		break;
	case vgCore::IMG_GLASS:
		return m_hSubGlass;
		break;
	case vgCore::IMG_METAL:
		return m_hSubMetal;
		break;
	case vgCore::IMG_MIRROR:
		return m_hSubMirror;
		break;
	case vgCore::IMG_BRIGHTFLOOR:
		return m_hSubFloor;
		break;
	case vgCore::IMG_SKYBOX:
		return m_hSubMoveskybox;
		break;
	case vgCore::IMG_AVINODE:
		return m_hSubVideotexture;
		break;
	case vgCore::IMG_AUTOBUILD:
		return m_hSubAutobuild;
		break;
	case vgCore::IMG_TRIGGER:
		return m_hSubTrigger;
		break;
	case vgCore::IMG_DYNAMICTEX:
		return m_hSubDynamictex;
		break;

	default:
		return m_hSubWater;
		break;
	}
}
//----------------------------------------------------------------
bool vgUIController::addEntryToRootByName( const String& root_name , 
										  vgBaseEntry* entry , const bool& add_if_need /*= true */ )
{
	TreeItemMap::iterator findres = _rootItems.find( root_name );

	if ( findres == _rootItems.end() )
	{
		if ( add_if_need )
		{
			vgBaseEntry* base = new vgRootEntry( root_name );
			HTREEITEM hitem = AddNode( base );

			TreeItemMapInsertRes ins = 
				_rootItems.insert( std::make_pair( root_name , HTreeItemWrap( hitem ) ));

			assert( ins.second == true );

			findres = ins.first;
		}
		else
		{
			return false;
		}
	}

	HTREEITEM itm = findres->second._hTreeItem;

	AddNode( itm , entry );

	if ( itm != NULL )
	{
		GetWorkSpaceBar()->Expand( itm  );
		GetWorkSpaceBar()->EnsureVisible( itm );
	}
	
	return true;
}