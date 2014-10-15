#include "vgStableHeaders.h"

#include <vgMod/vgObjectNode.h>
#include <vgMod/vgObjectSet.h> 

//t3DObject //////////////////////////////////////////////////////////////////////////
t3DObject::t3DObject(t3DModel* pParentScene /*= NULL*/)
: m_pParentScene(pParentScene)
{
	m_pVertsOriginal =NULL;
	m_pFacesForShadow =NULL;
	m_pTexcoord2D = NULL;m_pVertNormal3D =NULL; m_pPostion3D =NULL; m_pTexcoord2DLit =NULL; 
	m_pFaceNormal3D =NULL; m_pFacesAll =NULL; m_shadowFace =NULL; m_pFaceNoMaterail =NULL;
	m_pKeyFrames = NULL;

	m_nPosRenderType = m_nRenderType = m_nVersionMod = 0;
	m_pFile = NULL;
	m_pTheMtls = NULL;

	m_nNumOfVerts = m_nNumOfFaces = m_nNumOfTVerts = m_numOfFaceNoMaterail = 
		m_nNumOfRenderInterface= 0;

	m_bIsBaked = false;
	m_nObjectType = 0;

	m_nKeyFramesCount = 0;

	initilize();
}

t3DObject::~t3DObject()
{
	reset();
}

bool	t3DObject::reset()
{
	if (m_pVertsOriginal)
	{
		delete[] m_pVertsOriginal;
		m_pVertsOriginal = NULL;
	}
#if 1
	if (m_pFacesForShadow)
	{
		delete[] m_pFacesForShadow;
		m_pFacesForShadow = NULL;
	}
#endif
	if (m_pTexcoord2D)
	{
		delete[] m_pTexcoord2D;
		m_pTexcoord2D = NULL;
	}
	if (m_pVertNormal3D)
	{
		delete[] m_pVertNormal3D;
		m_pVertNormal3D = NULL;
	}

	if (m_pPostion3D)
	{
		delete[] m_pPostion3D;
		m_pPostion3D = NULL;
	}

	if (m_pTexcoord2DLit)
	{
		delete[] m_pTexcoord2DLit;
		m_pTexcoord2DLit = NULL;
	}
#if 1
	if (m_pFaceNormal3D)
	{
		delete[] m_pFaceNormal3D;
		m_pFaceNormal3D = NULL;
	}
#endif
	if (m_pFacesAll)
	{
		delete[] m_pFacesAll;
		m_pFacesAll = NULL;
	}

#if 1
	if (m_shadowFace)
	{
		delete[] m_shadowFace;
		m_shadowFace = NULL;
	}
#endif

	if (m_pFaceNoMaterail)
	{
		delete[] m_pFaceNoMaterail;
		m_pFaceNoMaterail = NULL;
	}

	if (m_pKeyFrames)
	{
		delete[] m_pKeyFrames;
		m_pKeyFrames = NULL;
	}

	vector<tMatandFace*>::iterator iter = m_vecRenderInterface.begin();
	vector<tMatandFace*>::iterator iterEnd = m_vecRenderInterface.end();
	for ( ; iter != iterEnd; iter++)
	{
		if (*iter)
		{
			delete *iter;
			*iter = NULL;
		}
	}

	return	true;
}
bool	t3DObject::initilize()
{
	//m_nVersionMod	= VERSION_MOD_CURRENT;//VERSION_MOD(year, updateTime);
	setStandardType();

	return	true;
}

int	t3DObject::Write(FILE* pFile, MeshMtlList* pMeshList, int nVersionMod)
{
	size_t blockSize = 0;

	m_pFile = pFile;
	m_pTheMtls = pMeshList;
	m_nVersionMod = nVersionMod;

	//	Part 1: ��������
	blockSize += writeHeader();
	
	//	Part 2: ������Ϣ
	blockSize += writeMaterial();
	
	//	Part 3: ������
	blockSize += writeVetex();
	
	//	Part 4: ������
	blockSize += writeFace();
	
	//	Part 5: ��Ӱ����
	blockSize += writeShadow();

	if(	m_nVersionMod	>=	VERSION_MOD(2008, 6) )
	{
		//	Part 6: �ؼ�֡����
		blockSize += writeKeyFrame();
	}

	//  Part 7: �������
	writeChange();

	fseek( m_pFile, 0 , SEEK_END ) ;

	return (int)blockSize;
}

int	t3DObject::writeHeader()
{
	size_t blockSize = 0;

	int		intReserved = 0;
	float	fltReserved = 0.0f;
	char	strReserved[20] ;
	memset( strReserved, 0, 20);
#if 0
	//	part 1: ��С������
	fwrite(&m_minPoint , sizeof(CVector3) , 1, m_pFile);
	fwrite(&m_maxPoint , sizeof(CVector3) , 1, m_pFile);

	//	part 2: ���ĵ�
	CVector3	centerPoint;
	centerPoint.x = (m_minPoint.x + m_maxPoint.x)*0.5f ;
	centerPoint.y = (m_minPoint.y + m_maxPoint.y)*0.5f;
	centerPoint.z = (m_minPoint.z + m_maxPoint.z)*0.5f ;
	fwrite(&centerPoint , sizeof(CVector3) , 1, m_pFile);
#endif
	blockSize += m_boundingBox.write( m_pFile );

	//	part 3: �������ͣ�Ϊƽ̨Ԥ����
	blockSize += fwrite(&m_nObjectType, sizeof(int), 1, m_pFile)*sizeof(int);

	//	part 4: ��������
	blockSize += fwrite(&m_nNumOfFaces , sizeof(int), 1, m_pFile)*sizeof(int);

	//	part 5: ��������
	blockSize += fwrite(m_strName,  sizeof(m_strName)  , 1, m_pFile)*sizeof(m_strName);

	//	part 6: ��Ⱦ����
	m_nPosRenderType = ftell( m_pFile );
	blockSize += fwrite(&m_nRenderType, sizeof(int), 1, m_pFile)*sizeof(int);

	return (int)blockSize;
}

