




#include <vgStableHeaders.h>
#include <vgShape/vgshShapeReader.h>
#include <vgShape/vgshPolygon.h>
#include <vgShape/vgshMultiPolygon.h>
#include <vgShape/vgshOgrUtility.h>
#include <vgShape/vgshSpecialPolygon.h>

#include <vgShape/vgshLine.h>
#include <vgShape/vgshPoint.h>
#include <vgShape/vgshMultiPoint.h>
#include <vgShape/vgshMultiLine.h>

#include <gdal/ogrsf_frmts.h>
#include <gdal/ogr_api.h>

#include <vgKernel/vgkSystemUtility.h>


namespace vgShape {
	
	
	
	
	//----------------------------------------------------------------
	DataSourcePtr ShapeReader::readFromShpFile( const String& abs_shp_path ,
		const String& temp_path , const OGRGeometry* geo_filter /*= NULL */,
		const bool& use_special_polygon /*= false*/ )
	{
		using namespace vgKernel;

		if ( vgKernel::SystemUtility::checkFileExist( abs_shp_path ) == false )
		{
			return DataSourcePtr();
		}

		//------------------------------------------
		// ɾ����ʱ�ļ�
		//------------------------------------------
		vgKernel::SystemUtility::removeFile( temp_path , false );

		if ( vgKernel::SystemUtility::checkFileExist( temp_path ) == true )
		{
			std::ostringstream o;
#if VGK_LANG_CHS
			o << "������ʱ�ļ�:" << temp_path << "ʱʧ��.";
			MessageBox( NULL , o.str().c_str(), "��ʾ" , MB_OK );
#else
			o << "When create temp file:" << temp_path << " failed.";
			MessageBox( NULL , o.str().c_str(), "Tips" , MB_OK );
#endif
			
			return DataSourcePtr();
		}

		OGRDataSource *poSrcDS = NULL;
		// ֻ����ʽ��
		poSrcDS = OGRSFDriverRegistrar::Open(  abs_shp_path.c_str() , FALSE );
		if( poSrcDS == NULL )
		{
			std::ostringstream o;
#if VGK_LANG_CHS
			o << "���ļ�:" << abs_shp_path << "ʱʧ��.";
			MessageBox( NULL , o.str().c_str(), "��ʾ" , MB_OK );
#else
			o << "When open:" << abs_shp_path << " failed.";
			MessageBox( NULL , o.str().c_str(), "Tips" , MB_OK );
#endif
			
			return DataSourcePtr();
		}

		// ��������ָ��,��֤ɾ��.
		vgKernel::SharePtr<OGRDataSource> srcDSptr( poSrcDS );

		VGSH_CHECK_OGR_ERROR();

		//------------------------------------------
		// ������ʱ��shp�ļ�
		//------------------------------------------
		const char *pszDriverName = "ESRI Shapefile";
		OGRSFDriver *poDriver = NULL;

		poDriver = OGRSFDriverRegistrar::GetRegistrar()->GetDriverByName(
			pszDriverName );
		if( poDriver == NULL )
		{
			std::ostringstream o;
			o << "����Ogrʱ:OGRSFDriverRegistrar::GetRegistrar()->GetDriverByNameʱʧ��.";
			MessageBox( NULL , o.str().c_str(), "��ʾ" , MB_OK );
			return DataSourcePtr();
		}

		OGRDataSource *poDstDS = NULL;

		poDstDS = poDriver->CreateDataSource( temp_path.c_str(), NULL );
		if( poDstDS == NULL )
		{
#if VGK_LANG_CHS
			std::ostringstream o;
			o << "����shp�ļ�:" << temp_path << "ʱ, ogr��������.";
			MessageBox( NULL , o.str().c_str(), "��ʾ" , MB_OK );
#else
			std::ostringstream o;
			o << "When create shp:" << temp_path << ", ogr failed.";
			MessageBox( NULL , o.str().c_str(), "tips" , MB_OK );
#endif
			return DataSourcePtr();
		}

		VGSH_CHECK_OGR_ERROR();
		//------------------------------------------
		// ����ogr
		//------------------------------------------
		bool copyok = OgrUtility::copyDataSource( poSrcDS , poDstDS );
		if ( copyok == false )
		{
			std::ostringstream o;
#if VGK_LANG_CHS
			o << "����shp�ļ�:" << abs_shp_path << "���ļ�:" <<
				temp_path << "ʱ, ��������.";
			MessageBox( NULL , o.str().c_str(), "��ʾ" , MB_OK );
#else
			o << "When copy shp:" << abs_shp_path << " to :" <<
				temp_path << " failed.";
			MessageBox( NULL , o.str().c_str(), "Tips" , MB_OK );
#endif
			return DataSourcePtr();
		}

		VGSH_CHECK_OGR_ERROR();
		
		LayerPtrVector* layers = readLayersFromDataSource( poDstDS , geo_filter ,
			use_special_polygon );

		if ( layers == NULL )
		{
			OGRDataSource::DestroyDataSource( poDstDS );
#if VGK_LANG_CHS
			MessageBox( NULL , "shp������Ϊ��" , "error" , MB_OK );
#else
			MessageBox( NULL , "shp contains nothing" , "error" , MB_OK );
#endif
			return DataSourcePtr();
		}

		//------------------------------------------
		// �ɹ�����
		//------------------------------------------
		return DataSourcePtr( new DataSource( abs_shp_path , poDstDS , layers ,
			use_special_polygon ) );
	}
	//----------------------------------------------------------------
	LayerPtrVector* ShapeReader::readLayersFromDataSource( 
		OGRDataSource* source , 
		const OGRGeometry* geo_filter /*= NULL*/ ,
		const bool& use_special_polygon /*= false */)
	{
		if ( source == NULL )
		{
			return NULL;
		}

		LayerPtrVector* ret = new LayerPtrVector;

		for( int iLayer = 0; iLayer < source->GetLayerCount(); 
			iLayer++ )
		{
			OGRLayer *poLayer = source->GetLayer(iLayer);

			if( poLayer == NULL )
			{
				assert(0);
				delete ret;
				return false;
			}

			VGSH_CHECK_OGR_ERROR();

			//------------------------------------------
			// �����ﶼ�趨Ϊ25D
			//------------------------------------------
			OGRFeatureDefn* def = poLayer->GetLayerDefn();

			if ( wkbFlatten( def->GetGeomType() ) == wkbPolygon )
			{
				def->SetGeomType( wkbPolygon25D );
			}

			//------------------------------------------
			// �趨vrgis field
			//------------------------------------------
			int fieldid = def->GetFieldIndex(VG_SHAPE_STYLE_FIELD);
			assert( fieldid != -1 );

			VGSH_CHECK_OGR_ERROR();

			LayerPtr pLayer = readLayerFromOGRLayer( poLayer , geo_filter , use_special_polygon );

			VGSH_CHECK_OGR_ERROR();

			if ( pLayer.isNull() == true )
			{
				assert(0);
				delete ret;
				return false;
			}

			ret->push_back( pLayer );
		}

		return ret;
	}
	//----------------------------------------------------------------
	LayerPtr ShapeReader::readLayerFromOGRLayer( OGRLayer* ogr_layer , 
		const OGRGeometry* geo_filter /*= NULL*/ ,
		const bool& use_special_polygon /*= false*/ )
	{
		if ( ogr_layer == NULL )
		{
			return LayerPtr();
		}

		//------------------------------------------
		// �趨filter
		//------------------------------------------
		ogr_layer->SetSpatialFilter( (OGRGeometry*)geo_filter );

		LayerPtr ret( new Layer );

		ret->initialise( ogr_layer );

		//------------------------------------------
		// ��ʼ����ÿһ��feature
		//------------------------------------------
		OGRFeature *poFeature = NULL;

		ogr_layer->ResetReading();
		while( (poFeature = ogr_layer->GetNextFeature()) != NULL )
		{
			OGRGeometry* geometry = poFeature->GetGeometryRef();
			assert( geometry != NULL );

			VGSH_CHECK_OGR_ERROR();

			//------------------------------------------
			// ����ֻ��polygon���д���
			//------------------------------------------
			OGRwkbGeometryType geotype = geometry->getGeometryType();
			Geometry* shapegeo = NULL;

			switch( wkbFlatten( geotype ) )
			{
			case wkbPolygon:
				if ( use_special_polygon )
				{
					shapegeo = new SpecialPolygon( poFeature , ret.getPointer() );
				}
				else
				{
					shapegeo = new Polygon( poFeature , ret.getPointer() );
				}
				break;

			case wkbMultiPolygon:
				shapegeo = new MultiPolygon( poFeature , ret.getPointer() );
				break;

			case wkbLineString:
				shapegeo = new LineString( poFeature , ret.getPointer() );
				break;

			case wkbPoint:
				shapegeo = new Point( poFeature , ret.getPointer() );
				break;

			case wkbMultiLineString:
				shapegeo = new MultiLine( poFeature , ret.getPointer() );
				break;

			case wkbMultiPoint:
				assert(0);//shapegeo = new MultiPoint( poFeature , ret.getPointer() );
				break;

			default:
				break;
			}
#if 0
			if ( wkbFlatten( geotype ) == wkbPolygon )
			{
				if ( use_special_polygon )
				{
					shapegeo = new SpecialPolygon( poFeature , ret.getPointer() );
				}
				else
				{
					shapegeo = new Polygon( poFeature , ret.getPointer() );
				}
			}
			else if( wkbFlatten( geotype ) == wkbMultiPolygon )
			{
				shapegeo = new MultiPolygon( poFeature , ret.getPointer() );
			}
			else if ( wkbFlatten( geotype ) == wkbLineString )
			{
				shapegeo = new LineString( poFeature , ret.getPointer() );
			}
			else if ( wkbFlatten( geotype ) == wkbPoint )
			{
				shapegeo = new Point( poFeature , ret.getPointer() );
			}
#endif

			// ��shp�ļ��ж�ȡ����
			if ( shapegeo != NULL )
			{
				VGSH_CHECK_OGR_ERROR();

				bool readok = shapegeo->readDataFromOGRFeature();

				VGSH_CHECK_OGR_ERROR();

				if ( readok == false )
				{
					assert(0);
					delete shapegeo;
					shapegeo = NULL;
				}
			}
		
			// ����layer����
			if ( shapegeo != NULL )
			{
				ret->pushGeometry( shapegeo );
			}

			VGSH_CHECK_OGR_ERROR();
			OGRFeature::DestroyFeature( poFeature );
		}

		return ret;
	}
	//----------------------------------------------------------------


}// end of namespace vgShape
