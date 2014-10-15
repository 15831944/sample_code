


#include <vgStableHeaders.h>
#include <vgTerritory/vgtGeoDemManager.h>
#include <vgImage/vgiImageUtility.h>
#include <vgTerritory/vgTerrainHugeImageProcessor.h>
#include <vgTerritory/vgtFileWriterVgtrToVtr.h>
#include <gdal/gdal_priv.h>
#include <gdal/cpl_multiproc.h>
#include <vgImage/vgiFileWriterDdsToVgi.h>
#include <vgTerritory/vgtHugeProcessHugeJobs.h>
#include <vgImage/vgiFreeImageWrapper.h>



namespace vgTerritory {
	
	
	//----------------------------------------------------------------
	GeoDemManager::GeoDemManager() : vgKernel::Singleton<GeoDemManager>( 
		VGT_SINGLETON_LEFE_GEODEMMGR )
	{
		setDefault();
	}
	//----------------------------------------------------------------
	GeoDemManager::~GeoDemManager()
	{
		clearData();

		if ( _originGeoBlocks != NULL )
		{
			GeoBlockPtrVec::iterator iter = _originGeoBlocks->begin();
			GeoBlockPtrVec::iterator iter_end = _originGeoBlocks->end();

			for ( ; iter != iter_end ; ++iter )
			{
				delete *iter;
			}

			delete _originGeoBlocks;
			_originGeoBlocks = NULL;
		}

		if ( _debugOrginGeoBlocks != NULL )
		{
			GeoBlockPtrVec::iterator iter = _debugOrginGeoBlocks->begin();
			GeoBlockPtrVec::iterator iter_end = _debugOrginGeoBlocks->end();

			for ( ; iter != iter_end ; ++iter )
			{
				delete *iter;
			}

			delete _debugOrginGeoBlocks;
			_debugOrginGeoBlocks = NULL;
		}

		if ( _originalImageBlocks != NULL )
		{
			GeoImageBlockPtrVec::iterator iter = _originalImageBlocks->begin();
			GeoImageBlockPtrVec::iterator iter_end = _originalImageBlocks->end();

			for ( ; iter != iter_end ; ++iter )
			{
				delete *iter;
			}

			delete _originalImageBlocks;
			_originalImageBlocks = NULL;
		}
	}
	//----------------------------------------------------------------
	