int	t3DObject::writeMaterial( )
{
	size_t blockSize = 0;

	int	k	= 0;

	blockSize += fwrite(&m_nNumOfRenderInterface , sizeof(int), 1, m_pFile)*sizeof(int);

	int  objChunkPos,objChunkLength ;
	for (int i= 0 ; i < m_nNumOfRenderInterface ; i++)
	{
		objChunkPos = ftell( m_pFile );
		for (int j = 0; j < m_pTheMtls->size() ; j++)
		{
			SMtl*savemtl = (*m_pTheMtls)[j] ;
			if ( strcmp( m_vecRenderInterface[i]->strName, savemtl->name ) == 0 )
			{
				if ( savemtl->bHasAlpha == true)
				{
					k = 1;
					// 2007/10/8 LIANGKAI �¼ӣ�
					m_nRenderType = 1;
				}
				else
					k = 0;
				blockSize += fwrite(&k , sizeof(int), 1, m_pFile)*sizeof(int);

				blockSize += fwrite( savemtl->name ,  255  , 1, m_pFile)*255;

				blockSize += fwrite( savemtl->diffuseBitmap ,  255  , 1, m_pFile)*255;

				blockSize += fwrite( savemtl->opacityBitmap,  255  , 1, m_pFile)*255;

				blockSize += fwrite( &savemtl->diffColor , 3 , 1, m_pFile)*3;
				
				unsigned int  tmpID = 0;
				blockSize += fwrite( &tmpID , sizeof( unsigned int ) , 1, m_pFile)*sizeof(int);
				//				break;			
			}// if ( strcmp(m_vecRenderInterface[i].m_strName, mod.pMaterials[j].m_strName) == 0 )

			// 2008��3��6��16:10:03 ���
			string str1(m_vecRenderInterface[i]->strNameBaked);
			string str2(savemtl->name);

			if ( isBakedType()
				&& ( str1 == str2) )
			{
				blockSize += fwrite( savemtl->diffuseBitmap  ,  255  , 1, m_pFile)*255;
				break;
			}			
		}// for (int j = 0; j < mod.numOfMaterials ; j++)

		objChunkLength = ftell(m_pFile) - objChunkPos;
		objChunkLength = objChunkLength;
	}

	return (int)blockSize;
}

int	t3DObject::writeVetex()
{
	size_t blockSize = 0;

	blockSize += fwrite(&m_nNumOfTVerts , sizeof(int) , 1, m_pFile)*sizeof(int);

	for ( int indexOfVertex=0; indexOfVertex < m_nNumOfTVerts ; indexOfVertex++ )
	{
		blockSize += fwrite( &m_pTexcoord2D[ indexOfVertex ] , sizeof(CVector2) , 1, m_pFile )*sizeof(CVector2);
		blockSize += fwrite( &m_pVertNormal3D[ indexOfVertex ] ,   sizeof(CVector3) , 1, m_pFile )*sizeof(CVector3);
		blockSize += fwrite( &m_pPostion3D[ indexOfVertex ]  , sizeof(CVector3) , 1, m_pFile )*sizeof(CVector3);
		if ( isBakedType() )
		{
			blockSize += fwrite( &m_pTexcoord2DLit[ indexOfVertex ] , sizeof(CVector2)  , 1, m_pFile )*sizeof(CVector2);
		}
	}

	return (int)blockSize;
}

int	t3DObject::writeFace( )
{
	size_t blockSize = 0;

	for (int i = 0; i < m_nNumOfRenderInterface; i++)
	{
		blockSize += fwrite(&m_vecRenderInterface[i]->bHasTexture , sizeof(bool) , 1, m_pFile)*sizeof(bool);
		blockSize += fwrite(&m_vecRenderInterface[i]->numOfFace , sizeof(int) , 1, m_pFile)*sizeof(int);
		blockSize += fwrite(m_vecRenderInterface[i]->RenderFace , sizeof(tFace2) * m_vecRenderInterface[i]->numOfFace , 1, m_pFile)*sizeof(tFace2);
	}
	/////////////////////////////�޲�������Ϣ ����������������
	blockSize += fwrite(&m_numOfFaceNoMaterail , sizeof(int) , 1, m_pFile)*sizeof(int);

	blockSize += fwrite(m_pFaceNoMaterail , sizeof(tFace2) * m_numOfFaceNoMaterail  , 1, m_pFile)*sizeof(tFace2);

	return (int)blockSize;
}

int	t3DObject::writeShadow( )
{
	size_t blockSize = 0;

	blockSize += fwrite(m_pFaceNormal3D, sizeof(CVector3) * m_nNumOfFaces, 1, m_pFile)*sizeof(CVector3);

	blockSize += fwrite(&m_nNumOfVerts , sizeof(int) , 1, m_pFile)*sizeof(int);
	blockSize += fwrite(m_pVertsOriginal, sizeof(CVector3) * m_nNumOfVerts, 1, m_pFile)*sizeof(CVector3);

	blockSize += fwrite(m_shadowFace, sizeof(sw_FaceIndex) * m_nNumOfFaces, 1, m_pFile)*sizeof(sw_FaceIndex);

	return (int)blockSize;
}


int	t3DObject::writeKeyFrame( )
{
	size_t blockSize = 0;

	//	д��ؼ�֡����
	blockSize += fwrite( &m_nKeyFramesCount, sizeof(int), 1, m_pFile)*sizeof(int);
	blockSize += fwrite( m_pKeyFrames, sizeof(ModKeyFrame)*m_nKeyFramesCount, 1, m_pFile)*sizeof(ModKeyFrame)*m_nKeyFramesCount;

	return (int)blockSize;
}


int	t3DObject::writeChange()
{
	//	��д����ͷ��Ϣ�� ��Ⱦ��ʽRenderType
	fseek( m_pFile, m_nPosRenderType, SEEK_SET );
	fwrite(&m_nRenderType, sizeof(int) , 1, m_pFile);

	fseek( m_pFile, 0 , SEEK_END ) ;

	return true;
}


bool t3DObject::isBakedType()
{
	return (bool)(m_nObjectType & OBJ_BAKED);

}
void	t3DObject::setStandardType()
{
	m_nObjectType	&= ~OBJ_BAKED;
	m_nObjectType	|= MTL_STANDARD;
	m_bIsBaked		= false;
}
void	t3DObject::setBakedType()
{
	m_nObjectType	&= ~MTL_STANDARD;
	m_nObjectType	|= OBJ_BAKED;
	m_bIsBaked		= true;
}

