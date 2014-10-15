


#ifndef __VGKOPENGLSUPPORT_H__
#define __VGKOPENGLSUPPORT_H__

#include <vgKernel/vgkForward.h>

	
namespace vgKernel {

	/**
		@date 	2007/02/25  14:12	
		@author  xy

		@brief 	OpenGL֧����,������ѯĿ�Ļ�����Opengl��֧�����.

		���඼Ϊ��̬����.

		@see    
	*/
	class VGK_EXPORT  OpenGLSupport
	{
	private:
		OpenGLSupport()
		{

		}
	public:

		~OpenGLSupport()
		{

		}
	public:

		static bool initialiseOpenGLExtension();

		// �鿴�����Ƿ�֧��ѹ������.
		static bool	checkTextureCompresstionSupport();

		static bool checkCubeMapSupport();

		// �鿴�����Ƿ�֧�ָ������Թ���.
		static bool  checkTextureFilterAnisotropicSupport();

		// �����������Թ���.
		static bool turnOnLargestTexFilterAnisotropic();

		static bool checkOpenGLErrorThrowException();

		static void checkOpenGLErrorThrowException( 
			const String& source , const String& file , const long& line );

		static bool checkOpenGLErrorWithoutThrow( String& errMsg );

		static void makeErrorStackEmpty();


		static String getOpenGLDetails();

	private:
		
		static const String getOpenGLErrorInfo( const GLenum& errorCode );
	};
	
	
}// end of namespace vgKernel


#if _DEBUG
	#define VGK_CHECK_OPENGL_ERROR() vgKernel::OpenGLSupport::checkOpenGLErrorThrowException( \
		"", __FILE__, __LINE__ );
#else
	#define VGK_CHECK_OPENGL_ERROR() 
#endif


	
	

	


#endif // end of __VGKOPENGLSUPPORT_H__