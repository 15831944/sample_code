

#include <vgStableHeaders.h>
#include <vgImage/vgiFileWriterVgi.h>
#include <vgImage/vgiImageUtility.h>
#include <vgImage/vgiFileDefinitionVgi.h>
#include <vgImage/vgiIoRequestDumpImage.h>
#include <vgImage/vgiImageManager.h>
#include <vgKernel/vgkStreamWriter.h>
#include <vgKernel/vgkStreamReader.h>
#include <vgKernel/vgkCrcUtility.h>
#include <vgKernel/vgkStringUtility.h>
#include <vgKernel/vgkSystemUtility.h>
#include <vgImage/vgiCximageWrapper.h>
#include <vgImage/vgiFileWriterDdsToVgi.h>
#include <vgImage/vgiFreeImageWrapper.h>




namespace vgImage {
	
	
	using namespace vgKernel;
	
	vgTerritory::ptIndexMap* FileWriterVgi::s_pixelIndexMap = NULL;
	String FileWriterVgi::s_tempPathDir = "";
	String FileWriterVgi::s_ddsFistName = "";
	char* FileWriterVgi::s_backgroundPicBuf = NULL;


	//----------------------------------------------------------------
	bool FileWriterVgi::writeToVgiFile( 
		const vgKernel::StringVectorPtr input_texnames , 
		const String& output_filename  , const long& nextfileid )
	{

		//------------------------------------------
		// 1.���ȴ���StringPairVector
		//------------------------------------------
		vgKernel::StringPairVectorPtr pairlist( 
			new vgKernel::StringPairVector );

		StringVector::iterator iter = input_texnames->begin();
		StringVector::iterator iter_end = input_texnames->end();

		for ( ; iter != iter_end ; ++iter )
		{
			pairlist->push_back( std::make_pair( *iter , *iter ) );
		}


		return writeToVgiFileWithRename( pairlist , output_filename , nextfileid );


#if 0

		if ( input_texnames.isNull() == true )
		{
			MessageBox( NULL , "������������Ϊ��" , "Error" , MB_OK );
			return false;
		}

		if ( input_texnames->empty() == true )
		{
			return false;
		}

		//------------------------------------------
		// 1.���ȶ���������������´���:
		//	-ɾ����dds�ļ�
		//	-ɾ��ͬ���ļ�
		//------------------------------------------
		std::set<String> nameSet;

		{
			vgKernel::StringVector::iterator iter = input_texnames->begin();
			vgKernel::StringVector::iterator iter_end = input_texnames->end();

			for ( ; iter != iter_end ; ++iter )
			{
				if ( ImageUtility::findImageFileExtension( *iter ) == "dds" &&
					iter->empty() == false )
				{
					nameSet.insert( *iter );
				}
			}
		}
		

		//------------------------------------------
		// 2.����д���ļ�Stream
		//------------------------------------------
		vgKernel::StreamWriterPtr pwriter = 
			vgKernel::StreamWriterFactory::createFromFile( output_filename , true );

		if ( pwriter.isNull() == true )
		{
			return false;
		}

		if ( pwriter->isValid() == false )
		{
			return false;
		}

		//------------------------------------------
		// 3.дͷ�ļ�
		//------------------------------------------
		VgiHeader header;
		long filebegin = pwriter->tell();

		pwriter->write( &header , sizeof( header ) );

		vgImage::ImageManager& imgmgr = 
			vgImage::ImageManager::getSingleton();

		//------------------------------------------
		// 4.��ʼ�õ��ļ�����,�����ļ����ݵõ�briefs
		//------------------------------------------
		VgiBriefPtrVec briefs;
		vgKernel::StringVector validLIst;

		{
			std::set<String>::iterator iter = nameSet.begin();
			std::set<String>::iterator iter_end = nameSet.end();

			for ( ; iter != iter_end ; ++iter )
			{
				long writebegpos = pwriter->tell();

				ImageMiniature* img = imgmgr.getImageMiniature( *iter );

				if ( img == NULL )
				{
					continue;
				}

				IoRequestDumpImage request( img );
				request.sendRequestAndDoAsynHandlerFunc( true );

				VgiBrief* brief = request.getBriefFromBuffer();
				assert( brief != NULL );

				// д��buf
				char* buf = request.getBuffer();
				long buf_len = request.getBufferSize();

				long writenum = pwriter->write( buf , buf_len );
				assert( writenum == buf_len );

				// ����brief
				brief->_metaFileOffset = writebegpos;

				briefs.push_back( VgiBriefPtr( brief ) );
				validLIst.push_back( *iter );

				delete[] buf;
			}
		}

		assert( briefs.size() == validLIst.size() );
		//------------------------------------------
		// 5.д��briefs,������header
		//------------------------------------------
		//����header
		header._positionOfBriefs = pwriter->tell();
		header._numberOfBriefs = briefs.size();
		// ����4�ֽ���У����
		header._sizeOfWholeFile = header._positionOfBriefs +
			header._numberOfBriefs * header._lengthOfOneBrief + sizeof( DWORD );

		header._nextFileID = nextfileid;

		assert( sizeof( DWORD ) == 4 );

		//------------------------------------------
		// ��дheader
		//------------------------------------------
		pwriter->seek( filebegin );
		pwriter->write( &header , sizeof( header ) );

		// ��ת��дbrief����
		pwriter->seek( header._positionOfBriefs );

		//------------------------------------------
		// д��brief
		//------------------------------------------
		{
			VgiBriefPtrVec::iterator iter = briefs.begin();
			VgiBriefPtrVec::iterator iter_end = briefs.end();

			for ( ; iter != iter_end ; ++iter )
			{
				VgiBrief* curbrief = (*iter).getPointer();
				long writenum =pwriter->write( curbrief, sizeof( VgiBrief ));
				assert( writenum == sizeof( VgiBrief ) );
			}
		}


		//------------------------------------------
		// 6.��ʼ����У����
		//------------------------------------------
		DWORD crcCode;

		CrcUtility::initCrc32Code( crcCode );

		// У��header
		for ( int i = 0 ; i < sizeof( header ) ; ++ i )
		{
			CrcUtility::calculateCrc32Code( (char*)&header + i , crcCode );
		}

		// У��������
		{
			StringVector::iterator iter = validLIst.begin();
			StringVector::iterator iter_end = validLIst.end();

			for ( ; iter != iter_end ; ++iter )
			{
				ImageMiniature* img = imgmgr.getImageMiniature( *iter );
				assert( img != NULL );

				IoRequestDumpImage request( img );
				request.sendRequestAndDoAsynHandlerFunc( true );

				char* buf = request.getBuffer();
				long buf_len = request.getBufferSize();

				for ( int  i = 0 ; i < buf_len ; ++ i )
				{
					CrcUtility::calculateCrc32Code( buf + i , crcCode );
				}

				delete[] buf;
			}
		}

		// У��brief
		{
			VgiBriefPtrVec::iterator iter = briefs.begin();
			VgiBriefPtrVec::iterator iter_end = briefs.end();

			for ( ; iter != iter_end ; ++iter )
			{
				for ( int i = 0 ; i < sizeof( VgiBrief ) ; ++ i )
				{
					CrcUtility::calculateCrc32Code( (char*)(*iter).getPointer() + i , crcCode );
				}
			}
		}

		// д��У����
		long writenum =pwriter->write( &crcCode , sizeof( DWORD ) );
		assert( writenum == sizeof( DWORD ) );

		pwriter.setNull();

		//------------------------------------------
		// ��ʼ����ļ���ȷ��
		//------------------------------------------
#if 0
		DWORD checkcode;
		bool getres = CrcUtility::getFileCrc32Code( output_filename , checkcode );
		assert( getres == true );

		if ( checkcode != 0xFFFFFFFF )
		{
			MessageBox( NULL , "�ļ�У��ʧ��,�����±���." , "����ʧ��" , 
				MB_OK | MB_ICONINFORMATION );

			return false;
		}

#endif

		//------------------------------------------
		// ������������report
		//------------------------------------------


		return true;

#endif

	}
	//----------------------------------------------------------------
	bool FileWriterVgi::writeToVgiFileFromTextureNames( 
		const vgKernel::StringVectorPtr input_texnames , 
		const String& output_filename )
	{
		//------------------------------------------
		// 1.���ȴ���StringPairVector
		//------------------------------------------
		vgKernel::StringPairVectorPtr pairlist( 
			new vgKernel::StringPairVector );

		StringVector::iterator iter = input_texnames->begin();
		StringVector::iterator iter_end = input_texnames->end();

		for ( ; iter != iter_end ; ++iter )
		{
			pairlist->push_back( std::make_pair( *iter , *iter ) );
		}


		return writeToVgiFromTexNamesWithRename( pairlist , output_filename );

#if 0
		//////////////////////////////////////////////////////////////////////////
		using namespace vgKernel;

		ImageManager& imgmgr = ImageManager::getSingleton();

		// ������ļ�����
		StringVectorPtr err_files( new StringVector );

		//------------------------------------------
		// ����Ҫ�ļ��
		//------------------------------------------
		if ( StringUtility::getFileExtension( output_filename ) != "vgi" )
		{
			return false;
		}
		if ( input_texnames.isNull() == true )
		{
			return false;
		}
		if ( input_texnames->empty() == true )
		{
			return false;
		}

		//------------------------------------------
		// ���ȶ���������������´���:
		//	-ɾ����dds�ļ�
		//	-ɾ��ͬ���ļ�
		//------------------------------------------
		StringVectorPtr input_dds( new StringVector );
		{
			std::set<String>* nameSet = new std::set<String>;

			vgKernel::StringVector::iterator iter = input_texnames->begin();
			vgKernel::StringVector::iterator iter_end = input_texnames->end();

			for ( ; iter != iter_end ; ++iter )
			{
				if ( ImageUtility::findImageFileExtension( *iter ) == "dds" &&
					iter->empty() == false )
				{
					nameSet->insert( *iter );
				}
			}

			input_dds->assign( nameSet->begin() , nameSet->end() );
			delete nameSet;
		}

		//------------------------------------------
		// ��ȡÿ�������ļ�,�жϴ��ڵ�ͬʱ,����
		// �ļ��Ĵ�С,Ȼ������ж�����и�vgi�ļ�.
		//------------------------------------------
		uint64 wholesize = 0;		// �ܹ����ļ���С.
		unsigned int one_vgi_size = 0;
		int cur_list_num = 0;

		typedef std::vector<StringVectorPtr> StringVecPtrVec;
		StringVecPtrVec split_list;

		int dds_size = input_dds->size();
		StringVector& ddsvec = *input_dds.getPointer();

		for ( int i = 0 ; i < dds_size ; ++ i )
		{
			ImageMiniature* img = imgmgr.getImageMiniature( ddsvec[i] );

			if ( img == NULL )
			{
				err_files->push_back( ddsvec[i] );
				continue;
			}

			int filesize = img->getMetaFileSize();

			if ( filesize == 0 )
			{
				err_files->push_back( ddsvec[i] );
				continue;
			}

			wholesize += filesize;
			one_vgi_size += filesize;

			if ( one_vgi_size >= VGI_MAX_SINGLE_FILE_SIZE )
			{
				one_vgi_size = filesize;	// ע�����ﲻ��0
				cur_list_num ++;
			}

			if ( cur_list_num == split_list.size() )
			{
				split_list.push_back( StringVectorPtr( new StringVector() ) );
			}

			assert( cur_list_num < split_list.size() );

			// ������
			split_list[ cur_list_num ]->push_back( ddsvec[i] );
		}

		//------------------------------------------
		// ��ʼ����
		//------------------------------------------
		String metaname =
			StringUtility::getFilenameWithoutExtFromAbsPath( output_filename );
		String path = StringUtility::getPathFromAbsoluteFilename( output_filename );

		for ( int i = 0 ; i < split_list.size() ; ++ i )
		{
			String curoutname;

			if ( i == 0 )
			{
				curoutname = output_filename;
			}
			else
			{
				std::ostringstream o;
				o << path << metaname << ".part" << i << ".vgi";
				curoutname = o.str();
			}

			bool writeok = writeToVgiFile( split_list[i] , 
				curoutname , 
				( i == split_list.size() - 1 ) ? 0 : i + 1 );

			if ( writeok == false )
			{
				return false;
			}
		}

		return true;


#if 0
		//------------------------------------------
		// ���ļ���Ŀ��ȡ�İ汾.
		//------------------------------------------
		using namespace vgKernel;

		//------------------------------------------
		// ����Ҫ�ļ��
		//------------------------------------------
		if ( StringUtility::getFileExtension( output_filename ) != "vgi" )
		{
			return false;
		}

		if ( input_texnames->empty() == true )
		{
			return false;
		}

		// �洢��Ҫ������б��ļ�.
		// VtrPair�е�int��ʾ����һ��nextfileid.
		typedef std::pair<int,StringVectorPtr> VtrPair;
		typedef std::vector< VtrPair > VtrPairVec;
		//------------------------------------------
		// ���ȼ������Ķ���,���б�Ҫ,�����п鴦��
		//------------------------------------------
		VtrPairVec fileVec;

		StringVector& in_names = *input_texnames.getPointer();

		int vtrnum = ( in_names.size() + VGI_MAX_FILENUM - 1 ) / VGI_MAX_FILENUM;


		for ( int i = 0 ; i < vtrnum ; ++ i )
		{
			StringVectorPtr strvec( new StringVector );
			for ( int j = 0 ; j < VGI_MAX_FILENUM ; ++ j )
			{
				if ( (VGI_MAX_FILENUM * i + j) < in_names.size() )
				{
					strvec->push_back( in_names[ VGI_MAX_FILENUM * i + j ] );
				}
			}

			int nextfileid = 0;

			if ( i == vtrnum - 1 )
			{
				nextfileid = 0;
			}
			else
			{
				nextfileid = i + 1;
			}

			fileVec.push_back( std::make_pair( nextfileid , strvec ) );
		}


		//------------------------------------------
		// �����Ѿ�����õ�map
		//------------------------------------------
		String out_terrainname = StringUtility::getFilenameWithoutExtFromAbsPath( output_filename );
		String path = StringUtility::getPathFromAbsoluteFilename( output_filename );

		for ( int i = 0 ; i < fileVec.size() ; ++ i )
		{
			String curoutname;

			if ( i == 0 )
			{
				curoutname = output_filename;
			}
			else
			{
				std::ostringstream o;
				o << path << "\\" << out_terrainname << ".part" << i << ".vgi";
				curoutname = o.str();
			}

			bool writeok = writeToVgiFile( fileVec[i].second , 
				curoutname , fileVec[i].first );

			if ( writeok == false )
			{
				return false;
			}
		}

		return true;
#endif


#endif

	}
	//----------------------------------------------------------------
	bool FileWriterVgi::writeToVgiFromTexNamesWithRename( 
		const vgKernel::StringPairVectorPtr texnames ,
		const String& output_filename )
	{
		using namespace vgKernel;

		ImageManager& imgmgr = ImageManager::getSingleton();

		// ������ļ�����
		StringVectorPtr err_files( new StringVector );

		//------------------------------------------
		// ����Ҫ�ļ��
		//------------------------------------------
		if ( StringUtility::getFileExtension( output_filename ) != "vgi" )
		{
			return false;
		}
		if ( texnames.isNull() == true )
		{
			return false;
		}
		if ( texnames->empty() == true )
		{
			return false;
		}

		//------------------------------------------
		// ���ȶ���������������´���:
		//	-ɾ����dds�ļ�
		//	-ɾ��ͬ���ļ�
		//------------------------------------------
		typedef std::map<String,String> StringMap;

		StringPairVectorPtr input_dds( new StringPairVector );
		{
			StringMap* nameSet = new StringMap;

			StringPairVector::iterator iter = texnames->begin();
			StringPairVector::iterator iter_end = texnames->end();

			for ( ; iter != iter_end ; ++iter )
			{
				if ( ImageUtility::findImageFileExtension( iter->first ) == "dds" &&
					iter->first.empty() == false )
				{
					nameSet->insert( std::make_pair( iter->first , iter->second ) );
				}
			}

			input_dds->assign( nameSet->begin() , nameSet->end() );
			delete nameSet;
		}

		//------------------------------------------
		// ��ȡÿ�������ļ�,�жϴ��ڵ�ͬʱ,����
		// �ļ��Ĵ�С,Ȼ������ж�����и�vgi�ļ�.
		//------------------------------------------
		uint64 wholesize = 0;		// �ܹ����ļ���С.
		unsigned int one_vgi_size = 0;
		int cur_list_num = 0;

		typedef std::vector<StringPairVectorPtr> StringPairVecPtrVec;
		StringPairVecPtrVec split_list;

		int dds_size = input_dds->size();
		StringPairVector& ddsvec = *input_dds.getPointer();

		for ( int i = 0 ; i < dds_size ; ++ i )
		{
			ImageMiniature* img = imgmgr.getImageMiniature( ddsvec[i].first );

			if ( img == NULL )
			{
				err_files->push_back( ddsvec[i].first );
				continue;
			}

			int filesize = img->getMetaFileSize();

			if ( filesize == 0 )
			{
				err_files->push_back( ddsvec[i].first );
				continue;
			}

			wholesize += filesize;
			one_vgi_size += filesize;

			if ( one_vgi_size >= VGI_MAX_SINGLE_FILE_SIZE )
			{
				one_vgi_size = filesize;	// ע�����ﲻ��0
				cur_list_num ++;
			}

			if ( cur_list_num == split_list.size() )
			{
				split_list.push_back( StringPairVectorPtr( new StringPairVector() ) );
			}

			assert( cur_list_num < split_list.size() );

			// ������
			split_list[ cur_list_num ]->push_back(
				std::make_pair( ddsvec[i].first , ddsvec[i].second ) );
		}

		//------------------------------------------
		// ��ʼ����
		//------------------------------------------
		String metaname =
			StringUtility::getFilenameWithoutExtFromAbsPath( output_filename );
		String path = StringUtility::getPathFromAbsoluteFilename( output_filename );

		for ( int i = 0 ; i < split_list.size() ; ++ i )
		{
			String curoutname;

			if ( i == 0 )
			{
				curoutname = output_filename;
			}
			else
			{
				std::ostringstream o;
				o << path << metaname << ".part" << i << ".vgi";
				curoutname = o.str();
			}

			bool writeok = writeToVgiFileWithRename( split_list[i] , 
				curoutname , 
				( i == split_list.size() - 1 ) ? 0 : i + 1 );

			if ( writeok == false )
			{
				return false;
			}
		}

		return true;
	}

