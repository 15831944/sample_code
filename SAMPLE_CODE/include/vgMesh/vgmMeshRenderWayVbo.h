


#ifndef __VGMMESHRENDERWAYVBO_H__
#define __VGMMESHRENDERWAYVBO_H__


#include <vgMesh/vgmDefinition.h>
#include <vgKernel/vgkForward.h>


	
namespace vgMesh {


	#define  VG_NULL_VBO_ID	-1

	/**
		used for MeshRenderWayVbo only
	*/
	class  VGM_EXPORT VboMetaFaceStruct
	{
	public:
		VboMetaFaceStruct();
		~VboMetaFaceStruct();
	
		int		_numberOfTexture;
		String* _texFileName;

		TexObsPointer* _texObserver;

		GLuint _elementBufferObjectID;

		// ע��,������int���͵�ԭ����Ŀ.��3�ı���
		long _numOfElements;
	};


	class MeshRenderWayVertexArray;

	/**
		@date 	2008/07/17  15:23	
		@author  leven
	
		@brief 	ע��,ֻ�������߳��д���,��Ϊ���캯���к��д���
		vbo������
	
		@see    
	*/
	class  VGM_EXPORT MeshRenderWayVbo : public MeshRenderWay
	{
	public:
		MeshRenderWayVbo( MeshRenderWayVertexArray* vertexarray );
		virtual ~MeshRenderWayVbo();

		virtual void render(const int& lod, int force_render_in_num_layers ,
			RgbaQuad* colorval );

		std::pair<bool, float> testIntersection(const vgKernel::Ray* ray);

		virtual bool testIntersection(const vgKernel::Segment3* segment);

		virtual bool initialiseInMainThread();
	
	private:

		void renderInOneLayer();
		void renderInTwoLayer();

		// ��2���bakeֻ��Ⱦһ��
		void renderTwoLayerInOneLayer();

		void requireTexObserverByLod( const int& lod );

		bool generateVbo( MeshRenderWayVertexArray* vertexarray );

	private:

		int		_renderType;

		// ��һ��lod�Ĳ���.���ڱȽ�lod�Ƿ���Ҫ����
		int _preLod;

		long _layersOfTexture;


		bool _isReadyForVBORender;

		GLuint _vertexBufferObjectID;

		long	_numberOfMetaFaces;
		VboMetaFaceStruct* _metaFaces;

		TextureManager* _texMgr;
	};
	
	
}// end of namespace vgMesh
	


#endif // end of __VGMMESHRENDERWAYVBO_H__