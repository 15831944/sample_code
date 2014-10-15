
#include <vgStableHeaders.h>
#include <vgMod/vgObjNode.h>

//#include <vgMod/vgQuadtreeSceneManager.h>
//#include <vgAsyn/vgaTextureObserver.h>
//#include <vgAsyn/vgaTextureManager.h> 
#include <vgKernel/vgkIntersectSegment3Triangle3.h>

#include <vgMesh/vgmMeshManager.h>
#include <vgMod/vgVersionControl.h>



#include <vgEffect/vgWaterNode.h>
#include <vgEffect/vgGlassNode.h>
#include <vgEffect/vgMirrorNode.h>
#include <vgEffect/vgMetalNode.h>
#include <vgEffect/vgBrightfloorNode.h>
#include <vgEffect/vgAviNode.h>
#include <vgEffect/vgSkyboxNode.h>
#include <vgEffect/vgDynamicTexNode.h>
#include <vgSolar/vgsoDefinition.h>

#include <vgKernel/vgkPluginManager.h>

#include <vgKernel/vgkTimelineManager.h>

namespace vgMod{

	VGK_RENDERER_IMPLEMENT( ObjectNode, RENDERER_TYPE_MODOJBECT )

	using namespace vgMath;

	//Vector3* ObjectNode::_shadowVector = NULL;

	//PropertiesParam	ObjectNodeAdapter::s_ParamArray[s_NumOfParam];

	//	Part 0 :����/��������
	ObjectNode::ObjectNode(int nVersionMod/* = VERSION_1*/)
		:m_nVersionMod(nVersionMod)
		,_uniqueID(vgKernel::UniqueIDFactory::getUniqueID() )
		, vgMod::Node( NODE_TYPE_MOD )
	{
		_multiplyMatrix = NULL;
		//	����
		TYPE					= VG_OBJECT;

		//m_PreRenderType = 0;
		_bOpacity = false;


		//	״̬
		//	VBO״̬
		_vertexBufferObjectID	= -1;
		_elementBufferNoMat		= -1;
		_isReadyForVBORender	= false;
		_deleteVBO				= true;
		//	��ʾ�б�״̬
		_displayListID = 0;
		_displayListWithoutTexID = 0;
		_readyForDisplayListRenderingWithoutTex = false;
		_readyForDisplayListRendering = false;
#if 0	
		//	AutoLOD״̬
		b_renderlod			= false;
		lod_eyeheight		= 200;
		lod_dis_small		= 2800;
		lod_dis_big			= 3200;
		lod_facenum			= 0;
#endif		
		//	����
		pMats				= NULL;
		pVertex				= NULL;
		pMatRender			= NULL;
		pFaceNoMat			= NULL;
		pNormalOfFaces		= NULL;
		pVert				= NULL;
		pShadowFaces		= NULL;

		//	����
		mShadowVolume		= -1;
		m_nVertexStride		= 0;
		
		_squaredDistanceToViewer = 0.0f;
		m_bReadMod			= false;

		_bRenderDoubleSides = true;

		_bSetAsTrigger = false;

		m_bUnderWater=false;

		vgKernel::TimelineManager::getSingleton().registerTarget(this);
	}

	ObjectNode::~ObjectNode()
	{
		if ( _multiplyMatrix != NULL )
		{
			delete _multiplyMatrix;
			_multiplyMatrix = NULL;
		}
		//vgThreads::ScopedLock locker(NodeAbstract::mCriticalSection);

		if ( _deleteVBO == false )
		{
			//------------------------------------------
			// ��ʱֱ�ӷ���
			//------------------------------------------
			return;
		}




		if ( _isReadyForVBORender )
		{
			//------------------------------------------
			// Now in explorer edition
			//------------------------------------------
			//_vboReadBufPos = NULL;
			if ( _vertexBufferObjectID != -1 )
			{
				glDeleteBuffersARB( 1, &_vertexBufferObjectID ); 
				_vertexBufferObjectID = -1;
			}

			//VG_CHECK_OPENGL_ERROR();

			//------------------------------------------
			// deal m_numOfMat
			//------------------------------------------
			for (int i = 0; i < m_numOfMat ; ++i )
			{
				ObjectRenderControl& RenderControl = pMatRender[i];

				int nArrayObjectSize = sizeof(FaceIndex) * RenderControl.m_numOfFace;

				if ( (nArrayObjectSize != 0) && (RenderControl._elementBufferObjectID != -1) )
				{
					glDeleteBuffersARB( 1, &RenderControl._elementBufferObjectID );
				}
			}

			//------------------------------------------
			// deal m_numOfNoMat
			//------------------------------------------
			{
				int nArrayObjectSize = sizeof(FaceIndex) * m_numOfNoMat;

				if ( (nArrayObjectSize != 0) && (_elementBufferNoMat != -1) )
				{
					glDeleteBuffersARB( 1, &_elementBufferNoMat );
				}
			}
		}




		if ( _displayListID != 0 )
		{
			glDeleteLists( _displayListID , 1 );
			_displayListID = 0;
		}

		if ( _displayListWithoutTexID != 0 )
		{
			glDeleteLists( _displayListWithoutTexID , 1 );
			_displayListWithoutTexID = 0;
		}

//#endif

		if ( pVertex != NULL )
		{
			delete[] pVertex;
			pVertex = NULL;
		}

		if ( pMats != NULL )
		{
			delete[] pMats;
			pMats = NULL;	
		}

		for (int i = 0; i < m_numOfMat ; i ++)
		{
			if ( pMatRender[i].pFace != NULL )
			{
				delete[] pMatRender[i].pFace;
				pMatRender[i].pFace = NULL;
			}
		}

		if ( pMatRender != NULL )
		{
			delete[] pMatRender;
			pMatRender = NULL;
		}


		if ( pFaceNoMat != NULL )
		{
			delete[] pFaceNoMat;
			pFaceNoMat = NULL;
		}


		if ( pNormalOfFaces != NULL )
		{
			delete[] pNormalOfFaces;	
			pNormalOfFaces = NULL;
		}


		if ( pVert != NULL )
		{
			delete[] pVert;	
			pVert = NULL;
		}


		if( pShadowFaces != NULL )
		{
			delete[] pShadowFaces;		
			pShadowFaces = NULL;	
		}

		vgKernel::TimelineManager::getSingleton().unregisterTarget(this);

	}


	

	//	Part 1 :ʵ�ָ���Ĵ��麯��
	void	ObjectNode::Initialize()
	{
		//	����ĳЩ�̶�����
		configureOnce();

		if (!confirmVBO())// VBO mode
		{//display list mode
			if ( _readyForDisplayListRendering == false )
			{
				sendRenderableDataToDisplaylistOnce();
			}
		}
		
		//	�������ͨ�����ظú��������������ʼ��
		initializeExtra();
	
	}//Initialize




	void	ObjectNode::Render()
	{

		if (!(_isReadyForVBORender||_readyForDisplayListRendering))
		{
			Initialize();
		}

		if (!m_bEvent)
		{
			return;
		}

		if (! _visible)
			return ;

		if (m_isVisible == false )
		{
			return;
		}

		glPushMatrix();
		


		prepareForRender();

		//////////////////////////////////////////////////////////////////////////
		//------------------------------------------
		// in explorer edtion
		//------------------------------------------

		if (confirmVBO())
		{
			//------------------------------------------
			// Now in VBO list show edition
			//------------------------------------------
			renderInVBO();	
		}
		else
		{

			//------------------------------------------
			// Now in display list show edition
			//------------------------------------------
			renderInDisplayList();
		}


		glPopMatrix();

		resetAfterRender();
	}//Render
	
	void	ObjectNode::WriteNodeToVG(CFile &fp)
	{
		//	����ͷ��Ϣ
		writeObjectHead(fp);
		
		//	����״̬
		writeStateToVG(fp);
		
		//	��������
		writePropertyToVG(fp);
		writePropertyExtraToVG(fp);

		//	����
		writeMaterialToVG(fp);

		//	��������
		writeVertexToVG(fp);

		//	���������в������޲��ʣ�
		writeFaceToVG(fp);

		//	��Ӱ
		writeShadowToVG(fp);

		//	��չ���ݿ�
		writeExtraDataToVG(fp);
		
		//	���㲢��д������С
		long	newpos = fp.GetPosition();
		nodelength = newpos - Posinfile - sizeof(long);
		fp.Seek(Posinfile, CFile::begin);
		fp.Write(&nodelength, sizeof(long));

		//	����
		fp.SeekToEnd();

	}//WriteNodeToVG
	
	//ObjectNode*	ObjectNode::ReadNodeFromVGByType(CFile &fp)
	//{
	//	// Steel������ڵ㳤�Ⱥͽڵ����ͣ������������ɲ�ͬ����
	//	//QuadtreeSceneManager* pQSM = QuadtreeSceneManager::getSingletonPtr();
	//	
	//	unsigned long pos = 0;
	//	int subtype = 0;

	//	long nodelength = 0;	// �ڵ㳤��
	//	fp.Read(&nodelength,sizeof(long));	


	//	int nNodeType = 0;		// �ڵ�����
	//	fp.Read(&nNodeType,sizeof(int));	
	//	nodelength -= sizeof(int);

	//	ObjectNode*		pNewNode	= NULL;
	//	switch (nNodeType)
	//	{

	//		// Objectģ��
	//	case	VG_OBJECT:
	//		pNewNode	= new ObjFrameNode(VERSION_LATEST);
	//		break;

	//	case	VG_OBJECT_BAKED:
	//		pNewNode	= new ObjectNodeBaked(VERSION_LATEST);
	//		break;

	//	case	VG_WATER:
	//		pNewNode	= new WaterNode(VERSION_LATEST);
	//		break;

	//	case	VG_GLASS:
	//		pNewNode	= new GlassNode(VERSION_LATEST);
	//		break;

	//	case	VG_METAL:
	//		pNewNode	= new MetalNode(VERSION_LATEST);
	//		break;

	//	case	VG_MIRROR:
	//		pNewNode	= new MirrorNode(VERSION_LATEST);
	//		break;

	//	case	VG_BRIGHTFLOOR:
	//		pNewNode	= new BrightfloorNode(VERSION_LATEST);
	//		break;

	//	case	VG_AVINODE:
	//		pNewNode	= new AviNode(VERSION_LATEST);
	//		break;

	//	case	VG_SKYBOX:
	//		pNewNode	= new SkyboxNode(VERSION_LATEST);
	//		break;

	//	case	VG_DYNAMICTEXNODE:
	//		pNewNode    = new DynamicTexNode(VERSION_LATEST);
	//		break;

	//	default:
	//		return	NULL;	
	//		break;
	//	}//switch(nNodeType)

	//	char *pNodeDataBuffer = NULL;

	//	pNodeDataBuffer = new char[nodelength];
	//	fp.Read(pNodeDataBuffer , nodelength);

	//	pNewNode->nodelength = nodelength;


	//	// Stee2�����ڴ��ʽ���ɽڵ���������
	//	pNewNode->PositDataToNode(pNodeDataBuffer);
	//	
	//	
	//	return pNewNode;

	//}
	
	void	ObjectNode::ReadNodeFromVG(CFile &fp)
	{
		//	����ͷ��Ϣ
		readObjectHead(fp);

		//	����״̬
		readStateFromVG(fp);
		readStateExtraFromVG(fp);
		
		//	�������ԣ� �����ٶ�ͷ��Ϣ����Ϊ��������֮ǰ�Ѿ�����
		readPropertyFromVG(fp);
		readPropertyExtraFromVG(fp);

		//	����
		readMaterialFromVG(fp);

		//	��������
		readVertexFromVG(fp);

		//	���������в������޲��ʣ�
		readFaceFromVG(fp);

		//	��Ӱ
		readShadowFromVG(fp);

		//	��չ���ݿ�
		readExtraDataFromVG(fp);

		//	���㲢��д������С
		/*long	newpos = fp.GetPosition();
		nodelength = newpos - Posinfile - sizeof(long);
		fp.Seek(Posinfile, CFile::begin);
		fp.Read(&nodelength, sizeof(long));*/

		//	����
		//fp.SeekToEnd();



	}//ReadNodeFromVG

	unsigned long ObjectNode::PositDataToNode( char *data )
	{
		unsigned long pos = 0;
		
		pos	+= readStateFromVG(data+pos);

		nodelength -= pos;

		pos	+= readFromMemery(data+pos);
		
		UpdatePos();
		
		return pos;
	}//PositDataToNode

	
	void	ObjectNode::translate( Vector3 offset )
	{
		if ( offset == Vector3::ZERO )
		{
			return;
		}



		mapVertexBuffer();


		// ƽ�ƶ��㽻������
		translateVetexArray(offset);

		// ƽ�ƴ�����������
		translatePointArray(offset);



		unmapVertexBuffer();


		ComputeBoundBox();

		UpdatePos();

		if ( _multiplyMatrix != NULL )
		{
			vgKernel::Mat4 tran;
			vgKernel::Mat4 tmp;

			vgKernel::Math::createTranslationMatrix(offset.x, offset.y, offset.z, tran);
			vgKernel::Math::multiplyMatrix(tran, *_multiplyMatrix, tmp);
			*_multiplyMatrix = tmp;
		}

		this->notifyOberversOnEvent(vgKernel::VG_OBS_PROPCHANGED, NULL);

	}

