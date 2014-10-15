/*
-----------------------------------------------------------------------------
  VRES(Virtual Reality GIS Engine System)    
  -------------------------------------------------------------
  License details ... waiting to add...
  -------------------------------------------------------------
  Copyright  (C) 2007 , leven - All Rights Reserved
-----------------------------------------------------------------------------
*/

#ifndef __VGFPLANE_H__
#define __VGFPLANE_H__

#include <vgMath/vgfPrerequisites.h>


namespace vgMath {

	/**
		@date 	2007/02/20  10:50	
		@author  leven
	
		@brief 	

			ƽ����.ͨ������Ax + By + Cz + D = 0 ����һ��ƽ��.����,
			A , B , C ������������ƽ���ϵķ�������.��Dֵ��ʾƽ���ԭ��ľ���.
		
			A plane is defined in 3D space by the equation
			Ax + By + Cz + D = 0
	
			This equates to a vector (the normal of the plane, whose x, y
		and z components equate to the coefficients A, B and C
		respectively), and a constant (D) which is the distance along
		the normal you have to go to move the plane back to the origin.

		@see    
	*/
	class VgfExport Plane
	{
	public:

		Plane ();
		Plane (const Plane& rhs);

		///���캯��,����Ϊnormal����normalΪ�����ƶ��ľ���,ע������.
		/** Construct a plane through a normal, and a distance to move the plane along the normal.*/
		Plane (const Vector3& rkNormal, Real fConstant);
		Plane (const Vector3& rkNormal, const Vector3& rkPoint);
		Plane (const Vector3& rkPoint0, const Vector3& rkPoint1,
			const Vector3& rkPoint2);

		/** The "positive side" of the plane is the half space to which the
		plane normal points. The "negative side" is the other half
		space. The flag "no side" indicates the plane itself.
		*/
		enum Side
		{
			NO_SIDE,		///��ʾ��ƽ����,��һ����AABBΪNULLʱ��ʾ�����κ�һ����.
			POSITIVE_SIDE,	///��ʾ������
			NEGATIVE_SIDE,	///��ʾ�ڸ���
			BOTH_SIDE		///��ʾ������2��,����AABB���.
		};

	public:	

		///ƽ��ķ�����.
		Vector3 normal;
		///ƽ���ԭ��ľ���,����normal����ȷ��Ψһ��Plane
		Real d;

	public:

		/// �õ�һ���������ƽ���λ��.����ΪSide
		Side getSide (const Vector3& rkPoint) const;

		/// �õ�һ����Χ�������ƽ���λ��.����ΪSide
		Side getSide (const AxisAlignedBox& rkBox) const;

		/** Returns which side of the plane that the given box lies on.
		The box is defined as centre/half-size pairs for effectively.
		@param centre The centre of the box.
		@param halfSize The half-size of the box.
		@returns
		POSITIVE_SIDE if the box complete lies on the "positive side" of the plane,
		NEGATIVE_SIDE if the box complete lies on the "negative side" of the plane,
		and BOTH_SIDE if the box intersects the plane.
		*/
		Side getSide (const Vector3& centre, const Vector3& halfSize) const;

		///����α����,��ֵ�����ɸ�.��ֵ��ʾ��negative side.
		Real getDistance (const Vector3& rkPoint) const;

		///ͨ��3�����ض���.
		/** Redefine this plane based on 3 points. */
		void redefine(const Vector3& rkPoint0, const Vector3& rkPoint1,
			const Vector3& rkPoint2);

		///ͨ��normal��һ�����ض���
		/** Redefine this plane based on a normal and a point. */
		void redefine(const Vector3& rkNormal, const Vector3& rkPoint);

		///������plane�ϵ�ͶӰ.
		/** Project a vector onto the plane. 
		@remarks This gives you the element of the input vector that is perpendicular 
		to the normal of the plane. You can get the element which is parallel
		to the normal of the plane by subtracting the result of this method
		from the original vector, since parallel + perpendicular = original.
		@param v The input vector
		*/
		Vector3 projectVector(const Vector3& v);

		/// ��λ��ƽ��.
		/** Normalises the plane.
		@remarks
		This method normalises the plane's normal and the length scale of d
		is as well.
		@note
		This function will not crash for zero-sized vectors, but there
		will be no changes made to their components.
		@returns The previous length of the plane's normal.
		*/
		Real normalise(void);



		/// Comparison operator
		bool operator == (const Plane& rhs) const
		{
			return (rhs.d == d && rhs.normal == normal);
		}
		bool operator != (const Plane& rhs) const
		{
			return (rhs.d != d && rhs.normal != normal);
		}

		inline VgfExport friend StdOStream& operator<< (StdOStream& o, const Plane& p)
		{
			o << _T("Plane(normal=") << p.normal << _T(", d=") << p.d << _T(")");
			return o;
		}

		inline String getDetails() const
		{
			StdOStringStream o;
			o << (*this);
			return o.str();
		}
	};

	typedef std::vector<Plane> PlaneList;

	//inline StdOStream& operator<< (StdOStream& o, const Plane& p)
	//{

	//}
	
}

#endif // end of __VGFPLANE_H__