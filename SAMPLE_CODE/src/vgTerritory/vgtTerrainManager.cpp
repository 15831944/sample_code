


#include <vgStableHeaders.h>
#include <vgTerritory/vgtTerrainManager.h>
#include <vgTerritory/vgDemImporter.h>
#include <vgTerritory/vgVgtrWriter.h>
#include <vgTerritory/vgTerrainHugeImageProcessor.h>
#include <vgImage/vgiCximageWrapper.h>


#include <vgTerritory/vgtFileHandleVtr.h>
#include <vgAsyn/vgaIoServer.h>
#include <vgTerritory/vgtElevationManager.h>
#include <vgTerritory/vgtTerrainCenterManager.h>
#include <vgImage/vgiImageManager.h>

#include <vgTerritory/vgtCutSurfaceAnalysis.h>
#include <vgTerritory/vgtCutSurfaceAnalysis2.h>
#include <vgTerritory/vgtCutSurfaceVisibility.h>
#include <vgTerritory/vgtCutSurfaceVisibility2.h>
#include <vgTerritory/vgtCutSurfaceLevelling.h>
#include <vgTerritory/vgtCutArea.h>
#include <vgTerritory/vgtSurfaceLengthSurvey.h>
#include <vgTerritory/vgtSurfaceAreaSurvey.h>
#include <vgTerritory/vgtFileWriterVtr.h>

#include <vgKernel/vgkWarning.h>


#include <vgConf/vgcoProjectConfigRegProxy.h>
#include <vgConf/vgcoProjectConfig.h>

#include <vgImage/vgiTextureIoRequestDispatcher.h>
#include <vgImage/vgiTextureManager.h>

#include <vgKernel/vgkCoordSystem.h>

#include <vgCam/vgcaCamManager.h>
#include <vgKernel/vgkRenderCommandManager.h>
#include <vgTerritory/vgTerrioryRenderCommand.h>

namespace vgTerritory {
	


	/**
		�����ﴴ��project config
	*/
	vgConf::ProjectConfigRegProxy terrain_project_reg1( 
		"Territory" , "path" , "" );
	
	vgConf::ProjectConfigRegProxy terrain_project_reg2( 
		"Territory" , "blend" , "0" );

	vgConf::ProjectConfigRegProxy terrain_project_reg3( 
		"Territory" , "collision" , "1" );

	vgConf::ProjectConfigRegProxy terrain_project_reg4( 
		"Territory" , "renderFlag" , "1" );
	
	//----------------------------------------------------------------
	TerrainManager::TerrainManager() : Singleton<TerrainManager>( 
		VGK_SINGLETON_LEFE_TERRAINMGR )
	{
		_renderFlag = false;
		//_fileOpened = false;
		//_terrrainFilepath = "";
		_gpuExtensionSupported = false;
		_terrainGpuProgramPtr  =NULL;
		_collisionDetectedFlag = true;

		_cutSurfaceAnalysis = NULL;
		_cutSurfaceAnalysis2 = NULL;
		_cutSurfaceVisibility = NULL;
		_cutSurfaceVisibility2 = NULL;
		_cutSurfaceLevelling = NULL;
		_surfaceLengthSurvey = NULL;
		_surfaceAreaSurvey = NULL;

		_blendRenderingFlag = false;
		_cutArea = NULL;

		flagCutSurf = false;
		flagDlgCellSize = false;
		posY = 0.0f;
		//_globalOffsetX = 1000.0f;
		//_globalOffsetZ = 600.0f;
		//_globalOffsetY = 0.0f;
		//bHaveModTerrain=false;

		_bSelectMode = false;

		m_processingDlg = new vgtProcessingDlg;
		m_processingDlg->Create(vgtProcessingDlg::IDD, NULL);

		VGK_TRACE(_T("TerrainManager created."));
	
	}
	//----------------------------------------------------------------
	TerrainManager::~TerrainManager()
	{
		flagCutSurf = false;
		flagDlgCellSize = false;

		if (m_processingDlg != NULL)
		{
			delete m_processingDlg;
		}

		VGK_TRACE(_T("TerrainManager destroyed."));
	}
	//----------------------------------------------------------------
	
	bool TerrainManager::initialise()
	{
		m_pRenderCommand = new vgTerritory::vgTerrainRenderCommand;

		vgKernel::RenderCommandFacade::AddCommand(m_pRenderCommand);

		return true;
	}
	
	bool TerrainManager::shutdown()
	{
		if (m_pRenderCommand != NULL)
		{
			vgKernel::RenderCommandFacade::RemoveCommand(m_pRenderCommand);
			m_pRenderCommand = NULL;
		}

		assert( _terrainRenderer.isNull() == true );
		return true;
	}

