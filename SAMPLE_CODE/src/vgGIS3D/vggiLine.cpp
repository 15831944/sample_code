


#include <vgStableHeaders.h>
#include <vgGis3d/vggiLine.h>
#include <vgGis3d/vggiLayer.h>
#include <gdal/ogrsf_frmts.h>
#include <gdal/ogr_api.h>
#include <vgKernel/vgkOpenGLSupport.h>
#include <vgKernel/vgkMath.h>



namespace vgGIS3D {

	//----------------------------------------------------------------
	LineString::LineString( OGRFeature* feat , Layer* father ) 
		: Geometry( feat , father )
	{
		_wkbSize = -1;
		_wkbBuffer = NULL;
		_isOpenGLCoordinate = false;
		_callListId = 0;
		_box3d = Box3d::NULL_BOX;
		_envelope = NULL;

		m_dis = 0.0;
	}
	//----------------------------------------------------------------

	//----------------------------------------------------------------
	void LineString::renderUsingDisplayList()
	{
		//assert(0);
		if ( _wkbBuffer == NULL )
		{
			return;
		}

		VGK_CHECK_OPENGL_ERROR();

		if ( _callListId == 0 )
		{
			_callListId = glGenLists( 1 );   
			assert( _callListId != 0 );

			glNewList( _callListId, GL_COMPILE );
			renderOnceForDisplayList();
			glEndList();	
		}
		VGK_CHECK_OPENGL_ERROR();

		assert( _callListId != 0 );

		glPushMatrix();
		glCallList( _callListId );
		glPopMatrix();

		return;
	}
	//----------------------------------------------------------------
	bool LineString::readDataFromOGRFeature( )
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

		if ( wkbFlatten( geo->getGeometryType() ) != wkbLineString )
		{
			assert(0);
			return false;
		}

		assert( wkbFlatten( geo->getGeometryType() )
			== wkbLineString );

		OGRLineString* line = ( OGRLineString* )geo;

		VGGI_CHECK_OGR_ERROR();
		//------------------------------------------
		// �趨Ϊ25Dģʽ
		//------------------------------------------
		int dimension = geo->getDimension();
		if ( dimension != 3 )
		{
			geo->setCoordinateDimension( 3 );
		}
		VGGI_CHECK_OGR_ERROR();

		assert( _wkbSize == -1 );

		_wkbSize = line->WkbSize();
		assert( _wkbSize > 0 );

		if ( _wkbSize <= 0 )
		{
			return false;
		}

		assert( _wkbBuffer == NULL );

		_wkbBuffer = new uchar[ _wkbSize ];
		memset( _wkbBuffer , 0 , _wkbSize );

		// ������wkb������
		OGRErr err = line->exportToWkb( wkbNDR , _wkbBuffer );
		if ( err != OGRERR_NONE )
		{
			clearRecoverableData();
			return false;
		}

		//------------------------------------------
		// ��ʼ��ȡwkb����
		//------------------------------------------
		bool readwkbok = readWkbData( _wkbBuffer , _wkbSize , _ring );
		if ( readwkbok == false )
		{
			clearRecoverableData();
			return false;
		}

		//------------------------------------------
		// ת������
		//------------------------------------------
		_isOpenGLCoordinate = false;

		converToCoordinate( _isOpenGLCoordinate , _ring , true );

		//------------------------------------------
		// ��ʼ����box3d��Ϣ
		//------------------------------------------
		for(int j = 0; j < _ring._numPoints; j++)
		{
			GLdouble* addr = _ring._pointsBeginAddr + 3 * j;
			_box3d.merge( Vec3d( addr) );
		}

		VGGI_CHECK_OGR_ERROR();

		//------------------------------------------
		// ���featureд��, �����Ժ�д��.
		//------------------------------------------
		setFeature( feature );

		VGGI_CHECK_OGR_ERROR();


		//------------------------------------------
		// ��ʼ��ȡstyle
		//------------------------------------------
		getDrawingDescriptor()->importFromOGRFeature( feature );