	//----------------------------------------------------------------
	bool FileWriterVgi::writeToVgiFromTexNamesWithRename( 
		const vgKernel::StringPairVectorPtr texnames , 
		const String& output_filename, 
		vgTerritory::ptIndexMap* pixelIndexMap )
	{
		using namespace vgKernel;
		
		assert( pixelIndexMap != NULL );
		s_pixelIndexMap = pixelIndexMap;

		s_ddsFistName = 
			StringUtility::getFilenameWithoutExtFromAbsPath( output_filename );
		assert( s_ddsFistName != "" );

		ImageManager& imgmgr = ImageManager::getSingleton();


		// ������ļ�����
		StringVectorPtr err_files( new StringVector );

		//------------------------------------------
		// ����Ҫ�ļ��
		//------------------------------------------
		if ( StringUtility::getFileExtension( output_filename ) != "vgi" )
		{
			return false;
		}
		if ( texnames.isNull() == true )
		{
			return false;
		}
		if ( texnames->empty() == true )
		{
			return false;
		}

		//------------------------------------------
		// ���ȶ���������������´���:
		//	-ɾ����dds�ļ�
		//	-ɾ��ͬ���ļ�
		//------------------------------------------
		typedef std::map<String,String> StringMap;

		StringPairVectorPtr input_dds( new StringPairVector );
		{
			StringMap* nameSet = new StringMap;

			StringPairVector::iterator iter = texnames->begin();
			StringPairVector::iterator iter_end = texnames->end();

			for ( ; iter != iter_end ; ++iter )
			{
				if ( ImageUtility::findImageFileExtension( iter->first ) == "dds" &&
					iter->first.empty() == false )
				{
					nameSet->insert( std::make_pair( iter->first , iter->second ) );
				}
			}

			input_dds->assign( nameSet->begin() , nameSet->end() );
			delete nameSet;
		}

		//------------------------------------------
		// ��ȡÿ�������ļ�,�жϴ��ڵ�ͬʱ,����
		// �ļ��Ĵ�С,Ȼ������ж�����и�vgi�ļ�.
		//------------------------------------------
		uint64 wholesize = 0;		// �ܹ����ļ���С.
		unsigned int one_vgi_size = 0;
		int cur_list_num = 0;

		typedef std::vector<StringPairVectorPtr> StringPairVecPtrVec;
		StringPairVecPtrVec split_list;

		int dds_size = input_dds->size();
		StringPairVector& ddsvec = *input_dds.getPointer();

		for ( int i = 0 ; i < dds_size ; ++ i )
		{
			ImageMiniature* img = imgmgr.getImageMiniature( ddsvec[i].first );
			
			if ( img == NULL )
			{
				err_files->push_back( ddsvec[i].first );
				continue;
			}

			int filesize = img->getMetaFileSize();

			if ( filesize == 0 )
			{
				err_files->push_back( ddsvec[i].first );
				continue;
			}

			wholesize += filesize;
			one_vgi_size += filesize;

			if ( one_vgi_size >= VGI_MAX_SINGLE_FILE_SIZE )
			{
				one_vgi_size = filesize;	// ע�����ﲻ��0
				cur_list_num ++;
			}

			if ( cur_list_num == split_list.size() )
			{
				split_list.push_back( StringPairVectorPtr( new StringPairVector() ) );
			}

			assert( cur_list_num < split_list.size() );

			// ������
			split_list[ cur_list_num ]->push_back(
				std::make_pair( ddsvec[i].first , ddsvec[i].second ) );
		}

		//------------------------------------------
		// ��ʼ����
		//------------------------------------------
		String metaname =
			StringUtility::getFilenameWithoutExtFromAbsPath( output_filename );
		String path = StringUtility::getPathFromAbsoluteFilename( output_filename );

		// ����һ���ڴ���Ϊ������ɫ����������vgi���ͷ�
		if ( split_list.size() > 0 )
		{
			vgKernel::StringPairVector::iterator iter = split_list[0]->begin();
			setBackgroundPicBuf( iter->first );
		}


		for ( int i = 0 ; i < split_list.size() ; ++ i )
		{
			String curoutname;

			if ( i == 0 )
			{
				curoutname = output_filename;
			}
			else
			{
				std::ostringstream o;
				o << path << metaname << ".part" << i << ".vgi";
				curoutname = o.str();
			}

			// �˲���ѡ���������������Ϊ����ɫ��������dds�ļ�
			vgKernel::StringVector resultDdsVec;
			bool setok = dealTextureData( split_list[i], resultDdsVec );

			if ( !setok )
			{
				String msg = "����ѡ�������������ʱ����";
				vgKernel::SystemUtility::showModelDialog( msg );
				return false;
			}

			// ��dds�ļ�д�뵽vgi��
			vgImage::FileWriterDdsToVgi vgiwriter;

			vgKernel::StringVectorPtr strVecPtr( new StringVector );
			for ( int i = 0; i < resultDdsVec.size(); i++ )
			{
				strVecPtr->push_back( resultDdsVec[i] );
			}

			vgiwriter.writeToVgiFileFromDdsNames(
				strVecPtr, output_filename, false );

		}

		// �Ƴ���ʱĿ¼
		assert( s_tempPathDir != "" );
		if ( s_tempPathDir == "" )
		{
			return false;
		}

		bool exist = vgKernel::SystemUtility::checkDirectoryExist( s_tempPathDir );
		if ( exist )
		{
			bool removeOk = vgKernel::SystemUtility::removeDirectory( s_tempPathDir );
			if ( !removeOk )
			{
				String msg = "�Ƴ���ʱĿ¼ʧ�ܣ�";
				vgKernel::SystemUtility::showModelDialog( msg );
				return false;
			}
		}

		// ��������Ϊ��ʼ��
		s_pixelIndexMap = NULL;
		s_tempPathDir = "";
		s_ddsFistName = "";

		if ( s_backgroundPicBuf != NULL )
		{
			delete[] s_backgroundPicBuf;
			s_backgroundPicBuf = NULL;
		}

		return true;
	}

