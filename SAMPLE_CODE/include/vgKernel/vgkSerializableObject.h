



#ifndef __VGKSERIALIZER_H__
#define __VGKSERIALIZER_H__

#include <vgKernel/vgkForward.h>
#include <vgKernel/vgkStreamReader.h>
#include <vgKernel/vgkStreamWriter.h>
#include <vgKernel/vgkClassIdentify.h>

	
namespace vgKernel {

	/**
		@date 	2009/05/11  20:49	
		@author  xy
	
		@brief 	
	
		@see    
	*/
	class SerializableObject :	public ClassIdentify
	{
	public:
		SerializableObject()
		{
			
		}
		virtual~SerializableObject()
		{
			
		}
	

	public:

		// ���ص��Ƕ�ȡ���ֽ���.
		// ��Ϊ����, ��ʾ����
		virtual int serializeFromFile( 
			vgKernel::StreamReaderPtr preader, 
			int filesize,
			int version,
			const String& version_info ) = 0;

		// ���ص���д����ֽ���.
		// ��Ϊ����, ��ʾ����
		virtual int serializeToFile( 
			vgKernel::StreamWriterPtr pwriter, 
			int version,
			const String& version_info ) = 0;

	};
	

//#define VGK_SERIALIZABLE_DECLARE( class_name )

	typedef std::vector<SerializableObject*> SerializeObjPtrVector;
	
}// end of namespace vgKernel
	


#endif // end of __VGKSERIALIZER_H__