	//----------------------------------------------------------------
	bool TerrainManager::initAfterOpenGLSetup()
	{

		//------------------------------------------
		// ���GLEW_NV_vertex_program3��չ
		//------------------------------------------
		if ( GLEW_NV_vertex_program3 == false )
		{
#if VGK_LANG_CHS
			VGK_SHOW( "Ӳ����֧��NV_vertex_program3, ���ν�������Ⱦ.\n" );
#else
			VGK_SHOW( "Hardware do not support NV_vertex_program3, terrain will not be rendered.\n" );
#endif 
			return false;
		}

		//------------------------------------------
		// ���������չ
		// �Ѿ�������Ҫ�����
		//------------------------------------------
#if 0
		int maxtexsize;
		glGetIntegerv(GL_MAX_TEXTURE_SIZE,&maxtexsize); 

		if ( maxtexsize < 2048 )
		{
			VGK_SHOW( "Ӳ����֧��2048���ϵ�����, ���ν�������Ⱦ.\n" );
			return false;
		}
#endif

		//------------------------------------------
		// ���CG��չ
		//------------------------------------------
		if ( vgKernel::GpuManager::getSingleton().getGpuSupport()->getVertexProfile()
			!= CG_PROFILE_VP40 || 
			vgKernel::GpuManager::getSingleton().getGpuSupport()->getPixelProfile()
			!= CG_PROFILE_FP40 )
		{
			VGK_SHOW( "Ӳ����֧������GPU���, ���ν�������Ⱦ.\n" );
			return false;
		}

		//------------------------------------------
		// ���gpuprogram�Ĵ���
		//------------------------------------------
		assert( _terrainGpuProgramPtr == NULL );
		_terrainGpuProgramPtr = new vgTerritory::TerrainGpuProgram;
		bool gpuinitret = _terrainGpuProgramPtr->init();
		if ( gpuinitret == false )
		{
#if VGK_LANG_CHS
			VGK_SHOW( "����GPU����ʧ��, ���ν�������Ⱦ..\n" );
#else
			VGK_SHOW( "Shader program create failed. Terrain will not be rendered.\n" );
#endif
			return false;
		}

		//------------------------------------------
		// Ĭ��Ϊfalse, ������������Ϊtrue���ɡ� 
		//------------------------------------------
		TerrainManager::getSingleton()._gpuExtensionSupported = true;

		return true;
	}
	//----------------------------------------------------------------
	void TerrainManager::render()
	{
		if ( _renderFlag == true )
		{
			if ( _gpuExtensionSupported == false )
			{
#if VGK_LANG_CHS
				MessageBox( AfxGetApp()->m_pMainWnd->m_hWnd , "Ӳ����֧�ֵ�����Ⱦ.", "��ʾ", MB_OK );
#else
				MessageBox( AfxGetApp()->m_pMainWnd->m_hWnd , "Hardware do not support terrain rendering.", "Tips", MB_OK );
#endif
				_renderFlag = false;
				return;
			}

			if ( _terrainRenderer.isNull() == true )
			{
				//MessageBox( AfxGetApp()->m_pMainWnd->m_hWnd , "��򿪵����ļ�", "��ʾ", MB_OK );
				_renderFlag = false;
				return;
			}

			//------------------------------------------
			// ����blendģʽ
			//------------------------------------------
			if ( _blendRenderingFlag )
			{
				glEnable( GL_BLEND );
				glBlendFunc( GL_CONSTANT_ALPHA ,
					GL_ONE_MINUS_CONSTANT_ALPHA );

				glBlendColor( 1.0f, 1.0f ,1.0f , 0.3f );
			}
			else
			{
				glDisable( GL_BLEND );
			}
			//------------------------------------------
			// ��ʽ��ʼ��Ⱦ
			//------------------------------------------
			//------------------------------------------
			// ��ʽ��Ⱦ
			//------------------------------------------
			assert( _terrainRenderer.isNull() == false );
			_terrainRenderer->render();



		} // end of RenderFlag

		if ( _collisionDetectedFlag )
		{
			//------------------------------------------
			// �������ε���ײ���
			//------------------------------------------
			Vec3 curpos = vgCam::CamManager::getSingleton().getCurrentPosition();

			float nowelev = 0.0f;
			bool getok = getElevationByPosition( curpos , nowelev, true );

			nowelev += 2.0f;
			if ( getok == true && curpos.y < nowelev&&vgCam::CamManager::getSingleton().getCollisionOnOff() )
			{
				curpos.y = nowelev;

				vgCam::CamManager::getSingleton().setCurrentPosition( curpos );
			}
		}

		//------------------------------------------
		//  Terrain Utility
		//------------------------------------------
		if ( _cutSurfaceAnalysis != NULL )
		{
			_cutSurfaceAnalysis->render();
		}

		if ( _cutSurfaceAnalysis2 != NULL )
		{
			_cutSurfaceAnalysis2->render();
		}

		if ( _cutSurfaceVisibility != NULL )
		{
			_cutSurfaceVisibility->render();
		}

		if ( _cutSurfaceVisibility2 != NULL )
		{
			_cutSurfaceVisibility2->render();
		}

		if ( _cutSurfaceLevelling != NULL )
		{
			_cutSurfaceLevelling->render();
		}


		if ( _cutArea != NULL )
		{
			_cutArea->render();
		}

		if ( _surfaceLengthSurvey != NULL)			//@ FengYK 2009-12-23
		{
			_surfaceLengthSurvey->render();
		}

		if ( _surfaceAreaSurvey != NULL )
		{
			_surfaceAreaSurvey->render();
		}

		return;
	}