	//----------------------------------------------------------------
	bool FileWriterVgi::writeToVgiFileWithRename( 
		const vgKernel::StringPairVectorPtr texnames ,
		const String& output_filename , const long& nextfileid )
	{
		if ( texnames.isNull() == true )
		{
			MessageBox( NULL , "������������Ϊ��" , "Error" , MB_OK );
			return false;
		}

		if ( texnames->empty() == true )
		{
			return false;
		}

		//------------------------------------------
		// 1.���ȶ���������������´���:
		//	-ɾ����dds�ļ�
		//	-ɾ��ͬ���ļ�
		//------------------------------------------
		typedef std::map<String, String> StringMap;
		StringMap nameSet;
		StringVector errList;

		{
			vgKernel::StringPairVector::iterator iter = texnames->begin();
			vgKernel::StringPairVector::iterator iter_end = texnames->end();

			for ( ; iter != iter_end ; ++iter )
			{
				if ( ImageUtility::findImageFileExtension( iter->first ) == "dds" &&
					iter->first.empty() == false &&
					iter->second.size() < VGI_NAME_LENGTH - 1 )
				{
					nameSet.insert(  std::make_pair( iter->first , iter->second ) );
				}
				else
				{
					errList.push_back( iter->first );
				}
			}
		}

		if ( nameSet.empty() == true )
		{
			return false;
		}

		//------------------------------------------
		// 2.����д���ļ�Stream
		//------------------------------------------
		vgKernel::StreamWriterPtr pwriter = 
			vgKernel::StreamWriterFactory::createFromFile( output_filename , true );

		if ( pwriter.isNull() == true )
		{
			return false;
		}

		if ( pwriter->isValid() == false )
		{
			return false;
		}

		//------------------------------------------
		// 3.дͷ�ļ�
		//------------------------------------------
		VgiHeader header;
		long filebegin = pwriter->tell();

		pwriter->write( &header , sizeof( header ) );

		vgImage::ImageManager& imgmgr = 
			vgImage::ImageManager::getSingleton();

		//------------------------------------------
		// 4.��ʼ�õ��ļ�����,�����ļ����ݵõ�briefs
		//------------------------------------------
		VgiBriefPtrVec briefs;
		vgKernel::StringVector validLIst;

		{
			StringMap::iterator iter = nameSet.begin();
			StringMap::iterator iter_end = nameSet.end();

			for ( ; iter != iter_end ; ++iter )
			{
				long writebegpos = pwriter->tell();

				ImageMiniature* img = imgmgr.getImageMiniature( iter->first );

				if ( img == NULL )
				{
					errList.push_back( iter->first );
					continue;
				}

				IoRequestDumpImage request( img );
				request.sendRequestAndDoAsynHandlerFunc( true );

				VgiBrief* brief = request.getBriefFromBuffer();
				assert( brief != NULL );

				//------------------------------------------
				// ע�����������
				//------------------------------------------
				if ( iter->second.size() > VGI_NAME_LENGTH - 1 )
				{
					errList.push_back( iter->first );
					continue;
				}

				StringUtility::toLowerCase( iter->second );
				strcpy( brief->_fileName , iter->second.c_str() );
				assert( brief->_fileName[ VGI_NAME_LENGTH - 1 ] == 0 );

				//------------------------------------------
				// ��ʼд��buffer
				//------------------------------------------
				// д��buf
				char* buf = request.getBuffer();
				long buf_len = request.getBufferSize();

				long writenum = pwriter->write( buf , buf_len );
				assert( writenum == buf_len );

				// ����brief
				brief->_metaFileOffset = writebegpos;

				briefs.push_back( VgiBriefPtr( brief ) );
				validLIst.push_back( iter->first );

				delete[] buf;
				buf = NULL;
			}
		}

		assert( briefs.size() == validLIst.size() );
		//------------------------------------------
		// 5.д��briefs,������header
		//------------------------------------------
		//����header
		header._positionOfBriefs = pwriter->tell();
		header._numberOfBriefs = briefs.size();
		// ����4�ֽ���У����
		header._sizeOfWholeFile = header._positionOfBriefs +
			header._numberOfBriefs * header._lengthOfOneBrief + sizeof( DWORD );

		header._nextFileID = nextfileid;

		assert( sizeof( DWORD ) == 4 );

		//------------------------------------------
		// ��дheader
		//------------------------------------------
		pwriter->seek( filebegin );
		pwriter->write( &header , sizeof( header ) );

		// ��ת��дbrief����
		pwriter->seek( header._positionOfBriefs );

		//------------------------------------------
		// д��brief
		//------------------------------------------
		{
			VgiBriefPtrVec::iterator iter = briefs.begin();
			VgiBriefPtrVec::iterator iter_end = briefs.end();

			for ( ; iter != iter_end ; ++iter )
			{
				VgiBrief* curbrief = (*iter).getPointer();
				long writenum =pwriter->write( curbrief, sizeof( VgiBrief ));
				assert( writenum == sizeof( VgiBrief ) );
			}
		}


		//------------------------------------------
		// 6.��ʼ����У����
		//------------------------------------------
		DWORD crcCode;

		CrcUtility::initCrc32Code( crcCode );

		// У��header
		for ( int i = 0 ; i < sizeof( header ) ; ++ i )
		{
			CrcUtility::calculateCrc32Code( (char*)&header + i , crcCode );
		}

		// У��������
		{
			StringVector::iterator iter = validLIst.begin();
			StringVector::iterator iter_end = validLIst.end();

			for ( ; iter != iter_end ; ++iter )
			{
				ImageMiniature* img = imgmgr.getImageMiniature( *iter );
				assert( img != NULL );

				IoRequestDumpImage request( img );
				request.sendRequestAndDoAsynHandlerFunc( true );

				char* buf = request.getBuffer();
				long buf_len = request.getBufferSize();

				for ( int  i = 0 ; i < buf_len ; ++ i )
				{
					CrcUtility::calculateCrc32Code( buf + i , crcCode );
				}

				delete[] buf;
				buf = NULL;
			}
		}

		// У��brief
		{
			VgiBriefPtrVec::iterator iter = briefs.begin();
			VgiBriefPtrVec::iterator iter_end = briefs.end();

			for ( ; iter != iter_end ; ++iter )
			{
				for ( int i = 0 ; i < sizeof( VgiBrief ) ; ++ i )
				{
					CrcUtility::calculateCrc32Code( (char*)(*iter).getPointer() + i , crcCode );
				}
			}
		}

		// д��У����
		long writenum =pwriter->write( &crcCode , sizeof( DWORD ) );
		assert( writenum == sizeof( DWORD ) );

		pwriter.setNull();

		//------------------------------------------
		// ��ʼ����ļ���ȷ��
		//------------------------------------------
#if 0
		DWORD checkcode;
		bool getres = CrcUtility::getFileCrc32Code( output_filename , checkcode );
		assert( getres == true );

		if ( checkcode != 0xFFFFFFFF )
		{
			MessageBox( NULL , "�ļ�У��ʧ��,�����±���." , "����ʧ��" , 
				MB_OK | MB_ICONINFORMATION );

			return false;
		}

#endif

		//------------------------------------------
		// ������������report
		//------------------------------------------
		if ( errList.empty() == false )
		{
			std::ostringstream o;
			o << "��������û�в��ҵ���Ӧ��������, �����ļ�����ʽ����ȷ, ������ѱ�����.\n";

			StringVector::iterator iter = errList.begin();
			StringVector::iterator iter_end = errList.end();

			for ( ; iter != iter_end ; ++ iter )
			{
				o << *iter << ", ";
			}

			MessageBox( AfxGetApp()->m_pMainWnd->m_hWnd , 
				o.str().c_str() , "��ʾ", MB_OK );
		}


		return true;
	}
	//----------------------------------------------------------------
	bool FileWriterVgi::dealTextureData( 
		const vgKernel::StringPairVectorPtr texnames,
		vgKernel::StringVector& outVec )
	{
		if ( texnames.isNull() == true )
		{
			String msg = "������������Ϊ��!";
			vgKernel::SystemUtility::showModelDialog( msg );
			return false;
		}

		if ( texnames->empty() == true )
		{
			return false;
		}

		//------------------------------------------
		// 1.���ȶ���������������´���:
		//	-ɾ����dds�ļ�
		//	-ɾ��ͬ���ļ�
		//------------------------------------------
		typedef std::map<String, String> StringMap;
		StringMap nameSet;
		StringVector errList;

		{
			vgKernel::StringPairVector::iterator iter = texnames->begin();
			vgKernel::StringPairVector::iterator iter_end = texnames->end();

			for ( ; iter != iter_end ; ++iter )
			{
				if ( ImageUtility::findImageFileExtension( iter->first ) == "dds" &&
					iter->first.empty() == false &&
					iter->second.size() < VGI_NAME_LENGTH - 1 )
				{
					nameSet.insert(  std::make_pair( iter->first , iter->second ) );
				}
				else
				{
					errList.push_back( iter->first );
				}
			}
		}

		if ( nameSet.empty() == true )
		{
			return false;
		}


		//------------------------------------------
		// 2. ����dds��������������ͼ 
		//------------------------------------------
		vgImage::ImageManager& imgmgr = 
			vgImage::ImageManager::getSingleton();
		StringMap::iterator iter = nameSet.begin();
		StringMap::iterator iter_end = nameSet.end();

		for ( ; iter != iter_end ; ++iter )
		{
			ImageMiniature* img = imgmgr.getImageMiniature( iter->first );

			if ( img == NULL )
			{
				errList.push_back( iter->first );
				continue;
			}

			IoRequestDumpImage request( img );
			request.sendRequestAndDoAsynHandlerFunc( true );

			char* buf = request.getBuffer();
			buf += 128;  // ����dds�ļ�ͷ��128���ֽ�

			long buf_len = request.getBufferSize();
			buf_len -= 128;

			int lod = 2;
			GLuint textureId =  img->createTextureID( buf, buf_len, lod );  // ��dds������������

			// ��ʱbuf�ڴ���ͷ�
			buf -= 128;
			delete[] buf;
			buf = NULL;

			long buf_size = img->getHeight()*img->getWidth()*3; // RGBģʽ ÿ����ռ3�ֽ�

			char* src_buffer = new char[buf_size];
			char* dst_buffer = new char[buf_size];
			memset( src_buffer, 0, buf_size );
			memset( dst_buffer, 0, buf_size );

			assert( s_backgroundPicBuf != NULL );
			memcpy( dst_buffer, s_backgroundPicBuf, buf_size );

			// ��������ID���������ݶ����ڴ����Ա����
			getTexDataFormTexture( textureId, src_buffer, img->getWidth() ); // ���=�߶�

			// �������������ñ�����������Ϊ����ɫ
			setPixelColor( src_buffer, dst_buffer, img->getMetaFileName(), img->getWidth() );

			// ���ڴ��е��������ݴ��dds�ļ�,����dds�ľ���·��
			String dds_abs_path = saveDataToDds( dst_buffer, img->getMetaFileName(), img->getWidth() );
			if ( dds_abs_path == "" )
			{
				String msg = "bmpתddsʧ�ܣ�";
				vgKernel::SystemUtility::showModelDialog( msg );
				return false;
			}

			outVec.push_back( dds_abs_path );

		}


		return true;
	}