	void	ObjectNode::scale( Vector3 scale )
	{

		mapVertexBuffer();

		Vector3		centerVec(m_BoundryBox.getCenter());

		// ���Ŷ��㽻������
		scaleVetexArray(scale, centerVec);

		// ���Ŵ�����������
		scalePointArray(scale, centerVec);


		unmapVertexBuffer();

		ComputeBoundBox();
		UpdatePos();

		this->notifyOberversOnEvent(vgKernel::VG_OBS_PROPCHANGED, NULL);

	}



	
	bool	ObjectNode::testIntersection( vgMath::Ray* ray )
	{
		bool test_result = false;
		if (m_isVisible == false )
		{
			return false;
		}

		if (!confirmVBO())
		{
			return false;
		}

		// vertex mapping
		mapVertexBuffer();

		// face index mapping
		mapFaceIndex();


		// ����
		for ( int i = 0 ; i < m_numOfMat ; ++ i )
		{
			for ( int j = 0 ; j < pMatRender[i].m_numOfFace ; ++ j)
			{
				int vecindex1 = pMatRender[i].pFace[ j ].x;
				int vecindex2 = pMatRender[i].pFace[ j ].y;
				int vecindex3 = pMatRender[i].pFace[ j ].z;

				Vector3 p1	= getVertexElementPosition(vecindex1);
				Vector3 p2	= getVertexElementPosition(vecindex2);
				Vector3 p3	= getVertexElementPosition(vecindex3);

				std::pair<bool, float> result = vgMath::Math::intersects( *ray , p1 , p2 , p3 );

				if( result.first == true )
				{
					test_result = true;
					goto TEST_INTERSECTION_END;
				}
			}
		}
		
#if _DEBUG
		VG_CHECK_OPENGL_ERROR();
#endif



TEST_INTERSECTION_END:


		unmapVertexBuffer();
		unmapFaceIndex();

		return test_result;
	}
	bool	ObjectNode::testIntersection( const vgCore::ImageRect& rect , 
		int *viewport , GLdouble* proj_mat , GLdouble* model_mat )
	{
		bool test_result = false;
		if (m_isVisible == false )
		{
			return false;
		}

		if (!confirmVBO())
		{
			return false;
		}

		// vertex mapping
		mapVertexBuffer();



		GLdouble winx;
		GLdouble winy;
		GLdouble winz;

		assert( rect.isSafe() );

		Vector3		tempVec;
		// ����
		for ( int i = 0 ; i < m_numOfVerts ; ++ i )
		{
			tempVec	= getVertexElementPosition(i);

			gluProject( tempVec.x , tempVec.y, tempVec.z, 
				model_mat , proj_mat , viewport , &winx , &winy , &winz );

			if ( rect.testInside( vgCore::ImagePos(winx,winy)) == true )
			{
				// ��������
				test_result = true;
				goto TEST_INTERSECTION_END2;
			}
		}

#if _DEBUG
		VG_CHECK_OPENGL_ERROR();
#endif


TEST_INTERSECTION_END2:



		unmapVertexBuffer();


		return test_result;
	}


	
	
	//	WriteNodeToVG��ReadNodeFromVG��ִ��ϸ��
	void	ObjectNode::writeObjectHead(CFile &fp)
	{
		Posinfile = fp.GetPosition();	
		fp.Write(&nodelength, sizeof(long));

		fp.Write(&TYPE , sizeof(int));
	}

	void	ObjectNode::writePropertyToVG(CFile &fp)
	{
		fp.Write(&m_BoundryBox , sizeof(BoundingBox));
		fp.Write(&m_Position , sizeof(Vector3));
		fp.Write(&TYPE , sizeof(int));
		fp.Write(&m_numOfFaces , sizeof(int));
		
		fp.Write(&m_name , sizeof(m_name));
		fp.Write(&m_RenderTypeInt , sizeof(int));
	}

	void	ObjectNode::writeStateToVG(CFile &fp)
	{
		fp.Write(&m_id , sizeof(int));                 //д�ýڵ��ID		
		fp.Write(m_LayerName , 20);
		fp.Write(&m_isVisible, sizeof(bool));
	}

	void	ObjectNode::writeMaterialToVG(CFile &fp)
	{
		fp.Write(&m_numOfMat , sizeof(int));
		for (int i= 0 ; i < m_numOfMat ; i++)
		{
			fp.Write(&pMats[i].materialinfo, sizeof(MaterialInfoStruct));
			pMats[i].fileoffset = fp.GetPosition();
		}
	}

	void	ObjectNode::writeVertexToVG(CFile &fp)
	{

		mapVertexBuffer();


		fp.Write(&m_numOfVerts, sizeof(int));
		fp.Write(pVertex , sizeof(VertexElementStruct) * m_numOfVerts);


		unmapVertexBuffer();

	}

	void	ObjectNode::writeFaceToVG(CFile &fp)
	{

		mapFaceIndex();

		
		//	�в�����
		for (int i = 0; i < m_numOfMat; i++)
		{
			fp.Write(&pMatRender[i].b_hastexture, sizeof(bool));
			fp.Write(&pMatRender[i].m_numOfFace, sizeof(int));
			fp.Write(pMatRender[i].pFace, sizeof(FaceIndex) * pMatRender[i].m_numOfFace);
		}

		//	�޲�����
		fp.Write(&m_numOfNoMat , sizeof(int));
		fp.Write(pFaceNoMat , sizeof(FaceIndex) * m_numOfNoMat);


		unmapFaceIndex();

	}

	void	ObjectNode::writeShadowToVG(CFile &fp)
	{
		//	�淨�� ����
		fp.Write(pNormalOfFaces, sizeof(Vector3) * m_numOfFaces);

		//	�������� ����
		fp.Write(&m_numOfVertexs , sizeof(long));           
		fp.Write(pVert, sizeof(Vector3) * m_numOfVertexs);

		//	��Ӱ������ ����
		fp.Write(pShadowFaces, sizeof(ShadowFaceIndex) * m_numOfFaces);
	}

	
	void	ObjectNode::readObjectHead(CFile &fp)
	{
		fp.Read(&nodelength, sizeof(long));

		fp.Read(&TYPE , sizeof(int));
	}
	void	ObjectNode::readPropertyFromVG(CFile &fp)
	{
		fp.Read(&m_BoundryBox , sizeof(BoundingBox));
		fp.Read(&m_Position , sizeof(Vector3));
		fp.Read(&TYPE , sizeof(int));
		fp.Read(&m_numOfFaces , sizeof(int));

		fp.Read(&m_name , sizeof(m_name));
		fp.Read(&m_RenderTypeInt , sizeof(int));
	}

	void	ObjectNode::readStateFromVG(CFile &fp)
	{
		fp.Read(&m_id , sizeof(int));                 //д�ýڵ��ID		
		fp.Read(m_LayerName , 20);
		fp.Read(&m_isVisible, sizeof(bool));
	}

	void	ObjectNode::readMaterialFromVG(CFile &fp)
	{
		fp.Read(&m_numOfMat , sizeof(int));
		pMats = new MaterialStruct[m_numOfMat];
		for (int i= 0 ; i < m_numOfMat ; i++)
		{
			fp.Read(&pMats[i].materialinfo, sizeof(MaterialInfoStruct));
			pMats[i].fileoffset = fp.GetPosition();

			try
			{
				vgCore::TextureManager &mgr = vgCore::TextureManager::getSingleton();

				string aa(pMats[i].materialinfo.strFile);

				//QuadtreeSceneManager &scenemgr = QuadtreeSceneManager::getSingleton();

				pMats[i].pTexture = mgr.getTexturePtr(aa);

			}
			catch (vgCore::Exception& e)
			{
				MessageBox( NULL , e.getFullDescription().c_str() ,"����", MB_OK);
			}

		}
	}

	void	ObjectNode::readVertexFromVG(CFile &fp)
	{
		fp.Read(&m_numOfVerts, sizeof(int));
		pVertex = new VertexElementStruct[m_numOfVerts];
		fp.Read(pVertex , sizeof(VertexElementStruct) * m_numOfVerts);
	}

	void	ObjectNode::readFaceFromVG(CFile &fp)
	{		
		pMatRender = new ObjectRenderControl[m_numOfMat];

		//	�в�����
		for (int i = 0; i < m_numOfMat; i++)
		{
			fp.Read(&pMatRender[i].b_hastexture, sizeof(bool));
			fp.Read(&pMatRender[i].m_numOfFace, sizeof(int));
			
			pMatRender[i].pFace = new FaceIndex[pMatRender[i].m_numOfFace];
			fp.Read(pMatRender[i].pFace, sizeof(FaceIndex) * pMatRender[i].m_numOfFace);
		}

		//	�޲�����
		fp.Read(&m_numOfNoMat , sizeof(int));
		pFaceNoMat = new FaceIndex[m_numOfNoMat];
		fp.Read(pFaceNoMat , sizeof(FaceIndex) * m_numOfNoMat);
	}

	void	ObjectNode::readShadowFromVG(CFile &fp)
	{
		//	�淨�� ����
		pNormalOfFaces = new Vector3[m_numOfFaces];
		fp.Read(pNormalOfFaces, sizeof(Vector3) * m_numOfFaces);

		//	�������� ����
		fp.Read(&m_numOfVertexs , sizeof(long));  
		pVert = new Vector3[m_numOfVertexs];
		fp.Read(pVert, sizeof(Vector3) * m_numOfVertexs);

		//	��Ӱ������ ����
		pShadowFaces = new ShadowFaceIndex[m_numOfFaces];
		fp.Read(pShadowFaces, sizeof(ShadowFaceIndex) * m_numOfFaces);
	}

	
	long	ObjectNode::readStateFromVG(char *pBytes)
	{
		long	posRelative	= 0;

		m_id = *(int *)(pBytes + posRelative);
		posRelative += sizeof(int);

		memcpy(m_LayerName, pBytes + posRelative, sizeof(m_LayerName) );
		posRelative += sizeof(m_LayerName);

		m_isVisible = *(bool *)(pBytes + posRelative);
		posRelative += sizeof(bool);

		return	posRelative;
	}


	
	//	Translate��Rotate��Scale��ִ��ϸ��
	void	ObjectNode::translateVetexArray( Vector3 offset )
	{
		for (int i =0 ; i < m_numOfVerts ; i++)
		{
			pVertex[i].positionX += offset.x;
			pVertex[i].positionY += offset.y;
			pVertex[i].positionZ += offset.z;
		}
	}

	void	ObjectNode::translatePointArray( Vector3 offset )
	{
		for (int i = 0; i < m_numOfVertexs; i++)
		{
			pVert[i]  = pVert[i] + offset;
		}
	}
	void	ObjectNode::translatePoint( Vector3 offset , Vector3* pVert, int n/*=1*/)
	{
		for (int index =0 ; index < n; index++)
		{
			*(pVert+index) = *(pVert+index) + offset;
		}
	}
	
	void	ObjectNode::rotateVetexArray( short axis, float angle ,Vector3 centerVec  )
	{
		for(int i =0 ; i < m_numOfVerts ; i++)
		{
			switch(axis)
			{
			case 0:
				RotatePoint(centerVec.y, centerVec.z, angle, 
					&pVertex[i].positionY, &pVertex[i].positionZ);
				break;

			case 1:
				RotatePoint(centerVec.z, centerVec.x, angle, 
					&pVertex[i].positionZ, &pVertex[i].positionX);
				break;

			default:
				RotatePoint(centerVec.x, centerVec.y, angle, 
					&pVertex[i].positionX, &pVertex[i].positionY);
				break;
			}//switch(axis)
		}//for(m_numOfVerts)	
	}

	void	ObjectNode::rotatePointArray( short axis, float angle ,Vector3 centerVec )
	{
		for(int i =0 ; i < m_numOfVertexs ; i++)
		{
			switch(axis)
			{
			case 0:
				RotatePoint(centerVec.y, centerVec.z, angle, &pVert[i].y, &pVert[i].z);
				break;

			case 1:
				RotatePoint(centerVec.z, centerVec.x, angle, &pVert[i].z, &pVert[i].x);
				break;

			default:
				RotatePoint(centerVec.x, centerVec.y, angle, &pVert[i].x, &pVert[i].y);
				break;
			}//switch(axis)
		}//for(m_numOfVertexs)	
	}

	void	ObjectNode::rotatePoint( short axis, float angle ,Vector3 centerVec , Vector3* pVert, int n/*=1*/)
	{
		for (int index = 0;index<n; index++)
		{
			switch(axis)
			{
			case 0:
				RotatePoint(centerVec.y, centerVec.z, angle, &(pVert+index)->y, &(pVert+index)->z);
				break;

			case 1:
				RotatePoint(centerVec.z, centerVec.x, angle, &(pVert+index)->z, &(pVert+index)->x);
				break;

			default:
				RotatePoint(centerVec.x, centerVec.y, angle, &(pVert+index)->x, &(pVert+index)->y);
				break;
			}//switch(axis)
		}
	}

