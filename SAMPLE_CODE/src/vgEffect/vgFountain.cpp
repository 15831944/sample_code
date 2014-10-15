
// includes
#include <vgStableHeaders.h>

#include "vgEffect/vgFountain.h"
#include "vgCam/vgcaCamManager.h"
//#include <vgMod/vgQuadtreeSceneManager.h>

#include <vgConf/vgcoExeConfig.h>

#define VGDATA_PATHCONF						"DataPathConfig"
#define VGDATA_PATHCONF_TEXTURE_PATH		"TexturePath"

#define VGDATA_FILECONF						"DataFileConfig"
#define VGDATA_FILECONF_TEXTURE_NAME		"FountainTexName"

// class vgFountain
namespace vgMod
{
	VGK_RENDERER_IMPLEMENT( vgFountain,RENDERER_TYPE_PARTICLE )
	
	vgFountain::vgFountain() 
	{
		m_pdrop	= NULL ;

		m_height = 25.0f;
		m_width  = 10.0f;
		m_speed	 = 5.0f;
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
		//m_texturename = "WaterDrop.jpg";
#else
		m_texturename =  VG_EXECONF_GET_STRING( 
			VGDATA_FILECONF,  
			VGDATA_FILECONF_TEXTURE_NAME);
#endif
		strcpy( m_name, "fountain");
		
		//TYPE = VG_FOUNTAIN;
		
		//m_RenderTypeInt = 2;
		m_RenderTypeInt = 1;

		//pSceneManager	= vgCore::QuadtreeSceneManager::getSingletonPtr() ;
		//pCurrentCamera	= &pSceneManager->GetCurrentCamera() ;
		//pCurrentCamera = NULL; //���޸ģ�

		m_PartLenth		=	0.1f;
		m_PartWidth		=	0.1f;

		m_eSubType = PARTICLE_FOUNTAIN;
	}
	
	vgFountain::~vgFountain()
	{
		delete[] m_pdrop;

		m_pdrop = NULL;
	} 
	
	
	void vgFountain::Initialize()
	{
		//assert( m_pTexture.isNull() );
		m_pTexture = vgCore::TextureManager::getSingleton().getTexturePtr( m_texturename );

		if ( m_pdrop)
		{
			delete[] m_pdrop;
		}
		m_pdrop = new tagDROP[ m_nParticleCount ];

		srand( GetTickCount() );


		GLuint	delayFrame ;
		delayFrame	= int ( m_height/m_speed * 20.0f + 0.5f);
		GLuint loop1;
		for (loop1=0;loop1< m_nParticleCount;loop1++)
		{

			m_pdrop[loop1].lifeFrame	=	rand()%delayFrame ;

			m_pdrop[loop1].active		=	false;
						
			m_pdrop[loop1].position =  m_Position ;
				
			m_pdrop[loop1].vgen.x =  m_height/100.0f;  //5.0f/100
			m_pdrop[loop1].vgen.y = float (rand()%(int)m_width + 2 ) * (float)PI / 180.0f ;
			m_pdrop[loop1].vgen.z = float (rand()%360)  * (float)PI / 180.0f ;
			
			m_pdrop[loop1].vlen.x  = m_pdrop[loop1].vgen.x * (float)sin( m_pdrop[loop1].vgen.y )  *
				(float)cos( m_pdrop[loop1].vgen.z );
			m_pdrop[loop1].vlen.y  = m_pdrop[loop1].vgen.x * (float)cos( m_pdrop[loop1].vgen.y ) ;
			m_pdrop[loop1].vlen.z  = m_pdrop[loop1].vgen.x * (float)sin( m_pdrop[loop1].vgen.y )  *
				(float)sin( m_pdrop[loop1].vgen.z ) ;
			
			m_pdrop[loop1].acc.x   = 0.0f;
			m_pdrop[loop1].acc.y   =-m_speed/1000.0f/*-0.005f*/;
			m_pdrop[loop1].acc.z   = 0.0f;				// Set Pull On Z Axis To Zero

			m_pdrop[loop1].bb.setHeight( m_PartLenth) ;
			m_pdrop[loop1].bb.setWidth( m_PartWidth) ;
			m_pdrop[loop1].bb.setTexturePtr( m_pTexture );
			m_pdrop[loop1].bb.setRenderType( Billboard::/*RENDERTYPE_EYE_ORIENTED*/RENDERTYPE_ROTATE_Y );
		}
		
		
				
	} // void vgFountain::Init()
	