	//----------------------------------------------------------------
	void FileWriterVgi::setBackgroundPicBuf( String ddsName )
	{
		ImageManager& imgmgr = ImageManager::getSingleton();
		ImageMiniature* img = imgmgr.getImageMiniature( ddsName );
		
		assert( img != NULL );
		if ( img == NULL )
		{
			return;
		}
		
		long buf_size = img->getWidth()*img->getHeight()*3;
		s_backgroundPicBuf = new char[buf_size];
		memset( s_backgroundPicBuf, 0, buf_size );

		for ( int i = 0; i < buf_size / 3; i++ )
		{
			s_backgroundPicBuf[3*i]   = 256*0.5;
			s_backgroundPicBuf[3*i+1] = 256*0.6;
			s_backgroundPicBuf[3*i+2] = 256*0.8;
		}
	}

	//----------------------------------------------------------------
	bool FileWriterVgi::getTexDataFormTexture( GLuint texId, char* src_buf, int texWidth )
	{
		assert( texId != 0 );
		assert( src_buf != NULL );

		if ( texId == 0 || src_buf == NULL )
		{
			return false;
		}

		glBindTexture( GL_TEXTURE_2D , texId );

		glGetTexLevelParameteriv( GL_TEXTURE_2D , 0 , GL_TEXTURE_WIDTH , &texWidth );
		glGetTexLevelParameteriv( GL_TEXTURE_2D , 0 , GL_TEXTURE_HEIGHT , &texWidth );

		// �������ж�ȡ���ݵ��ڴ�buf��
		glGetTexImage( GL_TEXTURE_2D , 0 , GL_RGB , GL_UNSIGNED_BYTE , src_buf );

		// �˴�������ID��Ϊ�˶��������ݶ����ڴ��У��ʶ����Ӧ�����ͷ�����ռ�е��Դ�
		glDeleteTextures( 1, &texId );

		return true;
	}

