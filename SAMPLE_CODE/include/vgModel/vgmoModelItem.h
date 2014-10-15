


#ifndef __VGMO_MESHITEM_H__
#define __VGMO_MESHITEM_H__



#include <vgModel/vgmoDefinition.h>
#include <vgKernel/vgkForward.h> 
#include <vgKernel/vgkBox.h>
#include <vgKernel/vgkUniqueID.h>
#include <vgModel/vgmoModelData.h>

#include <vgKernel/vgkVec3.h>
#include <vgKernel/vgkRenderer.h>
#include <vgKernel/vgkMat4.h>
#include <vgKernel/vgkRendererObserver.h>
#include <vgKernel/vgkShadowCastObject.h>
#include <vgKernel/vgkTransparentObject.h>
#include <vgKernel/vgkColorableObject.h>


namespace vgModel {

	using namespace vgKernel;

	class ModelData;
	class VmdBrief;
	class FileHandleVmd;


	/*
	5010 -- 5030֮��ΪQuadTreeSceneMananger�Ĺ�Ͻ��Χ֮��
	*/
	const vgKernel::RendererType RENDERER_TYPE_MODEL = 15000;


	/**
		@date 	2008/07/01  20:58	
		@author  leven
	
		@brief 	
	
		@see    
	*/
	class  VGMO_EXPORT ModelItem 
		: public vgKernel::Renderer,
		public vgKernel::ShadowCastObject,
		public vgKernel::TransparentObject,
		public vgKernel::ColorableObject
	{
		VGK_RENDERER_IN_DECLARE( ModelItem )
	public:
		ModelItem();
		~ModelItem();
	
	public:



		// ��Ӱ�ķ����ڵ�һ�ε�ʱ���������
		// ������ķ�������ʱ��Ҳʹ�ô˺�������֪ͨ��
		virtual void onLightDirectionChanged( const vgKernel::Vec3& light_dir );

		// ��Ⱦshadow��caster
		virtual void renderInShadowMode();

		virtual void turnOffShadowCast();

		virtual void turnOnShadowCast( const vgKernel::Vec3& light_dir,
			bool force_to_load );



		// ����load��dump����
		virtual bool load( VmdBrief* vgmbrief , FileHandleVmd* filehandle ,
			int version );
		virtual bool dumpInfoToVmdBrief( VmdBrief* vgmbrief );
		virtual bool dumpFileInfoToBuffer( char*& whole_file , long& file_length );


		void destroy();

		String getDetails();

		/**
			�̳�Renderer�����Ľӿ�
		*/
		virtual void setVisible( const bool& visible )
		{
			_isVisible = visible; 
		}

		virtual void render();

		virtual bool getVisible()
		{
			return _isVisible;
		}
		virtual UniqueID getUniqueID()
		{
			return _uniqueID;
		}
		virtual String getName()
		{
			return _modelName;
		}
		virtual String* getNamePtr()
		{
			return &_modelName;
		}
		virtual void setName( const String& newname )
		{
			_modelName = newname;
		}
		void translate(const float& x, const float& y, const float& z);
		void rotateSelf(const float& angle, const float& x, const float& y, const float& z);
		void scale(const float& cx, const float& cy, const float& cz, const float& x, const float& y, const float& z);
		void rotate(const float& angle, const float& cx, const float& cy, const float& cz, const float& x, const float& y, const float& z); 

		virtual vgKernel::Box getBoundingBox()
		{
			return _dynamicBox;
		}
		virtual std::pair<bool, float> testIntersectWithTriangles(
			const vgKernel::Ray& ray );
		virtual bool testIntersectWithTriangles( 
			const vgKernel::Segment3& segment );
		virtual long getNumOfTriangles()
		{
			return _numberOfTriangles;
		}
		virtual float setDistanceToViewer( const Vec3& viewer_pos  );
		virtual Vec3 getPosition()
		{
			return _dynamicBox.getCenter();
		}

		// �̳�TransparentObject�Ľӿ�
		virtual bool isTransparent();

		// �̳�TransparentObject�Ľӿ�
		virtual void setTransparent( bool set_to_transp = true );

		vgAsyn::FileInfo* getDataFileInfoRef()
		{
			return _meshDataFileInfo;
		}

		void invalidateTextures();

	private:

		void setDefault();

	private:

		float _squareDistNear;
		float _squareDistFar;

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
		String _modelName;
		bool _isVisible;
		vgKernel::Box _dynamicBox;
		vgKernel::Box _staticBox;

		vgKernel::Mat4 _matrix;

		long	_numberOfTriangles;

		vgAsyn::FileInfo* _wholeFileInfo;

		bool _isTransparent;

		//----------------------------------------------------------------
		ModelData* _meshData;
		vgAsyn::FileInfo* _meshDataFileInfo;
	};
	

	typedef std::vector<ModelItem*> ModelItemPtrVec;

	typedef std::map<UniqueID, ModelItem*> ModelItemMap;
	typedef std::pair<ModelItemMap::iterator, bool> ModelItemInsertRes;

	VGK_RENDERER_OUT_DECLARE( ModelItem )
	
}// end of namespace vgModel

#endif // end of __VGMO_MESHITEM_H__