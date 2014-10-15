



#include <vgStableHeaders.h>
#include <vgMesh/vgmShadowMetaFace.h>
#include <vgMesh/vgmMeshRenderWayVertexArray.h>
#include <vgMesh/vgmMeshDataPlus.h>


namespace vgMesh {
	
	

	
	
	//----------------------------------------------------------------
	ShadowMetaFace::ShadowMetaFace( )
	{
		_fatherDataPlus =NULL;
		_vaway = NULL;
		_metaFaceStruct = NULL;

		_shadowFaceData = NULL;
	}
	//----------------------------------------------------------------
	void ShadowMetaFace::setFromMetaFaceStruct( MeshRenderWayVertexArray* vaway , 
		MetaFaceStruct* metaface  ,MeshDataPlus* dataplus )
	{
		assert( dataplus != NULL);
		_fatherDataPlus = dataplus;
		assert( _vaway == NULL );

		//assert( vaway->_sizeOfVertexElements == sizeof( VgmVertexElement1 ) );

		if ( vaway->_sizeOfVertexElements == sizeof( VgmVertexElement1 ) )
		{
			VgmVertexElement1* elements = 
				(VgmVertexElement1*)vaway->_vertexElements;

			_vaway = vaway;
			assert( metaface != NULL );
			_metaFaceStruct = metaface;
			assert( _metaFaceStruct != NULL );

			_shadowFaceData = 
				new ShadowMetaFaceData[_metaFaceStruct->_numberOfFaceIndex];

			//vgKernel::Vec3 v1;
			//vgKernel::Vec3 v2;
			//------------------------------------------
			// ����normal
			//------------------------------------------
			for ( int i = 0 ; i < _metaFaceStruct->_numberOfFaceIndex ; ++ i )
			{
				_shadowFaceData[i]._fatherShadowFaceStruct = this;
				_shadowFaceData[i]._faceindex = 	&_metaFaceStruct->_faceIndex[i];

				vgKernel::Vec3* p1v = (vgKernel::Vec3*)
					&( elements[ _shadowFaceData[i]._faceindex->_point0 ].positionX );

				vgKernel::Vec3* p2v = (vgKernel::Vec3*)
					&( elements[ _shadowFaceData[i]._faceindex->_point1 ].positionX );

				vgKernel::Vec3* p3v = (vgKernel::Vec3*)
					&( elements[ _shadowFaceData[i]._faceindex->_point2 ].positionX );

				//v1 = ( *p1v - *p2v );
				//v2 = *p2v - *p3v ;

				//_shadowFaceData[i]._normal = v1.crossProduct( v2 );
				_shadowFaceData[i]._normal = ( *p1v - *p2v ).crossProduct( *p2v - *p3v  );
			}
		}
		else 
		{
			assert( vaway->_sizeOfVertexElements == sizeof( VgmVertexElement2 ));

			VgmVertexElement2* elements = 
				(VgmVertexElement2*)vaway->_vertexElements;

			_vaway = vaway;
			assert( metaface != NULL );
			_metaFaceStruct = metaface;
			assert( _metaFaceStruct != NULL );

			_shadowFaceData = 
				new ShadowMetaFaceData[_metaFaceStruct->_numberOfFaceIndex];

			//vgKernel::Vec3 v1;
			//vgKernel::Vec3 v2;
			//------------------------------------------
			// ����normal
			//------------------------------------------
			for ( int i = 0 ; i < _metaFaceStruct->_numberOfFaceIndex ; ++ i )
			{
				_shadowFaceData[i]._fatherShadowFaceStruct = this;
				_shadowFaceData[i]._faceindex = 	&_metaFaceStruct->_faceIndex[i];

				vgKernel::Vec3* p1v = (vgKernel::Vec3*)
					&( elements[ _shadowFaceData[i]._faceindex->_point0 ].positionX );

				vgKernel::Vec3* p2v = (vgKernel::Vec3*)
					&( elements[ _shadowFaceData[i]._faceindex->_point1 ].positionX );

				vgKernel::Vec3* p3v = (vgKernel::Vec3*)
					&( elements[ _shadowFaceData[i]._faceindex->_point2 ].positionX );

				//v1 = ( *p1v - *p2v );
				//v2 = *p2v - *p3v ;

				//_shadowFaceData[i]._normal = v1.crossProduct( v2 );
				_shadowFaceData[i]._normal = ( *p1v - *p2v ).crossProduct( *p2v - *p3v  );
			}

		}

	
		return;

	}
	//----------------------------------------------------------------
	void ShadowMetaFace::searchP1P2Adj( ShadowMetaFaceData* sw_face_data )
	{
		// ֻҪ�ж�������NULL�Ϳ��Ա�֤�Ѿ��������ˡ�
		if ( sw_face_data->p1p2AdjFace != NULL )
		{
			return;
		}

		assert( _vaway->_sizeOfVertexElements == sizeof( VgmVertexElement1 ) );

		VgmVertexElement1* elements = 
			(VgmVertexElement1*)_vaway->_vertexElements;

		const int& p1v =  sw_face_data->_faceindex->_point0;

		const int& p2v = sw_face_data->_faceindex->_point1;


		ShadowMetaFace& faces = *sw_face_data->_fatherShadowFaceStruct;

		for ( int i = 0 ; i < faces._metaFaceStruct->_numberOfFaceIndex ; ++ i )
		{
			ShadowMetaFaceData* sw_cur = &faces._shadowFaceData[ i ];

			if ( sw_cur == sw_face_data )
			{
				// ���ʱ���治��Ҫ�����ˡ�
				continue;
			}

			const int& p1jv = sw_cur->_faceindex->_point0;
			const int& p2jv = sw_cur->_faceindex->_point1;
			const int& p3jv = sw_cur->_faceindex->_point2;

			if ( 
				( sw_cur->p1p2AdjFace != NULL )
				&&
				( sw_cur->p2p3AdjFace != NULL )
				&&
				( sw_cur->p3p1AdjFace != NULL )
				)
			{
				// ���ڶ��Ѿ����ٱߵ�����˵����Ҫ�ٱȽ��ˡ�
				continue;
			}

			if ( ( sw_cur->p1p2AdjFace == NULL || sw_cur->p3p1AdjFace == NULL ) 
				&& p1v == p1jv )
			{
				if ( sw_cur->p1p2AdjFace == NULL && p2v == p2jv )
				{
					sw_face_data->p1p2AdjFace = sw_cur;
					sw_cur->p1p2AdjFace = sw_face_data;
					return;
				}
				else if ( sw_cur->p3p1AdjFace == NULL && p2v == p3jv )
				{
					sw_face_data->p1p2AdjFace = sw_cur;
					sw_cur->p3p1AdjFace = sw_face_data;
					return;
				}
			}
			else if ( ( sw_cur->p1p2AdjFace == NULL || sw_cur->p2p3AdjFace == NULL ) 
				&& p1v == p2jv )
			{
				if ( sw_cur->p1p2AdjFace == NULL && p2v == p1jv )
				{
					sw_face_data->p1p2AdjFace = sw_cur;
					sw_cur->p1p2AdjFace = sw_face_data;
					return;
				}
				else if ( sw_cur->p2p3AdjFace == NULL && p2v == p3jv )
				{
					sw_face_data->p1p2AdjFace = sw_cur;
					sw_cur->p2p3AdjFace = sw_face_data;
					return;
				}
			}
			else if (  ( sw_cur->p3p1AdjFace == NULL || sw_cur->p2p3AdjFace == NULL ) 
				&& p1v == p3jv )
			{
				if ( sw_cur->p3p1AdjFace == NULL && p2v == p1jv )
				{
					sw_face_data->p1p2AdjFace = sw_cur;
					sw_cur->p3p1AdjFace = sw_face_data;
					return;
				}
				else if ( sw_cur->p2p3AdjFace == NULL && p2v == p2jv )
				{
					sw_face_data->p1p2AdjFace = sw_cur;
					sw_cur->p2p3AdjFace = sw_face_data;
					return;
				}
			}

		}

		//////////////////////////////////////////////////////////////////////////


		//------------------------------------------
		// ��ʼ�������е���
		//------------------------------------------
		//for ( int metafacei = 0 ; metafacei < _vaway->_numberOfMetaFaces ; 
		//	++ metafacei )
		//{
		//	
		//}

		//------------------------------------------
		// �������ˣ���û��return
		// ��ʱ�������Ļ������û���ڽӱߡ�
		//------------------------------------------
		assert( sw_face_data->p1p2AdjFace == NULL );
		sw_face_data->p1p2AdjFace = sw_face_data;

		return;
	}
	//----------------------------------------------------------------
	void ShadowMetaFace::searchP2P3Adj( ShadowMetaFaceData* sw_face_data )
	{
		// ֻҪ�ж�������NULL�Ϳ��Ա�֤�Ѿ��������ˡ�
		if ( sw_face_data->p2p3AdjFace != NULL )
		{
			return;
		}

		assert( _vaway->_sizeOfVertexElements == sizeof( VgmVertexElement1 ) );

		VgmVertexElement1* elements = 
			(VgmVertexElement1*)_vaway->_vertexElements;

		const int& p2v = sw_face_data->_faceindex->_point1;
		const int& p3v = sw_face_data->_faceindex->_point2;

		ShadowMetaFace& faces = *sw_face_data->_fatherShadowFaceStruct;

		for ( int i = 0 ; i < faces._metaFaceStruct->_numberOfFaceIndex ; ++ i )
		{
			ShadowMetaFaceData* sw_cur = &faces._shadowFaceData[ i ];

			if ( sw_cur == sw_face_data )
			{
				// ���ʱ���治��Ҫ�����ˡ�
				continue;
			}

			const int& p1jv = sw_cur->_faceindex->_point0;
			const int& p2jv = sw_cur->_faceindex->_point1;
			const int& p3jv = sw_cur->_faceindex->_point2;


			if ( 
				( sw_cur->p1p2AdjFace != NULL )
				&&
				( sw_cur->p2p3AdjFace != NULL )
				&&
				( sw_cur->p3p1AdjFace != NULL )
				)
			{
				// ���ڶ��Ѿ����ٱߵ�����˵����Ҫ�ٱȽ��ˡ�
				continue;
			}

			if ( ( sw_cur->p1p2AdjFace == NULL || sw_cur->p3p1AdjFace == NULL ) 
				&& p2v == p1jv )
			{
				if ( sw_cur->p1p2AdjFace == NULL && p3v == p2jv )
				{
					sw_face_data->p2p3AdjFace = sw_cur;
					sw_cur->p1p2AdjFace = sw_face_data;
					return;
				}
				else if ( sw_cur->p3p1AdjFace == NULL && p3v == p3jv )
				{
					sw_face_data->p2p3AdjFace = sw_cur;
					sw_cur->p3p1AdjFace = sw_face_data;
					return;
				}
			}
			else if ( ( sw_cur->p1p2AdjFace == NULL || sw_cur->p2p3AdjFace == NULL ) 
				&& p2v == p2jv )
			{
				if ( sw_cur->p1p2AdjFace == NULL && p2v == p1jv )
				{
					sw_face_data->p2p3AdjFace = sw_cur;
					sw_cur->p1p2AdjFace = sw_face_data;
					return;
				}
				else if ( sw_cur->p2p3AdjFace == NULL && p2v == p3jv )
				{
					sw_face_data->p2p3AdjFace = sw_cur;
					sw_cur->p2p3AdjFace = sw_face_data;
					return;
				}
			}
			else if (  ( sw_cur->p3p1AdjFace == NULL || sw_cur->p2p3AdjFace == NULL ) 
				&& p2v == p3jv )
			{
				if ( sw_cur->p3p1AdjFace == NULL && p2v == p1jv )
				{
					sw_face_data->p2p3AdjFace = sw_cur;
					sw_cur->p3p1AdjFace = sw_face_data;
					return;
				}
				else if ( sw_cur->p2p3AdjFace == NULL && p2v == p2jv )
				{
					sw_face_data->p2p3AdjFace = sw_cur;
					sw_cur->p2p3AdjFace = sw_face_data;
					return;
				}
			}

		}


		//------------------------------------------
		// ��ʼ�������е���
		//------------------------------------------
		//for ( int metafacei = 0 ; metafacei < _vaway->_numberOfMetaFaces ; 
		//	++ metafacei )
		//{
		//	for ( int facej = 0 ; facej < _vaway->_metaFaces[ metafacei ]._numberOfFaceIndex ;
		//		++ facej )
		//	{
		//		ShadowMetaFaceData* sw_cur = 
		//			&_fatherDataPlus->_shadowMetaFaces[metafacei]._shadowFaceData[ facej ];

		//		if ( sw_cur == sw_face_data )
		//		{
		//			// ���ʱ���治��Ҫ�����ˡ�
		//			continue;
		//		}

		//		
		//	}
		//}

		//------------------------------------------
		// �������ˣ���û��return
		// ��ʱ�������Ļ������û���ڽӱߡ�
		//------------------------------------------
		assert( sw_face_data->p2p3AdjFace == NULL );
		sw_face_data->p2p3AdjFace = sw_face_data;

		return;

	}
	//----------------------------------------------------------------
	void ShadowMetaFace::searchP3P1Adj( ShadowMetaFaceData* sw_face_data )
	{
		// ֻҪ�ж�������NULL�Ϳ��Ա�֤�Ѿ��������ˡ�
		if ( sw_face_data->p3p1AdjFace != NULL )
		{
			return;
		}

		assert( _vaway->_sizeOfVertexElements == sizeof( VgmVertexElement1 ) );

		VgmVertexElement1* elements = 
			(VgmVertexElement1*)_vaway->_vertexElements;

		const int& p1v = sw_face_data->_faceindex->_point0;
		const int& p3v = sw_face_data->_faceindex->_point2;


		ShadowMetaFace& faces = *sw_face_data->_fatherShadowFaceStruct;

		for ( int i = 0 ; i < faces._metaFaceStruct->_numberOfFaceIndex ; ++ i )
		{
			ShadowMetaFaceData* sw_cur = &faces._shadowFaceData[ i ];

			if ( sw_cur == sw_face_data )
			{
				// ���ʱ���治��Ҫ�����ˡ�
				continue;
			}

			const int& p1jv = sw_cur->_faceindex->_point0;
			const int& p2jv = sw_cur->_faceindex->_point1;
			const int& p3jv = sw_cur->_faceindex->_point2;

			if ( 
				( sw_cur->p1p2AdjFace != NULL )
				&&
				( sw_cur->p2p3AdjFace != NULL )
				&&
				( sw_cur->p3p1AdjFace != NULL )
				)
			{
				// ���ڶ��Ѿ����ٱߵ�����˵����Ҫ�ٱȽ��ˡ�
				continue;
			}

			if ( ( sw_cur->p1p2AdjFace == NULL || sw_cur->p3p1AdjFace == NULL ) 
				&& p1v == p1jv )
			{
				if ( sw_cur->p1p2AdjFace == NULL && p3v == p2jv )
				{
					sw_face_data->p3p1AdjFace = sw_cur;
					sw_cur->p1p2AdjFace = sw_face_data;
					return;
				}
				else if ( sw_cur->p3p1AdjFace == NULL && p3v == p3jv )
				{
					sw_face_data->p3p1AdjFace = sw_cur;
					sw_cur->p3p1AdjFace = sw_face_data;
					return;
				}
			}
			else if ( ( sw_cur->p1p2AdjFace == NULL || sw_cur->p2p3AdjFace == NULL ) 
				&& p1v == p2jv )
			{
				if ( sw_cur->p1p2AdjFace == NULL && p3v == p1jv )
				{
					sw_face_data->p3p1AdjFace = sw_cur;
					sw_cur->p1p2AdjFace = sw_face_data;
					return;
				}
				else if ( sw_cur->p2p3AdjFace == NULL && p3v == p3jv )
				{
					sw_face_data->p3p1AdjFace = sw_cur;
					sw_cur->p2p3AdjFace = sw_face_data;
					return;
				}
			}
			else if (  ( sw_cur->p3p1AdjFace == NULL || sw_cur->p2p3AdjFace == NULL ) 
				&& p1v == p3jv )
			{
				if ( sw_cur->p3p1AdjFace == NULL && p3v == p1jv )
				{
					sw_face_data->p3p1AdjFace = sw_cur;
					sw_cur->p3p1AdjFace = sw_face_data;
					return;
				}
				else if ( sw_cur->p2p3AdjFace == NULL && p3v == p2jv )
				{
					sw_face_data->p3p1AdjFace = sw_cur;
					sw_cur->p2p3AdjFace = sw_face_data;
					return;
				}
			}

		}

		//------------------------------------------
		// ��ʼ�������е���
		//------------------------------------------
		//for ( int metafacei = 0 ; metafacei < _vaway->_numberOfMetaFaces ; 
		//	++ metafacei )
		//{
		//	for ( int facej = 0 ; facej < _vaway->_metaFaces[ metafacei ]._numberOfFaceIndex ;
		//		++ facej )
		//	{
		//		ShadowMetaFaceData* sw_cur = 
		//			&_fatherDataPlus->_shadowMetaFaces[metafacei]._shadowFaceData[ facej ];

		//		if ( sw_cur == sw_face_data )
		//		{
		//			// ���ʱ���治��Ҫ�����ˡ�
		//			continue;
		//		}

		//		
		//	}
		//}

		//------------------------------------------
		// �������ˣ���û��return
		// ��ʱ�������Ļ������û���ڽӱߡ�
		//------------------------------------------
		assert( sw_face_data->p3p1AdjFace == NULL );
		sw_face_data->p3p1AdjFace = sw_face_data;

		return;
	}
	//----------------------------------------------------------------
	void ShadowMetaFace::searchP1P2Adj_element2( ShadowMetaFaceData* sw_face_data )
	{
		// ֻҪ�ж�������NULL�Ϳ��Ա�֤�Ѿ��������ˡ�
		if ( sw_face_data->p1p2AdjFace != NULL )
		{
			return;
		}

		assert( _vaway->_sizeOfVertexElements == sizeof( VgmVertexElement2 ) );

		VgmVertexElement2* elements = 
			(VgmVertexElement2*)_vaway->_vertexElements;

		const int& p1v =  sw_face_data->_faceindex->_point0;

		const int& p2v = sw_face_data->_faceindex->_point1;


		ShadowMetaFace& faces = *sw_face_data->_fatherShadowFaceStruct;

		for ( int i = 0 ; i < faces._metaFaceStruct->_numberOfFaceIndex ; ++ i )
		{
			ShadowMetaFaceData* sw_cur = &faces._shadowFaceData[ i ];

			if ( sw_cur == sw_face_data )
			{
				// ���ʱ���治��Ҫ�����ˡ�
				continue;
			}

			const int& p1jv = sw_cur->_faceindex->_point0;
			const int& p2jv = sw_cur->_faceindex->_point1;
			const int& p3jv = sw_cur->_faceindex->_point2;

			if ( 
				( sw_cur->p1p2AdjFace != NULL )
				&&
				( sw_cur->p2p3AdjFace != NULL )
				&&
				( sw_cur->p3p1AdjFace != NULL )
				)
			{
				// ���ڶ��Ѿ����ٱߵ�����˵����Ҫ�ٱȽ��ˡ�
				continue;
			}

			if ( ( sw_cur->p1p2AdjFace == NULL || sw_cur->p3p1AdjFace == NULL ) 
				&& p1v == p1jv )
			{
				if ( sw_cur->p1p2AdjFace == NULL && p2v == p2jv )
				{
					sw_face_data->p1p2AdjFace = sw_cur;
					sw_cur->p1p2AdjFace = sw_face_data;
					return;
				}
				else if ( sw_cur->p3p1AdjFace == NULL && p2v == p3jv )
				{
					sw_face_data->p1p2AdjFace = sw_cur;
					sw_cur->p3p1AdjFace = sw_face_data;
					return;
				}
			}
			else if ( ( sw_cur->p1p2AdjFace == NULL || sw_cur->p2p3AdjFace == NULL ) 
				&& p1v == p2jv )
			{
				if ( sw_cur->p1p2AdjFace == NULL && p2v == p1jv )
				{
					sw_face_data->p1p2AdjFace = sw_cur;
					sw_cur->p1p2AdjFace = sw_face_data;
					return;
				}
				else if ( sw_cur->p2p3AdjFace == NULL && p2v == p3jv )
				{
					sw_face_data->p1p2AdjFace = sw_cur;
					sw_cur->p2p3AdjFace = sw_face_data;
					return;
				}
			}
			else if (  ( sw_cur->p3p1AdjFace == NULL || sw_cur->p2p3AdjFace == NULL ) 
				&& p1v == p3jv )
			{
				if ( sw_cur->p3p1AdjFace == NULL && p2v == p1jv )
				{
					sw_face_data->p1p2AdjFace = sw_cur;
					sw_cur->p3p1AdjFace = sw_face_data;
					return;
				}
				else if ( sw_cur->p2p3AdjFace == NULL && p2v == p2jv )
				{
					sw_face_data->p1p2AdjFace = sw_cur;
					sw_cur->p2p3AdjFace = sw_face_data;
					return;
				}
			}

		}

		//////////////////////////////////////////////////////////////////////////


		//------------------------------------------
		// ��ʼ�������е���
		//------------------------------------------
		//for ( int metafacei = 0 ; metafacei < _vaway->_numberOfMetaFaces ; 
		//	++ metafacei )
		//{
		//	
		//}

		//------------------------------------------
		// �������ˣ���û��return
		// ��ʱ�������Ļ������û���ڽӱߡ�
		//------------------------------------------
		assert( sw_face_data->p1p2AdjFace == NULL );
		sw_face_data->p1p2AdjFace = sw_face_data;

		return;
	}
	//----------------------------------------------------------------
	void ShadowMetaFace::searchP2P3Adj_element2( ShadowMetaFaceData* sw_face_data )
	{
		// ֻҪ�ж�������NULL�Ϳ��Ա�֤�Ѿ��������ˡ�
		if ( sw_face_data->p2p3AdjFace != NULL )
		{
			return;
		}

		assert( _vaway->_sizeOfVertexElements == sizeof( VgmVertexElement2 ) );

		VgmVertexElement2* elements = 
			(VgmVertexElement2*)_vaway->_vertexElements;

		const int& p2v = sw_face_data->_faceindex->_point1;
		const int& p3v = sw_face_data->_faceindex->_point2;

		ShadowMetaFace& faces = *sw_face_data->_fatherShadowFaceStruct;

		for ( int i = 0 ; i < faces._metaFaceStruct->_numberOfFaceIndex ; ++ i )
		{
			ShadowMetaFaceData* sw_cur = &faces._shadowFaceData[ i ];

			if ( sw_cur == sw_face_data )
			{
				// ���ʱ���治��Ҫ�����ˡ�
				continue;
			}

			const int& p1jv = sw_cur->_faceindex->_point0;
			const int& p2jv = sw_cur->_faceindex->_point1;
			const int& p3jv = sw_cur->_faceindex->_point2;


			if ( 
				( sw_cur->p1p2AdjFace != NULL )
				&&
				( sw_cur->p2p3AdjFace != NULL )
				&&
				( sw_cur->p3p1AdjFace != NULL )
				)
			{
				// ���ڶ��Ѿ����ٱߵ�����˵����Ҫ�ٱȽ��ˡ�
				continue;
			}

			if ( ( sw_cur->p1p2AdjFace == NULL || sw_cur->p3p1AdjFace == NULL ) 
				&& p2v == p1jv )
			{
				if ( sw_cur->p1p2AdjFace == NULL && p3v == p2jv )
				{
					sw_face_data->p2p3AdjFace = sw_cur;
					sw_cur->p1p2AdjFace = sw_face_data;
					return;
				}
				else if ( sw_cur->p3p1AdjFace == NULL && p3v == p3jv )
				{
					sw_face_data->p2p3AdjFace = sw_cur;
					sw_cur->p3p1AdjFace = sw_face_data;
					return;
				}
			}
			else if ( ( sw_cur->p1p2AdjFace == NULL || sw_cur->p2p3AdjFace == NULL ) 
				&& p2v == p2jv )
			{
				if ( sw_cur->p1p2AdjFace == NULL && p2v == p1jv )
				{
					sw_face_data->p2p3AdjFace = sw_cur;
					sw_cur->p1p2AdjFace = sw_face_data;
					return;
				}
				else if ( sw_cur->p2p3AdjFace == NULL && p2v == p3jv )
				{
					sw_face_data->p2p3AdjFace = sw_cur;
					sw_cur->p2p3AdjFace = sw_face_data;
					return;
				}
			}
			else if (  ( sw_cur->p3p1AdjFace == NULL || sw_cur->p2p3AdjFace == NULL ) 
				&& p2v == p3jv )
			{
				if ( sw_cur->p3p1AdjFace == NULL && p2v == p1jv )
				{
					sw_face_data->p2p3AdjFace = sw_cur;
					sw_cur->p3p1AdjFace = sw_face_data;
					return;
				}
				else if ( sw_cur->p2p3AdjFace == NULL && p2v == p2jv )
				{
					sw_face_data->p2p3AdjFace = sw_cur;
					sw_cur->p2p3AdjFace = sw_face_data;
					return;
				}
			}

		}


		//------------------------------------------
		// ��ʼ�������е���
		//------------------------------------------
		//for ( int metafacei = 0 ; metafacei < _vaway->_numberOfMetaFaces ; 
		//	++ metafacei )
		//{
		//	for ( int facej = 0 ; facej < _vaway->_metaFaces[ metafacei ]._numberOfFaceIndex ;
		//		++ facej )
		//	{
		//		ShadowMetaFaceData* sw_cur = 
		//			&_fatherDataPlus->_shadowMetaFaces[metafacei]._shadowFaceData[ facej ];

		//		if ( sw_cur == sw_face_data )
		//		{
		//			// ���ʱ���治��Ҫ�����ˡ�
		//			continue;
		//		}

		//		
		//	}
		//}

		//------------------------------------------
		// �������ˣ���û��return
		// ��ʱ�������Ļ������û���ڽӱߡ�
		//------------------------------------------
		assert( sw_face_data->p2p3AdjFace == NULL );
		sw_face_data->p2p3AdjFace = sw_face_data;

		return;

	}
	//----------------------------------------------------------------
	void ShadowMetaFace::searchP3P1Adj_element2( ShadowMetaFaceData* sw_face_data )
	{
		// ֻҪ�ж�������NULL�Ϳ��Ա�֤�Ѿ��������ˡ�
		if ( sw_face_data->p3p1AdjFace != NULL )
		{
			return;
		}

		assert( _vaway->_sizeOfVertexElements == sizeof( VgmVertexElement2 ) );

		VgmVertexElement2* elements = 
			(VgmVertexElement2*)_vaway->_vertexElements;

		const int& p1v = sw_face_data->_faceindex->_point0;
		const int& p3v = sw_face_data->_faceindex->_point2;


		ShadowMetaFace& faces = *sw_face_data->_fatherShadowFaceStruct;

		for ( int i = 0 ; i < faces._metaFaceStruct->_numberOfFaceIndex ; ++ i )
		{
			ShadowMetaFaceData* sw_cur = &faces._shadowFaceData[ i ];

			if ( sw_cur == sw_face_data )
			{
				// ���ʱ���治��Ҫ�����ˡ�
				continue;
			}

			const int& p1jv = sw_cur->_faceindex->_point0;
			const int& p2jv = sw_cur->_faceindex->_point1;
			const int& p3jv = sw_cur->_faceindex->_point2;

			if ( 
				( sw_cur->p1p2AdjFace != NULL )
				&&
				( sw_cur->p2p3AdjFace != NULL )
				&&
				( sw_cur->p3p1AdjFace != NULL )
				)
			{
				// ���ڶ��Ѿ����ٱߵ�����˵����Ҫ�ٱȽ��ˡ�
				continue;
			}

			if ( ( sw_cur->p1p2AdjFace == NULL || sw_cur->p3p1AdjFace == NULL ) 
				&& p1v == p1jv )
			{
				if ( sw_cur->p1p2AdjFace == NULL && p3v == p2jv )
				{
					sw_face_data->p3p1AdjFace = sw_cur;
					sw_cur->p1p2AdjFace = sw_face_data;
					return;
				}
				else if ( sw_cur->p3p1AdjFace == NULL && p3v == p3jv )
				{
					sw_face_data->p3p1AdjFace = sw_cur;
					sw_cur->p3p1AdjFace = sw_face_data;
					return;
				}
			}
			else if ( ( sw_cur->p1p2AdjFace == NULL || sw_cur->p2p3AdjFace == NULL ) 
				&& p1v == p2jv )
			{
				if ( sw_cur->p1p2AdjFace == NULL && p3v == p1jv )
				{
					sw_face_data->p3p1AdjFace = sw_cur;
					sw_cur->p1p2AdjFace = sw_face_data;
					return;
				}
				else if ( sw_cur->p2p3AdjFace == NULL && p3v == p3jv )
				{
					sw_face_data->p3p1AdjFace = sw_cur;
					sw_cur->p2p3AdjFace = sw_face_data;
					return;
				}
			}
			else if (  ( sw_cur->p3p1AdjFace == NULL || sw_cur->p2p3AdjFace == NULL ) 
				&& p1v == p3jv )
			{
				if ( sw_cur->p3p1AdjFace == NULL && p3v == p1jv )
				{
					sw_face_data->p3p1AdjFace = sw_cur;
					sw_cur->p3p1AdjFace = sw_face_data;
					return;
				}
				else if ( sw_cur->p2p3AdjFace == NULL && p3v == p2jv )
				{
					sw_face_data->p3p1AdjFace = sw_cur;
					sw_cur->p2p3AdjFace = sw_face_data;
					return;
				}
			}

		}

		//------------------------------------------
		// ��ʼ�������е���
		//------------------------------------------
		//for ( int metafacei = 0 ; metafacei < _vaway->_numberOfMetaFaces ; 
		//	++ metafacei )
		//{
		//	for ( int facej = 0 ; facej < _vaway->_metaFaces[ metafacei ]._numberOfFaceIndex ;
		//		++ facej )
		//	{
		//		ShadowMetaFaceData* sw_cur = 
		//			&_fatherDataPlus->_shadowMetaFaces[metafacei]._shadowFaceData[ facej ];

		//		if ( sw_cur == sw_face_data )
		//		{
		//			// ���ʱ���治��Ҫ�����ˡ�
		//			continue;
		//		}

		//		
		//	}
		//}

		//------------------------------------------
		// �������ˣ���û��return
		// ��ʱ�������Ļ������û���ڽӱߡ�
		//------------------------------------------
		assert( sw_face_data->p3p1AdjFace == NULL );
		sw_face_data->p3p1AdjFace = sw_face_data;

		return;
	}