	//----------------------------------------------------------------
	bool TerrainManager::dropTerrianData()
	{
		safeDeleteAllCutSurfaces();

		if ( _cutArea != NULL )
		{
			delete _cutArea;
			_cutArea = NULL;
		}

		_terrainRenderer.setNull();
		//_vgtrFilePtr.setNull();
		_renderFlag = false;
		//_fileOpened = false;
		//_terrrainFilepath = "";

		TerrainCenterManager::getSingleton().clearData();
		ElevationManager::getSingleton().clearData();

		return true;
	}
	//----------------------------------------------------------------
#if 0 
	bool TerrainManager::convertVgtrToHeightImage( 
		const vgTerritory::VgtrFilePtr pvgtrfile ,
		const String& outputimagename )
	{

		if ( pvgtrfile.isNull() == true )
		{
			assert(0);
			return false;
		}

		vgTerritory::VgtrHeaderPtr header = pvgtrfile->_headerPtr;
		float* data = pvgtrfile->_dataLod[VGLOD_CLEAR];

		//------------------------------------------
		// ����ͼ����ڴ沢��ֵ
		//------------------------------------------
		uchar* imgdata = new uchar[ header->xNum * header->zNum * 3 ];
		uchar tmp;
		for ( int i = 0 ; i < header->xNum * header->zNum ; ++ i )
		{
			tmp = data[i] * 255;
			imgdata[ 3 * i ] = tmp;
			imgdata[ 3 * i + 1 ] = tmp;
			imgdata[ 3 * i + 2 ] = tmp;
		}

		bool filpimg = false;

		/**
			bmp�ļ���Ҫ�ߵ�һ��
		*/
		if ( vgImage::ImageUtility::findImageFileExtension( outputimagename )
			== "bmp")
		{
			filpimg = true;
		}

		CximagePtr cximg( new CxImage );

		bool createresult = cximg->CreateFromArray( imgdata , 
			header->xNum , header->zNum , 24 , header->xNum * 3 , filpimg );

		if ( createresult == false )
		{
			return false;
		}

		CximageWrapperPtr img( new CximageWrapper( cximg ) );

		return img->saveToFile( outputimagename );
	}
#endif
	//----------------------------------------------------------------
#if 0	
	void TerrainManager::doLevelling(const bool& delete_points_list/* = true */)
	{

		assert(0);


		if ( _vgtrFilePtr.isNull() == true )
		{
			return;
		}

		if ( _levellingClickPointsList.size() < 3 )
		{
			if ( delete_points_list == true )
			{
				clearClickPointsList();
			}

			return;
		}

		DlgTerrainLevelling dlg( _vgtrFilePtr );

		int dlgret = dlg.DoModal();
		if ( dlgret == IDCANCEL )
		{
			if ( delete_points_list == true )
			{
				clearClickPointsList();
			}

			return;
		}

		const float height = dlg._heightOfLevelling;


		bool ret = vgTerritory::TerrainLevelling::doLevelling( _vgtrFilePtr , 
			_terrainRenderer->getHeightMapid() , 
			(vgKernel::Vec3*)&_levellingClickPointsList[0] , 
			_levellingClickPointsList.size() ,
			height ); 

		assert( ret == true );

		if ( delete_points_list == true )
		{
			clearClickPointsList();
		}

	}
#endif
	//----------------------------------------------------------------
#if 0	
	bool TerrainManager::saveVgtrFile()
	{
		assert(0);
		return true;


		String filepath;



		CString DefExt;

		DefExt.Format("%s","vgtr�ļ�(*.vgtr)|*.vgtr|");

		CFileDialog dlgvgtr(false,"vgtr",
			NULL,OFN_HIDEREADONLY|OFN_CREATEPROMPT|OFN_NONETWORKBUTTON,DefExt);

		dlgvgtr.m_ofn.lpstrTitle = "��������ļ�";



		if( dlgvgtr.DoModal() == IDOK )
		{
			filepath = dlgvgtr.GetFileName();
		}



		if ( filepath.empty() == true )
		{
			return false;
		}

		return vgTerritory::TerrainLevelling::saveVgtrFile( filepath , 
			_vgtrFilePtr , _terrainRenderer->getHeightMapid() );


	}

#endif
	//----------------------------------------------------------------
//	void TerrainManager::writeToFile( CFile* pCfile )
//	{
//		assert(0);
//
//#if 0
//		//------------------------------------------
//		// 1.д���ֱ��λ
//		//------------------------------------------
//		pCfile->Write( &_renderFlag , sizeof( bool ) );
//
//		//------------------------------------------
//		// 2. �洢vgtr�ļ�
//		//------------------------------------------
//
//		if ( _fileOpened == true )
//		{
//
//			string dest = mgr._currentSavingPath.substr(
//				0 , mgr._currentSavingPath.find_last_of("\\/")) + "\\ProAttachdata\\" + 
//				_terrrainFilepath ;
//
//
//			String exepath( mgr.pathbufferStatic );
//
//			string src = exepath + "\\" + _terrrainFilepath;
//
//			CreateDirectory( 
//				(mgr._currentSavingPath.substr(
//				0 , mgr._currentSavingPath.find_last_of("\\/")) + "\\ProAttachdata"  ).c_str() , NULL);
//
//			BOOL save_ret = CommonCopyFile( src.c_str() , dest.c_str() );
//
//			assert( save_ret == TRUE );
//		}
//
//#endif
//	}
	//----------------------------------------------------------------
	//bool TerrainManager::readFromFile( CFile* pCfile )
	//{
	//	//------------------------------------------
	//	// 1.�����ֱ��λ
	//	//------------------------------------------
	//	pCfile->Read( &_renderFlag , sizeof( bool ) );


	//	return true;
	//}
	//----------------------------------------------------------------
	bool TerrainManager::uninitBeforeOpenGLDestroy()
	{
		dropTerrianData();

		if ( _terrainGpuProgramPtr != NULL )
		{
			delete _terrainGpuProgramPtr;
			_terrainGpuProgramPtr = NULL;
		}
		return true;
	}
	//----------------------------------------------------------------

#if 0
	//----------------------------------------------------------------
	bool TerrainManager::selectDemsToVgtr( const StringVectorPtr input_dems ,
		const String& outputname , const int& cell_tex_size )
	{
		if ( input_dems.isNull() == true )
		{
			return false;
		}

		//------------------------------------------
		// ����path��filename
		//------------------------------------------
		std::set<String> abspathset;
		std::set<String> filenameSet;

		StringVector::const_iterator iter = input_dems->begin();
		StringVector::const_iterator iter_end = input_dems->end();

		for ( ; iter != iter_end ; ++ iter )
		{
			filenameSet.insert(
				iter->substr( iter->find_last_of("\\/") + 1 , std::string::npos ));

			abspathset.insert( *iter );
		}

		assert( abspathset.size() == input_dems->size() );
		if ( abspathset.size() != filenameSet.size() )
		{
			MessageBox( NULL , "����ͬ�����ļ�" ,"����" , MB_OK );
			return false;
		}


		//------------------------------------------
		// ����õ���dem�ļ�
		//------------------------------------------
		vgTerritory::DemBlockVectorPtr pblocks( new vgTerritory::DemBlockVector );

		std::set<String>::iterator fnameiter = abspathset.begin();
		std::set<String>::iterator fnameiter_end = abspathset.end();

		for ( ; fnameiter != fnameiter_end ; ++ fnameiter )
		{
			vgTerritory::DemBlockPtr block = 
				vgTerritory::DemImporter::readDemFile( *fnameiter );

			assert( block.isNull() == false );

			pblocks->push_back( block );
		}


		vgTerritory::VgtrFilePtr resultfile = 
			vgTerritory::VgtrWriter::convertDemBlocksToVgtr( "terrain2048-2048" , pblocks ,cell_tex_size, 8 );

		assert( resultfile.isNull() == false );

		if ( resultfile.isNull() == true )
		{
			return false;
		}

		return vgTerritory::VgtrWriter::writeVgtrToFile( resultfile , outputname );
	}
	//----------------------------------------------------------------

