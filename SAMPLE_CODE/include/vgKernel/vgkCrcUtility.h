


#ifndef __VGKCRCUTILITY_H__
#define __VGKCRCUTILITY_H__

#include <vgKernel/vgkForward.h>
#include "Crc32Static.h"

	
namespace vgKernel {

	/**
	@date 	2008/07/19  13:46	
	@author  xy

	@brief 	

	@see    
	*/
	class  VGK_EXPORT CrcUtility
	{
	private:
		CrcUtility()
		{

		}
	public:

		~CrcUtility()
		{

		}

	public:

		static inline void initCrc32Code( DWORD& crc_code )
		{
			crc_code = 0xFFFFFFFF;
		}

		static inline void calculateCrc32Code( const char& one_byte ,
			DWORD& crc_code )
		{
			CCrc32Static::CalcCrc32( one_byte , crc_code );
		}

		static inline void calculateCrc32Code( const char* ptr ,
			DWORD& crc_code )
		{
			CCrc32Static::CalcCrc32( *ptr , crc_code );
		}

		static bool getFileCrc32Code( const String& absolute_file_path , 
			DWORD& crc_code );


	private:


	};
	
	
}// end of namespace vgKernel
	


#endif // end of __VGKCRCUTILITY_H__