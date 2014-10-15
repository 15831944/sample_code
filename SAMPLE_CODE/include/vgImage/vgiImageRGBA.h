


#ifndef __VGIIMAGERGBA_H__
#define __VGIIMAGERGBA_H__

#include <vgKernel/vgkForward.h>
#include <vgKernel/vgkSharePtr.h>
#include <vgKernel/vgkStreamReader.h>
#include <vgKernel/vgkStreamWriter.h>


namespace vgImage {

	class BmpFactory;

	/**
	@date 	2009/04/18  19:23	
	@author  leven

	@brief 	ֻ���ڴ���RGBA��buffer����Ҫ��Ϊ�����Ч�ʡ�

	ע�⣺

	1.���Ͻ�Ϊ����

	@see    
	*/
	class VGI_EXPORT ImageRGBA
	{
		friend class BmpFactory;
	public:
		// take_ownership�Ƿ�������Ȩ�����Ϊfalse������
		// ������һ������
		ImageRGBA( char* buffer , long width, long height ,
			bool take_ownership , bool flip_Y );
		~ImageRGBA();

	public:

		// д���ڴ档
		// �����buf��Ҫ��NULL
		bool encodeToBmpMemory( char*& buf , long& buf_length );

		bool writeToBmpStream( vgKernel::StreamWriterPtr pwriter );

		bool writeToFile( const String& outout_filename )
		{
			vgKernel::StreamWriterPtr pw = 
				vgKernel::StreamWriterFactory::createFromFile( 
				outout_filename );

			return writeToBmpStream( pw );
		}

		void setFilpY( bool flipy )
		{
			_flipY = flipy;
		}

		void flipYNow()
		{
			long size = _width * _height * 4;
			char* buf = new char[ size ];
			long pos = 0;

			for ( int y = 0 ; y < _height ; ++ y )
			{
				memcpy( buf + pos, _pixelData + size - _width * 4 * ( y + 1) ,  _width * 4 );
				pos +=  _width * 4;
			}

			delete[] _pixelData;
			_pixelData = buf;
		}

		char* getRawPixelData()
		{
			return _pixelData;
		}

		void setOwnership( bool to_others )
		{
			_ownership = !to_others;
		}

		long getWidth() const
		{
			return _width;
		}

		long getHeight() const
		{
			return _height;
		}

	private:

		bool isValid();

	private:

		// RGBA��ʽ
		char	 *_pixelData;
		long		_width;
		long 		_height;

		bool _flipY;
		bool _ownership;
	};


	typedef vgKernel::SharePtr<ImageRGBA> ImageRGBAPtr;


}// end of namespace vgImage



#endif // end of __VGIIMAGERGBA_H__