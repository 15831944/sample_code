




#ifndef __VGKCOMMANDMANAGER_H__
#define __VGKCOMMANDMANAGER_H__

#include <vgKernel/vgkForward.h>
#include <vgKernel/vgkSingleton.h>

	
namespace vgKernel {


	/**
		@date 	2009/01/04  14:27	
		@author  xy
	
		@brief 	
	
		@see    
	*/
	class  VGK_EXPORT CommandManager : public vgKernel::Singleton<CommandManager>
	{
	private:

		friend class vgKernel::Singleton<CommandManager>;
	private:
		CommandManager()
			: Singleton<CommandManager>( VGK_SINGLETON_LEFE_COMMANDMGR ) 
		{
			VGK_TRACE(_T("CommandManager created."));
		}
	public:
		virtual~CommandManager()
		{
			VGK_TRACE(_T("CommandManager destroyed."));
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

	private:


	};
}// end of namespace vgKernel
	


#endif // end of __VGKCOMMANDMANAGER_H__