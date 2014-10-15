


#ifndef __VGKABSTRACTTREEFAC_H__
#define __VGKABSTRACTTREEFAC_H__

#include <vgKernel/vgkForward.h>

	
namespace vgKernel {

	class AbstractTree;
	/**
		@date 	2009/06/03  10:48	
		@author  xy
	
		@brief 	
	
		@see    
	*/
	class VGK_EXPORT AbstractTreeFac
	{
	public:
		AbstractTreeFac( bool autoreg = true );
		~AbstractTreeFac()
		{
			
		}
	
	public:

		virtual AbstractTree* create() = 0;

	
	
	};
	
	
}// end of namespace vgKernel
	


#endif // end of __VGKABSTRACTTREEFAC_H__