bool t3DObject::computerBound()
{
	for (int indexFace = 0; indexFace < m_nNumOfFaces; indexFace ++)
	{
		for (int indexVert = 0; indexVert<3; indexVert ++)
		{
			int index = m_pFacesForShadow[indexFace].vertIndex[indexVert];

			CVector3& pointEach = m_pVertsOriginal[ index ];

			m_boundingBox.merge( pointEach );
		}
	}

	return true;
}

CBox3& t3DObject::getBoundingBox()
{
	return m_boundingBox;
}
//class tMatandFace/////////////////////////////////////////////////////////////////

tMatandFace::tMatandFace()
{
	RenderFace = NULL;
	FaceID = NULL;
}
tMatandFace::~tMatandFace()
{
	if (RenderFace)
	{
		delete[] RenderFace;
		RenderFace = NULL;
	}
	if (FaceID)
	{
		delete[] FaceID;
		FaceID = NULL;
	}
}


void
t3DObject::SetVertsCount(int count) 
{
	m_nNumOfVerts = count;

	m_pVertsOriginal = new CVector3[m_nNumOfVerts];
}

void
t3DObject::SetTVertsCount(int count) 
{
	m_nNumOfTVerts = count;
	
	if (m_pPostion3D)
	{
		return;
	}
	m_pTexcoord2D = new CVector2[m_nNumOfTVerts];
	m_pTexcoord2DLit = new CVector2[m_nNumOfTVerts];
	m_pVertNormal3D = new CVector3[m_nNumOfTVerts];
	m_pPostion3D = new CVector3[m_nNumOfTVerts];
}

void
t3DObject::SetFacesCount(int count) 
{
	m_nNumOfFaces = count;

	m_pFacesAll = new  tFace[m_nNumOfFaces];							// ������
	memset( m_pFacesAll, 0, sizeof(tFace) * m_nNumOfFaces );

	m_pFacesForShadow = new  tFace2[m_nNumOfFaces];						// ��Ӱ������
	memset( m_pFacesForShadow, 0, sizeof(tFace2) * m_nNumOfFaces );

	m_pFaceNormal3D		= new CVector3 [m_nNumOfFaces];
	memset( m_pFaceNormal3D, 0, sizeof(CVector3) * m_nNumOfFaces );

	m_shadowFace		= new sw_FaceIndex[m_nNumOfFaces];
	memset( m_shadowFace, 0, sizeof(sw_FaceIndex) * m_nNumOfFaces );

	m_pFaceNoMaterail = new tFace2[0];

}

void
t3DObject::PutVertex(int index,Verts *v) 
{
	m_pVertsOriginal[index].x = v->x;
	m_pVertsOriginal[index].y = v->y;
	m_pVertsOriginal[index].z = v->z;
}

void
t3DObject::PutTVertex(int index,CVector2 *v) 
{
	m_pTexcoord2D[index].x = v->x;
	m_pTexcoord2D[index].y = v->y;
}

void
t3DObject::PutFace(int index,Faces *f) 
{
	m_pFacesForShadow[index].vertIndex[0] = f->a;
	m_pFacesForShadow[index].vertIndex[1] = f->b;
	m_pFacesForShadow[index].vertIndex[2] = f->c;

	memcpy(&m_pFacesAll[index], &m_pFacesForShadow[index], sizeof(tFace2));
}

void t3DObject::setObjectName( string name )
{
	name.substr(0, sizeof(m_strName));
	memcpy_s( m_strName, sizeof(m_strName), name.c_str(), sizeof(m_strName));
}

void t3DObject::formatRenderData()
{
#if 1
	//  ���콻������
	computerVertexArray();
#endif

	//  ����ÿ������İ�Χ��
	computerBound();

#if 1
	//	������Ⱦ�ӿڣ��ɲ��������棬��������
	//computerRenderInterface();
#endif
}

void t3DObject::computerVertexArray()
{
	// y' = z, z' = -y
	float tmp;
	for (int index = 0; index < m_nNumOfVerts ; index ++)
	{
		tmp = m_pVertsOriginal[index].y;
		m_pVertsOriginal[index].y = m_pVertsOriginal[index].z;
		m_pVertsOriginal[index].z = - tmp;
	}

	if (m_pPostion3D==0)
	{
		SetTVertsCount(m_nNumOfVerts);
	}

	memcpy_s(m_pPostion3D, sizeof(CVector3)*m_nNumOfVerts, 
		m_pVertsOriginal, sizeof(CVector3)*m_nNumOfVerts);
}

