


#ifndef __VGICXIMAGEWRAPPER_H__
#define __VGICXIMAGEWRAPPER_H__
#include <vgImage/vgiDefinition.h>
#include <vgKernel/vgkForward.h>
#include <vgKernel/vgkSharePtr.h>
#include <vgKernel/vgkStreamReader.h>
#include <vgKernel/vgkImageRect.h>




class CxImage;

namespace vgImage {

	using namespace vgKernel;


	class CxStreamReader;
	class CxStreamWriter;
	class CximageWrapper;

	typedef CxImage*  CximagePtr;
	typedef vgKernel::SharePtr<CximageWrapper> CximageWrapperPtr;

	/**
		@date 	2008/08/25  10:40	
		@author  leven
	
		@brief 	
	
		@see    
	*/
	class VGI_EXPORT CximageWrapper
	{
	public:

		enum  DdsFormat
		{
			DDS_FORMAT_DXT1		= 1,
			DDS_FORMAT_DXT3		= 2,
			DDS_FORMAT_DXT5		= 3,
		};

		

	private:




	public:
		
		CximageWrapper( CximagePtr cximg );
		CximageWrapper();

		~CximageWrapper();
	

	public:

		// ��ͼ��ȫ������Ϊ������ɫ.
		// ע��,����alphaͨ��.
		void clearColor( const RGBQUAD& color );
		void clearColor( const uchar& color_r , 
			const uchar& color_g , 	const uchar& color_b , 	const uchar& color_a );

		bool createFromArray(  void* buf, const long& dwWidth, 
			const long& dwHeight , const long& dBpp = 24 ,bool  bFlipImage = false ,
			bool reverse_R_and_B = false );

		// ע���Ǵ�д
		bool cxCreateFromArray(BYTE* pArray,DWORD dwWidth,DWORD dwHeight,
			DWORD dwBitsperpixel, DWORD dwBytesperline, bool bFlipImage);

		bool createFromSize( const long& dwWidth, 
			const long& dwHeight , const long& dBpp = 32 );

		/**
			ͨ��filename���ж��ļ���ʽ
		*/
		bool loadHeaderFromFile( const String& file_abs_path );
		

		bool loadFromFile( const String& file_abs_path );
	
		/**
			��dds�ļ��Ĵ洢
		*/
		bool	saveToFile( const String& output_filepath );

		/**
			dds�ļ��洢
		*/
		bool	saveToDdsFile( const String& file_abs_path ,
			const DdsFormat& dformat , 
			const bool& auto_resize_to_two_power = false );

		// ����Ƿ��кڱ�,�����ֶ�
		// �˺�����ʱ��������
		bool	saveToDdsFileWithBorderCheck( const String& file_abs_path ,
			const DdsFormat& dformat , const bool& border_left_black , 
			const bool& border_up_black );

		bool	saveToDdsFile( const String& file_abs_path ,
			const GLenum& dds_format , 
			const bool& auto_resize_to_two_power = false );

		void release();

		CximagePtr getCximagePtr()
		{
			return mCximagePtr;
		}

		String getFilePath() const
		{
			return mFilePath;
		}

		long getWidth() const
		{
			return this->mWidth;
		}

		long getHeight() const
		{
			return this->mHeight;
		}

		ImageSize getImageSize() const
		{
			return ImageSize( mWidth , mHeight );
		}


		bool resize( const ImageSize& imgSize );

		/**
			������������ͼ���С��Ŀ���.
		*/
		const CximageWrapperPtr  resizeCopy( const ImageSize& imgSize ) const;

		/**
			�и�ͼ��.
				
			@note	һ��ִ�е�ͼ�����С����.��Rect�Ĵ�С����ͼ��Ĵ�Сʱ�����ܱ�֤��ȷ.
				����ע��ͼ�������������.
				
			@param	rect �и�ͼ��ĳ����α�ʾ.����ԭͼ����ռ��λ��.
			@param	pDstImg ��ͼ������Ŀ�ĵ�.ע��Ϊ��ʱ,�Ὣ���ݴ��뱾��.
			@param	axisLeftTop �������Ƿ������Ͻ�.
		*/
		bool  crop( const ImageRect& rect , CximageWrapperPtr pDstImg , 
			const bool& axisLeftTop = true );

		/**
			��ͼ���ϻ���������Ϣ.
				
			@note	ע��,��ʱxƫ�ƺ�yƫ�ƶ��Ƕ��������������ϽǶ���.
				
			@param	xoffset xƫ��.
			@param	yoffset yƫ��.
			@param	text Ҫд������.
			@param	font ���ֵ���������.
			@param	fontsize ���ֵĴ�С.
			@param	setAlpha �Ƿ�������Ϊ͸��.
		*/
		bool drawString( const long& xoffset ,
			const long& yoffset , const String& text , const String& font = "" , 
			const long& fontsize = 30 , const bool& setAlpha = false );


		/**
			��imgsrc2��ͼ�����ݵ��ڱ�ͼ��֮��.
				
			@note	������Ҳ��������֮��.����ע��,�������ݲ�֧�ֳ���ͼ���С��Rect.
				
			@param	imgsrc2 �ص����ϲ��ͼ��.
			@param	rect ����ڵײ������������.
			@param	axisLeftTop �����Ƿ������Ͻ�.
		*/
		void overlap( CximageWrapper &imgsrc2, const ImageRect& rect ,
			const bool& axisLeftTop = true );

		// ע��!!�����Ͻ�Ϊԭ��!!
		void overlap2( CximageWrapper &imgsrc2, const long& xoffset , 
			const long& yoffset );


		


	private:
	

	
		void updateSelfInfoFromCximage();

		

	private:

		/// ͼ����ļ�·��.
		String		mFilePath;

		/// ͼ��ĳ���.
		long		mWidth;
		long		mHeight;

		/// �Ƿ��Ѿ���ʼ���ı�־.
		bool		mInitialised;

		CximagePtr	mCximagePtr;

	};
	
	bool VGI_EXPORT getImageFormatInfo( const String& filename , 
		long& width, long& height);







}// end of namespace vgImage
	


#endif // end of __VGICXIMAGEWRAPPER_H__