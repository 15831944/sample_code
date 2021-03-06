




#ifndef __VGKRENDERERHOLDERMANAGER_H__
#define __VGKRENDERERHOLDERMANAGER_H__

#include <vgKernel/vgkForward.h>
#include <vgKernel/vgkSingleton.h>
#include <vgKernel/vgkRendererHolder.h>

	
namespace vgKernel {

	class RendererManager;

	/**
		@date 	2009/01/04  14:59	
		@author  xy
	
		@brief 	
	
		@see    
	*/
	class  VGK_EXPORT RendererHolderManager : public vgKernel::Singleton<RendererHolderManager>
	{
	private:
		friend class vgKernel::Singleton<RendererHolderManager>;
	private:
		RendererHolderManager()
			: Singleton<RendererHolderManager>( VGK_SINGLETON_LEFE_RENDERERHOLDERMGR ) 
		{
			VGK_TRACE(_T("RendererHolderManager created."));
		}
	public:
		virtual~RendererHolderManager()
		{
			VGK_TRACE(_T("RendererHolderManager destroyed."));
		}

	protected:
		virtual bool initialise()
		{
			return true;
		}
		virtual bool shutdown()
		{
			return true;
		}

	public:

		virtual void reset(){};

		bool registerHolder( RendererHolder* holder );

		bool unregisterHolder( RendererHolder* holder );

		//------------------------------------------
		// 只用于RendererManager
		//------------------------------------------
		bool deleteRenderer( Renderer* renderer );

	private:

		// 查看是否存在相同的项.
		// 若存在,返回true
		bool checkExist( RendererHolder* holder );

		void deleteHolder( RendererHolder* holder );

	private:

		RendererHolderPtrVec _holders;

	};

	
	
}// end of namespace vgKernel
	


#endif // end of __VGKRENDERERHOLDERMANAGER_H__