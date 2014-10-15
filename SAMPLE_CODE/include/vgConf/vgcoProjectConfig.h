


#ifndef __VGCOPROJECTCONFIG_H__
#define __VGCOPROJECTCONFIG_H__

#include <vgKernel/vgkForward.h>
#include <vgKernel/vgkSingleton.h>
#include <vgConf/vgcoConfigInterface.h>

	
namespace vgConf {

	/**
		@date 	2008/12/04  9:12	
		@author  xy
	
		@brief 	
	
		@see    
	*/
	class VGK_EXPORT ProjectConfig : public vgKernel::Singleton<ProjectConfig>,
		public ConfigInterface
	{
		friend class vgKernel::Singleton<ProjectConfig>;
	private:
		ProjectConfig();
	public:

		virtual~ProjectConfig();

	protected:
		virtual bool initialise();
		virtual bool shutdown()
		{
			return true;
		}

	public:

		virtual void reset(){};

		virtual String getConfigName()
		{
			return "ProjectConfig";
		}

		bool setProfilePath( const String& proj_abs_path );

		/**
			����xml�б�.
		*/
		bool saveProfileToPath( const String& save_path );

	};
	
	
}// end of namespace vgConf
	

#define VG_PROJCONF_GET_STRING( section , key )	(  vgConf::ProjectConfig::getSingleton().getConfig( section , key ) )

#define VG_PROJCONF_GET_FLOAT( section , key )	( vgConf::ConfigUtility::stringToTarget<float>\
	( vgConf::ProjectConfig::getSingleton().getConfig( section , key ) ) )

#define VG_PROJCONF_GET_INT( section , key )	( vgConf::ConfigUtility::stringToTarget<int>\
	( vgConf::ProjectConfig::getSingleton().getConfig( section , key ) ) )

#define VG_PROJCONF_GET_BOOL( section , key )	( vgConf::ConfigUtility::stringToTarget<bool>\
	( vgConf::ProjectConfig::getSingleton().getConfig( section , key ) ) )




#define VG_PROJCONF_SET( section , key ,val )	 \
	( vgConf::ProjectConfig::getSingleton().setConfig( section , key , \
	vgConf::ConfigUtility::targetToString( val ) ) )



#endif // end of __VGCOPROJECTCONFIG_H__