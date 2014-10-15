


#include <vgStableHeaders.h>
#include <vgImage/vgiIoRequestDumpImage.h>


namespace vgImage {
	
	//----------------------------------------------------------------
	IoRequestDumpImage::IoRequestDumpImage( ImageMiniature* imgmini )
		:vgAsyn::IoRequest( imgmini->getMetaFileInfo() )
	{
		assert( _img != NULL );
		_img = imgmini;

		vgAsyn::FileInfo* fileinfo = _img->getMetaFileInfo();
		assert( fileinfo != NULL );

		// ����ͬ��ģʽ
		setAsynchronousMode( false );

		// ����buffer�Ĳ��Զ�ɾ��
		setDeleteBufferFlag( false );
	}
	//----------------------------------------------------------------
	IoRequestDumpImage::~IoRequestDumpImage()
	{

	}
	//----------------------------------------------------------------
	bool IoRequestDumpImage::anotherThreadHandler()
	{
		return true;
	}
	//----------------------------------------------------------------
	bool IoRequestDumpImage::mainThreadHandler()
	{
		return true;
	}
	//----------------------------------------------------------------
	VgiBrief* IoRequestDumpImage::getBriefFromBuffer()
	{
		assert( _buffer != NULL );
		assert( _img != NULL );

		VgiBrief* ret = new VgiBrief();

		bool getres = ret->getInfoFromBuffer( _img->getMetaFileName() , 
			_buffer , _fileInfo->_fileSize );

		if ( getres == false )
		{
			delete ret;
			return NULL;
		}

		return ret;
	}
	//----------------------------------------------------------------
}// end of namespace vgImage