	void	ObjectNode::rotateNormalArray(  short axis, float angle )
	{
		for(int i =0 ; i < m_numOfFaces ; i++)
		{
			switch(axis)
			{
			case 0:
				RotatePoint(0, 0, angle, &pNormalOfFaces[i].y, &pNormalOfFaces[i].z);
				break;

			case 1:
				RotatePoint(0, 0, angle, &pNormalOfFaces[i].z, &pNormalOfFaces[i].x);
				break;

			default:
				RotatePoint(0, 0, angle, &pNormalOfFaces[i].x, &pNormalOfFaces[i].y);
				break;
			}//switch(axis)
		}//for(m_numOfFaces)	
	}

	
	void	ObjectNode::scaleVetexArray( Vector3 scale , Vector3 centerVec)
	{
		//Vector3		centerVec(m_BoundryBox.getCenter());
		for (int i =0 ; i < m_numOfVerts ; i++)
		{
			Vector3		tempVec(pVertex[i].positionX, pVertex[i].positionY, pVertex[i].positionZ );

			tempVec		-= centerVec;
			tempVec		*= scale;
			tempVec		+= centerVec;

			pVertex[i].positionX = tempVec.x;
			pVertex[i].positionY = tempVec.y;
			pVertex[i].positionZ = tempVec.z;
		}
	}

	void	ObjectNode::scalePointArray( Vector3 scale , Vector3 centerVec)
	{
		//Vector3		centerVec(m_BoundryBox.getCenter());
		for (int i =0 ; i < m_numOfVertexs; i++)
		{
			pVert[i]	-= centerVec;
			pVert[i]	*= scale;
			pVert[i]	+= centerVec;
		}
	}

	void	ObjectNode::scalePoint( Vector3 scale , Vector3 centerVec, Vector3* pVert, int n/*=1*/)
	{
		for (int index = 0;index<n; index++)
		{
			*(pVert+index)	-= centerVec;
			*(pVert+index)	*= scale;
			*(pVert+index)	+= centerVec;
		}

	}
	
	
	//	Initialize��ִ��ϸ��
	void	ObjectNode::configureOnce()
	{
		//	���ó��ó���
		configureCommonData();

		//	���ý��������ೣ��
		configureExtraData();
	}

	BoundingBox ObjectNode::ComputeBoundBox()
	{

		mapVertexBuffer();


		/*Vector3		tempVec = pVert[0];
		m_BoundryBox	= BoundingBox(tempVec, tempVec);
		for (int i =1 ; i < m_numOfVertexs ; i++)
		{
			tempVec = pVert[i];

			m_BoundryBox.minPoint.makeFloor(tempVec);
			m_BoundryBox.maxPoint.makeCeil(tempVec);
		}*/
		Vector3		tempVec;
		m_BoundryBox	= BoundingBox();
		/*for (int indexFace = 0; indexFace < m_numOfFaces; indexFace ++)
		{
			for (int indexVert = 0; indexVert<3; indexVert ++)
			{
				int index = pShadowFaces[indexFace].vertIndex[indexVert];
				tempVec = pVert[index];

				m_BoundryBox.minPoint.makeFloor(tempVec);
				m_BoundryBox.maxPoint.makeCeil(tempVec);

			}
		}*/
		for (int i =0 ; i < m_numOfVertexs; i++)
		{
			m_BoundryBox.minPoint.makeFloor(pVert[i]);
			m_BoundryBox.maxPoint.makeCeil(pVert[i]);
		}
		/*Vector3		tempVec = getVertexElementPosition(0);
		m_BoundryBox	= BoundingBox(tempVec, tempVec);
		for (int i =1 ; i < m_numOfVerts ; i++)
		{
			tempVec = getVertexElementPosition(i);

			m_BoundryBox.minPoint.makeFloor(tempVec);
			m_BoundryBox.maxPoint.makeCeil(tempVec);
		}*/


		unmapVertexBuffer();


		return m_BoundryBox;
	}

	
	void	ObjectNode::RenderShadows()
	{
		return;
	}

	void	ObjectNode::render_ForShadow()
	{

		//	update_ForShadowEveryTime();
		glCallList( mShadowVolume );

	}

	void	ObjectNode::update_ForShadowEveryTime()
	{
		using namespace vgKernel;

		Plugin* plg = 
			PluginManager::getSingleton().getPluginRef( VGSO_DLL_NAME );


		if ( plg == NULL )
		{
			return;
		}

		VGK_DLL_DECLARE_CLASSFUNC( plg , SolarManager, getLightDirection, func );

		if ( func == NULL )
		{
			return ;
		}

		vgKernel::Vec3* ligi = (*func)();
		if ( ligi == NULL )
		{
			return;
		}

		vgKernel::Vec3 litdir = *ligi;
		Vector3 lightDir(litdir.x, litdir.y, litdir.z); 

		lightDir.normalise();		

		int iface = 0;

		// ���ȸ���ÿ��normalֵ
		for (  iface = 0 ; iface < m_numOfFaces ; ++ iface )
		{
			ShadowFaceIndex& cur_face = pShadowFaces[ iface ];

			assert( cur_face.normal >= 0 );
			Vector3 &curNormal = 
				pNormalOfFaces[ cur_face.normal ];

			if ( curNormal.dotProduct( lightDir ) <= 0 )
			{
				cur_face.faceVisible = true;
			}
			else
			{
				// �ڰ�ʱ��ʲôҲ��Ͷ�䡣
				cur_face.faceVisible = false;
			}
		}

		// ����visible�ĸ�ֵ
		for ( iface = 0 ; iface < m_numOfFaces ; ++ iface )
		{
			ShadowFaceIndex& cur_face = pShadowFaces[ iface ];

			//���-1ʱ

			if ( cur_face.faceVisible == true )
			{
				if ( cur_face.p1p2AdjFaceIndex != -1 )
				{
					const bool& linevisible =
						pShadowFaces[ cur_face.p1p2AdjFaceIndex ].faceVisible;
					cur_face.p1p2Visible = (linevisible == false ? true : false );
				}
				else
				{
					cur_face.p1p2Visible = true;
				}

				if ( cur_face.p2p3AdjFaceIndex != -1 )
				{
					const bool& linevisible =
						pShadowFaces[ cur_face.p2p3AdjFaceIndex ].faceVisible;
					cur_face.p2p3Visible = (linevisible == false ? true : false );
				}
				else
				{
					cur_face.p2p3Visible = true;
				}

				if ( cur_face.p3p1AdjFaceIndex != -1 )
				{
					const bool& linevisible =
						pShadowFaces[ cur_face.p3p1AdjFaceIndex ].faceVisible;
					cur_face.p3p1Visible = (linevisible == false ? true : false );
				}
				else
				{
					cur_face.p3p1Visible = true;
				}

			} // end of cur_face.faceVisible == true

		}

		this->buildShadowVolume();		
	}

	
	void	ObjectNode::RenderWithoutTex( float r, float g, float b )
	{
		//prepareForRender();	
		if (!confirmVBO())
		{
			if ( _readyForDisplayListRenderingWithoutTex == false )
			{
				_displayListWithoutTexID = glGenLists( 1 );
				assert( _displayListWithoutTexID != 0 );

				glNewList( _displayListWithoutTexID, GL_COMPILE );
				{
					renderWithoutTex();
				}
				glEndList();

				VG_CHECK_OPENGL_ERROR();

				_readyForDisplayListRenderingWithoutTex = true;
			}

			if ( _displayListWithoutTexID == 0 )
			{
				return;
			}

			glCallList( _displayListWithoutTexID );

			return;

		}//DISPLAY LIST

		else
		{
			//assert(0 && "�˴���Ҫ��!");

			if (m_isVisible == false )
			{
				return;
			}

			if (!(_isReadyForVBORender||_readyForDisplayListRendering))
			{
				Initialize();
			}

			prepareForRender();


			assert( _vertexBufferObjectID != -1 );
			glBindBufferARB( GL_ARRAY_BUFFER_ARB, _vertexBufferObjectID );

			glPushMatrix();

			for (int i = 0; i < m_numOfMat; i++)
			{
				if (pMatRender[i].b_hastexture == true)
				{
					glDisable( GL_TEXTURE_2D );
					assert( pMatRender[i]._elementBufferObjectID != -1 );
					glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, pMatRender[i]._elementBufferObjectID );

					glTexCoordPointer( 2, GL_FLOAT, m_nVertexStride, BUFFER_OFFSET(0) );
					glNormalPointer( GL_FLOAT, m_nVertexStride, BUFFER_OFFSET(m_nOffsetForNormals) );
					glVertexPointer( 3, GL_FLOAT, m_nVertexStride, BUFFER_OFFSET(m_nOffsetForVertices) );

					glEnableClientState( GL_TEXTURE_COORD_ARRAY );
					glEnableClientState( GL_NORMAL_ARRAY );
					glEnableClientState( GL_VERTEX_ARRAY );
					glEnableClientState( GL_INDEX_ARRAY );

					//glColor3d(1, 1, 1);	
					glColor3f(0.8f, 0.8f, 0.8f);		


					glDrawElements( GL_TRIANGLES, pMatRender[i].m_numOfFace*3, GL_UNSIGNED_INT, BUFFER_OFFSET(0));


					glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, NULL );
				}
				else
				{
					assert( pMatRender[i]._elementBufferObjectID != -1);
					glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, pMatRender[i]._elementBufferObjectID );

					glTexCoordPointer( 2, GL_FLOAT, m_nVertexStride, BUFFER_OFFSET(0) );
					glNormalPointer( GL_FLOAT, m_nVertexStride, BUFFER_OFFSET(m_nOffsetForNormals) );
					glVertexPointer( 3, GL_FLOAT, m_nVertexStride, BUFFER_OFFSET(m_nOffsetForVertices) );

					glEnableClientState( GL_TEXTURE_COORD_ARRAY );
					glEnableClientState( GL_NORMAL_ARRAY );
					glEnableClientState( GL_VERTEX_ARRAY );
					glEnableClientState( GL_INDEX_ARRAY );

					glDisable(GL_TEXTURE_2D);
					glColor3f (float(pMats[i].materialinfo.color[0]) / 255 ,float(pMats[i].materialinfo.color[1]) / 255, float(pMats[i].materialinfo.color[2])/255);

					glDrawElements( GL_TRIANGLES,pMatRender[i].m_numOfFace * 3, GL_UNSIGNED_INT, BUFFER_OFFSET(0));

					glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, NULL );

				}	
			}

			if (m_numOfNoMat != 0)
			{
				assert( _elementBufferNoMat != -1 );
				glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, _elementBufferNoMat );

				glTexCoordPointer( 2, GL_FLOAT, m_nVertexStride, BUFFER_OFFSET(0) );
				glNormalPointer( GL_FLOAT, m_nVertexStride, BUFFER_OFFSET(m_nOffsetForNormals) );
				glVertexPointer( 3, GL_FLOAT, m_nVertexStride, BUFFER_OFFSET(m_nOffsetForVertices) );

				glEnableClientState( GL_TEXTURE_COORD_ARRAY );
				glEnableClientState( GL_NORMAL_ARRAY );
				glEnableClientState( GL_VERTEX_ARRAY );
				glEnableClientState( GL_INDEX_ARRAY );

				glDisable(GL_TEXTURE_2D);
				//glColor3f(0.5, 0.5, 0.5);	
				glColor3f(0.8f, 0.8f, 0.8f);		

				glDrawElements( GL_TRIANGLES, m_numOfNoMat*3, GL_UNSIGNED_INT, BUFFER_OFFSET(0));

				glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, NULL );
			}

			glPopMatrix();

			glBindBufferARB( GL_ARRAY_BUFFER_ARB, NULL );

		}//VBO

	}

	

	//	��mod
	void	ObjectNode::ReadObjectFromMod(CFile&	oFile)
	{
		m_bReadMod = true;

		char			*pNodeDataBuffer = NULL;

		oFile.Read( &nodelength , sizeof(unsigned int) );
		assert( nodelength!=0 );
		
		pNodeDataBuffer = new char[nodelength];
		oFile.Read(pNodeDataBuffer , nodelength);

		readFromMemery(pNodeDataBuffer);

		if(pNodeDataBuffer)
		{
			delete[]	pNodeDataBuffer;
			pNodeDataBuffer	= NULL;
		}
	}
	long	ObjectNode::readFromMemery(char *pBytes/* = NULL*/, int _length)
	{
		if( !pBytes )
			return 0;

		long pos = 0;
		
		//	part1:	��ȡ����
		pos	+= readProperty(pBytes+pos);
		pos	+= readPropertyExtra(pBytes+pos);

		//	part2:	��ȡ����
		pos	+= readMaterial(pBytes+pos);
		
		//	part3:	��ȡ��
		pos	+= readVetex(pBytes+pos);
		
		//	part4:	��ȡ�棨�в��ʵ����޲��ʵģ�
		pos	+= readFace(pBytes+pos);
		
		//	part5:	��ȡ��Ӱ
		pos	+= readShadow(pBytes+pos);

		
		//	part6:	��������
		if (pos < nodelength)
		{
			nodelength -= pos;
			pos	+= readExtraData(pBytes+pos);
		}


		glBindBufferARB( GL_ARRAY_BUFFER_ARB, NULL );
		glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, NULL );
