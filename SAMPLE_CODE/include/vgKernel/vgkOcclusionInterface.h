



#ifndef __VGKOCCLUSIONINTERFACE_H__
#define __VGKOCCLUSIONINTERFACE_H__

#include <vgKernel/vgkForward.h>
#include <vgOcc/vgoOcclusionState.h>
	
namespace vgKernel {

	/**
		@date 	2009/01/05  10:26	
		@author  xy
	
		@brief 	
	
		@see    
	*/
	class OcclusionInterface
	{
	public:
		OcclusionInterface()
		{
			
		}
		virtual ~OcclusionInterface()
		{
			
		}
	
	public:

		vgOcclusion::OcclusionState& getOcclusionState()	
		{	
			return	m_occState;	 
		}

	private:
	
		vgOcclusion::OcclusionState m_occState;

	};
	
	
}// end of namespace vgKernel
	


#endif // end of __VGKOCCLUSIONINTERFACE_H__