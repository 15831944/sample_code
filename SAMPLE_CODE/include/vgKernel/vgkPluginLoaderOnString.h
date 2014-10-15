




#ifndef __VGKPLUGINLOADERONSTRING_H__
#define __VGKPLUGINLOADERONSTRING_H__

#include <vgKernel/vgkForward.h>

	
namespace vgKernel {

	/**
		@date 	2009/09/03  21:25	
		@author  xy
	
		@brief 	
	
		@see    
	*/
	class VGK_EXPORT PluginLoaderOnString
	{
	public:
		PluginLoaderOnString()
		{
			
		}
		~PluginLoaderOnString()
		{
			
		}
	

	public:

		static String load( bool bForceReLoad = false);

		static String& getAppTitle( );
		
		static bool getEnablePack( );

		static String load( String& dllNames,
			String dllPath);

	protected:
		// ���dll��exe�Ƿ�汾����(���磺ͬΪRelease�����)
		static bool	testVersionMap( String dllName );

		static bool testExist(const String& filepath_abs_or_not );

		static bool	testExtension( String dllName );

	protected:
	//public:
		static String _apptitle;
		static bool _enablePack;

		static String _dllNamesStd;
		static String _dllPathStd;
		
		static String _dllNamesPro;
		static String _dllPathPro;

		static String _dllNamesStatic;

	};
	
	
}// end of namespace vgKernel
	


#endif // end of __VGKPLUGINLOADERONSTRING_H__
