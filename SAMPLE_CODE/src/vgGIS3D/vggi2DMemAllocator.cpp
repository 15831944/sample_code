// 2DMemAllocator.cpp: implementation of the C2DMemAllocator class.
//
//////////////////////////////////////////////////////////////////////
#include <vgStableHeaders.h>
#include <vgGIS3D\vggi2DMemAllocator.h>


/*~~~~~~~��ά�������ݶ�̬����~~~~~~~~~~~~*/
//���亯��:
//AllocMemory2D(T ** &p,int rows,int columns)
//
//�ͷź���:
//FreeMemory2D(T **&pp)
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
namespace vgGIS3D
{

	C2DMemAllocator::C2DMemAllocator()
	{

	}

	C2DMemAllocator::~C2DMemAllocator()
	{

	}

	void C2DMemAllocator::FreeMemory2D(float **&pp)
	{
		//TRACE("in FreeMemory2D\n");
		if(pp)
		{//�ͷ�˳��::һάָ��-->��άָ��

			delete []pp[0];
			pp[0]=NULL;

			delete []pp;
			pp=NULL;
		}
	}


	BOOL C2DMemAllocator::AllocMemory2D(float ** &p,int rows,int columns)
	{

		float **pp;
		pp=NULL;
		pp=new float*[rows];
		if (!pp)
		{
			//TRACE("Alloc memory Open error!!\n");
			return FALSE;
		}

		pp[0]=new float [rows*columns];//allocate a big block

		if (!pp[0])
		{
			return FALSE;
		}

		for(int i=1;i<rows;i++)
		{
			pp[i]=pp[0]+(long)columns*i;
		}

		for(int j=0; j<rows;j++)
		{
			for(int i=0;i<columns;i++)
				pp[j][i] = 0.0f;
		}

		p = pp;

		return TRUE;
	}
}
