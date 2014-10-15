


#ifndef __VGMMESHITEM_H__
#define __VGMMESHITEM_H__


#include <vgMesh/vgmDefinition.h>
#include <vgKernel/vgkForward.h> 
#include <vgKernel/vgkBox.h>
#include <vgKernel/vgkUniqueID.h>
#include <vgAsyn/vgaIoRequest.h>
#include <vgMesh/vgmMeshData.h>
#include <vgMesh/vgmMesh.h>
#include <vgThreads/AutoEvent.h>
#include <vgKernel/vgkVec3.h>
#include <vgKernel/vgkRenderer.h>
#include <vgKernel/vgkMat4.h>
#include <vgKernel/vgkRendererObserver.h>
#include <vgKernel/vgkShadowCastObject.h>
#include <vgKernel/vgkTransparentObject.h>
#include <vgKernel/vgkColorableObject.h>


namespace vgMesh {

	using namespace vgKernel;

	class MeshData;
	class MeshDataPlus;
	class VgmBrief;

	class IoReqMeshItemDumpInfo;

	/*
	5010 -- 5030֮��ΪQuadTreeSceneMananger�Ĺ�Ͻ��Χ֮��
	*/
	const vgKernel::RendererType RENDERER_TYPE_MESH = 5000;


	/**
		@date 	2008/07/01  20:58	
		@author  leven
	
		@brief 	
	
		@see    
	*/
	class  VGM_EXPORT MeshItem 
		: public vgKernel::Renderer, 
		public vgKernel::ShadowCastObject,
		public vgKernel::TransparentObject,
		public vgKernel::ColorableObject
	{
		VGK_RENDERER_IN_DECLARE( MeshItem )
	public:

		MeshItem();

		~MeshItem();
	
	public:

		bool initialise(  VgmBrief* vgmbrief , vgAsyn::FileHandle* filehandle );

		// ��Ӱ�ķ����ڵ�һ�ε�ʱ���������
		// ������ķ�������ʱ��Ҳʹ�ô˺�������֪ͨ��
		virtual void onLightDirectionChanged( const vgKernel::Vec3& light_dir );

		// ��Ⱦshadow��caster
		virtual void renderInShadowMode();

		virtual void turnOffShadowCast();

		virtual void turnOnShadowCast( const vgKernel::Vec3& light_dir,
			bool force_to_load );


		bool dumpInfoToVgmBrief( VgmBrief* vgmbrief );

		bool dumpFileInfoToBuffer( char*& whole_file , long& file_length );

		void destroy();

#if 0
		// ������һ��ʼȫ����������.
		// ������ͬʱ,����Ƿ���Ҫ����
		// ע��!��ʱ����!
		inline void addReference( const int& lod )
		{
			assert( lod >= VG_LOD_COARSEST );
			assert( lod <= VG_LOD_CLEAREST );
			_meshLod[ _distanceLod ]->addRef();
		}

		inline void releaseReference( const int& lod )
		{
			assert( lod >= VG_LOD_COARSEST );
			assert( lod <= VG_LOD_CLEAREST );
			_meshLod[ _distanceLod ]->releaseRef();
		}
#endif


		void render();

		String getDetails() ;

		/**
			�̳�Renderer�����Ľӿ�
		*/
		virtual UniqueID getUniqueID()
		{
			return _uniqueID;
		}

		/**
			�̳�Renderer�����Ľӿ�
		*/
		virtual String getName()
		{
			return _meshName;
		}

		virtual String* getNamePtr()
		{
			return &_meshName;
		}
		/**
			�̳�Renderer�����Ľӿ�
		*/
		virtual void setName( const String& newname )
		{
			_meshName = newname;
		}


		Mesh* getMeshByLod( const int& lod )
		{
			assert( lod <= VGLOD_CLEAR );
			assert( lod >= VGLOD_COARSE );

			return _meshLod[ lod ];
		}

		/**
			�̳�Renderer�����Ľӿ�
		*/
		void translate(const float& x, const float& y, const float& z);

		/**
			�̳�Renderer�����Ľӿ�
		*/
		void rotateSelf(const float& angle, const float& x, const float& y, const float& z);

		/**
			�̳�Renderer�����Ľӿ�
		*/
		void scale(const float& cx, const float& cy, const float& cz, const float& x, const float& y, const float& z);

		/**
			�̳�Renderer�����Ľӿ�
		*/
		void rotate(const float& angle, const float& cx, const float& cy, const float& cz, const float& x, const float& y, const float& z); 

		inline float getSquaredDistanceToViewer() const
		{
			return _squaredDistanceToViewer;
		}

		inline int getDistanceLod() const
		{
			return _distanceLod;
		}

		inline const vgKernel::Mat4& getTranslationMaxtrix()
		{
			return _matrix;
		}
		/**
			�̳�Renderer�����Ľӿ�
		*/
		virtual vgKernel::Box getBoundingBox()
		{
			return _dynamicBox;
		}
		
		/**
			�̳�Renderer�����Ľӿ�
		*/
		virtual std::pair<bool, float> testIntersectWithTriangles( const vgKernel::Ray& ray );

		/**
			�̳�Renderer�����Ľӿ�
		*/
		virtual bool testIntersectWithTriangles( 
			const vgKernel::Segment3& segment );


		/**
			�̳�Renderer�����Ľӿ�
		*/
		virtual long getNumOfTriangles()
		{
			return _numberOfTriangles;
		}
		/**
			�̳�Renderer�����Ľӿ�
		*/
		virtual float setDistanceToViewer( const Vec3& viewer_pos  );

		virtual Vec3 getPosition()
		{
			return _dynamicBox.getCenter();
		}


		inline void setMeshName( const String& newname )
		{
			_meshName = newname;
		}

		/**
			�ͻ��˲���ɾ��.��Ҫ����,��ʹ��clone
		*/
		vgAsyn::FileInfo* getFileInfoByLod( const int& lod )
		{
			return _meshLod[ lod ]->_meshDataFileInfo;
		}

		/**
			ǧ��ע��setDistanceToViewer��addViewRef�Ĺ�ϵ.
			setDistanceToViewerҪ��addViewRef֮ǰִ��.
			������addViewRef����lod�ı仯.
		*/
		void addViewRef();

		void releaseViewRef();

		int getViewRef() const
		{
			return _viewReference;
		}

		bool getLodMeshFlag() const
		{
			return _isLodMesh;
		}

		// ��δ��������!!!!ѡ��༭��������֮���ٲ���!
		void setLodMeshFlag( const bool& islodmesh )
		{
			_isLodMesh = islodmesh;
		}

		//_squareDistNear get ��set������д
		//_squareDistFar get ��set������д

		void setSquareDistNear(float squareDistNear)
		{
			_squareDistNear = squareDistNear;
		}

		float getSquareDistNear()
		{
			return _squareDistNear;
		}

		void setSquareDistFar(float squareDistFar)
		{
			_squareDistFar = squareDistFar;
		}

		float getSquareDistFar()
		{
			return _squareDistFar;
		}

		// �̳�TransparentObject�Ľӿ�
		virtual bool isTransparent();

		// �̳�TransparentObject�Ľӿ�
		virtual void setTransparent( bool set_to_transp = true );

	private:

		void updateMeshLod();
		void setDefault();

	private:

		// �Ƿ����lod��Ⱦ.
		// ��Ϊfalse,��ֻ��Ⱦ�ϸһ��.
		// Ĭ��Ϊtrue
		bool _isLodMesh;

		float _squareDistNear;
		float _squareDistFar;


		// �ӵ�����ü���.
		// ��������ӵ�ʱ���Ӽ���.
		// �뿪�ӵ�ʱ���ټ���.
		int _viewReference;

		float _squaredDistanceToViewer;

		// ͨ��_distanceToViewer�������lod
		int		_distanceLod;


		// ��ǰ��Lod,ע��,��_currentLod��_distanceLod
		// �൱��_preLod
		// ��ȷ����仯ʱ,�ŷ���addRef��releaseRef���� 
		// Ĭ�ϵ�Ϊ�����һ��,��_currentLod = 0;
		int		_currentLod;

		UniqueID _uniqueID;
	
		// ע��,���64���ֽڳ���.
		String _meshName;

		vgKernel::Box _dynamicBox;
		vgKernel::Box _staticBox;

		vgKernel::Mat4 _matrix;

		long	_numberOfTriangles;

		vgAsyn::FileInfo* _wholeFileInfo;

		
		Mesh* _meshLod[ VGLOD_N ];

		// ����addRef��ReleaseRef�ı��λ
		// ע��,�����Texture��ͬ��ԭ��,ֻ����ʱ����
		// ���Ա���Ƿ���addRef������.
		// ���������ͬlod���������Ե�����
		Mesh* _refMesh[ VGLOD_N ];

		bool _isTransparent;

	};
	

	typedef std::vector<MeshItem*> MeshItemPtrVec;

	typedef std::map<UniqueID, MeshItem*> MeshItemMap;
	typedef std::pair<MeshItemMap::iterator, bool> MeshItemInsertRes;

	/**
		ͬ��IO����,���ڱ���vgm�ļ�
		ע��,ɾ���������ⲿ���
	*/
	class IoReqMeshItemDumpInfo : public vgAsyn::IoRequest
	{
	public:
		IoReqMeshItemDumpInfo( vgAsyn::FileInfo* fileinfo  );
		virtual ~IoReqMeshItemDumpInfo();
	
		// no dispatch to main thread
		virtual bool anotherThreadHandler();

		virtual bool mainThreadHandler();

	public:

	};

	VGK_RENDERER_OUT_DECLARE( MeshItem )
	
}// end of namespace vgMesh

#endif // end of __VGMMESHITEM_H__