	bool TerrainManager::selectDemsToVgtr()
	{

		//------------------------------------------
		// 1.���ȵ���ѡ��dem�ĶԻ���
		//------------------------------------------
		StringVectorPtr strVec( new StringVector );

		{
			CString DefExt;

			DefExt.Format("%s","dem�ļ�(*.dem)|*.dem|");

			CFileDialog dlgmodelFile(true,"dem",
				NULL,OFN_HIDEREADONLY|
				OFN_CREATEPROMPT|
				OFN_NONETWORKBUTTON|
				OFN_ALLOWMULTISELECT|   
				OFN_FILEMUSTEXIST,
				DefExt);

			DWORD   MAXFILE = 0xFFFF;   
			dlgmodelFile.m_ofn.nMaxFile=MAXFILE; 

			char*  pc = new char[MAXFILE];   

			dlgmodelFile.m_ofn.lpstrFile = pc;   
			dlgmodelFile.m_ofn.lpstrFile[0] = NULL;   
			dlgmodelFile.m_ofn.lpstrTitle="dem�ļ�����";



			int dlg_ret = dlgmodelFile.DoModal();

			if( dlg_ret != IDOK )
			{
				return false;
			}

			POSITION posi;   

			posi = dlgmodelFile.GetStartPosition();   

			while( posi != NULL )
			{   
				strVec->push_back( dlgmodelFile.GetNextPathName(posi).GetBuffer(0));
			}
		}

		//------------------------------------------
		// 2.��ε����洢Ŀ���ļ��ĶԻ���
		//------------------------------------------
		String file_save_path;

		{
			CString DefExt;

			DefExt.Format("%s","vgtr�ļ�(*.vgtr)|*.vgtr|");

			CFileDialog dlgvgtr(false,"vgtr",
				NULL,OFN_HIDEREADONLY|OFN_CREATEPROMPT|OFN_NONETWORKBUTTON,DefExt);

			dlgvgtr.m_ofn.lpstrTitle = "��������ļ�";



			if( dlgvgtr.DoModal() == IDOK )
			{
				file_save_path = dlgvgtr.GetFileName();
			}

		}

		if ( file_save_path.empty() == true )
		{
			return false;
		}

		//------------------------------------------
		// 3.���,ѡ���Ӧ��celltexsize�Ĵ�С
		//------------------------------------------
		assert(0&& "����Ҫ���");
		const int cell_tex_size = 8;

		//------------------------------------------
		// 4.���Ĵ�����
		//------------------------------------------
		bool dealing_result = selectDemsToVgtr( strVec , file_save_path , cell_tex_size );

		//------------------------------------------
		// 5.��ϲ
		//------------------------------------------
		if ( dealing_result == true )
		{
			MessageBox( NULL , "����vgtr�ļ����ɳɹ�!" , "��ʾ",  MB_OK );
		}
		else
		{
			MessageBox( NULL , "����vgtr�ļ�����ʧ��!" , "ERROR",  MB_OK );
		}

		return dealing_result;
	}

#endif

