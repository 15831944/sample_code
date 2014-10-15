
#ifndef  _VGCLOUD_H_INCLUDED_
#define  _VGCLOUD_H_INCLUDED_

//includes	
//#include <vgStableHeaders.h>							//��ͼ�ο�	
#include <vgMath/vgfPrerequisites.h>		//����ѧ��
#include <vgEffect/vgParticle.h>

/************************************************************************/
/*    vgCloud�������
���ܣ�����һ������
�÷���  1����opengl������ʵ����һ������ vgCloud pCloud;
		2����opengl��ʼ��ʱ���ò���         pCloud.Set*();
		3����opengl��ʼ��ʱִ�г�ʼ��       pCloud.Init();
		4����opengl��Ⱦʱִ��				pCloud.Render();
		5, ���κ�ʱ�򻹿ɿ���ѩ��			pCloud.Open() ;
ע�⣺      
/************************************************************************/

namespace vgMod
{
	
	
	class    vgCloud : public vgParticle
	{
	public:
		vgCloud();
		~vgCloud();
		void Init();
		void Render();
		void Open( BOOL bfogOpen=TRUE);
		
	public:
		
		typedef struct   
		{
			GLfloat width;
			GLfloat	height;
		} CLOUDBOUND;	

		// �����Ʋʵ�λ��
	    void SetCenter(float x = 0.0f, float y = 0.0f, float z = 0.0f) ;
		void SetCenter(Vector3 pos) ;

		// �����ƲʵĴ�С
		void SetBound( CLOUDBOUND cloudBound);
		void SetBound( GLfloat width, GLfloat height );
		
	public:

		CLOUDBOUND cloudBound;		
		BOOL	bcloudOpen;
		TexturePtr	m_pTexture;
	};
	
}	//namespace vgCore


#endif // _VGCLOUD_H_INCLUDED_