



#ifndef __VGKRENDERERFACTORYMANAGER_H__
#define __VGKRENDERERFACTORYMANAGER_H__

#include <vgKernel/vgkForward.h>
#include <vgKernel/vgkSingleton.h>

	
namespace vgKernel {

	class ClassFactory;

	typedef std::map<String, ClassFactory*> RendererFactoryMap;

	/**
		@date 	2009/05/25  21:07	
		@author  xy
	
		@brief 	
	
		@see    
	*/
	class VGK_EXPORT ClassFactoryManager
		: public vgKernel::Singleton<ClassFactoryManager>
	{
		friend class vgKernel::Singleton<ClassFactoryManager>;
	private:
		ClassFactoryManager();
	public:
		virtual~ClassFactoryManager();

	protected:

		virtual bool initialise()
		{
			return true;
		}

		virtual bool shutdown();
	

	public:
	
		virtual void reset(){};

		String getDetails();

		// ע��renderer����,ע��,���ฺ��ɾ��
		bool registerFactory( ClassFactory* fac );

		ClassFactory* getFactoryByClassName( const String& classname );


	private:

		RendererFactoryMap _factories;
	};
	
	
}// end of namespace vgKernel
	


#endif // end of __VGKRENDERERFACTORYMANAGER_H__