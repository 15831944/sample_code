


#ifndef __VGTABSTRACTTREE_H__
#define __VGTABSTRACTTREE_H__

#include <vgKernel/vgkForward.h>
#include <vgKernel/vgkRenderer.h>
#include <vgKernel/vgkAbstractTree.h>


	
namespace vgTree {



	/**
		@date 	2008/08/05  21:06	
		@author  leven
	
		@brief 	�κμ̳�AbstractTree���඼Ҫ�������µ�ְ��:
			1. �������AbstractLeaf�������ü�,������Ӷ���.
			2. ������Ŀ��Ӷ�����Ҫ����DistanceToViewer.
			���б�Ҫ,�Կ��Ӷ�������Ⱦ�Ⱥ�˳�������.
			����,�ӵ����λ������Ⱦ,����������������io����.
	
		@see    
	*/

	enum	TREE_TYPE
	{
		TREE_LINEAR,
		TREE_BINARY,
		TREE_QUAD
	};

	class AbstractTree : public vgKernel::AbstractTree
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


		/**
			��Ⱦ���ṹ��������
		*/
		virtual void renderTreeStruct(bool bDrawStruct = false)		= 0;
		
		/**
			�л�������ʽ������������
		*/
		virtual void switchTreeStruct( )		= 0;
		virtual bool isUseTree(	)	= 0;
		virtual TREE_TYPE	getType()	= 0;

	};


	
	
}// end of namespace vgTree
	


#endif // end of __VGTABSTRACTTREE_H__