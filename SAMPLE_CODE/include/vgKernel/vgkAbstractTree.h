


#ifndef __VGKABSTRACTTREE_H__
#define __VGKABSTRACTTREE_H__

#include <vgKernel/vgkForward.h>

	
namespace vgKernel {

	/**
		@date 	2009/06/03  10:46	
		@author  xy
	
		@brief 	
	
		@see    
	*/

	class AbstractTree
	{
	public:
		AbstractTree()
		{

		}
		virtual ~AbstractTree()
		{

		}


	public:

		// ����Զ���ü��Ĳ���
		// ��Ϊ����ʱ,��ʾdisable���������
		virtual void setDistanceCull( float distance_cull ) = 0;

		virtual float getDistanceCull() = 0;

		/**
		�������
		*/
		virtual void setRendererPackage( vgKernel::RendererPackage* package ) = 0;


		/**
		���¹����ڲ��ṹ.
		����,�Ĳ����ڵ�һ��getVisibleSet��ʱ������.
		�ƶ������,��Ҫ��������,��ʹ�ô˺���.
		*/
		virtual void invalidate() = 0;

		/**
		ע��,visible_set���������
		Tree��������㲢����.
		����Ϊ�˷�ֹ����Ҫ�Ŀ���.
		*/
		virtual void getVisibleSet( vgKernel::RendererPackage* visible_set ) = 0;

	};


	
}// end of namespace vgKernel
	


#endif // end of __VGKABSTRACTTREE_H__