void t3DObject::computerRenderInterface()
{
	m_numOfFaceNoMaterail = m_nNumOfFaces;

	if (m_pFacesForShadow==0 || m_numOfFaceNoMaterail==0)
	{
		return;
	}

	m_pFaceNoMaterail = new tFace2[m_numOfFaceNoMaterail];
	memcpy_s(m_pFaceNoMaterail, sizeof(tFace2)*m_numOfFaceNoMaterail, 
		m_pFacesForShadow, sizeof(tFace2)*m_numOfFaceNoMaterail );
}
#if IMPORT_MAX_LIB
int t3DObject::computerVerts( ObjectEntry *oe )
{
	INode*	node = oe->entry->node;			
	Object*	obj = oe->entry->obj;

	Matrix3 ident(TRUE);
	Matrix3 matrix = node->GetObjTMAfterWSM(0);
#if 0
	Point3 pos = matrix.GetRow(3);
	if (pos.x==0.0f && pos.y==0.0f && pos.z==0.0f) {
		pos.x = 0.001f;
		pos.y = 0.001f;
		pos.z = 0.001f;
		matrix.SetTrans(pos);
	}
#endif
	// �Ѽ���������Ļ�����Ϣ �����ռ�����ꡢ�ռ�������������UV�����ꡢ����UV������
	// �Լ��ռ����Ŀ������UV����Ŀ���ռ�����Ŀ������UV����Ŀ����ռ�������ͬ��
	Mesh		*pMeshCurrent  = NULL;			// ���� ����

	Matrix3		*mat  = NULL;				// ���� �任����

	//int		numOfVerts  ;			// ���� �ռ����Ŀ
	CVector3		*pMeshVerts  = NULL;			// ���� �ռ������
	//int		numOfFaces  ;			// ���� �ռ�����Ŀ
	Face		*pMeshFaces  = NULL;			// ���� �ռ�������

	MeshMap		*mapChannel1 = NULL;		// ��һ ��ͼͨ�� 
	//int		numOfTvert1 ;			// ��һ ����UV����Ŀ
	UVVert		*pTVerts1  = NULL;			// ��һ ����UV������
	TVFace		*pTFaces1  = NULL;			// ��һ ����UV������

	MeshMap		*mapChannel3 = NULL ;		// ���� ��ͼͨ�� 
	UVVert		*pTVerts3 = NULL;			// ���� ����UV������
	TVFace		*pTFaces3 = NULL ;			// ���� ����UV������


	bool	bFilterBaked = m_pParentScene->getFlagFilterBaked();
	bool	bSwapUVChannel = m_pParentScene->getFlagSwapChannel();
	bool	bTexture = m_pParentScene->getFlagEnableTexture();
	CVector3	oSceneCenter = m_pParentScene->getSceneCenter();
	float	fScale = m_pParentScene->getScale();

	mat				= &matrix ;
	pMeshCurrent	= &(oe->tri->GetMesh());

	MeshMtlList* pTheMtls = m_pParentScene->getMeshMtlList();
	setMtlType(node, pTheMtls);


	// #############################################################################
	// ��map channel ͨ�����ݽӿ�

	this->m_nNumOfVerts	= pMeshCurrent->getNumVerts() ;
	pMeshVerts			= pMeshCurrent->verts ;

	pMeshFaces			= pMeshCurrent->faces  ;
	
	// ����������������Ӧ�Ľṹ��
	int nFaceCount = pMeshCurrent->getNumFaces();
	setFaceCount(nFaceCount);

	//bIsBaked	= bIsBacked ;

	// #############################################################################
	// 2008��3��6��8:50:05 ��� 
	// map channel 1 UV���ݽӿ�
	mapChannel1 = pMeshCurrent->maps + MAP_CHANNEL_ONE  ;
	
	if ( bFilterBaked )
	{
		setStandardType();
	}
	else if(pMeshCurrent->numMaps >=4 )
	{// ��ͨ����UV
		mapChannel3 = pMeshCurrent->maps + MAP_CHANNEL_THREE  ;

		if (mapChannel3)
		{
			if( bSwapUVChannel )
			{
				mapChannel1->SwapContents(*mapChannel3);
				setStandardType();
			}
			else
			{
				pTVerts3	=  mapChannel3->tv;
				pTFaces3 =  mapChannel3->tf;  
			}			
		}

	}

	pTVerts1	=  mapChannel1->tv;
	pTFaces1	=  mapChannel1->tf;

	this->m_nNumOfTVerts	= mapChannel1->getNumVerts();

	{
		// #############################################################################
		// ͨ�� ���������� �� ��һ������ͼͨ��UV������ �������в�ͬ�Ŀ���Ⱦ�㣨���ꡢUV1��UV3������һ��ͬ��ͬ��
		vector<bool>	vertFlag;

		vertFlag.assign( m_nNumOfVerts, 0 );
		//vertFlag.clear();

		//Tab<Vert3D> tmpVerts ;			
		//Tab<tFace2> tmpFaces ,tmpFaces2;		// 2007/8/27 liangkai �¼�faces2��tmpFace2
		vector<Vert3D> tmpVerts ;			
		vector<tFace2> tmpFaces ,tmpFaces2;		// 2007/8/27 liangkai �¼�faces2��tmpFace2

		tmpVerts.assign( m_nNumOfVerts , Vert3D());
		tmpFaces.assign( m_nNumOfFaces , tFace2());
		tmpFaces2.assign( m_nNumOfFaces , tFace2());
		Vert3D	tmpVert ;
		tFace2  tmpFace ,tmpFace2;
		memset( &tmpVert, 0, sizeof(Vert3D) );
		memset( &tmpFace, 0, sizeof(tFace2) );
		memset( &tmpFace, 0, sizeof(tFace2) );


		for (int i = 0; i < m_nNumOfFaces; i++) 
		{
			Face   face   = pMeshFaces[i];		// ����������

			TVFace tvFace1( 0,0,0 ) ;
			if ( pTFaces1 )
			{
				tvFace1 = pTFaces1[i];	// ��һ��ͼͨ��UV������
			}

			TVFace tvFace3( 0,0,0 ) ;
			if ( isBakedType() )
			{
				tvFace3 = pTFaces3[i];	// ������ͼͨ��UV������
			}


			// �����ȡ������ֵ				
			for (int k = 0; k < 3; k++) 
			{
				tmpFace.vertIndex[k] = face.v[k];
				tmpFace2.vertIndex[k] = face.v[k];
			}

			// ��������ĸ����� ��ȡ�µ��λ���������������
			for (int j = 0; j < 3; j++) 
			{
				UVVert tVert1(0.0f, 0.0f,0.0f) ;
				if ( pTVerts1 )
				{
					tVert1 = pTVerts1[tvFace1.t[j]];
				}

				UVVert tVert3(0.0f, 0.0f,0.0f)  ;
				if (  isBakedType()  )
				{
					tVert3 = pTVerts3[tvFace3.t[j]];
				}

				CVector3 vert  = pMeshVerts[face.v[j]];

				vert = vert * (*mat); 

				tmpVert.pt = vert;
				tmpVert.tv1 = tVert1;
				tmpVert.tv3 = tVert3;

				if ( vertFlag[face.v[j]] ) 
				{ // �õ��ѱ��  
					if ( bTexture && !( isBakedType() ) )
					{// ֻ�� map channel 1
						if( tmpVerts[face.v[j]].tv1 != tmpVert.tv1 )
						{
							tmpFace.vertIndex[j] = tmpVerts.size();
							tmpVerts.push_back(tmpVert);
						}
					}
					else if ( bTexture &&  ( isBakedType() )   ) 
					{// ���� map channel 1&&3
						if( ( tmpVerts[face.v[j]].tv1 != tmpVert.tv1 ) || (tmpVerts[face.v[j]].tv3 != tmpVert.tv3 ) )
						{
							tmpFace.vertIndex[j] = tmpVerts.size();
							tmpVerts.push_back(tmpVert);
						}				
					}
					else 
						continue ;
				}
				else
				{ // �õ�δ��� ���벢���
					tmpVerts[face.v[j]] = tmpVert;
					vertFlag[face.v[j] ] = 1;
					if (44==face.v[j])
					{
						tmpVerts[face.v[j]] = tmpVert;
					}
				}
			}//for (int j = 0; j < 3; j++) �����������
			tmpFaces[i] = tmpFace;
			tmpFaces2[i] = tmpFace2;
		}//for (int i = 0; i < numOfFaces; i++)  ����������



		// ############################################################################# 
		//  ���newobj�ṹ��
		m_pVertsOriginal		= new  CVector3[m_nNumOfVerts];					// ��������
		memset( m_pVertsOriginal, 0, sizeof(CVector3) * m_nNumOfVerts );


		// ���õ�����������Ӧ�Ľṹ��
		int nVertexArraySize = tmpVerts.size();
		setVertexArraySize(nVertexArraySize);


		vertFlag.assign( m_nNumOfTVerts , 0);
		for ( int i=0 ;i< m_nNumOfFaces ;i++ )
		{
			for (int k = 0; k < 3; k++) 
			{
				m_pFacesAll[i].vertIndex[k] = tmpFaces[i].vertIndex[k];
				m_pFacesForShadow[i].vertIndex[k] = tmpFaces2[i].vertIndex[k];
			}
			for ( int j =0; j < 3; j++)
			{
				int  vertIndex1 = tmpFaces[i].vertIndex[j] ;
				int  vertIndex2 = tmpFaces2[i].vertIndex[j] ;

				if ( vertIndex1 >= m_nNumOfTVerts)
				{
					continue;
				}
				if ( !vertFlag[ vertIndex1 ] ) 
				{
					// �µ�Ľ������� (����������һ������)
					
					m_pPostion3D[ vertIndex1 ].x =  
						(tmpVerts[ vertIndex1 ].pt.x - oSceneCenter.x)*fScale;
					m_pPostion3D[ vertIndex1 ].y =  
						(tmpVerts[ vertIndex1 ].pt.z - oSceneCenter.z)*fScale;
					m_pPostion3D[ vertIndex1 ].z = 
						-(tmpVerts[ vertIndex1 ].pt.y - oSceneCenter.y)*fScale;					
					/*m_pPostion3D[ vertIndex1 ].x =  tmpVerts[ vertIndex1 ].pt.x;
					m_pPostion3D[ vertIndex1 ].y =  tmpVerts[ vertIndex1 ].pt.z;
					m_pPostion3D[ vertIndex1 ].z = -tmpVerts[ vertIndex1 ].pt.y;					
					*/

					m_pTexcoord2D[ vertIndex1 ].x = tmpVerts[ vertIndex1 ].tv1.x;
					m_pTexcoord2D[ vertIndex1 ].y = tmpVerts[ vertIndex1 ].tv1.y;

					// �������� �ƹ���ӰUV
					if (  isBakedType()  )
					{
						m_pTexcoord2DLit[ vertIndex1 ].x = tmpVerts[ vertIndex1 ].tv3.x;
						m_pTexcoord2DLit[ vertIndex1 ].y = tmpVerts[ vertIndex1 ].tv3.y;
					}

					m_pVertsOriginal[ vertIndex2 ].x =  (tmpVerts[ vertIndex2 ].pt.x - oSceneCenter.x)*fScale;
					m_pVertsOriginal[ vertIndex2 ].y =  (tmpVerts[ vertIndex2 ].pt.z - oSceneCenter.z)*fScale;
					m_pVertsOriginal[ vertIndex2 ].z = -(tmpVerts[ vertIndex2 ].pt.y - oSceneCenter.y)*fScale;
					

					vertFlag[vertIndex1] = 1 ;
				} 

			}//for ( int j =0; j < 3; j++) �����������
		}//for ( int i=0 ;i< m_nNumOfFaces ;i++ ) ����������
		// ͨ�� ���������� �� ��һ������ͼͨ��UV������ �������в�ͬ�Ŀ���Ⱦ�㣨���ꡢUV1��UV3������һ��ͬ��ͬ��
		// #############################################################################
	}


	return 1;
}
void t3DObject::setBoundingBox( Box3 box )
{
	CVector3 maxPoint( box.pmax.x, box.pmax.z, -box.pmax.y );
	CVector3 minPoint( box.pmin.x, box.pmin.z, -box.pmin.y );

	m_boundingBox.merge( maxPoint );
	m_boundingBox.merge( minPoint );

}


