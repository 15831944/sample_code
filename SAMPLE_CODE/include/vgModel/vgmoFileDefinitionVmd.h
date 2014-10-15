


#ifndef __VGMO_FILEDEFINITIONVMD_H__
#define __VGMO_FILEDEFINITIONVMD_H__


#include <vgModel/vgmoDefinition.h>
#include <vgKernel/vgkForward.h>
#include <vgModel/vgmoFileDefinitionMod.h>
#include <vgKernel/vgkBox.h>
#include <vgKernel/vgkUniqueID.h>
#include <vgKernel/vgkSharePtr.h>
#include <vgKernel/vgkStreamReader.h>
#include <vgKernel/vgkStreamWriter.h>
#include <vgKernel/vgkMat4.h>

#include <vgKernel/vgkRgbaQuad.h>

namespace vgModel {

	
	using namespace vgKernel;

	class ModelItem;
	class ModObject;


#define VMD_TEXTURE_NAME_LENGTH	64
#define VMD_MODEL_NAME_LENGTH	64


	//// ���Ͷ���
	//// �ͻ��˿����Լ����
	//const int VMD_BRIEFTYPE_MODEL = 0;
	//const int VMD_DATATYPE_MODEL = 0;


	struct  VGMO_EXPORT VmdHeader
	{
		VmdHeader()
		{
			setDefault();
		}
		~VmdHeader()
		{

		}

	private:

		void setDefault();

	public:

		char	_fourCC[4];
		int		_version;
		long	_sizeOfHeader;
		long	_sizeOfWholeFile;
		long	_numberOfBriefs;
		long	_lengthOfOneBrief;
		long	_positionOfBriefs;
		long	_reserved1;
		long	_reserved2;
		long	_reserved3;
		long	_reserved4;

	};

	struct VmdFaceIndex
	{
		int _point0;
		int _point1;
		int _point2;
	};

	/**
	����ṹ1
	*/
	struct VmdVertexElement1
	{
		float texCoorS;
		float texCoorT;

		float normalX;
		float normalY;
		float normalZ;

		float positionX;
		float positionY;
		float positionZ;

	};

	/**
	����ṹ2
	*/
	struct VmdVertexElement2
	{
		float texCoorS1;
		float texCoorT1;

		float normalX;
		float normalY;
		float normalZ;

		float positionX;
		float positionY;
		float positionZ;

		float texCoorS2;
		float texCoorT2;
	};

	/**
		ע��,��û����ֱ�Ӷ��������
		sizeof( VmdBrief ) ==228;
	*/
	class  VGMO_EXPORT VmdBrief
	{
	public:
		VmdBrief();
		~VmdBrief()
		{

		}

		bool setClassName( const String& classname );

		void getInfoFromModObject( ModObject* mod );

	private:

		void setDefault();

	public:

		// ��ʶClass��ȡ���ַ���
		char	_className[VMD_MODEL_NAME_LENGTH];
		char	_modelName[VMD_MODEL_NAME_LENGTH];

		UniqueID _parentID;
		UniqueID _uniqueId;

		//Ŀǰ����_positionOfFile��_positionOfModelData��ʾһ��
		// ���������lod��Ϣ,���߾Ͳ���һ����
		long	_positionOfFile;
		long	_lengthOfFile;
		long _positionOfModelData;
		long _lengthOfModelData;


		// �����Ϊ���ֽڶ���
		bool	_isVisible;
		char	_reserved0;
		char  _reserved1;
		char  _reserved2; 

		// �����ŵĲ���openGL�������matrix
		// ���������matrix
		// ʹ��ʱ����glMultTransposeMatrixf����
		vgKernel::Mat4	_multMatrix;
		Box _dynamicBoundingBox;
		Box _staticBoundingBox;

		long _numberOfFaces;

		//------------------------------------------
		// ����ʹ��
		// ��Ϊreservedʹ��
		//------------------------------------------
		long _reservedData[ 16 ];


		// ��һ����lod����.�����ӵ�������lod�ķ�Χ
		float _squaredDistanceOfLodNear;
		// �ڶ�����lod����.
		float _squaredDistanceOfLodFar;