	void vgFountain::Render()
	{
		UpdateEachFrame();

		//glDisable(GL_DEPTH_TEST);

		//glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
		glBlendFunc(GL_SRC_ALPHA,GL_ONE);
		glEnable(GL_BLEND) ;
		//glDisable(GL_BLEND) ;

		glAlphaFunc(GL_GREATER, 0.0f) ;
		//glEnable(GL_ALPHA_TEST);
		glDisable(GL_ALPHA_TEST);

		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE/*GL_REPLACE*/);
		glEnable(GL_TEXTURE_2D) ;

		m_pTexture->useBind();

		glColor4f(1.0f  ,1.0f  ,1.0f ,1.0f);
				
		for ( m_mapFountainParticleItor  = m_mapFountainParticle.end(), m_mapFountainParticleItor --;
			m_mapFountainParticleItor != m_mapFountainParticle.begin();
			m_mapFountainParticleItor -- )
		{
			pCurrentParticle	= m_mapFountainParticleItor->second ;

			pCurrentParticle->bb.setPosition( pCurrentParticle->position );

			pCurrentParticle->bb.updateCurrentViewVectors();

			pCurrentParticle->bb./*drawUsingEyeOriented*/drawUsingRotatedY();
			//float x=pCurrentParticle->position.x;						// Grab Our Particle X Position
			//float y=pCurrentParticle->position.y;						// Grab Our Particle Y Position
			//float z=pCurrentParticle->position.z;					// Particle Z Pos + Zoom

			//glBegin( GL_QUADS );		
			//glTexCoord2d(1,1); glVertex3f(x+0.2f, y+0.4f, z+0.0f); // Top Right
			//glTexCoord2d(0,1); glVertex3f(x-0.2f, y+0.4f, z+0.0f); // Top Left
			//glTexCoord2d(0,0); glVertex3f(x-0.2f, y-0.4f, z+0.0f); // Bottom Right
			//glTexCoord2d(1,0); glVertex3f(x+0.2f, y-0.4f, z+0.0f); // Bottom Left
			//glEnd();
			
		}// for (int loop)
		
		glTexEnvf    (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

		glDisable(GL_BLEND) ;
		glDisable(GL_TEXTURE_2D) ;
		glDisable(GL_ALPHA_TEST) ;
		//glEnable(GL_DEPTH_TEST) ;
		
	} // void vgFountain::Render()

	void vgFountain::UpdateEachFrame()
	{

		m_mapFountainParticle.clear();

		GLuint	delayFrame ;
		delayFrame	= int ( m_height/m_speed * 20.0f + 0.5f);

		GLuint loop1;

		for (loop1=0;loop1< m_nParticleCount ;loop1++)					// Loop Through All The Particles
		{
			if ( 0 == m_pdrop [loop1].lifeFrame )
			{
				m_pdrop [loop1].active	=	true;
			}
			else
			{
				m_pdrop [loop1].lifeFrame -- ;
			}

			if ( m_pdrop [loop1].active )							// If The Particle Is Active
			{	
				// ���� �����ӵ�����Զ��˳�� ������Ⱦ����

//////////////////////////////////////////////////////////////////////////chunyongma
				//vgKernel::Vec3 &pp =  QuadtreeSceneManager::getSingleton().m_pCurrentCamera->getCurrentPosition();
				vgKernel::Vec3 &pp =  vgCam::CamManager::getSingleton().getCurrentPosition();

				rayFromEye =  Vector3( pp.x, pp.y, pp.z)   - m_pdrop [loop1].position;

				distanceFromEye	=  rayFromEye.length() ;

				m_mapFountainParticle.insert( m_pairFountainParticle(distanceFromEye,  m_pdrop + loop1 ) );

				// Update & Prepare for the next flame
				m_pdrop [loop1].position	+=  m_pdrop [loop1].vlen ;

				m_pdrop [loop1].vlen		+=  m_pdrop  [loop1].acc ;
			
				if ( m_pdrop [loop1].position.y < m_prePositon.y )
				{
					//srand( GetTickCount() );

					m_pdrop[loop1].position =  m_Position ;

					m_pdrop[loop1].vgen.x =  m_height/100.0f;  //5.0f/100
					m_pdrop[loop1].vgen.y = float (rand()%(int)m_width + 2 ) * (float)PI / 180.0f ;
					m_pdrop[loop1].vgen.z = float (rand()%360)  * (float)PI / 180.0f ;

					m_pdrop[loop1].vlen.x  = m_pdrop[loop1].vgen.x * (float)sin( m_pdrop[loop1].vgen.y )  *
						(float)cos( m_pdrop[loop1].vgen.z );
					m_pdrop[loop1].vlen.y  = m_pdrop[loop1].vgen.x * (float)cos( m_pdrop[loop1].vgen.y ) ;
					m_pdrop[loop1].vlen.z  = m_pdrop[loop1].vgen.x * (float)sin( m_pdrop[loop1].vgen.y )  *
						(float)sin( m_pdrop[loop1].vgen.z ) ;

				}// if ( m_pdrop [loop1].position.y< m_Position.y ) ���������棬��������

			}// if ( m_pdrop [loop1].active ) ���ӵȴ�ʱ�̽�����������

		} // for(loop1) ������������

	}// void vgFountain::UpdateEachFrame()

