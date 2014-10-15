#include "vgStableHeaders.h"

#include <vgMod/vgObjectSet.h>

// t3DModel		//////////////////////////////////////////////////////////////////////
#if	IMPORT_MAX_LIB
t3DModel::t3DModel(std::string name, MeshMtlList* pMtls, Interface* pMaxInterface)
//:m_strModelName(name)
: m_pTheMtls(pMtls)
//, m_pMaxInterface(pMaxInterface)
{
	m_nTotalFaceCount = 0;

	m_pFile = NULL;
	m_puObjPos = NULL;
	m_uiObjPosPos = 0;

	setDefault();
}
#endif

t3DModel::t3DModel( std::string name, MeshMtlList* pMtls )
: m_pTheMtls(pMtls)
{
	m_nTotalFaceCount = 0;

	m_pFile = NULL;
	m_puObjPos = NULL;
	m_uiObjPosPos = 0;

	setDefault();
}

t3DModel::~t3DModel()
{
	reset();
}
void	t3DModel::push_back(t3DObject* pObject)
{
	m_vObject.push_back(pObject);
	m_vObjectType.push_back(pObject->m_nObjectType);
	m_vIsBaked.push_back(pObject->m_bIsBaked);

	m_boundingBox.merge( pObject->getBoundingBox() );
	m_nTotalFaceCount += pObject->m_nNumOfFaces;
}
int		t3DModel::size()
{
	return	(int)m_vObject.size();
}

bool	t3DModel::Write(FILE* pFile
						, TCHAR* pFormat, int nVersionMod/* = VERSION_MOD_CURRENT*/)
{
	m_pFile = pFile;

	_tcsncpy_s(m_sFormat, sizeof(m_sFormat), pFormat, sizeof(m_sFormat));
	m_nVersionMod = nVersionMod;

	//	Part 0: ��������
	writeFormat();
	
	//	Part 1: ��������
	writeHeader();

	//	Part 2: ģ������
	writeObjects();

	//	Part 3: ��������
	writeChange();

	fseek( m_pFile, 0 , SEEK_END ) ;

	return true;
}

bool	t3DModel::reset()
{
	vector<t3DObject*>::iterator iter = m_vObject.begin();
	vector<t3DObject*>::iterator iterEnd = m_vObject.end();
	for ( ; iter != iterEnd; iter++)
	{
		if (*iter)
		{
			delete *iter;
			*iter = NULL;
		}
	}
	m_vObject.clear();

	if(m_puObjPos)
	{
		delete	m_puObjPos;
		m_puObjPos = NULL;
	}

	return true;
}

bool	t3DModel::setDefault()
{
	//m_nVersionMod	= VERSION_MOD_CURRENT;//VERSION_MOD(year, updateTime);
	m_fScale = 1.0f;
	m_bSwapUVChannel = false;
	m_bFilterBaked = false;

	return	true;
}

bool	t3DModel::writeFormat()
{
	// Part 1: ��ʽ��Ϣ
	fwrite( m_sFormat, sizeof(m_sFormat), 1, m_pFile);

	// Part 2: �汾��Ϣ
	fwrite( &m_nVersionMod, sizeof(unsigned int), 1,m_pFile );
	return true;
}
bool	t3DModel::writeHeader()
{
	//	Part 1: ������������
	fwrite( &m_oSceneCenter, sizeof(CVector3), 1, m_pFile);

	int	nObjectCount = (int)m_vObject.size();
	//	Part 1: ��������
	fwrite( &nObjectCount, sizeof(int), 1,  m_pFile);

	//	Part 2: ����λ������
	m_uiObjPosPos = ftell(m_pFile);   //��λ�õ�����Ҫ�仯
	m_puObjPos = new unsigned int[nObjectCount];
	fwrite( m_puObjPos, sizeof(unsigned int) * nObjectCount, 1,  m_pFile);

	//	Part 3: �������ͣ���׼���決���ؼ�֡������
	if (m_nVersionMod == VERSION_MOD_CURRENT)
	{
		static TraverseVecForWriteObjectTypeInt	vecWriter(m_pFile);
		for_each( m_vObjectType.begin() ,m_vObjectType.end() ,vecWriter );
	}
	else if (m_nVersionMod == VERSION_MOD_LAST)
	{
		//static TraverseVecForWriteObjectType<bool> vecWriter(m_pFile);
		static TraverseVecForWriteObjectTypeBool	vecWriter(m_pFile);
		for_each( m_vIsBaked.begin() ,m_vIsBaked.end() ,vecWriter );
	}
	//static TraverseVecForWriteObjectTypeInt vecWriter(m_pFile);
	//for_each( m_vObjectType.begin() ,m_vObjectType.end() ,vecWriter );

	
	return	true;
}