	//----------------------------------------------------------------
	void GeoDemManager::clearData()
	{
		GeoDemMap::iterator iter = _dems.begin();
		GeoDemMap::iterator iter_end = _dems.end();

		for ( ; iter != iter_end ; ++ iter  )
		{
			delete iter->second;
		}

		_dems.clear();

		GeoImageMap::iterator iiter = _images.begin();
		GeoImageMap::iterator iiter_end  = _images.end();

		for ( ; iiter != iiter_end ; ++ iiter )
		{
			delete iiter->second;
		}

		_images.clear();

		setDefault();
	}
	//----------------------------------------------------------------
	bool GeoDemManager::addGeoDemAndCheckValid( const vgKernel::StringVectorPtr pdems )
	{
		assert( pdems.isNull() == false );
		assert( pdems->empty() == false );

		int size = pdems->size();
		int counter = 0;
		this->notifyProcessObserversStart( "��ʼ���dem����..." , 0 , size );

		vgKernel::StringVector::iterator iter = pdems->begin();
		vgKernel::StringVector::iterator iter_end = pdems->end();

		for ( ; iter != iter_end ; ++iter )
		{
			String curname = *iter ;

			this->notifyProcessObserversPos( curname ,  counter++ );
			//------------------------------------------
			// ����dem
			//------------------------------------------
			GeoDem* dem = new GeoDem( curname , &_globalData );

			bool openok = dem->doOpen();

			if ( openok == false )
			{
				std::ostringstream o;
				o << "���ļ�:" << curname << "����,�����������ļ�.";
				//MessageBox( NULL , 
				//	o.str().c_str() , "����dem����" , 
				//	MB_ICONINFORMATION | MB_OK );

				VGK_TRACE( o.str( ) );

				delete dem;
				continue;
			}

			bool addres = addGeoDemAndCheckValid( dem );

			if ( addres == false )
			{
				std::ostringstream o;
				o << "�����ļ�:" << curname << "����,���ļ��ѱ�����.�����������ļ�,����ִ��.";
				//MessageBox( NULL , 
				//	o.str().c_str() , "����dem����" , 
				//	MB_ICONINFORMATION | MB_OK );

				VGK_TRACE( o.str( ) );
				delete dem;
			}

		}// end for

		if ( checkDemsValid() == false )
		{
			clearData();
			return false;
		}

		this->notifyProcessObserversPos( "update dems..." ,  counter++ );

		updateGeoDemsFirst();

#if 0
		logGeoDemsDetails();
#endif

		this->notfiyProcessObserversFinish("���dem���.");

		return true;
	}
	//----------------------------------------------------------------
	bool GeoDemManager::initialise()
	{
		CPLSetErrorHandler( GeoDemManager::gdalErrorHandler );
		GDALAllRegister();

		return true;
	}
	//----------------------------------------------------------------
	void CPL_STDCALL GeoDemManager::gdalErrorHandler( CPLErr eErrClass, int err_no,
		const char *msg )
	{
		if ( eErrClass == CE_Fatal )
		{
			std::ostringstream o;
			o << "CPLErr: " << eErrClass << " , err_no:" << err_no
				<< " , msg: " << msg;
			VGK_TRACE( o.str() );

			MessageBox( NULL , o.str().c_str() , "Error" , MB_OK );
		}
		else 
		{
			std::ostringstream o;
			o << "CPLErr: " << eErrClass << " , err_no:" << err_no
				<< " , msg: " << msg;
			VGK_TRACE( o.str() );
		}
	}
	//----------------------------------------------------------------
	bool GeoDemManager::shutdown()
	{
		clearData();

		GDALDumpOpenDatasets( stderr );

		GDALDestroyDriverManager();

		CPLDumpSharedList( NULL );
		CPLCleanupTLS();


		return true;
	}
	//----------------------------------------------------------------
	void GeoDemManager::updateGeoDemsFirst()
	{
		assert( _dems.empty() == false );

		//------------------------------------------
		// ���ʱ���Ѿ�checkValid����.
		// ����PixelSize
		//------------------------------------------
		_globalData._globalPixelSizeX = _dems.begin()->second->_pixelSizeX;
		_globalData._globalPixelSizeY = _dems.begin()->second->_pixelSizeY;

		_globalData._globalAdjustCellsize = _globalData._globalPixelSizeX;

		//------------------------------------------
		// �ж�������
		//------------------------------------------
		if ( _globalData._globalPixelSizeY < 0.0 )
		{
			_globalData._axisLeftDown = true;
		}
		else
		{
			_globalData._axisLeftDown = false;
		}

		//------------------------------------------
		// ����globalԭ���elevation
		//------------------------------------------
		double tmpMaxX = VGT_MAX( 
			_dems.begin()->second->_upperLeftX , 
			_dems.begin()->second->_lowerRightX );

		double tmpMinX = VGT_MIN( 
			_dems.begin()->second->_upperLeftX , 
			_dems.begin()->second->_lowerRightX );

		double tmpMaxY = VGT_MAX( 
			_dems.begin()->second->_upperLeftY , 
			_dems.begin()->second->_lowerRightY );

		double tmpMinY = VGT_MIN( 
			_dems.begin()->second->_upperLeftY , 
			_dems.begin()->second->_lowerRightY );


		GeoDemMap::iterator iter = _dems.begin();
		GeoDemMap::iterator iter_end = _dems.end();

		for ( ; iter != iter_end ; ++ iter  )
		{
			GeoDem* curdem = iter->second;

			// ����������������ֵ����Сֵ

			tmpMaxX = VGT_MAX( 	tmpMaxX , 
				VGT_MAX( curdem->_lowerRightX , curdem->_upperLeftX ) );

			tmpMinX = VGT_MIN( 	tmpMinX , 
				VGT_MIN( curdem->_lowerRightX , curdem->_upperLeftX ) );

			tmpMaxY = VGT_MAX( 	tmpMaxY , 
				VGT_MAX( curdem->_lowerRightY , curdem->_upperLeftY ) );

			tmpMinY = VGT_MIN( 	tmpMinY , 
				VGT_MIN( curdem->_lowerRightY , curdem->_upperLeftY ) );
		}

		assert( tmpMinY < tmpMaxY );
		assert( tmpMinX < tmpMaxX );
		assert( _globalData._globalPixelSizeX > 0 );



		_globalData._globalUpperLeftX = tmpMinX;
		_globalData._globalLowerRightX = tmpMaxX;

		if ( _globalData._axisLeftDown == true )
		{
			_globalData._globalUpperLeftY = tmpMaxY;
			_globalData._globalLowerRightY = tmpMinY;
		}
		else
		{
			_globalData._globalUpperLeftY = tmpMinY;
			_globalData._globalLowerRightY = tmpMaxY;
		}

		//------------------------------------------
		// ����rect��Ϣ
		//------------------------------------------
		_globalData._globalRect.maxPos.x = 
			abs( _globalData._globalUpperLeftX - _globalData._globalLowerRightX ) /
			abs( _globalData._globalPixelSizeX );
		_globalData._globalRect.maxPos.x -= 1;
		assert( _globalData._globalRect.maxPos.x > 0 );

		_globalData._globalRect.maxPos.y = 
			abs( _globalData._globalUpperLeftY - _globalData._globalLowerRightY ) /
			abs( _globalData._globalPixelSizeY );
		_globalData._globalRect.maxPos.y -= 1;
		assert( _globalData._globalRect.maxPos.y > 0 );

		// 2��n�η���С
		long two_pow_n_xnum = 
			vgImage::ImageUtility::getBiggerPowerOfTwoNumber( 
			_globalData._globalRect.getWidth() + 1 );

		long two_pow_n_ynum =
			vgImage::ImageUtility::getBiggerPowerOfTwoNumber( 
			_globalData._globalRect.getHeight() + 1 );

		long last_num = VGT_MAX( two_pow_n_xnum , two_pow_n_ynum );

		assert( last_num > 0 );
		//Ҫ��ȫ������
		_globalData._globalAdjustRect.maxPos.x = last_num - 1;
		_globalData._globalAdjustRect.maxPos.y = last_num - 1;



		assert( _globalData._globalPixelSizeX > 0 );


		// ע�⣬���Ҫ������overview�Ļ�����Ҫ������һ�顣
		if ( _globalData._genElevationOverview == true )
		{
			updateGeoDemsSecond();
		}
	

		return;
	}
	//----------------------------------------------------------------
	void GeoDemManager::updateGeoDemsSecond()
	{
		if ( _globalData.isValid() == true )
		{
			return;
		}

		assert( _dems.empty() == false );

		int size = _dems.size();
		int counter = 0;
		this->notifyProcessObserversStart( "��ʼԤ��ȡdem����..." , 0 , size );


		// ����ȫ����Ϣ
		{
			GeoDemMap::iterator iter = _dems.begin();
			GeoDemMap::iterator iter_end = _dems.end();

			for ( ; iter != iter_end ; ++ iter  )
			{
				GeoDem* curdem = iter->second;

				this->notifyProcessObserversPos( "Ԥ��ȡ: " + curdem->getFilePath() ,  counter++ );

				curdem->readGlobalInfo( true );
			}
		}

		//------------------------------------------
		// ���ʱ���Ѿ�checkValid����.
		// ����PixelSize
		//------------------------------------------
		_globalData._globalPixelSizeX = _dems.begin()->second->_pixelSizeX;
		_globalData._globalPixelSizeY = _dems.begin()->second->_pixelSizeY;

		_globalData._globalAdjustCellsize = _globalData._globalPixelSizeX;

		//------------------------------------------
		// �ж�������
		//------------------------------------------
		if ( _globalData._globalPixelSizeY < 0.0 )
		{
			_globalData._axisLeftDown = true;
		}
		else
		{
			_globalData._axisLeftDown = false;
		}

		//------------------------------------------
		// ����globalԭ���elevation
		//------------------------------------------
		double tmpMaxX = VGT_MAX( 
			_dems.begin()->second->_upperLeftX , 
			_dems.begin()->second->_lowerRightX );

		double tmpMinX = VGT_MIN( 
			_dems.begin()->second->_upperLeftX , 
			_dems.begin()->second->_lowerRightX );

		double tmpMaxY = VGT_MAX( 
			_dems.begin()->second->_upperLeftY , 
			_dems.begin()->second->_lowerRightY );

		double tmpMinY = VGT_MIN( 
			_dems.begin()->second->_upperLeftY , 
			_dems.begin()->second->_lowerRightY );

		double tmpMaxElevation = _dems.begin()->second->_maxElevation;
		double tmpMinElevation = _dems.begin()->second->_minElevation;

		GeoDemMap::iterator iter = _dems.begin();
		GeoDemMap::iterator iter_end = _dems.end();

		for ( ; iter != iter_end ; ++ iter  )
		{
			GeoDem* curdem = iter->second;

			// ����������������ֵ����Сֵ

			tmpMaxX = VGT_MAX( 	tmpMaxX , 
				VGT_MAX( curdem->_lowerRightX , curdem->_upperLeftX ) );

			tmpMinX = VGT_MIN( 	tmpMinX , 
				VGT_MIN( curdem->_lowerRightX , curdem->_upperLeftX ) );

			tmpMaxY = VGT_MAX( 	tmpMaxY , 
				VGT_MAX( curdem->_lowerRightY , curdem->_upperLeftY ) );

			tmpMinY = VGT_MIN( 	tmpMinY , 
				VGT_MIN( curdem->_lowerRightY , curdem->_upperLeftY ) );

			tmpMaxElevation = VGT_MAX( tmpMaxElevation ,
				curdem->_maxElevation );

			tmpMinElevation = VGT_MIN( tmpMinElevation ,
				curdem->_minElevation );
		}

		assert( tmpMinY < tmpMaxY );
		assert( tmpMinX < tmpMaxX );
		assert( _globalData._globalPixelSizeX > 0 );

		_globalData._globalMinElevation = tmpMinElevation;
		_globalData._globalMaxElevation = tmpMaxElevation;

		_globalData._globalElevationScale = 
			(_globalData._globalMaxElevation - _globalData._globalMinElevation);

		_globalData._globalUpperLeftX = tmpMinX;
		_globalData._globalLowerRightX = tmpMaxX;

		if ( _globalData._axisLeftDown == true )
		{
			_globalData._globalUpperLeftY = tmpMaxY;
			_globalData._globalLowerRightY = tmpMinY;
		}
		else
		{
			_globalData._globalUpperLeftY = tmpMinY;
			_globalData._globalLowerRightY = tmpMaxY;
		}

		//------------------------------------------
		// ����rect��Ϣ
		//------------------------------------------
		_globalData._globalRect.maxPos.x = 
			abs( _globalData._globalUpperLeftX - _globalData._globalLowerRightX ) /
			abs( _globalData._globalPixelSizeX );
		_globalData._globalRect.maxPos.x -= 1;
		assert( _globalData._globalRect.maxPos.x > 0 );

		_globalData._globalRect.maxPos.y = 
			abs( _globalData._globalUpperLeftY - _globalData._globalLowerRightY ) /
			abs( _globalData._globalPixelSizeY );
		_globalData._globalRect.maxPos.y -= 1;
		assert( _globalData._globalRect.maxPos.y > 0 );

		// 2��n�η���С
		long two_pow_n_xnum = 
			vgImage::ImageUtility::getBiggerPowerOfTwoNumber( 
			_globalData._globalRect.getWidth() + 1 );

		long two_pow_n_ynum =
			vgImage::ImageUtility::getBiggerPowerOfTwoNumber( 
			_globalData._globalRect.getHeight() + 1 );

		long last_num = VGT_MAX( two_pow_n_xnum , two_pow_n_ynum );

		assert( last_num > 0 );
		//Ҫ��ȫ������
		_globalData._globalAdjustRect.maxPos.x = last_num - 1;
		_globalData._globalAdjustRect.maxPos.y = last_num - 1;


		// ����valid���λ
		_globalData.setValid( true );

		//------------------------------------------
		// ����dems��������
		//------------------------------------------
		{
			GeoDemMap::iterator iter = _dems.begin();
			GeoDemMap::iterator iter_end = _dems.end();

			for ( ; iter != iter_end ; ++ iter  )
			{
				GeoDem* curdem = iter->second;
				curdem->updateGlobalInfo();
			}
		}


		assert( _globalData._globalPixelSizeX > 0 );


		this->notfiyProcessObserversFinish("Ԥ��ȡdem���.");

		return;
	}

