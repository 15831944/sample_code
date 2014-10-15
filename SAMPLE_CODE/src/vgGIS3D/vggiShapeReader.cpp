




#include <vgStableHeaders.h>
#include <vgGis3d/vggiShapeReader.h>
#include <vgGis3d/vggiPolygon.h>
#include <vgGis3d/vggiMultiPolygon.h>
#include <vgGis3d/vggiOgrUtility.h>

#include <vgGis3d/vggiLine.h>
#include <vgGis3d/vggiPoint.h>
#include <vgGis3d/vggiMultiPoint.h>
#include <vgGis3d/vggiMultiLine.h>
#include <vgGis3d/vggiBuildingSolid.h>

#include <gdal/ogrsf_frmts.h>
#include <gdal/ogr_api.h>

#include <vgKernel/vgkSystemUtility.h>


namespace vgGIS3D {
	
	
	
	string ShapeReader::s_3dSolidFloorFiledName = "";
	string ShapeReader::s_3dSolidHeightFiledName = "";
	
	vector<string> ShapeReader::m_floorHeightProp;
	vector<string> ShapeReader::m_floorProp;

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

		VGGI_CHECK_OGR_ERROR();

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
			o << "������ʱshp�ļ�:" << temp_path << "ʱ, ogr��������.";
			MessageBox( NULL , o.str().c_str(), "��ʾ" , MB_OK );
#else
			std::ostringstream o;
			o << "When create shp:" << temp_path << ", ogr failed.";
			MessageBox( NULL , o.str().c_str(), "tips" , MB_OK );
#endif
			return DataSourcePtr();
		}

		VGGI_CHECK_OGR_ERROR();
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

		VGGI_CHECK_OGR_ERROR();
		
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
		DataSourcePtr re ( new DataSource( abs_shp_path , poDstDS , layers ,
			DataSource_SHP ));
		
		re->setType(DataSource_SHP);

		return re;  
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

			VGGI_CHECK_OGR_ERROR();

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
			int fieldid = def->GetFieldIndex(VG_GIS_STYLE_FIELD);
			assert( fieldid != -1 );

			VGGI_CHECK_OGR_ERROR();

			LayerPtr pLayer = readLayerFromOGRLayer( poLayer , geo_filter , use_special_polygon );
			pLayer->setLayerType(Layer_Type_SHP);

			VGGI_CHECK_OGR_ERROR();

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

		if (ogr_layer->GetLayerDefn()->GetGeomType() == wkbPolygon25D)
		{
			// AfxMessageBox("25D Poly");
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

			VGGI_CHECK_OGR_ERROR();

			//------------------------------------------
			// ����ֻ��polygon���д���
			//------------------------------------------
			OGRwkbGeometryType geotype = geometry->getGeometryType();
			Geometry* shapegeo = NULL;
			int fieldIndex = -1;
			switch( wkbFlatten( geotype ) )
			{
			case wkbPolygon:

				for (int i=0; i<m_floorProp.size(); i++)
				{
					fieldIndex = poFeature->GetFieldIndex(m_floorProp[i].c_str());

					if (fieldIndex != -1)
					{
						break;
					}
				}

				TRACE("Field index %d \n", fieldIndex);

				if ( fieldIndex != -1 )
				{
					// Building Solid
					//shapegeo = new SpecialPolygon( poFeature , ret.getPointer() );
					BuildingSolid *pbS = new BuildingSolid( poFeature, ret.getPointer() );
					pbS->setFloor(
						poFeature->GetFieldAsInteger(fieldIndex));
					
					// fieldIndex = poFeature->GetFieldIndex(s_3dSolidHeightFiledName.c_str());
					fieldIndex = -1;
					for (int i=0; i<m_floorHeightProp.size(); i++)
					{
						fieldIndex = poFeature->GetFieldIndex(m_floorHeightProp[i].c_str());

						if (fieldIndex != -1)
						{
							break;
						}
					}

					if (fieldIndex != -1)
					{
						pbS->setHeightPerLayer(
							poFeature->GetFieldAsInteger(fieldIndex));
					}
					else
					{
						pbS->setHeightPerLayer(3);
					}

					shapegeo = pbS;
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
				//assert(0);//
				shapegeo = new MultiPoint( poFeature , ret.getPointer() );
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
				VGGI_CHECK_OGR_ERROR();

				bool readok = shapegeo->readDataFromOGRFeature();

				VGGI_CHECK_OGR_ERROR();

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


			// ��������ɫ��Ĭ�ϰ���¥��߶���ɫ
#if 0
			BuildingSolid *pB = dynamic_cast<BuildingSolid*>(shapegeo);
	
			if (pB != NULL)
			{
				pB->colorSolidByHeight(true);
			}
#else

			BuildingSolid *pB = dynamic_cast<BuildingSolid*>(shapegeo);

			if (pB != NULL)
			{
				pB->colorSolidByHeight(false);        //Edit by KingHJ �����������Ľ�����ɫ��Ϊ��ɫ
			}
#endif

			VGGI_CHECK_OGR_ERROR();
			OGRFeature::DestroyFeature( poFeature );
		}

		return ret;
	}
	//----------------------------------------------------------------


}// end of namespace vgShape
