


#include <vgStableHeaders.h>
#include <vgMesh/vgmMeshRenderWayVertexArray.h>
#include <vgMesh/vgmMeshRenderWayVbo.h>
#include <vgMesh/vgmFileDefinitionMod.h>
#include <vgImage/vgiTextureManager.h>
#include <vgKernel/vgkOpenGLSupport.h>

#include <vgKernel/vgkMath.h>
#include <vgKernel/vgkIntersectSegment3Triangle3.h>

namespace vgMesh {
	
#define BUFFER_OFFSET(i) ((char *)NULL + (i))

	//----------------------------------------------------------------
	MeshRenderWayVbo::~MeshRenderWayVbo()
	{

		if ( _isReadyForVBORender == false )
		{
			return;
		}

		if ( _vertexBufferObjectID != VG_NULL_VBO_ID )
		{
			glDeleteBuffersARB( 1, &_vertexBufferObjectID ); 
			_vertexBufferObjectID = VG_NULL_VBO_ID;
		}

		if ( _metaFaces != NULL )
		{
			assert( _numberOfMetaFaces > 0 );

			for ( int i = 0 ; i < _numberOfMetaFaces ; ++i )
			{
				if ( _metaFaces[i]._elementBufferObjectID != VG_NULL_VBO_ID )
				{
					glDeleteBuffersARB( 1, &_metaFaces[i]._elementBufferObjectID ); 
					_metaFaces[i]._elementBufferObjectID = VG_NULL_VBO_ID;
				}
			}

			delete[] _metaFaces;
			_metaFaces = NULL;
		}
	}
	//----------------------------------------------------------------
	MeshRenderWayVbo::MeshRenderWayVbo( 
		MeshRenderWayVertexArray* vertexarray )
	{
		assert( vertexarray != NULL );
		_numberOfMetaFaces = 0;
		_metaFaces = NULL;
		_vertexBufferObjectID = VG_NULL_VBO_ID;

		_preLod = VGLOD_COARSE;

		_layersOfTexture = 0;

		_texMgr = vgImage::TextureManager::getSingletonPtr();
		assert( _texMgr != NULL );

		//------------------------------------------
		// 1.���ȳ�ʼ����Ҫ�ı���
		//------------------------------------------
		assert( vertexarray != NULL );
		_numberOfMetaFaces = vertexarray->_numberOfMetaFaces;
		assert( _numberOfMetaFaces > 0 );

		_renderType = vertexarray->_renderType;

		_metaFaces = new VboMetaFaceStruct[ _numberOfMetaFaces ];

		for ( int i = 0 ; i < _numberOfMetaFaces ; ++i )
		{
			_metaFaces[i]._numOfElements =
				vertexarray->_metaFaces[i]._numberOfFaceIndex * 3;
		}

		//------------------------------------------
		// 2.�������vbo
		//------------------------------------------
		_isReadyForVBORender = generateVbo( vertexarray );

		//------------------------------------------
		// 3.�����TextureObserver
		//------------------------------------------
		assert( _layersOfTexture == 0 );
		_layersOfTexture = vertexarray->_layersOfTexture;
		assert( _layersOfTexture != 0 );

		for ( int i = 0 ; i < _numberOfMetaFaces ; ++ i )
		{
			VboMetaFaceStruct& curface = _metaFaces[i];
			curface._numberOfTexture = vertexarray->_metaFaces[i]._numberOfTexture;

			assert( curface._texFileName == NULL );
			curface._texFileName = new String[ curface._numberOfTexture ];

			assert( curface._texObserver == NULL );
			curface._texObserver = 
				new TexObsPointer[VGLOD_N * curface._numberOfTexture];

			memset( curface._texObserver , NULL , 
				VGLOD_N * curface._numberOfTexture * sizeof( TexObsPointer ) );

			for ( int k = 0 ; k < curface._numberOfTexture ; ++ k )
			{
				_metaFaces[i]._texFileName[k] = 
					vertexarray->_metaFaces[i]._texFileName[k];
			}

			for ( int j = 0 ; j < VGLOD_N * curface._numberOfTexture; ++ j )
			{
				_metaFaces[i]._texObserver[j] = vertexarray->_metaFaces[i]._texObserver[j];
				if ( _metaFaces[i]._texObserver[j] != NULL )
				{
					_metaFaces[i]._texObserver[j]->addRef();
				}
			}
		}
	}
	//----------------------------------------------------------------
	bool MeshRenderWayVbo::generateVbo( 
		MeshRenderWayVertexArray* vertexarray )
	{
		assert( _metaFaces != NULL );

		assert( vertexarray != NULL );
		if ( vertexarray == NULL ||
			vertexarray->_wholeChunkOfData == NULL )
		{
			return false;
		}

#if _DEBUG
		VGK_CHECK_OPENGL_ERROR();
#endif

		//------------------------------------------
		// 1. create vertex buffer
		//------------------------------------------
		int nArrayObjectSize = vertexarray->_sizeOfVertexElements * 
			vertexarray->_numberOfVertexElements;

		int nParam_ArrayObjectSize = 0;

		if ( nArrayObjectSize != 0 )
		{		
			assert( _vertexBufferObjectID == VG_NULL_VBO_ID );

			glGenBuffersARB( 1, &_vertexBufferObjectID );
			glBindBufferARB( GL_ARRAY_BUFFER_ARB, _vertexBufferObjectID );
			glBufferDataARB( GL_ARRAY_BUFFER_ARB, nArrayObjectSize, 
				vertexarray->_vertexElements, GL_STATIC_DRAW_ARB );

			glGetBufferParameterivARB( GL_ARRAY_BUFFER_ARB, GL_BUFFER_SIZE_ARB, 
				&nParam_ArrayObjectSize );

			if( nParam_ArrayObjectSize <= 0 )
			{
				assert(0);
				::MessageBox(NULL,"glBufferDataARB failed to allocate any memory!",
					"ERROR",MB_OK|MB_ICONEXCLAMATION);
			}

#if _DEBUG
			VGK_CHECK_OPENGL_ERROR();
#endif
		}
		
		//------------------------------------------
		// 2.create element buffer
		//------------------------------------------
		for ( int i = 0; i < _numberOfMetaFaces ; ++i )
		{
			int nArrayObjectSize = sizeof(VgmFaceIndex) * vertexarray->_metaFaces[i]._numberOfFaceIndex;
			int nParam_ArrayObjectSize = 0;

			if ( nArrayObjectSize != 0 )
			{
				assert( _metaFaces[i]._elementBufferObjectID == VG_NULL_VBO_ID );

				glGenBuffersARB( 1, &_metaFaces[i]._elementBufferObjectID );
				glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, _metaFaces[i]._elementBufferObjectID );
				glBufferDataARB( GL_ELEMENT_ARRAY_BUFFER_ARB, nArrayObjectSize, 
					vertexarray->_metaFaces[i]._faceIndex, GL_STATIC_DRAW_ARB );

				glGetBufferParameterivARB( GL_ELEMENT_ARRAY_BUFFER_ARB, GL_BUFFER_SIZE_ARB, &nParam_ArrayObjectSize );

				if( nParam_ArrayObjectSize <= 0 )
				{
					assert(0);
					MessageBox(NULL,"glBufferDataARB failed to allocate any memory!",
						"ERROR",MB_OK|MB_ICONEXCLAMATION);
				}
			}
#if _DEBUG
			VGK_CHECK_OPENGL_ERROR();
#endif
		}

