

#ifndef __VGMMESHDATA_H__
#define __VGMMESHDATA_H__


#include <vgMesh/vgmDefinition.h>
#include <vgKernel/vgkForward.h>

#include <vgMesh/vgmMeshRenderWayVertexArray.h>

namespace vgMesh {

	/**
		@date 	2008/07/01  20:58	
		@author  leven
	
		@brief 	
	
		@see    
	*/
	class  VGM_EXPORT MeshData
	{
	public:
		MeshData();

		~MeshData();

	public:

		bool initialise( char* buf , long bufsize );

		bool initInMainThread();

		// lod����:
		// Ϊ0ʱ,��ʾ�����,Ҳ������Զ
		// Ϊ2ʱ,��ʾ������,���
		// ����ֵ��Ч
		// force_render_in_num_layersǿ����Ⱦ��ͼ������0ΪĬ�ϣ����Ϊ2
		void render( int lod , int force_render_in_num_layers ,
			bool transparent_mode ,
			RgbaQuad* colorval );


		void convertToVboRenderWay();

		//void convertToDisplayListRenderWay();//chunyongma

		MeshRenderWay* getRenderWay()
		{
			return _renderWay;
		}

	private:

		void destroy();
		void setDefault();

	private:

		MeshRenderWay* _renderWay;

	};
	
	
	
}// end of namespace vgMesh

#endif // end of __VGMMESHDATA_H__