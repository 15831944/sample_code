


#ifndef __VGIFILEDEFINITIONVGI_H__
#define __VGIFILEDEFINITIONVGI_H__
#include <vgImage/vgiDefinition.h>
#include <vgKernel/vgkForward.h>
#include <vgKernel/vgkSharePtr.h>

	
namespace vgImage {


	using namespace vgKernel;


#define VGI_NAME_LENGTH	64


//#define VGI_MAX_FILENUM		2048

// ����vgi�ļ����������.���ֽ�Ϊ��λ
#define VGI_MAX_SINGLE_FILE_SIZE		( 512 * 1024 * 1024 )
//#define VGI_MAX_SINGLE_FILE_SIZE		( 512 * 1024 )

	class VgiBrief;

	class  VGI_EXPORT VgiHeader
	{
	public:
		VgiHeader()
		{
			setDefault();
		}
		~VgiHeader()
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


		//long	_reserved1;
		// ʹ��_reserved1����ָ����һ�����ӵ�vgi�ļ�
		// ����, "test.vgi" , ���Ϊ0, ��ʾû����һ�������ļ�.
		// ���Ϊ1,��ʾ��һ���ļ�Ϊtest.part1.vgi
		long _nextFileID;

		long	_reserved2;
		long	_reserved3;
		long	_reserved4;

	};


	enum VgiBriefType
	{
		VGI_BRIEF_TYPE_DDS
	};

	class  VGI_EXPORT VgiBrief
	{
	public:
		VgiBrief()
		{
			setDefault();
		}
		~VgiBrief()
		{
			setDefault();
		}

		// load from whole image buffer.
		bool getInfoFromBuffer( const String& filemetaname , 
			const char* const buffer, 
			const long& buffer_size );

	private:
		void caluateLodOffsetAndSize();

		void setDefault();

		static long getMaxMipmapNum(const long& width,const long& height);

	public:

		VgiBriefType _type;

		// ע��,����Ķ���Сд��ʽ
		char _fileName[VGI_NAME_LENGTH];

		long  _metaFileOffset;
		long  _metaFileSize;

		// lod0 is coarsest.
		// ע��!�����������_metaFileOffset��ƫ��!!
		// _mipMapLod��ÿ��LOD��lod����
		long  _offsetLod[VGLOD_N];
		long  _sizeLod[VGLOD_N];
		long   _mipMapLod[VGLOD_N];


		long   _width;
		long   _height;

		GLenum  _openGLFormat;


	};

	typedef vgKernel::SharePtr<VgiBrief> VgiBriefPtr;

	typedef std::vector<VgiBriefPtr> VgiBriefPtrVec;
	
	
}// end of namespace vgImage
	


#endif // end of __VGIFILEDEFINITIONVGI_H__