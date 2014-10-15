



#include <vgStableHeaders.h>
#include <vgGIS3D/vggiMultiPolygon.h>

#include <vgGIS3D/vggiLayer.h>
#include <gdal/ogrsf_frmts.h>
#include <gdal/ogr_api.h>
#include <vgKernel/vgkOpenGLSupport.h>

namespace vgGIS3D {
	
	
	
	
	//----------------------------------------------------------------
	void MultiPolygon::renderUsingDisplayList()
	{
		//assert(0);
		if ( _wkbBuffer == NULL )
		{
			return;
		}

		if ( _callListId == 0 )
		{
			_callListId = glGenLists( 1 );   
			assert( _callListId != 0 );

			glNewList( _callListId, GL_COMPILE );
			renderOnceForDisplayList();
			glEndList();	
		}

		assert( _callListId != 0 );

		glPushMatrix();
		glCallList( _callListId );
		glPopMatrix();

		return;
	}
	//----------------------------------------------------------------
	bool MultiPolygon::readDataFromOGRFeature( )
	{
		OGRFeature* feature = getOGRFeature();

		if ( feature == NULL )
		{
			return false;
		}

		// ��������ָ������.
		assert( feature  != NULL );
		OGRFeaturePtr featPtr( feature );

		// ��ʼ��ȡwkb����
		OGRGeometry* geo = featPtr->GetGeometryRef();

		assert( geo != NULL );

		//geo->getEnvelope(&_envelope);

		if ( wkbFlatten( geo->getGeometryType() ) != wkbMultiPolygon )
		{
			assert(0);
			return false;
		}

		assert( wkbFlatten( geo->getGeometryType() )
			== wkbMultiPolygon );

		OGRMultiPolygon* poly = ( OGRMultiPolygon* )geo;

		//------------------------------------------
		// �趨Ϊ25Dģʽ
		//------------------------------------------
		int dimension = geo->getDimension();
		if ( dimension != 3 )
		{
			geo->setCoordinateDimension( 3 );
		}

		assert( _wkbSize == -1 );

		_wkbSize = poly->WkbSize();
		assert( _wkbSize > 0 );

		if ( _wkbSize <= 0 )
		{
			return false;
		}

		assert( _wkbBuffer == NULL );

		_wkbBuffer = new uchar[ _wkbSize ];
		memset( _wkbBuffer , 0 , _wkbSize );

		// ������wkb������
		OGRErr err = poly->exportToWkb( wkbNDR , _wkbBuffer );
		if ( err != OGRERR_NONE )
		{
			clearRecoverableData();
			return false;
		}

		//------------------------------------------
		// ��ʼ��ȡwkb����
		//------------------------------------------
		bool readwkbok = readWkbData( _wkbBuffer , _wkbSize , _polygonRings );
		if ( readwkbok == false )
		{
			clearRecoverableData();
			return false;
		}

		//------------------------------------------
		// ת������
		//------------------------------------------
		_isOpenGLCoordinate = false;
		converToCoordinate( _isOpenGLCoordinate , 
			_wkbBuffer , _polygonRings , true );

		//------------------------------------------
		// ��ʼ����box3d��Ϣ
		// �Լ���exterior����
		//------------------------------------------
		std::vector<RingStructVec>::iterator iter = _polygonRings.begin();
		std::vector<RingStructVec>::iterator iter_end = _polygonRings.end();

		for ( ; iter != iter_end ; ++iter )
		{
			RingStructVec& _rings = *iter;

			if ( _rings.empty() == false )
			{
				for(int j = 0; j < _rings[0]._numPoints; j++)
				{
					GLdouble* addr = _rings[0]._pointsBeginAddr + 3 * j;
					_box3d.merge( Vec3d( addr) );
				}
			}
		}

		//------------------------------------------
		// ���featureд��, �����Ժ�д��.
		//------------------------------------------
		setFeature( feature );


		//------------------------------------------
		// ��ʼ��ȡstyle
		//------------------------------------------
		getDrawingDescriptor()->importFromOGRFeature( feature );

		return true;
	}
	//----------------------------------------------------------------
	bool MultiPolygon::readWkbData(uchar* buf , int buf_size , 
		std::vector<RingStructVec>& out_polyrings )
	{
		out_polyrings.clear();

		/**
			WKBMultiPolygon {
				byte              byteOrder;
				uint32            wkbType;     // 6=wkbMultiPolygon
				uint32            num_wkbPolygons;
				WKBPolygon        wkbPolygons[num_wkbPolygons];
			};

			WKBPolygon    {
			byte                byteOrder;
			uint32            wkbType;     // 3=wkbPolygon
			uint32            numRings;
			LinearRing        rings[numRings];
		};
		*/

		assert( buf != NULL );
		uchar* pbuf = buf;

		// ���byteorder
		byte byteorder = *pbuf;
		assert( byteorder == wkbNDR );
		pbuf += sizeof( byte );

		// ���type
		int geotype = *(int*)pbuf;
		assert( geotype == wkbMultiPolygon25D );
		pbuf += sizeof( int );

		// num_wkbPolygons
		int num_wkbPolygons = *(int*)pbuf;
		pbuf += sizeof( int );

		for ( int i = 0 ; i < num_wkbPolygons ;++ i )
		{
			RingStructVec rings;

			long readnum = readSinglePolygonData( pbuf , rings );

			pbuf += readnum;

			out_polyrings.push_back( rings );
		}

		assert( (pbuf - buf) == buf_size );

		return true;
	}
	//----------------------------------------------------------------
	void MultiPolygon::renderOnceForDisplayList()
	{
		DrawingDescriptor* desc = getDrawingDescriptor();

		double penwidth = desc->getPenWidth();

		if ( penwidth >= vgKernel::Math::ZERO_TOLERANCE )
		{
			VGK_CHECK_OPENGL_ERROR();

			glDisable( GL_TEXTURE_2D );

			vgKernel::ColorVal col = desc->getPenColorVal();
			col.useBind();

			//// Green polygon
			//glColor3f(0.5f, 1.0f, 0.0f);

			glLineWidth( penwidth );
			glPushMatrix();

			std::vector<RingStructVec>::iterator iter = _polygonRings.begin();
			std::vector<RingStructVec>::iterator iter_end = _polygonRings.end();

			for ( ; iter != iter_end ; ++iter )
			{
				RingStructVec& _rings = *iter;

				assert( _rings.empty() == false );

				for ( int i = 0 ; i < _rings.size() ; ++ i )
				{
					glBegin( GL_LINE_STRIP );
					for ( int j = 0 ; j < _rings[i]._numPoints ;  ++ j )
					{
						double* addr =  _rings[i]._pointsBeginAddr + 3 * j;

						glVertex3dv( addr );
					}
					glEnd();
				}
			}

			glPopMatrix();
		}


		if ( desc->isDrawFilling() )
		{
			VGK_CHECK_OPENGL_ERROR();

			//glColor3f(0.3f, 0.3f, 0.3f); 
			//float randfloat = rand() *1.0 / RAND_MAX ;
			//glColor3f( randfloat, 0.3, 0.3f);
			vgKernel::ColorVal col = desc->getFillingColorVal();
			col.useBind();

			VGK_CHECK_OPENGL_ERROR();
			glDisable( GL_TEXTURE_2D );
			//glPushAttrib(GL_CURRENT_BIT);

			glPushMatrix();
			{
				std::vector<RingStructVec>::iterator iter = _polygonRings.begin();
				std::vector<RingStructVec>::iterator iter_end = _polygonRings.end();

				for ( ; iter != iter_end ; ++iter )
				{
					renderRings( *iter );
				}
			}

			VGK_CHECK_OPENGL_ERROR();

			glPopMatrix();
			//glPopAttrib();
		}

		return;
	}
	//----------------------------------------------------------------
	void MultiPolygon::clearRecoverableData()
	{
		if ( _wkbBuffer == NULL )
		{
			delete _wkbBuffer;
			_wkbBuffer = NULL;
		}
		_wkbSize = -1;
		_polygonRings.clear();
		_isOpenGLCoordinate = false;

		clearDisplayListID();
	}
	//----------------------------------------------------------------
	int MultiPolygon::getNumOfPoints()
	{
		int ret = 0;

		std::vector<RingStructVec>::iterator iter = _polygonRings.begin();
		std::vector<RingStructVec>::iterator iter_end = _polygonRings.end();

		for ( ; iter != iter_end ; ++iter )
		{
			RingStructVec& _rings = *iter;

			assert( _rings.empty() == false );

			for ( int i = 0 ; i < _rings.size() ; ++ i )
			{
				ret += _rings[i]._numPoints;
			}
		}

		return ret;
	}
	//----------------------------------------------------------------
	void MultiPolygon::renderRings( RingStructVec& ringstruct )
	{
		RingStructVec& _rings = ringstruct;

		GLUtesselator *pTess = NULL;
		pTess = gluNewTess();

		gluTessCallback(pTess, GLU_BEGIN, (void(__stdcall*)())glBegin);
		gluTessCallback(pTess, GLU_END, (void(__stdcall*)())glEnd);
		gluTessCallback(pTess, GLU_VERTEX, (void(__stdcall*)())glVertex3dv);
		gluTessProperty(pTess, GLU_TESS_WINDING_RULE, GLU_TESS_WINDING_ODD);

		gluTessBeginPolygon(pTess, NULL); // No user data

		for ( int i = 0 ; i < _rings.size() ; ++ i )
		{
			if ( i == 0 )
			{
				double* addr;
				// ��Ⱦexterior rings
				gluTessBeginContour(pTess);
				for(int j = 0; j < _rings[i]._numPoints; j++)
				{
					addr = _rings[i]._pointsBeginAddr + 3 * j;

					gluTessVertex( pTess, addr,  addr );
				}
				gluTessEndContour(pTess);
			}
			else
			{
				double* addr;
				// ��Ⱦexterior rings
				gluTessBeginContour(pTess);
				for(int j = 0; j < _rings[i]._numPoints; j++)
				{
					addr = _rings[i]._pointsBeginAddr + 3 * j;

					gluTessVertex( pTess, addr,  addr );
				}
				gluTessEndContour(pTess);
			}
		}

		// All done with polygon
		gluTessEndPolygon(pTess);

		// No longer need tessellator object
		gluDeleteTess(pTess);

		return;
	}
	//----------------------------------------------------------------
	long MultiPolygon::readSinglePolygonData(  uchar* buf , 
		RingStructVec& out_rings  )
	{
		uchar* pbuf = buf;

		// ���byteorder
		byte byteorder = *pbuf;
		assert( byteorder == wkbNDR );
		pbuf += sizeof( byte );

		// ���type
		int geotype = *(int*)pbuf;
		assert( geotype == wkbPolygon25D );
		pbuf += sizeof( int );

		// numRings
		int numRings = *(int*)pbuf;
		pbuf += sizeof( int );

		//------------------------------------------
		// ��ʼ��ȡLinearRing   
		//------------------------------------------
		/**
		LinearRing   {
		uint32  numPoints;
		Point   points[numPoints];
		};
		*/

		assert( numRings >= 0);
		if ( numRings <= 0 )
		{
			assert(0);
			return -1;
		}

		for ( int i = 0 ; i < numRings ; ++ i )
		{
			int numPoints = *(int*)pbuf;
			assert( numPoints >= 0 );
			pbuf += sizeof( int );

			out_rings.push_back( RingStruct( numPoints , (double*)pbuf ) );

			// ������Щ��.
			pbuf += sizeof( double ) * 3 * numPoints;
		}

		return pbuf - buf;
	}
	//----------------------------------------------------------------
	bool MultiPolygon::testIntersect(  const RayLine3d& rayline )
	{
		assert( OGRGeometryFactory::haveGEOS() == TRUE );

		//------------------------------------------
		// �����ж��Ƿ��envelope�ཻ.
		//------------------------------------------
		if( _box3d.testInside2D( rayline._pointEnd ) == false )
		{
			return false;
		}

		//------------------------------------------
		// ��ʼ����ÿ��������ж�
		//------------------------------------------
		OGRGeometry* geo = 
			OGRGeometryFactory::createGeometry( wkbPoint );

		assert( geo != NULL );
		assert( wkbFlatten( geo->getGeometryType() )== wkbPoint );

		geo->setCoordinateDimension(3);

		OGRPoint* pt = ( OGRPoint * )geo;

		pt->setX( rayline._pointEnd.x );
		pt->setY( -rayline._pointEnd.z ); // ����

		pt->flattenTo2D();

		assert( _wkbBuffer != NULL );
		// ����ת��opengl���굽shape����
		uchar* shp_buf = getShapeCoordinateBuf();
		assert( shp_buf != NULL );

		OGRMultiPolygon poly;
		OGRErr err = poly.importFromWkb( shp_buf , _wkbSize );
		assert( err == OGRERR_NONE );

		// ɾ��������
		delete[] shp_buf;
		shp_buf = NULL;

		// ��Ϊ2D
		poly.flattenTo2D();

		OGRBoolean bo = poly.Contains( pt );

		//OGRBoolean bo = poly.Intersect( ls );
		OGRGeometryFactory::destroyGeometry( geo );
		if ( bo == TRUE )
		{
			return true;
		}
		return false;
	}
	//----------------------------------------------------------------
	void MultiPolygon::renderInSelectedMode()
	{
		// red
		glColor3f( 1, 0, 0 );
		glDisable( GL_TEXTURE_2D );
		glPointSize( 5.0f );
		glPushMatrix();

		glDisable( GL_DEPTH_TEST );

		// ѡ��ģʽ�»���
		/*glBegin( GL_POINTS );
		std::vector<RingStructVec>::iterator iter = _polygonRings.begin();
		std::vector<RingStructVec>::iterator iter_end = _polygonRings.end();

		for ( ; iter != iter_end ; ++iter )
		{
			RingStructVec& _rings = *iter;

			assert( _rings.empty() == false );

			for ( int i = 0 ; i < _rings.size() ; ++ i )
			{
				for ( int j = 0 ; j < _rings[i]._numPoints ;  ++ j )
				{
					double* addr =  _rings[i]._pointsBeginAddr + 3 * j;

					glVertex3dv( addr );
				}
			}
		}
		glEnd();*/

		// ѡ��ģʽ�»���
		glLineWidth( 3.0 );
		glBegin( GL_LINES );
			std::vector<RingStructVec>::iterator iter = _polygonRings.begin();
			std::vector<RingStructVec>::iterator iter_end = _polygonRings.end();

			for ( ; iter != iter_end ; ++iter )
			{
				RingStructVec& _rings = *iter;

				assert( _rings.empty() == false );

				for ( int i = 0 ; i < _rings.size() ; ++ i )
				{
					for ( int j = 0 ; j < _rings[i]._numPoints-1;  ++ j )
					{
						GLdouble* addrFirst =  _rings[i]._pointsBeginAddr + 3 * j;
						GLdouble* addrSecond = _rings[i]._pointsBeginAddr + 3 * ( j + 1 );

						glVertex3dv( addrFirst );
						glVertex3dv( addrSecond );

					}// end for j 

					GLdouble* addrEnd = _rings[i]._pointsBeginAddr + 3 * ( _rings[i]._numPoints - 1 );
					GLdouble* addrStart = _rings[i]._pointsBeginAddr;

					glVertex3dv( addrEnd );
					glVertex3dv( addrStart );

				}// end for i

			}// end for iter
		glEnd();

		glEnable( GL_DEPTH_TEST );

		glPopMatrix();
	}
	//----------------------------------------------------------------
	void MultiPolygon::converToCoordinate( bool& isopenglCoord ,
		uchar* buf , std::vector<RingStructVec>& polyrings,
		const bool& convert_to_opengl )
	{
		// ת����opengl����ʱ.
		if ( convert_to_opengl )
		{
			if ( isopenglCoord )
			{
				return;
			}

			if ( buf == NULL )
			{
				assert(0);
				return;
			}

			std::vector<RingStructVec>::iterator iter = polyrings.begin();
			std::vector<RingStructVec>::iterator iter_end = polyrings.end();

			for ( ; iter != iter_end ; ++iter )
			{
				RingStructVec& _rings = *iter;

				assert( _rings.empty() == false );

				for ( int i = 0 ; i < _rings.size() ; ++ i )
				{
					for ( int j = 0 ; j < _rings[i]._numPoints ;  ++ j )
					{
						double* addr =  _rings[i]._pointsBeginAddr + 3 * j;

						double tmp = *( addr + 1 ); 
						*( addr + 1 ) = *( addr + 2 ); // ����߶�Y
						*( addr + 2 ) = -tmp; // z����ȡ��
					}
				}
			}
			isopenglCoord = true;

			return;
		}
		else
		{
			//------------------------------------------
			// ת����shape����
			//------------------------------------------
			if ( isopenglCoord == false )
			{
				return;
			}

			if ( buf == NULL )
			{
				assert(0);
				return;
			}

			std::vector<RingStructVec>::iterator iter = polyrings.begin();
			std::vector<RingStructVec>::iterator iter_end = polyrings.end();

			for ( ; iter != iter_end ; ++iter )
			{
				RingStructVec& _rings = *iter;

				assert( _rings.empty() == false );

				for ( int i = 0 ; i < _rings.size() ; ++ i )
				{
					for ( int j = 0 ; j < _rings[i]._numPoints ;  ++ j )
					{
						double* addr =  _rings[i]._pointsBeginAddr + 3 * j;

						double tmp = *( addr + 1 ); 
						*( addr + 1 ) = -*( addr + 2 ); // z����ȡ��
						*( addr + 2 ) = tmp; // ����߶�Y
					}
				}
			}

			isopenglCoord = false;
		}

		return;
	}
	//----------------------------------------------------------------
	uchar* MultiPolygon::getShapeCoordinateBuf()
	{
		if ( _wkbBuffer == NULL )
		{
			assert(0);
			return NULL;
		}

		//------------------------------------------
		// ��ʱΪshape����,ֱ�Ӹ��Ʒ��ؼ���.
		//------------------------------------------
		uchar* newbuf = NULL;
		newbuf = new uchar[ _wkbSize ];

		memset( newbuf , 0 , _wkbSize );
		memcpy( newbuf  , _wkbBuffer , _wkbSize );

		if( _isOpenGLCoordinate )
		{
			std::vector<RingStructVec> polyring;
			readWkbData( newbuf , _wkbSize , polyring );

			bool isopengl = true;
			converToCoordinate( isopengl , newbuf , polyring , false );
		}

		return newbuf;
	}
	//----------------------------------------------------------------
	bool MultiPolygon::attachToTerrain()
	{
		assert( _wkbBuffer != NULL );

		vgKernel::Vec3 vec3;

		std::vector<RingStructVec>::iterator iter = _polygonRings.begin();
		std::vector<RingStructVec>::iterator iter_end = _polygonRings.end();

		_box3d = Box3d::NULL_BOX;

		for ( ; iter != iter_end ; ++iter )
		{
			RingStructVec& _rings = *iter;

			assert( _rings.empty() == false );

			for ( int i = 0 ; i < _rings.size() ; ++ i )
			{
				for ( int j = 0 ; j < _rings[i]._numPoints ;  ++ j )
				{
					double* addr =  _rings[i]._pointsBeginAddr + 3 * j;

					vec3.x = *(addr);
					vec3.z = *(addr+2);
					//double z = *(addr+2);

					float height = getFatherLayer()->getHeightInOpenGL( vec3 );

					*(addr+1) = height;

					_box3d.merge(vec3);
				}
			}
		}

		//------------------------------------------
		// ��ʼ����wkb
		//------------------------------------------
		uchar* buf = new uchar[ _wkbSize ];
		memset( buf, 0 , _wkbSize );

		memcpy( buf, _wkbBuffer , _wkbSize );

		bool isopengl = _isOpenGLCoordinate;
		std::vector<RingStructVec> polyrings;

		readWkbData( buf , _wkbSize , polyrings );

		converToCoordinate( isopengl , buf , polyrings , false );

		OGRFeaturePtr pfeat = getOGRFeaturePtr();
		assert( pfeat.isNull() == false );


		OGRGeometry* geo = pfeat->GetGeometryRef();
		OGRErr err = geo->importFromWkb( buf );
		assert( err == OGRERR_NONE );

		//------------------------------------------
		// д��feature
		//------------------------------------------
		setFeature( pfeat.getPointer() );

		delete[] buf;
		buf = NULL;

		return true;
	}
	//----------------------------------------------------------------
	void MultiPolygon::invalidateRendering()
	{
		clearDisplayListID();
	}
	//----------------------------------------------------------------
	void MultiPolygon::clearDisplayListID()
	{
		if ( _callListId != 0 )
		{
			glDeleteLists(_callListId , 1 );
			_callListId = 0;
			VGK_CHECK_OPENGL_ERROR();
		}
	}
	//----------------------------------------------------------------


	
}// end of namespace vgShape