bool	t3DModel::writeObjects()
{
	int index=0;
	for ( vector<t3DObject*>::iterator iter = m_vObject.begin();
		iter != m_vObject.end(); iter++, index++)
	{
		m_puObjPos[index] = ftell(m_pFile);

		fwrite( &m_uiObjChunkSize , sizeof(unsigned int), 1, m_pFile);

		//	ÿ������ľ�̬������Ϣ
		(*iter)->Write(m_pFile, m_pTheMtls, m_nVersionMod);

		// ���½�����ʾ
		//m_pMaxInterface->ProgressUpdate((int)((float)index / m_vObject.size() * 100.0f)); 
	}

	return	true;
}

bool	t3DModel::writeChange()
{
	unsigned int	uiFilePos = ftell(m_pFile);
	int	nObjectCount = (int)m_vObject.size();

	//	Part 4: ��д�ļ�ͷ�� ����λ��
	fseek( m_pFile, m_uiObjPosPos , SEEK_SET);
	fwrite( m_puObjPos , sizeof(unsigned int) * nObjectCount , 1, m_pFile);//д��ÿ���ļ������ʼλ�ã�

	//	Part 5: ��д����ͷ�� ÿ������Ŀ��С
	for (int i = 0; i < nObjectCount -1 ; i++)
	{
		m_uiObjChunkSize = m_puObjPos[i+1] - m_puObjPos[i] -4;
		fseek( m_pFile, m_puObjPos[i] , SEEK_SET);
		fwrite( &m_uiObjChunkSize , sizeof(unsigned int) , 1, m_pFile);
	}													//д��ǰn-1���ļ���ĳ��ȣ�

	m_uiObjChunkSize = uiFilePos - m_puObjPos[nObjectCount - 1] -4;
	fseek( m_pFile, m_puObjPos[nObjectCount - 1] , SEEK_SET);
	fwrite( &m_uiObjChunkSize , sizeof(unsigned int) ,1 , m_pFile);

	fseek( m_pFile, 0 , SEEK_END ) ;//д�����һ���ļ���ĳ��ȣ�

	return	true;
}
#if 0
void t3DModel::setModName( const std::string& name )
{
	m_strModelName = name;
}

std::string t3DModel::getModName()
{
	return m_strModelName;
}
#endif
CBox3& t3DModel::getBoundingBox()
{
	return m_boundingBox;
}

void t3DModel::setFlagFilterBaked( bool filter )
{
	m_bFilterBaked = filter;
}

bool t3DModel::getFlagFilterBaked()
{
	return m_bFilterBaked;
}

void t3DModel::setFlagSwapChannel( bool swap )
{
	m_bSwapUVChannel = swap;
}

bool t3DModel::getFlagSwapChannel()
{
	return m_bSwapUVChannel;
}

void t3DModel::setFlagEnableTexture( bool texture )
{
	m_bEnableTexture = texture;
}

bool t3DModel::getFlagEnableTexture()
{
	return m_bEnableTexture;
}

void t3DModel::setScale( float scale )
{
	m_fScale = scale;
}

float	t3DModel::getScale()
{
	return m_fScale;
}

void t3DModel::setSceneCenter( CVector3 pos )
{
	m_oSceneCenter = pos;
}

vgMath::CVector3 t3DModel::getSceneCenter()
{
	return m_oSceneCenter;
}

