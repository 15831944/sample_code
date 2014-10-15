

#ifndef __VGAIMAGE_H__
#define __VGAIMAGE_H__

#include <vgImage/vgiDefinition.h>
#include <vgKernel/vgkForward.h>
#include <vgImage/vgiImageUtility.h>


namespace vgImage {


	/**
		@date 	2008/06/18  19:25	
		@author  leven
	
		@brief 	

		��vgImage��texture�������س�ʹ�õ���ImageMiniature
		������utility��,�����ֶ�����dds�ļ�


		//------------------------------------------
		// ��ʼ����Image
		//------------------------------------------
		FileInfoPtr pinfo = ArchiveManager::getSingleton().getFileInfo( "02.dds" );
		assert( pinfo.isNull() == false );
		int filesize = pinfo->uncompressedSize;
		char *filebuf = new char[filesize];
		pinfo->streamReaderPtr->read( filebuf , filesize );

		vgAsyn::Image img( filebuf , filesize );

		GLuint id = img.createTextureID();
	
		@see    
	*/
	class  VGI_EXPORT Image
	{
	public:

		//------------------------------------------
		// ע��,һ����ָ�봫�ݽ���
		// Image�ཫ����filebuf���ͷ�
		//------------------------------------------
		Image( char* filebuf , long filesize );

		~Image();
	
		bool isValid() const
		{
			return _loadOK;
		}

		GLuint createTextureID();


		void* getPixelDataPointer()
		{
			assert( mpPixelData != NULL );
			return mpPixelData;
		}

		long getPixelDataSize()
		{
			return mPixelDataSize;
		}

		long getClearestLayerDataSize();

	private:

		void setDefault();
		bool loadFromMemory();
		GLuint createTextureIDImpletation();

	public:

		char* _fileBuffer;
		long _fileSize;

		bool _loadOK;


		//------------------------------------------
		// ע��,��������ʱ�ı���
		// ֻ�Ǵ��������
		//------------------------------------------
		char	 *mpPixelData;
		long	 mPixelDataSize;
		long		mWidth;
		long 		mHeight;

		GLenum   mOpenGLFormat;
		long  mNumMipMaps;
	};
	
	
}// end of namespace vgAsyn



#endif // end of __VGAIMAGE_H__