	//----------------------------------------------------------------
	bool FileWriterVgi::setPixelColor( char* src_buf, char* dst_buf, String ddsName, int texWidth )
	{
		assert( src_buf != NULL );
		assert( dst_buf != NULL );
		assert( ddsName != "" );
		
		if ( src_buf == NULL || dst_buf == NULL || ddsName == "" )
		{
			return false;
		}

		// bmp���½�Ϊ��һ�����أ����辵��ȡֵ
		// des���Ͻ�ȡsrc���½ǣ�des���Ͻ�ȡsrc���½�
		vgTerritory::ptIndexMap::iterator itr = s_pixelIndexMap->find( ddsName );

		if ( itr != s_pixelIndexMap->end() )
		{
			vgTerritory::ptIndexVec* indexVec = itr->second;
			assert( indexVec != NULL );

			for ( int i = 0; i < indexVec->size(); i++ )
			{
				int des_index = indexVec->at(i);

				int picRow = des_index / texWidth;
				int picCol = des_index % texWidth;

				int src_index = ( texWidth - 1 - picRow )*texWidth + picCol;

				dst_buf[3*des_index]    = src_buf[3*src_index];   //����ɫR���� 
				dst_buf[3*des_index+1]  = src_buf[3*src_index+1]; //����ɫG����
				dst_buf[3*des_index+2]  = src_buf[3*src_index+2]; //����ɫB����
			}
		}
		else // ���������и���ű���
		{
			memcpy( dst_buf, src_buf, texWidth*texWidth*3 );
			
			int size = texWidth*3;
			char* temp = new char[size];
			memset( temp, 0, size );

			for ( int i = 0; i < texWidth / 2; i++ )
			{
				char* pa = dst_buf + i * size;
				char* pb = dst_buf + ( texWidth - 1 - i ) * size;

				memcpy( temp, pa, size );
				memcpy( pa, pb, size );
				memcpy( pb, temp, size );
			}
		}

		// ������Ҫsrc���ݣ��ͷ��ڴ�
		delete[] src_buf;
		src_buf = NULL;

		return true;
	}