	//----------------------------------------------------------------
	bool GeoDemManager::checkDemsValid()
	{
		assert( _dems.empty() == false );

		if ( _dems.empty() == true )
		{
			return false;
		}

		//------------------------------------------
		// ���ȼ��cellsize�Ƿ�һ�� 
		//------------------------------------------
		double pixel_x = _dems.begin()->second->_pixelSizeX;
		double pixel_y = _dems.begin()->second->_pixelSizeY;

		GeoDemMap::iterator iter = _dems.begin();
		GeoDemMap::iterator iter_end = _dems.end();

		for ( ; iter != iter_end ; ++ iter  )
		{
			GeoDem* curdem = iter->second;

			if ( pixel_x != curdem->_pixelSizeX || 
				pixel_y != curdem->_pixelSizeY )
			{
				return false;
			}
		}

		return true;
	}
	//----------------------------------------------------------------
	void GeoDemManager::setDefault()
	{
		assert( _dems.empty() == true );


		_originGeoBlocks = NULL;
		_debugOrginGeoBlocks = NULL;

		_originalImageBlocks = NULL;
	}
	//----------------------------------------------------------------
	void GeoDemManager::logGeoDemsDetails()
	{
		GeoDemMap::iterator iter = _dems.begin();
		GeoDemMap::iterator iter_end = _dems.end();

		for ( ; iter != iter_end ; ++ iter  )
		{
			VGK_TRACE( iter->second->getDetails() );
		}

		VGK_TRACE( _globalData.getDetails() );

		return;
	}
	//----------------------------------------------------------------
	bool GeoDemManager::saveDemToImages()
	{
		int size = _dems.size();
		int counter = 0;
		this->notifyProcessObserversStart( "��ʼ����dem����..." , 0 , size );

		fileBlockVectorPtr fileblocks( new fileBlockVector );
		CriticalSection fileblocks_sect;

#if 0
		GeoDemMap::iterator iter = _dems.begin();
		GeoDemMap::iterator iter_end = _dems.end();

		for ( ; iter != iter_end ; ++ iter )
		{
			GeoDem* curdem = iter->second;

			this->notifyProcessObserversPos( "����: " + curdem->getFilePath() ,  counter++ );

			fileBlock* block = curdem->saveToImage();

			assert( block != NULL );
			fileblocks->push_back( *block );

			delete block;
		}
#endif

		GeoDemMap::iterator iter = _dems.begin();
		GeoDemMap::iterator iter_end = _dems.end();

		using namespace HugeProcess;
		HugeJobs boss( this );

		for ( ; iter != iter_end ; ++ iter )
		{
			GeoDem* curdem = iter->second;

			GeoDemSaveImgJob* job = new GeoDemSaveImgJob(
				&boss , curdem , &fileblocks_sect , this , fileblocks.getPointer() );

			boss.submit( job );
		}

		boss.doAndWait();


		//------------------------------------------
		// ��ʼ������ԭʼ������.
		// ���������Ҫ��2��n�η�Ϊ��С
		//------------------------------------------
		if ( fileblocks->empty() == true )
		{
			return true;
		}

		bool setdirok = vgKernel::SystemUtility::setCurrentDirectory( 
			_globalData._demDirectory );
		assert( setdirok == true );

		vgTerritory::HugeImagesProcessor imgprocessor;
		vgTerritory::HugeImageBlockInfoVectorPtr vgtrinfovec;

		imgprocessor.registerProcessObs( getProcessObs() );

		imgprocessor.initialiseFileBlocks( fileblocks );

		// ע����������ɫ����
		imgprocessor.initialiseParameters( 
			_globalData._filledBlackColor,
			 _globalData._clearTempData, VGT_GEOMGR_TEMP_EXT,
			_globalData._demDirectory,
			_globalData._clearTempData,
			_globalData._terrainName + "_origin0" , 
			VGT_ORIGIN_IMG_SIZE , VGT_ORIGIN_IMG_SIZE ,
			VGT_GEOMGR_TEMP_EXT , VGT_GEOMGR_TEMP_EXT ,false,true,false,true
			,false,"",false,"",
			false, //�Ƿ�д�ϵ�����Ϣ
			vgKernel::ImagePos::ZERO ,0, -1);

		//imgprocessor.makeProcessing();
		imgprocessor.makeProcessingAndSaving();

		vgtrinfovec = imgprocessor.getHugeImageBlockInfos();



		assert( _originGeoBlocks  ==  NULL );

		_originGeoBlocks = new GeoBlockPtrVec;

		HugeImageBlockInfoVector::iterator biter = vgtrinfovec->begin();
		HugeImageBlockInfoVector::iterator biter_end = vgtrinfovec->end();

		for ( ; biter != biter_end ; ++biter )
		{
			HugeImageBlockInfo info = *biter;

			String img = _globalData._demDirectory + "\\" + info.m_BlockFileName;

			if( vgKernel::SystemUtility::checkFileExist( img ) == true )
			{
				GeoBlock* block = new GeoBlock( 
					getGlobalDataPtr() , img
				 );

				bool openok = block->open();
				if ( openok == false )
				{
					delete block;
					continue;
				}

				//////////////////////////////////////////////////////////////////////////
				//String ddd;
				//bool saveok = block->saveToVgtr( ddd );
				//assert( saveok == true );
				//////////////////////////////////////////////////////////////////////////
				_originGeoBlocks->push_back( block );
			}
			else
			{
				//assert(0);
			}
		}


		this->notfiyProcessObserversFinish("ת��dem���.");


		this->notifyProcessObserversStart( "��ʼɾ����ʱ�ļ�..." , 0 , 1 );

		//------------------------------------------
		// �������ԭʼ������
		//------------------------------------------
		{
			GeoDemMap::iterator iter = _dems.begin();
			GeoDemMap::iterator iter_end = _dems.end();

			for ( ; iter != iter_end ; ++ iter )
			{
				_globalData.pushNeedToCleanFile( (*iter).second->getImageFilePath() );
				_globalData.pushNeedToCleanFile( (*iter).second->getDebugImageFilePath() );
				delete iter->second;
			}
			_dems.clear();
		}

		_globalData.cleanTempFiles();

		this->notfiyProcessObserversFinish("ɾ����ʱ�ļ����.");

		return true;
	}
	//----------------------------------------------------------------
	bool GeoDemManager::saveToDebugImages()
	{
		if ( _globalData._genDebugImages == false )
		{
			return true;
		}

		if ( _dems.empty() == true )
		{
			return true;
		}

		fileBlockVectorPtr fileblocks( new fileBlockVector );

		GeoDemMap::iterator iter = _dems.begin();
		GeoDemMap::iterator iter_end = _dems.end();

		for ( ; iter != iter_end ; ++ iter )
		{
			GeoDem* curdem = iter->second;
			fileBlock* block = curdem->saveToDebugImage();

			assert( block != NULL );
			fileblocks->push_back( *block );

			delete block;
		}

		//------------------------------------------
		// ��ʼ������ԭʼ������.
		// ���������Ҫ��2��n�η�Ϊ��С
		//------------------------------------------
		bool setdirok = vgKernel::SystemUtility::setCurrentDirectory( 
			_globalData._debugImageDirectory );
		assert( setdirok == true );

		vgTerritory::HugeImagesProcessor imgprocessor;
		vgTerritory::HugeImageBlockInfoVectorPtr vgtrinfovec;

		imgprocessor.registerProcessObs( getProcessObs() );

		imgprocessor.initialiseFileBlocks( fileblocks );

		imgprocessor.initialiseParameters( 
			_globalData._filledColor,
			_globalData._clearTempData, VGT_GEOMGR_TEMP_EXT,
			_globalData._debugImageDirectory,
			_globalData._clearTempData,
			_globalData._terrainName + "_debug_origin0" , 
			VGT_ORIGIN_IMG_SIZE , VGT_ORIGIN_IMG_SIZE ,
			VGT_GEOMGR_TEMP_EXT, VGT_GEOMGR_TEMP_EXT,false,true,false,true
			,false,"",false,"",
			false, //�Ƿ�д�ϵ�����Ϣ
			vgKernel::ImagePos::ZERO ,0, -1);

		//imgprocessor.makeProcessing();
		imgprocessor.makeProcessingAndSaving();

		vgtrinfovec = imgprocessor.getHugeImageBlockInfos();

		assert( _debugOrginGeoBlocks  ==  NULL );

		_debugOrginGeoBlocks = new GeoBlockPtrVec;

		HugeImageBlockInfoVector::iterator biter = vgtrinfovec->begin();
		HugeImageBlockInfoVector::iterator biter_end = vgtrinfovec->end();

		for ( ; biter != biter_end ; ++biter )
		{
			HugeImageBlockInfo info = *biter;

			String img = _globalData._debugImageDirectory + "\\" + info.m_BlockFileName;

			if( vgKernel::SystemUtility::checkFileExist( img ) == true )
			{
				GeoBlock* block = new GeoBlock( 
					getGlobalDataPtr() , img
				 );

				bool openok = block->open();
				if ( openok == false )
				{
					delete block;
					continue;
				}

				_debugOrginGeoBlocks->push_back( block );
			}
			else
			{
				//assert(0);
			}
		}

		////------------------------------------------
		//// �ŵ�ɾ�������� 
		////------------------------------------------
		//{
		//	fileBlockVector::iterator iter = fileblocks->begin();
		//	fileBlockVector::iterator iter_end = fileblocks->end();
		//	for ( ; iter != iter_end ; ++ iter )
		//	{
		//		_globalData.pushNeedToCleanFile( (*iter).fileSrcName );
		//	}
		//}


		_globalData.cleanTempFiles();

		return true;
	}
	//----------------------------------------------------------------
	bool GeoDemManager::saveVgtrLayers()
	{
		assert( _originGeoBlocks != NULL );
		assert( _originGeoBlocks->empty() == false );

		if ( getOriginFileBlocks()->empty() == true )
		{
			return true;
		}

		int layernum = getLayerNum();
		assert( layernum != 0 && layernum != -1 );
		if ( layernum == 0 || layernum == -1 )
		{
			MessageBox( NULL , "��ֵ����" , "Error" , MB_OK );
			return false;
		}

		StringVectorPtr vgtrlist( new StringVector );
		StringVectorPtr debugimagelist( new StringVector );

		bool needResize = true;

		//------------------------------------------
		// ��ʼ���ɸ���layer
		//------------------------------------------
		for ( int i = 0 ; i < layernum ; ++ i )
		{
			if ( i == layernum - 1 )
			{
				needResize = false;
			}

			//assert(0&&"��ʼ����generateLayerImpl");

			generateDemLayerImpl( i , vgtrlist , debugimagelist );

			if ( needResize == true )
			{
				//assert(0&&"��ʼ resizeOriginImages");

				resizeOriginImages();
			}
		}

		FileWriterVgtrToVtr vtrwriter;

		vtrwriter.registerProcessObs( getProcessObs() );

		vtrwriter.writeToVtrFileFromVgtrNames( 
			_globalData.getVgtrHeaderPtr(), vgtrlist , 
			_globalData._globalOutputPath + "\\" + _globalData._terrainName + ".vtr" ,
			_globalData._filledColor );
		

		//////////////////////////////////////////////////////////////////////////
		// ���Ƿ���Ҫ����debug ��img
		if ( _globalData._genDebugImages == true )
		{
			StringVectorPtr debugddslist( new StringVector );
			StringVector::iterator iter = debugimagelist->begin();
			StringVector::iterator iter_end = debugimagelist->end();

			for ( ; iter != iter_end ; ++iter )
			{
				String src = *iter;
				String path = StringUtility::getPathFromAbsoluteFilename( src );
				String file = StringUtility::getFilenameWithoutExtFromAbsPath( src );
				String ddsname = path + "\\" + file + ".dds";

#define imageWrapper FreeImageWrapper

				imageWrapper img;
				bool loadok = img.loadFromFile( src );
				if ( loadok == false )
				{
					continue;
				}

				bool saveok = img.saveToDdsFile( ddsname , 
					imageWrapper::DDS_FORMAT_DXT1 );

				if ( saveok == false )
				{
					continue;
				}

				debugddslist->push_back( ddsname );
			}

			// �趨empty_not_insertΪfalse,û��Ҫ���
			vgImage::FileWriterDdsToVgi vgiwriter;

			vgiwriter.registerProcessObs( getProcessObs() );

			vgiwriter.writeToVgiFileFromDdsNames(
				debugddslist , _globalData._globalOutputPath + "\\" + 
				_globalData._terrainName + ".vgi" , false );

			//------------------------------------------
			// ����ɾ������
			//------------------------------------------
			_globalData.pushNeedToCleanFile( debugddslist );
			_globalData.pushNeedToCleanFile( debugimagelist );
			_globalData._genDebugImages = false;
		}

		//////////////////////////////////////////////////////////////////////////

		//------------------------------------------
		// ɾ������origin����
		//------------------------------------------
		{
			GeoBlockPtrVec::iterator iter = _originGeoBlocks->begin();
			GeoBlockPtrVec::iterator iter_end = _originGeoBlocks->end();

			for ( ; iter != iter_end ; ++ iter )
			{
				GeoBlock* curblock  = *iter;
				_globalData.pushNeedToCleanFile( curblock->getFilePath() );
				delete curblock;
			}
			_originGeoBlocks->clear();
		}

		_globalData.pushNeedToCleanFile( vgtrlist );
		_globalData.cleanTempFiles();

		return true;
	}
	//----------------------------------------------------------------
	fileBlockVectorPtr GeoDemManager::getOriginFileBlocks()
	{
		assert( _originGeoBlocks != NULL );
		assert( _originGeoBlocks->empty() == false );

		fileBlockVectorPtr ret( new fileBlockVector );

		GeoBlockPtrVec::iterator iter = _originGeoBlocks->begin();
		GeoBlockPtrVec::iterator iter_end = _originGeoBlocks->end();

		for ( ; iter != iter_end ; ++ iter )
		{
			fileBlock* block = (*iter)->getFileBlock();
			ret->push_back( *block );

			delete block;
		}

		return ret;
	}
	//----------------------------------------------------------------
	bool GeoDemManager::resizeOriginImages()
	{

		int size = _originGeoBlocks->size();
		int counter = 0;
		this->notifyProcessObserversStart( "��ʼresize..." , 0 , size );

		bool ret = true;

		GeoBlockPtrVec::iterator iter = _originGeoBlocks->begin();
		GeoBlockPtrVec::iterator iter_end = _originGeoBlocks->end();

		for ( ; iter != iter_end ; ++ iter )
		{
			GeoBlock* curblock  = *iter;

			this->notifyProcessObserversPos( curblock->getFilePath() ,  counter++ );

			bool resizeok = curblock->resize();
			assert( resizeok == true );
			if ( resizeok == false )
			{
				ret = false;
			}
		}

		this->notfiyProcessObserversFinish("resize���.");

		return ret;
	}
	//----------------------------------------------------------------
	long GeoDemManager::getLayerNum()
	{
		assert( _globalData._globalAdjustRect.getWidth() != 0 );
		assert( _globalData._globalN > 6 && _globalData._globalN < 9 );

		long adjust_width = _globalData._globalAdjustRect.getWidth() + 1;
		assert( adjust_width %2 == 0 );

		//------------------------------------------
		// ����layer������
		//------------------------------------------
		long width = adjust_width;
		long meshnum = _globalData._globalChunkSize;
		assert( width % meshnum == 0 );

		if ( width % meshnum != 0 )
		{
			return -1;
		}

		long layernum = log( (float)width / meshnum ) / log( 2.0f );
		assert( layernum > 0 );

		if ( layernum == 0 )
		{
			MessageBox( NULL , "����!����̫С!" , "Error" , MB_OK );
		}

		return layernum;
	}
	//----------------------------------------------------------------
	void GeoDemManager::generateDemLayerImpl( const long layernum, 
		const StringVectorPtr vgtrlist , const StringVectorPtr debugimagelist )
	{
		//------------------------------------------
		// ���õ�ǰ�ı���·��
		//------------------------------------------
		vgKernel::SystemUtility::setCurrentDirectory( 
			_globalData._demDirectory );

		//------------------------------------------
		// ��ʼ�п鴦��
		//------------------------------------------
		HugeImagesProcessor imgprocessor;
		HugeImageBlockInfoVectorPtr vgtrinfovec;

		imgprocessor.registerProcessObs( getProcessObs() );

		fileBlockVectorPtr fileblocks = getOriginFileBlocks();
		assert( fileblocks.isNull() == false );

		imgprocessor.initialiseFileBlocks( fileblocks );

		std::ostringstream o; 
		o << _globalData._terrainName << "_level" << layernum;
		String prefix = o.str();

		// ��ʱ��������ǳ������һ�����صĿ�.
		// ע�����һ������.
		imgprocessor.initialiseParameters( 
			_globalData._filledBlackColor,
			false, VGT_GEOMGR_TEMP_EXT,
			_globalData._demDirectory,
			_globalData._clearTempData,
			prefix , 
			_globalData._globalChunkSize , _globalData._globalChunkSize
			,VGT_GEOMGR_TEMP_EXT,VGT_GEOMGR_TEMP_EXT,
			false,true,false,true
			,false,"",false,"",
			false, //�Ƿ�д�ϵ�����Ϣ
			vgKernel::ImagePos::ZERO ,0, -1 ,
			1	// �����Ƿ��1 
			);

		//imgprocessor.makeProcessing();
		imgprocessor.makeProcessingAndSaving();

		vgtrinfovec = imgprocessor.getHugeImageBlockInfos();

		//------------------------------------------
		// ����û�����ݵ�ͼƬ
		// ���������,����vgtr
		//------------------------------------------
		HugeImageBlockInfoVector::iterator iter = vgtrinfovec->begin();
		HugeImageBlockInfoVector::iterator iter_end = vgtrinfovec->end();

		for ( ; iter != iter_end ; ++ iter )
		{
			String filename = _globalData._demDirectory + "\\" +
				iter->m_BlockFileName;

			if ( vgKernel::SystemUtility::checkFileExist( filename ) == true )
			{
				GeoBlock* block = new GeoBlock( &_globalData , filename );

				bool open = block->open();
				assert( open == true );

				//if ( block->isEmpty() == true )
				//{
				//	delete block;
				//	_globalData.pushNeedToCleanFile( filename );
				//	continue;
				//}
				//else
				//{
					String vgtrname;
					String debugimgname;
					bool saveok = block->saveToVgtr( vgtrname , debugimgname);
					assert( saveok == true );

					delete block;

					vgtrlist->push_back( vgtrname );

					if ( debugimgname.empty() == false )
					{
						debugimagelist->push_back( debugimgname );
					}

					//------------------------------------------
					// ע������,��bmpɾ����
					//------------------------------------------
					_globalData.pushNeedToCleanFile( filename );
				//}
			}
		}

		return;
	}
	//----------------------------------------------------------------
	void GeoDemManager::generateDebugLayerImpl( const long layernum, const StringVectorPtr vgtrlist )
	{
		//------------------------------------------
		// ���õ�ǰ�ı���·��
		//------------------------------------------
		vgKernel::SystemUtility::setCurrentDirectory( 
			_globalData._debugImageDirectory );

		//------------------------------------------
		// ��ʼ�п鴦��
		//------------------------------------------
		HugeImagesProcessor imgprocessor;
		HugeImageBlockInfoVectorPtr vgtrinfovec;

		fileBlockVectorPtr fileblocks = getDebugOriginFileBlocks();
		assert( fileblocks.isNull() == false );

		imgprocessor.registerProcessObs( getProcessObs() );

		imgprocessor.initialiseFileBlocks( fileblocks );

		std::ostringstream o; 
		o << _globalData._terrainName << "_level" << layernum;
		String prefix = o.str();

		// ��ʱ��������ǳ������һ�����صĿ�.
		// ע�����һ������.
		imgprocessor.initialiseParameters( 
			_globalData._filledColor,
			false, VGT_GEOMGR_TEMP_EXT,
			_globalData._debugImageDirectory,
			_globalData._clearTempData,
			prefix , 
			_globalData._globalChunkSize , _globalData._globalChunkSize
			,VGT_GEOMGR_TEMP_EXT,"dds",
			false,true,false,true
			,false,"",false,"",
			false, //�Ƿ�д�ϵ�����Ϣ
			vgKernel::ImagePos::ZERO ,0, -1 ,
			0	// �����Ƿ��1 
			);

		//imgprocessor.makeProcessing();
		imgprocessor.makeProcessingAndSaving();

		vgtrinfovec = imgprocessor.getHugeImageBlockInfos();

		//------------------------------------------
		// ����û�����ݵ�ͼƬ
		// ���������,����vgtr
		//------------------------------------------
		HugeImageBlockInfoVector::iterator iter = vgtrinfovec->begin();
		HugeImageBlockInfoVector::iterator iter_end = vgtrinfovec->end();

		for ( ; iter != iter_end ; ++ iter )
		{
			String filename = _globalData._debugImageDirectory + "\\" +
				iter->m_BlockFileName;

			if ( vgKernel::SystemUtility::checkFileExist( filename ) == true )
			{
				//GeoBlock* block = new GeoBlock( &_globalData , filename );

				//bool open = block->open();
				//assert( open == true );

				//if ( block->isEmpty() == true )
				//{
				//	delete block;
				//	vgKernel::SystemUtility::removeFile( filename );
				//	continue;
				//}
				//else
				//{
				//	String vgtrname;
				//	bool saveok = block->saveToVgtr( vgtrname );
				//	assert( saveok == true );

				//	delete block;

					vgtrlist->push_back( filename );
				//}
			}
		}

		return;
	}
	//----------------------------------------------------------------
	void GeoDemManager::generateImageLayerImpl( const long layernum,
		const StringVectorPtr vgtrlist )
	{
		//------------------------------------------
		// ���õ�ǰ�ı���·��
		//------------------------------------------
		vgKernel::SystemUtility::setCurrentDirectory( 
			_globalData._imageDirectory );

		//------------------------------------------
		// ��ʼ�п鴦��
		//------------------------------------------
		HugeImagesProcessor imgprocessor;
		HugeImageBlockInfoVectorPtr vgtrinfovec;

		imgprocessor.registerProcessObs( getProcessObs() );


		fileBlockVectorPtr fileblocks = getGeoImageFileBlocks();
		assert( fileblocks.isNull() == false );

		imgprocessor.initialiseFileBlocks( fileblocks );

		std::ostringstream o; 
		o << _globalData._terrainName << "_level" << layernum;
		String prefix = o.str();

		// ��ʱ��������ǳ������һ�����صĿ�.
		// ע�����һ������.
		imgprocessor.initialiseParameters( 
			_globalData._filledColor,
			false, VGT_GEOMGR_TEMP_EXT,
			_globalData._imageDirectory,
			_globalData._clearTempData,
			prefix , 
			_globalData._globalChunkSize * _globalData._globalCellTexSize, _globalData._globalChunkSize * _globalData._globalCellTexSize
			,VGT_GEOMGR_TEMP_EXT,"dds",
			false,true,false,true
			,false,"",false,"",
			false, //�Ƿ�д�ϵ�����Ϣ
			vgKernel::ImagePos::ZERO ,0, -1 ,
			0	// �����Ƿ��1 
			);

		//imgprocessor.makeProcessing();
		imgprocessor.makeProcessingAndSaving();

		vgtrinfovec = imgprocessor.getHugeImageBlockInfos();

		//------------------------------------------
		// ����û�����ݵ�ͼƬ
		//------------------------------------------
		HugeImageBlockInfoVector::iterator iter = vgtrinfovec->begin();
		HugeImageBlockInfoVector::iterator iter_end = vgtrinfovec->end();

		for ( ; iter != iter_end ; ++ iter )
		{
			String filename = _globalData._imageDirectory + "\\" +
				iter->m_BlockFileName;

			if ( vgKernel::SystemUtility::checkFileExist( filename ) == true )
			{
				//GeoBlock* block = new GeoBlock( &_globalData , filename );

				//bool open = block->open();
				//assert( open == true );

				//if ( block->isEmpty() == true )
				//{
				//	delete block;
				//	vgKernel::SystemUtility::removeFile( filename );
				//	continue;
				//}
				//else
				//{
				//	String vgtrname;
				//	bool saveok = block->saveToVgtr( vgtrname );
				//	assert( saveok == true );

				//	delete block;

				vgtrlist->push_back( filename );
				//}
			}
		}

		return;
	}