#endif

int		t3DObject::computerNormal()
{
	//Mesh &mesh = oe->tri->GetMesh();
	int count = m_nNumOfFaces;
	CVector3 vVector1, vVector2, vNormal, vPoly[3];

	//m_pFaceNormal3D		= new CVector3 [count];
	CVector3 *pTempNormals	= new CVector3 [count];
	for(int i=0; i < count; i++)
	{												
		vPoly[0] = m_pPostion3D[ m_pFacesAll[i].vertIndex[0] ];
		vPoly[1] = m_pPostion3D[ m_pFacesAll[i].vertIndex[1] ];
		vPoly[2] = m_pPostion3D[ m_pFacesAll[i].vertIndex[2] ];
		vVector1 = vgMath::subVector(vPoly[2], vPoly[1]);
		vVector2 = vgMath::subVector(vPoly[0], vPoly[2]);
		vNormal  = vgMath::crossProduct(vVector1, vVector2);
		pTempNormals[i] = vNormal;
		vNormal= vNormal.normalize();//Normalize(vNormal);
		m_pFaceNormal3D[i] = vNormal;
	}

	CVector3 vSum;
	memset(&vSum, 0, sizeof(CVector3) );
	CVector3 vZero;
	memset(&vZero, 0, sizeof(CVector3) );
	int shared=0;
	for (int i = 0; i < m_nNumOfTVerts; i++)
	{
		for (int j = 0; j < m_nNumOfFaces; j++)	
		{												
			if (m_pFacesAll[j].vertIndex[0] == i || 
				m_pFacesAll[j].vertIndex[1] == i || 
				m_pFacesAll[j].vertIndex[2] == i)
			{
				//vSum = vSum + newobj->m_pFaceNormal3D[j];
				for(int index=0 ;index<3; index++)
					vSum.vert[index] += m_pFaceNormal3D[j].vert[index];
				shared++;								// Increase the number of shared triangles
			}
		}      
		//vSum = vSum/float(shared);
		vSum = vSum.normalize();//Normalize(vSum);

		m_pVertNormal3D[i].x = vSum.x;
		m_pVertNormal3D[i].y = vSum.y;
		m_pVertNormal3D[i].z = vSum.z;

		m_pVertNormal3D[i].x = vSum.x;
		m_pVertNormal3D[i].y = vSum.y;
		m_pVertNormal3D[i].z = vSum.z;

		vSum = vZero;
		shared = 0;
	}

	return 1;
}
int		t3DObject::computerShadowFaces()
{
	// 2007/8/27 liangkai �¼� ������return������
	//	m_nNumOfVerts = verts2.Count();
	int numOfFaces = m_nNumOfFaces;
	//m_shadowFace = new sw_FaceIndex[numOfFaces];

	CVector3	lightDir;
	lightDir.vert[0] = 0.1f; 
	lightDir.vert[1] = -0.5f;
	lightDir.vert[2] = 0.1f;
	// �������ÿ��sw_FaceIndex���������㲢����normal
	for ( int i=0 ;i<numOfFaces ;i++ )
	{
		// ��ÿ��������һ��sw_FaceIndex
		sw_FaceIndex &newSWFace = m_shadowFace[i];

		// �õ���������	
		tFace2 &face = m_pFacesForShadow[i];
		int &point1 = newSWFace.p1;
		int &point2 = newSWFace.p2;
		int &point3 = newSWFace.p3;
		point1= face.vertIndex[0];
		point2= face.vertIndex[1];
		point3= face.vertIndex[2];

		newSWFace.normal = i;
		i*=1;

	}
	//////////////////////////////////// �������ÿ������������㲢����normal
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// ��θ���sw_FaceIndex������ֵ
	for ( int i=0 ;i<numOfFaces ;i++ )
	{
		// ��ÿ��������һ��sw_FaceIndex
		sw_FaceIndex &pSWFace = m_shadowFace[i];
		// ��ֵ
		int& point1 = pSWFace.p1;
		int& point2 = pSWFace.p2;
		int& point3 = pSWFace.p3;

		// �����ٱ�

		int	face_index1 = sw_searchForAdjacent( i , point1 , point2 );
		int face_index2 = sw_searchForAdjacent( i , point2 , point3 );
		int face_index3 = sw_searchForAdjacent( i , point3 , point1 );

		pSWFace.p1p2AdjFaceIndex = face_index1;
		pSWFace.p2p3AdjFaceIndex = face_index2;
		pSWFace.p3p1AdjFaceIndex = face_index3;

		if ( vgMath::
			dotProduct(m_pFaceNormal3D[ pSWFace.normal ], lightDir )<=0 )
		{
			// �ڰ�ʱ��ʲôҲ��Ͷ�䡣
			pSWFace.faceVisible = false;

			// 2007/8/30 liangkai �¼ӣ�
			// 			pSWFace.p1p2AdjNormal = face_index1;
			// 			pSWFace.p2p3AdjNormal = face_index2;
			// 			pSWFace.p3p1AdjNormal = face_index3;
			continue;
		}
		else 
		{
			pSWFace.faceVisible = true;

			// ����p1p2Visible
			if ( face_index1 != -1 )
			{
				//				int& _normal2 = m_shadowFace[face_index1].normal;
				//				pSWFace.p1p2AdjNormal = face_index1;

				float dot_result2 = vgMath::
					dotProduct(m_pFaceNormal3D[ face_index1 ], lightDir) ;

				if ( dot_result2 > 0 )
				{
					pSWFace.p1p2Visible = false;
				}
				else
				{
					pSWFace.p1p2Visible = true;
				}
			}
			else
			{
				pSWFace.p1p2Visible = true;
			}

			// ����p2p3Visible
			if ( face_index2 != -1 )
			{
				//				int& _normal2 = m_shadowFace[face_index1].normal;
				//				pSWFace.p2p3AdjNormal = face_index2;

				float dot_result2 =  vgMath::
					dotProduct(m_pFaceNormal3D[ face_index2 ],lightDir );

				if ( dot_result2 > 0 )
				{
					pSWFace.p2p3Visible = false;
				}
				else
				{
					pSWFace.p2p3Visible = true;
				}
			}
			else
			{
				pSWFace.p2p3Visible = true;
			}

			// ����p3p1Visible
			if ( face_index3 != -1 )
			{
				//				int& _normal2 = m_shadowFace[face_index1].normal;
				//				pSWFace.p3p1AdjNormal = face_index3;

				float dot_result2 =  vgMath::
					dotProduct(m_pFaceNormal3D[ face_index3 ],lightDir );

				if ( dot_result2 > 0 )
				{
					pSWFace.p3p1Visible = false;
				}
				else
				{
					pSWFace.p3p1Visible = true;
				}
			}
			else
			{
				pSWFace.p3p1Visible = true;
			}
		}
	}
	//////////////////////////////////// ��θ���sw_FaceIndex������ֵ
	//////////////////////////////////////////////////////////////////////////

	return 1;
}



