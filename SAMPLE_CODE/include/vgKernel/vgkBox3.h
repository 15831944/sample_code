


#ifndef __VGKBOX3_H__
#define __VGKBOX3_H__

#include <vgKernel/vgkForward.h>
#include <vgKernel/vgkVec3.h>

	
namespace vgKernel {

	/**
		@date 	2008/11/13  20:23	
		@author  xy
	
		@brief 	��AxisAlignedBox.
	
		@see    
	*/
	class VGK_EXPORT  Box3
	{
	public:
		// A box has center C, axis directions U[0], U[1], and U[2] (all
		// unit-length vectors), and extents e[0], e[1], and e[2] (all nonnegative
		// numbers).  A point X = C+y[0]*U[0]+y[1]*U[1]+y[2]*U[2] is inside or
		// on the box whenever |y[i]| <= e[i] for all i.

		// construction
		Box3 ();  // uninitialized
		Box3 (const Vec3& rkCenter, const Vec3* akAxis,
			const float* afExtent);
		Box3 (const Vec3& rkCenter, const Vec3& rkAxis0,
			const Vec3& rkAxis1, const Vec3& rkAxis2,
			float fExtent0, float fExtent1, float fExtent2);

		void ComputeVertices (Vec3 akVertex[8]) const;

		Vec3 Center;
		Vec3 Axis[3];  // must be an orthonormal set of vectors
		float Extent[3];         // must be nonnegative
	};
	
	
}// end of namespace vgKernel
	


#endif // end of __VGKBOX3_H__