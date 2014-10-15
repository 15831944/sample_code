


#ifndef __VGIBATCHPROCESSOR_H__
#define __VGIBATCHPROCESSOR_H__
#include <vgImage/vgiDefinition.h>
#include <vgKernel/vgkForward.h>

#include <vgImage/vgiHugeProcessHugeJobs.h>
#include <vgImage/vgiHugeProcessPieceJob.h>

	
namespace vgImage {

	using namespace vgKernel;

	/**
		@date 	2008/10/10  10:25	
		@author  leven

		@brief 	��������bmp��dds
		����Ҳ���Դ����������ļ���ʽ,��bmp��tga

		@see    
	*/
	class  VGI_EXPORT BatchProcessor
	{
	private:
		BatchProcessor()
		{

		}
		~BatchProcessor()
		{

		}

	public:

		// ���ؾ���out_extension��չ�����ļ��б�
		static StringVectorPtr process( const StringVectorPtr src_img_list ,
			const String& temppath, const String& out_extension , 
			const GLenum& dds_format , vgKernel::ProcessNotifier* notifier ,
			const bool& remove_input_files , 
			const float& resize_ratio = 1.0f , 
			const bool& use_ext_as_name = false );

		//static StringVectorPtr batchResize( const StringVectorPtr src_img_list , 
		//	)

	private:
		class DealOneImage : public HugeProcess::PieceJobString
		{
		public:
			DealOneImage( String src_image , String temppath ,
				const String& out_extension , 
				const GLenum& dds_format,
				HugeProcess::HugeJobsStringVector* boss , 
				vgKernel::ProcessNotifier* notifier , 
				bool remove_input , 
				float resizeRatio ,
				bool use_ext_as_name )
				: PieceJobString( boss , notifier )
			{
				_removeFiles = remove_input;
				_src = src_image;
				_temp = temppath;
				_format = dds_format;
				_ext = out_extension;
				_resizeRatio = resizeRatio;
				_useExtAsName = use_ext_as_name;
			}
			virtual ~DealOneImage()
			{
				
			}
		
			virtual String doJobRetString();
		
		private:

			String doJobImpl();
		
			bool _removeFiles;
			String _ext;
			GLenum _format;
			String _src;
		
			String _temp;
			float _resizeRatio;
		
			bool _useExtAsName;
		};
	};


}// end of namespace vgImage

	


#endif // end of __VGIBATCHPROCESSOR_H__