	//----------------------------------------------------------------
	bool TerrainManager::openVtrFile( const String& abs_vtr_filename )
	{
		if( vgKernel::SystemUtility::checkFileExist( abs_vtr_filename ) == false )
		{
			std::ostringstream o;
			o << "�ļ�: " << abs_vtr_filename << "������Ч��·��.";

			MessageBox( AfxGetApp()->m_pMainWnd->m_hWnd ,
				o.str().c_str(), "���ִ���", MB_OK );

			return false;
		}

		if ( _gpuExtensionSupported == false )
		{
			MessageBox( AfxGetApp()->m_pMainWnd->m_hWnd ,
				"Ӳ����֧�ֵ�����Ⱦ,���ν���������", "��ʾ", MB_OK );

			return false;
		}

		if ( _terrainRenderer.isNull() == false )
		{
			// ���ʱ���Ѿ����˵����ļ�.
			// ��Ҫѯ���Ƿ��滻
			int ret = VGK_WARN( vgKernel::Warning::WARN_INVALID_STATE ,
				vgKernel::Warning::BUTTON_YES_NO , 
				"�Ѻ��е����ļ�,���뽫ж�����еĵ�������,������?",
				"TerrainManager::openVtrFile");

			if ( ret == vgKernel::Warning::RETURN_NO )
			{
				return true;
			}

			//------------------------------------------
			// ���yes֮��,ɾ�����ݲ��Ҽ�������
			//------------------------------------------
			assert( ret == vgKernel::Warning::RETURN_YES );
			dropTerrianData();
		}

		// �õ�·��
		String openpath = 
			StringUtility::getPathFromAbsoluteFilename( abs_vtr_filename );

		// �õ��ļ��� 
		String vtrfile = 
			StringUtility::getFilenameFromAbsolutePath( abs_vtr_filename );

		//------------------------------------------
		// ���ȴ�vtr�ļ�
		//------------------------------------------
		FileHandleVtr* vtrhandle = 
			ElevationManager::getSingleton().openVtrFile( abs_vtr_filename  );

		if ( vtrhandle == NULL )
		{
			std::ostringstream o;
			o << "�򿪵����ļ�:" <<abs_vtr_filename << "ʱʧ��";
			MessageBox( AfxGetApp()->m_pMainWnd->m_hWnd ,
				o.str().c_str() , "��ʾ", MB_OK );

			return false;
		}

		assert( vtrhandle != NULL );
		VtrHeaderPtr pheader = vtrhandle->getHeader();
		//�õ��������Ͻǵĵ��λ�ã������������λ������livenson 20091102
		m_Position.y=0;
		m_Position.x=pheader->xTotalNum*pheader->cellsize/2;
		m_Position.z=pheader->zTotalNum*pheader->cellsize/2;
		//�����ε����긳������ԭ��				//FengYK 2009-12-22
		posY = pheader->originY;
		vgKernel::Vec3 coo( pheader->originX , pheader->originY , -pheader->originZ );
		vgKernel::CoordSystem::getSingleton().setProjectionCoord( coo );

		assert( pheader.isNull() == false );

		//------------------------------------------
		// Ȼ���vgi�ļ�
		//------------------------------------------
		String imgfile = pheader->imageFilename;
		bool bhavetexture=true;

		if ( imgfile.empty() == false && 
			vgKernel::SystemUtility::checkFileExist( openpath + imgfile ) == true )
		{
			vgImage::FileHandleVgi* vgihandle =  
				ElevationManager::getSingleton().openVgiFile( openpath + imgfile );

			if ( vgihandle == NULL )
			{
				std::ostringstream o;
				o << "�����ļ�:" << openpath + imgfile  << "ʱ���ִ���,���ν���������";
				MessageBox( AfxGetApp()->m_pMainWnd->m_hWnd ,
					o.str().c_str() , "��ʾ", MB_OK );
			}
		}
		else
		{
			//û�е�������ʱ����¼һ�£���ָ����ɫ��Ⱦ������
			//livenson --20091015
			bhavetexture=false;
		}


		//------------------------------------------
		// ���ɸ�����Ⱦ��centers
		//------------------------------------------
		bool gencen_ret = 
			TerrainCenterManager::getSingleton().generateCenters( vtrhandle );

		if ( gencen_ret == false )
		{
			std::ostringstream o;
			o << "���ɵ���ʱ���ִ���.�ļ�:" << abs_vtr_filename  << "";
			MessageBox( AfxGetApp()->m_pMainWnd->m_hWnd ,
				o.str().c_str() , "��ʾ", MB_OK );

			dropTerrianData();
			return false;
		}

		//------------------------------------------
		// ���ɸ����㼶������
		//------------------------------------------
		assert( _terrainRenderer.isNull() == true );
		_terrainRenderer.bind( 
			new vgTerritory::TerrainRenderer( _terrainGpuProgramPtr ) );
		_terrainRenderer->_terrainGpuProgram->_mbTexture=bhavetexture;

		bool render_init_ret = _terrainRenderer->init();

		if ( render_init_ret == false )
		{
			std::ostringstream o;
			o << "���ɵ���ʱ���ִ���.�ļ�:" << abs_vtr_filename  << "";
			MessageBox( AfxGetApp()->m_pMainWnd->m_hWnd ,
				o.str().c_str() , "��ʾ", MB_OK );

			dropTerrianData();

			return false;
		}

		//------------------------------------------
		// ���,�ɹ�����
		//------------------------------------------
		_renderFlag = true;

		notfiyUpdate();
		return true;
	}
	//----------------------------------------------------------------
	bool TerrainManager::saveData( const String& absolute_path )
	{
		if ( hasTerrainData() == false )
		{
			MessageBox( AfxGetApp()->m_pMainWnd->m_hWnd ,
				"û�е�������." , "��ʾ", MB_OK );

			return false;
		}

		//assert(0&&"Ҫ�޸�!!");
		//------------------------------------------
		// ���ȱ���vtr�ļ�
		//------------------------------------------
		FileHandleVtr* vtrhandle = 
			ElevationManager::getSingleton().getMainFileHandleVtr();

		assert( vtrhandle != NULL );


		FileHandleVtr* ne = vtrhandle;
		do
		{
			String longname = ne->getFilePath();
			String shortname =  vgKernel::StringUtility::getFilenameFromAbsolutePath( longname );
			String dst = absolute_path + "\\" + shortname;


			ne->copyWholeFile( dst );

			ne = ne->getNextFileHandle();
		}
		while ( ne != NULL );

		//------------------------------------------
		// ��α���vgi�ļ�
		//------------------------------------------
		vgImage::FileHandleVgi* vgihandle = 
			ElevationManager::getSingleton().getMainFileHandleVgi();

		assert( vgihandle != NULL );

		vgImage::FileHandleVgi* vgne = vgihandle;
		do
		{

			String longname = vgihandle->getFilePath();
			String shortname =  vgKernel::StringUtility::getFilenameFromAbsolutePath( longname );
			String dst = absolute_path + "\\" + shortname;

			//bool copyok = vgKernel::SystemUtility::copyFile( longname , dst );


			vgne->copyWholeFile( dst );

			//if ( copyok == false )
			//{
			//	std::ostringstream o;
			//	o << "���ļ�:" << longname  << "���浽:" << dst << "ʱ���ִ���.";
			//	MessageBox( AfxGetApp()->m_pMainWnd->m_hWnd ,
			//		o.str().c_str() , "��ʾ", MB_OK );
			//}
			vgne = vgne->getNextFileHandle();
		}
		while( vgne != NULL );

		std::ostringstream o;
		o << "�ļ�:" << vtrhandle->getFilePath() 
			<< "," << vgihandle->getFilePath() << " ���浽:" << absolute_path 
			<< "�ɹ����.";

		//MessageBox( AfxGetApp()->m_pMainWnd->m_hWnd ,
		//	o.str().c_str() , "��ʾ", MB_OK | MB_ICONINFORMATION );

		return true;
	}
	//----------------------------------------------------------------
	bool TerrainManager::getElevationByPosition( const Vec3& pos , 
		float& out_elevation , 
		const bool& use_accurate )
	{
		if ( hasTerrainData() == false )
		{
			return false;
		}

		//------------------------------------------
		// ���ڴ��ж�ȡ��ģʽ
		//------------------------------------------
		if ( use_accurate == false )
		{
			TerrainRenderer* renderer = _terrainRenderer.getPointer();

			//------------------------------------------
			// ���ȼ���chunk��number,�ϸ��һ��.
			//------------------------------------------
			long pow_2_num = pow( 2.0f, renderer->_N );

			long chunkxnum = floor( pos.x / renderer->_cellsize / pow_2_num );
			long chunkznum = floor( pos.z / renderer->_cellsize / pow_2_num );

			long layer = 0;
			long layernum = renderer->getVtrLayerNum();

			bool getit = true;

			while ( layer != layernum )
			{

				ElevationItem* item = ElevationManager::getSingleton().getElevationItem(
					layer , chunkxnum , chunkznum , false );

				if ( item == NULL )
				{
					getit = false;
					break;
				}

				if ( item->getElevationByPosInMemory( pos , out_elevation ) )
				{
					getit = true;
					break;
				}

				chunkxnum /= 2;
				chunkznum /= 2;
				layer += 1;
			}

			return getit;
		}


		assert( _terrainRenderer.isNull() == false );
		assert( use_accurate == true );

		TerrainRenderer* renderer = _terrainRenderer.getPointer();

		//------------------------------------------
		// ���ȼ���chunk��number,�ϸ��һ��.
		//------------------------------------------
		long pow_2_num = pow( 2.0f, renderer->_N );

		long chunkxnum = floor( pos.x / renderer->_cellsize / pow_2_num );
		long chunkznum = floor( pos.z / renderer->_cellsize / pow_2_num );


		ElevationItem* item = ElevationManager::getSingleton().getElevationItem(
			0 , chunkxnum , chunkznum , false );

		if ( item == NULL )
		{
			return false;
		}

		bool getit = item->getElevationByPos( pos , out_elevation );

		return true;
	}
	//----------------------------------------------------------------
	void TerrainManager::setCutSurfaceAnalysisFlag( const bool& enable )
	{
		if ( enable == true )
		{
			// ע��,Ҫ��ɾ���ɵ�.
			// ��Ϊ��ϡ��������Ҫ����inputsystem��status
			safeDeleteAllCutSurfaces();

			if ( _cutSurfaceAnalysis == NULL )
			{
				_cutSurfaceAnalysis = new CutSurfaceAnalysis;
				flagCutSurf = true;
			}
		}
		else
		{
			if ( _cutSurfaceAnalysis != NULL )
			{
				delete _cutSurfaceAnalysis;
				_cutSurfaceAnalysis = NULL;
				flagCutSurf = false;
			}
		}
	}
	//----------------------------------------------------------------
	void TerrainManager::setCutSurfaceAnalysisFlag2( const bool& enable )
	{
		if ( enable == true )
		{
			// ע��,Ҫ��ɾ���ɵ�.
			// ��Ϊ��ϡ��������Ҫ����inputsystem��status
			safeDeleteAllCutSurfaces();

			if ( _cutSurfaceAnalysis2 == NULL )
			{
				_cutSurfaceAnalysis2 = new CutSurfaceAnalysis2;
				_cutSurfaceAnalysis2->setCutSurfAnaType( SECT_VERTICAL );
			}
		}
		else
		{
			if ( _cutSurfaceAnalysis2 != NULL )
			{
				delete _cutSurfaceAnalysis2;
				_cutSurfaceAnalysis2 = NULL;
			}
		}
	}
	//----------------------------------------------------------------
	bool TerrainManager::getCutSurfaceAnalysisFlag() const
	{
		return _cutSurfaceAnalysis != NULL;
	}
	//----------------------------------------------------------------
	bool TerrainManager::getCutSurfaceAnalysisFlag2() const
	{
		return _cutSurfaceAnalysis2 != NULL;
	}
	//----------------------------------------------------------------
	void TerrainManager::setCutSurfaceVisibilityFlag( const bool& enable )
	{
		if ( enable == true )
		{
			// ע��,Ҫ��ɾ���ɵ�.
			// ��Ϊ��ϡ��������Ҫ����inputsystem��status
			// ɾ��������gis��������
			safeDeleteAllCutSurfaces();

			if ( _cutSurfaceVisibility == NULL )
			{
				_cutSurfaceVisibility = new CutSurfaceVisibility;
			}
		}
		else
		{
			if ( _cutSurfaceVisibility != NULL )
			{
				delete _cutSurfaceVisibility;
				_cutSurfaceVisibility = NULL;
			}
		}
	}
	//----------------------------------------------------------------
	bool TerrainManager::getCutSurfaceVisibilityFlag() const
	{
		return _cutSurfaceVisibility != NULL;
	}
	//----------------------------------------------------------------
	void TerrainManager::setCutSurfaceVisibilityFlag2( const bool& enable )
	{
		if ( enable == true )
		{
			// ע��,Ҫ��ɾ���ɵ�.
			// ��Ϊ��ϡ��������Ҫ����inputsystem��status
			// ɾ��������gis��������
			safeDeleteAllCutSurfaces();

			if ( _cutSurfaceVisibility2 == NULL )
			{
				_cutSurfaceVisibility2 = new CutSurfaceVisibility2;
			}
		}
		else
		{
			if ( _cutSurfaceVisibility2 != NULL )
			{
				delete _cutSurfaceVisibility2;
				_cutSurfaceVisibility2 = NULL;
			}
		}
	}
	//----------------------------------------------------------------
	bool TerrainManager::getCutSurfaceVisibilityFlag2() const
	{
		return _cutSurfaceVisibility2 != NULL;
	}
	//----------------------------------------------------------------
	void TerrainManager::setCutSurfaceLevellingFlag( const bool& enable )
	{
		if ( enable == true )
		{
			// ע��,Ҫ��ɾ���ɵ�.
			// ��Ϊ��ϡ��������Ҫ����inputsystem��status
			// ɾ��������gis��������
			safeDeleteAllCutSurfaces();

			if ( _cutSurfaceLevelling == NULL )
			{
				_cutSurfaceLevelling = new CutSurfaceLevelling;
			}
		}
		else
		{
			if ( _cutSurfaceLevelling != NULL )
			{
				delete _cutSurfaceLevelling;
				_cutSurfaceLevelling = NULL;
			}
		}
	}
	//----------------------------------------------------------------
	bool TerrainManager::getCutSurfaceLevellingFlag() const
	{
		return _cutSurfaceLevelling != NULL;
	}
	//----------------------------------------------------------------
	void TerrainManager::safeDeleteAllCutSurfaces()
	{
		if ( _cutSurfaceAnalysis != NULL )
		{
			delete _cutSurfaceAnalysis;
			_cutSurfaceAnalysis = NULL;
		}

		if ( _cutSurfaceAnalysis2 != NULL )
		{
			delete _cutSurfaceAnalysis2;
			_cutSurfaceAnalysis2 = NULL;
		}

		if ( _cutSurfaceVisibility != NULL )
		{
			delete _cutSurfaceVisibility;
			_cutSurfaceVisibility = NULL;
		}

		if ( _cutSurfaceVisibility2 != NULL )
		{
			delete _cutSurfaceVisibility2;
			_cutSurfaceVisibility2 = NULL;
		}

		if ( _cutSurfaceLevelling != NULL )
		{
			delete _cutSurfaceLevelling;
			_cutSurfaceLevelling = NULL;
		}

		if ( _surfaceLengthSurvey != NULL)
		{
			delete _surfaceLengthSurvey;
			_surfaceLengthSurvey = NULL;
		}

		if ( _surfaceAreaSurvey != NULL )
		{
			delete _surfaceAreaSurvey;
			_surfaceAreaSurvey = NULL;
		}

	}
	//----------------------------------------------------------------
	void TerrainManager::setBlendFlag( const bool& enable )
	{
		_blendRenderingFlag = enable;
	}
	//----------------------------------------------------------------
	bool TerrainManager::getBlendFlag() const
	{
		return _blendRenderingFlag;
	}
	//----------------------------------------------------------------
	void TerrainManager::setCutAreaFlag( const bool& enable )
	{
		if ( enable == true )
		{
			if ( _cutArea == NULL )
			{
				_cutArea = new CutArea;
			}
		}
		else
		{
			if ( _cutArea != NULL )
			{
				delete _cutArea;
				_cutArea = NULL;
			}
		}
	}
	//----------------------------------------------------------------
	bool TerrainManager::getCutAreaFlag() const
	{
		return _cutArea != NULL;
	}
	//----------------------------------------------------------------
#if 0
	long TerrainManager::writeInfoToVg( CFile &pfile )
	{
		long beg = pfile.GetPosition();

		bool hasdata = hasTerrainData();
		pfile.Write( &hasdata , sizeof(bool) );

		pfile.Write( &_blendRenderingFlag , sizeof(bool) );

		pfile.Write( &_collisionDetectedFlag , sizeof(bool) );

		pfile.Write( &_renderFlag , sizeof( bool ) );

		return pfile.GetPosition() - beg;
	}
	//----------------------------------------------------------------
	long TerrainManager::readInfoFromVg( CFile &pfile )
	{
		long beg = pfile.GetPosition();

		bool hasdata = false;
		pfile.Read( &hasdata , sizeof(bool) );

		pfile.Read( &_blendRenderingFlag , sizeof(bool) );

		pfile.Read( &_collisionDetectedFlag , sizeof(bool) );

		pfile.Read( &_renderFlag , sizeof( bool ) );

		return pfile.GetPosition() - beg;
	}
#endif
	//----------------------------------------------------------------
	//----------------------------------------------------------------
	void TerrainManager::readProject( const String& projpath , const String& projname  )
	{
		String terrain_path = VG_PROJCONF_GET_STRING( "Territory", "path" );

		if ( terrain_path == "" )
		{
			return;
		}

		bool openok = openVtrFile( projpath + "\\" + terrain_path );

		if ( openok == true )
		{
			_blendRenderingFlag = VG_PROJCONF_GET_BOOL( "Territory", "blend" );
			_collisionDetectedFlag = VG_PROJCONF_GET_BOOL( "Territory", "collision" );
			_renderFlag = VG_PROJCONF_GET_BOOL( "Territory", "renderFlag" );
		}
	}