int		t3DObject::sw_searchForAdjacent(int faceid , int& point1 , int& point2 )
{
	// 2007/8/28 liangkai �¼� ������return������
	int	numOfFaces = m_nNumOfFaces;
	for (int face = 0; face < numOfFaces; face++)
	{
		if ( face == faceid )
		{
			continue;
		}
		// �õ���������
		int& p1 = m_shadowFace[ face ].p1;
		int& p2 = m_shadowFace[ face ].p2;
		int& p3 = m_shadowFace[ face ].p3;

		if ( point1 == p1 )
		{
			if ( point2 == p2 || point2 == p3 )
			{
				return face ;
			}
		}
		else if ( point1 == p2 )
		{
			if ( point2 == p1 || point2 == p3 )
			{
				return face ;
			}
		}
		else if ( point1 == p3 )
		{
			if ( point2 == p1 || point2 == p2 )
			{
				return face ;
			}
		}

	}

	// û���ҵ�
	return -1;
}

void t3DObject::setFaceCount(int nFaceCount)
{
	m_nNumOfFaces = nFaceCount;

	m_pFacesAll = new  tFace[m_nNumOfFaces];							// ������
	memset( m_pFacesAll, 0, sizeof(tFace) * m_nNumOfFaces );

	m_pFacesForShadow = new  tFace2[m_nNumOfFaces];						// ��Ӱ������
	memset( m_pFacesForShadow, 0, sizeof(tFace2) * m_nNumOfFaces );

	m_pFaceNormal3D		= new CVector3 [m_nNumOfFaces];
	memset( m_pFaceNormal3D, 0, sizeof(CVector3) * m_nNumOfFaces );
	
	m_shadowFace		= new sw_FaceIndex[m_nNumOfFaces];
	memset( m_shadowFace, 0, sizeof(sw_FaceIndex) * m_nNumOfFaces );
}

