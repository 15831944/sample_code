


#include <vgStableHeaders.h>
#include <vgEffect/vgParticle.h>


#include <vgConf/vgcoExeConfig.h>

#define VGDATA_PATHCONF						"DataPathConfig"
#define VGDATA_PATHCONF_TEXTURE_PATH		"TexturePath"

#define VGDATA_FILECONF						"DataFileConfig"
#define VGDATA_FILECONF_TEXTURE_NAME		"ParticleTexName"

namespace vgMod 
{
	
	//----------------------------------------------------------------
	void vgParticle::translate(Vector3 offset)		//ƽ��
	{
		SetPosition(m_Position + offset);

		m_prePositon = m_Position;
		ComputeBoundBox();

		UpdatePos();
	}	//	translate(Vector3 offset)
	
	//----------------------------------------------------------------
	unsigned long vgParticle::PositDataToNode(char *data  )  //���ļ���ȡ
	{
		unsigned long pos = 0;
		
		//��ȡ�ڵ��ID	�� ���������
// 		m_id = *(int *)(data + pos);		
// 		pos += sizeof(int);
		for (int i = 0 ; i < 20; i ++)
		{
			m_LayerName[i] = *(data + pos);
			pos ++; 
		}

		for (int i = 0 ; i < 20; i ++)
		{
			m_name[i] = *(data + pos);
			pos ++; 
		}
		
		//��ȡ�ɼ��Ա��
		m_isVisible = *(bool *)(data + pos);
		
		pos += sizeof(bool);
		
		//��ȡ������
		char name[20];						
		
		for (int j =0; j < 20; j++)
		{
			name[j] = *(data + pos);
			
			pos ++;
		}
		m_texturename = name;
		
		//��ȡ�ڵ����ĵ�λ��
		m_Position  = *(Vector3 *)(data + pos);	
		pos += sizeof(Vector3);
		
		
		//д�������� ���߶ȡ���ȡ��ٶȡ���С
		m_height = *(float *)(data + pos);	
		pos += sizeof(float);
		
		m_width = *(float *)(data + pos);	
		pos += sizeof(float);
		
		m_speed = *(float *)(data + pos);	
		pos += sizeof(float);

		m_PartLenth = *(float *)(data + pos);	
		pos += sizeof(float);

		m_PartWidth = *(float *)(data + pos);	
		pos += sizeof(float);
		
		//��ȡ������Ŀ
		m_nParticleCount = *(int *)(data + pos);	
		pos += sizeof(int);
		
		//���ò�������ʼ��
		//SetCenter(m_Position);
		SetCount(m_nParticleCount);

		SetHeight(m_height);
		SetWidth(m_width);
		SetSpeed(m_speed);
		SetPartLength(m_PartLenth);
		SetPartWidth( m_PartWidth);

		Initialize();
		
		m_prePositon = m_Position;	
		ComputeBoundBox();
		
		return pos;		
	}


	
	//----------------------------------------------------------------
	void vgParticle::WriteNodeToVG(CFile &fp)	//д���ļ�
	{
		int i;

		i = VG_PARTICLE;
		long newpos;

		//fp.Write(&m_id , sizeof(int));                 //д�ýڵ��ID		

		Posinfile = fp.GetPosition();

		fp.Write(&nodelength, sizeof(long));			
		fp.Write(&i , sizeof(int));	                //д����type

		//i = 2;									//д������
		//fp.Write(&i, sizeof(int));
		//WriteSubTypeToVG(fp);
		fp.Write(&m_eSubType, sizeof(E_PARTICLE_TYPE) ); 


		fp.Write(m_LayerName , 20);					//������

		fp.Write(m_name, 20);						//�ڵ�����

		fp.Write(&m_isVisible, sizeof(bool));			//д�ɼ��Ա��

		/*	char name[20]=" ";*/			
		fp.Write(m_texturename.c_str(), 20);			//д������

		fp.Write(&m_Position.x , sizeof(float));		//д�ڵ����ĵ�λ��
		fp.Write(&m_Position.y, sizeof(float));	
		fp.Write(&m_Position.z , sizeof(float));

		fp.Write(&m_height, sizeof(float));	//д�������� ���߶ȡ���ȡ��ٶȡ���С
		fp.Write(&m_width, sizeof(float));
		fp.Write(&m_speed, sizeof(float));
		fp.Write(&m_PartLenth, sizeof(float));
		fp.Write(&m_PartWidth, sizeof(float));

		fp.Write(&m_nParticleCount, sizeof(int));		//д������Ŀ
		newpos = fp.GetPosition();

		nodelength = newpos - Posinfile- sizeof(long);

		fp.Seek(Posinfile, CFile::begin);

		fp.Write(&nodelength, sizeof(long));

		fp.SeekToEnd();
	}	//	WriteNodeToVG(CFile &fp)

	
	//----------------------------------------------------------------
	BoundingBox vgParticle::ComputeBoundBox()
	{
		// �ɲ���m_height��m_width��m_speed������Ч�ռ䷶Χ
		float	gravity =-m_speed / 1000.0f;
		float	maxVelocity= m_height/ 100.0f;
		float	maxTheta   = (m_width + 2.0f)*PI/ 180.0f;
		
		float	maxHeight = (maxVelocity*maxVelocity) 
			/ (-2.0f*gravity); // Smax = v^2 / 2*a;
		float   maxWidth  = (maxVelocity*maxVelocity*sin(maxTheta)) 
			/ (-gravity);
		
		m_BoundryBox.maxPoint.x = m_Position.x + maxWidth;
		m_BoundryBox.maxPoint.y = m_Position.y + maxHeight/2;
		m_BoundryBox.maxPoint.z = m_Position.z + maxWidth;
		
		m_BoundryBox.minPoint.x = m_Position.x - maxWidth;
		m_BoundryBox.minPoint.y = m_Position.y - maxHeight/2;
		m_BoundryBox.minPoint.z = m_Position.z - maxWidth;
		
		return m_BoundryBox;	
	}	//	ComputeBoundBox() 
	