	//----------------------------------------------------------------
	void TerrainManager::saveConfigToXml()
	{
		String terrain_path;

		if ( hasTerrainData() )
		{
			terrain_path =
				ElevationManager::getSingleton().getMainFileHandleVtr()->getFilePath();
		}
			
		VG_PROJCONF_SET( "Territory", "path" , terrain_path );
		VG_PROJCONF_SET( "Territory", "blend" , _blendRenderingFlag );
		VG_PROJCONF_SET( "Territory", "collision" , _collisionDetectedFlag );
		VG_PROJCONF_SET( "Territory", "renderFlag" , _renderFlag );

		return;
	}
	//----------------------------------------------------------------
	void TerrainManager::readConfigFromXml()
	{
		String terrain_path = VG_PROJCONF_GET_STRING( "Territory", "path" );

		if ( terrain_path == "" )
		{
			return;
		}

		bool openok = openVtrFile( terrain_path );

		if ( openok == true )
		{
			_blendRenderingFlag = VG_PROJCONF_GET_BOOL( "Territory", "blend" );
			_collisionDetectedFlag = VG_PROJCONF_GET_BOOL( "Territory", "collision" );
			_renderFlag = VG_PROJCONF_GET_BOOL( "Territory", "renderFlag" );
		}
	}
	//----------------------------------------------------------------
	//----------------------------------------------------------------
	void TerrainManager::saveProject( const String& projpath , const String& projname )
	{
		String terrain_path;

		if ( hasTerrainData() )
		{
			if ( !_bSelectMode )
			{
				terrain_path =
					ElevationManager::getSingleton().getMainFileHandleVtr()->getFilePath();

				saveData( projpath );

				terrain_path = vgKernel::StringUtility::getFilenameFromAbsolutePath( terrain_path );
			}
			else  // �п鱣��ģʽ
			{
				terrain_path = "terrain.vtr";
			}
		}

		VG_PROJCONF_SET( "Territory", "path" , terrain_path );
		VG_PROJCONF_SET( "Territory", "blend" , _blendRenderingFlag );
		VG_PROJCONF_SET( "Territory", "collision" , _collisionDetectedFlag );
		VG_PROJCONF_SET( "Territory", "renderFlag" , _renderFlag );

	}

