


#ifndef __VGMMESHRENDERWAYDISPLAYLIST_H__
#define __VGMMESHRENDERWAYDISPLAYLIST_H__


#if 0

#include <vgKernel/vgkForward.h>
#include <vgMesh/vgmMeshRenderWayVertexArray.h>

	
namespace vgMesh {


	#define  VG_NULL_VBO_ID	-1



	class MeshRenderWayVertexArray;

	/**
		@date 	2008/07/17  15:23	
		@author  leven
	
		@brief 	ע��,ֻ�������߳��д���,��Ϊ���캯���к��д���
		vbo������
	
		@see    
	*/

	class MeshRenderWayDisplayList : public MeshRenderWay
	{
	public:
		friend class MeshRenderWayVertexArray;

		MeshRenderWayDisplayList( MeshRenderWayVertexArray* vertexarray);
		virtual ~MeshRenderWayDisplayList();

		virtual void render(const int& lod);

		bool testIntersection(const vgKernel::Ray* ray);

		virtual bool testIntersection(const vgKernel::Segment3* segment);

		virtual bool initialiseInMainThread();
	
	private:

		void renderInOneLayer();
		void renderInTwoLayer();

		void requireTexObserverByLod( const int& lod );

		bool generateDisplaylist( MeshRenderWayVertexArray* vertexarray  );

	private:

		//////////////////////////////////////////////////////////////////////////

		MeshRenderWayVertexArray* _vertexarray;

		GLint *_lists;
		//////////////////////////////////////////////////////////////////////////

	};
	
	
}// end of namespace vgMesh
	

#endif


#endif // end of __VGMMESHRENDERWAYVBO_H__