void t3DObject::setVertexArraySize( int nVertexArraySize )
{
	m_nNumOfTVerts = nVertexArraySize;
	m_pVertNormal3D	= new CVector3[ m_nNumOfTVerts ];
	m_pPostion3D	= new CVector3[ m_nNumOfTVerts ];
	m_pTexcoord2D	= new CVector2[ m_nNumOfTVerts ];
	if ( isBakedType() )
	{
		m_pTexcoord2DLit	= new CVector2[ m_nNumOfTVerts ];
		memset( m_pTexcoord2DLit, 0, sizeof(CVector2) * m_nNumOfTVerts );
	}
	memset( m_pVertNormal3D, 0, sizeof(CVector3) * m_nNumOfTVerts );
	memset( m_pPostion3D, 0, sizeof(CVector3) * m_nNumOfTVerts );
	memset( m_pTexcoord2D, 0, sizeof(CVector2) * m_nNumOfTVerts );
}


#if	IMPORT_MAX_LIB
E_Return_Type 
t3DObject::computerMatAndFace( ObjectEntry *oe )
{
	INode*	node = oe->entry->node;			
	Object*	obj = oe->entry->obj;
	newmesh = &(oe->tri->GetMesh());

	MeshMtlList* pTheMtls = m_pParentScene->getMeshMtlList();

	//////////////////////////////////////////////////////////////////////////
	//  д��ÿ������Ĳ��ʼ����������Ϣ
	Mtl *mtl = node->GetMtl() ;
	mtlNumMap.assign(1, -1);
	mtlNumMap[0] = pTheMtls->FindMtl(mtl);
	mtlnum	= 0;
	bakedMtlNo = -1;

	multiMtl	= FALSE;

	bFaceOfMatAll.assign( m_nNumOfFaces , 0);
	//bFaceOfMatAll.clear();
	totleFaceWithMat = 0;

	// ���������������������Ⱦ�ӿ�
	if ( bTexture )
	{
		E_Return_Type	ret = collectMtlDataForEachObj(mtl);/*ExportModel(OBJECT_MATINFO, mtl )*/
		if ( RETURN_MTL_MULTITEX_NEST == ret)
			return ret;
	}

	// �޲���������
	m_numOfFaceNoMaterail = m_nNumOfFaces - totleFaceWithMat;

	if ( m_numOfFaceNoMaterail )
	{					
		m_pFaceNoMaterail = new tFace2[m_numOfFaceNoMaterail];
		m_numOfFaceNoMaterail = 0;

		for (int i=0; i< m_nNumOfFaces; i++)
		{
			if ( !bFaceOfMatAll[i])
			{
				m_pFaceNoMaterail[m_numOfFaceNoMaterail].
					vertIndex[0] = newmesh->faces[i].v[0];
				m_pFaceNoMaterail[m_numOfFaceNoMaterail].
					vertIndex[1] = newmesh->faces[i].v[1];
				m_pFaceNoMaterail[m_numOfFaceNoMaterail].
					vertIndex[2] = newmesh->faces[i].v[2];							
				m_numOfFaceNoMaterail ++;
			}
		}// for ( m_nNumOfFaces )
	}// if ( m_numOfFaceNoMaterail )

	return RETURN_TRUE;
}