	float* TerrainManager::getUserClipPlaneParam()
	{
		if ( _cutSurfaceAnalysis2 == NULL )
		{
			return NULL;
		}

		return _cutSurfaceAnalysis2->getUserClipPlaneParam();
	}
	//----------------------------------------------------------------

	//----------------------------------------------------------------
	//----------------------------------------------------------------
	vgImage::FileHandleVgi* ElevationManager::openVgiFile( 
		const String& vgifilename )
	{

		//------------------------------------------
		// �����ж��Ƿ��Ѿ�������
		//------------------------------------------
		FileHandleVgi* handlevgi =
			vgImage::ImageManager::getSingleton().getFileHandleVgi( vgifilename );

		if ( handlevgi != NULL )
		{
			_mainVgiFilehandle = handlevgi;
			return _mainVgiFilehandle;
		}

		//------------------------------------------
		// ��ʼ����
		//------------------------------------------
		bool importok = importImageFileOnly( vgifilename );

		if ( importok == false )
		{
			return NULL;
		}

		FileHandleVgi* handle =
			vgImage::ImageManager::getSingleton().getFileHandleVgi( vgifilename );

		_mainVgiFilehandle = handle;


		//------------------------------------------
		// ��ʼ���������ģ����ͼ��
		//------------------------------------------
		// ����,��Ҫʹ��ͬ��ģʽ
		vgImage::TextureIoRequestDispatcher::getSingleton().setAsynMode( false );

		assert( _mainVgiFilehandle != NULL );
		if ( _mainVgiFilehandle != NULL )
		{
			vgImage::FileHandleVgi* handle = _mainVgiFilehandle;

			do 
			{
				vgImage::TextureManager::getSingleton().addRefToTexturesBySpecificLod(
					handle, VGLOD_COARSE + 1);

				String nextfile = handle->getNextFilePath();

				handle = vgImage::ImageManager::getSingleton().getFileHandleVgi( nextfile );

			} while( handle != NULL );
		}

		// �ύ�޸�,��������
		vgImage::TextureIoRequestDispatcher::getSingleton().dealWithUpdateInfoEveryFrame();

		// �����첽ģʽ
		vgImage::TextureIoRequestDispatcher::getSingleton().setAsynMode( true );

		return _mainVgiFilehandle;


#if 0
		assert( _vgiFilehandle == NULL );

		_vgiFilehandle = 
			vgImage::ImageManager::getSingleton().addImageAndAssociateWithIoServer(
			vgifilename);

		if( _vgiFilehandle == NULL )
		{
			return NULL;
		}

		bool add_rest = 
			vgImage::TextureManager::getSingleton().addTexture( _vgiFilehandle );

		if( add_rest == false )
		{
			return NULL;
		}


		//////////////////////////////////////////////////////////////////////////
		//------------------------------------------
		// test
		//------------------------------------------

		vgImage::TextureManager::getSingleton().addRefToTexturesBySpecificLod(
			_vgiFilehandle, VGLOD_CLEAR );

		//////////////////////////////////////////////////////////////////////////

#endif

		//return vgifilehandle;
	}
	//-------------------------------------------------------------------------
	void TerrainManager::setSurfaceLengthSurvey( const bool& enable )
	{
		if ( enable == true )
		{
			// ע��,Ҫ��ɾ���ɵ�.
			// ��Ϊ��ϡ��������Ҫ����inputsystem��status
			safeDeleteAllCutSurfaces();

			if ( _surfaceLengthSurvey == NULL )
			{
				_surfaceLengthSurvey = new SurfaceLengthSurvey;

			}
		}
		else
		{
			if ( _surfaceLengthSurvey != NULL )
			{
				delete _surfaceLengthSurvey;
				_surfaceLengthSurvey = NULL;

			}
		}
	}
	//--------------------------------------------------------------------
	bool TerrainManager::getSurfaceLengthSurvey() const
	{
		return _surfaceLengthSurvey != NULL;
	}
	//---------------------------------------------------------------------
	void TerrainManager::setSurfaceAreaSurvey( const bool& enable )
	{
		if ( enable == true )
		{
			// ע��,Ҫ��ɾ���ɵ�.
			// ��Ϊ��ϡ��������Ҫ����inputsystem��status
			safeDeleteAllCutSurfaces();

			if ( _surfaceAreaSurvey == NULL )
			{
				_surfaceAreaSurvey = new SurfaceAreaSurvey;
				flagDlgCellSize = true;
				// Edit By FengYK @2010-05-10 �޸ĵ��α����������ʱ�Զ������ӽǵ�����
				//vgKernel::Vec3 m_postion = vgCam::CamManager::getSingleton().getCurrentPosition();
				//vgKernel::Vec3 m_direction = vgCam::CamManager::getSingleton().getCurrentDirection();
				//float m_forward = vgCam::CamManager::getSingleton().getForwardVelocity();
				//float m_pitch = vgCam::CamManager::getSingleton().getPitchDegrees();
				//vgCam::CamManager::getSingleton().setPitchDegrees(50);
				//m_postion.y = 2500;
				//vgCam::CamManager::getSingleton().setCurrentPosition(m_postion);
			}
		}
		else
		{
			if ( _surfaceAreaSurvey != NULL )
			{
				delete _surfaceAreaSurvey;
				_surfaceAreaSurvey = NULL;
				flagDlgCellSize = false;
			}
		}
	}
	//-----------------------------------------------------------------
	bool TerrainManager::getSurfaceAreaSurvey() const
	{
		return _surfaceAreaSurvey != NULL;
		
	}
	//------------------------------------------------------------------
	bool TerrainManager::saveSelectVtrAndVgi( const String& folderpath )
	{
		FileWriterVtr writer;

		String terrainname = "terrain"; 
		String path = vgKernel::StringUtility::getPathFromAbsoluteFilename( folderpath );

		assert( _cutArea != NULL );

		bool writeok = writer.writeToVtrFromItems( 
			ElevationManager::getSingleton().getMainFileHandleVtr(),
			terrainname ,  /*
			ElevationManager::getSingleton().getIntersectedItemsByBound( _cutArea->getSelectBound() ) ,*/
			ElevationManager::getSingleton().getIntersectedItemsByPolygon( _cutArea->getClickPoints() ),
			path ); 

		if ( writeok == true )
		{
			return true;
		}
		else
		{
			String msg = "����vtr��vgiʧ�ܣ�";
			vgKernel::SystemUtility::showModelDialog( msg );
			return false;
		}

	}

