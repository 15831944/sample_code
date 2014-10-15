


#include <vgStableHeaders.h>

#if 0

#include <vgModel/vgmoModelRenderWayDisplayList.h>
#include <vgModel/vgmoFileDefinitionMod.h>
#include <vgImage/vgiTextureManager.h>
#include <vgKernel/vgkOpenGLSupport.h>

#include <vgKernel/vgkMath.h>

namespace vgModel {

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

	//----------------------------------------------------------------
	ModelRenderWayDisplayList::~ModelRenderWayDisplayList()
	{
		if (_vertexarray != NULL )
		{
			for (int i =0 ; i <_vertexarray->_numberOfMetaFaces; i++)
			{
				glDeleteLists( _lists[i], 1);

				_lists[i] = 0;
			}
		}

		delete[] _lists;

		delete _vertexarray;
		

	}
	//----------------------------------------------------------------
	ModelRenderWayDisplayList::ModelRenderWayDisplayList( 
		ModelRenderWayVertexArray* vertexarray)
	{
		_lists = NULL;

		_vertexarray = NULL;

		_vertexarray = vertexarray;

		_lists = new GLint[vertexarray->_numberOfMetaFaces];

		memset( _lists, 0 , sizeof(GLint) * vertexarray->_numberOfMetaFaces );  

	}
	//----------------------------------------------------------------
	bool ModelRenderWayDisplayList::generateDisplaylist( 
		ModelRenderWayVertexArray* vertexarray )
	{

		if ( vertexarray->_layersOfTexture == 1 )
		{
			for ( int i = 0 ; i < vertexarray->_numberOfMetaFaces ; ++ i )
			{
				if (_lists[i] == 0)
				{
					_lists[i] = glGenLists( 1 );   
					glNewList(_lists[i], GL_COMPILE );

 					glInterleavedArrays( GL_T2F_N3F_V3F, 0 , vertexarray->_vertexElements );
 
 					glDrawElements(GL_TRIANGLES, vertexarray->_metaFaces[i]._numberOfFaceIndex * 3,
 						GL_UNSIGNED_INT , vertexarray->_metaFaces[i]._faceIndex );

					glEndList();					
				}
			}
		}
		else if ( vertexarray->_layersOfTexture == 2 )
		{	
		}
		else
		{
			assert(0);
		}




		return true;
	}
	//----------------------------------------------------------------
	//----------------------------------------------------------------
	void ModelRenderWayDisplayList::render(const int& lod)
	{

 		requireTexObserverByLod( lod );
 
 
 		if ( _vertexarray->_layersOfTexture == 1 )
 		{
 			renderInOneLayer();
 		}
 		else if ( _vertexarray->_layersOfTexture == 2 )
 		{
 			renderInTwoLayer();
 		}
 		else
 		{
 			assert(0);
 		}


	}
	//----------------------------------------------------------------
	void ModelRenderWayDisplayList::requireTexObserverByLod( const int& lod )
	{
		assert( lod <= VGLOD_CLEAR );
		assert( lod >= VGLOD_COARSE );

		if ( _vertexarray->_preLod == lod )
		{
			return;
		}

		//------------------------------------------
		// ��lod�б仯ʱ,����
		//------------------------------------------
		_vertexarray->_preLod = lod;

		if ( lod == VGLOD_COARSE )
		{
			//------------------------------------------
			// ��Ϊ�������һ��ʱ,
			// ж������������.
			//------------------------------------------
			for ( int i = 0 ; i < _vertexarray->_numberOfMetaFaces ; ++i )
			{
				TexObsPointer*& observers = _vertexarray->_metaFaces[i]._texObserver;
				const int& texNum =_vertexarray->_metaFaces[i]._numberOfTexture;
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
			for ( int i = 0 ; i < _vertexarray->_numberOfMetaFaces ; ++i )
			{
				TexObsPointer*& observers = _vertexarray->_metaFaces[i]._texObserver;
				const int& texNum = _vertexarray->_metaFaces[i]._numberOfTexture;
				//const int& texNum = 1;

				for ( int j = 0 ; j < texNum * VGLOD_N; j+= VGLOD_N)
				{
					if ( observers[ VGLOD_COARSE + j] == NULL )
					{
						continue;
					}

					if ( observers[ VGLOD_CLEAR + j] == NULL )
					{
						observers[ VGLOD_CLEAR + j]  = _vertexarray->_texMgr->getTextureObserver(
							_vertexarray->_metaFaces[i]._texFileName[j/VGLOD_N] , VGLOD_CLEAR );

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
			for ( int i = 0 ; i < _vertexarray->_numberOfMetaFaces ; ++i )
			{
				TexObsPointer*& observers = _vertexarray->_metaFaces[i]._texObserver;
				const int& texNum = _vertexarray->_metaFaces[i]._numberOfTexture;
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
					else if ( observers[ VGLOD_COARSE + 1 + j] == NULL )
					{
						// ����м伶,û����Ļ�,��������.
						observers[ VGLOD_COARSE + 1 + j] = _vertexarray->_texMgr->getTextureObserver(
							_vertexarray->_metaFaces[i]._texFileName[j/VGLOD_N] , VGLOD_COARSE + 1 );

						assert( observers[ VGLOD_COARSE + 1 + j] != NULL );
						observers[ VGLOD_COARSE + 1 + j]->addRef();
					}

				}
			}
		}


		return;
	}
	//----------------------------------------------------------------
	bool ModelRenderWayDisplayList::initialiseInMainThread()
	{		
		generateDisplaylist( _vertexarray );
		return true;
	}
	//----------------------------------------------------------------
	void ModelRenderWayDisplayList::renderInOneLayer()
	{	
		glPushMatrix();

		if(  _vertexarray->_renderType == 1 || _vertexarray->_renderType == 2 )
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

		for ( int i = 0 ; i < _vertexarray->_numberOfMetaFaces ; ++ i )
		{
			// ����useBind���Զ�������������һ��,����ֻҪ
			// ���������һ����observer����.
			vgImage::TextureObserver*& cur_tex = 
				_vertexarray->_metaFaces[i]._texObserver[ VGLOD_COARSE ];

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

			glColor3f(1, 1, 1);	
			//glColor3f(0.8, 0.8, 0.8);

			glCallList( _lists[i] );

		}
		glPopMatrix();
	}
	//----------------------------------------------------------------
	void ModelRenderWayDisplayList::renderInTwoLayer()
	{
		assert( _vertexarray->_layersOfTexture == 2);

		const GLsizei& nVertexStride = _vertexarray->_sizeOfVertexElements; 
		VertexElementStruct2* element = (VertexElementStruct2*)(_vertexarray->_vertexElements);


		glPushMatrix();

		for ( int i = 0 ; i < _vertexarray->_numberOfMetaFaces ; ++ i )
		{

			glActiveTexture( GL_TEXTURE0 );
			glColor4f(1.0f, 1.0f, 1.0f, 1.0f);	
			{
				TexObsPointer& cur_tex = _vertexarray->_metaFaces[i]._texObserver[
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
				TexObsPointer& cur_tex = _vertexarray->_metaFaces[i]._texObserver[
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



			glClientActiveTexture( GL_TEXTURE1 );
			glTexCoordPointer( 2, GL_FLOAT, nVertexStride, 
				&element[0].texCoorUForLit );
			glVertexPointer( 3, GL_FLOAT, nVertexStride,
				&element[0].positionX );
			glNormalPointer(  GL_FLOAT, nVertexStride, 
				&element[0].normalX );
			glEnableClientState( GL_VERTEX_ARRAY);
			glEnableClientState( GL_NORMAL_ARRAY);
			glEnableClientState( GL_INDEX_ARRAY );
			glEnableClientState( GL_TEXTURE_COORD_ARRAY);	


			glClientActiveTexture( GL_TEXTURE0 );
			glTexCoordPointer( 2, GL_FLOAT, nVertexStride, 
				&element[0].texCoorS );
			glVertexPointer( 3, GL_FLOAT, nVertexStride,
				&element[0].positionX );
			glNormalPointer(  GL_FLOAT, nVertexStride, 
				&element[0].normalX );
			glEnableClientState( GL_VERTEX_ARRAY);
			glEnableClientState( GL_NORMAL_ARRAY);
			glEnableClientState( GL_INDEX_ARRAY );
			glEnableClientState( GL_TEXTURE_COORD_ARRAY);	


			glDrawElements(GL_TRIANGLES, _vertexarray->_metaFaces[i]._numberOfFaceIndex * 3,
				GL_UNSIGNED_INT , _vertexarray->_metaFaces[i]._faceIndex );

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


		}

		glPopMatrix();

		VGK_CHECK_OPENGL_ERROR();

	}
	//----------------------------------------------------------------

	bool ModelRenderWayDisplayList::testIntersection(const vgKernel::Ray* ray)
	{
		return _vertexarray->testIntersection( ray );
	}

	bool ModelRenderWayDisplayList::testIntersection( const vgKernel::Segment3* segment )
	{
		return _vertexarray->testIntersection( segment );
	}


	//----------------------------------------------------------------


}// end of namespace vgModel


#endif