#if _DEBUG
		VG_CHECK_OPENGL_ERROR();
#endif


		return	pos;
	}



	int		ObjectNode::readRenderControl(char *pBytes , ObjectRenderControl &RenderControl )
	{
		int datapos = 0;

		RenderControl.b_hastexture = *(bool *)(pBytes + datapos);

		datapos += sizeof(bool);

		RenderControl.m_numOfFace = *(int *)(pBytes + datapos);

		datapos += sizeof(int);

		RenderControl.pFace = new FaceIndex[RenderControl.m_numOfFace];

		memcpy(RenderControl.pFace , pBytes+datapos , sizeof(FaceIndex) * RenderControl.m_numOfFace);

		datapos += sizeof(FaceIndex) * RenderControl.m_numOfFace;

		//generateFace2( RenderControl);

		return datapos;
	}

	
	//	Render��ִ��ϸ��
	void	ObjectNode::renderToDisplaylistOnce()
	{
		assert(0);

		if (m_isVisible)
		{
			glPushMatrix();

			for (int i = 0; i < m_numOfMat; i++)
			{
				if (pMatRender[i].b_hastexture == true)
				{
					if(m_RenderTypeInt == 1||m_RenderTypeInt == 2)
					{
						glEnable(GL_ALPHA_TEST);
						glAlphaFunc(GL_GREATER, 0);
						glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
						glEnable(GL_BLEND);
					}

					glEnableClientState( GL_TEXTURE_COORD_ARRAY );
					glEnableClientState( GL_NORMAL_ARRAY );
					glEnableClientState( GL_VERTEX_ARRAY );
					glEnableClientState( GL_INDEX_ARRAY );

					//glColor3d(1, 1, 1);		
					glColor3f(0.8f, 0.8f, 0.8f);		
					
					glEnable(GL_TEXTURE_2D);

					if ( pMats[i].pTexture.isNull() == false )
					{
						pMats[i].pTexture->useBind();
					}

					glInterleavedArrays(GL_T2F_N3F_V3F, 0 , pVertex);
					//glNormalPointer( GL_FLOAT, m_nVertexStride, &pVertex[0].normalX);
					//glVertexPointer( 3, GL_FLOAT, m_nVertexStride,  &pVertex[0].positionX);
					//glTexCoordPointer( 2, GL_FLOAT, m_nVertexStride, &pVertex[0].texCoorS );

					glDrawElements(GL_TRIANGLES, pMatRender[i].m_numOfFace * 3,
						GL_UNSIGNED_INT,pMatRender[i].pFace);

					glDisable(GL_TEXTURE_2D);
					glDisableClientState( GL_TEXTURE_COORD_ARRAY );
					glDisableClientState( GL_NORMAL_ARRAY );
					glDisableClientState( GL_VERTEX_ARRAY );
					glDisableClientState( GL_INDEX_ARRAY );

					if (m_RenderTypeInt == 1||m_RenderTypeInt == 2)
					{
						glDisable(GL_BLEND);
						glDisable(GL_ALPHA_TEST);
					}


				}
				else
				{
					glEnableClientState( GL_TEXTURE_COORD_ARRAY );
					glEnableClientState( GL_NORMAL_ARRAY );
					glEnableClientState( GL_VERTEX_ARRAY );
					glEnableClientState( GL_INDEX_ARRAY );

					glDisable(GL_TEXTURE_2D);

					glColor3f (float(pMats[i].materialinfo.color[0]) / 255 ,float(pMats[i].materialinfo.color[1]) / 255, float(pMats[i].materialinfo.color[2])/255);

					glInterleavedArrays(GL_T2F_N3F_V3F, 0 , pVertex);

					glDrawElements(GL_TRIANGLES, pMatRender[i].m_numOfFace * 3,
						GL_UNSIGNED_INT,pMatRender[i].pFace);

					glDisableClientState( GL_TEXTURE_COORD_ARRAY );
					glDisableClientState( GL_NORMAL_ARRAY );
					glDisableClientState( GL_VERTEX_ARRAY );
					glDisableClientState( GL_INDEX_ARRAY );


				}	
			}
			if (m_numOfNoMat != 0)
			{
				glEnableClientState( GL_TEXTURE_COORD_ARRAY );
				glEnableClientState( GL_NORMAL_ARRAY );
				glEnableClientState( GL_VERTEX_ARRAY );
				glEnableClientState( GL_INDEX_ARRAY );

				glDisable(GL_TEXTURE_2D);
				//glColor3f(0.5, 0.5, 0.5);
				glColor3f(0.8f, 0.8f, 0.8f);		
				
				glInterleavedArrays(GL_T2F_N3F_V3F, 0 , pVertex);			
				glDrawElements(GL_TRIANGLES, m_numOfNoMat * 3,
					GL_UNSIGNED_INT,pFaceNoMat);

				glDisableClientState( GL_TEXTURE_COORD_ARRAY );
				glDisableClientState( GL_NORMAL_ARRAY );
				glDisableClientState( GL_VERTEX_ARRAY );
				glDisableClientState( GL_INDEX_ARRAY );
			}

			glPopMatrix();
		}
	}
	void	ObjectNode::renderInDisplayList()
	{
		if ( _displayListID == 0 )
		{
			assert("You have Forgotten/Failed To Initialize DisplayList Before Rendering!");
			return;
		}
		glCallList( _displayListID );
		//renderToDisplaylistOnce();
	}

	void	ObjectNode::prepareForRender()
	{
		prepareMatrix();
		prepareExtra();
	}
	
	//	VBO
	void	ObjectNode::mapVertexBuffer()
	{
		if ( _vertexBufferObjectID == -1 )
		{
			return;
		}

		GLvoid *vertexbuf = NULL;
		glBindBufferARB( GL_ARRAY_BUFFER_ARB, _vertexBufferObjectID );
		vertexbuf = glMapBufferARB(GL_ARRAY_BUFFER_ARB , GL_READ_WRITE_ARB);
		assert( vertexbuf != NULL) ;

#	if _DEBUG
		VG_CHECK_OPENGL_ERROR();
#	endif

		pVertex = (VertexElementStruct*)vertexbuf;
	}
	void	ObjectNode::unmapVertexBuffer()
	{
		if ( _vertexBufferObjectID == -1 )
		{
			return;
		}

		glUnmapBufferARB(GL_ARRAY_BUFFER_ARB);
		pVertex = NULL;

		glBindBufferARB( GL_ARRAY_BUFFER_ARB, NULL );

#	if _DEBUG
		VG_CHECK_OPENGL_ERROR();
#	endif
	}
	void	ObjectNode::mapFaceIndex()
	{
		for ( int i = 0; i < m_numOfMat; ++ i )
		{
			if(pMatRender[i]._elementBufferObjectID == -1)
			{
				continue;
			}

			GLvoid* facemat = NULL;
			glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, pMatRender[i]._elementBufferObjectID );
			facemat = glMapBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB , GL_READ_WRITE_ARB);
			assert( facemat != NULL) ;

			pMatRender[i].pFace = (FaceIndex*)facemat;
		}

		if (m_numOfNoMat != 0 && _elementBufferNoMat != -1)
		{
			GLvoid* facenomat = NULL;
			glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, _elementBufferNoMat );
			facenomat = glMapBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB , GL_READ_WRITE_ARB);
			assert( facenomat != NULL);

			pFaceNoMat = (FaceIndex*)facenomat;
		}
	}
	void	ObjectNode::unmapFaceIndex()
	{
		for ( int i = 0; i < m_numOfMat; ++ i )
		{
			if(pMatRender[i]._elementBufferObjectID == -1)
			{
				continue;
			}

			glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, pMatRender[i]._elementBufferObjectID );
			glUnmapBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB );

			pMatRender[i].pFace = NULL;
		}

		if (m_numOfNoMat != 0 && _elementBufferNoMat != -1)
		{
			GLvoid* facenomat = NULL;
			glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, _elementBufferNoMat );
			glUnmapBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB);

			pFaceNoMat = NULL;
		}

#if _DEBUG
		VG_CHECK_OPENGL_ERROR();
