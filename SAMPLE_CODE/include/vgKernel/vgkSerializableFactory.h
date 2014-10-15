



#ifndef __VGKSERIALIZABLEFACTORY_H__
#define __VGKSERIALIZABLEFACTORY_H__

#include <vgKernel/vgkForward.h>
#include <vgKernel/vgkSerializableObject.h>

	
namespace vgKernel {

	/**
		@date 	2009/05/11  21:09	
		@author  xy
	
		@brief ע��,ĳ��������,��Ҫ�ػ������
	
		@see    
	*/
	class SerializableAbstractFactory
	{
	public:
		SerializableAbstractFactory()
		{
			
		}
		virtual ~SerializableAbstractFactory()
		{
			
		}
	
	
	public:

		
		virtual SerializableObject* createSerializable() = 0;
	
	};
	

	template <class T>
	class SerializableDefaultFac
		: public SerializableAbstractFactory
	{
	public:
		SerializableDefaultFac()
		{
			
		}
		~SerializableDefaultFac()
		{
			
		}
	
	

	public:
	
		virtual SerializableObject* createSerializable()
		{
			SerializableObject* ret = new T;

#if _DEBUG
			assert( ret != NULL );
#endif

			return ret;
		}
	
	
	};
	
}// end of namespace vgKernel
	


#endif // end of __VGKSERIALIZABLEFACTORY_H__