


#ifndef __VGKGPUSUPPORT_H__
#define __VGKGPUSUPPORT_H__

#include <vgKernel/vgkForward.h>
#include <Cg/cg.h>
#include <Cg/cgGL.h>

	
namespace vgKernel {


	class  VGK_EXPORT GpuContext
	{
	public:
		// �����ڹ��캯����  
		GpuContext();
		~GpuContext();

		CGcontext getContext()
		{
			assert( _CgContext != NULL );
			return _CgContext;
		}

	private:

		// CGcontext�������һ��ָ��
		CGcontext   _CgContext;
	};



	/**
		ע��,�������Ҫʵ����,������GpuManager��.
		����ǰ��ͬ����:�ܶ�static��Ա�Ѿ�����֧��.
		��Ҫ��GpuMananger::getSingleton().getGpuSupport()֮����ʹ��
	*/
	class  VGK_EXPORT GpuSupport
	{
	public:
		GpuSupport();
		~GpuSupport();

		CGprofile getVertexProfile() 
		{
			assert( isValid() );
			return _vertexProfile;
		}

		CGprofile getPixelProfile() 
		{
			assert( isValid() );
			return _pixelProfile;
		}


		GpuContext* getGpuContextPointer()
		{
			assert(isValid());
			return _gpuContext;
		}

		bool isValid() 
		{
			return _vertexProfile != CG_PROFILE_UNKNOWN &&
				_pixelProfile != CG_PROFILE_UNKNOWN;
		}

	private:

		bool setupValidCgProfile( CGprofile& vertex_profile ,
			CGprofile& pixel_profile );

		static void checkCgErrorStack();

	private:

		GpuContext* _gpuContext;

		bool _cubeMapSupported;
		bool _vertexTextureFetchSupported;

		CGprofile   _vertexProfile;
		CGprofile   _pixelProfile;

	};


	
}// end of namespace vgKernel



#endif // end of __VGKGPUSUPPORT_H__