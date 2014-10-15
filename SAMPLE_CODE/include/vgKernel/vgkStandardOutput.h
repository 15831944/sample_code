


#ifndef __VGKSTANDARDOUTPUT_H__
#define __VGKSTANDARDOUTPUT_H__

#include <vgKernel/vgkForward.h>


	
namespace vgKernel {


	typedef void (*StandardOutputFunc)( const char* str );

	/**
		@date 	2009/05/20  9:32	
		@author  xy
	
		@brief 	
	
		@see    
	*/
	class VGK_EXPORT StandardOutput
	{
	private:
		StandardOutput()
		{
			
		}
	public:
		~StandardOutput()
		{
			
		}
	

	public:
	
		static void redirectOutput( StandardOutputFunc func );
		
		static StandardOutputFunc getOutputFunc();

		static void dumpToOutput( const String& str );

		static void dumpToOutputW( const std::wstring& wstr );
	
		static void restoreToDefaultOutput();

		template <class T>
		static void createOutput( const T& info )
		{
			std::ostringstream o; 
			o << info;

			dumpToOutput( info.str() );
		}

	private:

		static void DefaultOutput( const char* str );

	private:
	
		static StandardOutputFunc _stdout;
	
	};
	
	
}// end of namespace vgKernel


#define VGK_OUTPUT(info)  vgKernel::StandardOutput::dumpToOutput( info )

#define VGK_OUTPUT_T(info)  vgKernel::StandardOutput::createOutput( info )


#endif // end of __VGKSTANDARDOUTPUT_H__