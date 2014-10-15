


#ifndef __VGCOCONFIGUTILITY_H__
#define __VGCOCONFIGUTILITY_H__

#include <vgKernel/vgkForward.h>

	
namespace vgConf {

	/**
		@date 	2008/12/04  16:34	
		@author  xy
	
		@brief 	
	
		@see    
	*/
	class VGK_EXPORT ConfigUtility
	{
	private:
		ConfigUtility()
		{
			
		}
		~ConfigUtility()
		{
			
		}
	
	public:
		
		template< typename Target >
		static Target stringToTarget(const String &arg)
		{
			Target result;

			try
			{
				std::stringstream sstream;
				sstream << arg ;

				sstream >> result;
			}
			catch (...)
			{
				assert(0);
			}

			return result;
		}


		template< typename Target >
		static String targetToString(const Target &arg)
		{
			std::ostringstream o;

			o << arg;

			return o.str();
		}


	};

	
	
}// end of namespace vgConf
	


#endif // end of __VGCOCONFIGUTILITY_H__