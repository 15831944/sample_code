
#include <vgStableHeaders.h>
#include "vgObjectSet.h"

// t3DModel		//////////////////////////////////////////////////////////////////////
t3DModel::t3DModel()
{
	m_nObjectCount	= m_nTotalFaceCount = 0;
	memset( m_strModelName, 0, sizeof(m_strModelName) );
	memset( m_strFileName, 0, sizeof(m_strFileName) );
	m_pFile = NULL;
	m_puObjPos = NULL;
	m_uiObjPosPos = 0;
	m_pMaxInterface	= NULL;

	initilize();
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
}
int		t3DModel::size()
{
	return	(int)m_vObject.size();
}

bool	t3DModel::Write(FILE* pFile, MeshMtlList* pMeshList, Interface* pMaxInterface
						, TCHAR* pFormat, int nVersionMod/* = VERSION_MOD_CURRENT*/)
{
	m_pFile = pFile;
	m_pTheMtls = pMeshList;
	m_pMaxInterface = pMaxInterface;
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

bool	t3DModel::initilize()
{
	//m_nVersionMod	= VERSION_MOD_CURRENT;//VERSION_MOD(year, updateTime);

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
		m_pMaxInterface->ProgressUpdate((int)((float)index / m_vObject.size() * 100.0f)); 
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
// t3DModel		//////////////////////////////////////////////////////////////////////