	//----------------------------------------------------------------
	void ShadowMetaFace::searchAndSetAdj()
	{
		if ( _vaway->_sizeOfVertexElements == sizeof( VgmVertexElement1 ) )
		{
			//------------------------------------------
			// ����ÿ���棬�����ٱ�
			//------------------------------------------
			for ( int i = 0 ; i < _metaFaceStruct->_numberOfFaceIndex ; ++ i )
			{
				searchP1P2Adj( &_shadowFaceData[i] );
				searchP2P3Adj( &_shadowFaceData[i] );
				searchP3P1Adj( &_shadowFaceData[i] );
			}
		}
		else
		{
			assert( _vaway->_sizeOfVertexElements == sizeof( VgmVertexElement2 ) );
			//------------------------------------------
			// ����ÿ���棬�����ٱ�
			//------------------------------------------
			for ( int i = 0 ; i < _metaFaceStruct->_numberOfFaceIndex ; ++ i )
			{
				searchP1P2Adj_element2( &_shadowFaceData[i] );
				searchP2P3Adj_element2( &_shadowFaceData[i] );
				searchP3P1Adj_element2( &_shadowFaceData[i] );
			}

		}

	}
	//----------------------------------------------------------------
	void ShadowMetaFace::debugCheckAdj()
	{
		for ( int i = 0 ; i < _metaFaceStruct->_numberOfFaceIndex ; ++ i )
		{
			assert( _shadowFaceData[i].p1p2AdjFace != NULL);
			assert( _shadowFaceData[i].p2p3AdjFace != NULL);
			assert( _shadowFaceData[i].p3p1AdjFace != NULL);
		}
	}
	//----------------------------------------------------------------



}// end of namespace vgMesh