	//----------------------------------------------------------------
	fileBlockVectorPtr GeoDemManager::getDebugOriginFileBlocks()
	{
		assert( _debugOrginGeoBlocks != NULL );
		assert( _debugOrginGeoBlocks->empty() == false );

		fileBlockVectorPtr ret( new fileBlockVector );

		GeoBlockPtrVec::iterator iter = _debugOrginGeoBlocks->begin();
		GeoBlockPtrVec::iterator iter_end = _debugOrginGeoBlocks->end();

		for ( ; iter != iter_end ; ++ iter )
		{
			fileBlock* block = (*iter)->getFileBlock();
			ret->push_back( *block );

			delete block;
		}

		return ret;
	}
	//----------------------------------------------------------------
	fileBlockVectorPtr GeoDemManager::getGeoImageFileBlocks()
	{
		assert( _originalImageBlocks != NULL );
		assert( _originalImageBlocks->empty() == false );

		fileBlockVectorPtr ret( new fileBlockVector );

		GeoImageBlockPtrVec::iterator iter = _originalImageBlocks->begin();
		GeoImageBlockPtrVec::iterator iter_end = _originalImageBlocks->end();

		for ( ; iter != iter_end ; ++ iter )
		{
			fileBlock* block = (*iter)->getFileBlock();
			ret->push_back( *block );

			delete block;
		}

		return ret;
	}
	//----------------------------------------------------------------
	bool GeoDemManager::saveDebugLayers()
	{
		if ( _globalData._genDebugImages == false )
		{
			return true;
		}

		if ( _debugOrginGeoBlocks->empty() == true )
		{
			return true;
		}

		assert( _debugOrginGeoBlocks != NULL );
		assert( _debugOrginGeoBlocks->empty() == false );

		int layernum = getLayerNum();
		assert( layernum != 0 && layernum != -1 );
		if ( layernum == 0 || layernum == -1 )
		{
			MessageBox( NULL , "��ֵ����" , "Error" , MB_OK );
			return false;
		}

		StringVectorPtr vgilist( new StringVector );

		bool needResize = true;

		//------------------------------------------
		// ��ʼ���ɸ���layer
		//------------------------------------------
		for ( int i = 0 ; i < layernum ; ++ i )
		{
			if ( i == layernum - 1 )
			{
				needResize = false;
			}

			generateDebugLayerImpl( i , vgilist);

			if ( needResize == true )
			{
				resizeDebugOriginImages();
			}
		}

		//FileWriterVgtrToVtr vtrwriter;
		//vtrwriter.writeToVtrFileFromVgtrNames( 
		//	_globalData.getVgtrHeaderPtr(), vgilist , 
		//	_globalData._globalOutputPath + "\\" + _globalData._terrainName + ".vtr" );


		// �趨empty_not_insertΪtrue,�յ�dds�ļ�����Ҫ���� 
		vgImage::FileWriterDdsToVgi vgiwriter;

		vgiwriter.registerProcessObs( getProcessObs() );

		vgiwriter.writeToVgiFileFromDdsNames(
			vgilist , _globalData._globalOutputPath + "\\" + 
			_globalData._terrainName + ".vgi" , true );

		//------------------------------------------
		// �����һ����orgin����ɾ������
		//------------------------------------------
		GeoBlockPtrVec::iterator iter = _debugOrginGeoBlocks->begin();
		GeoBlockPtrVec::iterator iter_end = _debugOrginGeoBlocks->end();

		for ( ; iter != iter_end ; ++ iter )
		{
			GeoBlock* curblock  = *iter;

			_globalData.pushNeedToCleanFile( curblock->getFilePath() );

			delete curblock;
		}

		_debugOrginGeoBlocks->clear();

		_globalData.pushNeedToCleanFile( vgilist );
		_globalData.cleanTempFiles();


		//------------------------------------------
		// �����λ����!!!����Ҫ!!!!!!!!!!!!!!!!
		//------------------------------------------
		_globalData._genDebugImages = false;

		return true;
	}
	//----------------------------------------------------------------
	bool GeoDemManager::saveImageLayers()
	{
		if ( _originalImageBlocks == NULL )
		{
			return true;
		}

		if ( _originalImageBlocks->empty() == true )
		{
			return true;
		}

		assert( _originalImageBlocks != NULL );
		assert( _originalImageBlocks->empty() == false );

		int layernum = getLayerNum();
		assert( layernum != 0 && layernum != -1 );
		if ( layernum == 0 || layernum == -1 )
		{
			MessageBox( NULL , "��ֵ����" , "Error" , MB_OK );
			return false;
		}

		StringVectorPtr vgilist( new StringVector );

		bool needResize = true;

		//------------------------------------------
		// ��ʼ���ɸ���layer
		//------------------------------------------
		for ( int i = 0 ; i < layernum ; ++ i )
		{
			if ( i == layernum - 1 )
			{
				needResize = false;
			}

			generateImageLayerImpl( i , vgilist);

			if ( needResize == true )
			{
				resizeOriginGeoImages();
			}
		}

		//FileWriterVgtrToVtr vtrwriter;
		//vtrwriter.writeToVtrFileFromVgtrNames( 
		//	_globalData.getVgtrHeaderPtr(), vgilist , 
		//	_globalData._globalOutputPath + "\\" + _globalData._terrainName + ".vtr" );


		// �趨empty_not_insertΪtrue,�յ�dds�ļ�����Ҫ���� 
		vgImage::FileWriterDdsToVgi vgiwriter;

		vgiwriter.registerProcessObs( getProcessObs() );

		vgiwriter.writeToVgiFileFromDdsNames(
			vgilist , _globalData._globalOutputPath + "\\" + 
			_globalData._terrainName + ".vgi" , true );


		//------------------------------------------
		// �����յ�origin images�ŵ�ɾ������
		// ��������������
		//------------------------------------------
		GeoImageBlockPtrVec::iterator iter = _originalImageBlocks->begin();
		GeoImageBlockPtrVec::iterator iter_end = _originalImageBlocks->end();

		for ( ; iter != iter_end ; ++ iter )
		{
			GeoImageBlock* curblock  = *iter;
			_globalData.pushNeedToCleanFile( curblock->getFilePath() );
			delete curblock;
		}

		_originalImageBlocks->clear();

		_globalData.pushNeedToCleanFile( vgilist );
		_globalData.cleanTempFiles();



		return true;
	}

