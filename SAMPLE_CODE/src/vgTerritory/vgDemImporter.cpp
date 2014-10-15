


#include <vgStableHeaders.h>
#include <vgTerritory/vgDemImporter.h>
#include <vgImage/vgiImageUtility.h>
#include <vgKernel/vgkStreamReader.h>
#include <vgKernel/vgkStreamWriter.h>

namespace vgTerritory{



	//----------------------------------------------------------------
	DemBlockPtr DemImporter::readDemFile( const String& dem_file )
	{
		vgKernel::StreamReaderPtr pstream = 
			vgKernel::StreamReaderFactory::createFromFile( dem_file );
		
		if ( pstream.isNull() == true )
		{
			errorMsgOutput("Dem file is not exist!");
			return DemBlockPtr();
		}

		String basename =
			vgImage::ImageUtility::findFilenameFromAbsolutePath( dem_file );

		//------------------------------------------
		// ��ȡ�ļ�ͷ
		//------------------------------------------
		uint ncols			= 0;
		uint nrows			= 0;
		double xllcorner		= 0;
		double yllcorner 		= 0;
		float cellsize		= 0;
		int NODATA_value	= 0;

		String bufferstr;

		//------------------------------------------
		// ���������ݵ�stringstream
		//------------------------------------------
		int filesize = pstream->size();
		char *buf = new char[ filesize ];
		pstream->read( buf , filesize );
		std::stringstream stream(buf);
		delete[] buf;

		//------------------------------------------
		// read ncols
		//------------------------------------------
		stream >> bufferstr;
		if ( bufferstr != "ncols" )
		{
			errorMsgOutput("DEM file is corrupted!");
			return DemBlockPtr();
		}
		stream >> ncols;
		assert( ncols != 0 );

		//------------------------------------------
		// read nrows
		//------------------------------------------
		stream >> bufferstr;
		if ( bufferstr != "nrows" )
		{
			errorMsgOutput("DEM file is corrupted!");
			return DemBlockPtr();
		}
		stream >> nrows;
		assert( nrows != 0 );

		//------------------------------------------
		// read xllcorner
		//------------------------------------------
		stream >> bufferstr;
		if ( bufferstr != "xllcorner" )
		{
			errorMsgOutput("DEM file is corrupted!");
			return DemBlockPtr();
		}
		stream >> xllcorner;
		assert( xllcorner != 0 );


		//------------------------------------------
		// read yllcorner
		//------------------------------------------
		stream >> bufferstr;
		if ( bufferstr != "yllcorner" )
		{
			errorMsgOutput("DEM file is corrupted!");
			return DemBlockPtr();
		}
		stream >> yllcorner;
		assert( yllcorner != 0 );

		//------------------------------------------
		// read cellsize
		//------------------------------------------
		stream >> bufferstr;
		if ( bufferstr != "cellsize" )
		{
			errorMsgOutput("DEM file is corrupted!");
			return DemBlockPtr();
		}
		stream >> cellsize;
		assert( cellsize != 0 );

		//------------------------------------------
		// read NODATA_value
		//------------------------------------------
		stream >> bufferstr;
		if ( bufferstr != "NODATA_value" )
		{
			errorMsgOutput("DEM file is corrupted!");
			return DemBlockPtr();
		}
		stream >> NODATA_value;
		assert( NODATA_value != 0 );


		//------------------------------------------
		// begin to read the block data
		//------------------------------------------

		float* realdata = new float[ ncols * nrows ];

		for ( int i = 0 ; i <  ncols * nrows ; ++ i )
		{
			// ע��,����ת�ò���,����������Ĳ�һ��
			//stream >> realdata[ (i % ncols)*nrows + i / ncols ];
			//stream >> realdata[i];
			stream >> realdata[ (ncols - 1 - i % ncols) * nrows + i / ncols ];
		}

		//------------------------------------------
		// �õ���С�߶�ֵ�����߶�ֵ
		//------------------------------------------
		assert( ncols != 0 );
		assert( nrows != 0 );
		float origin_y = NODATA_value;
		float top_y = NODATA_value;
		bool tmpfirsttime = true;
		for ( int i = 0 ; i <  ncols * nrows ; ++ i )
		{
			if ( NODATA_value == realdata[i] )
			{
				continue;
			}

			if ( tmpfirsttime == true )
			{
				tmpfirsttime = false;
				origin_y = realdata[i];
				top_y = realdata[i];
			}

			if ( origin_y > realdata[i] )
			{
				origin_y = realdata[i];
			}

			if ( top_y < realdata[i] )
			{
				top_y = realdata[i];
			}
		}

		//------------------------------------------
		// ��ֵ��nodata����
		// ע��,����Ч�ʷǳ���,��������һ���Բ���
		// ��ʱ����
		//------------------------------------------
		for ( int i = 0 ; i <  ncols * nrows ; ++ i )
		{
			if (  NODATA_value == realdata[i]  )
			{
				realdata[i] = origin_y;
			}
		}

		//------------------------------------------
		// ��ֵ��Demblock������
		//------------------------------------------
		DemBlock* retblock = new DemBlock;

		//------------------------------------------
		// ����dem������x����,������ȡ��
		//------------------------------------------
		retblock->setData( basename , 
			ncols , nrows , xllcorner , -yllcorner , origin_y ,top_y ,
			cellsize , realdata );

		//VG_TRACE( retblock->getDetails() );

		return DemBlockPtr(retblock);
	}
	//----------------------------------------------------------------
	void DemImporter::errorMsgOutput( const String& errMsg /*= "" */ )
	{
		MessageBox(NULL , errMsg.c_str() , "Dem��ȡ����!" , MB_OK );
		return;
	}
	//----------------------------------------------------------------

	//----------------------------------------------------------------

}