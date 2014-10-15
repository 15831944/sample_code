


#ifndef __VGMFILEDEFINITIONVGM_H__
#define __VGMFILEDEFINITIONVGM_H__


#include <vgMesh/vgmDefinition.h>
#include <vgKernel/vgkForward.h>
#include <vgMesh/vgmFileDefinitionMod.h>
#include <vgKernel/vgkBox.h>
#include <vgKernel/vgkUniqueID.h>
#include <vgKernel/vgkSharePtr.h>
#include <vgKernel/vgkStreamReader.h>
#include <vgKernel/vgkStreamWriter.h>
#include <vgKernel/vgkMat4.h>

#include <vgKernel/vgkRgbaQuad.h>

namespace vgMesh {

	
	using namespace vgKernel;

	class ModObject;


#define VGM_TEXTURE_NAME_LENGTH	64
#define VGM_MESH_NAME_LENGTH	64

	enum VgmBriefType
	{
		VGM_BRIEFTYPE_MESH,
		VGM_BRIEFTYPE_GROUP
	};

	enum VgmDataType
	{
		VGM_DATATYPE_OBJECT
	};

	enum VgmDataPlusType
	{
		VGM_DATAPLUSTYPE_SHADOW
	};


	struct  VGM_EXPORT VgmHeader
	{
		VgmHeader()
		{
			setDefault();
		}
		~VgmHeader()
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

	struct VgmFaceIndex
	{
		int _point0;
		int _point1;
		int _point2;
	};

	/**
	����ṹ1
	*/
	struct VgmVertexElement1
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
	struct VgmVertexElement2
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
		sizeof( VgmBrief ) ==228;
	*/
	class  VGM_EXPORT VgmBrief
	{
	public:
		VgmBrief();
		~VgmBrief()
		{

		}

		void getInfoFromModObject( ModObject* mod );

	private:

		void setDefault();

	public:

		VgmBriefType _type;


		UniqueID _parentID;
		UniqueID _uniqueId;

		long	_positionOfFile;
		long	_lengthOfFile;
		char	_meshName[VGM_MESH_NAME_LENGTH];

		VgmDataType	_meshDataType;
		VgmDataPlusType _meshDataPlusType;

		bool	_isVisible;
		// �Ƿ���lod��Mesh.
		// Ĭ��Ϊtrue.���Ϊfalse,ֻ��Ⱦ�ϸ��һ��.
		bool  _isLodMesh;
		char	_reserved0;
		char  _reserved1;

		// �����ŵĲ���openGL�������matrix
		// ���������matrix
		// ʹ��ʱ����glMultTransposeMatrixf����
		vgKernel::Mat4	_multMatrix;
		Box _dynamicBoundingBox;
		Box _staticBoundingBox;
		long _numberOfFaces;

		//------------------------------------------
		// Lod0 is coarest.
		//------------------------------------------
		long _positionOfMeshData[ VGLOD_N ];
		long _lengthOfMeshData[ VGLOD_N ];
		long _positionOfMeshDataPlus[ VGLOD_N ];
		long _lengthOfMeshPlus[VGLOD_N ];


		// ��һ����lod����.�����ӵ�������lod�ķ�Χ
		float _squaredDistanceOfLodNear;
		// �ڶ�����lod����.
		float _squaredDistanceOfLodFar;

		// Ϊ�Ժ�����׼��
		vgKernel::RgbaQuad _colorQuad;
		//long	_reserved3;
		long	_reserved4;
		long	_reserved5;
		long	_reserved6;
	};

	typedef vgKernel::SharePtr<VgmBrief> VgmBriefPtr;

	typedef std::vector<VgmBriefPtr> VgmBriefPtrVec;


	/**
		����face���ݽṹ
	*/
	struct  VGM_EXPORT VgmMetaFaceStruct
	{
		VgmMetaFaceStruct();
		~VgmMetaFaceStruct();

		long writeToStream( const vgKernel::StreamWriterPtr pwriter );

	private:
		void setDefault();
		void destroy();
	public:
		unsigned char _colorRGBA[4];


		// ָ���������_numberOfTexture
		// �ļ�������VGM_TEXTURE_NAME_LENGTH
		int		_numberOfTexture;
		long	_sizeOfOneTexName;
		char **_textureFileName;

		// ע��!�����Ǳ�3��������,��ʹ��ʱҪ����3
		long _numberOfFaceIndex;
		long _sizeOfFaceIndex;
		VgmFaceIndex* _faceIndex;

	};


	/**
		���һ��object������face
	*/
	struct  VGM_EXPORT VgmDataStruct
	{
		VgmDataStruct();
		~VgmDataStruct();

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

		VgmDataType _dataType;

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
		VgmMetaFaceStruct* _metaFaces;
	};


	typedef vgKernel::SharePtr<VgmDataStruct> VgmDataStructPtr;

	typedef std::vector<VgmDataStructPtr> VgmDataStructPtrVec;



	class  VGM_EXPORT VgmDataPlus
	{
	public:
		VgmDataPlus()
		{
			_type = VGM_DATAPLUSTYPE_SHADOW;
			_shadowNumberOfFaces = 0;
			_shadowNormalOfFaces = NULL;
			_shadowNumberOfVertices = 0;
			_shadowVerticesOfFaces = NULL;
			_shadowIndicesOfFaces = NULL;
		}
		~VgmDataPlus()
		{
			if ( _shadowNormalOfFaces != NULL )
			{
				delete[] _shadowNormalOfFaces;
				_shadowNormalOfFaces = NULL;
			}

			if ( _shadowVerticesOfFaces != NULL )
			{
				delete[] _shadowVerticesOfFaces;
				_shadowVerticesOfFaces = NULL;
			}
			if ( _shadowIndicesOfFaces != NULL )
			{
				delete[] _shadowIndicesOfFaces;
				_shadowIndicesOfFaces = NULL;
			}
		}
		
	
	private:
	
		VgmDataPlusType _type;

		// DataPlus
		int		_shadowNumberOfFaces;
		Vec3*	_shadowNormalOfFaces;
		int		_shadowNumberOfVertices;
		Vec3*	_shadowVerticesOfFaces;
		ShadowFaceIndex*	_shadowIndicesOfFaces;
	
	};

	
}// end of namespace vgMesh

#endif // end of __VGMFILEDEFINITIONVGM_H__