	//----------------------------------------------------------------
	String FileWriterVgi::saveDataToDds( const char* dst_buf, String ddsFileName, int texWidth )
	{
		assert( dst_buf != NULL );
		assert( ddsFileName != "" );

		if ( dst_buf == NULL || ddsFileName == "" )
		{
			return "";
		}

		//================================================================
		// 1. �Ƚ����ݱ����bmp�ļ�
		vgImage::CximageWrapper bmpimg;
		bmpimg.createFromArray( (void*)dst_buf, texWidth, texWidth, 24, true, true );

			// ��ϵͳ��ʱĿ¼�����ļ���
		String sysTempPath = vgKernel::SystemUtility::getSystemTempPath();
		String abs_path = sysTempPath + "\\bmpForCutScene";

		s_tempPathDir = abs_path; // �����Ƴ�ǰһ��������ʱ�ļ���

		bool exist = vgKernel::SystemUtility::checkDirectoryExist( abs_path );
		if ( !exist )
		{
			bool creatOk = vgKernel::SystemUtility::createDirectory( abs_path );
			if ( !creatOk )
			{
				String msg = "��ϵͳ��ʱĿ¼�����ļ���ʧ�ܣ�";
				vgKernel::SystemUtility::showModelDialog( msg );
				return "";
			}
		}

			// ȷ��bmp�ļ�����
		String fileNameWithoutExt = vgKernel::StringUtility::getFilenameWithoutExtension( ddsFileName );
		String bmp_name = fileNameWithoutExt + ".bmp";

			// �����ڴ��е����ݵ�ָ��Ŀ¼��bmp�ļ�
		String abs_bmp = abs_path + "\\" + bmp_name;
		bmpimg.saveToFile( abs_bmp );


		delete[] dst_buf;
		dst_buf = NULL;


		//================================================================
		// 2. ��bmp�ļ�ת����dds�ļ�
		FreeImageWrapper img;
			
			// ����bmp�ļ�
		bool loadok = img.loadFromFile( abs_bmp );
		if ( loadok == false )
		{
			return "";
		}
			
			// �˴�dds�ļ�����������vtr��vgtr��������һ��
		// ������Ӳ����Ϊ��terrain_level0-9-7.dds
		String dds_otherName = fileNameWithoutExt + ".dds";
		String::size_type pos = dds_otherName.find_first_of( "_");
		dds_otherName = dds_otherName.substr( pos );

		String dds_name = s_ddsFistName + dds_otherName;
		String abs_dds = abs_path + "\\" + dds_name;
			
			// ��ָ����ʽ���浽ָ��Ŀ¼��dds�ļ�
		GLenum dds_format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT; //DXT1��ѹ����ʽ

		bool saveok = img.saveToDdsFile( abs_dds , dds_format , true );
		if ( saveok == false )
		{
			String msg = "����dds�ļ�ʧ�ܣ�";
			vgKernel::SystemUtility::showModelDialog( msg );
			return "";
		}


		//================================================================
		// 3. �Ƴ�bmp�ļ�
		bool removeOk = vgKernel::SystemUtility::removeFile( abs_bmp, true );
		if ( !removeOk )
		{
			String msg = "�Ƴ�bmp�ļ�ʧ��!";
			vgKernel::SystemUtility::showModelDialog( msg );
			return "";
		}

		return abs_dds;
	}


}// end of namespace vgImage
