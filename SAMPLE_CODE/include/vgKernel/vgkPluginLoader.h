



#ifndef __VGKPLUGINLOADER_H__
#define __VGKPLUGINLOADER_H__

#include <vgKernel/vgkForward.h>

	
namespace vgKernel {

	/**
		@date 	2009/07/21  21:10	
		@author  xy
	
		@brief 	
	
		@see    
	*/
	class PluginLoader
	{
	public:
		PluginLoader()
		{
			
		}
		virtual~PluginLoader()
		{
			
		}
	

	public:

		virtual void loadPlugin( const String& plugin_name ) = 0;
	
		virtual void unloadPlugin( const String& plugin_name ) = 0;	

		virtual void loadRightMenuPlugin(const String& plugin_name, CMenu& RightMenu) = 0;
	
	};
	

	typedef std::vector<PluginLoader*> PluginLoaderPointerVec;
	
}// end of namespace vgKernel
	


#endif // end of __VGKPLUGINLOADER_H__