




#ifndef __VGCOEXECONFIGREGPROXY_H__
#define __VGCOEXECONFIGREGPROXY_H__


	




#include <vgKernel/vgkForward.h>

	
namespace vgConf {

	/**
		@date 	2008/12/04  14:57	
		@author  xy
	
		@brief 	�����ע�������.
	
		@see    
	*/
	class VGK_EXPORT ExeConfigRegProxy
	{
	public:
		ExeConfigRegProxy( String section  , String key , String defaultvalue );
		virtual ~ExeConfigRegProxy()
		{
			
		}

	public:
	
	};


	
}// end of namespace vgConf
	


#endif // end of __VGCOEXECONFIGREGPROXY_H__