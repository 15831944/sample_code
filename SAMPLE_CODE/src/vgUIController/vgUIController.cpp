#include <vgStableHeaders.h>
#include <vgUIController/vgUIController.h>

#include <vgKernel/vgkSingletonLifeTimeDefine.h>

#include <vgUIController/vgRootTabPage.h>

#include <vgKernel/vgkInputCodeDef.h>


//#include <vgEntry/vgFogEntry.h>
#include <vgEntry/vgRootEntry.h>
#include <vgEntry/vgCamMgrEntry.h>
//#include <vgEntry/vgSoundBackgroundEntry.h>

#include <vgEntry/vgFrustrumEntry.h>
#include <vgEntry/vgTerrainEntry.h>
//#include <vgEntry/vgBackgroundEntry.h>

//#include <vgOcc/vgoOcclusion.h>
//#include <vgMod/vgModMisc.h>
//#include <vgSound/vgSoundManager.h>
#include <vgMesh/vgmMeshManager.h>


namespace vgUI
{

	UIController::UIController(void)
		: vgKernel::Singleton<UIController>(VGK_SIGNLETON_LEFE_UICONTROLMGR)
	{	
		// m_pTabView = new vgPropertiesViewBar();
		m_pTabView = NULL;

		// m_pWorkSpaceBar = new vgWorkSpaceBar();
		m_pWorkSpaceBar = NULL;

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

	UIController::~UIController(void)
	{
		//m_pTabView->RemoveAllTab();

		delete m_propPage;

		for (int i=0; i<TAB_ARRAY_SIZ; i++)
		{
			delete m_listTabPage[i];
		}

		// double delete test
// 		if (m_pTabView != NULL)
// 		{
// 			delete(m_pTabView);
// 			m_pTabView = NULL;
// 		}

// 		if (m_pWorkSpaceBar != NULL)
// 		{
// 			delete m_pWorkSpaceBar;
// 			m_pWorkSpaceBar = NULL;
// 		}

		if (m_wndStatusBar != NULL)
		{
			delete m_wndStatusBar;
			m_wndStatusBar = NULL;
		}

		vgKernel::InputSystem::getSingleton().removeHandle(this);
	}

	vgBasedTabPage* UIController::GetTabPage(long ID)
	{
		return m_listTabPage[ID];
	}

	vgPropertyPage* UIController::GetPropPage()
	{
		return m_propPage;
	}

	vgPropertiesViewBar* UIController::GetPropertiesViewBar()
	{
		return m_pTabView;
	}

	void UIController::SetPropertiesViewBar(vgPropertiesViewBar* pPorpView)
	{
		m_pTabView= pPorpView;
	}

	vgWorkSpaceBar* UIController::GetWorkSpaceBar()
	{
		return m_pWorkSpaceBar;
	}

	void UIController::SetWorkSpaceBar(vgWorkSpaceBar *pWorkSpaceBar)
	{
		if (m_pWorkSpaceBar != NULL)
		{
			delete m_pWorkSpaceBar;
		}

		m_pWorkSpaceBar = pWorkSpaceBar;
	}

	void UIController::AddTabPage(string pageName, CWnd* page)
	{
		m_pTabView->AddTab(pageName, page);

		return ;
	}

	void UIController::RemoveAllPages()
	{
		m_pTabView->RemoveAllTab();
	}

	void UIController::OnChar(int keyCode)
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
//			vgMod::ModMisc::getSingleton().setStatus( 1);


		default:
			break; 
		}
	}


	// ��ʼ��������
	bool UIController::initUIControl()
	{
		//HTREEITEM		tempEntryRootItem;
		vgBaseEntry*	tempEntry = NULL;

		HTREEITEM hStaticRoot = appendStaticEntryRoot( "����" );
		appendEntryRootItem( STATIC_ENTRY_ROOT, hStaticRoot );

		// ��һ��////////////////////////////////////////////////////////////////////////////
		tempEntry = new vgCoordinateEntry("����ԭ������");
		AddNode( tempEntry );

		tempEntry = new vgFrustrumEntry("�Ӿ���");
		AddNode( tempEntry );

		tempEntry = new vgTerrainEntry();
		AddNode(tempEntry);

		tempEntry = new vgBackgroundEntry();
		AddNode(tempEntry);

		tempEntry = new vgFogEntry();
		AddNode(tempEntry);		


		vgKernel::StringVector rootStaticEntryNames;
		rootStaticEntryNames.assign( STATIC_ENTRY_COUNT, String());
		rootStaticEntryNames[STATIC_ENTRY_MODEL]		= String("ģ��");
		rootStaticEntryNames[STATIC_ENTRY_CAMERA]		= String("���");
		rootStaticEntryNames[STATIC_ENTRY_EFFECT]		= String("��Ч");
		rootStaticEntryNames[STATIC_ENTRY_ANIMATION]	= String("����");
		rootStaticEntryNames[STATIC_ENTRY_SOUND]		= String("��Ч");
		rootStaticEntryNames[STATIC_ENTRY_SPECIFIC_ANALYSIS] = String("ר�����");
		rootStaticEntryNames[STATIC_ENTRY_SHAPEFILE]	= String("ʸ��ͼ��");
		rootStaticEntryNames[STATIC_ENTRY_TRIGGER]     = String("������");
		rootStaticEntryNames[STATIC_ENTRY_PIPELINE]     = String("GIS���߷���");
		int nType = 0;
		for (vgKernel::StringVector::iterator itr = rootStaticEntryNames.begin();
			itr != rootStaticEntryNames.end(); itr ++, nType ++)
		{
			if ( nType != 0 )
			{
				HTREEITEM hStaticRoot = appendStaticEntryRoot( *itr );
				appendEntryRootItem( (EStaticEntryType)nType, hStaticRoot );
			}// ����0ʱ ��һ�����ڵ�("����") ���������
		}


		// �ڶ���////////////////////////////////////////////////////////////////////////////
		tempEntry = new vgCamMgrEntry();
		appendStaticEntryRoot(STATIC_ENTRY_CAMERA, tempEntry);



		vgKernel::StringVector rootDynamicEntryNames;
		rootDynamicEntryNames.assign( DYNAMIC_ENTRY_COUNT, String());

		rootDynamicEntryNames[MODEL_VGM]					= String("VGMģ��");
		rootDynamicEntryNames[MODEL_MOD]					= String("MODģ��");

		rootDynamicEntryNames[CAMERA_VIEW]				= String("�������");

		rootDynamicEntryNames[EFFECT_BILLBOARD_PLANT]	= String("ֲ��");

		rootDynamicEntryNames[EFFECT_PARTICLE_FOUNTAIN]	= String("��Ȫ");
		rootDynamicEntryNames[EFFECT_PARTICLE_FIRE]		= String("��");
		rootDynamicEntryNames[EFFECT_PARTICLE_SMOKE]		= String("��");
		//rootDynamicEntryNames[EFFECT_PARTICLE_FLAG]		= String("����");

		rootDynamicEntryNames[EFFECT_MOD_WATER]			= String("ˮ");
		rootDynamicEntryNames[EFFECT_MOD_GLASS]			= String("����");
		rootDynamicEntryNames[EFFECT_MOD_MIRROR]			= String("����");
		rootDynamicEntryNames[EFFECT_MOD_METAL]			= String("����");
		rootDynamicEntryNames[EFFECT_MOD_FLOOR]			= String("��������");
		rootDynamicEntryNames[EFFECT_MOD_AVI]			= String("��ƵAVI");
		rootDynamicEntryNames[EFFECT_MOD_SKYBOX]			= String("��պ�");
		rootDynamicEntryNames[EFFECT_MOD_DYNAMIC]		= String("��̬����");

		rootDynamicEntryNames[EFFECT_MOD_TRIGGER]		= String("������");
		rootDynamicEntryNames[EFFECT_MOD_AUTOBUILD]		= String("�Զ���ģ");

		rootDynamicEntryNames[ANIMATION_KEYFRAME]		= String("�ؼ�֡");
		rootDynamicEntryNames[ANIMATION_MS3D]			= String("����");
		rootDynamicEntryNames[ANIMATION_VCR]				= String("����");

		//rootDynamicEntryNames[SOUND_3D]					= String("������Դ");
		rootDynamicEntryNames[SPECIFIC_ANALYSIS_ROAD_GEN]= String("��·ƥ��");
		rootDynamicEntryNames[SHAPEFILE]					= String("ShapeFile");

		nType = 0;
		for (vgKernel::StringVector::iterator itr = rootDynamicEntryNames.begin();
			itr != rootDynamicEntryNames.end(); itr ++, nType ++)
		{
			EDynamicEntryType entyrType = (EDynamicEntryType)nType;
			HTREEITEM hDynamicRoot = appendDynamicEntryRoot( *itr, entyrType );
			appendEntryRootItem( entyrType, hDynamicRoot );
		}

		HTREEITEM hRootItem = getEntryRootItemByType(STATIC_ENTRY_ROOT);
		GetWorkSpaceBar()->Expand(hRootItem);
		GetWorkSpaceBar()->EnsureVisible(hRootItem);

		return	true;

	}

	//new Status Bar ===============================================================

	void UIController::EnableProgressBar()
	{
		m_wndStatusBar->EnablePaneProgressBar((int)StatusProgress, 100);
	}

	void UIController::DisableProgressBar()
	{
		m_wndStatusBar->EnablePaneProgressBar((int)StatusProgress, -1);
	}

	void UIController::SetProgress(UINT cnt)
	{
		m_wndStatusBar->SetPaneProgress((int)StatusProgress, cnt);
	}

	void UIController::SetFrameSpeed(UINT speed)
	{
		char strSpeed[24];

		sprintf_s(strSpeed, 24, "%d", speed);

		m_wndStatusBar->SetPaneText( (int)StatusFrame, strSpeed);

		if (speed > 30)
		{
			m_wndStatusBar->SetPaneTextColor((int)StatusFrame, GREEN);
		}
		else if (speed > 20)
		{
			m_wndStatusBar->SetPaneTextColor((int)StatusFrame, LIGHTBLUE);
		}
		else
		{
			m_wndStatusBar->SetPaneTextColor((int)StatusFrame, RED);
		}
	}

	void UIController::SetNewOccCnt(UINT tcnt,UINT rcnt,UINT cnt)
	{

		//return ;
		char strCnt[24];
#if 0
		if (vgOcclusion::vgOcclusionManager::getSingleton().GetOccState())
			//sprintf_s(strCnt, 24, "%u-%u=%u",tcnt,cnt,rcnt );
			sprintf_s(strCnt, 24, "Occ ON");
		else
			sprintf_s(strCnt, 24, "Occ OFF");
#endif
		m_wndStatusBar->SetPaneText( (int)StatusOcc, strCnt);

		if (cnt > 30)
		{
			m_wndStatusBar->SetPaneTextColor((int)StatusOcc, GREEN);
		}
		else if (cnt > 20)
		{
			m_wndStatusBar->SetPaneTextColor((int)StatusOcc, LIGHTBLUE);
		}
		else
		{
			m_wndStatusBar->SetPaneTextColor((int)StatusOcc, RED);
		}
	}


	//----------------------------------------------------------------
	bool UIController::addEntryToRootByName( const String& root_name , 
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

	void UIController::reset()
	{
		HTREEITEM hItem = getEntryRootItemByType( STATIC_ENTRY_ROOT );
		m_pWorkSpaceBar->cleartree( hItem );

		m_pWorkSpaceBar->DeleteAllNode();

		RemoveAllPages();

		m_pOutputBar->ClearContent();

		initUIControl();

		VGK_SHOW(" Welcome to VR-GIS platform! \r\n");


	}



	EntryRootManager* UIController::getEntryRoot()
	{
		return &m_entryRoot;
	}

	HTREEITEM UIController::getEntryRootItemByType( EDynamicEntryType entryType )
	{
		return m_entryRoot.getEntryRootByType(entryType);
	}

	HTREEITEM UIController::getEntryRootItemByType( EStaticEntryType entryType )
	{
		return m_entryRoot.getEntryRootByType(entryType);
	}

	void UIController::appendEntryRootItem( EDynamicEntryType entryType, HTREEITEM item )
	{
		if (item == NULL)
		{
			return;
		}
		m_entryRoot.appendEntryRootItem(entryType, item);
	}

	void UIController::appendEntryRootItem( EStaticEntryType entryType, HTREEITEM item )
	{
		if (item == NULL)
		{
			return;
		}
		m_entryRoot.appendEntryRootItem(entryType, item);
	}

	HTREEITEM UIController::appendStaticEntryRoot( String rootname )
	{
		if (rootname.empty())
		{
			return NULL;
		}
		vgBaseEntry*	tempEntry = new vgRootEntry( rootname );

		return AddNode( tempEntry );
	}

	HTREEITEM UIController::appendStaticEntryRoot( EStaticEntryType entryType,
		vgBaseEntry* baseEntry )
	{
		HTREEITEM item = getEntryRootItemByType(entryType);
		return AddNode(item, baseEntry);	
	}
	HTREEITEM UIController::appendDynamicEntryRoot( String& rootname,
		EDynamicEntryType entryType )
	{
		if (rootname.empty())
		{
			return NULL;
		}

		vgBaseEntry*	tempEntry = new vgRootEntry( rootname );

		HTREEITEM item;
		if ( entryType <= MODEL_MOD )
		{
			item = getEntryRootItemByType( STATIC_ENTRY_MODEL );
		} 
		else if( entryType <= CAMERA_VIEW )
		{
			item = getEntryRootItemByType( STATIC_ENTRY_CAMERA );
		}
		else if( entryType <= EFFECT_MOD_AUTOBUILD )
		{
			item = getEntryRootItemByType( STATIC_ENTRY_EFFECT );
		}
		else if( entryType <= ANIMATION_VCR )
		{
			item = getEntryRootItemByType( STATIC_ENTRY_ANIMATION );
		}
		else if( entryType <= SOUND_3D )
		{
			item = getEntryRootItemByType( STATIC_ENTRY_SOUND );
		}
		else if( entryType <= SPECIFIC_ANALYSIS_ROAD_GEN )
		{
			item = getEntryRootItemByType( STATIC_ENTRY_SPECIFIC_ANALYSIS );
		}
		else if( entryType <= SHAPEFILE )
		{
			item = getEntryRootItemByType( STATIC_ENTRY_SHAPEFILE );
		}
		else if( entryType <= TRIGGER )
		{
			item = getEntryRootItemByType( STATIC_ENTRY_TRIGGER );
		}
		else if( entryType <= PIPELINE )
		{
			item = getEntryRootItemByType( STATIC_ENTRY_PIPELINE );
		}
		else
		{
			assert(0);
		}

		return	AddNode(item, tempEntry);
	}

	void UIController::unload()
	{
		HTREEITEM hItem = getEntryRootItemByType( STATIC_ENTRY_ROOT );
		m_pWorkSpaceBar->cleartree( hItem );

		m_pWorkSpaceBar->DeleteAllNode();

		// Unload ����Ҫ������е�TabPage ���Զ�����
		// RemoveAllPages();
		// ����Outputbar ����������
		// m_pOutputBar->ClearContent();
	}
}// namespace vgUI