#endif
	}


	
	void	ObjectNode::sendVetexToVBOOnce()
	{
		int nArrayObjectSize = m_nVertexStride * m_numOfVerts;
		int nParam_ArrayObjectSize = 0;

		if ( nArrayObjectSize != 0 )
		{		
			glGenBuffersARB( 1, &_vertexBufferObjectID );
			glBindBufferARB( GL_ARRAY_BUFFER_ARB, _vertexBufferObjectID );
			glBufferDataARB( GL_ARRAY_BUFFER_ARB, nArrayObjectSize, pVertex, GL_STATIC_DRAW_ARB );

			glGetBufferParameterivARB( GL_ARRAY_BUFFER_ARB, GL_BUFFER_SIZE_ARB, &nParam_ArrayObjectSize );

			if( nParam_ArrayObjectSize <= 0 )
			{
				assert(0);
				::MessageBox(NULL,"glBufferDataARB failed to allocate any memory!",
					"ERROR",MB_OK|MB_ICONEXCLAMATION);
			}

#if _DEBUG
			//VG_CHECK_OPENGL_ERROR();
#endif
		}//if ( nArrayObjectSize != 0 )

	}//sendVetexToVBOOnce

	
	void	ObjectNode::sendFaceWithMaterialToVBOOnce()
	{
		for (int i = 0; i < m_numOfMat ; ++i )
		{
			ObjectRenderControl& RenderControl = pMatRender[i];

			int nArrayObjectSize = sizeof(FaceIndex) * RenderControl.m_numOfFace;
			int nParam_ArrayObjectSize = 0;

			if ( nArrayObjectSize != 0 )
			{
				glGenBuffersARB( 1, &RenderControl._elementBufferObjectID );
				glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, RenderControl._elementBufferObjectID );
				glBufferDataARB( GL_ELEMENT_ARRAY_BUFFER_ARB, nArrayObjectSize, RenderControl.pFace, GL_STATIC_DRAW_ARB );

				glGetBufferParameterivARB( GL_ELEMENT_ARRAY_BUFFER_ARB, GL_BUFFER_SIZE_ARB, &nParam_ArrayObjectSize );

				if( nParam_ArrayObjectSize <= 0 )
				{
					MessageBox(NULL,"glBufferDataARB failed to allocate any memory!",
						"ERROR",MB_OK|MB_ICONEXCLAMATION);
				}
			}
			//////////////////////////////////////////////////////////////////////////2008.6.26
			/*int nArrayObjectSize2 = sizeof(FaceIndex) * RenderControl.m_numOfFace2;
			int nParam_ArrayObjectSize2 = 0;
			if ( nArrayObjectSize2 != 0 )
			{
				glGenBuffersARB( 1, &RenderControl._elementBufferObjectID2 );
				glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, RenderControl._elementBufferObjectID2 );
				glBufferDataARB( GL_ELEMENT_ARRAY_BUFFER_ARB, nArrayObjectSize2, RenderControl.pFace2, GL_STATIC_DRAW_ARB );

				glGetBufferParameterivARB( GL_ELEMENT_ARRAY_BUFFER_ARB, GL_BUFFER_SIZE_ARB, &nParam_ArrayObjectSize2 );

				if( nParam_ArrayObjectSize2 <= 0 )
				{
					MessageBox(NULL,"glBufferDataARB failed to allocate any memory!",
						"ERROR",MB_OK|MB_ICONEXCLAMATION);
				}
			}*/

		}//for ( m_numOfMat )

	}//sendFaceWithMaterialToVBOOnce

	
	void	ObjectNode::sendFaceNoMaterialToVBOOnce()
	{
		int nArrayObjectSize = sizeof(FaceIndex) * m_numOfNoMat;
		int nParam_ArrayObjectSize = 0;

		if ( nArrayObjectSize != 0 )
		{
			glGenBuffersARB( 1, &_elementBufferNoMat );
			glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, _elementBufferNoMat );
			glBufferDataARB( GL_ELEMENT_ARRAY_BUFFER_ARB, nArrayObjectSize, pFaceNoMat, GL_STATIC_DRAW_ARB );

			glGetBufferParameterivARB( GL_ELEMENT_ARRAY_BUFFER_ARB, GL_BUFFER_SIZE_ARB, &nParam_ArrayObjectSize );

			if( nParam_ArrayObjectSize <= 0 )
			{
				MessageBox(NULL,"glBufferDataARB failed to allocate any memory!",
					"ERROR",MB_OK|MB_ICONEXCLAMATION);
			}
		}
	}

	
	void	ObjectNode::sendRenderableDataToVBOOnce()
	{
		//	���㽻��	��������VBO��VeRtex Object Buffer��
		sendVetexToVBOOnce();

		//	�в�����	��������VBO��VeRtex Object Buffer��
		sendFaceWithMaterialToVBOOnce();

		//	�޲�����	��������VBO��VeRtex Object Buffer��
		sendFaceNoMaterialToVBOOnce();

		//	������������
		clearUnvboData();

		_isReadyForVBORender = true;

	}
	
	void	ObjectNode::clearUnvboData()
	{
		if ( pVertex != NULL )
		{
			delete[] pVertex;
			pVertex = NULL;
		}

		for (int i = 0; i < m_numOfMat ; i ++)
		{
			if ( pMatRender[i].pFace != NULL )
			{
				delete[] pMatRender[i].pFace;
				pMatRender[i].pFace = NULL;
			}
			if ( pMatRender[i].pFace2 != NULL )
			{
				delete[] pMatRender[i].pFace2;
				pMatRender[i].pFace2 = NULL;
			}
		}

		if ( pFaceNoMat != NULL )
		{
			//delete[] pFaceNoMat;
			delete	pFaceNoMat;
			pFaceNoMat = NULL;
		}

		clearUnvboDataExtra();
	}


	
	void	ObjectNode::renderInVBO()
	{
		if ( _vertexBufferObjectID == -1 )
		{
			assert("You have Forgotten/Failed To Initialize VBO Before Rendering!");
			return;
		}

		//	ʵʱ����LOD���
		//calLodLevel();

		//	׼����Ⱦ
		glEnable( GL_TEXTURE_2D);
		glBindBufferARB( GL_ARRAY_BUFFER_ARB, _vertexBufferObjectID );
		glPushMatrix();

		//	��Ⱦ�в�����
		renderFaceWithMaterial();

		//	��Ⱦ�޲�����
		renderFaceNoMaterial();

		//	������Ⱦ
		glPopMatrix();
		glBindBufferARB( GL_ARRAY_BUFFER_ARB, NULL );
		glDisable(GL_BLEND);

	}
	void	ObjectNode::renderFaceWithMaterial( )
	{
		glEnableClientState( GL_TEXTURE_COORD_ARRAY );
		glEnableClientState( GL_NORMAL_ARRAY );
		glEnableClientState( GL_VERTEX_ARRAY );
		glEnableClientState( GL_INDEX_ARRAY );

		for (int i = 0; i < m_numOfMat; i++)
		{


			if (pMatRender[i].b_hastexture == true)
			{
				renderFaceWithTexture(i);
			}//	if(�����ļ�)
			else
			{
				renderFaceWithColor(i);
			}// else(��ɫ)

		}//for ( m_numOfMat )

	}//renderFaceWithMaterial
	
	void	ObjectNode::renderFaceWithTexture( int i)
	{
		if ( pMatRender[i]._elementBufferObjectID == -1 )
		{
			return;
		}

		glDisable(GL_BLEND);


		//	part 1��׼����Ⱦ������OpenGL״̬����ĳ��Lod��������
		setOGLTestStates();


		glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, pMatRender[i]._elementBufferObjectID );
		
		
		//	part 2��ָ�������ꡢ��������1������1
		glNormalPointer( GL_FLOAT, m_nVertexStride, BUFFER_OFFSET(m_nOffsetForNormals) );
		glVertexPointer( 3, GL_FLOAT, m_nVertexStride, BUFFER_OFFSET(m_nOffsetForVertices) );

		assignTexture(i);
		

		//	part 3��ָ�������ꡢ��������1������1
	    glDrawElements( GL_TRIANGLES, pMatRender[i].m_numOfFace*3, GL_UNSIGNED_INT, BUFFER_OFFSET(0));
		

		//	part 4����Ⱦ��ʱ����
		resetTexture();
		
		resetOGLTestStates();

		glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, NULL );

	}//renderFaceWithTexture
	
	void	ObjectNode::assignTexture(int i)
	{
		if ( pMats[i].pTexture.isNull() == false )
		{
			pMats[i].pTexture->useBind();
		}
		glTexCoordPointer( 2, GL_FLOAT, m_nVertexStride, BUFFER_OFFSET(0) );
	}

	void	ObjectNode::renderFaceWithColor(int i)
	{
		if ( pMatRender[i]._elementBufferObjectID == -1 )
		{
			return;
		}

		//////////////////////////////////////////////////////////////////////////
		//	part 1��׼����Ⱦ������OpenGL״̬����ĳ��Lod��������
		glDisable(GL_TEXTURE_2D);
		glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, pMatRender[i]._elementBufferObjectID );


		//////////////////////////////////////////////////////////////////////////
		//	part 2��ָ�������ꡢ������ɫ
		glTexCoordPointer( 2, GL_FLOAT, m_nVertexStride, BUFFER_OFFSET(0) );
		glNormalPointer( GL_FLOAT, m_nVertexStride, BUFFER_OFFSET(m_nOffsetForNormals) );
		glVertexPointer( 3, GL_FLOAT, m_nVertexStride, BUFFER_OFFSET(m_nOffsetForVertices) );

		if ( !isColorable() )
		{
			assignColor(i);
		}
		else
		{
			glColor3f(getRgbaQuadRef()->red / 255.0f, 
				getRgbaQuadRef()->green / 255.0f , 
				getRgbaQuadRef()->blue / 255.0f );
		}

		//////////////////////////////////////////////////////////////////////////
		//	part 3��ָ�������ꡢ��������1������1
		glDrawElements( GL_TRIANGLES,pMatRender[i].m_numOfFace * 3, GL_UNSIGNED_INT, BUFFER_OFFSET(0));

		//////////////////////////////////////////////////////////////////////////
		//	part 4����Ⱦ��ʱ����
		glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, NULL );
		glEnable(GL_TEXTURE_2D);
	}

	void	ObjectNode::assignColor(int i)
	{
		glColor3f(	float(pMats[i].materialinfo.color[0]) / 255 ,
					float(pMats[i].materialinfo.color[1]) / 255 , 
					float(pMats[i].materialinfo.color[2]) / 255	);
	}

	void	ObjectNode::renderFaceNoMaterial( )
	{
		if (m_numOfNoMat != 0)
		{
			if ( _elementBufferNoMat == -1 )
			{
				//goto RENDER_END;
				return;
			}

			glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, _elementBufferNoMat );


			glTexCoordPointer( 2, GL_FLOAT, m_nVertexStride, BUFFER_OFFSET(0) );
			glNormalPointer( GL_FLOAT, m_nVertexStride, BUFFER_OFFSET(m_nOffsetForNormals) );
			glVertexPointer( 3, GL_FLOAT, m_nVertexStride, BUFFER_OFFSET(m_nOffsetForVertices) );

			glEnableClientState( GL_TEXTURE_COORD_ARRAY );
			glEnableClientState( GL_NORMAL_ARRAY );
			glEnableClientState( GL_VERTEX_ARRAY );
			glEnableClientState( GL_INDEX_ARRAY );

			glDisable(GL_TEXTURE_2D);

			if ( !isColorable() )
			{
				glColor3f(0.8f, 0.8f, 0.8f);		
			}
			else
			{
				glColor3f(getRgbaQuadRef()->red / 255.0f, 
					getRgbaQuadRef()->green / 255.0f , 
					getRgbaQuadRef()->blue / 255.0f );
			}

			glDrawElements( GL_TRIANGLES, m_numOfNoMat*3, GL_UNSIGNED_INT, BUFFER_OFFSET(0));

			glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, NULL );
		
		}//if (m_numOfNoMat != 0)
	
	}//renderFaceNoMaterial
	
	
	//	��modִ��ϸ�ڣ��ֿ��ʽ������	
	long	ObjectNode::readProperty(char *pBytes)
	{
		long	posRelative	= 0;
		if( m_nVersionMod < VERSION_4 )
		{
			// Part 1: ��Χ��
			Vector3&	maxPoint = m_BoundryBox.maxPoint;
			Vector3&	minPoint = m_BoundryBox.minPoint;

			maxPoint.x = *(float *)(pBytes + posRelative);
			posRelative += sizeof(float);
			minPoint.x = *(float *)(pBytes + posRelative);
			posRelative += sizeof(float);

			maxPoint.y = *(float *)(pBytes + posRelative);
			posRelative += sizeof(float);
			minPoint.y = *(float *)(pBytes + posRelative);
			posRelative += sizeof(float);

			maxPoint.z = *(float *)(pBytes + posRelative);
			posRelative += sizeof(float);
			minPoint.z = *(float *)(pBytes + posRelative);
			posRelative += sizeof(float);


			// Part 2: ���ĵ�
			m_Position	= m_BoundryBox.getCenter();

			// Part 3: ��Ⱦ����
			m_RenderTypeInt = *(int *)(pBytes + posRelative); //����~10.8
			posRelative += sizeof(int);

			// Part 4: �ڵ�����
			memcpy( m_name, pBytes+posRelative, 20);
			posRelative		+= 20;

			// Part 5: ����
			m_numOfFaces = *(long *) (pBytes + posRelative);	
			posRelative += sizeof(long);
		}// �ɰ汾 2008��6��22����ǰ

		else
		{
			// Part 1: ��Χ��
			m_BoundryBox = *(BoundingBox *)(pBytes + posRelative); 
			posRelative += sizeof(BoundingBox);

			// Part 2: ���ĵ�
			m_Position = *(Vector3 *)(pBytes + posRelative); 
			posRelative += sizeof(Vector3);

			// Part 3: �ڵ�����
			TYPE = *(int *)(pBytes + posRelative); 
			posRelative += sizeof(int);

			// Part 4: ����
			m_numOfFaces = *(long *) (pBytes + posRelative);	
			posRelative += sizeof(long);
			assert(m_numOfFaces >=0 );

			// Part 5: �ڵ�����
			memcpy( m_name, pBytes+posRelative, 20);
			posRelative		+= 20;

			// Part 6: ��Ⱦ����
			m_RenderTypeInt = *(int *)(pBytes + posRelative);
			posRelative += sizeof(int);

		}// �°汾 2008��6��23���Ժ�
		return	posRelative;
	}
	long	ObjectNode::readMaterial(char *pBytes)
	{
		long	posRelative	= 0;

		m_numOfMat = *(int *) (pBytes + posRelative);
		assert( m_numOfMat  >= 0 );
		posRelative += sizeof(int);

		pMats = new MaterialStruct [m_numOfMat];
		for (int	i = 0; i <m_numOfMat ; i++)
		{
			pMats[i].materialinfo =  *(MaterialInfoStruct *)(pBytes + posRelative);

			try
			{
				vgCore::TextureManager &mgr = vgCore::TextureManager::getSingleton();

				string aa(pMats[i].materialinfo.strFile);

				//QuadtreeSceneManager &scenemgr = QuadtreeSceneManager::getSingleton();

				pMats[i].pTexture = mgr.getTexturePtr(aa);

			}
			catch (vgCore::Exception& e)
			{
				MessageBox( NULL , e.getFullDescription().c_str() ,"����", MB_OK);
			}

			posRelative += sizeof(MaterialInfoStruct);
		}
		return	posRelative;
	}//readMaterial

	long	ObjectNode::readVetex(char *pBytes)
	{
		long	posRelative	= 0;

		m_numOfVerts = *(int *)(pBytes + posRelative);
		posRelative += sizeof(int);

		if (m_numOfVerts<=0)
		{
			//assert( 0 );
			std::ostringstream oStr;
#if VGK_LANG_CHS
			oStr << "\n\t������:"<< m_name << "������Ϊ�㣡\n";
#else
			oStr << "\n\tFail: "<< m_name << ",has zero vertex! \n";
#endif
			VGK_SHOW( oStr.str() );
		}
		pVertex = new VertexElementStruct[m_numOfVerts];
		
		memcpy(pVertex , pBytes+posRelative , sizeof(VertexElementStruct) * m_numOfVerts);
		posRelative += sizeof(VertexElementStruct) * m_numOfVerts;

		return	posRelative;
	}

	long	ObjectNode::readFace(char	*pBytes)
	{
		long	posRelative	= 0;

		pMatRender = new ObjectRenderControl[m_numOfMat];
		for (int	i = 0; i < m_numOfMat ; i ++)
		{
			posRelative += readRenderControl(pBytes+posRelative , pMatRender[i] );
		}

		m_numOfNoMat = *(int *)(pBytes + posRelative);
		posRelative += sizeof(int);

		pFaceNoMat = new FaceIndex[m_numOfNoMat];

		memcpy(pFaceNoMat , pBytes+posRelative , sizeof(FaceIndex) * m_numOfNoMat);
		posRelative += sizeof(FaceIndex) * m_numOfNoMat;

		return	posRelative;
	}

	long	ObjectNode::readShadow(char	*pBytes)
	{
		long	posRelative	= 0;
		// �����淨��
		pNormalOfFaces = new Vector3[m_numOfFaces];	
		memcpy(pNormalOfFaces , pBytes+posRelative , 
			sizeof(Vector3) * m_numOfFaces);	
		posRelative += sizeof(Vector3) * m_numOfFaces;

		// ʵ�ʵ���Ŀ	
		m_numOfVertexs = *(long *)(pBytes + posRelative);	
		posRelative += sizeof(long);	

		// ����������
		pVert = new Vector3[m_numOfVertexs];	
		memcpy(pVert , pBytes+posRelative , 
			sizeof(Vector3) * m_numOfVertexs);	
		posRelative += sizeof(Vector3) * m_numOfVertexs;

		// ��Ӱ�������ṹ��
		pShadowFaces = new ShadowFaceIndex[m_numOfFaces];
		memcpy(pShadowFaces , pBytes+posRelative , 
			sizeof(ShadowFaceIndex) * m_numOfFaces);	
		posRelative += sizeof(ShadowFaceIndex) * m_numOfFaces;
		return	posRelative;
	}

	long ObjectNode::readExtraData( char *pBytes )
	{
		long	posRelative	= 0;
		return	posRelative;
	}

	
	void	ObjectNode::generateFace2( ObjectRenderControl &Rendercontrol)
	{
		for (int i= 0; i < Rendercontrol.m_numOfFace; i++)
		{
			FaceIndex pp = Rendercontrol.pFace[i];

			Vector3 a	= getVertexElementPosition(pp.x);
			Vector3 b	= getVertexElementPosition(pp.y);
			Vector3 c	= getVertexElementPosition(pp.z);

			float _area = calFaceArea(a,b,c);

			/*int kk = _area / 10;

			if (kk >= 5)
			{
			facenum[5]++;
			}
			else
			facenum[kk]++;*/

			if ( _area > FACEAREA )//��
			{
				Rendercontrol.tempface.push_back( pp );
			}
		}

		int k = Rendercontrol.tempface.size();

		Rendercontrol.m_numOfFace2 = k;

		Rendercontrol.pFace2 = new FaceIndex[k];

		for (int i =0 ; i < k; i++)
		{
			Rendercontrol.pFace2[i].x = Rendercontrol.tempface[i].x;
			Rendercontrol.pFace2[i].y = Rendercontrol.tempface[i].y;
			Rendercontrol.pFace2[i].z = Rendercontrol.tempface[i].z;
		}

		//lod_facenum += Rendercontrol.m_numOfFace2;
	}

	
	
	//	�ӿ�testIntersection�ĵײ�ִ��ϸ��
	Vector3	ObjectNode::getVertexElementPosition(int i)
	{
		Vector3	tempVec(pVertex[i].positionX, 
			pVertex[i].positionY, 
			pVertex[i].positionZ );
		return	tempVec;
	}


	
	
	//	Part 4 :�޲�����Ⱦִ��ϸ��
	void	ObjectNode::renderWithoutTex()
	{
		if (m_isVisible)
		{
			glPushMatrix();

			for (int i = 0; i < m_numOfMat; i++)
			{
				if (pMatRender[i].b_hastexture == true)
				{
					glEnableClientState( GL_TEXTURE_COORD_ARRAY );
					glEnableClientState( GL_NORMAL_ARRAY );
					glEnableClientState( GL_VERTEX_ARRAY );
					glEnableClientState( GL_INDEX_ARRAY );

					glInterleavedArrays(GL_T2F_N3F_V3F, 0 , pVertex);
					glDrawElements(GL_TRIANGLES, pMatRender[i].m_numOfFace * 3,
						GL_UNSIGNED_INT,pMatRender[i].pFace);

					glDisableClientState( GL_TEXTURE_COORD_ARRAY );
					glDisableClientState( GL_NORMAL_ARRAY );
					glDisableClientState( GL_VERTEX_ARRAY );
					glDisableClientState( GL_INDEX_ARRAY );
				}
				else
				{
					glEnableClientState( GL_TEXTURE_COORD_ARRAY );
					glEnableClientState( GL_NORMAL_ARRAY );
					glEnableClientState( GL_VERTEX_ARRAY );
					glEnableClientState( GL_INDEX_ARRAY );

					glInterleavedArrays(GL_T2F_N3F_V3F, 0 , pVertex);

					glDrawElements(GL_TRIANGLES, pMatRender[i].m_numOfFace * 3,
						GL_UNSIGNED_INT,pMatRender[i].pFace);

					glDisableClientState( GL_TEXTURE_COORD_ARRAY );
					glDisableClientState( GL_NORMAL_ARRAY );
					glDisableClientState( GL_VERTEX_ARRAY );
					glDisableClientState( GL_INDEX_ARRAY );

				}	
			}
			if (m_numOfNoMat != 0)
			{
				glEnableClientState( GL_TEXTURE_COORD_ARRAY );
				glEnableClientState( GL_NORMAL_ARRAY );
				glEnableClientState( GL_VERTEX_ARRAY );
				glEnableClientState( GL_INDEX_ARRAY );

				glInterleavedArrays(GL_T2F_N3F_V3F, 0 , pVertex);			
				glDrawElements(GL_TRIANGLES, m_numOfNoMat * 3,
					GL_UNSIGNED_INT,pFaceNoMat);

				glDisableClientState( GL_TEXTURE_COORD_ARRAY );
				glDisableClientState( GL_NORMAL_ARRAY );
				glDisableClientState( GL_VERTEX_ARRAY );
				glDisableClientState( GL_INDEX_ARRAY );
			}

			glPopMatrix();
		}
	}

	
	
	//	Part 5 :����
	void	ObjectNode::WriteDDS(CFile &fp ,const string &filename)
	{

	}

	
	bool	ObjectNode::cloneFromObjectNode( ObjectNode* src_node )
	{

		//*TRACE("clone from ObjectNode node %d \n\n", src_node->m_numOfVertexs);*/
		src_node->Initialize();

		if ( !src_node->confirmVBO() )
		{
			return false;
		}

		// vertex mapping
		src_node->mapVertexBuffer();

		// face index mapping
		src_node->mapFaceIndex();



		m_isSelected		=src_node->m_isSelected;       
		m_isCollideCheck	=src_node->m_isCollideCheck; 
		m_isVisible			=src_node->m_isVisible;       
		m_isDeleted			=src_node->m_isDeleted;      
		m_BoundryBox		= src_node->m_BoundryBox;
		m_Pivot				= src_node->m_Pivot;
		m_prePositon		=src_node->m_prePositon;
		m_Position			=src_node->m_Position;         
		m_PreRotation		= src_node->m_PreRotation;		
		m_Rotation			= src_node->m_Rotation;        
		m_PreScale			=src_node->m_PreScale;
		m_Scale				= src_node->m_Scale;
		m_numOfMat			= src_node->m_numOfMat;
		pMats				= src_node->pMats;
		nodelength			=src_node->nodelength;	
		Posinfile			= src_node->Posinfile;
		m_numOfFaces		= src_node->m_numOfFaces;
		m_numOfVerts		= src_node->m_numOfVerts;

		// Part 3: ��Ⱦ����
		m_RenderTypeInt = src_node->m_RenderTypeInt;

		// Part 4: �ڵ�����
		memcpy( m_name , src_node->m_name , 20 * sizeof( TCHAR ) );

		// Part 5: ����
		m_numOfFaces = src_node->m_numOfFaces;	
		//////////////////////////////////////////////////////////////////////////
		m_numOfMat = src_node->m_numOfMat;

		cloneMtlFromObjectNode(src_node);
		//////////////////////////////////////////////////////////////////////////

		cloneVertexFromObjectNode(src_node);
		//////////////////////////////////////////////////////////////////////////

		pMatRender = new ObjectRenderControl[m_numOfMat];
		for (int	i = 0; i < m_numOfMat ; i ++)
		{
			pMatRender[i].b_hastexture = src_node->pMatRender[i].b_hastexture;

			pMatRender[i].m_numOfFace = src_node->pMatRender[i].m_numOfFace;

			pMatRender[i].pFace = new FaceIndex[pMatRender[i].m_numOfFace];

			memcpy(pMatRender[i].pFace , src_node->pMatRender[i].pFace , sizeof(FaceIndex) * pMatRender[i].m_numOfFace);
		}

		m_numOfNoMat = src_node->m_numOfNoMat;

		pFaceNoMat = new FaceIndex[m_numOfNoMat];

		memcpy(pFaceNoMat , src_node->pFaceNoMat, sizeof(FaceIndex) * m_numOfNoMat);
		//////////////////////////////////////////////////////////////////////////

		// �����淨��
		pNormalOfFaces = new Vector3[m_numOfFaces];	
		memcpy(pNormalOfFaces , src_node->pNormalOfFaces , 
			sizeof(Vector3) * m_numOfFaces);	
		// ʵ�ʵ���Ŀ	
		m_numOfVertexs = src_node->m_numOfVertexs;	
		// ����������
		pVert = new Vector3[m_numOfVertexs];	
		memcpy(pVert , src_node->pVert , 
			sizeof(Vector3) * m_numOfVertexs);	

		// ��Ӱ�����Խṹ��
		pShadowFaces = new ShadowFaceIndex[m_numOfFaces];
		memcpy(pShadowFaces , src_node->pShadowFaces , 
			sizeof(ShadowFaceIndex) * m_numOfFaces);


		//b_renderlod         = src_node->b_renderlod;

		_isReadyForVBORender = false;


		src_node->unmapVertexBuffer();
		src_node->unmapFaceIndex();

		TRACE("numberOfVertex %d %d \n", m_numOfVertexs, src_node->m_numOfVertexs);
		return true;
	}


	void	ObjectNode::buildShadowVolume()
	{
		using namespace vgKernel;

		Plugin* plg = 
			PluginManager::getSingleton().getPluginRef( VGSO_DLL_NAME );


		if ( plg == NULL )
		{
			return;
		}

		VGK_DLL_DECLARE_CLASSFUNC( plg , SolarManager, getLightDirection, func );

		if ( func == NULL )
		{
			return ;
		}

		vgKernel::Vec3* ligi = (*func)();
		if ( ligi == NULL )
		{
			return;
		}

		vgKernel::Vec3 litdir = *ligi;


		if( mShadowVolume != -1 && mShadowVolume != 0 )
		{
			glDeleteLists( mShadowVolume , 1 );
			mShadowVolume = 0;
		}

		mShadowVolume = glGenLists(1);

		glNewList( mShadowVolume , GL_COMPILE );
		{
			glDisable( GL_LIGHTING );


			Vector3 lightDir(litdir.x, litdir.y, litdir.z); 

			lightDir.normalise();

			glPushMatrix();

			// 			float centerx,centery,centerz;
			// 			
			// 			centerx = (m_BoundryBox.maxPoint.x + m_BoundryBox.minPoint.x ) / 2;
			// 			centery = (m_BoundryBox.maxPoint.y + m_BoundryBox.minPoint.y ) / 2;
			// 			centerz = (m_BoundryBox.maxPoint.z + m_BoundryBox.minPoint.z ) / 2;
			// 			
			// 			glTranslatef(centerx , centery , centerz);
			// 			glRotatef(m_Rotation.x , 1, 0, 0);
			// 			glRotatef(m_Rotation.y , 0, 1, 0);
			// 			glRotatef(m_Rotation.z , 0, 0, 1);
			// 			glTranslatef(-centerx , -centery , -centerz);
			// 			
			// 			glTranslatef(centerx , centery , centerz);
			// 			glScalef(m_Scale.x , m_Scale.y , m_Scale.z);
			// 			glTranslatef(-centerx , -centery , -centerz);
			// 			
			// 			glTranslatef(m_Position.x , m_Position.y, m_Position.z);

			//glDisable( GL_TEXTURE_2D );


			//////////////////////////////////////////////////////////////////////////
			for ( int iface = 0 ; iface < m_numOfFaces ; ++ iface )
			{
				ShadowFaceIndex& cur_face = pShadowFaces[ iface ];

				assert( cur_face.p1 >= 0 );
				Vector3 &point1 = pVert[ cur_face.p1 ];
				assert( cur_face.p2 >= 0 );
				Vector3 &point2 = pVert[ cur_face.p2 ];
				assert( cur_face.p3 >= 0 );
				Vector3 &point3 = pVert[ cur_face.p3 ];


				Vector3 farpoint1;
				farpoint1 = point1 + lightDir * 10000;
				Vector3 farpoint2;
				farpoint2 = point2 + lightDir * 10000;
				Vector3 farpoint3;
				farpoint3 = point3 + lightDir * 10000;

				if ( cur_face.faceVisible == true )
				{
					//glBegin( GL_TRIANGLES );
					//{
					//	glVertex3fv( point1.v );
					//	glVertex3fv( point2.v );
					//	glVertex3fv( point3.v );
					//}
					//glEnd();

					//glColor4f( 1 , 0.5 , 1 , 0.3f);

					//glBegin( GL_LINES);
					glBegin( GL_QUADS );
					{
						if ( cur_face.p1p2Visible )
						{
							glVertex3fv( point1.v );
							glVertex3fv( farpoint1.v );
							glVertex3fv( farpoint2.v );
							glVertex3fv( point2.v );
						}
						if ( cur_face.p2p3Visible )
						{

							glVertex3fv( point2.v );
							glVertex3fv( farpoint2.v );
							glVertex3fv( farpoint3.v );
							glVertex3fv( point3.v );
						}
						if ( cur_face.p3p1Visible )
						{
							glVertex3fv( point3.v );
							glVertex3fv( farpoint3.v );
							glVertex3fv( farpoint1.v );
							glVertex3fv( point1.v );
						}
					}
					glEnd();
				}

			}

			glPopMatrix();

		}
		glEndList();


	}




	float	ObjectNode::calFaceArea( Vector3 a, Vector3 b, Vector3 c)
	{
		float ab = a.distance(b);

		float ac = a.distance(c);

		float bc = b.distance(c);

		float s = (ab + ac + bc) / 2.0;

		float _area = sqrt(s*(s-ab)*(s-bc)*(s-ac));

		return _area;
	}

