



#ifndef __VGIFREEIMAGEWRAPPER_H__
#define __VGIFREEIMAGEWRAPPER_H__


#include <vgImage/vgiDefinition.h>
#include <vgKernel/vgkForward.h>
#include <vgKernel/vgkSharePtr.h>
#include <vgKernel/vgkImageRect.h>


#include <FreeImage.h>
#include <FreeImagePlus.h>

	
namespace vgImage {

	/**
		@date 	2009/05/03  14:32	
		@author  leven
	
		@brief 	
	
		@see    
	*/
	class VGI_EXPORT FreeImageWrapper : public fipWinImage
	{
	public:

		enum  DdsFormat
		{
			DDS_FORMAT_DXT1		= 1,
			DDS_FORMAT_DXT3		= 2,
			DDS_FORMAT_DXT5		= 3,
		};

	public:
		FreeImageWrapper()
			: fipWinImage()
		{
			
		}
		~FreeImageWrapper()
		{
			
		}
	
	public:


		inline void release()
		{
			clear();
		}


		bool createFromSize( long dwWidth, long dwHeight , long dBpp = 32 );

		// ��ͼ��ȫ������Ϊ������ɫ.
		// ע��,����alphaͨ��.
		void clearColor( const RGBQUAD& color );
		void clearColor( uchar color_r , uchar color_g , uchar color_b , uchar color_a );


		bool resize( const vgKernel::ImageSize& imgSize );

		/**
			������������ͼ���С��Ŀ���.
		*/
		FreeImageWrapper*  resizeCopy( const vgKernel::ImageSize& imgSize ) const;

		// �뱾���load��ͬ����, ������������Ż�
		// һ���ǽ�����ȫ�������ڴ�,���ڴ��н�ѹ
		bool loadFromFile( const String& filepath, int flag = 0 );

		// ͬ��,���Ҳ���Ż�
		// ע��,�����ͬ������,������ʧ��,�����Ա���
		bool saveToFile( const String& save_path ,int flag = 0 );

		
		// ע���ʱ������
		// axis_left_downΪtrueʱ,��һ��������
		// axis_left_downΪfalseʱ,��һ��������
		// ����ҪflipY,ʹ��flipVertical()
		// ����ҪRed��Blue��ת��, ��ʹ��swapRedAndBlueChannel()
		bool createFromArray( void* buf, long dwWidth, 
			long dwHeight , long dBpp = 24 , bool axis_left_up = true );

		
		bool swapRedAndBlueChannel();

		/**
			dds�ļ��洢
		*/
		bool	saveToDdsFile( const String& file_abs_path ,
			const DdsFormat& dformat , 
			const bool& auto_resize_to_two_power = false );

		bool	saveToDdsFile( const String& file_abs_path ,
			const GLenum& dds_format , 
			const bool& auto_resize_to_two_power = false );

		// �õ�������RGBA����RGB��ʽ
		// ��accessAlignedPixels��ͬ����,һ�㷵�صĲ���NULL
		BYTE* getAlignedPixelsCopy();

		// ֱ�ӷ���������RGBA����RGB��ʽ��������Ǵ˸�ʽ����û�ж��룬����NULL
		BYTE* accessAlignedPixels();


		/**
			�и�ͼ��.
				
			@note	һ��ִ�е�ͼ�����С����.��Rect�Ĵ�С����ͼ��Ĵ�Сʱ�����ܱ�֤��ȷ.
				����ע��ͼ�������������.
				
			@param	rect �и�ͼ��ĳ����α�ʾ.����ԭͼ����ռ��λ��.
			@param	pDstImg ��ͼ������Ŀ�ĵ�.ע��Ϊ��ʱ,�Ὣ���ݴ��뱾��.
			@param	axisLeftTop �������Ƿ������Ͻ�.
		*/
		bool  crop( const vgKernel::ImageRect& rect , FreeImageWrapper* pDstImg , 
			bool axisLeftTop = true );


		/**
			��imgsrc2��ͼ�����ݵ��ڱ�ͼ��֮��.
				
			@note	������Ҳ��������֮��.����ע��,�������ݲ�֧�ֳ���ͼ���С��Rect.
				
			@param	imgsrc2 �ص����ϲ��ͼ��.
			@param	rect ����ڵײ������������.
			@param	axisLeftTop �����Ƿ������Ͻ�.
		*/
		bool overlap( FreeImageWrapper &imgsrc2, const vgKernel::ImageRect& rect ,
			bool axisLeftTop = true );

		// �����Ͻ�Ϊԭ��
		bool overlap2( FreeImageWrapper &overlapped, long left , long top);

	private:
	
		bool saveToFileImpl( const String& save_path, fipMemoryIO &alloc );
	
	};
	

	typedef vgKernel::SharePtr<FreeImageWrapper> FreeImageWrapperPtr;
	
}// end of namespace vgImage
	


#endif // end of __VGIFREEIMAGEWRAPPER_H__