	BoundingBox vgFountain::ComputeBoundBox()
	{
		maxHeight	=	( m_height * m_height ) / ( 20.0f * m_speed ) ;
		maxWidth	=	maxHeight * sin (PI * m_width / 90.0f) * 1.25f;

		m_BoundryBox.maxPoint.x = m_Position.x + maxWidth;
		m_BoundryBox.maxPoint.y = m_Position.y + maxHeight;
		m_BoundryBox.maxPoint.z = m_Position.z + maxWidth;

		m_BoundryBox.minPoint.x = m_Position.x - maxWidth;
		m_BoundryBox.minPoint.y = m_Position.y ;
		m_BoundryBox.minPoint.z = m_Position.z - maxWidth;

		return	m_BoundryBox;
	}	//BoundingBox ComputeBoundBox()

	void vgFountain::UpdatePos()
	{
		m_Position.x = (m_BoundryBox.maxPoint.x + m_BoundryBox.minPoint.x) / 2;

		m_Position.y = m_BoundryBox.minPoint.y;

		m_Position.z = (m_BoundryBox.maxPoint.z + m_BoundryBox.minPoint.z) / 2;

		m_prePositon = m_Position;

		/*vgCore::QuadtreeSceneManager &mgr = 
			vgCore::QuadtreeSceneManager::getSingleton();

		if (mgr.GetNodeAbstract(m_id))
		{
			mgr.GetNodeAbstract(m_id)->GetBoundryBox() = m_BoundryBox;

			mgr.GetNodeAbstract(m_id)->m_Position = m_Position;
		}*/

	}

	void vgFountain::translate(Vector3 offset)		//ƽ��
	{
		Initialize();


		m_prePositon = m_Position;

		SetPosition(m_Position + offset  );

		ComputeBoundBox();

		

		UpdatePos();
	}	//	translate(Vector3 offset)
	
	
	//void vgFountain::WriteNodeToVG(CFile &fp)	//д���ļ�
	//{


	//	//fp.Write(&m_id , sizeof(int));                 //д�ýڵ��ID		

	//	Posinfile = fp.GetPosition();
	//	fp.Write(&nodelength, sizeof(long));	

	//	int i = VG_PARTICLE;
	//	fp.Write(&i , sizeof(int));	               //д����type

	//	i = 0;									//д��Ȫ����
	//	fp.Write(&i, sizeof(int));
	//	fp.Write(m_LayerName , 20);						

	//	fp.Write(&m_isVisible, sizeof(bool));			//д�ɼ��Ա��

	//	/*	char name[20]=" ";*/			
	//	fp.Write(m_texturename.c_str(), 20);			//д������

	//	fp.Write(&m_Position , sizeof(Vector3));		//д�ڵ����ĵ�λ��

	//	fp.Write(&m_height, sizeof(float));	//д�������� ���߶ȡ���ȡ��ٶȡ���С
	//	fp.Write(&m_width, sizeof(float));
	//	fp.Write(&m_speed, sizeof(float));
	//	fp.Write(&m_PartLenth, sizeof(float));
	//	fp.Write(&m_PartWidth, sizeof(float));

	//	fp.Write(&m_nParticleCount, sizeof(int));		//д������Ŀ

	//	long newpos = fp.GetPosition();
	//	nodelength = newpos - Posinfile- sizeof(long);
	//	fp.Seek(Posinfile, CFile::begin);
	//	fp.Write(&nodelength, sizeof(long));
	//	fp.SeekToEnd();
	//}	//	WriteNodeToVG(CFile &fp)

	
}	//	namespace vgCore