#if 0
	void	ObjectNode::calLodLevel()
	{
		//////////////////////////////////////////////////////////////////////////
		// Ӧ�üӵ��Ĳ������޳����������÷���
		//QuadtreeSceneManager &PP = QuadtreeSceneManager::getSingleton();

		//Vector3 aa(PP.m_pCurrentCamera.g_eye[0], PP.m_pCurrentCamera.g_eye[1], PP.m_pCurrentCamera.g_eye[2]);
		
		vgKernel::Vec3* camPos = vgCam::CamManager::getSingletonPtr()->getCurrentPositionPtr();

		Vector3	aa(camPos->x,camPos->y, camPos->z);

		float dis = aa.distance(m_Position);

		if (aa.y <= lod_eyeheight)
		{
			if (dis >= lod_dis_small)
			{
				b_renderlod = true;
			}	
			else
				b_renderlod = false;
		}
		else
		{
			if (dis >= lod_dis_big)
			{
				b_renderlod = true;
			}
			else
				b_renderlod = false;
		}
	}
#endif
	void	ObjectNode::configureCommonData()
	{
		m_nVertexStride			= sizeof(VertexElementStruct);		
		m_nOffsetForNormals		= sizeof(const float&) * 2;
		m_nOffsetForVertices	= sizeof(const float&) * 5;
	}

	void	ObjectNode::sendRenderableDataToDisplaylistOnce()
	{
		_displayListID = glGenLists( 1 );
		assert( _displayListID != 0 );

		glNewList( _displayListID, GL_COMPILE );
		{
			renderToDisplaylistOnce();
		}
		glEndList();

		VG_CHECK_OPENGL_ERROR();

		_readyForDisplayListRendering = true;
	}

	
	//	�̳�Renderer�����Ľӿ�
	std::pair<bool, float> ObjectNode::testIntersectWithTriangles( 
		const vgKernel::Ray& ray )

	{
		std::pair<bool, float> result;

		bool test_result = false;
		if (m_isVisible == false )
		{
			result.first = false;
			return result;
		}

		if (!confirmVBO())
		{
			result.first = false;
			return result;
		}

		// vertex mapping
		mapVertexBuffer();

		// face index mapping
		mapFaceIndex();


		// ����
		for ( int i = 0 ; i < m_numOfMat ; ++ i )
		{
			for ( int j = 0 ; j < pMatRender[i].m_numOfFace ; ++ j)
			{
				int vecindex1 = pMatRender[i].pFace[ j ].x;
				int vecindex2 = pMatRender[i].pFace[ j ].y;
				int vecindex3 = pMatRender[i].pFace[ j ].z;

				Vector3 p1	= getVertexElementPosition(vecindex1);
				Vector3 p2	= getVertexElementPosition(vecindex2);
				Vector3 p3	= getVertexElementPosition(vecindex3);

				vgKernel::Vec3	p11 = vgKernel::Vec3(p1.x, p1.y, p1.z);
				vgKernel::Vec3	p21 = vgKernel::Vec3(p2.x, p2.y, p2.z);
				vgKernel::Vec3	p31 = vgKernel::Vec3(p3.x, p3.y, p3.z);
				result = vgKernel::Math::intersects( ray , p11 , p21, p31 );

				if( result.first == true )
				{
					TRACE("Test intersection treue %s\n", this->getName().c_str());

					test_result = true;
					goto TEST_INTERSECTION_END;
				}
			}
		}

		for ( int j = 0 ; j < m_numOfNoMat ; ++ j)
		{
			int vecindex1 = pFaceNoMat[ j ].x;
			int vecindex2 = pFaceNoMat[ j ].y;
			int vecindex3 = pFaceNoMat[ j ].z;

			Vector3 p1	= getVertexElementPosition(vecindex1);
			Vector3 p2	= getVertexElementPosition(vecindex2);
			Vector3 p3	= getVertexElementPosition(vecindex3);

			vgKernel::Vec3	p11 = vgKernel::Vec3(p1.x, p1.y, p1.z);
			vgKernel::Vec3	p21 = vgKernel::Vec3(p2.x, p2.y, p2.z);
			vgKernel::Vec3	p31 = vgKernel::Vec3(p3.x, p3.y, p3.z);
			result = vgKernel::Math::intersects( ray , p11 , p21, p31 );

			if( result.first == true )
			{
				TRACE("Test intersection treue %s\n", this->getName().c_str());
				test_result = true;
				goto TEST_INTERSECTION_END;
			}
		}
#if _DEBUG
		VG_CHECK_OPENGL_ERROR();
#endif



TEST_INTERSECTION_END:


		unmapVertexBuffer();
		unmapFaceIndex();

		return result;
	}

	bool ObjectNode::testIntersectWithTriangles( const vgKernel::Segment3& segment )
	{


		bool test_result = false;
		if (m_isVisible == false )
		{
			test_result = false;
			return test_result;
		}

#if VG_VBO_SUPPORT
		if ( _isReadyForVBORender == false )
		{
			sendRenderableDataToVBOOnce();
		}
		if ( _vertexBufferObjectID == -1 )
		{
			test_result = false;
			return test_result;
		}

		// vertex mapping
		mapVertexBuffer();

		// face index mapping
		mapFaceIndex();

#endif
		// ����
		for ( int i = 0 ; i < m_numOfMat ; ++ i )
		{
			for ( int j = 0 ; j < pMatRender[i].m_numOfFace ; ++ j)
			{
				int vecindex1 = pMatRender[i].pFace[ j ].x;
				int vecindex2 = pMatRender[i].pFace[ j ].y;
				int vecindex3 = pMatRender[i].pFace[ j ].z;

				Vector3 p1	= getVertexElementPosition(vecindex1);
				Vector3 p2	= getVertexElementPosition(vecindex2);
				Vector3 p3	= getVertexElementPosition(vecindex3);


				vgKernel::Vec3	p11 = vgKernel::Vec3(p1.x, p1.y, p1.z);
				vgKernel::Vec3	p21 = vgKernel::Vec3(p2.x, p2.y, p2.z);
				vgKernel::Vec3	p31 = vgKernel::Vec3(p3.x, p3.y, p3.z);


				Triangle3 tri( p11,p21,p31);

				IntrSegment3Triangle3 intr(  segment , tri );

				bool result = intr.Test();

				if( result == true )
				{
					//TRACE("Test intersection treue %s\n", this->getName().c_str());

					test_result = true;


					goto TEST_INTERSECTION_END2;
				}
			}
		}

		for ( int j = 0 ; j < m_numOfNoMat ; ++ j)
		{
			int vecindex1 = pFaceNoMat[ j ].x;
			int vecindex2 = pFaceNoMat[ j ].y;
			int vecindex3 = pFaceNoMat[ j ].z;

			Vector3 p1	= getVertexElementPosition(vecindex1);
			Vector3 p2	= getVertexElementPosition(vecindex2);
			Vector3 p3	= getVertexElementPosition(vecindex3);

			vgKernel::Vec3	p11 = vgKernel::Vec3(p1.x, p1.y, p1.z);
			vgKernel::Vec3	p21 = vgKernel::Vec3(p2.x, p2.y, p2.z);
			vgKernel::Vec3	p31 = vgKernel::Vec3(p3.x, p3.y, p3.z);
			Triangle3 tri( p11,p21,p31);

			IntrSegment3Triangle3 intr(  segment , tri );

			bool result = intr.Test();
			if( result == true )
			{
				//TRACE("Test intersection treue %s\n", this->getName().c_str());
				test_result = true;
				goto TEST_INTERSECTION_END2;
			}
		}
#if _DEBUG
		VG_CHECK_OPENGL_ERROR();
#endif



TEST_INTERSECTION_END2:

#if VG_VBO_SUPPORT
		unmapVertexBuffer();
		unmapFaceIndex();
#endif
		return test_result;

	}

	float	ObjectNode::setDistanceToViewer( const vgKernel::Vec3& viewer_pos )
	{
		// 1.����ͨ����Χ�м������ӵ�ľ���.
		float delta[3];

		for ( int i = 0 ; i < 3 ; ++ i )
		{
			float _cur = viewer_pos[i];
			float _max = m_BoundryBox.maxPoint.v[i];
			float _min = m_BoundryBox.maxPoint.v[i];

			if ( _cur > _max )
			{
				delta[i] = _cur - _max;
			}
			else if ( _cur < _min )
			{
				delta[i] = _min - _cur;
			}
			else
			{
				delta[i] = 0.0f;
			}
		}

		_squaredDistanceToViewer = 
			delta[0] * delta[0] + delta[1] * delta[1] + delta[2] * delta[2];

		// 2.����_distanceLod

		return _squaredDistanceToViewer;
	}



	void	ObjectNode::translate(const float& x, const float& y, const float& z)
	{
		translate(Vector3(x,y,z));
	}



	void	ObjectNode::scale(const float& cx, const float& cy, const float& cz, const float& x, const float& y, const float& z)
	{
		if ( _multiplyMatrix != NULL )
		{
			vgKernel::Mat4 tran;
			vgKernel::Mat4 tmp;


			vgKernel::Math::createTranslationMatrix(-cx, -cy, -cz, tran);
			vgKernel::Math::multiplyMatrix(tran, *_multiplyMatrix, tmp);
			*_multiplyMatrix = tmp;

			vgKernel::Math::createScaleMatrix(x, y, z, tran);
			vgKernel::Math::multiplyMatrix(tran, *_multiplyMatrix, tmp);
			*_multiplyMatrix = tmp;

			vgKernel::Math::createTranslationMatrix(cx, cy,cz, tran);
			vgKernel::Math::multiplyMatrix(tran, *_multiplyMatrix, tmp);
			*_multiplyMatrix = tmp;
		}




		mapVertexBuffer();
	

		Vector3		centerVec(cx, cy, cz);
		Vector3		scale(x, y, z);

		// ���Ŷ��㽻������
		scaleVetexArray(scale, centerVec);

		// ���Ŵ�����������
		scalePointArray(scale, centerVec);



		unmapVertexBuffer();


		ComputeBoundBox();
		UpdatePos();

		this->notifyOberversOnEvent(vgKernel::VG_OBS_PROPCHANGED, NULL);

	}

	
	void	ObjectNode::rotateSelf(const float& angle, const float& x, const float& y, const float& z)
	{
		return;

	}
	
	void	ObjectNode::rotate(const float& angle, const float& cx, const float& cy, const float& cz, 
		const float& x, const float& y, const float& z) 
	{

		if ( _multiplyMatrix != NULL )
		{
			vgKernel::Mat4 tran;
			vgKernel::Mat4 tmp;

			vgKernel::Math::createTranslationMatrix(-cx, -cy, -cz, tran);
			vgKernel::Math::multiplyMatrix(tran, *_multiplyMatrix, tmp);
			*_multiplyMatrix = tmp;

			vgKernel::Math::createRotationMatrix(angle, x, y, z, tran);
			vgKernel::Math::multiplyMatrix(tran, *_multiplyMatrix, tmp);
			*_multiplyMatrix = tmp;

			vgKernel::Math::createTranslationMatrix(cx, cy,cz, tran);
			vgKernel::Math::multiplyMatrix(tran, *_multiplyMatrix, tmp);
			*_multiplyMatrix = tmp;
		}



		short axis = -1;
		if(x>1.0e-3)
		{
			axis = 0;
		}
		else if(y>1.0e-3)
		{
			axis = 1;
		}
		else if(z>1.0e-3)
		{
			axis = 2;
		}

		if(axis != -1)
		{


			mapVertexBuffer();
			mapFaceIndex();


			Vector3 &centerVec = Vector3(cx, cy, cz);

			// ��ת���㽻������
			rotateVetexArray( axis, angle, centerVec);

			// ��ת������������
			rotatePointArray( axis, angle, centerVec);

			// ��ת�淨��
			rotateNormalArray( axis, angle);



			unmapVertexBuffer();
			unmapFaceIndex();


			ComputeBoundBox();
			UpdatePos();

			this->notifyOberversOnEvent(vgKernel::VG_OBS_PROPCHANGED, NULL);
		}
	}

	void ObjectNode::setOGLTestStates()
	{
		if(m_RenderTypeInt == 1||m_RenderTypeInt == 2) 
		{
			glAlphaFunc(GL_GREATER, 0.0f);   // �˺����ĵڶ�������Ϊ0ʱ��ʵ�ְ�͸����0.5ʱ��ֻ��ȫ͸��ȫ��͸
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			
			glEnable(GL_ALPHA_TEST);
			glEnable(GL_BLEND);
		}
		
		if (_bOpacity)
		{
			//glBlendFunc(GL_ONE_MINUS_DST_COLOR, GL_ONE_MINUS_SRC_COLOR);
			//glBlendFunc(GL_DST_COLOR, GL_ONE_MINUS_SRC_COLOR);
			//glBlendFunc(GL_DST_COLOR, GL_SRC_COLOR);
			//glBlendFunc(GL_ONE_MINUS_DST_COLOR, GL_SRC_COLOR);
			
			//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			
			//glBlendFunc(GL_DST_COLOR, GL_ONE_MINUS_DST_COLOR);
			glBlendFunc(GL_SRC_COLOR, GL_ONE_MINUS_SRC_COLOR);

			glEnable(GL_BLEND);			// Turn Blending On
			//glDisable(GL_DEPTH_TEST);	// Turn Depth Testing Off
		}

		//��Ч���ȼ������뵭��>��˸>��ɫ�ɱ༭
		if (isDissolveable())
		{
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glEnable(GL_BLEND);			

		}

		if ( isColorable() )
		{
			//------------------------------------------
			// ��ʼ�����ɫ
			//------------------------------------------

			float ratio = getRgbaQuadRef()->alpha / 255.0f;

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

			glColor4f(	getRgbaQuadRef()->red / 255.0f, 
				getRgbaQuadRef()->green / 255.0f , 
				getRgbaQuadRef()->blue / 255.0f,
				getDissolveAlpha());

		}
		else
		{
			glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
			glColor4f( 1.0f, 1.0f, 1.0f, getDissolveAlpha() );	

		}

		if ( isFlickered() )
		{
			float ratio = getCurrentColor().alpha / 255.0f;

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

			glColor4f(	getCurrentColor().red / 255.0f, 
				getCurrentColor().green / 255.0f , 
				getCurrentColor().blue / 255.0f,
				getDissolveAlpha());
		}


	}

	void ObjectNode::resetOGLTestStates()
	{
		if (m_RenderTypeInt == 1||m_RenderTypeInt == 2)
		{
			glDisable(GL_BLEND);
			glDisable(GL_ALPHA_TEST);
		}
		
		if (_bOpacity)
		{
			glDisable(GL_BLEND);		// Turn Blending Off
			//glEnable(GL_DEPTH_TEST);	// Turn Depth Testing On
		}

		if (isDissolveable())
		{
			glDisable(GL_BLEND);
		}
	}

	void ObjectNode::writePropertyExtraToVG( CFile &fp )
	{	
		// Part 1����һ�ε���Ⱦ״̬��״̬��ԭʹ�ã�
		writeOpacityFlag(fp);	
	}

	void ObjectNode::readPropertyExtraFromVG( CFile &fp )
	{
		String	vgVersion = VersionControl::getVgVersionRead();
			//vgGlobal::GlobalFacade::getSingleton().getVgVersionRead();
		if( vgVersion >= String("VRGIS.VERSION 2.16") )
		{		
			// Part 1����һ�ε���Ⱦ״̬��״̬��ԭʹ�ã�
			readOpacityFlag(fp);
		}
	}


	long ObjectNode::readPropertyExtra( char *pBytes )
	{
		long	posRelative	= 0;

		String	vgVersion = VersionControl::getVgVersionRead();
		//vgGlobal::GlobalFacade::getSingleton().getVgVersionRead();
		if( !m_bReadMod && vgVersion >= String("VRGIS.VERSION 2.16") )
		{
			// Part 1����һ�ε���Ⱦ״̬��״̬��ԭʹ�ã�
			posRelative += readOpacityFlag(pBytes);
		}

		return	posRelative;
	}

	// ��д��һ�ε���Ⱦ״̬��״̬��ԭʹ�ã�
	void ObjectNode::writeOpacityFlag(CFile &fp)
	{
		fp.Write( &_bOpacity, sizeof(bool));
	}
	void ObjectNode::readOpacityFlag( CFile &fp )
	{
		fp.Read( &_bOpacity, sizeof(bool));
	}
	long ObjectNode::readOpacityFlag( char *pBytes )
	{
		long	posRelative	= 0;

		_bOpacity = *(bool *)(pBytes + posRelative);
		posRelative += sizeof(bool);

		return	posRelative;
	}

	void ObjectNode::resetAfterRender()
	{
		resetMatrix();
		resetExtra();
	}

	void ObjectNode::renderInShadowMode()
	{
		assert( _multiplyMatrix != NULL );

		glPushMatrix();
		prepareMatrix();

		if ( _multiplyMatrix != NULL )
		{
			glMultMatrixf( _multiplyMatrix->_m );
		}
		
		glCallList( mShadowVolume );
		glPopMatrix();
	}

	void ObjectNode::turnOnShadowCast( const vgKernel::Vec3& light_dir, bool force_to_load )
	{
		ShadowCastObject::turnOnShadowCast( light_dir , force_to_load );

		/*_shadowVector->x = light_dir.x;
		_shadowVector->y = light_dir.y;
		_shadowVector->z = light_dir.z;*/

		assert(_multiplyMatrix == NULL );
		_multiplyMatrix = new vgKernel::Mat4;
		(*_multiplyMatrix) = vgKernel::Mat4::IDENTITY;

		assert( mShadowVolume == 0 || mShadowVolume == -1 );
		update_ForShadowEveryTime();
	}

	void ObjectNode::turnOffShadowCast()
	{
		ShadowCastObject::turnOffShadowCast();

		if ( mShadowVolume != 0 )
		{
			glDeleteLists( mShadowVolume , 1 );
			mShadowVolume = 0;
		}

		if ( _multiplyMatrix != NULL )
		{
			delete _multiplyMatrix;
			_multiplyMatrix = NULL;
		}
	}

	void ObjectNode::onLightDirectionChanged( const vgKernel::Vec3& light_dir )
	{
		ShadowCastObject::onLightDirectionChanged( light_dir );

		assert( _multiplyMatrix != NULL );
		if ( _multiplyMatrix != NULL )
		{
			delete _multiplyMatrix;
			_multiplyMatrix = NULL;
		}

		assert(_multiplyMatrix == NULL );
		_multiplyMatrix = new vgKernel::Mat4;
		(*_multiplyMatrix) = vgKernel::Mat4::IDENTITY;

		if ( mShadowVolume != 0 )
		{
			glDeleteLists( mShadowVolume , 1 );
			mShadowVolume = 0;
		}

		/*_shadowVector->x = light_dir.x;
		_shadowVector->y = light_dir.y;
		_shadowVector->z = light_dir.z;*/

		assert( mShadowVolume == 0 || mShadowVolume == -1 );
		update_ForShadowEveryTime();
	}

	bool ObjectNode::confirmVBO()
	{
#if	VG_VBO_SUPPORT
		if ( _isReadyForVBORender == false )
		{
			sendRenderableDataToVBOOnce();
		}

		if ( _vertexBufferObjectID == -1 )
		{
			return false;
		}
		else
		{
			return true;
		}
#endif
		return false;
	}

	//--class	Trigger--------------------------------------------------
	bool ObjectNode::testTriggered(const vgKernel::Vec3& eyePos )
	{
		vgKernel::Box eyeBox(eyePos, eyePos);
		return getBoundingBox().intersects(eyeBox);
	}

	String& ObjectNode::getTriggerInfo()
	{
		return	_strTriggerInfo;
	}

	void ObjectNode::setTriggerInfo( const String& str )
	{
		_strTriggerInfo = str;
	}

	void ObjectNode::flip( bool x, bool y, bool z )
	{
		if ( !( x | y | z) )
		{
			return;
		}

		mapVertexBuffer();


		for (int i =0 ; i < m_numOfVerts ; i++)
		{
			if (x)
			{
				pVertex[i].positionX *= -1.0f;
			}
			if (y)
			{
				pVertex[i].positionY *= -1.0f;
			}
			if (z)
			{
				pVertex[i].positionZ *= -1.0f;
			}
		}
	
		for (int i = 0; i < m_numOfVertexs; i++)
		{
			Vector3	factor(x,y,z);
			factor *= -1.0f;
			pVert[i] *= factor;
		}

		unmapVertexBuffer();


		ComputeBoundBox();

		UpdatePos();

		this->notifyOberversOnEvent(vgKernel::VG_OBS_PROPCHANGED, NULL);

	}

	bool ObjectNode::cloneMtlFromObjectNode( ObjectNode* src_node )
	{
		pMats = new MaterialStruct [m_numOfMat];
		for (int	i = 0; i <m_numOfMat ; i++)
		{
			pMats[i].materialinfo =  src_node->pMats[i].materialinfo;
#if 1
			try
			{
				vgCore::TextureManager &mgr = vgCore::TextureManager::getSingleton();

				string aa(pMats[i].materialinfo.strFile);

				//QuadtreeSceneManager &scenemgr = QuadtreeSceneManager::getSingleton();

				pMats[i].pTexture = mgr.getTexturePtr(aa);

			}
			catch (vgCore::Exception& e)
			{
				MessageBox( NULL , e.getFullDescription().c_str() ,"����", MB_OK);
			}
#endif
		}

		return true;
	}

	bool ObjectNode::cloneVertexFromObjectNode( ObjectNode* src_node )
	{
		m_numOfVerts = src_node->m_numOfVerts;

		pVertex = new VertexElementStruct[m_numOfVerts];

		memcpy(pVertex , src_node->pVertex, sizeof(VertexElementStruct) * m_numOfVerts);

		return true;
	}

	void ObjectNode::respondAfterTriggered()
	{
		//_strTriggerInfo = "һ��������&���߰˾�ʮ\nһ�����������߰˾�ʮ&\
		//	һ�����������߰˾�ʮ&01234567890123456789&012345678901234&01234567890";
		//vgFont::FontManager::getSingleton().drawString(_strTriggerInfo);
#if 0
		vgFont::FontManager::getSingleton().drawString(getName());
#else

		VGK_DLL_RUN_CLASSFUNC( VGF_DLL_NAME , FontManager, drawString )(getName());

#endif

	}

	bool& ObjectNode::getTriggerFlag()
	{
		return _bSetAsTrigger;
	}

	void ObjectNode::setAsTrigger( const bool& bSet )
	{
		_bSetAsTrigger = bSet;

		if ( getTriggerFlag() )
		{
			setVisible(false);
		}
	}

	void ObjectNode::collisionDetection(vgPhys::CollisionPacket& collisionPacket)
	{
		ObjectNode* curleaf = this;

		// ���CollisionPacket�İ�Χ��û��������İ�Χ���ཻ���򲻿��ܲ�����ײ
		if (!m_enableCollision || ! collisionPacket.boundingBox.intersects(getBoundingBox()))
		{
			return ;
		}

		// ֻ��Ҫ�ó�������Ƭ���䷨�ߣ�����Բ�ռ��������
		// Ȼ���collisionPacketһ�𴫸���д�õ�static����CollisionObject::checkTriangle

		for (int i =0 ; i < curleaf->m_numOfFaces; i++)
		{
			int k0 = curleaf->pShadowFaces[i].vertIndex[0];
			int k1 = curleaf->pShadowFaces[i].vertIndex[1];
			int k2 = curleaf->pShadowFaces[i].vertIndex[2];
			int ni = curleaf->pShadowFaces[i].normal;

			vgKernel::Vec3 p1, p2, p3, normal;

			p1.x = curleaf->pVert[k0].x / collisionPacket.eRadius.x;
			p1.y = curleaf->pVert[k0].y / collisionPacket.eRadius.y;
			p1.z = curleaf->pVert[k0].z / collisionPacket.eRadius.z;

			p2.x = curleaf->pVert[k1].x / collisionPacket.eRadius.x;
			p2.y = curleaf->pVert[k1].y / collisionPacket.eRadius.y;
			p2.z = curleaf->pVert[k1].z / collisionPacket.eRadius.z;

			p3.x = curleaf->pVert[k2].x / collisionPacket.eRadius.x;
			p3.y = curleaf->pVert[k2].y / collisionPacket.eRadius.y;
			p3.z = curleaf->pVert[k2].z / collisionPacket.eRadius.z;

			vgKernel::Vec3 edge1 = p1 - p2;
			vgKernel::Vec3 edge2 = p1 - p3;

			vgKernel::Vec3 newNormal = edge1.crossProduct(edge2);
			if (newNormal.dotProduct(normal) < 0)
			{
				newNormal = -newNormal;
			}
			//   					
			normal = newNormal;
			normal.normalise();

			CollisionObject::checkTriangle(&collisionPacket, p1, p2, p3, normal);
		}
	}

	bool ObjectNode::testPointInObject( const vgKernel::Vec3& pos )
	{
		for (int i =0 ; i < m_numOfFaces; i++)
		{
			int k0 = pShadowFaces[i].vertIndex[0];
			int k1 = pShadowFaces[i].vertIndex[1];
			int k2 = pShadowFaces[i].vertIndex[2];
			int ni = pShadowFaces[i].normal;

			vgKernel::Vec3 p1, p2, p3, normal;

			p1.x = pVert[k0].x;
			p1.y = pVert[k0].y;
			p1.z = pVert[k0].z;


			normal.x = pNormalOfFaces[ni].x;
			normal.y = pNormalOfFaces[ni].y;
			normal.z = pNormalOfFaces[ni].z;

			normal.normalise();

			vgKernel::Plane plane(normal, p1);

			if (plane.getSide(pos) == vgKernel::Plane::POSITIVE_SIDE)
			{
				return false;
			}
		}

		return true;
	}
}//namespace vgMod