	//----------------------------------------------------------------
	bool GeoDemManager::resizeDebugOriginImages()
	{
		bool ret = true;

		GeoBlockPtrVec::iterator iter = _debugOrginGeoBlocks->begin();
		GeoBlockPtrVec::iterator iter_end = _debugOrginGeoBlocks->end();

		for ( ; iter != iter_end ; ++ iter )
		{
			GeoBlock* curblock  = *iter;

			bool resizeok = curblock->resize();
			assert( resizeok == true );
			if ( resizeok == false )
			{
				ret = false;
			}
		}

		return ret;
	}
	//----------------------------------------------------------------
	//----------------------------------------------------------------
	bool GeoDemManager::resizeOriginGeoImages()
	{
		assert( _originalImageBlocks != NULL );

#if 0
		bool ret = true;

		GeoImageBlockPtrVec::iterator iter = _originalImageBlocks->begin();
		GeoImageBlockPtrVec::iterator iter_end = _originalImageBlocks->end();

		for ( ; iter != iter_end ; ++ iter )
		{
			GeoImageBlock* curblock  = *iter;

			bool resizeok = curblock->resize();
			assert( resizeok == true );
			if ( resizeok == false )
			{
				ret = false;
			}
		}

		return ret;
#endif

		HugeProcess::HugeJobs boss( this );

		GeoImageBlockPtrVec::iterator iter = _originalImageBlocks->begin();
		GeoImageBlockPtrVec::iterator iter_end = _originalImageBlocks->end();

		for ( ; iter != iter_end ; ++ iter )
		{
			GeoImageBlock* curblock  = *iter;

			HugeProcess::PieceJob* job = new GeoImageResizeJob( curblock , &boss );

			boss.submit( job );
		}

		boss.doAndWait();

		return true;
	}
	//----------------------------------------------------------------
	bool GeoDemManager::addGeoImages( const vgKernel::StringVectorPtr pimgs )
	{
		assert( pimgs.isNull() == false );
		assert( pimgs->empty() == false );
		assert( _images.empty() == true );

		int size = pimgs->size();
		int counter = 0;
		this->notifyProcessObserversStart( "��ʼ���image����..." , 0 , size );

		vgKernel::StringVector::iterator iter = pimgs->begin();
		vgKernel::StringVector::iterator iter_end = pimgs->end();

		for ( ; iter != iter_end ; ++iter )
		{
			String curname = *iter ;

			this->notifyProcessObserversPos( curname ,  counter++ );
			//------------------------------------------
			// ����image
			//------------------------------------------
			GeoImage* dem = new GeoImage( curname , &_globalData );

			bool openok = dem->doOpen();

			if ( openok == false )
			{
				std::ostringstream o;
				o << "���ļ�:" << curname << "����,�����������ļ�.";
				MessageBox( NULL , 
					o.str().c_str() , "����dem����" , 
					MB_ICONINFORMATION | MB_OK );

				VGK_TRACE( o.str( ) );

				delete dem;
				continue;
			}

			bool addres = addGeoImageAndCheckValid( dem );

			if ( addres == false )
			{
				std::ostringstream o;
				o << "�����ļ�:" << curname << "����,���ļ��ѱ�����.�����������ļ�,����ִ��.";
				//MessageBox( NULL , 
				//	o.str().c_str() , "����dem����" , 
				//	MB_ICONINFORMATION | MB_OK );

				VGK_TRACE( o.str( ) );

				delete dem;
			}

		}// end for

		//if ( checkDemsValid() == false )
		//{
		//	clearData();
		//	return false;
		//}

		//this->notifyProcessObserversPos( "update dems..." ,  counter++ );

		//updateGeoDemsFirst();

//#if 1
//		logGeoDemsDetails();
//#endif

		this->notfiyProcessObserversFinish("���image���.");


		return true;
	}
	//----------------------------------------------------------------
	bool GeoDemManager::saveOrthoToImages()
	{
		if ( _images.empty() == true )
		{
			return true;
		}

		assert( _globalData._globalChunkSize != 0 );

		//------------------------------------------
		// ���Ȳ����ʺ�1024�������,doubleRect
		//------------------------------------------
		//long aimBlockSize = _globalData._globalCellTexSize * VGT_ORIGIN_IMG_SIZE;
		double originx = _globalData._globalUpperLeftX;
		double originy = _globalData._globalUpperLeftY;

		std::vector<GeoDoubleRect> doubleRects;

		long adjustWidth = _globalData._globalAdjustRect.getWidth() + 1;

		if ( adjustWidth % VGT_ORIGIN_IMG_SIZE != 0 )
		{
			MessageBox( NULL , "�����������̫С",  "Error" , MB_OK);
			return false;
		}

		assert( adjustWidth % VGT_ORIGIN_IMG_SIZE == 0 );

		long chunkx = adjustWidth / VGT_ORIGIN_IMG_SIZE;
		long chunky = adjustWidth / VGT_ORIGIN_IMG_SIZE;

		for ( int i = 0 ; i < chunkx ; ++ i )
		{
			for ( int j = 0 ; j < chunky ; ++ j )
			{
				
				
				double topLeftX = originx + i * VGT_ORIGIN_IMG_SIZE * _globalData._globalPixelSizeX
					- _globalData._globalPixelSizeX * 2;
				double topLeftY = originy + j * VGT_ORIGIN_IMG_SIZE * _globalData._globalPixelSizeY
					- _globalData._globalPixelSizeY * 2;

				double lowRightX = originx + i * VGT_ORIGIN_IMG_SIZE * _globalData._globalPixelSizeX
					+ VGT_ORIGIN_IMG_SIZE * _globalData._globalPixelSizeX
					+ _globalData._globalPixelSizeX * 2;
				double lowRightY = originy + j * VGT_ORIGIN_IMG_SIZE * _globalData._globalPixelSizeY
					+ VGT_ORIGIN_IMG_SIZE * _globalData._globalPixelSizeY
					+ _globalData._globalPixelSizeY * 2;

				long pixelWidth = ( VGT_ORIGIN_IMG_SIZE + 4 ) * _globalData._globalCellTexSize;
				long pixelHeight = ( VGT_ORIGIN_IMG_SIZE + 4 ) * _globalData._globalCellTexSize;

				GeoDoubleRect rect( topLeftX, topLeftY, lowRightX , lowRightY, 
					pixelWidth, pixelHeight , &_globalData );

				doubleRects.push_back( rect );
			}
		}
		
#if 0
		{
			int size = doubleRects.size();

			for ( int i = 0 ; i < size ; ++ i )
			{
				for ( int j = 0 ; j < size ; ++ j )
				{
					if ( i == j )
					{
						continue;
					}

					assert( doubleRects[j].testIntersect( doubleRects[i] )
						 == false );
				}
				//doubleRects[i]
			}
		}

#endif

		int size = _images.size();
		int counter = 0;
		this->notifyProcessObserversStart( "��ʼ�п�Ӱ������..." , 0 , size );

		GeoImageFileBlockVec tmpimages;
		vgThreads::CriticalSection tmpimg_sect;

#if 0
		GeoImageMap::iterator iter = _images.begin();
		GeoImageMap::iterator iter_end = _images.end();

		for ( ; iter != iter_end ; ++iter )
		{
			GeoImage* img = iter->second;

			this->notifyProcessObserversPos( img->getFilePath() ,  counter++ );

			GeoImageFileBlockVecPtr blocks = img->cutToFileBlocks( &doubleRects );

			if ( blocks.isNull() == false )
			{
				GeoImageFileBlockVec::iterator iter = blocks->begin();
				GeoImageFileBlockVec::iterator iter_end = blocks->end();

				for ( ; iter != iter_end ; ++iter )
				{
					tmpimages.push_back( *iter );
				}
			}
		}
#endif

		GeoImageMap::iterator iter = _images.begin();
		GeoImageMap::iterator iter_end = _images.end();

		using namespace HugeProcess;

		HugeJobs boss( this );

		for ( ; iter != iter_end ; ++iter )
		{
			GeoImage* img = iter->second;

			PieceJob* job = new GeoImageCutFileJob( &boss , 
				img , &doubleRects , &tmpimages , &tmpimg_sect , this );

			boss.submit( job );
		}

		boss.doAndWait();

		this->notfiyProcessObserversFinish("�п�Ӱ���������.");

		//////////////////////////////////////////////////////////////////////////
		// ����,��ʼ���ɵ�origin��ͼ��.

		//------------------------------------------
		// ��ʼ������ԭʼ������.
		// ���������Ҫ��2��n�η�Ϊ��С
		//------------------------------------------
		bool setdirok = vgKernel::SystemUtility::setCurrentDirectory( 
			_globalData._imageDirectory );
		assert( setdirok == true );

		vgTerritory::HugeImagesProcessor imgprocessor;
		vgTerritory::HugeImageBlockInfoVectorPtr vgtrinfovec;

		imgprocessor.registerProcessObs( getProcessObs() );

		imgprocessor.initialiseFileBlocks( tmpimages.getFileBlockVecPtr() );

		imgprocessor.initialiseParameters( 
			_globalData._filledColor,
			_globalData._clearTempData, VGT_GEOMGR_TEMP_EXT,
			_globalData._imageDirectory,
			_globalData._clearTempData,
			_globalData._terrainName + "_img_origin0" , 
			VGT_ORIGIN_IMG_SIZE  , VGT_ORIGIN_IMG_SIZE ,
			VGT_GEOMGR_TEMP_EXT,VGT_GEOMGR_TEMP_EXT,false,true,false,true
			,false,"",false,"",
			false, //�Ƿ�д�ϵ�����Ϣ
			vgKernel::ImagePos::ZERO ,0, -1);




		//imgprocessor.makeProcessing();
		imgprocessor.makeProcessingAndSaving();

		vgtrinfovec = imgprocessor.getHugeImageBlockInfos();


		assert( _originalImageBlocks == NULL );
		_originalImageBlocks = new GeoImageBlockPtrVec;
		assert( _originalImageBlocks->empty()  ==  true );

		HugeImageBlockInfoVector::iterator biter = vgtrinfovec->begin();
		HugeImageBlockInfoVector::iterator biter_end = vgtrinfovec->end();

		for ( ; biter != biter_end ; ++biter )
		{
			HugeImageBlockInfo info = *biter;

			String img = _globalData._imageDirectory + "\\" + info.m_BlockFileName;

			if( vgKernel::SystemUtility::checkFileExist( img ) == true )
			{
				GeoImageBlock* block = new GeoImageBlock( 
					getGlobalDataPtr() , img
				 );

				bool openok = block->open();
				if ( openok == false )
				{
					delete block;
					continue;
				}

				//////////////////////////////////////////////////////////////////////////
				//String ddd;
				//bool saveok = block->saveToVgtr( ddd );
				//assert( saveok == true );
				//////////////////////////////////////////////////////////////////////////
				_originalImageBlocks->push_back( block );
			}
			else
			{
				//assert(0);
			}
		}

		//------------------------------------------
		// ���GeoImageMap���� 
		//------------------------------------------
		{
			GeoImageMap::iterator iter =  _images.begin();
			GeoImageMap::iterator iter_end = _images.end();

			for ( ; iter != iter_end ; ++ iter )
			{
				delete iter->second;
			}
			_images.clear();
		}

		//------------------------------------------
		// ɾ������Ҫ���ļ�
		//------------------------------------------
		_globalData.pushNeedToCleanFile( tmpimages.getFileNameList() );
		_globalData.cleanTempFiles();


		return true;
	}
	//----------------------------------------------------------------
	void GeoDemManager::setGlobalData( const String& terrainname , 
		const String& temp_path , const String& output_path ,
		const long& n , const float& cellsize , const long& cellTexSize  ,
		const bool& clearTempData  , 
		const bool& genDebugImages ,
		unsigned char* filled_color)
	{
		//------------------------------------------
		// ���ȿ�ʼ��GlobalData���и�ֵ.
		//------------------------------------------
		assert( temp_path.empty() == false );
		_globalData._globalTempPath = temp_path;
		_globalData._terrainName = terrainname;
		_globalData._globalOutputPath = output_path;
		_globalData._genDebugImages = genDebugImages;

		for ( int i = 0 ; i < 4 ; ++ i )
		{
			_globalData._filledColor[i] = filled_color[i];
		}
		

		assert( n > 6 );
		assert( n <= 8 );

		_globalData._globalN = n;

		_globalData._globalChunkSize = 
			pow( 2.0f , n );
		_globalData._globalAdjustCellsize = cellsize;

		_globalData._globalCellTexSize = cellTexSize;
		assert( cellTexSize != 0 );

		_globalData._clearTempData = clearTempData;

		_globalData._demDirectory = 
			_globalData._globalTempPath + "\\___dem_" + 
			UniqueIDFactory::getUniqueID().getString();

		_globalData._debugImageDirectory = 
			_globalData._globalTempPath + "\\___debugImg_" + 
			UniqueIDFactory::getUniqueID().getString();

		_globalData._imageDirectory = 
			_globalData._globalTempPath + "\\___img_" + 
			UniqueIDFactory::getUniqueID().getString();


		bool createdirok = false;
		createdirok = SystemUtility::createDirectory( _globalData._demDirectory );
		assert( createdirok = true );
		createdirok = SystemUtility::createDirectory( _globalData._debugImageDirectory );
		assert( createdirok = true );
		createdirok = SystemUtility::createDirectory( _globalData._imageDirectory );
		assert( createdirok = true );

		_globalData._maxLayerNum = getLayerNum();

		return;
	}
	//----------------------------------------------------------------
	StringVector GeoDemManager::getOverviewGeoImageFileNames()
	{
		StringVector ret;

		if ( _globalData._genImageOverview == false )
		{
			return ret;
		}
		
		GeoImageMap::iterator iter = _images.begin();
		GeoImageMap::iterator iter_end = _images.end();

		for ( ; iter != iter_end ; ++ iter )
		{
			ret.push_back( iter->second->getFilePath() );
		}

		return ret;
	}
	//----------------------------------------------------------------
	bool GeoDemManager::clearTempData()
	{


		getGlobalDataPtr()->cleanTempFiles();

		Sleep(50);

		//------------------------------------------
		// ɾ��Ŀ¼
		//------------------------------------------
		SystemUtility::removeEmptyDirectory( _globalData._demDirectory );
		SystemUtility::removeEmptyDirectory( _globalData._debugImageDirectory );
		SystemUtility::removeEmptyDirectory( _globalData._imageDirectory );

		return true;
	}
	//---------------------------------------
	void GeoDemManager::clearOverviews()
	{
		GeoDemMap::iterator iter = _dems.begin();
		GeoDemMap::iterator iter_end = _dems.end();

		for ( ; iter != iter_end ; ++ iter  )
		{
			GeoDem* curdem = iter->second;

			curdem->clearOverview();
		}

		return;
	}
	//----------------------------------------------------------------




}// end of namespace vgTerritory
