


#ifndef __VGKPLANE_H__
#define __VGKPLANE_H__

#include <vgKernel/vgkForward.h>
#include <vgKernel/vgkBox.h>


	
namespace vgKernel {


	/**
		@date 	2007/02/20  10:50	
		@author  xy
	
		@brief 	

			平面类.通过方程Ax + By + Cz + D = 0 表征一个平面.其中,
			A , B , C 的物理意义是平面上的法线向量.而D值表示平面和原点的距离.
		
			A plane is defined in 3D space by the equation
			Ax + By + Cz + D = 0
	

		@see    
	*/
	class  VGK_EXPORT Plane
	{
	public:

		Plane ();
		Plane (const Plane& rhs);

		///构造函数,参数为normal和以normal为方向移动的距离,注意正负.
		Plane (const Vec3& rkNormal, float fConstant);
		Plane (const Vec3& rkNormal, const Vec3& rkPoint);
		Plane (const Vec3& rkPoint0, const Vec3& rkPoint1,
			const Vec3& rkPoint2);

		enum Side
		{
			NO_SIDE,		///表示在平面上,另一个当AABB为NULL时表示不在任何一边上.
			POSITIVE_SIDE,	///表示在正面
			NEGATIVE_SIDE,	///表示在负面
			BOTH_SIDE		///表示在正负2面,用于AABB检测.
		};

	public:	

		///平面的法向量.
		Vec3 _normal;
		///平面和原点的距离,联合normal可以确定唯一个Plane
		float _distance;

	public:

		/// 得到一个点相对于平面的位置.返回为Side
		Side getSide (const Vec3& rkPoint) const;

		/// 得到一个包围盒相对于平面的位置.返回为Side
		Side getSide (const Box& rkBox) const;

		/** Returns which side of the plane that the given box lies on.
		The box is defined as centre/half-size pairs for effectively.
		@param centre The centre of the box.
		@param halfSize The half-size of the box.
		@returns
		POSITIVE_SIDE if the box complete lies on the "positive side" of the plane,
		NEGATIVE_SIDE if the box complete lies on the "negative side" of the plane,
		and BOTH_SIDE if the box intersects the plane.
		*/
		Side getSide (const Vec3& centre, const Vec3& halfSize) const;

		///返回伪距离,此值可正可负.负值表示在negative side.
		float getDistance (const Vec3& rkPoint) const;

		///通过3个点重定义.
		/** Redefine this plane based on 3 points. */
		void redefine(const Vec3& rkPoint0, const Vec3& rkPoint1,
			const Vec3& rkPoint2);

		///通过normal和一个点重定义
		/** Redefine this plane based on a _normal and a point. */
		void redefine(const Vec3& rkNormal, const Vec3& rkPoint);

		///向量在plane上的投影.
		/** Project a vector onto the plane. 
		@remarks This gives you the element of the input vector that is perpendicular 
		to the _normal of the plane. You can get the element which is parallel
		to the _normal of the plane by subtracting the result of this method
		from the original vector, since parallel + perpendicular = original.
		@param v The input vector
		*/
		//Vec3 projectVector(const Vec3& v);

		float normalise(void);

		bool isFrontFacingTo(const Vec3& direction) const {
			double dot = _normal.dotProduct(direction);

			return (dot <= 0);
		}

		/// Comparison operator
		bool operator == (const Plane& rhs) const
		{
			return (rhs._distance == _distance && rhs._normal == _normal);
		}
		bool operator != (const Plane& rhs) const
		{
			return (rhs._distance != _distance && rhs._normal != _normal);
		}

		String getDetails()
		{
			std::ostringstream o;
			o << " Plane:( _normal: " <<  _normal.getDetails() << " , distance: " << _distance << " )";
			return o.str();
		}
	};
	
}// end of namespace vgKernel
	


#endif // end of __VGKPLANE_H__