	//----------------------------------------------------------------
	vgParticle::vgParticle()
		:_uniqueID(vgKernel::UniqueIDFactory::getUniqueID() )
		,vgMod::Node( NODE_TYPE_PARTICLE )
	{
		m_Position.x = 0.0f;
		m_Position.y = 0.0f;
		m_Position.z = 0.0f;
		pMats		 = NULL;

		m_nParticleCount = PARTICLECOUNT;
		m_bStopSE	= TRUE;
		m_bSwitchSE = FALSE;
		m_height	= 0.0f;
		m_width		= 0.0f;
		m_speed		= 0.0f;


		m_numOfFaces = 499;

		TYPE = VG_PARTICLE;

		configTextureName();
	}

	void vgParticle::configTextureName()
	{
#if 0
		String exepath = vgKernel::SystemUtility::getCurrentProcessPath();

		String texturePath =  VG_EXECONF_GET_STRING( 
			VGDATA_PATHCONF,  
			VGDATA_PATHCONF_TEXTURE_PATH);

		String textureName =  VG_EXECONF_GET_STRING( 
			VGDATA_FILECONF,  
			VGDATA_FILECONF_TEXTURE_NAME);

		texturePath = exepath + texturePath;
		m_texturename = texturePath + textureName;
#else

		m_texturename =  VG_EXECONF_GET_STRING( 
			VGDATA_FILECONF,  
			VGDATA_FILECONF_TEXTURE_NAME);

#endif
	}
	
	
	//----------------------------------------------------------------
	vgParticle::~vgParticle()	
	{
		if ( pMats )
		{
			delete pMats;
			pMats	= NULL;
		}
	}






	
	
	//--�̳�Renderer�����Ľӿ�-------------------------------------------
	float	vgParticle::setDistanceToViewer( const vgKernel::Vec3& viewer_pos )
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



	
	
	//----------------------------------------------------------------
	std::pair<bool, float> vgParticle::testIntersectWithTriangles( 
		const vgKernel::Ray& ray )

	{
		std::pair<bool, float> result;

		bool test_result = false;
		if (m_isVisible == false )
		{
			result.first = false;
			return result;
		}

		// ����

		result = vgKernel::Math::intersects( ray , getBoundingBox() );

		if( result.first == true )
		{
			test_result = true;
			goto TEST_INTERSECTION_END;
		}

#if _DEBUG
		VG_CHECK_OPENGL_ERROR();
#endif

TEST_INTERSECTION_END:
		return result;
	}

}	//	namespace vgCore