		return true;
	}
	//----------------------------------------------------------------
	bool LineString::readWkbData(  uchar* buf , int buf_size , 
		RingStruct& out_ring )
	{
		assert( buf != NULL );
		if ( buf == NULL )
		{
			return false;
		}


		/**
		WKBLineString {
		byte     byteOrder;
		uint32   wkbType;     // 2=wkbLineString
		uint32   numPoints;
		Point    points[numPoints];
		};
		*/

		uchar* pbuf = buf;

		// ���byteorder
		byte byteorder = *pbuf;
		assert( byteorder == wkbNDR );
		pbuf += sizeof( byte );

		// ���type
		int geotype = *(int*)pbuf;
		assert( geotype == wkbLineString25D );
		pbuf += sizeof( int );

		// numPoints
		int numPoints = *(int*)pbuf;
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

		assert( numPoints >= 0);
		if ( numPoints <= 0 )
		{
			return false;
		}

		out_ring._numPoints = numPoints;
		out_ring._pointsBeginAddr = (double*)pbuf;

		pbuf += sizeof( double ) * 3 * numPoints;
		
		assert( (pbuf - buf) == buf_size );

		return true;
	}
	//----------------------------------------------------------------
	void LineString::renderOnceForDisplayList()
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

			glBegin( GL_LINE_STRIP );
			for(int j = 0; j < _ring._numPoints; j++)
			{
				GLdouble* addr = _ring._pointsBeginAddr + 3 * j;

#if 1
				glVertex3dv( addr );
#else
				Vec3d	tempVec(addr);
				tempVec /= 10000.0f;
				glVertex3dv(  (double*)&tempVec);
#endif
			}
			glEnd();


			glPopMatrix();
			VGK_CHECK_OPENGL_ERROR();
		}



		return;
	}

	//----------------------------------------------------------------
	void LineString::converToCoordinate( bool& isopenglCoord , 
		RingStruct& ring, const bool& convert_to_opengl)
	{
		// ת����opengl����ʱ.
		if ( convert_to_opengl )
		{
			if ( isopenglCoord )
			{
				return;
			}


			for ( int j = 0 ; j < ring._numPoints ;  ++ j )
			{
				double* addr =  ring._pointsBeginAddr + 3 * j;

				double tmp = *( addr + 1 ); 
				*( addr + 1 ) = *( addr + 2 ); // ����߶�Y
				*( addr + 2 ) = -tmp; // z����ȡ��
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


			for ( int j = 0 ; j < ring._numPoints ;  ++ j )
			{
				double* addr =  ring._pointsBeginAddr + 3 * j;

				double tmp = *( addr + 1 ); 
				*( addr + 1 ) = -*( addr + 2 ); // z����ȡ��
				*( addr + 2 ) = tmp; // ����߶�Y
			}

			isopenglCoord = false;
		}

		return;
	}
	//----------------------------------------------------------------
	void LineString::clearRecoverableData()
	{
		if ( _wkbBuffer != NULL )
		{
			delete _wkbBuffer;
			_wkbBuffer = NULL;
		}
		_wkbSize = -1;
		_isOpenGLCoordinate = false;

		clearDisplayListID();
	}
	//----------------------------------------------------------------
	int LineString::getNumOfPoints()
	{
		return _ring._numPoints;
	}
	//----------------------------------------------------------------
	bool LineString::testIntersect(  const RayLine3d& rayline  )
	{
		assert( OGRGeometryFactory::haveGEOS() == TRUE );

		//------------------------------------------
		// �����ж��Ƿ��envelope�ཻ.
		//------------------------------------------
		vgKernel::Vec3 boxMin;
		vgKernel::Vec3 boxMax;

		boxMax.x = _box3d.getMaximum().x + m_dis;
		boxMax.y = _box3d.getMaximum().y + m_dis;
		boxMax.z = _box3d.getMaximum().z + m_dis;

		boxMin.x = _box3d.getMinimum().x - m_dis;
		boxMin.y = _box3d.getMinimum().y - m_dis;
		boxMin.z = _box3d.getMinimum().z - m_dis;

		//TRACE("boxMax = %f, %f, %f \n", boxMax.x, boxMax.y, boxMax.z );
		//TRACE("boxMin = %f, %f, %f \n", boxMin.x, boxMin.y, boxMin.z );
		//TRACE("MousPt = %f, %f, %f \n", rayline._pointEnd.x, rayline._pointEnd.y, rayline._pointEnd.z );
		//TRACE("CamePt = %f, %f, %f \n", rayline._pointStart.x, rayline._pointStart.y, rayline._pointStart.z );
		vgKernel::Box new_box = vgKernel::Box::NULL_BOX;
		new_box.setMaximum( boxMax );
		new_box.setMinimum( boxMin );

		std::pair<bool, float> result = vgKernel::Math::intersects( 
			rayline.getRay3d().getRayFloat(), new_box );

		if (result.first != false)
		{
			return true;
		}
		else
		{
			return false;
		}
 		//if ( new_box.testInside2D( rayline._pointEnd ) == false )
 		//{
 		//	return false;
 		//}

 		//if( m_.testInside2D( rayline._pointEnd ) == false )
 		//{
 		//	return false;
 		//}
#if 0

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

		if (_envelope==NULL)
		{
			_envelope = Buffer(1,5);
		}

		OGRBoolean isintersect = _envelope->Intersects(pt);

		return isintersect;
#endif
		//----------------------------------------------
		//  Edit By FengYK @2010-01-27 15:59
		//  ʹ��"�������ߺ�ÿ���߶εİ�Χ���ཻ"�ķ����жϵ�ѡ
		//----------------------------------------------
#if 0
		//--------------------------------------
		//   Edit By FengYK @2010-01-25 13:59
		//--------------------------------------

		vgKernel::Vec3 lineStartPt;
		vgKernel::Vec3 lineEndPt;

		lineStartPt.x = rayline._pointStart.x;
		lineStartPt.y = rayline._pointStart.y;
		lineStartPt.z = rayline._pointStart.z;

		lineEndPt.x = rayline._pointEnd.x;
		lineEndPt.y = rayline._pointEnd.y;
		lineEndPt.z = rayline._pointEnd.z;

		vgKernel::Ray lookray = vgKernel::Ray( lineStartPt, lineEndPt );

		std::vector<aRound_Point>::iterator iter = aroundingPoints.begin();
		std::vector<aRound_Point>::iterator iter_end = aroundingPoints.end();
		
		for ( ; iter != iter_end ; ++ iter )
		{
			aRound_Point aPoint = *iter;

			//tempPt1 = aPoint.arPoint1;
			//tempPt2 = aPoint.arPoint2;
			//tempPt3 = aPoint.arPoint3;
			//tempPt4 = aPoint.arPoint4;

			// �����Ƿ񴩹�������
			if ( tri_Interect(lookray, aPoint.arPoint1, aPoint.arPoint2, aPoint.arPoint3) )
			{
				return true;
			}

			if ( tri_Interect(lookray, aPoint.arPoint2, aPoint.arPoint3, aPoint.arPoint4) )
			{
				return true;
			}

		}

		return false;
#endif
		

#if 0
		assert( _wkbBuffer != NULL );
		// ����ת��opengl���굽shape����
		uchar* shp_buf = getShapeCoordinateBuf();
		assert( shp_buf != NULL );

		OGRLineString	line;
		OGRErr err = line.importFromWkb( shp_buf , _wkbSize );
		assert( err == OGRERR_NONE );


		// ɾ��������
		delete[] shp_buf;
		shp_buf = NULL;

		OGRLinearRing* pring = new OGRLinearRing;
		
		int	nPointCount = line.getNumPoints();
		OGRRawPoint * paoPointsOut = new OGRRawPoint[nPointCount];
		double * padfZ = new double[nPointCount];
		
		line.getPoints(paoPointsOut, padfZ);
		pring->setPoints( nPointCount, paoPointsOut, padfZ);
		pring->closeRings();
		if(pring->isClockwise())
			pring->reverseWindingOrder();

		OGRBoolean bo = pring->isPointInRing(pt);
		//OGRPolygon	poly;
		//poly.addRingDirectly(pring);
		
		// ��Ϊ2D
		//poly.flattenTo2D();
		//pline->flattenTo2D();

		//OGRBoolean bo = poly.Contains( pt );

		//OGRBoolean bo = poly.Intersect( ls );
		OGRGeometryFactory::destroyGeometry( geo );
		if ( bo == TRUE )
		{
			return true;
		}
		return false;

#endif
	}
	//----------------------------------------------------------------
	void LineString::renderInSelectedMode()
	{
		// red
		glColor3f( 1, 0, 0 );
		glDisable( GL_TEXTURE_2D );
		glPointSize( 5.0f );
		glPushMatrix();

		glDisable( GL_DEPTH_TEST );

#if 0
		// ѡ��ģʽ�»���
		glBegin( GL_POINTS );

		for(int j = 0; j < _ring._numPoints; j++)
		{
			GLdouble* addr = _ring._pointsBeginAddr + 3 * j;

			//double x = *(addr);
			//double y = *(addr+1);
			//double z = *(addr+2);

			glVertex3dv( addr );
		}

		glEnd();
#endif

		// ѡ��ģʽ�»���
		glBegin( GL_LINE_STRIP );

		for(int j = 0; j < _ring._numPoints; j++)
		{
			GLdouble* addr = _ring._pointsBeginAddr + 3 * j;

			//double x = *(addr);
			//double y = *(addr+1);
			//double z = *(addr+2);

			glVertex3dv( addr );
		}

		glEnd();

		//glBegin( GL_TRIANGLES );

		//glVertex3f( tempPt1.x, tempPt1.y, tempPt1.z );
		//glVertex3f( tempPt2.x, tempPt2.y, tempPt2.z );
		//glVertex3f( tempPt3.x, tempPt3.y, tempPt3.z );

		//glVertex3f( tempPt2.x, tempPt2.y, tempPt2.z );
		//glVertex3f( tempPt3.x, tempPt3.y, tempPt3.z );
		//glVertex3f( tempPt4.x, tempPt4.y, tempPt4.z );

		//glEnd();

		glEnable( GL_DEPTH_TEST );

		glPopMatrix();
	}
	//----------------------------------------------------------------
	uchar* LineString::getShapeCoordinateBuf() 
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
			RingStruct ring;

			readWkbData( newbuf , _wkbSize , ring );

			bool isopengl = true;
			converToCoordinate( isopengl , ring , false );
		}

		return newbuf;
	}
	//----------------------------------------------------------------
	bool LineString::attachToTerrain()
	{
		assert( _wkbBuffer != NULL );

		vgKernel::Vec3 vec3;
		_box3d = Box3d::NULL_BOX;

		for(int j = 0; j < _ring._numPoints; j++)
		{
			GLdouble* addr = _ring._pointsBeginAddr + 3 * j;

			vec3.x = *(addr);
			vec3.z = *(addr+2);
			//double z = *(addr+2);

			float height = getFatherLayer()->getHeightInOpenGL( vec3 );

			*(addr+1) = height;

			_box3d.merge(vec3);
		}

#if 1	
		//------------------------------------------
		// ��ʼ����wkb
		//------------------------------------------
		uchar* buf = new uchar[ _wkbSize ]; 
		memset( buf, 0 , _wkbSize );

		memcpy( buf, _wkbBuffer , _wkbSize );

		bool isopengl = _isOpenGLCoordinate;
		RingStruct ringTemp;

		readWkbData( buf , _wkbSize , ringTemp );

		converToCoordinate( isopengl , ringTemp , false );

		OGRFeaturePtr pfeat = getOGRFeaturePtr();
		assert( pfeat.isNull() == false );


		//OGRGeometry* geo = pfeat->GetGeometryRef();

		OGRGeometry* newgeo =
			OGRGeometryFactory::createGeometry( wkbLineString25D );

		assert( newgeo != NULL );

		OGRErr err = newgeo->importFromWkb( buf );
		assert( err == OGRERR_NONE );

		err = pfeat->SetGeometry( newgeo );
		assert( err == OGRERR_NONE );

		//------------------------------------------
		// д��feature
		//------------------------------------------
		setFeature( pfeat.getPointer() );

		delete[] buf;
		buf = NULL;

		delete newgeo;
#endif

		return true;
	}
	//----------------------------------------------------------------
	void LineString::invalidateRendering()
	{
		clearDisplayListID();

		//----------------------------------------------
		// Edit By FengYK @2010-01-27 15:59
		// ʹ��"�������ߺ�ÿ���߶εİ�Χ���ཻ"�ķ����жϵ�ѡ
		//----------------------------------------------
		setIntersectDis();
#if 0
		RingStruct m_ring = _ring;
		Vec3d line_Pot1;
		Vec3d line_Pot2;

		if ( !aroundingPoints.empty() )
		{
			aroundingPoints.clear();
		}

		for (int j=0; j<m_ring._numPoints - 1; j++)
		{
			GLdouble* addr = m_ring._pointsBeginAddr + 3 * j;

			double startPot_x = *( addr);
			double startPot_y = *( addr + 1 );
			double startPot_z = *( addr + 2 );

			double endPot_x = *( addr + 3 );
			double endPot_y = *( addr + 4 );
			double endPot_z = *( addr + 5 );

			line_Pot1.x = startPot_x;
			line_Pot1.y = startPot_y;
			line_Pot1.z = startPot_z;

			line_Pot2.x = endPot_x;
			line_Pot2.y = endPot_y;
			line_Pot2.z = endPot_z;

			// ����������ɵ�����Y��ƽ��ʱ�����ܲ�ˣ��ų���Y��ƽ�е����
			if ( (line_Pot1.x == line_Pot2.x) && (line_Pot1.z == line_Pot2.z) )
			{
				return;
			}

			// ���������ɵ�����
			Vec3d line_Vec;
			line_Vec.x = line_Pot1.x - line_Pot2.x;
			line_Vec.y = line_Pot1.y - line_Pot2.y;
			line_Vec.z = line_Pot1.z - line_Pot2.z;

			Vec3d AxisY;	// Y�ᵥλ����
			AxisY.x = 0.0;
			AxisY.y = 1.0;
			AxisY.z = 0.0;

			// ���,��������ɵ�������Y��ĵ�λ����(0, 1, 0) ��˺�Ľ��
			Vec3d crossedProduct = line_Vec.crossProduct( AxisY );

			// ��λ��
			Vec3d unitization = crossedProduct.normalisedRef();

			// ���Ծ���d ,�õ��µ�ʸ��
			Vec3d multiply_d;
			multiply_d.x = unitization.x * m_dis;
			multiply_d.y = unitization.y * m_dis;
			multiply_d.z = unitization.z * m_dis;

			// ��1
			vgKernel::Vec3 aRound_Pot1;
			aRound_Pot1.x = multiply_d.x + line_Pot1.x;
			aRound_Pot1.y = multiply_d.y + line_Pot1.y;
			aRound_Pot1.z = multiply_d.z + line_Pot1.z;

			// ��2
			vgKernel::Vec3 aRound_Pot2;
			aRound_Pot2.x = -multiply_d.x + line_Pot1.x;
			aRound_Pot2.y = -multiply_d.y + line_Pot1.y;
			aRound_Pot2.z = -multiply_d.z + line_Pot1.z;

			// ��3
			vgKernel::Vec3 aRound_Pot3;
			aRound_Pot3.x = multiply_d.x + line_Pot2.x;
			aRound_Pot3.y = multiply_d.y + line_Pot2.y;
			aRound_Pot3.z = multiply_d.z + line_Pot2.z;

			// ��4
			vgKernel::Vec3 aRound_Pot4;
			aRound_Pot4.x = -multiply_d.x + line_Pot2.x;
			aRound_Pot4.y = -multiply_d.y + line_Pot2.y;
			aRound_Pot4.z = -multiply_d.z + line_Pot2.z;

			aRound_Point apoints;
			apoints.arPoint1 = aRound_Pot1;
			apoints.arPoint2 = aRound_Pot2;
			apoints.arPoint3 = aRound_Pot3;
			apoints.arPoint4 = aRound_Pot4;
			
			aroundingPoints.push_back(apoints);

		}
#endif
		
	}
	//----------------------------------------------------------------
	void LineString::clearDisplayListID()
	{
		if ( _callListId != 0 )
		{
			VGK_CHECK_OPENGL_ERROR();

			glDeleteLists( _callListId , 1 );
			_callListId = 0;

			VGK_CHECK_OPENGL_ERROR();
		}
	}
	//----------------------------------------------------------------
	bool LineString::tri_Interect( const vgKernel::Ray& ray, vgKernel::Vec3 pt1, vgKernel::Vec3 pt2, vgKernel::Vec3 pt3 )
	{
		std::pair<bool, float> result = vgKernel::Math::intersects( ray , pt1 , pt2 , pt3 );
		if(result.first == true)
			return true;
		else
			return false;
	}
	//-----------------------------------------------------------------
	void LineString::setIntersectDis()
	{
		vgKernel::Box fatherLayerBoundBox = this->getFatherLayer()->getBoundingBox();
		Vec3d m_boxSize = fatherLayerBoundBox.getSize();

		if ( m_boxSize.x > m_boxSize.z )
		{
			m_dis = m_boxSize.z * 0.003;
		}
		else
		{
			m_dis = m_boxSize.x * 0.003;
		}

		if ( m_dis > 0.5 )
		{
			m_dis = 0.5;
		}
	}
	//-----------------------------------------------------------------

}// end of namespace vgShape
