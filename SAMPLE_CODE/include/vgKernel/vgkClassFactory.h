


#ifndef __VGKRENDERERFACTORY_H__
#define __VGKRENDERERFACTORY_H__

#include <vgKernel/vgkForward.h>

	
namespace vgKernel {

	class ClassIdentify;

	/**
		@date 	2009/05/25  21:06	
		@author  xy
	
		@brief 	
	
		@see    
	*/
	class ClassFactory
	{
	public:
		ClassFactory()
		{
			
		}
		virtual~ClassFactory()
		{
			
		}
	

	public:

		virtual ClassIdentify* create() = 0;

		virtual String getClassName() = 0;
	
	};


	template< class T >
	class TClassFactory : public ClassFactory
	{
		virtual ClassIdentify* create() 
		{
			return new T;
		}

		virtual String getClassName() 
		{
			ClassIdentify* tmp = create();
			String ret = tmp->getClassName();
			delete tmp;
			return ret;
		}
	};
	

	class VGK_EXPORT ClassFacRegProxy
	{
	public:
		ClassFacRegProxy( ClassFactory* fac );
		~ClassFacRegProxy()
		{
			
		}
	};
	
}// end of namespace vgKernel
	


#endif // end of __VGKRENDERERFACTORY_H__