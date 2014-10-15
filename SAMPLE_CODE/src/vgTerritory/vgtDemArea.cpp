

#include <vgStableHeaders.h>
#include <vgTerritory/vgtDemArea.h>
#include <vgKernel/vgkSystemUtility.h>


namespace vgTerritory {
	
	
	
	
	//----------------------------------------------------------------
	DemArea::DemArea( String dempath , String imgpath )
	{
		_demPath = dempath;
		_imagePath = imgpath;
		_demHeader = new DemHeader;

		_img.bind( new CximageWrapper );
	}
	//----------------------------------------------------------------
	DemArea::~DemArea()
	{
		if ( _demHeader != NULL )
		{
			delete _demHeader;
			_demHeader = NULL;
		}
	}
	//----------------------------------------------------------------
	bool DemArea::readFileAndCheckValid()
	{
		if ( _demHeader->readFromFile( _demPath ) == false )
		{
			std::ostringstream o;
			o << "��ȡ�ļ�" << _demPath << "ʱʧ��.";

			MessageBox( NULL , 
				o.str().c_str() , "��ȡ����" , MB_OK );

			return false;
		}

		if ( _img->loadHeaderFromFile( _imagePath ) == false)
		{
			std::ostringstream o;
			o << "��ȡ�ļ�" << _imagePath << "ʱʧ��.";

			MessageBox( NULL , 
				o.str().c_str() , "��ȡ����" , MB_OK );

			return false;
		}

		//------------------------------------------
		// �������Ƿ�һ��
		//------------------------------------------
		long tmp = _img->getWidth() * (_demHeader->_xNum - 1);

		if ( (_img->getHeight() * (_demHeader->_zNum-1) ) != tmp )
		{
			std::ostringstream o;
			o << "�ļ�" << _imagePath <<"��" << _demPath << "��С��һ��.";

			MessageBox( NULL , 
				o.str().c_str() , "��ȡ����" , MB_OK );

			return false;
		}

		if (  _img->getWidth() % (_demHeader->_xNum - 1) != 0 )
		{
			std::ostringstream o;
			o << "�ļ�" << _imagePath <<"��" << _demPath << "��С��һ��.";

			MessageBox( NULL , 
				o.str().c_str() , "��ȡ����" , MB_OK );

			return false;
		}
		
		return true;
	}
	//----------------------------------------------------------------
	void DemArea::setAbsoluteRect( const double& originx ,const double& originz )
	{
		_absDemRect.minPos.x = 
			( _demHeader->_originX - originx ) / _demHeader->_cellSize;

		_absDemRect.minPos.y = 
			( _demHeader->_originZ - originz ) / _demHeader->_cellSize;

		_absDemRect.maxPos.x = 
			_absDemRect.minPos.x + _demHeader->_xNum - 1 ;

		_absDemRect.maxPos.y = 
			_absDemRect.minPos.y + _demHeader->_zNum - 1 ;

		long scale = getScaleOfDemToImage();

		_absImgRect.minPos = _absDemRect.minPos * scale;
		_absImgRect.maxPos = _absDemRect.maxPos * scale;

		assert( _absImgRect.getImageSize().getWidth() == _img->getWidth() );

	}
	//----------------------------------------------------------------
	long DemArea::getScaleOfDemToImage()
	{
		int len = _demHeader->_xNum - 1;
		int imglen = _img->getWidth();

		assert( len <= imglen );

		assert(  imglen % len == 0 );

		assert(  imglen / len != 0 );

		return imglen / len;
	}
	//----------------------------------------------------------------
	bool DemArea::resizeImg( const long& scale_dem_to_img , 
		const String& temp_folder )
	{
		
		assert( scale_dem_to_img != getScaleOfDemToImage() );

		//------------------------------------------
		// ��ʼ�������ļ�
		//------------------------------------------
		CximageWrapperPtr cximg( new CximageWrapper );
		
		bool loadok = cximg->loadFromFile( _imagePath );

		assert( loadok == true );

		if ( loadok == false )
		{
			return false;
		}

		//------------------------------------------
		// ����resize�Ĵ�С
		//------------------------------------------
		assert( scale_dem_to_img == 1 ||
			scale_dem_to_img == 2 ||
			scale_dem_to_img == 8 );

		long new_width = _absDemRect.getWidth() * scale_dem_to_img;
		long new_height = _absDemRect.getHeight() * scale_dem_to_img;

		assert( new_height > 0 );

		String metaname = _imagePath.substr(
			_imagePath.find_last_of("\\/") + 1, String::npos );

		String new_path = temp_folder + metaname + ".temp.bmp";


		bool resizeres = 
			cximg->resize( vgKernel::ImageSize( new_width , new_height ) );

		if ( resizeres == false )
		{
			MessageBox( NULL , 
				"resizeͼ��ʱ����,�ڴ治������Դ�ļ���" , "����" , MB_OK );

			return false;
		}


		bool saveok = cximg->saveToFile( new_path ); 
		assert( saveok == true );

		if ( saveok == false )
		{
			return false;
		}

		cximg.setNull();

		//------------------------------------------
		// �����������Ϣ
		//------------------------------------------
		_imagePath = new_path;
		_img.setNull();
		_img.bind( new CximageWrapper );
		delete _demHeader;
		_demHeader = new DemHeader;

		readFileAndCheckValid();


		assert( _absDemRect.getWidth() == _demHeader->_xNum - 1);

		long scale = getScaleOfDemToImage();

		_absImgRect.minPos = _absDemRect.minPos * scale;
		_absImgRect.maxPos = _absDemRect.maxPos * scale;

		assert( _absImgRect.getImageSize().getWidth() == _img->getWidth() );


		_tempFiles.push_back( new_path );

		return true;
	}
	//----------------------------------------------------------------
	void DemArea::clearTempFile()
	{
		StringVector::iterator iter =  _tempFiles.begin();
		StringVector::iterator iter_end = _tempFiles.end();

		for ( ; iter != iter_end ; ++iter )
		{
			SystemUtility::removeFile( *iter , false);	
		}
	}
	//----------------------------------------------------------------
	bool DemArea::resizeImageForLod( const String& absolute_temp_folder )
	{
		//------------------------------------------
		// ��ʼ�������ļ�
		//------------------------------------------
		CximageWrapperPtr cximg( new CximageWrapper );

		bool loadok = cximg->loadFromFile( _imagePath );



		assert( loadok == true );

		if ( loadok == false )
		{
			return false;
		}

		//------------------------------------------
		// ����resize�Ĵ�С
		//------------------------------------------

		long new_width = cximg->getWidth() / 2;
		long new_height = cximg->getHeight() / 2;

		assert( new_width != 0 );
		assert( new_height != 0 );
		assert( new_height > 0 );

		String metaname = _imagePath.substr(
			_imagePath.find_last_of("\\/") + 1, String::npos );

		String new_path = absolute_temp_folder + "\\" + metaname + ".temp.bmp";


		bool resizeres = 
			cximg->resize( vgKernel::ImageSize( new_width , new_height ) );

		if ( resizeres == false )
		{
			MessageBox( NULL , 
				"resizeͼ��ʱ����,�ڴ治������Դ�ļ���" , "����" , MB_OK );

			return false;
		}


		bool saveok = cximg->saveToFile( new_path ); 
		assert( saveok == true );

		if ( saveok == false )
		{
			return false;
		}

		cximg.setNull();

		//------------------------------------------
		// �����������Ϣ
		//------------------------------------------
		_imagePath = new_path;
		_img.setNull();
		_img.bind( new CximageWrapper );

		loadok = _img->loadHeaderFromFile( _imagePath );
		assert( loadok == true );

		assert( _img->getWidth() == _img->getHeight() );

		_absImgRect /= 2;


		assert( _absImgRect.getImageSize().getWidth() == _img->getWidth() );



		//------------------------------------------
		// ���뵽tempĿ¼
		//------------------------------------------
		_tempFiles.push_back( new_path );

		return true;
	}
	//----------------------------------------------------------------
}// end of namespace vgTerritory