E_Return_Type
t3DObject::collectMtlDataForEachObj( Mtl*mtl )
{
	MeshMtlList* pTheMtls = m_pParentScene->getMeshMtlList();

	Mtl *sub;
	unsigned long mtlClassID =0;
	if( mtl )
	{
		mtlClassID = mtl->ClassID().PartA();
	}
	// #########################################################################################
	// ���ʷ���
	switch( mtlClassID )
	{
	case	DMTL_CLASS_ID :	
		{	// ��׼����
			/*mtlNumMap.SetCount(0);	
			mtlnum = 0;*/
			int numOfFace = m_nNumOfFaces;
			int numOfVert = m_nNumOfTVerts;
			int numOfFaceWithMat =0;

			bFaceOfMatOne.assign( numOfFace  , 0);
			//bFaceOfMatOne.clear();
			vertFlag.assign( numOfVert  , 0);
			//vertFlag.clear();

			/*mtlNumMap[0] = pTheMtls->FindMtl(mtl) ;*/

			SMtl *smtl;
			smtl = (*pTheMtls)[ mtlNumMap[mtlnum] ].sm;
			//assert(smtl);

			newrender = new  tMatandFace;
			//memset( newrender, 0 , sizeof(tMatandFace) );

			// ʹ���˸ò��ʵ�����ͳ��							
			if (multiMtl)
			{			// ��ά�Ӳ������
				for ( int ix=0; ix< m_nNumOfFaces; ix++ )
				{
					if ( !bFaceOfMatAll[ix] )
					{				
						int mtlIndex = newmesh->getFaceMtlIndex( ix );//m->faces[ix].getMatID();
						if (mtlnum == mtlIndex)
						{
							numOfFaceWithMat ++;
							bFaceOfMatOne[ix] =1; 
							bFaceOfMatAll[ix] =1;
						}	//if								
					} //if
				}//for(m_nNumOfFaces)

			} //if(multiMtl)
			else
			{			// ���������
				numOfFaceWithMat = newmesh->numFaces;
				bFaceOfMatOne.assign( numOfFace  , 1);
				bFaceOfMatAll.assign( numOfFace  , 1);
			}
			newrender->numOfFace = numOfFaceWithMat;
			totleFaceWithMat += numOfFaceWithMat;

			if ( (fabsf(smtl->uTile - 1.0f) < 1.0e-4 )&&
				(fabsf(smtl->vTile - 1.0f) < 1.0e-4 )	)
			{//����uv�ظ���Ϊ1�����õ���uv
				vertFlag.assign( numOfFace  , 1);
			}

			// ʹ���˸ò��ʵ���ID������������
			if ( numOfFaceWithMat )
			{	
				newrender->FaceID = new	short[numOfFaceWithMat];
				memset( newrender->FaceID, 0, sizeof(short) * numOfFaceWithMat );
				newrender->RenderFace = new tFace2[numOfFaceWithMat];
				memset( newrender->RenderFace, 0, sizeof(struct tFace2)* numOfFaceWithMat );

				numOfFaceWithMat = 0;
				float	tempTile = 1.0f;
				float	deltaTileU = 0.0f, deltaTileV = 0.0f;
				int  vertIndex1;
				for (int ix=0; ix< m_nNumOfFaces ; ix++) 
				{
					if ( bFaceOfMatOne[ix] )
					{
						newrender->FaceID[numOfFaceWithMat] = ix;
						newrender->RenderFace[numOfFaceWithMat].vertIndex[0] = m_pFacesAll[ix].vertIndex[0];
						newrender->RenderFace[numOfFaceWithMat].vertIndex[1] = m_pFacesAll[ix].vertIndex[1];
						newrender->RenderFace[numOfFaceWithMat].vertIndex[2] = m_pFacesAll[ix].vertIndex[2];
						numOfFaceWithMat ++;

						deltaTileU	=	0.0f;
						deltaTileV	=	0.0f;
						// �������uv���ظ����������ʹ���˸ò��ʵ���Ƭ��Ӧ��uv����
						for ( int j =0; j < 3; j++)
						{
							vertIndex1 = m_pFacesAll[ix].vertIndex[j] ;

							if ( !vertFlag[ vertIndex1 ] ) 
							{
								tempTile	=	m_pTexcoord2D[ vertIndex1 ].x * smtl->uTile /*-0.001f*/;
								//tempTile	-=	int(tempTile);
								m_pTexcoord2D[ vertIndex1 ].x = tempTile;

								tempTile	=	m_pTexcoord2D[ vertIndex1 ].y * smtl->vTile;
								//tempTile	-=	int(tempTile);
								m_pTexcoord2D[ vertIndex1 ].y = tempTile;

								vertFlag[vertIndex1] = 1 ;

							}//if (��δ���)

						}//for (�������)

					}//if (��δ���)

				}//for (ÿ���棩 

			}//if (�ò��ʵĹ�������)

			// �ò��ʵ����� ��ɫ ID 
			_tcscpy_s(newrender->strName ,/*255,*/ smtl->name);

			newrender->red   = smtl->diffColor.r;
			newrender->green = smtl->diffColor.g;
			newrender->blue  = smtl->diffColor.b;
			newrender->textureId = mtlnum;

			if ( smtl->diffuseBitmap[0] )
			{
				newrender->bHasTexture	= TRUE;
			}

			// 2008��3��6��15:58:51 ���
			bool isBaked = isBakedType();

			if ( isBaked && ( bakedMtlNo > 0) )
			{
				smtl = (*pTheMtls)[ bakedMtlNo ].sm;
				_tcscpy_s(newrender->strNameBaked ,/*255,*/ smtl->name);
				//newrender->bHasTexture	= TRUE;
			}
			else
			{
				newrender->strNameBaked[0] = '\0';
			}

			if ( numOfFaceWithMat )
			{
				m_vecRenderInterface.push_back( newrender );
				m_nNumOfRenderInterface ++;
				newrender = NULL;
			}
			else
			{
				delete newrender;
				newrender = NULL;
			}
		}// case	DMTL_CLASS_ID :
		break;

		//////////////////////////////////////////////////////////////////////////
	case	MULTI_CLASS_ID :
		{   // ��ά�Ӳ���
			multiMtl = TRUE;
			mtlNumMap.assign(mtl->NumSubMtls(), -1);
			int mtlIndex = 0;
			for (int i = 0; i < mtl->NumSubMtls(); i++) 
			{
				Mtl* sub  = mtl->GetSubMtl(i);
				if (sub)
				{
					mtlNumMap[i] = pTheMtls->FindMtl(sub);
				}
				else
				{
					mtlNumMap[i] = 0;
				}
				mtlIndex = mtlNumMap[i];
			}

			for (int i = 0; i < mtl->NumSubMtls(); i++) 
			{
				Mtl* sub  = mtl->GetSubMtl(i);
				if (sub)
				{
					mtlnum = i ;//% (mtl->NumSubMtls());

					if (sub->NumSubMtls())
					{
						return RETURN_MTL_MULTITEX_NEST;
					}

					E_Return_Type	ret = collectMtlDataForEachObj(sub);
					if ( RETURN_MTL_MULTITEX_NEST == ret)
						return ret;

				}
			}

		}//case	MULTI_CLASS_ID :
		break;

	case	BAKE_SHELL_CLASS_ID :
		{   // �ǲ���
			sub = mtl->GetSubMtl(1);
			if ( sub )
				bakedMtlNo = pTheMtls->FindMtl(sub);

			sub= mtl->GetSubMtl(0);
			mtlNumMap[0] = pTheMtls->FindMtl(sub) ;
			if ( sub )
			{
				E_Return_Type	ret = collectMtlDataForEachObj(sub);
				if ( RETURN_MTL_MULTITEX_NEST == ret)
					return ret;
			}
		}//case	BAKE_SHELL_CLASS_ID
		break;

	default:
		break;
	}//switch( mtlClassID )

	return	RETURN_TRUE;
}

void 
t3DObject::setMtlType( INode* node, MeshMtlList* pTheMtls )
{
	if ( node->GetMtl() )
	{
		bTexture = TRUE;

		if ( pTheMtls->IsShellMtl( node->GetMtl() ) )
		{
			setBakedType();
		}
	}
	else
	{
		bTexture = FALSE;
	}
}
#endif
void 
t3DObject::computerMatandFaceEach( tMatandFace* newrender )
{
	for (int indexFace = 0 ; indexFace < newrender->numOfFace; indexFace ++)
	{
		int faceId = newrender->FaceID[indexFace];
		tFace& eachFace = m_pFacesAll[faceId];
		
		for (int indexVert = 0 ; indexVert < 3; indexVert ++)
		{	
			newrender->RenderFace[indexFace].vertIndex[indexVert] = 
				eachFace.vertIndex[indexVert];
		}
	}
}