	//------------------------------------------------------------------
	bool TerrainManager::hasCutArea()
	{
		if ( _cutArea != NULL )
		{
			Vec3Vector* vec = _cutArea->getClickPoints();
			assert( vec != NULL );

			return vec->size() >= 3;
		} 

		return false;
	}

	//------------------------------------------------------------------
	void TerrainManager::showProcessingDlg(bool show)
	{
		assert(m_processingDlg != NULL);

		if (show)
		{
			m_processingDlg->ShowWindow(SW_SHOW);
		}
		else
		{
			m_processingDlg->ShowWindow(SW_HIDE);
		}
	}
	//------------------------------------------------------------------
	void TerrainManager::setProcessingInfo(std::string info)
	{
		assert(m_processingDlg != NULL);

		m_processingDlg->setInfo(info);
	}
	//------------------------------------------------------------------
	void TerrainManager::setProcessingProgress(int pos)
	{
		assert( m_processingDlg != NULL );

		m_processingDlg->setProgress(pos);
	}
	//------------------------------------------------------------------
	void TerrainManager::setVerSectionAnaFlag( const bool& enable )
	{
		if ( enable == true )
		{
			// ע��,Ҫ��ɾ���ɵ�.
			// ��Ϊ��ϡ��������Ҫ����inputsystem��status
			safeDeleteAllCutSurfaces();

			if ( _cutSurfaceAnalysis2 == NULL )
			{
				_cutSurfaceAnalysis2 = new CutSurfaceAnalysis2;
				_cutSurfaceAnalysis2->setCutSurfAnaType( SECT_VERTICAL );
			}
			else
			{
				_cutSurfaceAnalysis2->setCutSurfAnaType( SECT_VERTICAL );
			}
		}
		else
		{
			if ( _cutSurfaceAnalysis2 != NULL )
			{
				delete _cutSurfaceAnalysis2;
				_cutSurfaceAnalysis2 = NULL;
			}
		}
	}
	//------------------------------------------------------------------
	bool TerrainManager::getVerSectionAnaFlag() const
	{
		if ( _cutSurfaceAnalysis2 != NULL )
		{
			if ( _cutSurfaceAnalysis2->_cutSufSecType == SECT_VERTICAL )
			{
				return true;
			}
		}
		return false;
	}
	//------------------------------------------------------------------
	void TerrainManager::setHorSectionAnaFlag( const bool& enable )
	{
		if ( enable == true )
		{
			// ע��,Ҫ��ɾ���ɵ�.
			// ��Ϊ��ϡ��������Ҫ����inputsystem��status
			safeDeleteAllCutSurfaces();

			if ( _cutSurfaceAnalysis2 == NULL )
			{
				_cutSurfaceAnalysis2 = new CutSurfaceAnalysis2;
				_cutSurfaceAnalysis2->setCutSurfAnaType( SECT_HORIZONTAL );
			}
			else
			{
				_cutSurfaceAnalysis2->setCutSurfAnaType( SECT_HORIZONTAL );
			}
		}
		else
		{
			if ( _cutSurfaceAnalysis2 != NULL )
			{
				delete _cutSurfaceAnalysis2;
				_cutSurfaceAnalysis2 = NULL;
			}
		}
	}
	//------------------------------------------------------------------
	bool TerrainManager::getHorSectionAnaFlag() const
	{
		if ( _cutSurfaceAnalysis2 != NULL )
		{
			if ( _cutSurfaceAnalysis2->_cutSufSecType == SECT_HORIZONTAL )
			{
				return true;
			}
		}
		return false;
	}
}// end of namespace vgTerritory
