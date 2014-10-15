

#include <vgStableHeaders.h>
#include <vgTerritory/vgtDemAreaBag.h>
#include <vgTerritory/vgDemImporter.h>
#include <vgTerritory/vgVgtrWriter.h>
#include <vgImage/vgiImageUtility.h>
#include <vgTerritory/vgtVgtrGenerator.h>
#include <vgTerritory/vgtFileWriterVgtrToVtr.h>
#include <vgKernel/vgkSystemUtility.h>

namespace vgTerritory {
	
	
	//----------------------------------------------------------------
	DemAreaBag::DemAreaBag()
	{
		_absOriginX = 0.0f;
		_absOriginX = 0.0f;
		_dems = new DemAreaPtrVec;
		_scaleOfDemToImg = 0;
	}
	//----------------------------------------------------------------
	DemAreaBag::~DemAreaBag()
	{
		if ( _dems != NULL )
		{
			delete _dems;
			_dems = NULL;
		}
	}
	//----------------------------------------------------------------
	bool DemAreaBag::readFromFilePairs( const StringPairVector& inputlist )
	{
		assert( _dems->empty() == true );
		assert( inputlist.empty() == false);

		StringPairVector::const_iterator iter  = inputlist.begin();
		StringPairVector::const_iterator iter_end = inputlist.end();

		for ( ; iter != iter_end ; ++iter )
		{
			const StringPair curpair = *iter;

			DemAreaPtr tmp( new DemArea( curpair.first , curpair.second ) );

			if ( tmp->readFileAndCheckValid() == true )
			{
				_dems->push_back( tmp );
			}
		}

		if ( _dems->empty() == true )
		{
			return false;
		}

		//------------------------------------------
		// ���cellsize
		//------------------------------------------
		{
			// �Ѿ���֤��Ϊ����
			_cellsize = (*_dems->begin())->_demHeader->_cellSize;

			DemAreaPtrVec::iterator iter = _dems->begin();
			DemAreaPtrVec::iterator iter_end = _dems->end();

			for ( ;iter != iter_end ; ++iter )
			{
				if ( (*iter)->_demHeader->_cellSize != _cellsize )
				{
					MessageBox( NULL , 
						"�ļ���cellsize��һ��", "����" , MB_OK );

					return false;
				}
			}
		}

		//------------------------------------------
		// ��������ͼ��ı���
		//------------------------------------------
		{
			_scaleOfDemToImg = (*_dems->begin() )->getScaleOfDemToImage();

			DemAreaPtrVec::iterator iter = _dems->begin();
			DemAreaPtrVec::iterator iter_end = _dems->end();

			for ( ;iter != iter_end ; ++iter )
			{
				if ( (*iter)->getScaleOfDemToImage() != _scaleOfDemToImg )
				{
					MessageBox( NULL , 
						"�������ݺ��ļ��Ĵ�С������һ��.", "����" , MB_OK );

					return false;
				}
			}
		}

		//------------------------------------------
		// ��ʼ����
		// �õ���ԭʼ������
		//------------------------------------------
		//------------------------------------------
		// �ҳ���Сԭ��
		//------------------------------------------
		{
			_absOriginX = (*_dems->begin())->_demHeader->_originX;
			_absOriginZ = (*_dems->begin())->_demHeader->_originZ;

			DemAreaPtrVec::iterator iter = _dems->begin();
			DemAreaPtrVec::iterator iter_end = _dems->end();

			for ( ; iter != iter_end ; ++iter )
			{
				if ( (*iter)->_demHeader->_originX < _absOriginX  )
				{
					_absOriginX = (*iter)->_demHeader->_originX;
				}

				if ( (*iter)->_demHeader->_originZ < _absOriginZ )
				{
					_absOriginZ = (*iter)->_demHeader->_originZ;
				}
			}
		}

		//------------------------------------------
		// ����ÿ��dem��absolute Rect
		//------------------------------------------
		{
			DemAreaPtrVec::iterator iter = _dems->begin();
			DemAreaPtrVec::iterator iter_end = _dems->end();

			for ( ; iter != iter_end ; ++iter )
			{
				(*iter)->setAbsoluteRect( _absOriginX , _absOriginZ );
			}
		}


		return true;
	}
	//----------------------------------------------------------------
	fileBlockVectorPtr DemAreaBag::getFileBlockVec()
	{
		assert( _dems->empty() == false );

		fileBlockVectorPtr ret( new fileBlockVector );

		DemAreaPtrVec::iterator iter = _dems->begin();
		DemAreaPtrVec::iterator iter_end = _dems->end();

		for ( ; iter != iter_end ; ++iter )
		{
			fileBlock tmpblock;

			tmpblock.fileSrcName = (*iter)->_imagePath;
			tmpblock.fileAbsoluteRect = (*iter)->_absImgRect;

			ret->push_back( tmpblock );
		}

		return ret;
	}
	//----------------------------------------------------------------
	bool DemAreaBag::isNeedToResizeImg()
	{
		assert( _scaleOfDemToImg != 0 );
		if ( _scaleOfDemToImg == 1 || 
			_scaleOfDemToImg == 2 || 
			_scaleOfDemToImg == 8 )
		{
			return false;
		}

		return true;
	}
	//----------------------------------------------------------------
	bool DemAreaBag::resizeImages(const long& new_scale , 
		const String& absolute_temp_folder)
	{
		assert( new_scale != getScaleOfDemToImg() );

		if ( _dems->empty() == true )
		{
			return false;
		}

		this->notifyProcessObserversStart( "��ʼ����ͼ���ļ�" , 0 ,
			_dems->size() - 1 );

		DemAreaPtrVec::iterator iter = _dems->begin();
		DemAreaPtrVec::iterator iter_end = _dems->end();

		int counter = 0;
		for ( ; iter != iter_end ; ++iter )
		{
			this->notifyProcessObserversPos( (*iter)->_imagePath , counter ++ );

			bool resize = (*iter)->resizeImg( new_scale , absolute_temp_folder );
			if ( resize == false )
			{
				return false;
			}
		}

		std::ostringstream o ;
		o << "�������,�ɹ�����:" << _dems->size() << "���ļ�.";
		this->notfiyProcessObserversFinish( o.str() );

		return true;
	}
	//----------------------------------------------------------------
	bool DemAreaBag::saveToVtr( const vgKernel::StringVectorPtr ddsfilenames ,
		const String& terrain_name ,
		const String& absolute_output_filepath , const long& N )
	{
		assert( _dems->empty() == false );
		assert( ddsfilenames.isNull()  ==false );


		this->notifyProcessObserversStart( "��ʼ����dem�ļ�" , 0 ,
			_dems->size() );

		vgTerritory::DemBlockVectorPtr pblocks( new vgTerritory::DemBlockVector );

		DemAreaPtrVec::iterator iter = _dems->begin();
		DemAreaPtrVec::iterator iter_end = _dems->end();

		int counter = 0;

		for ( ; iter != iter_end ; ++iter )
		{
			vgTerritory::DemBlockPtr block = 
				vgTerritory::DemImporter::readDemFile( (*iter)->_demPath );

			this->notifyProcessObserversPos( (*iter)->_demPath , counter ++ );

			assert( block.isNull() == false );

			pblocks->push_back( block );
		}

		this->notifyProcessObserversPos( absolute_output_filepath , counter ++ );

		vgTerritory::VgtrFilePtr resultfile = 
			vgTerritory::VgtrWriter::convertDemBlocksToVgtr( terrain_name , 
			pblocks , _cellsize , N );

		assert( resultfile.isNull() == false );

		if ( resultfile.isNull() == true )
		{
			return false;
		}

		//------------------------------------------
		// ���dem�б�
		//------------------------------------------
		pblocks.setNull();

		//------------------------------------------
		// д��һ���ܵ�vgtr�ļ�
		//------------------------------------------
		//String wholefilename = absolute_output_filepath + "\\" + terrain_name + ".vgtr";
		//vgTerritory::VgtrWriter::writeVgtrToFile( resultfile , wholefilename );

		//bool saveok = vgTerritory::VgtrWriter::writeVgtrToFile( resultfile , absolute_output_filepath );
		vgKernel::StringVectorPtr saveret = 
			vgTerritory::VgtrGenerator::generateLayers( resultfile , absolute_output_filepath);

		assert( saveret.isNull() == false );
		if ( saveret.isNull() == true  )
		{
			return false;
		}

		//------------------------------------------
		// ������,���˵�û�����ݵ�����
		//------------------------------------------
		vgKernel::StringVectorPtr  vgtrlist( new vgKernel::StringVector );
		{
			vgKernel::StringVector::iterator iter = saveret->begin();
			vgKernel::StringVector::iterator iter_end = saveret->end();

			for ( ; iter != iter_end ; ++iter )
			{
				String vgtrprefix = (*iter).substr(
					(*iter).find_last_of("\\/") + 1 , String::npos );
				vgtrprefix = vgtrprefix.substr( 
					0 , vgtrprefix.find_last_of(".") );

				//���dds�������Ƿ��������Ƶ�����
				vgKernel::StringVector::iterator ddsiter = ddsfilenames->begin();
				vgKernel::StringVector::iterator ddsiter_end = ddsfilenames->end();

				for (  ; ddsiter != ddsiter_end ; ++ ddsiter)
				{
					String ddsprefix = (*ddsiter).substr(
						(*ddsiter).find_last_of("\\/") + 1 , String::npos );
					ddsprefix = ddsprefix.substr( 
						0 , ddsprefix.find_last_of(".") );

					if ( ddsprefix == vgtrprefix )
					{
						vgtrlist->push_back( *iter );
						break;
					}
				}
			}
		}



		//------------------------------------------
		// ����vtr�ļ�
		//------------------------------------------
		String vtrfilename = absolute_output_filepath + "\\" + terrain_name + ".vtr";

		vgTerritory::FileWriterVgtrToVtr vtrwriter;
		bool savevtr = vtrwriter.writeToVtrFileFromVgtrNames( resultfile->_headerPtr ,
			vgtrlist , vtrfilename, NULL );

		if ( savevtr == false )
		{
			MessageBox(NULL , "����vtrʱ���ִ���" , "Error" , MB_OK );
			return false;
		}

		// ���ɱ���
		std::ostringstream o;
		o << "�������,��" << saveret->size() << "���ļ�.";
		this->notfiyProcessObserversFinish( o.str() );

		_needToClearList = saveret;


		return true;
	}	
	//----------------------------------------------------------------
	void DemAreaBag::clearTempFiles()
	{
		if ( _needToClearList.isNull() == false )
		{
			StringVector::iterator iter = _needToClearList->begin();
			StringVector::iterator iter_end = _needToClearList->end();
			for ( ; iter != iter_end ; ++ iter )
			{
				vgKernel::SystemUtility::removeFile( *iter , false );
			}
		}

		DemAreaPtrVec::iterator iter = _dems->begin();
		DemAreaPtrVec::iterator iter_end = _dems->end();

		for ( ; iter != iter_end ; ++iter )
		{
			(*iter)->clearTempFile();
		}
	}
	//----------------------------------------------------------------
	bool DemAreaBag::resizeImageForLod( const String& absolute_temp_folder )
	{
		if ( _dems->empty() == true )
		{
			return false;
		}

		this->notifyProcessObserversStart( "��ʼ����ͼ���ļ�" , 0 ,
			_dems->size() - 1 );

		DemAreaPtrVec::iterator iter = _dems->begin();
		DemAreaPtrVec::iterator iter_end = _dems->end();

		int counter = 0;
		for ( ; iter != iter_end ; ++iter )
		{
			this->notifyProcessObserversPos( (*iter)->_imagePath , counter ++ );

			bool resize = (*iter)->resizeImageForLod( absolute_temp_folder );
			if ( resize == false )
			{
				return false;
			}
		}

		std::ostringstream o ;
		o << "�������,�ɹ�����:" << _dems->size() << "���ļ�.";
		this->notfiyProcessObserversFinish( o.str() );

		return true;
	}
	//----------------------------------------------------------------
	long DemAreaBag::getDemWidth()
	{
		DemAreaPtrVec::iterator iter = _dems->begin();
		DemAreaPtrVec::iterator iter_end = _dems->end();

		ImageRect vgtr_rect;

		int counter = 0;
		for ( ; iter != iter_end ; ++iter )
		{
			vgtr_rect.merge( ( *iter)->_absDemRect );
		}

		int ret = vgImage::ImageUtility::getBiggerPowerOfTwoNumber( 
			vgtr_rect.getWidth() + 1 );

		return ret;
	}
	//----------------------------------------------------------------
}// end of namespace vgTerritory
