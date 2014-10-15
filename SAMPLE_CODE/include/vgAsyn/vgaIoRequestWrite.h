


#ifndef __VGAIOREQUESTWRITE_H__
#define __VGAIOREQUESTWRITE_H__
#include <vgAsyn/vgaDefinition.h>
#include <vgKernel/vgkForward.h>
#include <vgAsyn/vgaIoRequest.h>

	
namespace vgAsyn {

	/**
		@date 	2008/11/08  8:45	
		@author  leven
	
		@brief  д���ļ���IoRequest.
		ע��,��ʱֻʵ����ͬ���İ汾.�첽�汾��ʱ��������.
		�����buffer����������ʱ���Զ��ͷŵ�.
	
		@see    
	*/
	class VGA_EXPORT IoRequestWrite : public IoRequest
	{
	public:
		IoRequestWrite( FileInfo* fileinfo , void* buffer )
			: IoRequest( fileinfo )
		{
			_buffer = (char*)buffer;
			setAsynchronousMode( false );
		}

		virtual ~IoRequestWrite()
		{
			
		}
	
		
		void sendIoRequestWrite();

	
	private:
	
	
	};
	
	
}// end of namespace vgAsyn
	


#endif // end of __VGAIOREQUESTWRITE_H__