#if	IMPORT_MAX_LIB

void 
t3DModel::dumpObjListProcessNoUI()
{
	int ox = 0,ix=1;
	ObjectEntry*	oe = m_pTheObjects->head;
	SceneEntry*		se = m_pTheSceneEnum->head;
	Object *obj;

	for (; 
		oe != NULL; 
		oe = oe->next,ox++,ix++, se = se->next) 
	{
		// �½�һ�� t3DObject newobj �ṹ��
		newobj = new t3DObject(this);
		obj = oe->entry->obj;
		oe->tri = (TriObject *)obj->ConvertToType(m_pTheSceneEnum->i->GetTime(), triObjectClassID);

		if(!oe->tri)
		{
			delete newobj;
			newobj = NULL;
			continue;
		}

		// ��ȡһ���������Ϣ�ݴ����ڴ����newobj
		E_Return_Type status = collectGeoDataForEachObjNoUI(oe);//ExportModel(OBJECT_HEAD,oe); 

		// ������ȱ�ݵ�ģ�ͻ���ݾ�������ɸѡ
		if( filterFaultyObject(se->node, status) )
			continue;

		// ����OBJ_BAKED��;����չ����ʱ���ڰ汾����
		if ( newobj->isBakedType() )
		{
			newobj->m_nObjectType |= MTL_STANDARD;
			newobj->m_nObjectType |= MTL_SHELL;
		}

		this->push_back(newobj);

		newobj = NULL;
	}
}

E_Return_Type 
t3DModel::collectGeoDataForEachObjNoUI( ObjectEntry *oe )
{
	// Part 1:��������
	string name(oe->entry->name); 

	name = name.substr(0, 20);
	newobj->setObjectName( string(name.data()) );

	//  ����ÿ������ĵ������Ϣ
	newobj->computerVerts( oe );

	//  ����ÿ������İ�Χ��
	Box3 objBox = getObjectBox( oe );
	newobj->setBoundingBox(objBox);

#if SWITCH_MOD_EXPORT_NORMAL
	//  ����ÿ���������е�ķ�����
	newobj->computerNormal();
#endif

#if SWITCH_MOD_EXPORT_SHADOW
	//  ������Ӱ�ṹ��sw_faces
	newobj->computerShadowFaces();
#endif


	//  ��������mod��Ⱦ�ӿ�
	if ( RETURN_MTL_MULTITEX_NEST == newobj->computerMatAndFace(oe) )
	{
		return RETURN_MTL_MULTITEX_NEST;
	}

	return	RETURN_TRUE;
}

bool t3DModel::filterFaultyObject( INode* pNode, E_Return_Type status )
{
	//  ����ģ��case1������������Ϊ��, �������������Ϊ�㣬���˳�������
	if (
		((newobj->m_nNumOfVerts == 0)
		||(newobj->m_nNumOfFaces == 0))
		) 
	{
		delete newobj;
		newobj = NULL;
		return true;
	}

	//	����ģ��case2����ά��Ƕ��
	if ( RETURN_MTL_MULTITEX_NEST == status /*&& g_bMulti2Text*/) //Ĭ���˳�
	{
		delete newobj;
		newobj = NULL;
		return true;
	}

	//	��ȡÿ������Ĺؼ�֡��Ϣ
	//ComputerKeyFrames(pNode);

	//	����ģ��case4���˳��ؼ�֡	����			

	//	����ģ��case5���˳��ǹؼ�֡����				
	if ( 
		(0 != newobj->m_nKeyFramesCount)
	)
	{
		delete newobj;
		newobj = NULL;
		return true;
	}
	return false;
}

Box3 t3DModel::getObjectBox( ObjectEntry* oe )
{
	Box3 bb;
	ViewExp *vpt = m_pTheSceneEnum->i->GetViewport(NULL);

	oe->tri->GetWorldBoundBox(m_pTheSceneEnum->i->GetTime(), 
		oe->entry->node, 
		vpt, 
		bb);

	return bb;
}
#endif
// t3DModel		//////////////////////////////////////////////////////////////////////