		vgKernel::RgbaQuad _colorQuad;
		long	_reserved3;
		long	_reserved4;
		long	_reserved5;
		long	_reserved6;

		long	_reserved7[256];
	};

	typedef vgKernel::SharePtr<VmdBrief> VmdBriefPtr;

	typedef std::vector<VmdBriefPtr> VmdBriefPtrVec;




	struct VmdKeyFrame
	{
		int		m_fTime;		//	Time at which keyframe is started
		vgKernel::Mat4 _matrix;
	};

	/**
	@date 	2009/05/21  20:21	
	@author  leven

	@brief 	

	@see    
	*/
	class VmdKeyFrameInfo
	{
	public:
		VmdKeyFrameInfo()
		{
			_frameCount = 0;
			_startFrame = 0;
			_endFrame = 0;

			_play = true;
			_loop = true;

			_frames = NULL;
		}
		~VmdKeyFrameInfo()
		{
			if ( _frames != NULL )
			{
				delete[] _frames;
				_frames = NULL;
			}

		}

		VmdKeyFrameInfo* createCopy()
		{
			VmdKeyFrameInfo* ret = new VmdKeyFrameInfo;
			ret->_frameCount = _frameCount;
			ret->_startFrame = _startFrame;
			ret->_endFrame = _endFrame;

			ret->_play = _play;
			ret->_loop = _loop;

			ret->_frames = new VmdKeyFrame[ _frameCount ];
			memcpy( ret->_frames , _frames , sizeof( VmdKeyFrame ) * 
				_frameCount );

			return ret;
		}


	public:

		int _frameCount;
		int _startFrame;
		int _endFrame;

		bool _play;
		bool _loop;

		VmdKeyFrame* _frames;
	};

	/**
		����face���ݽṹ
	*/
	struct  VGMO_EXPORT VmdMetaFaceStruct
	{
		VmdMetaFaceStruct();
		~VmdMetaFaceStruct();

		long writeToStream( const vgKernel::StreamWriterPtr pwriter );

	private:
		void setDefault();
		void destroy();
	public:
		unsigned char _colorRGBA[4];


		// ָ���������_numberOfTexture
		// �ļ�������VMD_TEXTURE_NAME_LENGTH
		int		_numberOfTexture;
		long	_sizeOfOneTexName;
		char **_textureFileName;

		// ע��!�����Ǳ�3��������,��ʹ��ʱҪ����3
		long _numberOfFaceIndex;
		long _sizeOfFaceIndex;
		VmdFaceIndex* _faceIndex;

	};


	/**
		���һ��object������face
	*/
	struct  VGMO_EXPORT VmdDataStruct
	{
		VmdDataStruct();
		~VmdDataStruct();

		void getInfoFromModObject( ModObject* mod );

		long writeToStream( const vgKernel::StreamWriterPtr pwriter );

		long getNumberOfTriangles();

#if 0
		// only for testing
		void render();
#endif

		void destroy();

	private:

		void setDefault();
	public:


		int		_renderType;

		// ��������,����bake
		// ��ʱֻ֧��1��2
		long _layersOfTexture;

		// ������
		long	_numberOfVertexElements;
		long	_sizeOfVertexElements;
		void*	_vertexElements;

		// ������
		long _numberOfMetaFaces;
		VmdMetaFaceStruct* _metaFaces;


		// DataPlus
		int		_shadowNumberOfFaces;
		Vec3*	_shadowNormalOfFaces;
		int		_shadowNumberOfVertices;
		Vec3*	_shadowVerticesOfFaces;
		ShadowFaceIndex*	_shadowIndicesOfFaces;

		// keyframe
		VmdKeyFrameInfo* _keyframeInfo;
	};


	typedef vgKernel::SharePtr<VmdDataStruct> VmdDataStructPtr;

	typedef std::vector<VmdDataStructPtr> VmdDataStructPtrVec;




	
}// end of namespace vgModel

#endif // end of __VGMO_FILEDEFINITIONVMD_H__