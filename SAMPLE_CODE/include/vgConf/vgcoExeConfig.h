


#ifndef __VGCOEXECONFIG_H__
#define __VGCOEXECONFIG_H__

#include <vgKernel/vgkForward.h>
#include <vgKernel/vgkSingleton.h>
#include <vgConf/vgcoConfigInterface.h>
#include <vgKernel/vgkSystemUtility.h>
	
namespace vgConf {




	/**
		@date 	2008/12/04  9:11	
		@author  xy
	
		@brief 	

			�������е�������.

		ʹ�÷���:

		1.ʹ��ExeConfigRegProxy����ע��Ĭ�ϵ���.
		ע��,Ҫ��cpp��������Щʵ��.�����޹�,ֻҪ��֤û����������.
		����:
			// String����.
			vgConf::ExeConfigRegProxy reg1( "section1" , "key1" , "value1" );

			// float����
			vgConf::ExeConfigRegProxy reg2( "section1" , "key2" , "0.3" );

			// int����
			vgConf::ExeConfigRegProxy reg6( "section1" , "key3" , "123" );

			// bool����.ע��!һ����Ҫдtrue����false,Ҫ��0����1��ʾ.
			vgConf::ExeConfigRegProxy reg652( "section2" , "key4" , "1" );


		2.Ȼ��ʼ�õ��������.
		�õ����õ�ֵʱ,����ʹ�ú�,����Ҫע��,����С��ʹ��,���ת���д���,���ܱ���
		(����ı���ָ�������Ͳ�ƥ��.����һ��"ffg"ǿ��ת��Ϊbool����ܱ���):

			String str = VG_EXECONF_GET_STRING( "section1" , "key2" );
			float f = VG_EXECONF_GET_FLOAT( "section1" , "key2" );
			int i = VG_EXECONF_GET_INT( "section1" , "key3"  );
			bool b = VG_EXECONF_GET_BOOL(  "section2" , "key4"  );

		Ҫϸ�¿���,ʹ�����������:

			// ���ȵõ��ַ���
			String str = vgConf::ExeConfig::getSingleton().getConfig( "section2" , "key3" );
			// ���ַ���ת������Ӧ��ֵ.
			bool b = vgConf::ConfigUtility::stringToTarget<bool>(  str );


		@see    
	*/
	class VGK_EXPORT ExeConfig : public vgKernel::Singleton<ExeConfig>,
		public ConfigInterface
	{
	private:

		friend class vgKernel::Singleton<ExeConfig>;
	private:
		ExeConfig();

	public:
		virtual~ExeConfig();

	protected:

		virtual bool initialise();
		virtual bool shutdown();

	public:

		

		virtual void reset(){};

		virtual String getConfigName()
		{
			return "ExeConfig";
		}

		String getXmlFilePath() const
		{
			return _xmlFilePath;
		}

		String invalidate( bool bForceReLoad = false );

		bool saveXmlFile()
		{
			return shutdown();
		}

	protected:

		void init();

		virtual String getValueFromProfile( const String& section , 
			const String& key );

	private:


		String _xmlFilePath;

		bool _inited;

	};
	
	
	
}// end of namespace vgConf
	
#define VG_EXECONF_GET_STRING( section , key )	(  vgConf::ExeConfig::getSingleton().getConfig( section , key ) );

#define VG_EXECONF_GET_FLOAT( section , key )	( vgConf::ConfigUtility::stringToTarget<float>\
	( vgConf::ExeConfig::getSingleton().getConfig( section , key ) ) );

#define VG_EXECONF_GET_INT( section , key )	( vgConf::ConfigUtility::stringToTarget<int>\
	( vgConf::ExeConfig::getSingleton().getConfig( section , key ) ) );

#define VG_EXECONF_GET_BOOL( section , key )	( vgConf::ConfigUtility::stringToTarget<bool>\
	( vgConf::ExeConfig::getSingleton().getConfig( section , key ) ) );




#define VG_EXECONF_SET( section , key ,val )	 \
	( vgConf::ExeConfig::getSingleton().setConfig( section , key , \
	vgConf::ConfigUtility::targetToString( val ) ) );

#define VG_EXECONF_INVALIDATE( bForce )		(  vgConf::ExeConfig::getSingleton().invalidate( bForce ) );

#define VG_EXECONF_SAVE()	 \
	( vgConf::ExeConfig::getSingleton().saveXmlFile() );


#endif // end of __VGCOEXECONFIG_H__