		return true;
	}
	//----------------------------------------------------------------
	//----------------------------------------------------------------
	void MeshRenderWayVbo::render(const int& lod, int force_render_in_num_layers,
		RgbaQuad* colorval )
	{
		if ( _isReadyForVBORender == false)
		{ 
			return;
		}

		requireTexObserverByLod( lod );

		assert( _vertexBufferObjectID !=VG_NULL_VBO_ID );



		if(  _renderType == 1 || _renderType == 2 )
		{
			glEnable(GL_ALPHA_TEST);
			glAlphaFunc(GL_GREATER, 0.5f);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glEnable(GL_BLEND);
		}
		else
		{
			glDisable( GL_ALPHA_TEST );
			glDisable( GL_BLEND );
		}

		if ( _isTransparent )
		{
			glBlendFunc(GL_SRC_COLOR, GL_ONE_MINUS_SRC_COLOR);
			glEnable(GL_BLEND);	
			glDisable(GL_DEPTH_TEST);
		}

		if ( colorval->alpha == 0 )
		{
			glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
			glColor4f( 1.0f, 1.0f, 1.0f, 1.0f );	
		}
		else
		{
			//------------------------------------------
			// ��ʼ�����ɫ
			//------------------------------------------
			float ratio = colorval->alpha / 255.0f;

			float arg[4] = { ratio, ratio, ratio , ratio };

			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
			glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_INTERPOLATE);
			glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_RGB, GL_PRIMARY_COLOR);
			glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_RGB, GL_SRC_COLOR);
			glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE1_RGB, GL_TEXTURE);
			glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND1_RGB, GL_SRC_COLOR);
			glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE2_RGB, GL_CONSTANT);
			glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND2_RGB, GL_SRC_COLOR);
			glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, arg);

			glColor3f(	colorval->red / 255.0f, 
				colorval->green / 255.0f , 
				colorval->blue / 255.0f );
		}


		if ( _layersOfTexture == 1 )
		{
			renderInOneLayer();
		}
		else if ( _layersOfTexture == 2 )
		{
			if ( force_render_in_num_layers == 1 )
			{
				VGK_CHECK_OPENGL_ERROR();
				renderTwoLayerInOneLayer();
				VGK_CHECK_OPENGL_ERROR();
			}
			else
			{
				VGK_CHECK_OPENGL_ERROR();
				renderInTwoLayer();
				VGK_CHECK_OPENGL_ERROR();
			}
		}
		else
		{
			assert(0);
		}

		if ( _isTransparent )
		{
			glDisable(GL_BLEND);	
			glEnable(GL_DEPTH_TEST);
		}

		if(  _renderType == 1 || _renderType == 2 )
		{
			glDisable( GL_ALPHA_TEST );
			glDisable( GL_BLEND );
		}

	}
	//----------------------------------------------------------------
	void MeshRenderWayVbo::requireTexObserverByLod( const int& lod )
	{
		assert( lod <= VGLOD_CLEAR );
		assert( lod >= VGLOD_COARSE );

		if ( _preLod == lod )
		{
			return;
		}

		//------------------------------------------
		// ��lod�б仯ʱ,����
		//------------------------------------------
		_preLod = lod;

		if ( lod == VGLOD_COARSE )
		{
			//------------------------------------------
			// ��Ϊ�������һ��ʱ,
			// ж������������.
			//------------------------------------------
			for ( int i = 0 ; i < _numberOfMetaFaces ; ++i )
			{
				TexObsPointer*& observers = _metaFaces[i]._texObserver;
				const int& texNum = _metaFaces[i]._numberOfTexture;
				//const int& texNum = 1;

				for ( int j = 0 ; j < texNum * VGLOD_N; j+= VGLOD_N)
				{
					if ( observers[ VGLOD_COARSE + j ] == NULL )
					{
						continue;
					}

					for ( int k = 1 + j; k < VGLOD_N + j; ++ k )
					{
						if ( observers[k] != NULL )
						{
							observers[k]->releaseRef();
							observers[k] = NULL;
						}
					}
				}
			}// end for i
		}
		else if ( lod == VGLOD_CLEAR )
		{
			//------------------------------------------
			// ��Ϊ��������һ��ʱ,ֻҪȷ��
			// Clearest Observer ���뼴��
			//------------------------------------------
			for ( int i = 0 ; i < _numberOfMetaFaces ; ++i )
			{
				TexObsPointer*& observers = _metaFaces[i]._texObserver;
				const int& texNum = _metaFaces[i]._numberOfTexture;
				//const int& texNum = 1;

				for ( int j = 0 ; j < texNum * VGLOD_N; j+= VGLOD_N)
				{
					if ( observers[ VGLOD_COARSE + j] == NULL )
					{
						continue;
					}

					if ( observers[ VGLOD_CLEAR + j] == NULL )
					{
						observers[ VGLOD_CLEAR + j]  = _texMgr->getTextureObserver(
							_metaFaces[i]._texFileName[j/VGLOD_N] , VGLOD_CLEAR );

						// coarest���õ���,��ô��������һ���϶�Ҳ�ܵõ�.
						assert( observers[ VGLOD_CLEAR + j] != NULL );

						observers[ VGLOD_CLEAR + j]->addRef();
					}
					else
					{
						// ��������һ���Ѿ�����������,ж�صڶ���
						if ( observers[ VGLOD_COARSE + 1 + j] != NULL )
						{
							observers[ VGLOD_COARSE + 1 + j]->releaseRef();
							observers[ VGLOD_COARSE + 1 + j] = NULL;
						}
					}
				}
			} // end for
		}
		else
		{

			//------------------------------------------
			// ��Ϊ�м��һ��ʱ
			//------------------------------------------
			for ( int i = 0 ; i < _numberOfMetaFaces ; ++i )
			{
				TexObsPointer*& observers = _metaFaces[i]._texObserver;
				const int& texNum = _metaFaces[i]._numberOfTexture;
				//const int& texNum = 1;

				for ( int j = 0 ; j < texNum * VGLOD_N; j+= VGLOD_N)
				{
					if ( observers[ VGLOD_COARSE + j] == NULL )
					{
						continue;
					}

					// ������������Ƿ��Ѿ�����
					if ( observers[ VGLOD_CLEAR + j] != NULL &&
						observers[ VGLOD_COARSE + 1 + j] != NULL)
					{
						observers[ VGLOD_CLEAR + j]->releaseRef();
						observers[ VGLOD_CLEAR + j] = NULL;

					}
					//else if ( observers[ VGLOD_CLEAR ] != NULL && 
					//	observers[ VGLOD_COARSE + 1 ] == NULL )
					//{
					//	// do nothing.
					//}
					else if ( observers[ VGLOD_COARSE + 1 + j] == NULL )
					{
						// ����м伶,û����Ļ�,��������.
						observers[ VGLOD_COARSE + 1 + j] = _texMgr->getTextureObserver(
							_metaFaces[i]._texFileName[j/VGLOD_N] , VGLOD_COARSE + 1 );

						assert( observers[ VGLOD_COARSE + 1 + j] != NULL );
						observers[ VGLOD_COARSE + 1 + j]->addRef();
					}

				}
			}
		}


		return;
	}
	//----------------------------------------------------------------
	bool MeshRenderWayVbo::initialiseInMainThread()
	{
		//------------------------------------------
		// ���������vbo�Ĳ�����ʵ���Էŵ�����
		//------------------------------------------
		return true;
	}
	//----------------------------------------------------------------
	void MeshRenderWayVbo::renderInOneLayer()
	{
		glPushMatrix();



		assert( _vertexBufferObjectID != VG_NULL_VBO_ID );

		glBindBufferARB( GL_ARRAY_BUFFER_ARB, _vertexBufferObjectID );

		for ( int i = 0 ; i < _numberOfMetaFaces ; ++i )
		{
			// ����useBind���Զ�������������һ��,����ֻҪ
			// ���������һ����observer����.
			vgImage::TextureObserver*& cur_tex = 
				_metaFaces[i]._texObserver[ VGLOD_COARSE ];

			if ( cur_tex != NULL )
			{
				if ( cur_tex->isEmptyTexture() == true )
				{
					continue;
				}
				glEnable( GL_TEXTURE_2D );
				cur_tex->useBind();
			}
			else
			{
				glDisable( GL_TEXTURE_2D );
			}

			//glColor3f(1, 1, 1);	
			//glColor3f(0.8f, 0.8f, 0.8f);	


			assert( _metaFaces[i]._elementBufferObjectID != VG_NULL_VBO_ID);
			glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, 
				_metaFaces[i]._elementBufferObjectID );

			const int nVertexStride = sizeof(VertexElementStruct);
			const int nIndexStride = sizeof(GLubyte);
			const int nOffsetForNormals = sizeof(GLfloat) * 2;
			const int nOffsetForVertices = sizeof(GLfloat) * 5;

			glTexCoordPointer( 2, GL_FLOAT, nVertexStride, BUFFER_OFFSET(0) );
			glNormalPointer( GL_FLOAT, nVertexStride, BUFFER_OFFSET(nOffsetForNormals) );
			glVertexPointer( 3, GL_FLOAT, nVertexStride, BUFFER_OFFSET(nOffsetForVertices) );

			glEnableClientState( GL_TEXTURE_COORD_ARRAY );
			glEnableClientState( GL_NORMAL_ARRAY );
			glEnableClientState( GL_VERTEX_ARRAY );
			glEnableClientState( GL_INDEX_ARRAY );

			//glColor3d(1, 1, 1);	
			//glColor3f(0.8f, 0.8f, 0.8f);	

			glDrawElements( GL_TRIANGLES, 
				_metaFaces[i]._numOfElements , GL_UNSIGNED_INT, 
				BUFFER_OFFSET(0));

			glDisableClientState( GL_TEXTURE_COORD_ARRAY );
			glDisableClientState( GL_NORMAL_ARRAY );
			glDisableClientState( GL_VERTEX_ARRAY );
			glDisableClientState( GL_INDEX_ARRAY );

			glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, NULL );

		}


		glBindBufferARB( GL_ARRAY_BUFFER_ARB, NULL );

		glPopMatrix();
	}
	//----------------------------------------------------------------
	void MeshRenderWayVbo::renderInTwoLayer()
	{
		assert( _layersOfTexture == 2);

		glPushMatrix();

		assert( _vertexBufferObjectID != VG_NULL_VBO_ID );
		glBindBufferARB( GL_ARRAY_BUFFER_ARB, _vertexBufferObjectID );

		for ( int i = 0 ; i < _numberOfMetaFaces ; ++i )
		{
			glActiveTexture( GL_TEXTURE0 );
			//glColor4f(1.0f, 1.0f, 1.0f, 1.0f);	
			//glColor4f(0.8f, 0.8f, 0.8f, 1.0);	
			{
				TexObsPointer& cur_tex = _metaFaces[i]._texObserver[
					VGLOD_COARSE];

					if ( cur_tex != NULL && cur_tex->isEmptyTexture() == false)
					{
						glEnable( GL_TEXTURE_2D );
						cur_tex->useBind();
					}
					else
					{
						glDisable( GL_TEXTURE_2D );
						glActiveTexture( GL_TEXTURE0 );
						glDisable( GL_TEXTURE_2D );
						continue;
					}
			}

			glActiveTexture( GL_TEXTURE1 );
			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
			{
				TexObsPointer& cur_tex = _metaFaces[i]._texObserver[
					VGLOD_COARSE  + VGLOD_N ];

					if ( cur_tex != NULL && cur_tex->isEmptyTexture() == false)
					{
						glEnable( GL_TEXTURE_2D );
						cur_tex->useBind();
					}
					else
					{
						glDisable( GL_TEXTURE_2D );
					}
			}


			assert( _metaFaces[i]._elementBufferObjectID != VG_NULL_VBO_ID );
			glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, 
				_metaFaces[i]._elementBufferObjectID );

			const int nVertexStride = sizeof(VertexElementStruct2);
			const int nIndexStride = sizeof(GLubyte);
			const int nOffsetForNormals = sizeof(GLfloat) * 2;
			const int nOffsetForVertices = sizeof(GLfloat) * 5;
			const int nOffsetForBake = sizeof( GLfloat ) * 8; 

			glClientActiveTexture( GL_TEXTURE1 );
			glTexCoordPointer( 2, GL_FLOAT, nVertexStride, BUFFER_OFFSET(nOffsetForBake) );
			glVertexPointer( 3, GL_FLOAT, nVertexStride,BUFFER_OFFSET(nOffsetForVertices));
			glNormalPointer(  GL_FLOAT, nVertexStride, BUFFER_OFFSET(nOffsetForNormals));

			glEnableClientState( GL_VERTEX_ARRAY);
			glEnableClientState( GL_NORMAL_ARRAY);
			glEnableClientState( GL_INDEX_ARRAY );
			glEnableClientState( GL_TEXTURE_COORD_ARRAY);	


			glClientActiveTexture( GL_TEXTURE0 );
			glTexCoordPointer( 2, GL_FLOAT, nVertexStride, BUFFER_OFFSET(0) );
			glVertexPointer( 3, GL_FLOAT, nVertexStride,BUFFER_OFFSET(nOffsetForVertices));
			glNormalPointer(  GL_FLOAT, nVertexStride, BUFFER_OFFSET(nOffsetForNormals));
			glEnableClientState( GL_VERTEX_ARRAY);
			glEnableClientState( GL_NORMAL_ARRAY);
			glEnableClientState( GL_INDEX_ARRAY );
			glEnableClientState( GL_TEXTURE_COORD_ARRAY);	


			glDrawElements( GL_TRIANGLES, 
				_metaFaces[i]._numOfElements , GL_UNSIGNED_INT, 
				BUFFER_OFFSET(0));


			glDisableClientState( GL_NORMAL_ARRAY);
			glDisableClientState( GL_VERTEX_ARRAY);
			glDisableClientState( GL_INDEX_ARRAY );
			glDisableClientState( GL_TEXTURE_COORD_ARRAY);

			glClientActiveTexture( GL_TEXTURE1 );
			glDisableClientState( GL_NORMAL_ARRAY);
			glDisableClientState( GL_VERTEX_ARRAY);
			glDisableClientState( GL_INDEX_ARRAY );
			glDisableClientState( GL_TEXTURE_COORD_ARRAY);

			glClientActiveTexture( GL_TEXTURE0 );
			glDisableClientState( GL_NORMAL_ARRAY);
			glDisableClientState( GL_VERTEX_ARRAY);
			glDisableClientState( GL_INDEX_ARRAY );
			glDisableClientState( GL_TEXTURE_COORD_ARRAY);


			glDisable( GL_TEXTURE_2D );
			glActiveTexture( GL_TEXTURE1 );
			glDisable(GL_TEXTURE_2D);
			glActiveTexture( GL_TEXTURE0 );
			glDisable(GL_TEXTURE_2D);


			glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, NULL );
		}

		glBindBufferARB( GL_ARRAY_BUFFER_ARB, NULL );

		glPopMatrix();


	}
	//----------------------------------------------------------------

	std::pair<bool, float> MeshRenderWayVbo::testIntersection(const vgKernel::Ray* ray)
	{
		GLvoid *vertexbuf = NULL;
		std::pair<bool, float>   bResult;

		bResult.first = false;
		bResult.second = 1e10;

		glBindBufferARB( GL_ARRAY_BUFFER_ARB, _vertexBufferObjectID );
		vertexbuf = glMapBufferARB(GL_ARRAY_BUFFER_ARB , GL_READ_WRITE_ARB);

		assert( vertexbuf != NULL) ;

		char* pVertex = (char*)vertexbuf;

		GLvoid* facemat = NULL;
		FaceIndex* pFaceIndex;
		int i, j;

		int v0Index;
		int v1Index;
		int v2Index;

		void* vp0 = NULL;
		void* vp1 = NULL;
		void* vp2 = NULL;

		for (i=0; i<_numberOfMetaFaces; i++)
		{

			glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, _metaFaces[i]._elementBufferObjectID);
			facemat = glMapBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB , GL_READ_WRITE_ARB);
			assert( facemat != NULL) ;

			pFaceIndex = (FaceIndex*)facemat;

			for (j=0; j<_metaFaces[i]._numOfElements / 3; j++)
			{
				v0Index = pFaceIndex[j].x;
				v1Index = pFaceIndex[j].y;
				v2Index = pFaceIndex[j].z;


				if ( _layersOfTexture == 1 )
				{
					vp0 = pVertex + sizeof( VertexElementStruct ) * v0Index + 
						sizeof( float ) * 5;
					vp1 = pVertex + sizeof( VertexElementStruct ) * v1Index + 
						sizeof( float ) * 5;
					vp2 = pVertex + sizeof( VertexElementStruct ) * v2Index + 
						sizeof( float ) * 5;
				}
				else
				{
					vp0 = pVertex + sizeof( VertexElementStruct2 ) * v0Index + 
						sizeof( float ) * 5;
					vp1 = pVertex + sizeof( VertexElementStruct2 ) * v1Index + 
						sizeof( float ) * 5;
					vp2 = pVertex + sizeof( VertexElementStruct2 ) * v2Index + 
						sizeof( float ) * 5;
				}

				Vec3* p0 =  (Vec3*)vp0;
				Vec3* p1 =  (Vec3*)vp1;
				Vec3* p2 =  (Vec3*)vp2;

				std::pair<bool, float> result = vgKernel::Math::intersects( *ray , *p0 , *p1 , *p2 );
				// 				if (result.first == TRUE)
				// 				{
				// 					bResult = true;
				// 					break;
				// 				}
				if (result.first && result.second < bResult.second)
				{
					bResult.first = true;
					bResult.second = result.second;
				}
			}

			glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, _metaFaces[i]._elementBufferObjectID);
			glUnmapBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB );

			// 			if (bResult == TRUE)
			// 			{
			// 				break;
			// 			}
		}


		glUnmapBufferARB(GL_ARRAY_BUFFER_ARB);
		pVertex = NULL;
		glBindBufferARB( GL_ARRAY_BUFFER_ARB, NULL );

		return bResult;


	}
	//----------------------------------------------------------------
	bool MeshRenderWayVbo::testIntersection( const vgKernel::Segment3* segment )
	{
		GLvoid *vertexbuf = NULL;
		bool   bResult = false;

		glBindBufferARB( GL_ARRAY_BUFFER_ARB, _vertexBufferObjectID );
		vertexbuf = glMapBufferARB(GL_ARRAY_BUFFER_ARB , GL_READ_WRITE_ARB);

		assert( vertexbuf != NULL) ;

		char* pVertex = (char*)vertexbuf;

		GLvoid* facemat = NULL;
		FaceIndex* pFaceIndex;
		int i, j;

		int v0Index;
		int v1Index;
		int v2Index;

		void* vp0 = NULL;
		void* vp1 = NULL;
		void* vp2 = NULL;

		for (i=0; i<_numberOfMetaFaces; i++)
		{

			glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, _metaFaces[i]._elementBufferObjectID);
			facemat = glMapBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB , GL_READ_WRITE_ARB);
			assert( facemat != NULL) ;

			pFaceIndex = (FaceIndex*)facemat;

			for (j=0; j<_metaFaces[i]._numOfElements / 3; j++)
			{
				v0Index = pFaceIndex[j].x;
				v1Index = pFaceIndex[j].y;
				v2Index = pFaceIndex[j].z;

				if ( _layersOfTexture == 1 )
				{
					vp0 = pVertex + sizeof( VertexElementStruct ) * v0Index + 
						sizeof( float ) * 5;
					vp1 = pVertex + sizeof( VertexElementStruct ) * v1Index + 
						sizeof( float ) * 5;
					vp2 = pVertex + sizeof( VertexElementStruct ) * v2Index + 
						sizeof( float ) * 5;
				}
				else
				{
					vp0 = pVertex + sizeof( VertexElementStruct2 ) * v0Index + 
						sizeof( float ) * 5;
					vp1 = pVertex + sizeof( VertexElementStruct2 ) * v1Index + 
						sizeof( float ) * 5;
					vp2 = pVertex + sizeof( VertexElementStruct2 ) * v2Index + 
						sizeof( float ) * 5;
				}

				Vec3* p0 =  (Vec3*)vp0;
				Vec3* p1 =  (Vec3*)vp1;
				Vec3* p2 =  (Vec3*)vp2;


				Triangle3 tri( *p0 , *p1 , *p2 	);


				IntrSegment3Triangle3 intr(  *segment , tri );

				bool result = intr.Test();
				if (result == true)
				{
					bResult = true;
					break;
				}
			}

			glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, _metaFaces[i]._elementBufferObjectID);
			glUnmapBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB );

			if (bResult == true)
			{
				break;
			}
		}


		glUnmapBufferARB(GL_ARRAY_BUFFER_ARB);
		pVertex = NULL;
		glBindBufferARB( GL_ARRAY_BUFFER_ARB, NULL );

		return bResult;
	}
	//----------------------------------------------------------------
	void MeshRenderWayVbo::renderTwoLayerInOneLayer()
	{

		assert( _layersOfTexture == 2);

		glPushMatrix();

		assert( _vertexBufferObjectID != VG_NULL_VBO_ID );
		glBindBufferARB( GL_ARRAY_BUFFER_ARB, _vertexBufferObjectID );

		for ( int i = 0 ; i < _numberOfMetaFaces ; ++i )
		{
			glColor4f(1.0f, 1.0f, 1.0f, 1.0f);	
			//glColor4f(0.8f, 0.8f, 0.8f, 1.0);	
			{
				TexObsPointer& cur_tex = _metaFaces[i]._texObserver[
					VGLOD_COARSE];

					if ( cur_tex != NULL && cur_tex->isEmptyTexture() == false)
					{
						glEnable( GL_TEXTURE_2D );
						cur_tex->useBind();
					}
					else
					{
						glDisable( GL_TEXTURE_2D );
						glActiveTexture( GL_TEXTURE0 );
						glDisable( GL_TEXTURE_2D );
						continue;
					}
			}


			assert( _metaFaces[i]._elementBufferObjectID != VG_NULL_VBO_ID );
			glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, 
				_metaFaces[i]._elementBufferObjectID );

			const int nVertexStride = sizeof(VertexElementStruct2);
			const int nIndexStride = sizeof(GLubyte);
			const int nOffsetForNormals = sizeof(GLfloat) * 2;
			const int nOffsetForVertices = sizeof(GLfloat) * 5;
			const int nOffsetForBake = sizeof( GLfloat ) * 8; 


			glTexCoordPointer( 2, GL_FLOAT, nVertexStride, BUFFER_OFFSET(0) );
			glVertexPointer( 3, GL_FLOAT, nVertexStride,BUFFER_OFFSET(nOffsetForVertices));
			glNormalPointer(  GL_FLOAT, nVertexStride, BUFFER_OFFSET(nOffsetForNormals));
			glEnableClientState( GL_VERTEX_ARRAY);
			glEnableClientState( GL_NORMAL_ARRAY);
			glEnableClientState( GL_INDEX_ARRAY );
			glEnableClientState( GL_TEXTURE_COORD_ARRAY);	


			glDrawElements( GL_TRIANGLES, 
				_metaFaces[i]._numOfElements , GL_UNSIGNED_INT, 
				BUFFER_OFFSET(0));


			glDisableClientState( GL_NORMAL_ARRAY);
			glDisableClientState( GL_VERTEX_ARRAY);
			glDisableClientState( GL_INDEX_ARRAY );
			glDisableClientState( GL_TEXTURE_COORD_ARRAY);


			glDisable(GL_TEXTURE_2D);


			glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, NULL );
		}

		glBindBufferARB( GL_ARRAY_BUFFER_ARB, NULL );

		glPopMatrix();


	}
	//----------------------------------------------------------------


	//----------------------------------------------------------------
	VboMetaFaceStruct::VboMetaFaceStruct()
	{
		_texFileName = NULL;
		_texObserver = NULL;
		_elementBufferObjectID = VG_NULL_VBO_ID;
		_numOfElements = 0;
		_texObserver = NULL;

		_numberOfTexture = 0;
	}

	//----------------------------------------------------------------
	VboMetaFaceStruct::~VboMetaFaceStruct()
	{
		if ( _texObserver != NULL )
		{
			assert( _numberOfTexture != 0 );

			for ( int i = 0 ; i < VGLOD_N * _numberOfTexture ; ++ i )
			{
				if ( _texObserver[i] != NULL )
				{
					_texObserver[i]->releaseRef();
					_texObserver[i] = NULL;
				}
			}

			delete[] _texObserver;
			_texObserver = NULL;
		}

		if ( _texFileName != NULL )
		{
			delete[] _texFileName;
			_texFileName = NULL;
		}
	}
	//----------------------------------------------------------------



}// end of namespace vgMesh
