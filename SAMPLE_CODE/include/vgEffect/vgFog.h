
#ifndef  _VGFOG_H_INCLUDED_
#define  _VGFOG_H_INCLUDED_

//includes	
//#include <vgStableHeaders.h>							//��ͼ�ο�	
#include <vgMath/vgfPrerequisites.h>		//����ѧ��
#include <vgEffect/vgParticle.h>

/************************************************************************/
/*    vgFog�������
���ܣ�����һ������
�÷���  1����opengl������ʵ����һ������ vgFog pFog;
		2����opengl��ʼ��ʱ���ò���         pFog.Set*();
		3����opengl��ʼ��ʱִ�г�ʼ��       pFog.Init();
		4����opengl��Ⱦʱִ��				pFog.Render();
		5, ���κ�ʱ�򻹿ɿ���ѩ��			pFog.Open() ;
ע�⣺      
/************************************************************************/

namespace vgMod
{
	
	// class    vgFog
	class    vgFog : public vgParticle
	{
		VGK_RENDERER_IN_DECLARE( vgFog )

	public:
		vgFog();
		~vgFog();
		void Initialize();
		void Render();
//		void StopSpecialEffect(BOOL m_bStopSE);
		
	public:
		enum    FOGMODE
		{
			typeExp		= GL_EXP, 
			typeExp2	= GL_EXP2, 
			typeLine	= GL_LINEAR 
		};
		
		typedef struct   
		{
			GLfloat start;
			GLfloat	end;
		} FOGBOUND;	

		// ��������ܶȱ仯ģʽ
		void SetMode( FOGMODE fogMode );  //vgFog::typeExp��typeExp2��typeLine
		void SetMode( GLuint  fogModeIndex ); //0~2
		
		// ���������ɫ
		void SetColor(GLfloat r, GLfloat g, GLfloat b,float a=1.0f);
		void SetColor( GLuint fogColorIndex ); //0~1
			
		// ��������ܶ�
		void SetDensity(GLfloat fogDensity);
		
		// �������Զ����Zֵ
		void SetBound( FOGBOUND fogBound);
		void SetBound( GLfloat fogStart, GLfloat fogEnd );
		
	public:
		GLuint	fogModeIndex;
		FOGMODE  fogModeVar;
		
		GLuint  fogColorIndex;
		GLfloat fogColorVar[4];
		
		GLfloat	fogDensity;
		FOGBOUND fogBound;
		
		BOOL	bfogOpen;
	};//class    vgFog
	

	VGK_RENDERER_OUT_DECLARE( vgFog )


}	//namespace vgCore


#endif // _VGFOG_H_INCLUDED_